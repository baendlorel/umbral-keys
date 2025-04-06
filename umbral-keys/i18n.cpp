#include "i18n.h"

bool I18N::isChinese = true;

I18N::I18N(const WCHAR *zh, const WCHAR *en) : zh(zh), en(en) {}

const WCHAR *I18N::wstr() const { return isChinese ? zh : en; }

void I18N::Initialize() {
  // ���ÿ���̨���Ϊ UTF-8 ���룬������ʾ����
  SetConsoleOutputCP(CP_UTF8);

  WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
  int ret = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
  if (ret) {
    wprintf(L"System locale: %s\n", localeName);  // ���� "zh-CN"
    if (wstring(localeName) == wstring(L"zh-CN")) {
      isChinese = true;
    } else {
      isChinese = false;
    }
  } else {
    cerr << "Failed to get locale name\n";
  }
}

const WCHAR *I18N::Get(const WCHAR *zh, const WCHAR *en) {
  return isChinese ? zh : en;
}
