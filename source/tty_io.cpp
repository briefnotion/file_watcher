#ifndef TTY_IO_CPP
#define TTY_IO_CPP

#include "tty_io.h"

using namespace std;
// ------------------------------------------------------------------------- //

int kbhit()
{
  termios oldt, newt;
  int ch;
  int oldf;

  if (tcgetattr(STDIN_FILENO, &oldt) != 0) 
  {
    perror("tcgetattr");
    return 0;
  }

  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) 
  {
    perror("tcsetattr");
    return 0;
  }

  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  if (oldf == -1) 
  {
    perror("fcntl");
    return 0;
  }

  if (fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK) == -1) 
  {
    perror("fcntl");
    return 0;
  }

  ch = getchar();

  if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0) 
  {
    perror("tcsetattr");
    return 0;
  }

  if (fcntl(STDIN_FILENO, F_SETFL, oldf) == -1) 
  {
    perror("fcntl");
    return 0;
  }

  if (ch != EOF) 
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

void print_XY(int X, int Y)
{
  move_cursor(0, 0);
  cout << "[" << X << "," << Y << "]" << flush;
}

void print_XYZ(int X, int Y, int Z)
{
  move_cursor(0, 0);
  cout << "[" << X << "," << Y << "," << Z << "]" << flush;
}

void move_cursor(int X, int Y)
{
  cout << "\033[" << Y+1 << ";" << X+1 << "H" << flush;
}

void clear_screen()
{
  cout << "\033[2J\033[H";
}

void clear_line()
{
  cout << "\033[2K" << flush; // Clear the entire line
}

void clear_lines(int Number_Of_Lines)
{
  for ( int i = 1 ; i <= Number_Of_Lines ; ++i )
  {
    cout << "\033[2K\n" << flush; // Clear the entire line
  }
}

void move_up_a_line()
{
  cout << "\033[A" << flush;  // Move cursor up one line
}

void get_console_size(int &Rows, int &Cols)
{
  // THIS ROUTINE NEEDS WORK
  // CIN BRINGS IN INFO TO INPUT LINE AND DOESNT GET CLEARED.

  // Save the current cursor position
  cout << "\033[s";

  // Move the cursor to the bottom-right corner
  cout << "\033[999;999H";

  // Query the cursor position
  cout << "\033[6n";

  // Read the response from the terminal
  char buf[32];
  cin >> buf;

  // Parse the response
  if (sscanf(buf, "\033[%d;%dR", &Rows, &Cols) != 2)
  {
    Rows = Cols = -1; // Error
  }

  // Restore the cursor position
  cout << "\033[u";
}

string reverse(string Text)
{
  return "\033[7;37m" + Text + "\033[0m";
}

void kb_pause(string Message)
{
  char char_received = 'X';

  if (Message != "DO NOT PRINT")
  {
    move_cursor(0, 0);
    clear_lines(10);
    move_cursor(0, 0);
    cout << "PAUSE" << endl << endl;
    cout << Message << endl;
  }

  while (char_received != ' ')
  {
    if (kbhit())
    {
      char_received = getchar();
    }
    else
    {
      sleep(1);
    }
  }
}

// ------------------------------------------------------------------------- //

void TTY_OUTPUT_FOCUS::set_focus(int focus)
{
  if (ID_WITH_FOCUS != focus)
  {
    ID_WITH_FOCUS = focus;
    FOCUS_CHANGED = true;
  }
}

