#ifndef UI_CPP
#define UI_CPP

#include "ui.h"
#include "definitions.h"

#include <ncursesw/curses.h>
#include <csignal>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Ncurses leaves the terminal in raw mode if the process dies without
//  calling endwin(). Restore it on Ctrl+C so a killed session doesn't
//  leave the shell looking broken.
static void handle_interrupt_signal(int)
{
  endwin();
  exit(0);
}

static int digit_count(size_t Value)
{
  int count = 1;
  while (Value >= 10)
  {
    Value /= 10;
    ++count;
  }
  return count;
}

void TERMINAL_UI::init()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(1);
  timeout(INPUT_POLL_INTERVAL_MS);

  signal(SIGINT, handle_interrupt_signal);

  getmaxyx(stdscr, rows, cols);
}

void TERMINAL_UI::shutdown()
{
  endwin();
}

int TERMINAL_UI::content_height() const
{
  // One row for the status bar, one for the command line.
  return max(0, rows - 2);
}

UI_INPUT_RESULT TERMINAL_UI::poll_input()
{
  UI_INPUT_RESULT result;

  int ch = getch();

  switch (ch)
  {
    case ERR:
      // Nothing arrived within the poll interval.
      break;

    case KEY_RESIZE:
      // Standard ncurses idiom to make LINES/COLS reflect the new size.
      endwin();
      refresh();
      getmaxyx(stdscr, rows, cols);
      result.action = UI_ACTION::RESIZE;
      break;

    case KEY_UP:
      result.action = UI_ACTION::SCROLL_UP;
      break;

    case KEY_DOWN:
      result.action = UI_ACTION::SCROLL_DOWN;
      break;

    case KEY_PPAGE:
      result.action = UI_ACTION::PAGE_UP;
      break;

    case KEY_NPAGE:
      result.action = UI_ACTION::PAGE_DOWN;
      break;

    case KEY_HOME:
      result.action = UI_ACTION::GOTO_TOP;
      break;

    case KEY_END:
      result.action = UI_ACTION::GOTO_BOTTOM;
      break;

    case KEY_BACKSPACE:
    case 127:
    case 8:
      if (!command_buffer.empty())
      {
        command_buffer.pop_back();
      }
      break;

    case '\n':
    case '\r':
    case KEY_ENTER:
      if (command_buffer == "q" || command_buffer == "quit")
      {
        result.action = UI_ACTION::QUIT;
      }
      else if (!command_buffer.empty() &&
               command_buffer.find_first_not_of("0123456789") == string::npos)
      {
        result.action = UI_ACTION::GOTO_LINE;
        result.goto_line = atoi(command_buffer.c_str());
      }
      command_buffer.clear();
      break;

    default:
      if (ch >= 32 && ch <= 126 && (int)command_buffer.size() < cols - 4)
      {
        command_buffer += (char)ch;
      }
      break;
  }

  return result;
}

void TERMINAL_UI::draw(const vector<string> &Lines, const string &Filename,
                        const string &Last_Change_Time, bool Following, int Top_Line)
{
  erase();

  // ---- Status bar ----
  string status = " " + Filename + "    changed: " + Last_Change_Time + "    ";

  if (Following)
  {
    status += "[FOLLOWING]";
  }
  else if (!Lines.empty())
  {
    status += "line " + to_string(Top_Line + 1) + "/" + to_string(Lines.size());
  }
  else
  {
    status += "line 0/0";
  }

  attron(A_REVERSE);
  mvhline(0, 0, ' ', cols);
  mvaddnstr(0, 0, status.c_str(), cols);
  attroff(A_REVERSE);

  // ---- Content area, with a line-number gutter ----
  int content_h = content_height();
  int gutter_w = max(3, digit_count(Lines.size()));

  for (int i = 0; i < content_h; ++i)
  {
    int line_index = Top_Line + i;
    int screen_row = 1 + i;

    if (line_index >= 0 && line_index < (int)Lines.size())
    {
      string num_str = to_string(line_index + 1);
      if ((int)num_str.size() < gutter_w)
      {
        num_str = string((size_t)gutter_w - num_str.size(), ' ') + num_str;
      }

      attron(A_DIM);
      mvaddnstr(screen_row, 0, num_str.c_str(), gutter_w);
      attroff(A_DIM);

      mvaddch(screen_row, gutter_w + 1, '|');

      int text_x = gutter_w + 3;
      int text_w = cols - text_x;

      if (text_w > 0)
      {
        mvaddnstr(screen_row, text_x, Lines[(size_t)line_index].c_str(), text_w);
      }
    }
    else
    {
      mvaddch(screen_row, 0, '~');
    }
  }

  // ---- Command line ----
  int cmd_row = rows - 1;
  string cmd_display = "> " + command_buffer;
  mvaddnstr(cmd_row, 0, cmd_display.c_str(), cols);
  move(cmd_row, min(cols - 1, (int)cmd_display.size()));

  refresh();
}

#endif // UI_CPP
