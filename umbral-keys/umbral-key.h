#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
#include "array.h"

using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
public:
  static map<WORD, UmbralKey *> Instances;
  static HHOOK KeyboardHook;
  static void initKeyboardHook();
  static UmbralKey *add(const char *name, WORD origin_key, WORD *umbral_keys,
                        int umbraSize);

private:
  bool isInited;
  bool isActive;
  string name;
  int count;

private:
  WORD origin;
  Array<WORD> umbras;
  int umbraSize;
  INPUT *umbralInput;
  INPUT *umbralRelease;
  string umbralMessage;

private:
  void init(string &name, WORD origin_key, WORD *umbral_keys,
            int umbraSize);

public:
  void umbral();

private:
  UmbralKey();

public:
  ~UmbralKey();
};