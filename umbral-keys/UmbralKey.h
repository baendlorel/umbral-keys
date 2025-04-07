#pragma once
#include <Windows.h>

#include <iostream>
#include <unordered_map>

#include "Array.h"

typedef const char *chars;
#define CHARS new const char *

using namespace std;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
 public:
  static unordered_map<WORD, UmbralKey *> Instances;
  static HHOOK KeyboardHook;
  static void InitializeKeyboardHook();
  static UmbralKey *Add(chars origin, const Array<chars> &umbras);
  static void ApplyConfig(const unordered_map<WORD, Array<WORD>> &config);
  static wstring ViewUmbras();

  // È«¾Ö½ûÓÃ
  static bool disabledAll;
  static inline void ToggleAll() { disabledAll = !disabledAll; };
  static inline bool GetDisabledAll() { return disabledAll; };

 private:
  bool isInitialized;
  bool disabled;
  size_t count;
  size_t size;
  INPUT *press;
  INPUT *release;
  string message;

 private:
  void Initialize(WORD origin, const Array<WORD> &umbras);

 public:
  void umbral();
  inline void toggle() { disabled = !disabled; };
  inline bool getDisabled() const { return disabled; }

 private:
  UmbralKey();

 public:
  ~UmbralKey();
};