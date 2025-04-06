#include "umbral-key.h"

#include <format>

#include "logger.h"
#include "utils.h"

INPUT *createInputArray(size_t size) {
  INPUT *arr = new INPUT[size];
  for (size_t i = 0; i < size; ++i) {
    // ������������е�ÿ��Ԫ��
    arr[i] = {0};  // C++11��ʼ�����������г�Ա

    // ����ÿ�� INPUT �� type Ϊ KEYBOARD ����
    arr[i].type = INPUT_KEYBOARD;

    // ��ʼ�� KEYBDINPUT
    arr[i].ki.wVk = 0;          // ������루0 ��ʾû���ض�����
    arr[i].ki.wScan = 0;        // ɨ����
    arr[i].ki.dwFlags = 0;      // �����ñ�־����ʾ����
    arr[i].ki.time = 0;         // ����Ϊ 0��ϵͳ�Զ�����ʱ�����
    arr[i].ki.dwExtraInfo = 0;  // �޸�����Ϣ
  }
  return arr;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // ��ȡ�����¼�
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::Instances[(WORD)(pKeyboard->vkCode & 0xFFFF)];
    if (u != nullptr && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
      u->umbral();
      return 1;  // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
    }
  }

  // �����¼�����һ��������
  return CallNextHookEx(UmbralKey::KeyboardHook, nCode, wParam, lParam);
}

// ��̬��Ա
unordered_map<WORD, UmbralKey *> UmbralKey::Instances;
HHOOK UmbralKey::KeyboardHook;

void UmbralKey::start() {
  KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (KeyboardHook == NULL) {
    Logger::Abort(I18N{L"�趨���̹���ʧ��", L"Failed to install keyboard hook"},
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
  Instances[_origin] = u;  // ��ʵ����ӵ� map ��
  return u;
}

// ��Ա
void UmbralKey::umbral() {
  // ���Ͱ�������
  SendInput(size, press, sizeof(INPUT));

  // �����ͷŵİ���
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

  // ����ԭʼ����
  this->size = size;

  // ����ȫ���úõس�ʼ��������SendInput�����Ṥ������Ҳ������������Ч
  press = createInputArray(size);
  release = createInputArray(size);

  string *names = new string[size];

  for (size_t i = 0; i < size; i++) {
    // ����
    press[i].ki.wVk = umbras[i];

    // �ͷ�
    release[i].ki.wVk = umbras[i];
    release[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // ����
    names[i] = getKeyName(umbras[i]);  // ��ȡ��������
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
