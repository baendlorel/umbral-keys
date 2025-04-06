#pragma once
#include "i18n.h"
#include <chrono> // 用于获取时间
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

using namespace std;

class Logger {
private:
  static const WCHAR *title;

  static wstring ToDisplay(const WCHAR *from, const WCHAR *message);

  static string ToConsole(const WCHAR *from, const WCHAR *message);
  static string ToConsole(const char *from, const char *message);

  static string getCurrentTimestamp();

public:
  static void MsgBox(const WCHAR *message);
  static void MsgBox(const WCHAR *message, const WCHAR *from);

  static void Log(const char *message, const char *from = "");
  static void Log(const string &message, const char *from = "");
  static void Log(const ostringstream &message, const char *from = "");

  static void Abort(const WCHAR *message, const WCHAR *from);
  static void Throw(const char *message, const char *from);

  // 多国语言版

  static void MsgBox(const I18N &message);
  static void MsgBox(const I18N &message, const I18N &from);
  static void Abort(const I18N &&message, const WCHAR *from);
};

const WCHAR *Logger::title = L"影键 / UmbralKeys";
