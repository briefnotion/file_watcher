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

// Finds the visual row where a (0-indexed) source line first appears,
//  after wrapping. Clamps out-of-range source lines to the nearest end.
static int visual_row_for_source_line(const vector<VISUAL_ROW> &Visual_Rows, int Source_Line)
{
  if (Visual_Rows.empty())
  {
    return 0;
  }

  if (Source_Line <= Visual_Rows.front().source_line)
  {
    return 0;
  }

  for (size_t i = 0; i < Visual_Rows.size(); ++i)
  {
    if (Visual_Rows[i].source_line == Source_Line && Visual_Rows[i].is_first_segment)
    {
      return (int)i;
    }
  }

  // Past the end of the file: land on the last line's first segment.
  for (size_t i = Visual_Rows.size(); i-- > 0;)
  {
    if (Visual_Rows[i].is_first_segment)
    {
      return (int)i;
    }
  }

  return 0;
}

int main_loop(const string &Filename)
{
  FILE_WATCH watcher;
  watcher.start(Filename);

  TERMINAL_UI ui;
  ui.init();

  vector<string> lines;
  vector<VISUAL_ROW> visual_rows;
  string last_change = "--:--:--";

  int top_row = 0;
  bool following = true;
  bool running = true;
  bool first_check = true;

  auto rebuild_visual_rows = [&]()
  {
    visual_rows = wrap_lines(lines, ui.content_text_width(lines.size()));
  };

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
      rebuild_visual_rows();

      if (following)
      {
        top_row = clamp_top_line((int)visual_rows.size(), (int)visual_rows.size(), ui.content_height());
      }
      else
      {
        top_row = clamp_top_line(top_row, (int)visual_rows.size(), ui.content_height());
      }
    }

    ui.draw(visual_rows, lines.size(), Filename, last_change, following, top_row);

    UI_INPUT_RESULT result = ui.poll_input();

    switch (result.action)
    {
      case UI_ACTION::QUIT:
        running = false;
        break;

      case UI_ACTION::RESIZE:
        // The content width may have changed, so wrapping needs redoing.
        rebuild_visual_rows();
        if (following)
        {
          top_row = clamp_top_line((int)visual_rows.size(), (int)visual_rows.size(), ui.content_height());
        }
        else
        {
          top_row = clamp_top_line(top_row, (int)visual_rows.size(), ui.content_height());
        }
        break;

      case UI_ACTION::SCROLL_UP:
        following = false;
        top_row = clamp_top_line(top_row - 1, (int)visual_rows.size(), ui.content_height());
        break;

      case UI_ACTION::SCROLL_DOWN:
        top_row = clamp_top_line(top_row + 1, (int)visual_rows.size(), ui.content_height());
        following = (top_row >= max(0, (int)visual_rows.size() - ui.content_height()));
        break;

      case UI_ACTION::PAGE_UP:
        following = false;
        top_row = clamp_top_line(top_row - ui.content_height(), (int)visual_rows.size(), ui.content_height());
        break;

      case UI_ACTION::PAGE_DOWN:
        top_row = clamp_top_line(top_row + ui.content_height(), (int)visual_rows.size(), ui.content_height());
        following = (top_row >= max(0, (int)visual_rows.size() - ui.content_height()));
        break;

      case UI_ACTION::GOTO_TOP:
        following = false;
        top_row = 0;
        break;

      case UI_ACTION::GOTO_BOTTOM:
        following = true;
        top_row = clamp_top_line((int)visual_rows.size(), (int)visual_rows.size(), ui.content_height());
        break;

      case UI_ACTION::GOTO_LINE:
        following = false;
        top_row = clamp_top_line(visual_row_for_source_line(visual_rows, result.goto_line - 1),
                                  (int)visual_rows.size(), ui.content_height());
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
