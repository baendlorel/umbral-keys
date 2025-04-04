#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // 获取键盘事件
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::Instances[pKeyboard->vkCode];
    if (u != nullptr && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
      u->umbral();
      return 1; // 返回 1 会阻止 CapsLock 键的默认行为
    }
  }

  // 传递事件到下一个钩子链
  return CallNextHookEx(UmbralKey::KeyboardHook, nCode, wParam, lParam);
}

// 静态成员
map<WORD, UmbralKey *> UmbralKey::Instances;
HHOOK UmbralKey::KeyboardHook;

void UmbralKey::initKeyboardHook() {
  KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (KeyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }
}

UmbralKey *UmbralKey::add(const char *name, WORD origin, WORD *umbras,
                          int umbraSize) {
  UmbralKey *u = new UmbralKey();
  string _name(name);
  u->init(_name, origin, umbras, umbraSize);
  Instances[origin] = u; // 将实例添加到 map 中
  return u;
}

// # 成员函数

void UmbralKey::umbral() {
  count++;

  // for (size_t i = 0; i < umbralKeysLength; i++) {
  //   cout << "umbralKeysInput "<< keyName(umbralKeysInput[i].ki.wVk) << endl;
  // }

  // for (size_t i = 0; i < umbralKeysLength; i++) {
  //   cout << "umbralKeysRelease " << keyName(umbralKeysRelease[i].ki.wVk) <<
  //   endl;
  // }

  INPUT inputs[2] = {};   // 定义输入结构体数组
  INPUT releases[2] = {}; // 定义输入结构体数组

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
  SendInput(2, inputs, sizeof(INPUT));
  std::cout << "combo sent" << std::endl;

  // 发送释放的按键
  SendInput(2, releases, sizeof(INPUT));
  std::cout << "combo released" << std::endl;

  //// 发送按键输入
  //SendInput(umbralKeysLength, umbralKeysInput, sizeof(INPUT));
  std::cout << "umbralKeysLength: " << umbraSize << std::endl;

  //// 发送释放的按键
  //SendInput(umbralKeysLength, umbralKeysRelease, sizeof(INPUT));

  std::cout << "[" << name << "] combo sent " << count
            << "th time: " << umbralMessage << std::endl;
}

void UmbralKey::init(string &name, WORD origin, WORD *umbras,
                     int umbraSize) {
  if (isInited) {
    cout << "UmbralKey [" << this->name << ": " << umbralMessage
         << "] is already initialized!" << endl;
    return;
  }
  isInited = true;
  isActive = true;
  this->name = name;
  count = 0;

  // 设置原始按键
  this->umbraSize = umbraSize;
  umbralInput = new INPUT[umbraSize];   // 定义输入结构体数组
  umbralRelease = new INPUT[umbraSize]; // 定义输入结构体数组
  string *umbral_keys_name = new string[umbraSize];

  for (size_t i = 0; i < umbraSize; i++) {
    // 按下
    umbralInput[i].type = INPUT_KEYBOARD;
    umbralInput[i].ki.wVk = umbras[i];

    // 释放
    umbralRelease[i].type = INPUT_KEYBOARD;
    umbralRelease[i].ki.wVk = umbras[i];
    umbralRelease[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // 键名
    umbral_keys_name[i] = getKeyName(umbras[i]); // 获取按键名称
  }

  umbralMessage = "'" + getKeyName(origin) + "' -> '" +
                  join(umbral_keys_name, umbraSize, " + ") + "'";
}

UmbralKey::UmbralKey() {
  isActive = false;
  isInited = false;
  name = "";
  count = 0;

  origin = 0;
  umbraSize = 0;
  umbralInput = nullptr;
  umbralRelease = nullptr;
}

UmbralKey::~UmbralKey() {
  if (umbralInput != nullptr) {
    delete[] umbralInput;
  }
  if (umbralRelease != nullptr) {
    delete[] umbralRelease;
  }
}
