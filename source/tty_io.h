#ifndef TTY_IO_H
#define TTY_IO_H

#include <iostream>
#include <string>
#include <vector>


// ------------------------------------------------------------------------- //
// This is a bit of a future proof that is completely untested.
//  Keeping this around just as a reminder that it is possible.
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "definitions.h"
#include "stringthings.h"

using namespace std;
// ------------------------------------------------------------------------- //

// Simple IO interfaces for getting input and printing things to the screen 
//  asynchonously.

// ------------------------------------------------------------------------- //

int kbhit();
// Returns 0 if no key is pressed.

void print_XY(int X, int Y); // Print to a specific XY coordinate on the screen.
void print_XYZ(int X, int Y, int Z); // Print to a specific XYZ coordinate on the screen.
void move_cursor(int X, int Y); // Move cursor to position (x,y) on the screen
                                // 0,0 is the top left corner of the screen.

void clear_screen();  // Clears the screen.
void clear_line();    // Clears the current line.
void clear_lines(int Number_Of_Lines); // Clears a number of lines starting at the current line.
void move_up_a_line(); // Moves the cursor up one line.
void get_console_size(int &Rows, int &Cols); // Gets the size of the console window.
string reverse(string Text); // Reverse the order of characters in a string.

void kb_pause(string Message = "DO NOT PRINT");
// pause program till space is pressed
// does not work in extra thread.

// ------------------------------------------------------------------------- //

// Vecor list for control of focus.
class TTY_OUTPUT_FOCUS
{
  private:

  int ID_WITH_FOCUS = -1;
  bool FOCUS_CHANGED = true;;

  public:

  void set_focus(int focus);
  bool focus_changed(int focus);
  void unchanged();
};

class TTY_OUTPUT_PROPERTIES
{
  public:

  string TITLE = "";

  int POSITION_X = 0;
  int POSITION_Y = 0;
  int LINES = -1;

  bool RECORD_HISTORY = false;
};

class TTY_OUTPUT
{
  private:
  
  // Cursor position.

  string OUTPUT_STRING_FOCUS_CHANGES;
  string OUTPUT_STRING = "";

  bool ENTER_RECEIVED = false;
  bool NEEDS_REDRAW = false;

  int FOCUS_ID = -1; // -1 means focus ID is not set.

  bool DRAW_TITLE = false;

  bool check_char_recieived(int Character);

  void clean_for_print();

  void record_to_history(string Text);

  public:

  TTY_OUTPUT_PROPERTIES PROPS;
  vector<string>        HISTORY;

  bool CHANGED = false;

  void create(int Focus_ID);

  void redraw();

  void clear();

  bool add_to(int Character, TTY_OUTPUT_FOCUS &Output_Focus, bool Record_If_Allowed = true);
  void add_to(string Text, TTY_OUTPUT_FOCUS &Output_Focus, bool Record_If_Allowed = true);

  void seperater(TTY_OUTPUT_FOCUS &Output_Focus);

  bool pressed_enter();
  // READ ONCE
  // Clears after being read.

  string value();

  void output(TTY_OUTPUT_FOCUS &Output_Focus);
};

// ------------------------------------------------------------------------- //

class TTY_INPUT
{
  private:
  termios STARTING_TERMINAL_STATE;

  int CHAR_RECEIVED_LATEST = ' ';

  bool CHAR_RECEIVED_ENTER = false;
  bool CHAR_RECEIVED_BACSPACE = false;

  //char get_char();
  void set_nonblocking_mode();

  bool check_char_recieived();
  // returns true if the loop should continue on
  // returns false if enter pressed, stop the loop.

  public:

  void restore_terminal_settings();

  void create();

  void read_input(TTY_OUTPUT &Output_Container);
  void read_input(TTY_OUTPUT &Output_Container, TTY_OUTPUT_FOCUS &Output_Focus);

  void clear_screeen();
};

// ------------------------------------------------------------------------- //

#endif // TTY_IO_H