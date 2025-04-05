#include "i18n.h"
#include "logger.h"
#include "umbral-key.h"
#include "utils.h"

int main() {
  I18N::Initialize();
  // UmbralKey::add("CapsLock", "capslock", new const char*[2]{"ctrl", "space"},
  //                2);
  // UmbralKey::start();
  NativeArray<string> umbras = split("ctrl++space+", '+');
  cout << "size: " << umbras.size << endl;
  for (size_t i = 0; i < umbras.size; i++) {
    cout << i << " -> " << umbras.size << endl;
    cout << "[" << umbras.data[i] << "]" << endl;
  }

  Logger::MsgBox(I18N(L"from来源", L"from source"),
                 I18N(L"测试内容", L"test content"));
  Logger::Throw(I18N(L"trhwoer", L"中文"),
                 I18N(L"trhwoer content", L"中文 content"));
  return 0;
}
