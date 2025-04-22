#include "../include/exception.h"
#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

Exception::Exception(string message, string path, int code, LogType severity)
{
  this->message = message;
  this->path = path;
  this->code = code;
  this->severity = severity;
}

Exception::~Exception() {}

const string Exception::Message(string *returnval)
{
  if (returnval != nullptr)
    returnval = &message;
  return message;
}

const int Exception::Code(int *returnval)
{
  if (returnval != nullptr)
    returnval = &code;
  return code;
}

const string Exception::Path(string *returnval)
{
  if (returnval != nullptr)
    returnval = &path;
  return path;
}

const LogType Exception::Severity(LogType *returnval)
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

void Logging::writeTime()
{
  time_t currTime = time(NULL);
  tm timestamp = *localtime(&currTime);
  char logstamp[9];
  strftime(logstamp, 9, "%H:%M:%S", &timestamp);
  *output << "[" << logstamp << "] ";
}

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
  output->flush(); // Ensure the output is written immediately
}

void Logging::log(Exception &e)
{
  log("Exception occured at " + e.Path() + " with code " + to_string(e.Code()) + ". Message: " + e.Message(), e.Severity());
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
