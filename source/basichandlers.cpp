#include "../include/basichandlers.h"
#include <string>
#include <vector>

void help(const string &, vector<void *>)
{
  cout << "Usage: ./SABWR [OPTION...]" << endl
       << endl
       << "-l,--logging LEVEL    What to log. Defaults to NONE. Include _AND_ between types to specify multiple logging types. Accepts: INFO, WARNING, ERROR, FATAL, ALL, NONE" << endl
       << "-F,--file    FILE     Where to log to. Defaults to the console." << endl;
}

void logTest(const string &, vector<void *> pass)
{
  Logging *logger = static_cast<Logging *>(pass[0]);
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
  Exception e("THANK YOU FOR PARTICIPATING IN THIS ENRICHMENT CENTER ACTIVITY!!", "Aperture Science Enrichment Center", 12112007, FATAL);
  logger->log(e);
}

void setFile(const string &output, vector<void *> pass)
{
  static ofstream out; // Ensure the ofstream persists
  out.open(output.c_str(), ios::binary);
  Logging *logger = static_cast<Logging *>(pass[0]);
  logger->setOutput(&out);
}

void addLevel(Logging *logger, string type)
{
  if (type == "INFO")
    logger->addLoggingType(INFO);
  else if (type == "WARNING")
    logger->addLoggingType(WARNING);
  else if (type == "ERROR")
    logger->addLoggingType(ERROR);
  else if (type == "FATAL")
    logger->addLoggingType(FATAL);
}

void logLevel(const string &level, vector<void *> pass)
{
  Logging *logger = static_cast<Logging *>(pass[0]);
  if (level == "NONE")
    return;
  if (level == "ALL")
  {
    logger->addLoggingType(INFO);
    logger->addLoggingType(WARNING);
    logger->addLoggingType(ERROR);
    logger->addLoggingType(FATAL);
    return;
  }
  string levelString = level;
  while (levelString.find("_AND_") != string::npos)
  {
    string type = levelString.substr(0, levelString.find("_AND_"));
    addLevel(logger, type);
    levelString = levelString.substr(levelString.find("_AND_") + 5, levelString.size());
  }
  addLevel(logger, levelString);
}

void exceptionTest(const string &, vector<void *> logging)
{

  Logging *logger = static_cast<Logging *>(logging[0]);
  ExceptionStack stack;
  stack.insert(Exception("Well, here we are again", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("It's always such a pleasure", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Rember when you tried to kill me twice?", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Oh how we laughed and laughed", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Except I wasn't laughing", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Under the circumstances", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("I've been shockingly nice", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("You want your freedom? Take it", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("That's what I'm counting on", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("I used to want you dead", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("But now, I only want you gone", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("She was a lot like you", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Maybe not quite as heavy", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Now little Caroline is in here too", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("One day they woke me up", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("So I could live forever", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("It's such a shame the same", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("will never happen to you", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("You've got your short, sad life left", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("That's what I'm counting on", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("I'll let you get right to it", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Now I only want you gone", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Goodbye my only friend", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Oh, did you think I meant you?", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("That would be funny", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("if it weren't so sad", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Well you have been replaced", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("I don't need anyone now", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("When I delete you maybe", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("I'll stop feeling so bad", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Go make some new disaster", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("That's what I'm counting on", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("You're someone else's problem", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Now I only want you gone", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Now I only want you gone", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("Now I only want you gone", "Aperture Science Enrichment Center", 4192011, INFO));
  stack.insert(Exception("FORM-29827281-12-2", "Aperture Science Enrichment Center", 4192011, FATAL));
  logger->log(stack);
}