#include "umbral-key.h"
#include "logger.h"
#include "utils.h"
#include <format>

INPUT *createInputArray(size_t size) {
  INPUT *arr = new INPUT[size];
  for (size_t i = 0; i < size; ++i) {
    // ������������е�ÿ��Ԫ��
    arr[i] = {0}; // C++11��ʼ�����������г�Ա

    // ����ÿ�� INPUT �� type Ϊ KEYBOARD ����
    arr[i].type = INPUT_KEYBOARD;

    // ��ʼ�� KEYBDINPUT
    arr[i].ki.wVk = 0;         // ������루0 ��ʾû���ض�����
    arr[i].ki.wScan = 0;       // ɨ����
    arr[i].ki.dwFlags = 0;     // �����ñ�־����ʾ����
    arr[i].ki.time = 0;        // ����Ϊ 0��ϵͳ�Զ�����ʱ�����
    arr[i].ki.dwExtraInfo = 0; // �޸�����Ϣ
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
      return 1; // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
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
  Instances[_origin] = u; // ��ʵ����ӵ� map ��
  return u;
}

// ��Ա
void UmbralKey::umbral() {
  // ���Ͱ�������
  SendInput(umbraSize, umbralInput, sizeof(INPUT));

  // �����ͷŵİ���
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

  // ����ԭʼ����
  this->umbraSize = umbraSize;
  // ����ȫ���úõس�ʼ��������SendInput�����Ṥ������Ҳ������������Ч
  umbralInput = createInputArray(umbraSize);
  umbralRelease = createInputArray(umbraSize);

  string *names = new string[umbraSize];

  for (size_t i = 0; i < umbraSize; i++) {
    // ����
    umbralInput[i].ki.wVk = umbras[i];

    // �ͷ�
    umbralRelease[i].ki.wVk = umbras[i];
    umbralRelease[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // ����
    names[i] = getKeyName(umbras[i]);  // ��ȡ��������
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
