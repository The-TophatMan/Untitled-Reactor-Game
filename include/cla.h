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
 * @note This file uses the `std` namespace globally. Consider avoiding this in larger projects.
 *
 * @author @Ben2445Fun
 * @date 04/23/2025
 * @copyright All Rights Reserved
 */
#ifndef CLA_H
#define CLA_H
#include "exception.h"
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

/**
 * @class CLA_Handler
 * @brief A class for handling command-line arguments.
 *
 * The CLA_Handler class provides functionality to parse and process command-line
 * arguments, including single-character arguments (e.g., `-a`), multi-character
 * arguments (e.g., `--argument`), and non-flag arguments. It allows users to
 * register, modify, and handle arguments with associated callback functions.
 *
 * Features:
 * - Register single-character and multi-character arguments with callback functions.
 * - Modify existing arguments or their associated behaviors.
 * - Handle non-flag arguments with a dedicated callback.
 * - Parse and process command-line arguments based on predefined mappings.
 *
 * Usage:
 * - Use `addArgument` to register arguments and their handlers.
 * - Use `addnoargArgument` to register a no-argument handler.
 * - Use `modifyArgument` to update existing arguments or their behaviors.
 * - Use `modifynoargArgument` to update the no-argument handler.
 * - Use `cycleArguments` to parse and process command-line arguments.
 */
class CLA_Handler
{
private:
  map<char, pair<function<void(const string&, vector<void*>&)>, vector<void*>>> sca = {};   // Single Character Arguments. Prefixed by -
  map<string, pair<function<void(const string&, vector<void*>&)>, vector<void*>>> mca = {}; // Multi-Character Arguments. Prefixed by --
  pair<function<void(const string&, vector<void*>&)>, vector<void*>> noargArgument;

protected:
public:
  CLA_Handler();
  CLA_Handler(const CLA_Handler&) = default;
  CLA_Handler(CLA_Handler&&) = default;
  CLA_Handler& operator=(const CLA_Handler&) = default;
  CLA_Handler& operator=(CLA_Handler&&) = default;

  /**
   * @brief Registers a new argument with its associated callback function.
   *
   * @param argument The argument to register (e.g., "-a" or "--argument").
   * @param func The callback function to execute when the argument is encountered.
   * @param extradata Optional additional data to pass to the callback function.
   *
   * @throws Exception if the argument already exists.
   */
  void addArgument(const string argument, const function<void(const string&, vector<void*>&)>& func, vector<void*> extradata = {});

  /**
   * @brief Registers a no-argument handler.
   *
   * @param func The callback function to execute when a non-flag argument is encountered.
   * @param extradata Optional additional data to pass to the callback function.
   *
   * @throws Exception if a no-argument handler already exists.
   */
  void addnoargArgument(function<void(const string&, vector<void*>&)>& func, vector<void*> extradata = {});

  /**
   * @brief Modifies an existing argument's callback function and/or additional data.
   *
   * @param argument The argument to modify (e.g., "-a" or "--argument").
   * @param newFunc The new callback function to associate with the argument (optional).
   * @param extradata The new additional data to associate with the argument (optional).
   *
   * @throws Exception if the argument does not exist.
   */
  void modifyArgument(const string argument, const function<void(const string&, vector<void*>&)>& newFunc = NULL, const vector<void*> extradata = {});

  /**
   * @brief Renames an existing argument to a new argument name.
   *
   * @param oldArg The current name of the argument.
   * @param newArg The new name for the argument.
   *
   * @throws Exception if the old argument does not exist or the new argument already exists.
   */
  void modifyArgument(const string oldArg, const string newArg);

  /**
   * @brief Updates the no-argument handler.
   *
   * @param func The new callback function to associate with the no-argument handler.
   */
  void modifynoargArgument(function<void(const string&, vector<void*>&)>& func);

  /**
   * @brief Parses and processes command-line arguments.
   *
   * @param argc The number of command-line arguments.
   * @param argv The array of command-line arguments.
   */
  void cycleArguments(const int argc, char** argv);
};
#endif