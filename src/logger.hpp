/**
 * @file   logger.hpp
 * @author yuhonglin <yuhonglin1986@gmail.com>
 * @date   Tue Apr  7 12:29:12 2015
 *
 * @brief  A simple logger system
 *
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <initializer_list>

#include <cstdlib>
#include <ctime>

using std::string;
using std::set;
using std::endl;

class Logger {

public:
  enum LogType {
    LOGTYPE_INFO,
    LOGTYPE_DEBUG,
    LOGTYPE_WARNING,
    LOGTYPE_ERROR
  };

  virtual ~Logger() {};
  static Logger *get_instance();

  /// verbose
  static bool enable_time;
  static bool enable_position;

  /// if use exception, otherwise use abort
  static bool enable_exception;

  /// whether to break when logging message
  static bool enable_error_break;
  static bool enable_warning_break;
  static bool enable_info_break;
  static bool enable_debug_break;
  static bool is_using_file;

  /// messages to be ignored, i.g. not break
  static set<string> ignored;

  /// the log interface
  template <typename... Args>
  void log(string ccHead, string logtype, string ccTail, string file, int line,
           Args... args) {
#ifdef __linux__
    if (is_using_file == false) {
      _lout << ccHead;
    }
#endif
    _lout << "[" << logtype << "]";
#ifdef __linux__
    if (is_using_file == false) {
      _lout << ccTail;
    }
#endif
    if (enable_position == true) {
      _lout << " (in \"" << file << "\" at line " << line << ") ";
    }
    _log(args...);
  }

  /// set log file name
  static void set_filename(string filename);

  /// use file or std
  static void use_std();

  /// use file
  static void use_file();

private:
  /// singleton
  Logger() {};

  /// output file streams
  static std::ofstream _output_stream;
  static std::ostream _lout;

  /// output filename
  static string _output_filename;

  /// generate time string
  static string get_current_time_string();

  template <typename T> void _log(T t) { _lout << t << std::endl; }

  template <typename T, typename... Args>
  void _log(T t, Args... args) // recursive variadic function
  {
    _lout << t;
    _log(args...);
  }
};

/**
 * Define useful macros
 *
 */
#define LOGGER Logger::get_instance()

/**
 * LOG macro
 *
 */

#define LOG_INFO(...)                                                          \
  LOGGER->log("\033[1;32m", "INFO", "\033[1;0m ", __FILE__, __LINE__,          \
              __VA_ARGS__)

#define LOG_DEBUG(...)                                                         \
  LOGGER->log("\033[1;36m", "DEBUG", "\033[1;0m ", __FILE__, __LINE__,         \
              __VA_ARGS__)

#define LOG_WARNING(...)                                                       \
  LOGGER->log("\033[1;33m", "WARNING", "\033[1;0m ", __FILE__, __LINE__,       \
              __VA_ARGS__)

#define LOG_ERROR(...)                                                         \
  LOGGER->log("\033[1;31m", "ERROR", "\033[1;0m ", __FILE__, __LINE__,         \
              __VA_ARGS__)

/**
 * LOG ONCE MICRO
 *
 */
#define CONCATINATE(x, y) x##y
#define MAKE_VARIABLE(x, y) CONCATINATE(x, y)

#define LOG_INFO_ONCE(...)                                                     \
  static bool MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = true;                 \
  if (MAKE_VARIABLE(b##__FUNCTION__, __LINE__)) {                              \
    LOGGER->log("\033[1;32m", "INFO", "\033[1;0m ", __FILE__, __LINE__,        \
                __VA_ARGS__);                                                  \
    MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = false;                          \
  }

#define LOG_DEBUG_ONCE(...)                                                    \
  static bool MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = true;                 \
  if (MAKE_VARIABLE(b##__FUNCTION__, __LINE__)) {                              \
    LOGGER->log("\033[1;36m", "DEBUG", "\033[1;0m ", __FILE__, __LINE__,       \
                __VA_ARGS__);                                                  \
    MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = false;                          \
  }

#define LOG_WARNING_ONCE(...)                                                  \
  static bool MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = true;                 \
  if (MAKE_VARIABLE(b##__FUNCTION__, __LINE__)) {                              \
    LOGGER->log("\033[1;33m", "WARNING", "\033[1;0m ", __FILE__, __LINE__,     \
                __VA_ARGS__);                                                  \
    MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = false;                          \
  }

#define LOG_ERROR_ONCE(...)                                                    \
  static bool MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = true;                 \
  if (MAKE_VARIABLE(b##__FUNCTION__, __LINE__)) {                              \
    LOGGER->log("\033[1;31m", "ERROR", "\033[1;0m ", __FILE__, __LINE__,       \
                __VA_ARGS__);                                                  \
    MAKE_VARIABLE(b##__FUNCTION__, __LINE__) = false;                          \
  }

/**
 * TODO MICRO
 *
 */
#define TODO LOG_ERROR_ONCE("\"", __PRETTY_FUNCTION__, "\" is not implemented")

#endif /* LOGGER_H */
