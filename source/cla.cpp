#include "../include/cla.h"
#include <stdexcept>

using namespace std;

template <typename Func, typename internalArg>
Function bindFunction(Func func, internalArg arguments)
{
  return [func, arguments](const vector<string> &input)
  {
    func(arguments, input);
  };
}

//* CLA_Callback

CLA_Callback::CLA_Callback(Function func, size_t expectedItemCount)
    : func(func), itemCount(expectedItemCount)
{
  if (!this->func)
    throw invalid_argument("Attempted to create callback with empty function");
}

size_t CLA_Callback::getItemCount() const
{
  return itemCount;
}

void CLA_Callback::callFunction(const vector<string> &input) const
{
  func(input);
}

//* CLA_Handler

void CLA_Handler::emplace(const string &argument, const CLA_Callback &input)
{
  if (argument.empty()) // No empty arguments
    throw invalid_argument("Attempted to add empty command-line argument");
  for (unsigned char c : argument) // Only allow alphanumeric characters
    if (!isalnum(c))
      throw invalid_argument(argument + " contains non-alphanumeric characters");
  if (arguments.find(argument) != arguments.end()) // Check if the argument has already been added
    throw invalid_argument(argument + " already registered");
  arguments.emplace(argument, input);
}

int CLA_Handler::processArg(int argc, char **argv, int i, const string &arg) const
{
  const auto mapping = arguments.find(arg);
  if (mapping == arguments.end()) // Prevent unrecognized arguments
    throw invalid_argument("-" + arg + " is not a recognized argument");
  const CLA_Callback &callback = mapping->second;
  const size_t items = callback.getItemCount(), current = i + items;
  if (current >= argc) // Prevent not enough inputs for argument
    throw out_of_range("Not enough arguments for " + arg);
  vector<string> input;
  for (int o = 1; o <= items; o++)
  {
    input.emplace_back(argv[i + o]);
  }
  callback.callFunction(input);
  return current;
}

void CLA_Handler::addArgument(const string &argument, const Function &func, size_t expectedItemCount)
{
  emplace(argument, CLA_Callback(func, expectedItemCount));
}

void CLA_Handler::addArgument(const string &argument, const CLA_Callback &input)
{
  emplace(argument, input);
}

void CLA_Handler::addPositional(const Function &func, size_t expectedItemCount)
{
  positionals.emplace_back(func, expectedItemCount);
}

void CLA_Handler::addPositional(const CLA_Callback &input)
{
  positionals.push_back(input);
}

void CLA_Handler::parse(int argc, char **argv) const
{
  size_t posCount = 0;
  for (int i = 1; i < argc; i++)
  {
    const string fullArg = argv[i];
    if (fullArg[1] == '-') // Multi-character arguments
      i = processArg(argc, argv, i, fullArg.substr(2));
    else if (fullArg[0] == '-') // Single-character arguments
    {
      const unsigned char last = fullArg[fullArg.size() - 1];
      for (unsigned char c : fullArg.substr(1))
      {
        if (c == last)
          break;
        const string arg(1, c);
        const auto mapping = arguments.find(arg);
        if (mapping == arguments.end()) // Prevent unrecognized arguments
          throw invalid_argument("-" + arg + " is not a recognized argument");
        const CLA_Callback &callback = mapping->second;
        if (callback.getItemCount() > 0)
          throw invalid_argument("-" + arg + " requires input, but is not last argument in list " + fullArg);
        callback.callFunction({});
      }
      i = processArg(argc, argv, i, string(1, last));
    }
    else // Direct inputs
    {
      if (posCount >= positionals.size())
        throw out_of_range("Received more inputs than expected");
      const CLA_Callback &callback = positionals[posCount++];
      const int items = callback.getItemCount();
      if (i + items >= argc) // Prevent not enough inputs for no-arg input
        throw out_of_range("Not enough inputs for input " + to_string(posCount));
      vector<string> input;
      for (int o = 1; o <= items; o++)
        input.emplace_back(argv[i + o]);
      callback.callFunction(input);
      i += items;
    }
  }
}
