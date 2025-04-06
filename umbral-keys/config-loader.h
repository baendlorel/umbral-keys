#pragma once
#include "logger.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <format>
#include <unordered_map>
#include <windows.h>

using namespace std;

class Config {
public:
  static unordered_map<string, NativeArray<string>> Load();
};
