/**
 * @file cla.cpp
 * @brief Implementation of the CLA_Handler class for handling and processing command-line arguments.
 *
 * This file contains the implementation of the CLA_Handler class, which provides functionality to
 * register, modify, and handle command-line arguments with associated callback functions. It supports
 * single-character arguments, multi-character arguments, and non-flag arguments. The class also includes
 * methods for parsing and processing command-line arguments.
 *
 * Features:
 * - Register single-character and multi-character arguments with callback functions.
 * - Modify existing arguments or their associated behaviors.
 * - Handle non-flag arguments with a dedicated callback.
 * - Parse and process command-line arguments based on predefined mappings.
 *
 * Usage:
 * - Include the corresponding header file in your project.
 * - Create an instance of the CLA_Handler class.
 * - Use `addArgument` to register arguments and their handlers.
 * - Use `modifyArgument` to update existing arguments or their behaviors.
 * - Use `cycleArguments` to parse and process command-line arguments.
 *
 * Example:
 * @code
 * #include "cla.h"
 * CLA_Handler handler;
 * handler.addArgument("-a", [](const string& value, vector<string>& data) {
 *     // Handle argument logic here
 * });
 * handler.cycleArguments(argc, argv);
 * @endcode
 *
 * Dependencies:
 * - exception.h
 * - Standard C++ libraries: <functional>, <map>, <string>, <utility>, <vector>, <iostream>
 *
 * @author @Ben2445Fun
 * @date 04/23/2025
 * @copyright All Rights Reserved
 */
#include "../include/cla.h"
#include <stdexcept>

using namespace std;

//* CLA_Input

CLA_Input::CLA_Input(const Function func, const vector<void *> internalInputs = {}, const uint externalInputsExpected = 0)
{
  this->func = func;
  this->internalInputs = internalInputs;
  externalInputs = externalInputsExpected;
}

uint CLA_Input::getExternalInputAmount()
{
  return externalInputs;
}

void CLA_Input::callFunction(const vector<string> input)
{
  func(internalInputs, input);
}

//* CLA_Handler

CLA_Handler::CLA_Handler() {}

void CLA_Handler::appendArg(const pair<string, CLA_Input> arg)
{
  for (int i = 0; i < arg.first.size(); i++) // Only allow alphanumeric characters
    if (isalnum(arg.first[i]))
      throw invalid_argument(arg.first + " contains non-alphanumeric characters");
  if (arguments.find(arg.first) != arguments.end()) // Check if the argument has already been added
    throw invalid_argument(arg.first + " already registered");
  arguments.insert(arg);
}

int CLA_Handler::executeArg(const int argc, const char **argv, const int i, const string arg)
{
  if (arguments.find(arg) == arguments.end()) // Prevent unrecognized arguments
    throw invalid_argument(arg + " is not a recognized argument");
  CLA_Input func = arguments.at(arg);
  if (i + func.getExternalInputAmount() >= argc) // Prevent not enough inputs for argument
    throw out_of_range("Not enough arguments for " + arg);
  vector<string> input;
  for (int o = 0; o < func.getExternalInputAmount(); o++)
  {
    if (argv[i + o][0] == '-') // Prevent reading in argument as input
      throw out_of_range(arg + " ran into another argument before finding all required inputs");
    input.push_back(argv[i + o]);
  }
  func.callFunction(input);
  return i + func.getExternalInputAmount();
}

void CLA_Handler::addArgument(const string argument, const Function func, const vector<void *> interalInputs, const uint externalInputsExpected)
{
  appendArg(pair<string, CLA_Input>(argument, CLA_Input(func, interalInputs, externalInputsExpected)));
}

void CLA_Handler::addArgument(const string argument, const CLA_Input input)
{
  appendArg(pair<string, CLA_Input>(argument, input));
}

void CLA_Handler::addArgument(const pair<string, CLA_Input> argument)
{
  appendArg(argument);
}

void CLA_Handler::addInput(const Function func, const vector<void *> internalInputs, const uint externalInputsExpected)
{
  this->inputs.push_back(CLA_Input(func, internalInputs, externalInputsExpected));
}

void CLA_Handler::addInput(const CLA_Input input)
{
  inputs.push_back(input);
}

void CLA_Handler::cycleArguments(const int argc, char **argv)
{
  uint noarg = 0;
  for (int i = 0; i < argc; i++)
  {
    if (argv[i][1] == '-') // Multi-character arguments
    {
      const string arg = argv[i] + 2;
      i = executeArg(argc, argv, i, arg);
    }
    else if (argv[i][0] == '-') // Single-character arguments
    {
      const int lastelem = sizeof(argv[i]) - 1;
      for (int o = 1; o < lastelem; o++)
      {
        const string arg = to_string(argv[i][o]);
        if (arguments.find(arg) == arguments.end()) // Prevent unrecognized arguments
          throw invalid_argument("-" + arg + " is not a recognized argument");
        CLA_Input func = arguments.at(arg);
        if (func.getExternalInputAmount() > 0)
          throw invalid_argument("-" + arg + " requires input, but is not last argument in list " + argv[i]);
        func.callFunction(vector<string>());
      }
      i = executeArg(argc, argv, i, to_string(argv[i][lastelem]));
    }
    else // Direct inputs
    {
      CLA_Input func = inputs.front();
      noarg++;
      if (func.getExternalInputAmount() >= argc) // Prevent not enough inputs for no-arg input
        throw out_of_range("Not enough inputs for input " + noarg);
      vector<string> input;
      for (int o = 0; o < func.getExternalInputAmount(); o++)
        input.push_back(argv[i + o]);
      func.callFunction(input);
      i = i + func.getExternalInputAmount();
    }
  }
}
