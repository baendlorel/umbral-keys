#pragma once
#include <Windows.h>

#include <iostream>
#include <unordered_map>
#include "array.h"

typedef const char *chars;
#define CHARS new const char*

using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
 public:
  static unordered_map<WORD, UmbralKey *> Instances;
  static HHOOK KeyboardHook;
  static void start();
  static UmbralKey *Add(chars origin, const Array<chars> &umbras);
  static void ApplyConfig(const unordered_map<WORD, Array<WORD>>& config);

 private:
  bool isInitialized;
  size_t count;
  size_t size;
  INPUT *press;
  INPUT *release;
  string message;

 private:
  void Initialize(WORD origin, const Array<WORD> &umbras);

 public:
  void umbral();

 private:
  UmbralKey();

 public:
  ~UmbralKey();
};