#pragma once
#include <Windows.h>

#include <iostream>
#include <unordered_map>

typedef const char *chars;
#define CHARS new const char*

using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
 public:
  static unordered_map<WORD, UmbralKey *> Instances;
  static HHOOK KeyboardHook;
  static void start();
  static UmbralKey *add(chars origin, chars umbras[], int size);

 private:
  bool isInited;
  int count;
  int size;
  INPUT *press;
  INPUT *release;
  string message;

 private:
  void Initialize(WORD origin, WORD *umbras, int size);

 public:
  void umbral();

 private:
  UmbralKey();

 public:
  ~UmbralKey();
};