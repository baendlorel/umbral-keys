#pragma once
#include <Windows.h>
#include <iostream>

void _SendKeyCombo() {
  INPUT inputs[3] = {};   // ��������ṹ������
  INPUT releases[3] = {}; // ��������ṹ������

  // ģ�ⰴ�� Ctrl
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = VK_CONTROL;

  // ģ�ⰴ�� Space
  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = VK_SPACE;

  // �ͷ� Ctrl
  releases[0].type = INPUT_KEYBOARD;
  releases[0].ki.wVk = VK_CONTROL;
  releases[0].ki.dwFlags = KEYEVENTF_KEYUP;

  // �ͷ� Space
  releases[1].type = INPUT_KEYBOARD;
  releases[1].ki.wVk = VK_SPACE;
  releases[1].ki.dwFlags = KEYEVENTF_KEYUP;

  // ���Ͱ�������
  SendInput(3, inputs, sizeof(INPUT));
  std::cout << "combo sent" << std::endl;

  // �����ͷŵİ���
  SendInput(3, releases, sizeof(INPUT));
  std::cout << "combo released" << std::endl;
}

HHOOK keyboardHook;

int i = 0;
LRESULT CALLBACK _KeyboardProc(int nCode, WPARAM wParam,
                                                  LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // ��ȡ�����¼�
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    // ����Ƿ��� CapsLock �� (VK_CAPITAL)
    if (pKeyboard->vkCode == VK_CAPITAL) {
      if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        i++;
        // CapsLock ��������ʱ���Զ�����Ϊ
        std::cout << "CapsLock Key Pressed! But we can change its behavior."
                  << i << "th time" << std::endl;

        // ������������Զ�����Ϊ��������� CapsLock ��
        // ȡ�� CapsLock ��ԭʼ��Ϊ
        _SendKeyCombo();
        return 1; // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
      }
    }
  }

  // �����¼�����һ��������
  return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void _SetCapsLockBehavior() {
  // ��װ��������������¼�
  keyboardHook =
      SetWindowsHookEx(WH_KEYBOARD_LL, _KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }

  // ������Ϣѭ���Բ�������¼�
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
