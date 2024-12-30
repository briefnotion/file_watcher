#ifndef SYSTEM_H
#define SYSTEM_H

// Main includes
#include "definitions.h"
#include "helper.h"
#include "fled_time.h"
#include "tty_io.h"
#include "threading.h"

// Secondary Includes
#include "rasapi.h"

using namespace std;

// -------------------------------------------------------------------------------------

// Contains all varibles to be access by the main and provides a communications 
//  channel between them.

// -------------------------------------------------------------------------------------

class SYSTEM
{
  public:

  // File to watch
  string FILEWATCH_FILENAME   = "";
  FILE_WATCH FILE_WATCHER;

  // Main Program Sleep Timers
  TIMED_IS_READY    SCREENIO_SLEEP_TIMER;
  TIMED_IS_READY    FILE_WATCHER_TIMER;

  // I/O
  TTY_INPUT         INPUT;
  
  TTY_OUTPUT_FOCUS  OUTPUT_FOCUS;
  
  TTY_OUTPUT        OUTPUT_CLOCK;
  TTY_OUTPUT        OUTPUT_INPUT;
  TTY_OUTPUT        OUTPUT_RESPONSE;

  FLED_TIME        PROGRAM_TIME;

  //THREADING_INFO FILE_WATCHER_THREAD;

};

// ------------------------------------------------------------------------- //


#endif // SYSTEM_H