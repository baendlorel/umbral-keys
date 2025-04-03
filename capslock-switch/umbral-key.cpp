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
    INPUT *inputs = new INPUT[umbralKeysLength]; // ��������ṹ������
    INPUT *releases = new INPUT[umbralKeysLength]; // ��������ṹ������

    for (size_t i = 0; i < umbralKeysLength; i++) {
      inputs[i].type = INPUT_KEYBOARD;
      inputs[i].ki.wVk = umbralKeys[i];

      releases[i].type = INPUT_KEYBOARD;
      releases[i].ki.wVk = umbralKeys[i];
      releases[i].ki.dwFlags = KEYEVENTF_KEYUP;
    }

    // ���Ͱ�������
    SendInput(umbralKeysLength, inputs, sizeof(INPUT));
    // �����ͷŵİ���
    SendInput(umbralKeysLength, releases, sizeof(INPUT));

    std::cout << "combo sent" << std::endl;

    // �����ڴ�
    delete[] inputs;
    delete[] releases;
  }

  LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
      // ��ȡ�����¼�
      KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

      // ����Ƿ��� CapsLock �� (VK_CAPITAL)
      if (pKeyboard->vkCode == VK_CAPITAL) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
          count++;
          // CapsLock ��������ʱ���Զ�����Ϊ
          std::cout << "CapsLock Key Pressed! But we can change its behavior."
                    << count << "th time" << std::endl;
          SendKeyCombo();
          return 1; // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
        }
      }
    }

    // �����¼�����һ��������
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

