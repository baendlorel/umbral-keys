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

unordered_map<WORD, Array<WORD>> LoadConfig();
