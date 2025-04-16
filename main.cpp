#include <cstring>
#include <iostream>
#include "include/exception.h"
#include "include/game.h"
using namespace std;

int main(int argc, char* argv[])
{
  bool logTest = false;
  int logging = 0;
  string fout = "latest.log";
  if (argc == 2 && (!strcmp(argv[1], "help") || !strcmp(argv[1], "?")))
  {
    cout << "Usage: ./SABWR [OPTION...]" << endl
      << endl;
    cout << "-l,--logging LEVEL    What to log. Default is 0. Accepts: ";
    cout << "0 - No Logging" << endl;
    cout << "                                            1 - Fatal Crashes" << endl;
    cout << "                                            2 - Errors and Fatal" << endl;
    cout << "                                            3 - Warnings, Errors, and Fatal" << endl;
    cout << "                                            4 - All Logging" << endl;
    cout << "-f,--file FILE        Where to log to. Default is latest.log." << endl;
    exit(0);
  }
  for (int i = 0; i < argc; i++)
  {
    if (argc >= i + 1 && (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--logging")) && (stoi(argv[i + 1]) >= 0 && stoi(argv[i + 1]) <= 4))
    {
      logging = stoi(argv[i + 1]);
      i++;
    }
    else if (argc >= i + 1 && (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) && strcmp(argv[i + 1], ""))
    {
      fout = argv[i + 1];
      i++;
    }
    else if (!strcmp(argv[i], "-p"))
    {
      logTest = true;
    }
  }
  Logging consoleLog(logging, fout);
  if (logTest)
  {
    consoleLog.log("This was a triumph");
    consoleLog.log("I'm making a note here \"Huge success\"");
    consoleLog.log("It's hard to overstate my satisfaction");
    consoleLog.log("Aperture Science", 1);
    consoleLog.log("We do what we must because we can");
    consoleLog.log("For the good of all of us, except the ones who are dead", 3);
    consoleLog.log("But there's no sense crying over every mistake", 2);
    consoleLog.log("You just keep on trying 'til you run out of cake", 1);
    consoleLog.log("And the science gets done, and you make a neat gun");
    consoleLog.log("For the people who are still alive");
    consoleLog.log("I'm not even angry", 1);
    consoleLog.log("I'm being so sincere right now", 1);
    consoleLog.log("Even though you broke my heart and killed me", 3);
    consoleLog.log("And tore me to pieces", 2);
    consoleLog.log("And threw every piece into a fire", 2);
    consoleLog.log("As they burned, it hurt because I was happy for you", 1);
    consoleLog.log("Now these points of data make a beautiful line");
    consoleLog.log("And we're out of beta, we're releasing on time");
    consoleLog.log("So I'm glad I got burned; think of all the things we learned");
    consoleLog.log("For the people who are still alive");
    consoleLog.log("Go ahead and leave me", 1);
    consoleLog.log("I think I prefer to stay inside");
    consoleLog.log("Maybe you'll find someone else to help you");
    consoleLog.log("Maybe Black Mesa", 1);
    consoleLog.log("That was a joke; ha-ha, fat chance", 3);
    consoleLog.log("Anyway, this cake is great, it's so delicious and moist", 1);
    consoleLog.log("Look at me still talking when there's science to do");
    consoleLog.log("When I look out there, it makes me glad I'm not you");
    consoleLog.log("I've experiments to run, there is research to be done");
    consoleLog.log("On the people who are still alive");
    consoleLog.log("And believe me, I am still alive");
    consoleLog.log("I'm doing science and I'm still alive");
    consoleLog.log("I feel fantastic and I'm still alive");
    consoleLog.log("While you're dying, I'll be still alive", 1);
    consoleLog.log("And when you're dead, I will be still alive", 3);
    consoleLog.log("Still alive");
    consoleLog.log("Still alive");
    exit(12112007);
  }
  exit(0);
}