#include <SDL2/SDL_events.h>
#include <string>
#include <iostream>
#include "include/cla.h"
#include "include/exception.h"
#include "include/basichandlers.h"
#include "include/window.h"
using namespace std;

int main(int argc, char **argv)
{
  Logging logger(cout);
  CLA_Handler cla;
  cla.addArgument("h", help);
  cla.addArgument("help", help);
  cla.addArgument("f", bindFunction(setFile, 3), 1);
  cla.addArgument("file", bindFunction(setFile, &logger), 1);
  cla.addArgument("p", bindFunction(logTest, &logger));
  cla.addArgument("l", bindFunction(logLevel, &logger), 1);
  cla.addArgument("logging", bindFunction(logLevel, &logger), 1);
  cla.addArgument("P", bindFunction(exceptionTest, &logger));
  cla.parse(argc, argv);
  // Window game("Untitled Reactor Game");
  // game.errorStack()->addPath("game");
  // logger.log(*game.errorStack());
  // game.errorStack()->flushStack();
  // KeyboardEventHandler keyHandler;
  // SDL_Event event;
  // while (game.isRunning(&event))
  // {
  //   while (SDL_PollEvent(&event))
  //   {
  //     cout << event.key.keysym.sym << endl;
  //   }
  //   SDL_Delay(10);
  // }
  exit(0);
}
