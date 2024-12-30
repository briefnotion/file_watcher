# file_watcher

## Overview
`file_watcher` is a program that reads and displays the contents of a file on the screen in real-time. Any changes made to the file will be reflected on the screen immediately. The program also supports two special commands:
- `bye`: Exits the program gracefully.
- `FORCEEXIT`: Terminates the program immediately.

## Requirements
- Boost libraries are required to compile and run the program.
- A `CMakeLists.txt` file is provided for building the project.

## Functionality
1. **Initialization**:
   - The program takes a filename as a command-line argument.
   - It initializes necessary resources and opens the specified file for reading.

2. **File Monitoring**:
   - The program continuously monitors the file for any changes.
   - Whenever a change is detected, the program reads the updated content and refreshes the display.

3. **Command Handling**:
   - The user can input commands in the terminal while the program is running.
   - Typing `bye` will gracefully exit the program, ensuring all resources are properly released.
   - Typing `FORCEEXIT` will immediately terminate the program.

4. **Error Handling**:
   - Proper error handling is implemented for some scenarios.

## Usage
1. Build the project using CMake from the existing `build` directory:
   ```sh
   cd build
   cmake ..
   make

2. Run the program with the filename as the argument:

   ```sh
   ./file_watcher filename.txt

Enter bye to exit or FORCEEXIT to quit the program forcefully.