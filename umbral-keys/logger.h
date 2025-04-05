#pragma once
#include "i18n.h"
#include <iostream>
#include <string>
#include <chrono>   // 用于获取时间
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

    string head = getCurrentTimestamp();
    return head.append(" ").append(result);
  }

  static string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(
                  now.time_since_epoch()) %
              1000;
    auto timer = chrono::system_clock::to_time_t(now);

    tm bt;
    localtime_s(&bt, &timer); // Windows

    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &bt);
    return string(buf) + "." + to_string(ms.count()).substr(0, 3);
  }

public:
  static void MsgBox(const WCHAR *message) {
    MessageBox(NULL, ToDisplay(L"", message).c_str(), title,
               MB_OK | MB_TOPMOST);
    cout << ToConsole(L"", message) << endl;
  }

  static void MsgBox(const WCHAR *message, const WCHAR *from) {
    MessageBox(NULL, ToDisplay(from, message).c_str(), title,
               MB_OK | MB_TOPMOST);
    cout << ToConsole(from, message) << endl;
  }

  static void Log(const WCHAR *message, const WCHAR *from) {
    cout << ToConsole(from, message) << endl;
  }

  static void Throw(const WCHAR *message, const WCHAR *from) {
    MsgBox(from, message);
    throw runtime_error(ToConsole(from, message));
  }

  // 多国语言版

  static void MsgBox(const I18N &message) {
    MessageBox(NULL, ToDisplay(L"", message.wstr()).c_str(), title, MB_OK | MB_TOPMOST);
    cout << ToConsole(L"", message.wstr()) << endl;
  }

  static void MsgBox(const I18N &message, const I18N &from) {
    MessageBox(NULL, ToDisplay(from.wstr(), message.wstr()).c_str(), title,
               MB_OK | MB_TOPMOST);
    cout << ToConsole(from.wstr(), message.wstr()) << endl;
  }

  static void Log(const I18N &&message, const I18N &&from) {
    cout << ToConsole(from.wstr(), message.wstr()) << endl;
  }

  static void Throw(const I18N &&message, const I18N &&from) {
    MsgBox(from, message);
    throw runtime_error(ToConsole(from.wstr(), message.wstr()));
  }
};

const WCHAR *Logger::title = L"影键 / UmbralKeys";
