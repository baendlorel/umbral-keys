#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;

class UmbralKey {
private:
  int count;
  HHOOK keyboardHook;

private:
  int originKey;
  int *umbralKeys;
  int umbralKeysLength;

private:
  void SendKeyCombo();
  LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

public:
  UmbralKey();
};