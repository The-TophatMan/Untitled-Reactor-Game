#ifndef BASICHANDLERS_H
#define BASICHANDLERS_H
#include "exception.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void help(const vector<string>);

void setFile(Logging &logger, const vector<string> &input);

void logTest(Logging &logger, const vector<string>);

void logLevel(Logging &logger, const vector<string> &input);

void exceptionTest(Logging &logger, const vector<string>);
#endif
