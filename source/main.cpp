#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

int main_loop(string FileName)
{
  // Main System Varibles
  SYSTEM sdSystem;

  // Print Revision
  string version_info = "";
  version_info += PROGRAM_DESCRIPTION;
  version_info += ": ";
  version_info += REVISION;
  version_info += "\n";
  sdSystem.OUTPUT_RESPONSE.add_to(version_info, sdSystem.OUTPUT_FOCUS);

  // Set options from command line
  sdSystem.FILEWATCH_FILENAME = FileName;

  // ------------------------------------------------------------------------- //

  // Control Loop Variables
  bool main_loop_exit = false;
  bool generate_closing = false;
  bool get_opening = true;

  // ------------------------------------------------------------------------- //

  // Initialize Timers 
  sdSystem.SCREENIO_SLEEP_TIMER.set( 1000 / SCREENIO_SLEEP_TIMER_DELAY );
  sdSystem.FILE_WATCHER_TIMER.set( 1000 / FILE_WATCHER_DELAY );

  sdSystem.PROGRAM_TIME.create();    //  Get current time.  This will be our timeframe to work in.
  if (sdSystem.PROGRAM_TIME.setframetime() == true)
  {
    sdSystem.PROGRAM_TIME.clear_error();
  }

  // ------------------------------------------------------------------------- //
  // Properties Setup
  {
    // Prepair Variables:
    sdSystem.INPUT.create();

    // Clock, and other things, masking as the input title.
    sdSystem.OUTPUT_CLOCK.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_CLOCK.PROPS.POSITION_Y = 0;
    sdSystem.OUTPUT_CLOCK.PROPS.LINES = 1;
    sdSystem.OUTPUT_CLOCK.create(0);

    // Input Text Box:
    //sdSystem.OUTPUT_INPUT.PROPS.TITLE = "  INPUT";
    sdSystem.OUTPUT_INPUT.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_INPUT.PROPS.POSITION_Y = 1;
    sdSystem.OUTPUT_INPUT.PROPS.LINES = 1;
    sdSystem.OUTPUT_INPUT.create(1);

    // Output Text Box:
    sdSystem.OUTPUT_RESPONSE.PROPS.TITLE = "  OUTPUT  ----- ( " + sdSystem.FILEWATCH_FILENAME + " ) ";
    sdSystem.OUTPUT_RESPONSE.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_RESPONSE.PROPS.POSITION_Y = 2;
    sdSystem.OUTPUT_RESPONSE.PROPS.LINES = 100;
    sdSystem.OUTPUT_RESPONSE.PROPS.RECORD_HISTORY = false;
    sdSystem.OUTPUT_RESPONSE.create(2);

    // Blank the screen.
    sdSystem.INPUT.clear_screeen();
  }

  {
    sdSystem.FILE_WATCHER.PROP.READ_FROM_BEGINNING = true;
    sdSystem.FILE_WATCHER.PROP.WATCH_SIZE_CHANGE = false;
    sdSystem.FILE_WATCHER.PROP.WATCH_TIME_CHANGE = true;
  }

  // ------------------------------------------------------------------------- //
  // Send the output of the create to the screen.
  //sdSystem.OUTPUT_RESPONSE.output(sdSystem.OUTPUT_FOCUS);

  // ------------------------------------------------------------------------- //
  // Clock and status
  SIMPLE_MAIN_LOOP_PROCESSOR_USAGE processor;
  string processor_status_display_simple_old = "";
  string processor_status_display_simple = "";

  // ------------------------------------------------------------------------- //

  // Main Thread Loop
  while (main_loop_exit == false)
  {
    // ------------------------------------------------------------------------- //
    {
      // Main loop set up and start
      if (sdSystem.PROGRAM_TIME.setframetime() == true)
      {
        sdSystem.OUTPUT_RESPONSE.add_to("CLOCK SKEW DETECTED\n", sdSystem.OUTPUT_FOCUS);
        sdSystem.PROGRAM_TIME.clear_error();
      }

      // Threading placeholder

      // Close all completed and active threads after sleep cycle is complete.
      // THOUGHTS_SYSTEM.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.check_for_completition();
      // THOUGHTS_SYSTEM.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.check_for_completition();
    }

    // ------------------------------------------------------------------------- //
    // Show Clock and Status
    if (sdSystem.OUTPUT_INPUT.value() != "")
    {
      processor_status_display_simple = " (File Watcher) ";

      if (processor.changed() || processor_status_display_simple_old != processor_status_display_simple)
      {
        processor_status_display_simple_old = processor_status_display_simple;
        
        sdSystem.OUTPUT_CLOCK.clear();
        sdSystem.OUTPUT_CLOCK.redraw();
        sdSystem.OUTPUT_CLOCK.add_to(reverse(linemerge_left_justify("-------------------------------------------------------------------------", 
                                      processor.what_is_it() +
                                      processor_status_display_simple +
                                      + " ( " + sdSystem.FILEWATCH_FILENAME + " ) " + 
                                      " INPUT:")), sdSystem.OUTPUT_FOCUS);
      }
    }

    // ------------------------------------------------------------------------- //
    // Opening and Closing routines
    {
      if (get_opening)
      {
        get_opening = false;
        // Place Holders
        sdSystem.FILE_WATCHER.start(sdSystem.FILEWATCH_FILENAME);
      }

      if (generate_closing)
      {
        // Place Holders
        main_loop_exit = true;
        sdSystem.FILE_WATCHER.stop();
      }
    }

    // ------------------------------------------------------------------------- //
    // FileWatch check routine

    //  Never comment this out or the system will never sleep
    if (sdSystem.FILE_WATCHER_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      if (sdSystem.FILE_WATCHER.changed())
      {
        sdSystem.INPUT.clear_screeen();
        sdSystem.OUTPUT_RESPONSE.clear();
        sdSystem.OUTPUT_RESPONSE.add_to(reverse("***** BEGIN *****"), sdSystem.OUTPUT_FOCUS);
        sdSystem.OUTPUT_RESPONSE.seperater(sdSystem.OUTPUT_FOCUS);
        sdSystem.OUTPUT_RESPONSE.add_to(file_to_string(sdSystem.FILEWATCH_FILENAME), sdSystem.OUTPUT_FOCUS);
        sdSystem.OUTPUT_RESPONSE.add_to(reverse("*****  END *****"), sdSystem.OUTPUT_FOCUS);
      }
    }

    // ------------------------------------------------------------------------- //
    // Screen IO Check.

    //  Never comment this out or the system will never sleep
    if (sdSystem.SCREENIO_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      sdSystem.INPUT.read_input(sdSystem.OUTPUT_INPUT, sdSystem.OUTPUT_FOCUS);

      if (sdSystem.OUTPUT_INPUT.CHANGED)
      {
        if (sdSystem.OUTPUT_INPUT.pressed_enter())
        {
          string input_entered = sdSystem.OUTPUT_INPUT.value();

          // Send Command Placeholder
          sdSystem.OUTPUT_RESPONSE.add_to("YOU:\n     " + input_entered, sdSystem.OUTPUT_FOCUS);
          sdSystem.OUTPUT_RESPONSE.seperater(sdSystem.OUTPUT_FOCUS);
          sdSystem.OUTPUT_INPUT.clear();

          if (input_entered.size() > 0)
          {
            if (input_entered == "bye")
            {
              // turn off the recorder to save the conclusion
              sdSystem.OUTPUT_RESPONSE.PROPS.RECORD_HISTORY = false;
              sdSystem.OUTPUT_RESPONSE.add_to("   *---- EXITING THE SYSTEM\n", sdSystem.OUTPUT_FOCUS);
              generate_closing = true;
            }
            else if (input_entered == "FORCEEXIT")
            {
              main_loop_exit = true;
            }
            else
            {
              // Send a Command Placeholder
            }
          }
        }
      }

      sdSystem.OUTPUT_CLOCK.output(sdSystem.OUTPUT_FOCUS);
      sdSystem.OUTPUT_INPUT.output(sdSystem.OUTPUT_FOCUS);
      sdSystem.OUTPUT_RESPONSE.output(sdSystem.OUTPUT_FOCUS);
    }

    // ------------------------------------------------------------------------- //
    // Sleep till next cycle check.
    
    // Make sure non of these are commented out, or the system will never sleep.
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.SCREENIO_SLEEP_TIMER.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.FILE_WATCHER_TIMER.get_ready_time());

    sdSystem.PROGRAM_TIME.sleep_till_next_frame();

  } // end of main loop

  // ------------------------------------------------------------------------- //
  // ------------------------------------------------------------------------- //

  // Clean up and exit.
  sdSystem.INPUT.clear_screeen();


  // Shutdown any open threads process
  {
    // PlaceHolders for closing threads
    // FILE_WATCHER_THREAD.wait_for_thread_to_finish("FILE_WATCHER_THREAD");
  }
  
  // Restore the terminal
  sdSystem.INPUT.restore_terminal_settings();

  return 0;

}


// ------------------------------------------------------------------------- //

int main(int argc, char* argv[])
{
  int ret_error_code = 0;
  if (argc < 2)
  {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    cerr << "  Commands:" << endl;
    cerr << "    bye       - Exits the program" << endl;
    cerr << "    FORCEEXIT - Ends the program" << endl;
    ret_error_code = 1;
  }
  else
  {
    return main_loop(argv[1]);
  }
  return ret_error_code;
}

#endif // MAIN_CPP