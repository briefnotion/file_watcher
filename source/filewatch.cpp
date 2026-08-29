#ifndef FILEWATCH_CPP
#define FILEWATCH_CPP

#include "filewatch.h"

#include <fstream>
#include <cstdio>
#include <sys/stat.h>

using namespace std;

void FILE_WATCH::start(const string &Filename)
{
  filename = Filename;
}

bool FILE_WATCH::changed()
{
  struct stat st{};

  if (stat(filename.c_str(), &st) != 0)
  {
    // File does not exist (or is not accessible). Report the disappearance
    //  once, then go quiet until it reappears.
    bool was_existing = file_existed;
    file_existed = false;
    return was_existing;
  }

  bool is_change = (!file_existed) || (st.st_mtime != last_mtime);

  file_existed = true;
  last_mtime = st.st_mtime;

  return is_change;
}

bool FILE_WATCH::exists() const
{
  return file_existed;
}

vector<string> FILE_WATCH::load_lines() const
{
  vector<string> lines;
  ifstream file(filename);

  if (!file.is_open())
  {
    return lines;
  }

  string line;
  while (getline(file, line))
  {
    lines.push_back(line);
  }

  return lines;
}

string FILE_WATCH::last_change_time_string() const
{
  if (!file_existed)
  {
    return "--:--:--";
  }

  tm tm_buf{};
  localtime_r(&last_mtime, &tm_buf);

  char buf[16];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", tm_buf.tm_hour, tm_buf.tm_min, tm_buf.tm_sec);

  return string(buf);
}

const string &FILE_WATCH::name() const
{
  return filename;
}

#endif // FILEWATCH_CPP
