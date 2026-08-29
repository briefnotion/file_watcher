#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "filewatch.h"
#include "ui.h"
#include "definitions.h"

using namespace std;

// ------------------------------------------------------------------------- //

static int clamp_top_line(int Top_Line, int Total_Lines, int Content_Height)
{
  int max_top = max(0, Total_Lines - Content_Height);

  if (Top_Line < 0)
  {
    Top_Line = 0;
  }
  if (Top_Line > max_top)
  {
    Top_Line = max_top;
  }

  return Top_Line;
}

// ------------------------------------------------------------------------- //

int main_loop(const string &Filename)
{
  FILE_WATCH watcher;
  watcher.start(Filename);

  TERMINAL_UI ui;
  ui.init();

  vector<string> lines;
  string last_change = "--:--:--";

  int top_line = 0;
  bool following = true;
  bool running = true;
  bool first_check = true;

  while (running)
  {
    if (watcher.changed() || first_check)
    {
      first_check = false;

      if (watcher.exists())
      {
        lines = watcher.load_lines();
      }
      else
      {
        lines.clear();
        lines.push_back("(file not found: " + Filename + ")");
      }

      last_change = watcher.last_change_time_string();

      if (following)
      {
        top_line = clamp_top_line((int)lines.size(), (int)lines.size(), ui.content_height());
      }
    }

    ui.draw(lines, Filename, last_change, following, top_line);

    UI_INPUT_RESULT result = ui.poll_input();

    switch (result.action)
    {
      case UI_ACTION::QUIT:
        running = false;
        break;

      case UI_ACTION::RESIZE:
        top_line = clamp_top_line(top_line, (int)lines.size(), ui.content_height());
        break;

      case UI_ACTION::SCROLL_UP:
        following = false;
        top_line = clamp_top_line(top_line - 1, (int)lines.size(), ui.content_height());
        break;

      case UI_ACTION::SCROLL_DOWN:
        top_line = clamp_top_line(top_line + 1, (int)lines.size(), ui.content_height());
        following = (top_line >= max(0, (int)lines.size() - ui.content_height()));
        break;

      case UI_ACTION::PAGE_UP:
        following = false;
        top_line = clamp_top_line(top_line - ui.content_height(), (int)lines.size(), ui.content_height());
        break;

      case UI_ACTION::PAGE_DOWN:
        top_line = clamp_top_line(top_line + ui.content_height(), (int)lines.size(), ui.content_height());
        following = (top_line >= max(0, (int)lines.size() - ui.content_height()));
        break;

      case UI_ACTION::GOTO_TOP:
        following = false;
        top_line = 0;
        break;

      case UI_ACTION::GOTO_BOTTOM:
        following = true;
        top_line = clamp_top_line((int)lines.size(), (int)lines.size(), ui.content_height());
        break;

      case UI_ACTION::GOTO_LINE:
        following = false;
        top_line = clamp_top_line(result.goto_line - 1, (int)lines.size(), ui.content_height());
        break;

      case UI_ACTION::NONE:
      default:
        break;
    }
  }

  ui.shutdown();

  return 0;
}

// ------------------------------------------------------------------------- //

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    cerr << "  While running:" << endl;
    cerr << "    Up/Down/PgUp/PgDn - scroll" << endl;
    cerr << "    Home/End          - jump to top / follow the end of the file" << endl;
    cerr << "    <number> Enter    - go to that line" << endl;
    cerr << "    q Enter           - quit" << endl;
    return 1;
  }

  return main_loop(argv[1]);
}

#endif // MAIN_CPP
