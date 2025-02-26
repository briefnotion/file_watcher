// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: definitions.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define RASFLED

// Debug Counters
#define DEBUG_COUNTER false

// -------------------------------------------------------------------------------------
// DEFINITIONS
// -------------------------------------------------------------------------------------

#define PROGRAM_DESCRIPTION "File Watcher"
#define REVISION            "0.003_240125"
#define QACODE              "X012"

// -------------------------------------------------------------------------------------
// CONSOLE AND INTEFACE SETUP

// Variable Redraw Rates
// 60 - 15 fps
// 30 - 30 fps
// 15 - 60 fps
//#define SCREENUPDATEDELAY 30  // in milliseconds 
#define SCREENIO_SLEEP_TIMER_DELAY  30  // This will be in frames per second (fps)
#define FILE_WATCHER_DELAY          5   // This will be in frames per second (fps)

#define TEST_MODE           false

// Inteface Mode
#define INTEFACE_NON_BLOCKING_MODE true

// StupidRandom Parameters
#define StuRNDsize              100

#endif