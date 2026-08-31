#ifndef UI_CPP
#define UI_CPP

#include "ui.h"
#include "definitions.h"

#include <ncursesw/curses.h>
#include <csignal>
#include <clocale>
#include <cstdlib>
#include <cwchar>
#include <wchar.h>
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

// Splits Text into chunks whose terminal display width - accounting for
//  multi-byte UTF-8 characters and their actual column width - does not
//  exceed Width. Never cuts in the middle of a multi-byte character.
static vector<string> wrap_text_by_display_width(const string &Text, size_t Width)
{
  vector<string> segments;

  if (Text.empty())
  {
    segments.push_back("");
    return segments;
  }

  mbstate_t state{};

  size_t seg_start = 0;
  size_t seg_width = 0;
  size_t pos = 0;

  while (pos < Text.size())
  {
    wchar_t wc;
    size_t char_bytes = mbrtowc(&wc, Text.c_str() + pos, Text.size() - pos, &state);
    size_t advance;
    int char_width;

    if (char_bytes == (size_t)-1 || char_bytes == (size_t)-2)
    {
      // Invalid or incomplete multi-byte sequence: consume one raw byte
      //  so we still make forward progress instead of getting stuck.
      state = mbstate_t{};
      advance = 1;
      char_width = 1;
    }
    else
    {
      // mbrtowc() returns 0 for an embedded NUL, but it still consumed
      //  that one byte.
      advance = (char_bytes == 0) ? 1 : char_bytes;

      int w = wcwidth(wc);
      char_width = (w < 0) ? 1 : w; // unprintable/control: count as 1, stay safe
    }

    if (seg_width + (size_t)char_width > Width && seg_width > 0)
    {
      segments.push_back(Text.substr(seg_start, pos - seg_start));
      seg_start = pos;
      seg_width = 0;
    }

    seg_width += (size_t)char_width;
    pos += advance;
  }

  segments.push_back(Text.substr(seg_start, pos - seg_start));

  return segments;
}

vector<VISUAL_ROW> wrap_lines(const vector<string> &Lines, int Width)
{
  vector<VISUAL_ROW> rows;

  size_t width = (size_t)max(1, Width);

  for (int i = 0; i < (int)Lines.size(); ++i)
  {
    vector<string> segments = wrap_text_by_display_width(Lines[(size_t)i], width);

    bool first = true;
    for (const string &segment : segments)
    {
      rows.push_back(VISUAL_ROW{i, first, segment});
      first = false;
    }
  }

  return rows;
}

void TERMINAL_UI::init()
{
  setlocale(LC_ALL, ""); // required before initscr() for wide/UTF-8 output

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

int TERMINAL_UI::gutter_width(size_t Total_Source_Lines) const
{
  return max(3, digit_count(Total_Source_Lines));
}

int TERMINAL_UI::content_text_width(size_t Total_Source_Lines) const
{
  // Gutter, then a space and a separator before the text starts.
  return cols - (gutter_width(Total_Source_Lines) + 3);
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

void TERMINAL_UI::draw(const vector<VISUAL_ROW> &Visual_Rows, size_t Total_Source_Lines,
                        const string &Filename, const string &Last_Change_Time,
                        bool Following, int Top_Row)
{
  erase();

  // ---- Status bar ----
  string status = " " + Filename + "    changed: " + Last_Change_Time + "    ";

  if (Following)
  {
    status += "[FOLLOWING]";
  }
  else if (Top_Row >= 0 && Top_Row < (int)Visual_Rows.size())
  {
    status += "line " + to_string(Visual_Rows[(size_t)Top_Row].source_line + 1) +
               "/" + to_string(Total_Source_Lines);
  }
  else
  {
    status += "line 0/" + to_string(Total_Source_Lines);
  }

  // addnstr's length limit counts bytes, not display columns, so a
  //  multibyte status string (e.g. a Unicode filename) needs clipping by
  //  display width ourselves before handing it to an unbounded add call.
  string status_clipped = wrap_text_by_display_width(status, (size_t)max(0, cols))[0];

  attron(A_REVERSE);
  mvhline(0, 0, ' ', cols);
  mvaddstr(0, 0, status_clipped.c_str());
  attroff(A_REVERSE);

  // ---- Content area, with a line-number gutter ----
  int content_h = content_height();
  int gutter_w = gutter_width(Total_Source_Lines);

  for (int i = 0; i < content_h; ++i)
  {
    int row_index = Top_Row + i;
    int screen_row = 1 + i;

    if (row_index >= 0 && row_index < (int)Visual_Rows.size())
    {
      const VISUAL_ROW &row = Visual_Rows[(size_t)row_index];

      if (row.is_first_segment)
      {
        string num_str = to_string(row.source_line + 1);
        if ((int)num_str.size() < gutter_w)
        {
          num_str = string((size_t)gutter_w - num_str.size(), ' ') + num_str;
        }

        attron(A_DIM);
        mvaddnstr(screen_row, 0, num_str.c_str(), gutter_w);
        attroff(A_DIM);
      }
      // Wrapped continuation rows leave the gutter blank.

      mvaddch(screen_row, gutter_w + 1, '|');

      int text_x = gutter_w + 3;

      // row.text was already wrapped to fit content_text_width() display
      //  columns, so it can be printed unbounded rather than re-clamped
      //  by addnstr's byte-count limit (which could otherwise cut a
      //  multibyte character in half).
      if (cols - text_x > 0)
      {
        mvaddstr(screen_row, text_x, row.text.c_str());
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
