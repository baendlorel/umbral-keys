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
  Instances[origin] = u; // ��ʵ����ӵ� map ��
  return u;
}


// ��Ա
void UmbralKey::umbral() {
  count++;

  // ���Ͱ�������
  SendInput(umbraSize, umbralInput, sizeof(INPUT));

  // �����ͷŵİ���
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

  // ����ԭʼ����
  this->umbraSize = umbraSize;
  umbralInput = new INPUT[umbraSize];
  umbralRelease = new INPUT[umbraSize];
  // ����ȫ����ʼ��Ϊ0������SendInput�����Ṥ������Ҳ������������Ч
  ZeroMemory(umbralInput, sizeof(INPUT) * umbraSize);
  ZeroMemory(umbralRelease, sizeof(INPUT) * umbraSize);

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
