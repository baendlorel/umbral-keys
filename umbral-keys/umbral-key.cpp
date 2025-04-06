#include "umbral-key.h"
#include "logger.h"
#include "utils.h"
#include <format>

INPUT *createInputArray(size_t size) {
  INPUT *arr = new INPUT[size];
  for (size_t i = 0; i < size; ++i) {
    // 清空输入数组中的每个元素
    arr[i] = {0}; // C++11初始化，清零所有成员

    // 设置每个 INPUT 的 type 为 KEYBOARD 输入
    arr[i].type = INPUT_KEYBOARD;

    // 初始化 KEYBDINPUT
    arr[i].ki.wVk = 0;         // 虚拟键码（0 表示没有特定键）
    arr[i].ki.wScan = 0;       // 扫描码
    arr[i].ki.dwFlags = 0;     // 不设置标志，表示按下
    arr[i].ki.time = 0;        // 设置为 0（系统自动处理时间戳）
    arr[i].ki.dwExtraInfo = 0; // 无附加信息
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
      return 1; // 返回 1 会阻止 CapsLock 键的默认行为
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

UmbralKey *UmbralKey::add(const char *name, const char *origin,
                          const char *umbras[], int umbraSize) {
  UmbralKey *u = new UmbralKey();
  string _name(name);

  WORD _origin = getKeyCode(origin);
  WORD *_umbras = new WORD[umbraSize];
  for (size_t i = 0; i < umbraSize; i++) {
    _umbras[i] = getKeyCode(umbras[i]);
  }

  u->Initialize(_name, _origin, _umbras, umbraSize);
  Instances[_origin] = u; // 将实例添加到 map 中
  return u;
}

// 成员
void UmbralKey::umbral() {
  // 发送按键输入
  SendInput(umbraSize, umbralInput, sizeof(INPUT));

  // 发送释放的按键
  SendInput(umbraSize, umbralRelease, sizeof(INPUT));

  Logger::Log(format("{} {}th time", umbralMessage, ++count),
              "UmbralKey::umbral");
}

void UmbralKey::Initialize(string &name, WORD origin, WORD *umbras,
                           int umbraSize) {
  if (isInited) {
    Logger::Log(umbralMessage + " is already initialized!",
                "UmbralKey::Initialize");
    return;
  }
  isInited = true;
  isActive = true;
  this->name = name;
  count = 0;

  // 设置原始按键
  this->umbraSize = umbraSize;
  // 必须全部好好地初始化，否则SendInput将不会工作，但也不报错，就是无效
  umbralInput = createInputArray(umbraSize);
  umbralRelease = createInputArray(umbraSize);

  string *names = new string[umbraSize];

  for (size_t i = 0; i < umbraSize; i++) {
    // 按下
    umbralInput[i].ki.wVk = umbras[i];

    // 释放
    umbralRelease[i].ki.wVk = umbras[i];
    umbralRelease[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // 键名
    names[i] = getKeyName(umbras[i]);  // 获取按键名称
  }

  umbralMessage = "{ " + this->name + ": '" + getKeyName(origin) +
                  "' -> '" +
                  join(names, umbraSize, " + ") + "' }";

  Logger::Log(umbralMessage + " is initialized successfully",
              "UmbralKey::Initialize");
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
