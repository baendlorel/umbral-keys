#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
using namespace std;

LRESULT CALLBACK keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam);

class UmbralKey {
public:
  static map<WORD, UmbralKey *> instances;
  static HHOOK keyboard_hook;
  static void init_keyboard_hook();
  static UmbralKey *add(const char *name, WORD origin_key, WORD *umbral_keys,
                        int umbral_keys_size);

private:
  bool is_ready;
  bool is_active;
  string name;
  int count;

private:
  WORD origin_key;
  WORD umbral_keys_size;
  INPUT *umbral_input;
  INPUT *umbral_release;
  string umbral_message;

private:
  void init(string &name, WORD origin_key, WORD *umbral_keys,
            int umbral_keys_size);

public:
  void umbral();

private:
  UmbralKey();

public:
  ~UmbralKey();
};