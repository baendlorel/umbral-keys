#include <Windows.h>
#include <iostream>
#include "umbral-key.h"

void SendKeyCombo() {
  INPUT inputs[3] = {};   // 定义输入结构体数组
  INPUT releases[3] = {}; // 定义输入结构体数组

  // 模拟按下 Ctrl
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = VK_CONTROL;

  // 模拟按下 Space
  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = VK_SPACE;


  // 释放 Ctrl
  releases[0].type = INPUT_KEYBOARD;
  releases[0].ki.wVk = VK_CONTROL;
  releases[0].ki.dwFlags = KEYEVENTF_KEYUP;

  // 释放 Space
  releases[1].type = INPUT_KEYBOARD;
  releases[1].ki.wVk = VK_SPACE;
  releases[1].ki.dwFlags = KEYEVENTF_KEYUP;


  // 发送按键输入
  SendInput(3, inputs, sizeof(INPUT));
  std::cout << "combo sent" << std::endl;

  // 发送释放的按键
  SendInput(3, releases, sizeof(INPUT));
  std::cout << "combo released" << std::endl;
}

HHOOK keyboardHook;

int i = 0;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // 获取键盘事件
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    // 检查是否是 CapsLock 键 (VK_CAPITAL)
    if (pKeyboard->vkCode == VK_CAPITAL) {
      if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        i++;
        // CapsLock 键被按下时的自定义行为
        std::cout << "CapsLock Key Pressed! But we can change its behavior."
                  << i << "th time" << std::endl;

        // 你可以在这里自定义行为，比如禁用 CapsLock 键
        // 取消 CapsLock 的原始行为
        SendKeyCombo();
        return 1; // 返回 1 会阻止 CapsLock 键的默认行为
      }
    }
  }

  // 传递事件到下一个钩子链
  return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void SetCapsLockBehavior() {
  // 安装钩子来捕获键盘事件
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }

  // 进入消息循环以捕获键盘事件
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

int main() {
  std::cout << "Program running, press CapsLock to test behavior..."
            << std::endl;

  UmbralKey::add("CapsLock", VK_CAPITAL, new int[2]{VK_CONTROL, VK_SPACE}, 2);

  // 进入消息循环以捕获键盘事件
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}
