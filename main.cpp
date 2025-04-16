#include <string>
#include <iostream>
#include "include/cla.h"
#include "include/exception.h"
#include "include/basichandlers.h"
using namespace std;

int main(int argc, char** argv)
{
  Logging logger(4, cout);
  CLA_Handler cla;
  cla.addArgument("-h", help);
  cla.addArgument("--help", help);
  cla.addArgument("-F", setFile, std::vector<void*>{&logger});
  cla.addArgument("--file", setFile, std::vector<void*>{&logger});
  cla.addArgument("-p", logTest, std::vector<void*>{&logger});
  cla.cycleArguments(argc, argv);
  exit(0);
}