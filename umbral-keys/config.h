#pragma once
#include "Logger.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cstdlib>
#include <format>

using namespace std;
using namespace std::filesystem;

class Config {
 public:
  static unordered_map<WORD, Array<WORD>> Load();
  static void OpenConfigFile();
};
