#include "umbral-key.h"
#include <vector>

class UmbralKey {
private:
  int count;
  HHOOK keyboardHook;

  int originKey;
  int *umbralKeys;
  int umbralKeysLength;

  void SendKeyCombo() {
    INPUT *inputs = new INPUT[umbralKeysLength]; // 定义输入结构体数组
    INPUT *releases = new INPUT[umbralKeysLength]; // 定义输入结构体数组

    for (size_t i = 0; i < umbralKeysLength; i++) {
      inputs[i].type = INPUT_KEYBOARD;
      inputs[i].ki.wVk = umbralKeys[i];

      releases[i].type = INPUT_KEYBOARD;
      releases[i].ki.wVk = umbralKeys[i];
      releases[i].ki.dwFlags = KEYEVENTF_KEYUP;
    }

    // 发送按键输入
    SendInput(umbralKeysLength, inputs, sizeof(INPUT));
    // 发送释放的按键
    SendInput(umbralKeysLength, releases, sizeof(INPUT));

    std::cout << "combo sent" << std::endl;

    // 清理内存
    delete[] inputs;
    delete[] releases;
  }

  LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
      // 获取键盘事件
      KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

      // 检查是否是 CapsLock 键 (VK_CAPITAL)
      if (pKeyboard->vkCode == VK_CAPITAL) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
          count++;
          // CapsLock 键被按下时的自定义行为
          std::cout << "CapsLock Key Pressed! But we can change its behavior."
                    << count << "th time" << std::endl;
          SendKeyCombo();
          return 1; // 返回 1 会阻止 CapsLock 键的默认行为
        }
      }
    }

    // 传递事件到下一个钩子链
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
  }

public:
  UmbralKey() {
    count = 0;
    keyboardHook =
        SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
      std::cerr << "Failed to install keyboard hook!" << std::endl;
      exit(1);
    }
  }
};

