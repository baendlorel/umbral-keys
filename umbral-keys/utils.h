#pragma once
#include <Windows.h>
#include <string>
using namespace std;

inline string getKeyName(int keycode) {
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
    if (keycode >= 0x30 && keycode <= 0x39) { // 0-9
      char num = '0' + (keycode - 0x30);
      string result(1, num);
      return result;
    } else if (keycode >= 0x41 && keycode <= 0x5A) { // A-Z
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

string join(string *arr, size_t length, string delimiter = ", ") {
  string result;
  for (size_t i = 0; i < length; i++) {
    result += arr[i];
    if (i < length - 1) {
      result += delimiter; // 添加逗号和空格作为分隔符
    }
  }
  return result;
}