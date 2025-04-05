#pragma once
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class I18N {
private:
  static bool isChinese;

public:
  static void Initialize() {
    // ���ÿ���̨���Ϊ UTF-8 ���룬������ʾ����
    SetConsoleOutputCP(CP_UTF8);

    WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
    int ret = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
    if (ret) {
      wprintf(L"System locale: %s\n", localeName); // ���� "zh-CN"
      if (wstring(localeName) == wstring(L"zh-CN")) {
        isChinese = true;
      } else {
        isChinese = false;
      }
    } else {
      cerr << "Failed to get locale name\n";
    }
  }

  static const WCHAR *Get(const WCHAR *zh, const WCHAR *en) {
    return isChinese ? zh : en;
  }

private:
  const WCHAR *zh;
  const WCHAR *en;

public:
  I18N(const WCHAR *zh, const WCHAR *en) : zh(zh), en(en) {}
  const WCHAR *wstr() const { return isChinese ? zh : en; }
};
bool I18N::isChinese = true;
