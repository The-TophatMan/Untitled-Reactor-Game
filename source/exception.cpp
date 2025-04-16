#include "../include/exception.h"
#include <iostream>
#include <ctime>
using namespace std;

Exception::Exception(string message, string path, int code, int severity)
{
  this->message = message;
  this->path = path;
  this->code = code;
  this->severity = severity;
}

Exception::~Exception() {}

const std::string Exception::getMessage(string* returnval) {
  if (returnval != nullptr) returnval = &message;
  return message;
}

const int Exception::getCode(int* returnval) {
  if (returnval != nullptr)
    returnval = &code;
  return code;
}

const std::string Exception::getPath(std::string* returnval) {
  if (returnval != nullptr)
    returnval = &path;
  return path;
}

const int Exception::getSeverity(int* returnval) {
  if (returnval != nullptr)
    returnval = &severity;
  return severity;
}

Exception Exception::operator<<(string addToPath)
{
  string newPath = addToPath;
  newPath += "::";
  newPath += path;
  path = newPath;
  return *this;
}

void Logging::writeTime()
{
  time_t currTime = time(NULL);
  tm timestamp = *localtime(&currTime);
  char logstamp[9];
  strftime(logstamp, 9, "%H:%M:%S", &timestamp);
  *output << "[" << logstamp << "] ";
}

Logging::Logging(int level, ostream& stream)
{
  loggingLevel = level;
  output = &stream;
}

void Logging::log(string message, int severity) {
  if (!output || loggingLevel == 0 || severity < 0 || severity > loggingLevel) return;
  writeTime();
  if (severity == 0)
    *output << "INFO    ";
  else if (severity == 1)
    *output << "WARNING ";
  else if (severity == 2)
    *output << "ERROR   ";
  else
    *output << "FATAL   ";
  *output << message << endl;
}

void Logging::log(Exception& e)
{
  if (!output || loggingLevel == 0 || e.getSeverity() > loggingLevel) return;
  writeTime();
  if (e.getSeverity() == 1)
    *output << "WARNING ";
  else if (e.getSeverity() == 2)
    *output << "ERROR   ";
  else
    *output << "FATAL   ";
  *output << "Exception occured at " << e.getPath() << " with code " << e.getCode() << ". Message: " << e.getMessage() << endl;
}


