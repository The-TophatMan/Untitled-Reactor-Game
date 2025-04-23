#include <string>
#include <iostream>
#include "include/cla.h"
#include "include/exception.h"
#include "include/basichandlers.h"
#include "include/window.h"
using namespace std;

int main(int argc, char** argv)
{
  Logging logger(cout);
  CLA_Handler cla;
  cla.addArgument("-h", help);
  cla.addArgument("--help", help);
  cla.addArgument("-F", setFile, vector<void*>{&logger});
  cla.addArgument("--file", setFile, vector<void*>{&logger});
  cla.addArgument("-p", logTest, vector<void*>{&logger});
  cla.addArgument("-l", logLevel, vector<void*>{&logger});
  cla.addArgument("--logging", logLevel, vector<void*>{&logger});
  cla.cycleArguments(argc, argv);
  Window game("Untitled Reactor Game");
  vector<Exception> gameStack = game.errorStack();
  game.flushErrorStack();
  addPath(&gameStack, "game");
  logger.log(gameStack);
  KeyboardEventHandler keyHandler;
  SDL_Event event;
  while (game.isRunning(&event))
  {
    keyHandler.cycleKeyboardInputs(&event);
    SDL_Delay(10);
  }
  exit(0);
}