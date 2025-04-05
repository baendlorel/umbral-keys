#include "utils.h"
#include "umbral-key.h"
#include "logger.h"

int main() {
  SetConsoleOutputCP(CP_UTF8);

  // UmbralKey::add("CapsLock", "capslock", new const char*[2]{"ctrl", "space"},
  //                2);
  // UmbralKey::start();
  NativeArray<string> umbras = split("ctrl++space+", '+');
  cout << "size: " << umbras.size << endl;
  for (size_t i = 0; i < umbras.size; i++) {
    cout << i << " -> " << umbras.size << endl;
    cout << "[" << umbras.data[i] << "]" << endl;
  }
  Logger::MsgBox(L"from来源", L"测试内容");
  return 0;
}
