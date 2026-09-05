/**
 * @file cla.h
 * @brief Header file for the CLA_Handler class for handling and processing command-line arguments.
 *
 * This file contains the declaration of the CLA_Handler class, which provides functionality to
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
 * - Include this header file in your project.
 * - Create an instance of the CLA_Handler class.
 * - Use `addArgument` to register arguments and their handlers.
 * - Use `addnoargArgument` to register a no-argument handler.
 * - Use `modifyArgument` to update existing arguments or their behaviors.
 * - Use `modifynoargArgument` to update the no-argument handler.
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
 * @note This file uses the `std` namespace globally. Consider avoiding this in larger projects.
 *
 * @author @Ben2445Fun
 * @date 04/23/2025
 * @copyright All Rights Reserved
 */
#ifndef CLA_H
#define CLA_H
#include <functional>
#include <map>
#include <string>
#include <vector>

using namespace std;
using Function = function<void(const vector<void *> &, const vector<string> &)>;

class CLA_Input
{
private:
  Function func;                 // The function to call
  vector<void *> internalInputs; // Any variables the function is expecting defined inside the program
  uint externalInputs;           // The number of CLA_Inputs the function is expecting from the command line

public:
  CLA_Input(const Function func, const vector<void *> internalInputs = {}, const uint externalInputsExpected = 0);

  uint getExternalInputAmount();
  void callFunction(const vector<string> input);
};

class CLA_Handler
{
private:
  map<string, CLA_Input> arguments;
  vector<CLA_Input> inputs;

  void appendArg(const pair<string, CLA_Input> arg);
  int executeArg(const int argc, const char **argv, const int i, const string arg);

public:
  CLA_Handler();
  CLA_Handler(const CLA_Handler &) = default;
  CLA_Handler(CLA_Handler &&) = default;
  CLA_Handler &operator=(const CLA_Handler &) = default;

  void addArgument(const string argument, const Function func, const vector<void *> interalInputs = {}, const uint externalInputsExpected = 0);
  void addArgument(const string argument, const CLA_Input input);
  void addArgument(const pair<string, CLA_Input> argument);

  void addInput(const Function func, const vector<void *> internalInputs = {}, const uint externalInputsExpected = 0);
  void addInput(const CLA_Input input);

  /**
   * @brief Parses and processes command-line arguments.
   *
   * @param argc The number of command-line arguments.
   * @param argv The array of command-line arguments.
   */
  void cycleArguments(const int argc, char **argv);
};
#endif
