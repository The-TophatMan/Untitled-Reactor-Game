#ifndef CLA_H
#define CLA_H
#include <functional>
#include <map>
#include <string>
#include <vector>

using namespace std;
using Function = function<void(const vector<void *> &, const vector<string> &)>;

class CLA_Callback
{
private:
  Function func;                 // The function to call
  vector<void *> internalInputs; // Any variables the function is expecting defined inside the program
  size_t externalInputs;         // The number of CLA_Callbacks the function is expecting from the command line

public:
  CLA_Callback(const Function func, const vector<void *> internalInputs = {}, const size_t externalInputsExpected = 0);

  size_t getExternalInputAmount() const;
  void callFunction(const vector<string> input) const;
};

class CLA_Handler
{
private:
  map<string, CLA_Callback> arguments;
  vector<CLA_Callback> positionals;

  void emplace(const string argument, const CLA_Callback input);
  int processArg(const int argc, char **argv, const int i, const string arg) const;

public:
  CLA_Handler() = default;

  void addArgument(const string argument, const Function func, const vector<void *> interalInputs = {}, const size_t externalInputsExpected = 0);
  void addArgument(const string argument, const CLA_Callback input);

  void addPositional(const Function func, const vector<void *> internalInputs = {}, const size_t externalInputsExpected = 0);
  void addPositional(const CLA_Callback input);

  /**
   * @brief Parses and processes command-line arguments.
   *
   * @param argc The number of command-line arguments.
   * @param argv The array of command-line arguments.
   */
  void parse(const int argc, char **argv) const;
};
#endif
