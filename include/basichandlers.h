#ifndef BASICHANDLERS_H
#define BASICHANDLERS_H
#include "exception.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

/**
 * @brief Displays the usage information and available command-line options for the program.
 *
 * This function prints a help message to the standard output, detailing the available
 * options for running the program, including logging levels and log file configuration.
 *
 * Usage:
 *   ./SABWR [OPTION...]
 *
 * Options:
 *   -l, --logging LEVEL    Specifies the logging level. Default is 0. Acceptable values:
 *                          0 - No Logging
 *                          1 - Fatal Crashes
 *                          2 - Errors and Fatal
 *                          3 - Warnings, Errors, and Fatal
 *                          4 - All Logging
 *   -f, --file FILE        Specifies the log file. Default is "latest.log".
 */
void help(const string &, vector<void *>);

/**
 * @brief Sets the output file for logging and configures the logger.
 *
 * This function opens a binary output file stream for the specified file path
 * and sets it as the output stream for the logger. The logger is retrieved
 * from the provided vector of pointers.
 *
 * @param output The file path to the output file as a string.
 * @param pass A vector of void pointers, where the first element is expected
 *             to be a pointer to a Logging object.
 *
 * @note The caller must ensure that the first element of the `pass` vector
 *       is a valid pointer to a Logging object. Undefined behavior may occur
 *       otherwise.
 */
void setFile(const string &output, vector<void *> pass);

/**
 * @brief Logs a series of messages, primarily lyrics from the song "Still Alive" by Jonathan Coulton,
 *        used in the game Portal, to a logging system.
 *
 * @param message A placeholder string parameter (not used in the function).
 * @param pass A vector of void pointers, where the first element is expected to be a pointer to a Logging object.
 *
 * @details This function demonstrates logging functionality by outputting a sequence of predefined messages
 *          to the provided Logging object. The messages include various log levels (e.g., 1, 2, 3) to simulate
 *          different levels of importance or categorization. The function ends by exiting the program with
 *          a specific exit code (12112007).
 *
 * @note The function assumes that the first element of the `pass` vector is a valid Logging object.
 *       Passing an invalid or null pointer will result in undefined behavior.
 *
 * @exitcode 12112007 The program exits with this code after logging all messages.
 */
void logTest(const string &, vector<void *> pass);

/**
 * @brief Updates the logging level of the logger instance.
 *
 * This function takes a logging level as a string and updates the logging
 * level of the logger instance provided in the `pass` vector.
 *
 * @param level A string representing the desired logging level. It is expected
 *              to be convertible to an integer using `std::stoi`.
 * @param pass  A vector of void pointers, where the first element is expected
 *              to be a pointer to a `Logging` instance.
 *
 * @note The function assumes that `pass[0]` is a valid pointer to a `Logging`
 *       object and that `level` can be successfully converted to an integer.
 *       Undefined behavior may occur if these assumptions are violated.
 */
void logLevel(const string &level, vector<void *> pass);
#endif