bool TTY_OUTPUT_FOCUS::focus_changed(int focus)
{
  if (ID_WITH_FOCUS != focus)
  {
    ID_WITH_FOCUS = focus;
    FOCUS_CHANGED = true;
    return true;
  }
  else if (FOCUS_CHANGED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void TTY_OUTPUT_FOCUS::unchanged()
{
  FOCUS_CHANGED = false;
}

// ------------------------------------------------------------------------- //

bool TTY_OUTPUT::check_char_recieived(int Character)
{
  CHANGED = true;

  if (Character == 10)
  {
    // Enter Pressed.
    ENTER_RECEIVED = true;
    return false;
  }
  else if (Character == 127)
  {
    // Focus
    OUTPUT_STRING_FOCUS_CHANGES += (char)Character;

    NEEDS_REDRAW = true;

    // Backspace Pressed.
    if (OUTPUT_STRING.size() > 0)
    {
      OUTPUT_STRING = OUTPUT_STRING.substr(0, OUTPUT_STRING.length()-1);
    }
    return true;
  }
  else
  {
    // Focus
    OUTPUT_STRING_FOCUS_CHANGES += (char)Character;

    // Any other character pressed.
    OUTPUT_STRING += (char)Character;
    return true;
  }
}

void TTY_OUTPUT::clean_for_print()
{
  // ----------------------
  // THIS ROUTINE NEEDS WORK:
  // When adding data text needs wrap.
  // When data size exceeds available line count, top needs to be 
  //  trimmed out.

  // simple (string too large) clear
  int linefeeds = count_char_in_string(OUTPUT_STRING, '\n');

  int max_size = PROPS.LINES;

  if (linefeeds > max_size)
  {
    int pos = 0;
    for (int count = 0; count < (linefeeds - max_size); count++)
    {
      pos  = OUTPUT_STRING.find("\n", pos + 1);
    }

    OUTPUT_STRING.erase(0, pos+1);
    NEEDS_REDRAW = true;
  }
}

void TTY_OUTPUT::record_to_history(string Text)
{
  if (PROPS.RECORD_HISTORY == true)
  {
    HISTORY.push_back(Text);
  }
}

void TTY_OUTPUT::create(int Focus_ID)
{
  FOCUS_ID = Focus_ID;
  if (PROPS.TITLE.size() > 0)
  {
    DRAW_TITLE = true;
  }
  else
  {
    DRAW_TITLE = false;
  }
}

void TTY_OUTPUT::redraw()
{
  NEEDS_REDRAW = true;
}

void TTY_OUTPUT::clear()
{
  OUTPUT_STRING_FOCUS_CHANGES = "";
  OUTPUT_STRING = "";
  ENTER_RECEIVED = false;
}

bool TTY_OUTPUT::add_to(int Character, TTY_OUTPUT_FOCUS &Output_Focus, bool Record_If_Allowed)
{
    if (Record_If_Allowed)
  {
    record_to_history(string(1, static_cast<char>(Character)));
  }

  CHANGED = true;
  Output_Focus.set_focus(FOCUS_ID);
  return check_char_recieived(Character);
}

void TTY_OUTPUT::add_to(string Text, TTY_OUTPUT_FOCUS &Output_Focus, bool Record_If_Allowed)
{
  if (Text.size() > 0)
  {
    if (Record_If_Allowed)
    {
      record_to_history(Text);
    }

    OUTPUT_STRING_FOCUS_CHANGES += Text;
    OUTPUT_STRING += Text;
    CHANGED = true;
    Output_Focus.set_focus(FOCUS_ID);
  }
}

void TTY_OUTPUT::seperater(TTY_OUTPUT_FOCUS &Output_Focus)
{
  record_to_history("\n");
  add_to("\n-----\n", Output_Focus, false);
}

bool TTY_OUTPUT::pressed_enter()
{
  if (ENTER_RECEIVED)
  {
    ENTER_RECEIVED = false;
    return true;
  }
  else
  {
    return false;
  }
}

string TTY_OUTPUT::value()
{
  return OUTPUT_STRING;
}

void TTY_OUTPUT::output(TTY_OUTPUT_FOCUS &Output_Focus)
{
  if (CHANGED)
  {
    CHANGED = false;

    //clean_for_print();

    // ----------------------

    if (Output_Focus.focus_changed(FOCUS_ID) || NEEDS_REDRAW)
    {
      Output_Focus.unchanged();
      NEEDS_REDRAW = false;

      OUTPUT_STRING_FOCUS_CHANGES = "";

      //clear_screen();
      move_cursor(PROPS.POSITION_X, PROPS.POSITION_Y);
      clear_lines(PROPS.LINES);
      move_cursor(PROPS.POSITION_X, PROPS.POSITION_Y);

      if (DRAW_TITLE)
      {
        cout << reverse(linemerge_left_justify("-------------------------------------------------------------------------", 
                      PROPS.TITLE)) << endl;
        cout << OUTPUT_STRING << flush;
      }
      else
      {
        cout << OUTPUT_STRING << flush;
      }
    }
    else
    {
      cout << OUTPUT_STRING_FOCUS_CHANGES << flush;
      OUTPUT_STRING_FOCUS_CHANGES = "";
    }
  }
}

// ------------------------------------------------------------------------- //

void TTY_INPUT::set_nonblocking_mode()
{
  tcgetattr(STDIN_FILENO, &STARTING_TERMINAL_STATE); // Save old settings
  termios newt = STARTING_TERMINAL_STATE;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void TTY_INPUT::restore_terminal_settings()
{
  if (INTEFACE_NON_BLOCKING_MODE)
  {
    tcsetattr(STDIN_FILENO, TCSANOW, &STARTING_TERMINAL_STATE); // Restore old settings
  }
}

void TTY_INPUT::create()
{
  if (INTEFACE_NON_BLOCKING_MODE)
  {
    set_nonblocking_mode();
  }
}

void TTY_INPUT::read_input(TTY_OUTPUT &Output_Container, TTY_OUTPUT_FOCUS &Output_Focus)
{
  bool keep_checking = true;

  while (keep_checking)
  {
    if (kbhit())
    {
      CHAR_RECEIVED_LATEST = getchar();
      
      keep_checking = Output_Container.add_to(CHAR_RECEIVED_LATEST, Output_Focus);
    } 
    else 
    {
      keep_checking = false;
    }
  }
}

void TTY_INPUT::clear_screeen()
{
  clear_screen();
}

// ------------------------------------------------------------------------- //

#endif // TTY_IO_CPP