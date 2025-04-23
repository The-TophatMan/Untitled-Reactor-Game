/**
 * @file exception.h
 * @brief Header file defining the Exception and Logging classes for error handling and logging.
 *
 * This file contains the declarations of the Exception and Logging classes, which provide
 * structured exception handling and logging capabilities. The Exception class encapsulates
 * detailed information about exceptions, including messages, error codes, paths, and severity
 * levels. The Logging class facilitates logging messages and exceptions with varying severity
 * levels to a specified output stream.
 *
 * @author @Ben2445Fun
 * @date 04/23/2025
 * @copyright All Rights Reserved
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <ostream>
#include <vector>

using namespace std;

/**
 * @enum LogType
 * @brief Enum representing different logging levels.
 */
enum LogType
{
  INFO,    ///< Informational messages
  WARNING, ///< Warning messages
  ERROR,   ///< Error messages
  FATAL,   ///< Fatal error messages
  empty    ///< No logging
};

/**
 * @class Exception
 * @brief A class to represent exceptions with detailed information such as message, code, path, and severity.
 */
class Exception
{
private:
  string message; ///< Exception message
  int code;       ///< Error code
  string path;    ///< File path where the exception occurred
  LogType severity; ///< Severity level of the exception

public:
  Exception(const Exception&) = default;
  Exception(Exception&&) noexcept = default;
  Exception& operator=(const Exception&) = default;
  Exception& operator=(Exception&&) noexcept = default;

  /**
   * @brief Constructs an Exception object with detailed information.
   *
   * @param message The exception message.
   * @param path The file path where the exception occurred.
   * @param code The error code associated with the exception.
   * @param severity The severity level of the exception.
   */
  Exception(string message, string path, int code, LogType severity);

  /**
   * @brief Destroys the Exception object.
   */
  ~Exception();

  /**
   * @brief Retrieves the exception message.
   *
   * @param returnval Optional pointer to store the message.
   * @return The exception message.
   */
  const string Message(string* returnval = nullptr);

  /**
   * @brief Retrieves the error code of the exception.
   *
   * @param returnval Optional pointer to store the error code.
   * @return The error code.
   */
  const int Code(int* returnval = nullptr);

  /**
   * @brief Retrieves the file path where the exception occurred.
   *
   * @param returnval Optional pointer to store the file path.
   * @return The file path.
   */
  const string Path(string* returnval = nullptr);

  /**
   * @brief Retrieves the severity level of the exception.
   *
   * @param returnval Optional pointer to store the severity level.
   * @return The severity level.
   */
  const LogType Severity(LogType* returnval = nullptr);

  /**
   * @brief Appends a specified path to the existing path of the exception.
   *
   * @param addToPath The path to append.
   * @return A reference to the updated Exception object.
   */
  Exception operator<<(string addToPath);
};

/**
 * @class Logging
 * @brief A class for logging messages to a stream with different severity levels.
 */
class Logging
{
private:
  LogType loggingLevel[4] = { LogType::empty, LogType::empty, LogType::empty, LogType::empty };
  ostream* output; ///< Output stream for logging

  void writeTime(); ///< Writes the current time to the output stream

public:
  Logging(const Logging&) = delete;
  Logging(Logging&&) = default;
  Logging& operator=(const Logging&) = delete;
  Logging& operator=(Logging&&) = default;

  /**
   * @brief Constructs a Logging object for logging messages to a stream.
   *
   * @param stream The output stream for logging.
   * @param level Optional initializer list of logging levels to enable.
   */
  Logging(ostream& stream, initializer_list<LogType> level = {});

  /**
   * @brief Logs a message with a specified severity level.
   *
   * @param message The message to log.
   * @param severity The severity level of the message (default is INFO).
   */
  void log(string message, LogType severity = INFO);

  /**
   * @brief Logs an Exception object.
   *
   * @param e The Exception object to log.
   */
  void log(Exception& e);

  /**
   * @brief Logs a stack of Exception objects.
   *
   * @param exceptionStack A vector of Exception objects to log.
   */
  void log(vector<Exception> exceptionStack);

  /**
   * @brief Sets the output stream for logging.
   *
   * @param out Pointer to the new output stream.
   */
  void setOutput(ostream* out);

  /**
   * @brief Adds a new logging level to the Logging object.
   *
   * @param newType The new logging level to add.
   */
  void addLoggingType(const LogType newType);

  /**
   * @brief Removes a logging level from the Logging object.
   *
   * @param oldType The logging level to remove.
   */
  void removeLoggingType(const LogType oldType);

  /**
   * @brief Destroys the Logging object and closes the output stream if applicable.
   */
  ~Logging();
};

/**
 * @brief Appends a specified path to each Exception object in the stack.
 *
 * @param stack Pointer to a vector of Exception objects to which the path will be added.
 * @param pathtoadd The path string to append to each Exception in the stack.
 */
void addPath(vector<Exception>* stack, string pathtoadd);

#endif