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

void UmbralKey::start() {
  KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (KeyboardHook == NULL) {
    cerr << "Failed to install keyboard hook!" << endl;
    exit(1);
  }

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
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


// 成员
void UmbralKey::umbral() {
  count++;

  // 发送按键输入
  SendInput(umbraSize, umbralInput, sizeof(INPUT));

  // 发送释放的按键
  SendInput(umbraSize, umbralRelease, sizeof(INPUT));

  cout << "[" << name << "].[" << count << "]" << umbralMessage << endl;
}

void UmbralKey::init(string &name, WORD origin, WORD *umbras, int umbraSize) {
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
  umbralInput = new INPUT[umbraSize];
  umbralRelease = new INPUT[umbraSize];
  // 必须全部初始化为0，否则SendInput将不会工作，但也不报错，就是无效
  ZeroMemory(umbralInput, sizeof(INPUT) * umbraSize);
  ZeroMemory(umbralRelease, sizeof(INPUT) * umbraSize);

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

  cout << "UmbralKey [" << this->name << ": " << umbralMessage
       << "] is initialized successfully" << endl;
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
