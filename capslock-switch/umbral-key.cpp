#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // ��ȡ�����¼�
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::instances[pKeyboard->vkCode];
    if (u != nullptr && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
      u->umbral();
      return 1; // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
    }
  }

  // �����¼�����һ��������
  return CallNextHookEx(UmbralKey::keyboardHook, nCode, wParam, lParam);
}

// ��̬��Ա

map<DWORD, UmbralKey *> UmbralKey::instances;
HHOOK UmbralKey::keyboardHook;

void UmbralKey::initKeyboardHook() {
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }
}

UmbralKey *UmbralKey::add(const char *name, int originKey, int *umbralKeys,
                          int umbralKeysLength) {
  UmbralKey *u = new UmbralKey();
  string _name(name);
  u->init(_name, originKey, umbralKeys, umbralKeysLength);
  instances[originKey] = u; // ��ʵ����ӵ� map ��
  return u;
}

// # ��Ա����

void UmbralKey::umbral() {
  count++;

  //for (size_t i = 0; i < umbralKeysLength; i++) {
  //  cout << "umbralKeysInput "<< keyName(umbralKeysInput[i].ki.wVk) << endl;
  //}

  //for (size_t i = 0; i < umbralKeysLength; i++) {
  //  cout << "umbralKeysRelease " << keyName(umbralKeysRelease[i].ki.wVk) << endl;
  //}


  // ���Ͱ�������
  UINT sentInputs = SendInput(umbralKeysLength, umbralKeysInput, sizeof(INPUT));
  if (sentInputs != umbralKeysLength) {
    std::cerr << "Failed to send key inputs. Error: " << GetLastError()
              << std::endl;
  }

  // �����ͷŵİ���
  sentInputs = SendInput(umbralKeysLength, umbralKeysRelease, sizeof(INPUT));
  if (sentInputs != umbralKeysLength) {
    std::cerr << "Failed to send key releases. Error: " << GetLastError()
              << std::endl;
  }

  std::cout << "[" << name << "] combo sent " << count
            << "th time: " << umbralMessage << std::endl;
}

void UmbralKey::init(string &name, int originKey, int *umbralKeys,
                     int umbralKeysLength) {
  if (isInited) {
    cout << "UmbralKey [" << this->name << ": " << umbralMessage
         << "] is already initialized!" << endl;
    return;
  }

  isInited = true;
  this->name = name;
  count = 0;

  // ����ԭʼ����
  this->umbralKeysLength = umbralKeysLength;
  umbralKeysInput = new INPUT[umbralKeysLength];   // ��������ṹ������
  umbralKeysRelease = new INPUT[umbralKeysLength]; // ��������ṹ������
  string *umbralKeysName = new string[umbralKeysLength];

  for (size_t i = 0; i < umbralKeysLength; i++) {
    // ����
    umbralKeysInput[i].type = INPUT_KEYBOARD;
    umbralKeysInput[i].ki.wVk = umbralKeys[i];

    // �ͷ�
    umbralKeysRelease[i].type = INPUT_KEYBOARD;
    umbralKeysRelease[i].ki.wVk = umbralKeys[i];
    umbralKeysRelease[i].ki.dwFlags = KEYEVENTF_KEYUP;

    // ����
    umbralKeysName[i] = keyName(umbralKeys[i]); // ��ȡ��������
  }

  umbralMessage = "'" + keyName(originKey) + "' -> '" +
                  join(umbralKeysName, umbralKeysLength, " + ") + "'";
}

UmbralKey::UmbralKey() {
  isInited = false;
  name = "";
  count = 0;

  originKey = 0;
  umbralKeysLength = 0;
  umbralKeysInput = nullptr;
  umbralKeysRelease = nullptr;
}

UmbralKey::~UmbralKey() {
  if (umbralKeysInput != nullptr) {
    delete[] umbralKeysInput;
  }
  if (umbralKeysRelease != nullptr) {
    delete[] umbralKeysRelease;
  }
}
