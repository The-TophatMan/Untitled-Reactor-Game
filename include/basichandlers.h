#ifndef BASICHANDLERS_H
#define BASICHANDLERS_H
#include "exception.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void help(vector<void *>, vector<string>);

void setFile(vector<void *> log, vector<string> input);

void logTest(vector<void *> log, vector<string> input);

void logLevel(vector<void *> log, vector<string> input);

void exceptionTest(vector<void *> log, vector<string> input);
#endif
