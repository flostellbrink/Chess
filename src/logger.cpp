#include <iostream>
#include "logger.h"

/**
 * Log information message
 */
void Logger::info(std::string message) {
  std::cout << message << std::endl;
}

/**
 * Log warning message
 */
void Logger::warn(std::string message) {
  std::cerr << message << std::endl;
}

/**
 * Log error message. Abort execution when debugging.
 */
void Logger::error(std::string message) {
  std::cerr << message << std::endl;
#ifndef NODEBUG
  throw std::runtime_error(message);
#endif
}
