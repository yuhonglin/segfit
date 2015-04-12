#include "logger.hpp"

/**
 * actually define the logger members
 * and initialize them
 *
 */
bool Logger::enable_time = false;
bool Logger::enable_position = false;
bool Logger::enable_exception = true;
bool Logger::enable_error_break = true;
bool Logger::enable_warning_break = false;
bool Logger::enable_info_break = false;
bool Logger::enable_debug_break = false;
bool Logger::is_using_file = false;
set<string> Logger::ignored;
std::ofstream Logger::_output_stream;
std::ostream Logger::_lout(std::cout.rdbuf());
string Logger::_output_filename;

Logger *Logger::get_instance() {
  static Logger instance;
  return &instance;
}

void Logger::set_filename(string filename) { _output_filename = filename; }

void Logger::use_std() {
  _output_stream.close();
  _lout.rdbuf(std::cout.rdbuf());
  is_using_file = false;
}

void Logger::use_file() {
  _output_stream.open(_output_filename);
  _lout.rdbuf(_output_stream.rdbuf());
  is_using_file = true;
}

string Logger::get_current_time_string() {
  time_t t = time(0);
  struct tm *now = localtime(&t);
  std::stringstream ss;
  ss << now->tm_year + 1900 << '-' << std::setfill('0') << std::setw(2)
     << (now->tm_mon + 1) << '-' << std::setfill('0') << std::setw(2)
     << now->tm_mday << ' ' << std::setfill('0') << std::setw(2) << now->tm_hour
     << ':' << std::setfill('0') << std::setw(2) << now->tm_min << ':'
     << std::setfill('0') << std::setw(2) << now->tm_sec;
  return ss.str();
}
