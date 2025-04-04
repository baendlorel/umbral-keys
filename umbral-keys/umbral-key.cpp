#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // ��ȡ�����¼�
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::Instances[pKeyboard->vkCode];
    if (u != nullptr && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
      u->umbral();
      return 1; // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
    }
  }

  // �����¼�����һ��������
  return CallNextHookEx(UmbralKey::KeyboardHook, nCode, wParam, lParam);
}

// ��̬��Ա
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
  Instances[origin] = u; // ��ʵ����ӵ� map ��
  return u;
}

// # ��Ա����

void UmbralKey::umbral() {
  count++;

  // for (size_t i = 0; i < umbralKeysLength; i++) {
  //   cout << "umbralKeysInput "<< keyName(umbralKeysInput[i].ki.wVk) << endl;
  // }

  // for (size_t i = 0; i < umbralKeysLength; i++) {
  //   cout << "umbralKeysRelease " << keyName(umbralKeysRelease[i].ki.wVk) <<
  //   endl;
  // }

  INPUT inputs[2] = {};   // ��������ṹ������
  INPUT releases[2] = {}; // ��������ṹ������

  // ģ�ⰴ�� Ctrl
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = VK_CONTROL;

  // ģ�ⰴ�� Space
  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = VK_SPACE;

  // �ͷ� Ctrl
  releases[0].type = INPUT_KEYBOARD;
  releases[0].ki.wVk = VK_CONTROL;
  releases[0].ki.dwFlags = KEYEVENTF_KEYUP;

  // �ͷ� Space
  releases[1].type = INPUT_KEYBOARD;
  releases[1].ki.wVk = VK_SPACE;
  releases[1].ki.dwFlags = KEYEVENTF_KEYUP;

  // ���Ͱ�������
  SendInput(2, inputs, sizeof(INPUT));
  std::cout << "combo sent" << std::endl;

  // �����ͷŵİ���
  SendInput(2, releases, sizeof(INPUT));
  std::cout << "combo released" << std::endl;

  //// ���Ͱ�������
  //SendInput(umbralKeysLength, umbralKeysInput, sizeof(INPUT));
  std::cout << "umbralKeysLength: " << umbraSize << std::endl;

  //// �����ͷŵİ���
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

  // ����ԭʼ����
  this->umbraSize = umbraSize;
  umbralInput = new INPUT[umbraSize];   // ��������ṹ������
  umbralRelease = new INPUT[umbraSize]; // ��������ṹ������
  string *umbral_keys_name = new string[umbraSize];

  for (size_t i = 0; i < umbraSize; i++) {
    // ����
    umbralInput[i].type = INPUT_KEYBOARD;
    umbralInput[i].ki.wVk = umbras[i];

    // �ͷ�
    umbralRelease[i].type = INPUT_KEYBOARD;
    umbralRelease[i].ki.wVk = umbras[i];
    umbralRelease[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // ����
    umbral_keys_name[i] = getKeyName(umbras[i]); // ��ȡ��������
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
