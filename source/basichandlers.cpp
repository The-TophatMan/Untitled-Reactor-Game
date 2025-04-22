#include "../include/basichandlers.h"
#include <string>
#include <vector>

void help(const string&, vector<void*>)
{
  cout << "Usage: ./SABWR [OPTION...]" << endl
    << endl
    << "-l,--logging LEVEL    What to log. Defaults to NONE. Include _AND_ between types to specify multiple logging types. Accepts: INFO, WARNING, ERROR, FATAL, ALL, NONE" << endl
    << "-F,--file    FILE     Where to log to. Defaults to the console." << endl;
}

void logTest(const string&, vector<void*> pass)
{
  Logging* logger = static_cast<Logging*>(pass[0]);
  logger->log("This was a triumph");
  logger->log("I'm making a note here \"Huge success\"");
  logger->log("It's hard to overstate my satisfaction");
  logger->log("Aperture Science", WARNING);
  logger->log("We do what we must because we can");
  logger->log("For the good of all of us, except the ones who are dead", FATAL);
  logger->log("But there's no sense crying over every mistake", ERROR);
  logger->log("You just keep on trying 'til you run out of cake", WARNING);
  logger->log("And the science gets done, and you make a neat gun");
  logger->log("For the people who are still alive");
  logger->log("I'm not even angry", WARNING);
  logger->log("I'm being so sincere right now", WARNING);
  logger->log("Even though you broke my heart and killed me", FATAL);
  logger->log("And tore me to pieces", ERROR);
  logger->log("And threw every piece into a fire", ERROR);
  logger->log("As they burned, it hurt because I was happy for you", WARNING);
  logger->log("Now these points of data make a beautiful line");
  logger->log("And we're out of beta, we're releasing on time");
  logger->log("So I'm glad I got burned; think of all the things we learned");
  logger->log("For the people who are still alive");
  logger->log("Go ahead and leave me", WARNING);
  logger->log("I think I prefer to stay inside");
  logger->log("Maybe you'll find someone else to help you");
  logger->log("Maybe Black Mesa", WARNING);
  logger->log("That was a joke; ha-ha, fat chance", FATAL);
  logger->log("Anyway, this cake is great, it's so delicious and moist", WARNING);
  logger->log("Look at me still talking when there's science to do");
  logger->log("When I look out there, it makes me glad I'm not you");
  logger->log("I've experiments to run, there is research to be done");
  logger->log("On the people who are still alive");
  logger->log("And believe me, I am still alive");
  logger->log("I'm doing science and I'm still alive");
  logger->log("I feel fantastic and I'm still alive");
  logger->log("While you're dying, I'll be still alive", WARNING);
  logger->log("And when you're dead, I will be still alive", FATAL);
  logger->log("Still alive");
  logger->log("Still alive");
  logger->log("Still alive");
  throw Exception("THANK YOU FOR PARTICIPATING IN THIS ENRICHMENT CENTER ACTIVITY!!", "Aperture Science Enrichment Center", 12112007, FATAL);
}

void setFile(const string& output, vector<void*> pass)
{
  static ofstream out; // Ensure the ofstream persists
  out.open(output.c_str(), ios::binary);
  Logging* logger = static_cast<Logging*>(pass[0]);
  logger->setOutput(&out);
}

void logLevel(const string& level, vector<void*> pass)
{
  Logging* logger = static_cast<Logging*>(pass[0]);
  if (level == "NONE")
    return;
  if (level == "ALL") {
    logger->addLoggingType(INFO);
    logger->addLoggingType(WARNING);
    logger->addLoggingType(ERROR);
    logger->addLoggingType(FATAL);
    return;
  }
  string levelString = level;
  for (int i = 0;i < 1023;i++) {
    string type = levelString.substr(0, levelString.find("_AND_"));
    levelString = levelString.substr(levelString.find("_AND_"), levelString.size());
    if (type == "INFO") logger->addLoggingType(INFO);
    else if (type == "WARNING") logger->addLoggingType(WARNING);
    else if (type == "ERROR")logger->addLoggingType(ERROR);
    else if (type == "FATAL")logger->addLoggingType(FATAL);
    if (!levelString.find("_AND_")) break;
  }
}
