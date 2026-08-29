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

  // Blocks up to INPUT_POLL_INTERVAL_MS waiting for a keypress and
  //  interprets it. Returns UI_ACTION::NONE if nothing arrived in time.
  UI_INPUT_RESULT poll_input();

  // Redraws the whole screen from scratch.
  void draw(const std::vector<std::string> &Lines, const std::string &Filename,
            const std::string &Last_Change_Time, bool Following, int Top_Line);
};

#endif
