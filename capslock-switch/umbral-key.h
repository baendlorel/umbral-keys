#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
public:
  static map<DWORD, UmbralKey *> instances;
  static HHOOK keyboardHook;
  static void initKeyboardHook();
  static UmbralKey *add(const char *name, int originKey, int *umbralKeys,
                        int umbralKeysLength);

private:
  bool isInited;
  string name;
  int count;

private:
  DWORD originKey;
  int umbralKeysLength;
  INPUT *umbralKeysInput;
  INPUT *umbralKeysRelease;
  string umbralMessage;

private:
  void init(string &name, int originKey, int *umbralKeys, int umbralKeysLength);

public:
  void umbral();

private:
  UmbralKey();

public:
  ~UmbralKey();
};