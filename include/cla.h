#ifndef CLA_H
#define CLA_H
#include <functional>
#include <map>
#include <string>
#include <vector>

using namespace std;

template <typename result, typename arguments>
using Function = function<result(arguments)>;

template <typename Func, typename internalArg>
Function bindFunction(Func func, internalArg arguments);

// Callback class to store argument function and expected items
class CLA_Callback
{
private:
  Function func;    // The function to call
  size_t itemCount; // The number of items the function is expecting from the command line

public:
  CLA_Callback(Function func, size_t expectedItemCount = 0);

  size_t getItemCount() const;
  void callFunction(const vector<string> &input) const;
};

class CLA_Handler
{
private:
  map<string, CLA_Callback> arguments;
  vector<CLA_Callback> positionals;

  void emplace(const string &argument, const CLA_Callback &input);
  int processArg(int argc, char **argv, int i, const string &arg) const;

public:
  CLA_Handler() = default;

  void addArgument(const string &argument, const Function &func, size_t expectedItemCount = 0);
  void addArgument(const string &argument, const CLA_Callback &input);

  void addPositional(const Function &func, size_t expectedItemCount = 0);
  void addPositional(const CLA_Callback &input);

  /**
   * @brief Parses and processes command-line arguments.
   *
   * @param argc The number of command-line arguments.
   * @param argv The array of command-line arguments.
   */
  void parse(int argc, char **argv) const;
};
#endif
