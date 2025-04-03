#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // 获取键盘事件
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::instances[pKeyboard->vkCode];
    if (u == nullptr) {
      // 没有找到对应的 UmbralKey 实例，继续传递事件
      return CallNextHookEx(UmbralKey::keyboardHook, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      u->umbral();
      return 1; // 返回 1 会阻止 CapsLock 键的默认行为
    }
  }

  // 传递事件到下一个钩子链
  return CallNextHookEx(UmbralKey::keyboardHook, nCode, wParam, lParam);
}

// 静态成员

map<DWORD, UmbralKey *> UmbralKey::instances;

void UmbralKey::initKeyboardHook() {
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }
}

UmbralKey *UmbralKey::add(const char *name, int originKey, int *umbralKeys,
                          int umbralKeysLength) {
  UmbralKey *u = new UmbralKey();
  string _name(name);
  u->init(_name, originKey, umbralKeys, umbralKeysLength);
  instances[originKey] = u; // 将实例添加到 map 中
}

// # 成员函数

void UmbralKey::umbral() {
  count++;

  // 发送按键输入
  SendInput(umbralKeysLength, umbralKeysInput, sizeof(INPUT));
  // 发送释放的按键
  SendInput(umbralKeysLength, umbralKeysRelease, sizeof(INPUT));

  std::cout << "[" << name << "] combo sent " << count
            << "th time: " << umbralMessage << std::endl;
}

void UmbralKey::init(string &name, int originKey, int *umbralKeys,
                     int umbralKeysLength) {
  if (isInited) {
    cout << "UmbralKey [" << this->name << ": " << umbralMessage
         << "] is already initialized!" << endl;
    return;
  }

  isInited = true;
  this->name = name;
  count = 0;

  // 设置原始按键
  this->umbralKeysLength = umbralKeysLength;
  umbralKeysInput = new INPUT[umbralKeysLength];   // 定义输入结构体数组
  umbralKeysRelease = new INPUT[umbralKeysLength]; // 定义输入结构体数组
  string *umbralKeysName = new string[umbralKeysLength];

  for (size_t i = 0; i < umbralKeysLength; i++) {
    // 按下
    umbralKeysInput[i].type = INPUT_KEYBOARD;
    umbralKeysInput[i].ki.wVk = umbralKeys[i];

    // 释放
    umbralKeysRelease[i].type = INPUT_KEYBOARD;
    umbralKeysRelease[i].ki.wVk = umbralKeys[i];
    umbralKeysRelease[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // 键名
    umbralKeysName[i] = keyName(umbralKeys[i]); // 获取按键名称
  }

  umbralMessage = "'" + keyName(originKey) + "' -> '" +
                  join(umbralKeysName, umbralKeysLength, " + ") + "'";
}

UmbralKey::UmbralKey() {
  isInited = false;
  name = "";
  count = 0;

  originKey = 0;
  umbralKeysLength = 0;
  umbralKeysInput = nullptr;
  umbralKeysRelease = nullptr;
}

UmbralKey::~UmbralKey() {
  if (umbralKeysInput != nullptr) {
    delete[] umbralKeysInput;
  }
  if (umbralKeysRelease != nullptr) {
    delete[] umbralKeysRelease;
  }
}
