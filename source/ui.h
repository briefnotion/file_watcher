#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

// What the user asked the UI to do, decoded from a keypress.
enum class UI_ACTION
{
  NONE,
  QUIT,
  SCROLL_UP,
  SCROLL_DOWN,
  PAGE_UP,
  PAGE_DOWN,
  GOTO_TOP,
  GOTO_BOTTOM,
  GOTO_LINE,
  RESIZE,
};

struct UI_INPUT_RESULT
{
  UI_ACTION action = UI_ACTION::NONE;
  int goto_line = 0; // Only valid when action == GOTO_LINE. 1-indexed.
};

// One screen row's worth of content. A source line longer than the
//  content width becomes several consecutive VISUAL_ROWs (wrapping),
//  each pointing back at the source line it came from.
struct VISUAL_ROW
{
  int source_line = 0;          // 0-based index into the original file lines.
  bool is_first_segment = true; // False for the wrapped continuation of a line.
  std::string text;
};

// Wraps Lines to Width, splitting any line longer than Width into
//  consecutive VISUAL_ROWs. Width is clamped to at least 1.
std::vector<VISUAL_ROW> wrap_lines(const std::vector<std::string> &Lines, int Width);

// Renders the file view: a reverse-video status bar, a line-numbered,
//  scrollable content area, and a bottom command line for typed commands.
// Resize-aware: recomputes its dimensions on KEY_RESIZE.
class TERMINAL_UI
{
  private:
  int rows = 0;
  int cols = 0;

  std::string command_buffer;

  public:
  void init();
  void shutdown();

  // Number of content rows available between the status bar and the
  //  command line. Use this to know how many lines fit on screen.
  int content_height() const;

  // Width of the line-number gutter for a file with this many lines.
  int gutter_width(size_t Total_Source_Lines) const;

  // Width available for line text once the gutter is accounted for.
  //  Feed this to wrap_lines() so wrapping matches what will be drawn.
  int content_text_width(size_t Total_Source_Lines) const;

  // Blocks up to INPUT_POLL_INTERVAL_MS waiting for a keypress and
  //  interprets it. Returns UI_ACTION::NONE if nothing arrived in time.
  UI_INPUT_RESULT poll_input();

  // Redraws the whole screen from scratch. Top_Row indexes into Visual_Rows.
  void draw(const std::vector<VISUAL_ROW> &Visual_Rows, size_t Total_Source_Lines,
            const std::string &Filename, const std::string &Last_Change_Time,
            bool Following, int Top_Row);
};

#endif
