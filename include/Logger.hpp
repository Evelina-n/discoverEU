#pragma once
#include <string>
#include <fstream>

namespace DEU {

enum LogLevel {
  DEBUG, INFO, WARN, FATAL
};

struct Logger {
  Logger();
  Logger(std::string log_file);

  void log(LogLevel level, std::string message);

  static void Log(LogLevel level, std::string message) { s_Logger->log(level, message); }
private:
  static Logger *s_Logger;
  std::ostream *fl;
};
};