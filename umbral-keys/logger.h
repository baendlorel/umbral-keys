#pragma once
#include <chrono> // ���ڻ�ȡʱ��
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>
#include <boost/stacktrace.hpp>
#include "i18n.h"

using namespace std;

class Logger {
 private:
  static wstring ToDisplay(const WCHAR *from, const WCHAR *message);

  static string ToConsole(const WCHAR *from, const WCHAR *message);
  static string ToConsole(const char *from, const char *message);

  static string getDateTimeString();

 public:
  static void MsgBox(const WCHAR *message);
  static void MsgBox(const WCHAR *message, const WCHAR *from);

  static void Log(const char *message, const char *from = "");
  static void Log(const string &message, const char *from = "");
  static void Log(const ostringstream &message, const char *from = "");

  static void Abort(const WCHAR *message, const WCHAR *from);
  static void Err(const char *message, const char *from);

  // ������԰�
  static void MsgBox(const I18N &message);
  static void MsgBox(const I18N &message, const I18N &from);
  static void Abort(const I18N &&message, const WCHAR *from);


  // ������־��¼
  static void SaveError(const std::string &message);
};