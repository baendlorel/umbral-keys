#include "umbral-key.h"

#include <format>

#include "logger.h"
#include "utils.h"

INPUT *createInputArray(size_t size) {
  INPUT *arr = new INPUT[size];
  for (size_t i = 0; i < size; ++i) {
    // 清空输入数组中的每个元素
    arr[i] = {0};  // C++11初始化，清零所有成员

    // 设置每个 INPUT 的 type 为 KEYBOARD 输入
    arr[i].type = INPUT_KEYBOARD;

    // 初始化 KEYBDINPUT
    arr[i].ki.wVk = 0;          // 虚拟键码（0 表示没有特定键）
    arr[i].ki.wScan = 0;        // 扫描码
    arr[i].ki.dwFlags = 0;      // 不设置标志，表示按下
    arr[i].ki.time = 0;         // 设置为 0（系统自动处理时间戳）
    arr[i].ki.dwExtraInfo = 0;  // 无附加信息
  }
  return arr;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // 获取键盘事件
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::Instances[(WORD)(pKeyboard->vkCode & 0xFFFF)];
    if (u != nullptr && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
      u->umbral();
      return 1;  // 返回 1 会阻止 CapsLock 键的默认行为
    }
  }

  // 传递事件到下一个钩子链
  return CallNextHookEx(UmbralKey::KeyboardHook, nCode, wParam, lParam);
}

// 静态成员
unordered_map<WORD, UmbralKey *> UmbralKey::Instances;
HHOOK UmbralKey::KeyboardHook;

void UmbralKey::start() {
  KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (KeyboardHook == NULL) {
    Logger::Abort(I18N{L"设定键盘钩子失败", L"Failed to install keyboard hook"},
                  L"UmbralKey::start");
  }

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

UmbralKey *UmbralKey::add(chars origin, chars umbras[], int size) {
  UmbralKey *u = new UmbralKey();

  WORD _origin = getKeyCode(origin);
  WORD *_umbras = new WORD[size];
  for (size_t i = 0; i < size; i++) {
    _umbras[i] = getKeyCode(umbras[i]);
  }

  u->Initialize(_origin, _umbras, size);
  Instances[_origin] = u;  // 将实例添加到 map 中
  return u;
}

// 成员
void UmbralKey::umbral() {
  // 发送按键输入
  SendInput(size, press, sizeof(INPUT));

  // 发送释放的按键
  SendInput(size, release, sizeof(INPUT));

  Logger::Log(format("{} {}", message, ++count));
}

void UmbralKey::Initialize(WORD origin, WORD *umbras, int size) {
  if (isInited) {
    Logger::Log(message + " is already initialized!",
                "UmbralKey::Initialize");
    return;
  }

  isInited = true;
  count = 0;

  // 设置原始按键
  this->size = size;

  // 必须全部好好地初始化，否则SendInput将不会工作，但也不报错，就是无效
  press = createInputArray(size);
  release = createInputArray(size);

  string *names = new string[size];

  for (size_t i = 0; i < size; i++) {
    // 按下
    press[i].ki.wVk = umbras[i];

    // 释放
    release[i].ki.wVk = umbras[i];
    release[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // 键名
    names[i] = getKeyName(umbras[i]);  // 获取按键名称
  }

  message = getKeyName(origin) + " -> " + join(names, size, " + ");

  Logger::Log(message + " is initialized", "UmbralKey::Initialize");
}

UmbralKey::UmbralKey() {
  isInited = false;
  count = 0;

  size = 0;
  press = nullptr;
  release = nullptr;
}

UmbralKey::~UmbralKey() {
  if (press != nullptr) {
    delete[] press;
  }
  if (release != nullptr) {
    delete[] release;
  }
}
