#pragma once
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;


class Logger {
private:
  static const WCHAR *title;

  static wstring ToDisplay(const WCHAR *from, const WCHAR *message) {
    wstring m;
    if (!wstring(from).empty()) {
      m.append(from);
      m.append(L": ");
    }
    m.append(message);
    return wstring(m.begin(), m.end());
  }

  static string ToConsole(const WCHAR *from, const WCHAR *message) {
    wstring m;
    if (wstring(from).empty()) {
      m.append(L"[UmbralKeys] ");
    } else {
      m.append(L"[UmbralKeys ");
      m.append(from);
      m.append(L"] ");
    }
    m.append(message);

    if (m.empty()) {
      return "";
    }

    int size_needed =
        WideCharToMultiByte(CP_UTF8, 0, m.c_str(), -1, NULL, 0, NULL, NULL);

    string result(size_needed - 1, 0); // 不要最后的 \0
    WideCharToMultiByte(CP_UTF8, 0, m.c_str(), -1, &result[0], size_needed,
                        NULL, NULL);
    return result;
  }

public:
  static void MsgBox(const WCHAR *message) {
    MessageBox(NULL, ToDisplay(L"", message).c_str(), title, MB_OK);
    cout << ToConsole(L"", message) << endl;
  }

  static void MsgBox(const WCHAR *from, const WCHAR *message) {
    MessageBox(NULL, ToDisplay(from, message).c_str(), title, MB_OK);
    cout << ToConsole(from, message) << endl;
  }

  static void Throw(const WCHAR *from, const WCHAR *message) {
    MsgBox(from, message);
    throw runtime_error(ToConsole(from, message));
  }
};

const WCHAR *Logger::title = L"影键 / UmbralKeys";
