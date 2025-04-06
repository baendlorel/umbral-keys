#pragma once
#include "logger.h"

const WCHAR *Logger::title = L"影键 / UmbralKeys";

wstring Logger::ToDisplay(const WCHAR *from, const WCHAR *message) {
  wstring m;
  if (!wstring(from).empty()) {
    m.append(from);
    m.append(L": ");
  }
  m.append(message);
  return wstring(m.begin(), m.end());
}

string Logger::ToConsole(const WCHAR *from, const WCHAR *message) {
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

  string result(size_needed - 1, 0);  // 不要最后的 \0
  WideCharToMultiByte(CP_UTF8, 0, m.c_str(), -1, &result[0], size_needed, NULL,
                      NULL);

  string head = getCurrentTimestamp();
  return head.append(" ").append(result);
}

string Logger::ToConsole(const char *from, const char *message) {
  string m = getCurrentTimestamp();
  m.append(" ");
  if (string(from).empty()) {
    m.append("[UmbralKeys] ");
  } else {
    m.append("[UmbralKeys ");
    m.append(from);
    m.append("] ");
  }
  m.append(message);
  return m;
}

string Logger::getCurrentTimestamp() {
  auto now = chrono::system_clock::now();
  auto ms =
      chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) %
      1000;
  auto timer = chrono::system_clock::to_time_t(now);

  tm bt;
  localtime_s(&bt, &timer);  // Windows

  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &bt);
  return string(buf) + "." + to_string(ms.count()).substr(0, 3);
}

void Logger::MsgBox(const WCHAR *message) {
  MessageBox(NULL, ToDisplay(L"", message).c_str(), title, MB_OK | MB_TOPMOST);
  cout << ToConsole(L"", message) << endl;
}

void Logger::MsgBox(const WCHAR *message, const WCHAR *from) {
  MessageBox(NULL, ToDisplay(from, message).c_str(), title, MB_OK | MB_TOPMOST);
  cout << ToConsole(from, message) << endl;
}

void Logger::Log(const char *message, const char *from) {
  cout << ToConsole(from, message) << endl;
}

void Logger::Log(const string &message, const char *from) {
  cout << ToConsole(from, message.c_str()) << endl;
}

void Logger::Log(const ostringstream &message, const char *from) {
  cout << ToConsole(from, message.str().c_str()) << endl;
}

void Logger::Abort(const WCHAR *message, const WCHAR *from) {
  MsgBox(from, message);
  throw runtime_error(ToConsole(from, message));
}

void Logger::Throw(const char *message, const char *from) {
  throw runtime_error(ToConsole(from, message));
}

// 多国语言版
void Logger::MsgBox(const I18N &message) {
  MessageBox(NULL, ToDisplay(L"", message.wstr()).c_str(), title,
             MB_OK | MB_TOPMOST);
  cout << ToConsole(L"", message.wstr()) << endl;
}
void Logger::MsgBox(const I18N &message, const I18N &from) {
  MessageBox(NULL, ToDisplay(from.wstr(), message.wstr()).c_str(), title,
             MB_OK | MB_TOPMOST);
  cout << ToConsole(from.wstr(), message.wstr()) << endl;
}

void Logger::Abort(const I18N &&message, const WCHAR *from) {
  MsgBox(message.wstr(), from);
  throw runtime_error(ToConsole(from, message.wstr()));
}
