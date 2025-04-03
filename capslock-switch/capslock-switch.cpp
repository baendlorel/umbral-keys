// capslock-switch.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <Windows.h>
#include <iostream>


// 发送组合键（例如 Ctrl + Shift + A）
void SendKeyCombo() {
  INPUT inputs[4] = {}; // 定义输入结构体数组

  // 模拟按下 Ctrl
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = VK_CONTROL;

  // 模拟按下 Shift
  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = VK_SHIFT;

  // 模拟按下 A
  inputs[2].type = INPUT_KEYBOARD;
  inputs[2].ki.wVk = 'A';

  // 释放 A
  inputs[3].type = INPUT_KEYBOARD;
  inputs[3].ki.wVk = 'A';
  inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

  // 释放 Shift
  INPUT releaseShift = inputs[1];
  releaseShift.ki.dwFlags = KEYEVENTF_KEYUP;

  // 释放 Ctrl
  INPUT releaseCtrl = inputs[0];
  releaseCtrl.ki.dwFlags = KEYEVENTF_KEYUP;

  // 发送按键输入
  SendInput(4, inputs, sizeof(INPUT));

  // 发送释放的按键
  SendInput(1, &releaseShift, sizeof(INPUT));
  SendInput(1, &releaseCtrl, sizeof(INPUT));
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
                  << i << "th time"
                  << std::endl;

        // 你可以在这里自定义行为，比如禁用 CapsLock 键
        // 取消 CapsLock 的原始行为
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
  SetCapsLockBehavior();
  return 0;
}
