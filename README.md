# file_watcher

## Overview
`file_watcher` is a terminal program that displays the contents of a file and
keeps them live-updated as the file changes — a scrollable, ncurses-based
viewer with line numbers, a status bar, and a command line for jumping
around.

## Requirements
- A built `ncursesw` at `../../ncurses-snapshots/install` (a sibling checkout
  next to this project, same convention as the `olli` project). If it isn't
  there yet:
  ```sh
  git clone https://github.com/ThomasDickey/ncurses-snapshots.git ../../ncurses-snapshots
  cd ../../ncurses-snapshots
  mkdir build && cd build
  ../configure --prefix=$(pwd)/../install --without-shared \
    --enable-widec --without-debug --without-ada --without-tests \
    --without-manpages --without-progs
  make -j && make install
  ```
- A `CMakeLists.txt` file is provided for building the project.

## Functionality

- **Live updates**: the file is re-read whenever its modification time
  changes, and the view follows the end of the file automatically as long
  as you haven't scrolled away from the bottom.
- **Line numbers**: a gutter on the left shows the line number of every
  displayed line.
- **Status bar**: the top line shows the filename, the time of the last
  detected change, and either `[FOLLOWING]` or your current line position.
- **Resizable**: resizing the terminal reflows the view immediately.
- **Scrolling and commands**, via the keyboard:
  | Key                | Action                                   |
  |--------------------|-------------------------------------------|
  | `Up` / `Down`      | Scroll one line                           |
  | `PgUp` / `PgDn`    | Scroll one screen                         |
  | `Home`             | Jump to the top                           |
  | `End`              | Jump to the bottom and resume following   |
  | *(number)* `Enter` | Go to that line number                    |
  | `q` `Enter`        | Quit                                      |

## Compilation and Usage
1. Build the project using CMake from the existing `build` directory:
   ```sh
   cd build
   cmake -S ../source -B .
   make
   ```

2. Run the program with the filename as the argument:

   ```sh
   ./filewatch filename.txt
   ```

## Installing the Command System-Wide

To run `filewatch` from any directory without typing the full path, install it
under your own account — no `sudo` required:

```sh
mkdir -p ~/.local/bin
cp build/filewatch ~/.local/bin/
```

`~/.local/bin` is already on your `PATH`, so `filewatch` will be available in
any new terminal session right away. (If it isn't on your `PATH`, add
`export PATH="$HOME/.local/bin:$PATH"` to your `~/.bashrc` or `~/.zshrc` and
reload your shell.)

If you'd rather install it for every user on the machine, `build/install.sh`
copies the binary to `/usr/local/bin` instead, which does require `sudo`.
