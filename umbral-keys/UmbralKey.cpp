#include "UmbralKey.h"

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

void UmbralKey::InitializeKeyboardHook() {
  KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (KeyboardHook == NULL) {
    Logger::Abort(I18N{L"�趨���̹���ʧ��", L"Failed to install keyboard hook"},
                  L"UmbralKey::start");
  }
}

UmbralKey *UmbralKey::Add(chars origin, const Array<chars> &umbras) {
  size_t size = umbras.getSize();
  WORD _origin = getKeyCode(origin);
  Array<WORD> _umbras;
  for (size_t i = 0; i < size; i++) {
    _umbras.push(getKeyCode(umbras[i]));
  }

  UmbralKey *u = new UmbralKey();
  u->Initialize(_origin, _umbras);
  Instances[_origin] = u;  // ��ʵ����ӵ� map ��
  return u;
}

void UmbralKey::ApplyConfig(const unordered_map<WORD, Array<WORD>> &config) {
  for (const auto &[origin, umbras] : config) {
    UmbralKey *u = new UmbralKey();
    u->Initialize(origin, umbras);
    Instances[origin] = u;  // ��ʵ����ӵ� map ��
  }
}

wstring UmbralKey::ViewUmbras() { 
    wstring data;
  for (const auto &[origin, u] : Instances) {
      data.append(stow(u->message)).append(L"\n");
  }
  return data;
}

bool UmbralKey::disabledAll = false;

// ��Ա
void UmbralKey::umbral() {
  if (disabledAll || disabled) {
    return;
  }

  static int _inputSize = sizeof(INPUT);

  UINT _size = static_cast<UINT>(size);

  // ���Ͱ�������
  SendInput(_size, press, _inputSize);

  // �����ͷŵİ���
  SendInput(_size, release, _inputSize);

  Logger::Log(format("{} {}", message, ++count));
}

void UmbralKey::Initialize(WORD origin, const Array<WORD> &umbras) {
  if (isInitialized) {
    Logger::Log(message + " is already initialized!", "UmbralKey::Initialize");
    return;
  }

  isInitialized = true;
  disabled = false;
  count = 0;
  size = umbras.getSize();

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
  isInitialized = false;
  disabled = true;
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
