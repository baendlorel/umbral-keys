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
  static UmbralKey *add(chars origin, const Array<chars>& umbras);

 private:
  bool isInited;
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