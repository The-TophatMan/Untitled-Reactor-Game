#include <cstring>
#include <iostream>
#include "include/cla.h"
#include "include/exception.h"
#include "include/window.h"
#include "include/basichandlers.h"
using namespace std;

int main(int argc, char* argv[])
{
  Logging logger(4, cout);
  CLA_Handler cla;
  cla.addArgument("-h", help);
  cla.addArgument("--help", help);
  cla.addArgument("-F", setFile, &logger);
  cla.addArgument("--file", setFile, &logger);
  cla.addArgument("-p", logTest, &logger);
  exit(0);
}