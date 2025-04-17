#include <string>
#include <iostream>
#include "include/cla.h"
#include "include/exception.h"
#include "include/basichandlers.h"
#include "include/window.h"
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
    cla.addArgument("--logging", logLevel, vector<void *>{&logger});
    cla.cycleArguments(argc, argv);
    Window game("Semi-Accurate Boiling Water Reactor", 1920, 1080);
    SDL_Event event;
    while (game.isRunning())
    {
      game.cycleKeyboardInputs(&event);
      SDL_Delay(10);
    }
  }
  catch (Exception &err)
  {
    // Add proper error handling here
  }
  exit(0);
}