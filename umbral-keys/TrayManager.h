#pragma once
#include <windows.h>

#include "resource.h"
#include "I18N.h"
#include "UmbralKey.h"

enum MenuItem { VIEW_CONFIG = 1, EDIT_CONFIG, RELOAD, ENABLE, ABOUT, EXIT };

class TrayManager {
 public:
  void init(HWND hwnd);
  void cleanup();
  void showMenu();
  inline HMENU getMenu() { return menu; }

 private:
  NOTIFYICONDATA nid = {};
  HWND hWnd = nullptr;
  HMENU menu = nullptr;
};
