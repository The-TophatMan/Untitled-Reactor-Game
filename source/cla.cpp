#include "../include/cla.h"
#include <utility>

using namespace std;

CLA_Handler::CLA_Handler() {}

void CLA_Handler::addArgument(const string argument, const function<void(const string&, vector<void*>&)>& func, vector<void*> extradata)
{
  string finalArg = argument;
  if (argument[0] == '-')
    finalArg = argument.substr(1);
  if (finalArg[0] == '-')
  {
    string tempArg = finalArg;
    finalArg = tempArg.substr(1);
  }
  if (finalArg.size() == 1)
  {
    auto it = sca.find(finalArg[0]);
    if (it != sca.end())
      throw Exception("Attempted to add argument " + finalArg +
        " that already exists",
        "CLA_Handler::addArgument", -1, ERROR);
    sca.insert(make_pair(finalArg[0], make_pair(func, extradata)));
  }
  else
  {
    auto it = mca.find(finalArg);
    if (it != mca.end())
      throw Exception("Attempted to add argument " + finalArg +
        " that already exists",
        "CLA_Handler::addArgument", -1, ERROR);
    mca.insert(make_pair(finalArg, make_pair(func, extradata)));
  }
}

void CLA_Handler::addnoargArgument(function<void(const string&, vector<void*>&)>& func, vector<void*> extradata)
{
  if (noargArgument.first)
    throw Exception("Attempted to add noargArgument, but it already exists",
      "CLA_Handler::addnoargArgument", -1, ERROR);
  noargArgument = make_pair(func, extradata);
}

void CLA_Handler::modifyArgument(const string argument, const function<void(const string&, vector<void*>&)>& newFunc, const vector<void*> extradata)
{
  string finalArg = argument;
  if (argument[0] == '-')
    finalArg = argument.substr(1);
  if (finalArg[0] == '-')
  {
    string tempArg = finalArg;
    finalArg = tempArg.substr(1);
  }
  if (finalArg.size() == 1)
  {
    auto it = sca.find(finalArg[0]);
    if (it == sca.end())
      throw Exception("Could not modify argument " + finalArg + ", argument does not exist", "CLA_Handler::modifyArgument.1", -1, ERROR);
    if (newFunc != NULL)
      it->second.first = newFunc;
    if (!extradata.empty())
      it->second.second = extradata;
  }
  else
  {
    auto it = mca.find(finalArg);
    if (it == mca.end())
      throw Exception("Could not modify argument" + finalArg +
        ", argument does not exist",
        "CLA_Handler::modifyArgument.1", -1, ERROR);
    if (newFunc != NULL)
      it->second.first = newFunc;
    if (!extradata.empty())
      it->second.second = extradata;
  }
}

void CLA_Handler::modifyArgument(const string oldArg, const string newArg)
{
  string finaloldArg = oldArg, finalnewArg = newArg;
  if (oldArg[0] == '-')
    finaloldArg = oldArg.substr(1);
  if (finaloldArg[0] == '-')
  {
    string tempArg = finaloldArg;
    finaloldArg = tempArg.substr(1);
  }
  if (newArg[0] == '-')
    finalnewArg = newArg.substr(1);
  if (finalnewArg[0] == '-')
  {
    string tempArg = finalnewArg;
    finalnewArg = tempArg.substr(1);
  }
  if (finaloldArg.size() == 1)
  {
    auto it = sca.find(finaloldArg[0]);
    if (it == sca.end())
      throw Exception("Could not modify argument " + finaloldArg +
        ", argument does not exist",
        "CLA_Handler::modifyArgument.2", -1, ERROR);
    pair<function<void(const string&, vector<void*>&)>, vector<void*>> func = it->second; // Save the associated function
    if (finalnewArg.size() != 1)
    {
      auto dup = mca.find(finalnewArg);
      if (dup != mca.end())
        throw Exception("Could not modify argument " + finaloldArg +
          ", new argument already exists",
          "CLA_Handler::modifyArgument.2", -1, ERROR);
      mca.insert(make_pair(finalnewArg, func));
    }
    sca.erase(it); // Remove the old key-value pair
    sca.insert(
      make_pair(finalnewArg[0], func)); // Insert the new key-value pair
  }
  else
  {
    auto it = mca.find(finaloldArg);
    if (it == mca.end())
      throw Exception("Could not modify argument " + finaloldArg +
        ", argument does not exist",
        "CLA_Handler::modifyArgument.2", -1, ERROR);
    pair<function<void(const string&, vector<void*>&)>, vector<void*>> func = it->second;
    if (finalnewArg.size() == 1)
    {
      auto dup = sca.find(finalnewArg[0]);
      if (dup != sca.end())
        throw Exception("Could not modify argument " + finaloldArg +
          ", new argument already exists",
          "CLA_Handler::modifyArgument.2", -1, ERROR);
      sca.insert(make_pair(finalnewArg[0], func));
    }
    mca.erase(it);
    mca.insert(make_pair(finalnewArg, func));
  }
}

void CLA_Handler::cycleArguments(const int argc, char** argv)
{
  bool noargArg = false;
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      if (argv[i][1] == '-')
      {
        string slimArg = argv[i] + 2;
        auto it = mca.find(slimArg);
        if (it == mca.end())
          cout << "Unknown argument " << slimArg << endl;
        else
        {
          if (i + 1 < argc)
          {
            if (argv[i + 1][0] == '-')
              it->second.first("", it->second.second);
            else
            {
              it->second.first(argv[i + 1], it->second.second);
              i++;
            }
          }
          else
            it->second.first("", it->second.second);
        }
      }
      else
      {
        auto it = sca.find(argv[i][1]);
        if (it == sca.end())
          cout << "Unknown argument " << argv[i][1] << endl;
        else
        {
          if (i + 1 < argc)
          {
            if (argv[i + 1][0] == '-')
              it->second.first("", it->second.second);
            else
            {
              it->second.first(argv[i + 1], it->second.second);
              i++;
            }
          }
          else
            it->second.first("", it->second.second);
        }
      }
    }
    else if (!noargArg && noargArgument.first)
    {
      noargArgument.first(argv[i], noargArgument.second);
      noargArg = true;
    }
    else
    {
      cout << "Unknown argument " << argv[i] << endl;
    }
  }
}

void CLA_Handler::modifynoargArgument(function<void(const string&, vector<void*>&)>& func)
{
  noargArgument.first = func;
}
