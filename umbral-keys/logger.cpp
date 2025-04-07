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
  if (!wstring(from).empty()) {
    m.append(L"[");
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
  return head.append("  ").append(result);
}

string Logger::ToConsole(const char *from, const char *message) {
  string m = getCurrentTimestamp();
  m.append("  ");
  if (!string(from).empty()) {
    m.append("[");
    m.append(from);
    m.append("] ");
  }
  m.append(message);
  return m;
}

string Logger::getCurrentTimestamp() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
  auto timer = system_clock::to_time_t(now);

  tm bt;
  localtime_s(&bt, &timer);  // Windows

  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &bt);

  long long msc = ms.count();

  if (msc < 10) {
    return string(buf) + ".00" + to_string(msc);
  } else if (msc < 100) {
    return string(buf) + ".0" + to_string(msc);
  } else {
    return string(buf) + "." + to_string(msc);
  }
}

void Logger::MsgBox(const WCHAR *message, const WCHAR *from) {
  MessageBox(NULL, ToDisplay(from, message).c_str(), title, MB_OK | MB_TOPMOST);
  cout << ToConsole(from, message) << endl;
}

void Logger::MsgBox(const WCHAR *message) {
  MsgBox(message, L"");  // 直接调用上面的函数
}

void Logger::Abort(const WCHAR *message, const WCHAR *from) {
  MsgBox(message, from);
  exit(1);
}

// 多国语言版
void Logger::MsgBox(const I18N &message) { MsgBox(message.wstr()); }

void Logger::MsgBox(const I18N &message, const I18N &from) {
  MsgBox(message.wstr(), from.wstr());
}

void Logger::Abort(const I18N &&message, const WCHAR *from) {
  Abort(message.wstr(), from);
}

// 仅控制台输出

void Logger::Log(const char *message, const char *from) {
  cout << ToConsole(from, message) << endl;
}

void Logger::Log(const string &message, const char *from) {
  Log(message.c_str(), from);
}

void Logger::Log(const ostringstream &message, const char *from) {
  Log(message.str().c_str(), from);
}

void Logger::Err(const char *message, const char *from) {
  cerr << ToConsole(from, message) << endl;
  exit(1);
}