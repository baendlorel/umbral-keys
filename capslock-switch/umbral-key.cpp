#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // 获取键盘事件
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::find(pKeyboard->vkCode);
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

UmbralKey *UmbralKey::instances[] = new UmbralKey *[5];
int UmbralKey::instanceLength = 5;
int UmbralKey::instanceIndex = 5;

void UmbralKey::initKeyboardHook() {
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }
}

UmbralKey *UmbralKey::find(int originKey) {
  for (size_t i = 0; i <= instanceIndex; i++) {
    if (instances[i].originKey == originKey) {
      return instances[i];
    }
  }
  return nullptr;
}

UmbralKey *UmbralKey::add(string &name, int originKey, int *umbralKeys,
                          int umbralKeysLength) {
  UmbralKey *u = new UmbralKey();
  u->init(name, originKey, umbralKeys, umbralKeysLength);
  if (instanceIndex >= instanceLength - 1) {
    // 扩展实例数组
    UmbralKey **newInstances = new UmbralKey *[instanceLength + 5];
    for (int i = 0; i < instanceLength; i++) {
      newInstances[i] = instances[i];
    }
    delete[] instances;
    instances = newInstances;
    instanceLength += 5;
  }
  instanceIndex++;
  instances[instanceIndex] = u;
}

// # 成员函数

void UmbralKey::umbral() {
  count++;

  // 发送按键输入
  SendInput(umbralKeysLength, umbralKeysInput, sizeof(INPUT));
  // 发送释放的按键
  SendInput(umbralKeysLength, umbralKeysRelease, sizeof(INPUT));

  std::cout << "[" << name << "] combo sent " << count << "th time: '"
            << keyName(originKey) << "' umbraled to '"
            << join(umbralKeysName, umbralKeysLength, " + ") << "'"
            << std::endl;
}

void UmbralKey::init(string &name, int originKey, int *umbralKeys,
                     int umbralKeysLength) {
  this->name = name;
  count = 0;

  // 设置原始按键
  this->umbralKeysLength = umbralKeysLength;
  originKeyName = keyName(originKey);
  umbralKeysInput = new INPUT[umbralKeysLength];   // 定义输入结构体数组
  umbralKeysRelease = new INPUT[umbralKeysLength]; // 定义输入结构体数组
  umbralKeysName = new string[umbralKeysLength];

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
}

UmbralKey::~UmbralKey() {
  // 清理内存
  delete[] umbralKeysInput;
  delete[] umbralKeysRelease;
  delete[] umbralKeysName;
}
