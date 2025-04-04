#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // 获取键盘事件
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::instances[pKeyboard->vkCode];
    if (u != nullptr && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
      u->umbral();
      return 1; // 返回 1 会阻止 CapsLock 键的默认行为
    }
  }

  // 传递事件到下一个钩子链
  return CallNextHookEx(UmbralKey::keyboard_hook, nCode, wParam, lParam);
}

// 静态成员

map<WORD, UmbralKey *> UmbralKey::instances;
HHOOK UmbralKey::keyboard_hook;

void UmbralKey::init_keyboard_hook() {
  keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, NULL, 0);
  if (keyboard_hook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }
}

UmbralKey *UmbralKey::add(const char *name, WORD origin_key, WORD *umbral_keys,
                          int umbral_keys_size) {
  UmbralKey *u = new UmbralKey();
  string _name(name);
  u->init(_name, origin_key, umbral_keys, umbral_keys_size);
  instances[origin_key] = u; // 将实例添加到 map 中
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
  std::cout << "umbralKeysLength: " << umbral_keys_size << std::endl;

  //// 发送释放的按键
  //SendInput(umbralKeysLength, umbralKeysRelease, sizeof(INPUT));

  std::cout << "[" << name << "] combo sent " << count
            << "th time: " << umbral_message << std::endl;
}

void UmbralKey::init(string &name, WORD origin_key, WORD *umbral_keys,
                     int umbral_keys_size) {
  if (is_ready) {
    cout << "UmbralKey [" << this->name << ": " << umbral_message
         << "] is already initialized!" << endl;
    return;
  }
  is_ready = true;
  is_active = true;
  this->name = name;
  count = 0;

  // 设置原始按键
  this->umbral_keys_size = umbral_keys_size;
  umbral_input = new INPUT[umbral_keys_size];   // 定义输入结构体数组
  umbral_release = new INPUT[umbral_keys_size]; // 定义输入结构体数组
  string *umbral_keys_name = new string[umbral_keys_size];

  for (size_t i = 0; i < umbral_keys_size; i++) {
    // 按下
    umbral_input[i].type = INPUT_KEYBOARD;
    umbral_input[i].ki.wVk = umbral_keys[i];

    // 释放
    umbral_release[i].type = INPUT_KEYBOARD;
    umbral_release[i].ki.wVk = umbral_keys[i];
    umbral_release[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // 键名
    umbral_keys_name[i] = keyName(umbral_keys[i]); // 获取按键名称
  }

  umbral_message = "'" + keyName(origin_key) + "' -> '" +
                  join(umbral_keys_name, umbral_keys_size, " + ") + "'";
}

UmbralKey::UmbralKey() {
  is_ready = false;
  name = "";
  count = 0;

  origin_key = 0;
  umbral_keys_size = 0;
  umbral_input = nullptr;
  umbral_release = nullptr;
}

UmbralKey::~UmbralKey() {
  if (umbral_input != nullptr) {
    delete[] umbral_input;
  }
  if (umbral_release != nullptr) {
    delete[] umbral_release;
  }
}
