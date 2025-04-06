#pragma once
#include <Windows.h>

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "array.h"

using namespace std;

string getKeyName(WORD keycode);

WORD getKeyCode(const char *keyname);

string join(string *arr, size_t length, string delimiter = ", ");

Array<string> split(const string &input, char delimiter = ',');
