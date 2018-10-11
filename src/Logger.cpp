#include <iostream>
#include "Logger.h"

/**
 * Log information message
 */
void Logger::Info(const std::string& message) {
  std::cout << message << std::endl;
}

/**
 * Log warning message
 */
void Logger::Warn(const std::string& message) {
  std::cerr << message << std::endl;
}

/**
 * Log error message. Abort execution when debugging.
 */
void Logger::Error(const std::string& message) {
  std::cerr << message << std::endl;
#ifndef NODEBUG
  throw std::runtime_error(message);
#endif
}
