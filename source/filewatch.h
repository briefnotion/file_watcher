#ifndef FILEWATCH_H
#define FILEWATCH_H

#include <string>
#include <vector>
#include <ctime>

// Watches a file's modification time and reports when it changes.
class FILE_WATCH
{
  private:
  std::string filename;
  time_t last_mtime = 0;
  bool file_existed = false;

  public:
  void start(const std::string &Filename);

  // Returns true once per detected change: file created, modified, or removed.
  bool changed();

  // True if the file existed as of the most recent changed() call.
  bool exists() const;

  // Reads the file fresh and returns its contents split into lines
  //  (no trailing newlines).
  std::vector<std::string> load_lines() const;

  // Formatted HH:MM:SS of the last detected modification time, or
  //  "--:--:--" if the file has never been seen.
  std::string last_change_time_string() const;

  const std::string &name() const;
};

#endif
