#pragma once
#include "logger.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cstdlib>

using namespace std;
using namespace std::filesystem;

unordered_map<string, Array<string>> LoadConfig();
