# Minimum required version of CMake
cmake_minimum_required(VERSION 3.18.0)

set(BUILD_RELEASE ON)
set(BUILD_STRICT ON)

if(BUILD_RELEASE)
      set(CMAKE_BUILD_TYPE Release)
else()
      set(CMAKE_BUILD_TYPE Debug)
endif()

# Project name and description
project(filewatch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ncursesw (wide-char): not packaged system-wide here, so it's built from a
#  sibling checkout (../../ncurses-snapshots, same convention used by the
#  olli project) into its own local install prefix rather than installed
#  system-wide - see ncurses-snapshots/install after running its own
#  configure/make/make install. Static (--without-shared) so filewatch stays
#  a single self-contained binary.
# curses.h itself does "#include <ncursesw/ncurses_dll.h>", so the include
#  path needs to be the parent of the ncursesw/ directory, not that directory
#  itself - hence searching for ncursesw/curses.h rather than just curses.h.
find_path(NCURSES_INCLUDE_DIR
    NAMES ncursesw/curses.h
    HINTS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../ncurses-snapshots/install/include"
)

find_library(NCURSES_LIBRARY
    NAMES ncursesw
    HINTS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../ncurses-snapshots/install/lib"
)

if(NOT NCURSES_INCLUDE_DIR OR NOT NCURSES_LIBRARY)
    message(FATAL_ERROR
        "Could not find a built ncursesw. Clone "
        "https://github.com/ThomasDickey/ncurses-snapshots.git next to this "
        "project (../../ncurses-snapshots) and build it there: "
        "mkdir build && cd build && "
        "../configure --prefix=$(pwd)/../install --without-shared "
        "--enable-widec --without-debug --without-ada --without-tests "
        "--without-manpages --without-progs && make -j && make install")
endif()

message(STATUS "Using ncursesw headers from: ${NCURSES_INCLUDE_DIR}")
message(STATUS "Using ncursesw library at: ${NCURSES_LIBRARY}")

add_executable(filewatch)

target_include_directories(filewatch PRIVATE
  ${NCURSES_INCLUDE_DIR}
)

target_sources(filewatch PRIVATE
  filewatch.cpp
  ui.cpp
  main.cpp
)

if(BUILD_STRICT)
  # Strict
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(filewatch PRIVATE -Wall -Wextra -Werror)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(filewatch PRIVATE -Weverything -Werror)
  endif()
else()
  # Not Strict
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(filewatch PRIVATE -Wall -Wextra)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(filewatch PRIVATE -Weverything)
  endif()
endif()

target_link_libraries(filewatch PRIVATE
    ${NCURSES_LIBRARY}
)
