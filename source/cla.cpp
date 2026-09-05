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
 * handler.addArgument("-a", [](const string& value, vector<void*>& data) {
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
#include <utility>

using namespace std;

CLA_Handler::CLA_Handler() {}

void CLA_Handler::addArgument(const string argument, const function<void(const string &, vector<void *> &)> &func, vector<void *> extradata)
{
  string finalArg = argument;
  if (argument[0] == '-')
    finalArg = argument.substr(1);
  if (finalArg[0] == '-')
  {
    string tempArg = finalArg;
    finalArg = tempArg.substr(1);
  }
  if (finalArg.size() == 1)
  {
    auto it = sca.find(finalArg[0]);
    if (it != sca.end())
      throw Exception("Attempted to add argument " + finalArg +
                          " that already exists",
                      "CLA_Handler::addArgument", -1, ERROR);
    sca.insert(make_pair(finalArg[0], make_pair(func, extradata)));
  }
  else
  {
    auto it = mca.find(finalArg);
    if (it != mca.end())
      throw Exception("Attempted to add argument " + finalArg +
                          " that already exists",
                      "CLA_Handler::addArgument", -1, ERROR);
    mca.insert(make_pair(finalArg, make_pair(func, extradata)));
  }
}

void CLA_Handler::addnoargArgument(function<void(const string &, vector<void *> &)> &func, vector<void *> extradata)
{
  if (noargArgument.first)
    throw Exception("Attempted to add noargArgument, but it already exists",
                    "CLA_Handler::addnoargArgument", -1, ERROR);
  noargArgument = make_pair(func, extradata);
}

void CLA_Handler::cycleArguments(const int argc, char **argv)
{
  bool noargArg = false;
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      if (argv[i][1] == '-')
      {
        string slimArg = argv[i] + 2;
        auto it = mca.find(slimArg);
        if (it == mca.end())
          cout << "Unknown argument " << slimArg << endl;
        else
        {
          if (i + 1 < argc)
          {
            if (argv[i + 1][0] == '-')
              it->second.first("", it->second.second);
            else
            {
              it->second.first(argv[i + 1], it->second.second);
              i++;
            }
          }
          else
            it->second.first("", it->second.second);
        }
      }
      else
      {
        auto it = sca.find(argv[i][1]);
        if (it == sca.end())
          cout << "Unknown argument " << argv[i][1] << endl;
        else
        {
          if (i + 1 < argc)
          {
            if (argv[i + 1][0] == '-')
              it->second.first("", it->second.second);
            else
            {
              it->second.first(argv[i + 1], it->second.second);
              i++;
            }
          }
          else
            it->second.first("", it->second.second);
        }
      }
    }
    else if (!noargArg && noargArgument.first)
    {
      noargArgument.first(argv[i], noargArgument.second);
      noargArg = true;
    }
    else
    {
      cout << "Unknown argument " << argv[i] << endl;
    }
  }
}
