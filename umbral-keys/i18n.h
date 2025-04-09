#pragma once
#include <windows.h>

#include <iostream>
#include <string>

using namespace std;

class I18N {
 private:
  static bool isChinese;

 public:
  static void Initialize();
  static const WCHAR *Get(const WCHAR *zh, const WCHAR *en);
  static const char *Get(const char *zh, const char *en);

 private:
  const WCHAR *zh;
  const WCHAR *en;

 public:
  I18N(const WCHAR *zh, const WCHAR *en);
  const WCHAR *wstr() const;
};
