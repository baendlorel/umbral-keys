#include <Windows.h>
#include <iostream>
#include "umbral-key.h"

int main() {
  std::cout << "Program running, press CapsLock to test behavior..."
            << std::endl;

  UmbralKey::add("CapsLock", VK_CAPITAL, new int[2]{VK_CONTROL, VK_SPACE}, 2);
  UmbralKey::initKeyboardHook();

  // 进入消息循环以捕获键盘事件
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}
