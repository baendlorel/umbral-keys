#include "tray.h"

// �˵���ص�����
void UpdateMenuItemState(HMENU hMenu, UINT itemID, bool isDisabled) {
  // ���²˵��ѡ״̬
  CheckMenuItem(hMenu, itemID,
                MF_BYCOMMAND | (isDisabled ? MF_UNCHECKED : MF_CHECKED));

  // ���²˵�������֣�������ʾ�����á��򡰽��á���
  ModifyMenu(hMenu, itemID, MF_BYCOMMAND | MF_STRING, itemID,
             isDisabled ? L"Ӱ��������" : L"Ӱ���ѽ���");
}

void InitializeUmbras() {
  I18N::Initialize();
  LoadConfig();
  UmbralKey::InitializeKeyboardHook();

  wstring info = I18N::Get(
      L"Ӱ����UmbralKeys�������ɹ��������Ҽ����ϵͳ����ͼ��鿴�˵�\n\n",
      L"UmbralKeys is launched! Right click the tray icon to view menu\n\n");
  info.append(UmbralKey::ViewUmbras());
  Logger::MsgBox(info.c_str());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  static TrayManager tray;
  HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

  wstring msg;
  switch (message) {
    case WM_CREATE:
      tray.init(hWnd);
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case MenuItem::VIEW_CONFIG:
          msg.append(UmbralKey::ViewUmbras());
          Logger::MsgBox(msg.c_str());
          break;
        case MenuItem::EDIT_CONFIG:
          //Config::OpenConfigFile();
          //ShowConfigEditorWindow(hInstance);
          DialogBox(GetModuleHandle(NULL),               // Ӧ��ʵ�����
                    MAKEINTRESOURCE(IDD_CONFIG_DIALOG),  // �Ի�����Դ ID
                    hWnd,                                // �����ھ��
                    ConfigEditorProc                     // �Ի�����̺���
          );
          Logger::MsgBox(L"������");
          break;
        case MenuItem::RELOAD:
          LoadConfig();
          msg.append(I18N::Get(L"���¼���ӳ�����óɹ���\n\n",
                               L"Reload key mappings successfully!\n\n"));
          msg.append(UmbralKey::ViewUmbras());
          Logger::MsgBox(msg.c_str());
          break;
        case MenuItem::ENABLE:
          UmbralKey::ToggleAll();
          break;
        case MenuItem::ABOUT:
          Logger::MsgBox(I18N{UK_ABOUT_ZH, UK_ABOUT_EN});
          break;
        case MenuItem::EXIT:
          tray.cleanup();
          PostQuitMessage(0);
          break;
      }
      break;

    case WM_USER + 1:
      if (lParam == WM_RBUTTONUP) {
        tray.showMenu();
      }
      break;

    case WM_DESTROY:
      tray.cleanup();
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}