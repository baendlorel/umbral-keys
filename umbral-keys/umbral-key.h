#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_map>

using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
public:
  static unordered_map<WORD, UmbralKey *> Instances;
  static HHOOK KeyboardHook;
  static void start();
  static UmbralKey *add(const char *name, const char *origin,
                        const char *umbras[],
                        int umbraSize);

private:
  bool isInited;
  bool isActive;
  string name;
  int count;

private:
  WORD origin;
  int umbraSize;
  INPUT *umbralInput;
  INPUT *umbralRelease;
  string umbralMessage;

private:
  void init(string &name, WORD origin, WORD *umbras, int umbraSize);

public:
  void umbral();

private:
  UmbralKey();

public:
  ~UmbralKey();
};