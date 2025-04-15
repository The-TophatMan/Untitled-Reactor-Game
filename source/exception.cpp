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
  output << "[" << logstamp << "] ";
}

Logging::Logging(int level, string file)
{
  loggingLevel = level;
  output.open(file, ios::binary);
}

void Logging::log(string message, int type)
{
  writeTime();
  if (type == 0)
    output << "INFO    ";
  else if (type == 1)
    output << "WARNING ";
  else if (type == 2)
    output << "ERROR   ";
  else
    output << "FATAL   ";
  output << message << endl;
}

void Logging::log(std::string message)
{
  writeTime();
  output << "INFO    " << message << endl;
}

void Logging::log(Exception &e)
{
  writeTime();
  if (e.getSeverity() == 1)
    output << "WARNING ";
  else if (e.getSeverity() == 2)
    output << "ERROR   ";
  else
    output << "FATAL   ";
  output << "Exception occured at " << e.getPath() << " with code " << e.getCode() << ". Message: " << e.getMessage() << endl;
}
