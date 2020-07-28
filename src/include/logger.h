#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#define LOG_FILE_PATH "log.txt"
#define LOG_DEBUG(str) (Logger::get_instance()->Debug(str))
#define LOG_INFO(str) (Logger::get_instance()->Info(str))
#define LOG_WARN(str) (Logger::get_instance()->Warning(str))
#define LOG_ERR(str) (Logger::get_instance()->Error(str))

namespace cocktorrent {
class Logger {
 private:
  std::ofstream file{LOG_FILE_PATH};

 public:
  static Logger *get_instance();

  void Debug(const std::string &);

  void Info(const std::string &);

  void Warning(const std::string &);

  void Error(const std::string &);

 private:
  ~Logger() = default;

  Logger() = default;

  Logger(const Logger &);

  void Write(const std::string &, const std::string &);

  Logger &operator=(const Logger &) { return *this; };
};
}  // namespace cocktorrent

#endif  // LOGGER_H