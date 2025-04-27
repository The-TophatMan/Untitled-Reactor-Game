/**
 * @file exception.cpp
 * @brief Implementation of the Exception and Logging classes for error handling and logging.
 *
 * This file contains the implementation of the Exception class, which represents
 * an exception with a message, path, code, and severity. It also includes the Logging
 * class, which provides functionality for logging messages and exceptions with different
 * severity levels.
 *
 * @author @Ben2445Fun
 * @date 04/23/2025
 * @copyright All Rights Reserved
 */
#include "../include/exception.h"
#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

// Exception implementation
Exception::Exception(string message, string path, int code, LogType severity)
{
  this->message = message;
  this->path = path;
  this->code = code;
  this->severity = severity;
}

Exception::~Exception() {}

string Exception::Message(string *returnval)
{
  if (returnval != nullptr)
    returnval = &message;
  return message;
}

int Exception::Code(int *returnval)
{
  if (returnval != nullptr)
    returnval = &code;
  return code;
}

string Exception::Path(string *returnval)
{
  if (returnval != nullptr)
    returnval = &path;
  return path;
}

LogType Exception::Severity(LogType *returnval)
{
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

// Exception Stack implementation
void ExceptionStack::addPath(string pathtoadd)
{
  for (int i = 0; i < stack.size(); i++)
    stack[i] << pathtoadd;
}

int ExceptionStack::size(int *returnval)
{
  if (returnval != nullptr)
    *returnval = stack.size();
  return stack.size();
}

Exception ExceptionStack::exception(const int pos, Exception *returnval)
{
  if (pos < 0 || pos >= stack.size())
    return Exception(
        "Attempted to access exception outside of exception stack range",
        "ExceptionStack::exception", -1, ERROR);
  if (returnval != nullptr)
    *returnval = stack[pos];
  return stack[pos];
}

void ExceptionStack::insert(Exception e) { stack.push_back(e); }

void ExceptionStack::flushStack() { stack.clear(); }

// Logging implementation
Logging::Logging(ostream &stream, initializer_list<LogType> level)
{
  copy(level.begin(), level.end(), loggingLevel);
  output = &stream;
}

void Logging::log(string message, LogType severity)
{
  if (severity == LogType::empty)
    return;
  for (int i = 0; i < 4; i++)
    if (loggingLevel[i] == severity)
      break;
    else if (i >= 3)
      return;
  writeTime();
  string severityText;
  switch (severity)
  {
  case LogType::INFO:
    severityText = "INFO    ";
    break;
  case LogType::WARNING:
    severityText = "WARNING ";
    break;
  case LogType::ERROR:
    severityText = "ERROR   ";
    break;
  case LogType::FATAL:
    severityText = "FATAL   ";
    break;
  default:
    severityText = "UNKNOWN ";
    break;
  }
  *output << severityText << message << endl;
  output->flush();
}

void Logging::log(Exception e)
{
  if (e.Severity() == INFO)
    log(e.Path() + " > " + e.Message(), INFO);
  else
    log(e.Path() + " > " + e.Message() + ", Code " + to_string(e.Code()), e.Severity());
  if (e.Severity() == FATAL)
    exit(e.Code());
}

void Logging::log(ExceptionStack exceptionStack)
{
  for (int i = 0; i < exceptionStack.size(); i++)
  {
    Exception e = exceptionStack.exception(i);
    log(e);
    if (e.Severity() == FATAL)
      exit(e.Code());
  }
}

void Logging::setOutput(ostream *out) { output = out; }

void Logging::addLoggingType(const LogType newLevel)
{
  if (newLevel == INFO)
    loggingLevel[3] = INFO;
  else if (newLevel == WARNING)
    loggingLevel[2] = WARNING;
  else if (newLevel == ERROR)
    loggingLevel[1] = ERROR;
  else if (newLevel == FATAL)
    loggingLevel[0] = FATAL;
}

void Logging::removeLoggingType(const LogType oldType)
{
  if (oldType == INFO)
    loggingLevel[3] = LogType::empty;
  else if (oldType == WARNING)
    loggingLevel[2] = LogType::empty;
  else if (oldType == ERROR)
    loggingLevel[1] = LogType::empty;
  else if (oldType == FATAL)
    loggingLevel[0] = LogType::empty;
}

void Logging::writeTime()
{
  time_t currTime = time(NULL);
  tm timestamp = *localtime(&currTime);
  char logstamp[9];
  strftime(logstamp, 9, "%H:%M:%S", &timestamp);
  *output << "[" << logstamp << "] ";
}

Logging::~Logging()
{
  try
  {
    if (output)
    {
      if (fstream *fileStream = dynamic_cast<fstream *>(output))
        if (fileStream->is_open())
          fileStream->close();
      output = nullptr;
    }
  }
  catch (exception &err)
  {
    cout << err.what();
  }
}
