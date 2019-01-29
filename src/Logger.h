#ifndef LOGGER_H
#define LOGGER_H

#include <string>

/**
 * Persist or output log messages
 */
class Logger
{
public:
  static void Info(const std::string& message);
  static void Warn(const std::string& message);
  static void Error(const std::string& message);
};

#endif //LOGGER_H
