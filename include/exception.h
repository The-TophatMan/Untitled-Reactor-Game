#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <fstream>
/// @brief A basic exception class.
///
/// Stores a provided message, error path, error code, and the severity level.
class Exception
{
private:
  std::string message;
  int code;
  std::string path;
  int severity; // 0 - Warning, 1 - Error, 2 - Fatal
public:
  // Begin Constructors
  /// @brief Exception constructor.
  /// @param message The message to pass through.
  /// @param path The path to follow to find the errror. Use @ref << to add more to the path structure.
  /// @param code The code to pass through. Use in conjunction with a fatal severity to let the program exit with a certain code.
  /// @param severity The severity of the exception. 0 = Warning, 1 = Error, 2 = Fatal
  Exception(std::string message, std::string path, int code, int severity);
  ~Exception() {}
  // End Constructors
  // Begin Getters
  /// @brief Get the message provided by the exception.
  /// @return A string containing the exception message.
  std::string getMessage() { return message; }
  /// @brief Get the exception code. Use in conjunction with @ref getSeverity to exit the program with this code.
  /// @return An integer containing the exception code.
  int getCode() { return code; }
  /// @brief Get the exception path. Use in multiple throw-catch blocks to get a direct line to where the exception was thrown.
  /// @return A string containing the path all the way to the original throw block.
  std::string getPath() { return path; }
  /// @brief Get the severity level of the exception.
  /// @return An integer relating to the exceptions severity. 0 = Warning, 1 = Error, 2 = Fatal
  int getSeverity() { return severity; }
  // End Getters
  // Begin Overloads
  /// @brief Add an extension to the exception's path to help follow through multiple throw-catch blocks.
  Exception operator<<(std::string addToPath);
  // End Overloads
};

/// @brief A basic logging class able to log directly to a file.
class Logging
{
private:
  int loggingLevel = 0;
  std::ofstream output;
  void writeTime();

public:
  /// @brief Log file constructor.
  /// @param level How much should be logged. 0 = No Logging, 1 = Fatals, 2 = Errors and Fatals, 3 = Warnings, Errors, and Fatals, 4 = Info, Warnings, Errors, and Fatals.
  /// @param file The file to log to.
  Logging(int level, std::string file);
  /// @brief A message to write to the file.
  /// @param message The message to write.
  /// @param type The severity of the message. 0 = Info, 1 = Warning, 2 = Error, 3 = Fatal
  void log(std::string message, int type);
  /// @brief A message to write to the file. All messages are logged as info.
  /// @param message The message to write.
  void log(std::string message);
  /// @brief A message to write to the file. Writes special data from the Exception class.
  /// @param e The exception to log
  void log(Exception &e);
};

#endif