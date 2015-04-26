#include "StandardWindowsLogger.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include <streambuf>

int StandardWindowsLogger::count = 0;

StandardWindowsLogger::StandardWindowsLogger() {
#ifdef WIN32
  if (count == 0) {
    setp(0, 0);

    // set std::cout to use the custom streambuf
    backupOut = std::cout.rdbuf(this);
    backupErr = std::cerr.rdbuf(this);
    backupLog = std::clog.rdbuf(this);
  }
#endif

  ++count;
}


StandardWindowsLogger::~StandardWindowsLogger() throw() {
#ifdef WIN32
  if (count == 0) {
    // make sure to restore the original so we don't get a crash on close!
    std::cout.rdbuf(backupOut);
    std::cerr.rdbuf(backupErr);
    std::clog.rdbuf(backupLog);
  }
#endif

  --count;
}

std::streambuf::int_type StandardWindowsLogger::overflow(int_type c = traits_type::eof()) {
#ifdef WIN32
  currentLine.push_back(c);
  int value = fputc(c, stdout);

  if (value == EOF || value == '\n') {
    OutputDebugStringA(currentLine.c_str());
    currentLine.clear();
  }
#endif
  return c;
}