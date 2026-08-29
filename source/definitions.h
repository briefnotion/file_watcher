#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PROGRAM_DESCRIPTION "File Watcher"
#define REVISION            "0.005_260829"

// How often (in ms) the watched file's modification time is checked.
#define FILE_POLL_INTERVAL_MS   200

// How long (in ms) getch() blocks waiting for a keypress before returning,
//  so the main loop can go check the file and redraw even with no input.
#define INPUT_POLL_INTERVAL_MS  100

#endif
