#include "../include/cla.h"
#include <stdexcept>

using namespace std;

//* CLA_Callback

CLA_Callback::CLA_Callback(Function func, vector<void *> internalInputs, size_t externalInputsExpected)
    : func(func), internalInputs(internalInputs), externalInputs(externalInputsExpected) {}

size_t CLA_Callback::getExternalInputAmount() const
{
  return externalInputs;
}

void CLA_Callback::callFunction(const vector<string> input) const
{
  func(internalInputs, input);
}

//* CLA_Handler

CLA_Handler::CLA_Handler() {}

void CLA_Handler::appendArg(const string argument, const CLA_Callback input)
{
  if (argument == "")
    throw invalid_argument("Attempted to add empty command-line argument");
  for (int i = 0; i < argument.size(); i++) // Only allow alphanumeric characters
    if (!isalnum(argument[i]))
      throw invalid_argument(argument + " contains non-alphanumeric characters");
  if (arguments.find(argument) != arguments.end()) // Check if the argument has already been added
    throw invalid_argument(argument + " already registered");
  arguments.emplace(argument, input);
}

int CLA_Handler::executeArg(const int argc, char **argv, const int i, const string arg) const
{
  auto mapping = arguments.find(arg);
  if (mapping == arguments.end()) // Prevent unrecognized arguments
    throw invalid_argument("-" + arg + " is not a recognized argument");
  CLA_Callback func = mapping->second;
  if (i + func.getExternalInputAmount() >= argc) // Prevent not enough inputs for argument
    throw out_of_range("Not enough arguments for " + arg);
  vector<string> input;
  for (int o = 1; o < func.getExternalInputAmount() + 1; o++)
  {
    if (argv[i + o][0] == '-') // Prevent reading in argument as input
      throw out_of_range(arg + " ran into another argument before finding all required inputs");
    input.push_back(argv[i + o]);
  }
  func.callFunction(input);
  return i + func.getExternalInputAmount();
}

void CLA_Handler::addArgument(const string argument, const Function func, const vector<void *> interalInputs, const size_t externalInputsExpected)
{
  appendArg(argument, CLA_Callback(func, interalInputs, externalInputsExpected));
}

void CLA_Handler::addArgument(const string argument, const CLA_Callback input)
{
  appendArg(argument, input);
}

void CLA_Handler::addInput(const Function func, const vector<void *> internalInputs, const size_t externalInputsExpected)
{
  this->inputs.push_back(CLA_Callback(func, internalInputs, externalInputsExpected));
}

void CLA_Handler::addInput(const CLA_Callback input)
{
  inputs.push_back(input);
}

void CLA_Handler::cycleArguments(const int argc, char **argv) const
{
  size_t noarg = 0;
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][1] == '-') // Multi-character arguments
    {
      const string arg = argv[i] + 2;
      i = executeArg(argc, argv, i, arg);
    }
    else if (argv[i][0] == '-') // Single-character arguments
    {
      const int lastelem = string(argv[i]).size() - 1;
      for (int o = 1; o < lastelem; o++)
      {
        const string arg(1, argv[i][o]);
        const auto mapping = arguments.find(arg);
        if (mapping == arguments.end()) // Prevent unrecognized arguments
          throw invalid_argument("-" + arg + " is not a recognized argument");
        CLA_Callback func = mapping->second;
        if (func.getExternalInputAmount() > 0)
          throw invalid_argument("-" + arg + " requires input, but is not last argument in list " + argv[i]);
        func.callFunction({});
      }
      i = executeArg(argc, argv, i, string(1, argv[i][lastelem]));
    }
    else // Direct inputs
    {
      if (noarg >= inputs.size())
        throw out_of_range("Received more inputs than expected");
      CLA_Callback func = inputs[noarg];
      noarg++;
      if (i + func.getExternalInputAmount() >= argc) // Prevent not enough inputs for no-arg input
        throw out_of_range("Not enough inputs for input " + to_string(noarg));
      vector<string> input;
      for (int o = 1; o < func.getExternalInputAmount() + 1; o++)
        input.push_back(argv[i + o]);
      func.callFunction(input);
      i = i + func.getExternalInputAmount();
    }
  }
}
