#include "../include/basichandlers.h"
#include <vector>

void help(const string &, vector<void *>)
{
  cout << "Usage: ./SABWR [OPTION...]" << endl
       << endl
       << "-l,--logging LEVEL    What to log. Default is 0. Accepts: 0 - No Logging" << endl
       << "                                                          1 - Fatal Crashes" << endl
       << "                                                          2 - Errors and Fatal" << endl
       << "                                                          3 - Warnings, Errors, and Fatal" << endl
       << "                                                          4 - All Logging" << endl
       << "-F,--file    FILE     Where to log to. Default is latest.log." << endl;
}

void logTest(const string &, vector<void *> pass)
{
  Logging *logger = static_cast<Logging *>(pass[0]);
  logger->log("This was a triumph");
  logger->log("I'm making a note here \"Huge success\"");
  logger->log("It's hard to overstate my satisfaction");
  logger->log("Aperture Science", 1);
  logger->log("We do what we must because we can");
  logger->log("For the good of all of us, except the ones who are dead", 3);
  logger->log("But there's no sense crying over every mistake", 2);
  logger->log("You just keep on trying 'til you run out of cake", 1);
  logger->log("And the science gets done, and you make a neat gun");
  logger->log("For the people who are still alive");
  logger->log("I'm not even angry", 1);
  logger->log("I'm being so sincere right now", 1);
  logger->log("Even though you broke my heart and killed me", 3);
  logger->log("And tore me to pieces", 2);
  logger->log("And threw every piece into a fire", 2);
  logger->log("As they burned, it hurt because I was happy for you", 1);
  logger->log("Now these points of data make a beautiful line");
  logger->log("And we're out of beta, we're releasing on time");
  logger->log("So I'm glad I got burned; think of all the things we learned");
  logger->log("For the people who are still alive");
  logger->log("Go ahead and leave me", 1);
  logger->log("I think I prefer to stay inside");
  logger->log("Maybe you'll find someone else to help you");
  logger->log("Maybe Black Mesa", 1);
  logger->log("That was a joke; ha-ha, fat chance", 3);
  logger->log("Anyway, this cake is great, it's so delicious and moist", 1);
  logger->log("Look at me still talking when there's science to do");
  logger->log("When I look out there, it makes me glad I'm not you");
  logger->log("I've experiments to run, there is research to be done");
  logger->log("On the people who are still alive");
  logger->log("And believe me, I am still alive");
  logger->log("I'm doing science and I'm still alive");
  logger->log("I feel fantastic and I'm still alive");
  logger->log("While you're dying, I'll be still alive", 1);
  logger->log("And when you're dead, I will be still alive", 3);
  logger->log("Still alive");
  logger->log("Still alive");
  logger->log("Still alive");
  throw Exception("THANK YOU FOR PARTICIPATING IN THIS ENRICHMETN CENTER ACTIVITY!!", "Aperture Science Enrichment Center", 12112007, 4);
}

void setFile(const string &output, vector<void *> pass)
{
  static ofstream out; // Ensure the ofstream persists
  out.open(output.c_str(), ios::binary);
  Logging *logger = static_cast<Logging *>(pass[0]);
  logger->setOutput(&out);
}

void logLevel(const string &level, vector<void *> pass)
{
  Logging *logger = static_cast<Logging *>(pass[0]);
  logger->updateLoggingLevel(stoi(level));
}
