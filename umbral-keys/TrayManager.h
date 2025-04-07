#pragma once
#include <windows.h>
#include "I18N.h"

enum MenuItem { EDIT_CONFIG = 1, ENABLE, ABOUT, EXIT };

class TrayManager {
 public:
  void init(HWND hwnd);
  void cleanup();
  void showMenu();

 private:
  NOTIFYICONDATA nid = {};
  HWND hWnd = nullptr;
};
