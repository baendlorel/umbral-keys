#include "umbral-key.h"
#include "utils.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    // ��ȡ�����¼�
    KBDLLHOOKSTRUCT *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    if (pKeyboard == NULL) {
      return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    UmbralKey *u = UmbralKey::find(pKeyboard->vkCode);
    if (u == nullptr) {
      // û���ҵ���Ӧ�� UmbralKey ʵ�������������¼�
      return CallNextHookEx(UmbralKey::keyboardHook, nCode, wParam, lParam);
    }
       if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
         u->umbral();
         return 1; // ���� 1 ����ֹ CapsLock ����Ĭ����Ϊ
       }
   
  }

  // �����¼�����һ��������
  return CallNextHookEx(UmbralKey::keyboardHook, nCode, wParam, lParam);
}

// ��̬��Ա

UmbralKey *UmbralKey::instances[] = new UmbralKey *[5];
int UmbralKey::instanceLength = 5;
int UmbralKey::instanceIndex = 5;

void UmbralKey::initKeyboardHook() {
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  if (keyboardHook == NULL) {
    std::cerr << "Failed to install keyboard hook!" << std::endl;
    exit(1);
  }
}

UmbralKey *UmbralKey::find(int originKey) {
  for (size_t i = 0; i <= instanceIndex; i++) {
    if (instances[i].originKey == originKey) {
      return instances[i];
    }
  }
  return nullptr;
}

UmbralKey *UmbralKey::add(string &name, int originKey, int *umbralKeys,
                          int umbralKeysLength) {
  UmbralKey *u = new UmbralKey();
  u->init(name, originKey, umbralKeys, umbralKeysLength);
  if (instanceIndex >= instanceLength - 1) {
    // ��չʵ������
    UmbralKey **newInstances = new UmbralKey *[instanceLength + 5];
    for (int i = 0; i < instanceLength; i++) {
      newInstances[i] = instances[i];
    }
    delete[] instances;
    instances = newInstances;
    instanceLength += 5;
  }
  instanceIndex++;
  instances[instanceIndex] = u;
}

// # ��Ա����

void UmbralKey::umbral() {
  count++;

  // ���Ͱ�������
  SendInput(umbralKeysLength, umbralKeysInput, sizeof(INPUT));
  // �����ͷŵİ���
  SendInput(umbralKeysLength, umbralKeysRelease, sizeof(INPUT));

  std::cout << "[" << name << "] combo sent " << count << "th time: '"
            << keyName(originKey) << "' umbraled to '"
            << join(umbralKeysName, umbralKeysLength, " + ") << "'"
            << std::endl;
}

void UmbralKey::init(string &name, int originKey, int *umbralKeys,
                     int umbralKeysLength) {
  this->name = name;
  count = 0;

  // ����ԭʼ����
  this->umbralKeysLength = umbralKeysLength;
  originKeyName = keyName(originKey);
  umbralKeysInput = new INPUT[umbralKeysLength];   // ��������ṹ������
  umbralKeysRelease = new INPUT[umbralKeysLength]; // ��������ṹ������
  umbralKeysName = new string[umbralKeysLength];

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
}

UmbralKey::~UmbralKey() {
  // �����ڴ�
  delete[] umbralKeysInput;
  delete[] umbralKeysRelease;
  delete[] umbralKeysName;
}
