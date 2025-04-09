#pragma once
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cstdlib>
#include <format>
#include "Logger.h"
#include "consts.h"
#include "utils.h"

using namespace std;
using namespace std::filesystem;

class Config {
 public:
  static unordered_map<WORD, Array<WORD>> Load();
  static void OpenConfigFile();
};
