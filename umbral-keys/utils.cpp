#pragma once
#include "utils.h"

string getKeyName(WORD keycode) {
  switch (keycode) {
    case VK_BACK:
      return "Backspace";
    case VK_TAB:
      return "Tab";
    case VK_RETURN:
      return "Enter";
    case VK_SHIFT:
      return "Shift";
    case VK_CONTROL:
      return "Ctrl";
    case VK_MENU:
      return "Alt";
    case VK_PAUSE:
      return "Pause";
    case VK_CAPITAL:
      return "CapsLock";
    case VK_ESCAPE:
      return "Escape";
    case VK_SPACE:
      return "Space";
    case VK_PRIOR:
      return "Page Up";
    case VK_NEXT:
      return "Page Down";
    case VK_END:
      return "End";
    case VK_HOME:
      return "Home";
    case VK_LEFT:
      return "Left Arrow";
    case VK_UP:
      return "Up Arrow";
    case VK_RIGHT:
      return "Right Arrow";
    case VK_DOWN:
      return "Down Arrow";
    case VK_INSERT:
      return "Insert";
    case VK_DELETE:
      return "Delete";
    // Function keys
    case VK_F1:
      return "F1";
    case VK_F2:
      return "F2";
    case VK_F3:
      return "F3";
    case VK_F4:
      return "F4";
    case VK_F5:
      return "F5";
    case VK_F6:
      return "F6";
    case VK_F7:
      return "F7";
    case VK_F8:
      return "F8";
    case VK_F9:
      return "F9";
    case VK_F10:
      return "F10";
    case VK_F11:
      return "F11";
    case VK_F12:
      return "F12";
    // Numeric keys
    default:
      if (keycode >= 0x30 && keycode <= 0x39) {  // 0-9
        char num = '0' + (keycode - 0x30);
        string result(1, num);
        return result;
      } else if (keycode >= 0x41 && keycode <= 0x5A) {  // A-Z
        char letter = 'A' + (keycode - 0x41);
        string result(1, letter);
        return result;
      } else {
        // For other keys, just convert to string
        char buffer[16];
        sprintf_s(buffer, "%d", keycode);
        return string(buffer);
      }
  }
}

WORD getKeyCode(const char *keyname) {
  string kn = keyname;
  for (char &c : kn) {
    c = tolower(c);
  }

  if (kn.size() == 1) {
    char c = kn[0];
    if (c >= 'a' && c <= 'z') {
      return static_cast<WORD>(toupper(c));
    }
    if (c >= '0' && c <= '9') {
      return static_cast<WORD>(c);
    }
  }

  static const unordered_map<string, WORD> keyMap = {
      {"enter", VK_RETURN},   {"esc", VK_ESCAPE},
      {"space", VK_SPACE},    {"tab", VK_TAB},
      {"shift", VK_SHIFT},    {"ctrl", VK_CONTROL},
      {"alt", VK_MENU},       {"capslock", VK_CAPITAL},
      {"backspace", VK_BACK}, {"delete", VK_DELETE},
      {"insert", VK_INSERT},  {"home", VK_HOME},
      {"end", VK_END},        {"pageup", VK_PRIOR},
      {"pagedown", VK_NEXT},

      {"left", VK_LEFT},      {"right", VK_RIGHT},
      {"up", VK_UP},          {"down", VK_DOWN},

      {"f1", VK_F1},          {"f2", VK_F2},
      {"f3", VK_F3},          {"f4", VK_F4},
      {"f5", VK_F5},          {"f6", VK_F6},
      {"f7", VK_F7},          {"f8", VK_F8},
      {"f9", VK_F9},          {"f10", VK_F10},
      {"f11", VK_F11},        {"f12", VK_F12},
  };

  auto it = keyMap.find(kn);
  if (it != keyMap.end()) {
    return it->second;
  } else {
    return 0;  // 0 表示未找到
  }
}

string join(string *arr, size_t length, string delimiter) {
  string result;
  for (size_t i = 0; i < length; i++) {
    result.append(arr[i]);
    if (i < length - 1) {
      result.append(delimiter);  // 添加逗号和空格作为分隔符
    }
  }
  return result;
}

Array<string> split(const string &input, char delimiter) {
  stringstream ss(input);
  string item;
  Array<string> arr;
  while (getline(ss, item, delimiter)) {
    string _item = item;
    arr.push(_item);
  }
  return arr;
}

wstring stow(const string &str) {
  int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
  wstring wstr(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
  wstr.pop_back();  // 移除额外的 null 字符
  return wstr;
}

void centerWindow(HWND hwnd) {
  RECT rc;
  GetWindowRect(hwnd, &rc);

  int winWidth = rc.right - rc.left;
  int winHeight = rc.bottom - rc.top;

  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  int x = (screenWidth - winWidth) / 2;
  int y = (screenHeight - winHeight) / 2;

  MoveWindow(hwnd, x, y, winWidth, winHeight, TRUE);
}
