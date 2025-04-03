#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
private:
  map<int, UmbralKey *> instances;
  static UmbralKey *instances[];
  static int instanceLength;
  static int instanceIndex;

public:
  static HHOOK keyboardHook;
  static void initKeyboardHook();
  static UmbralKey *find(int originKey);
  static UmbralKey *add(string &name, int originKey, int *umbralKeys,
                           int umbralKeysLength);

private:
  string name;
  int count;

private:
  int originKey;
  string originKeyName;
  int umbralKeysLength;
  INPUT *umbralKeysInput;
  INPUT *umbralKeysRelease;
  string *umbralKeysName;

private:
  void init(string& name, int originKey, int *umbralKeys, int umbralKeysLength);

public:
  void umbral();

private:
  UmbralKey();

public:
  ~UmbralKey();
};