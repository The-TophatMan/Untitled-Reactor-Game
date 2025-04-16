#include <string>
#include <iostream>
#include "include/cla.h"
#include "include/exception.h"
#include "include/basichandlers.h"
using namespace std;

int main(int argc, char **argv)
{
  try
  {
    Logging logger(4, cout);
    CLA_Handler cla;
    cla.addArgument("-h", help);
    cla.addArgument("--help", help);
    cla.addArgument("-F", setFile, vector<void *>{&logger});
    cla.addArgument("--file", setFile, vector<void *>{&logger});
    cla.addArgument("-p", logTest, vector<void *>{&logger});
    cla.addArgument("-l", logLevel, vector<void *>{&logger});
    cla.cycleArguments(argc, argv);
  }
  catch (Exception &err)
  {
    cout << "Caught exception: " << err.getMessage() << " at " << err.getPath();
  }
  exit(0);
}