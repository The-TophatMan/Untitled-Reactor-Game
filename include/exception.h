#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <fstream>
struct Exception
{
private:
  std::string message;
  int code;
  std::string path;
  int severity; // 0 - Warning, 1 - Error, 2 - Fatal
public:
  // Begin Constructors
  Exception(std::string message, std::string path, int code, int severity);
  ~Exception() {}
  // End Constructors
  // Begin Getters
  std::string getMessage() { return message; }
  int getCode() { return code; }
  std::string getPath() { return path; }
  int getSeverity() { return severity; }
  // End Getters
  // Begin Overloads
  Exception operator<<(std::string addToPath);
  // End Overloads
};

class Logging
{
private:
  int loggingLevel = 0;
  std::ofstream output;

public:
  Logging(int level, std::string file);
  void log(std::string message, int type);
};

#endif