#include <windows.h>

#include "I18N.h"
#include "TrayManager.h"
#include "UmbralKey.h"
#include "config.h"
#include "info.h"
#include "utils.h"

void LoadConfig() { UmbralKey::ApplyConfig(Config::Load()); }

TrayManager tray;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_CREATE:
      tray.init(hWnd);
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case MenuItem::VIEW_CONFIG:
          Logger::MsgBox(UmbralKey::ViewUmbras().c_str());
          break;
        case MenuItem::EDIT_CONFIG:
          Config::OpenConfigFile();
          break;
        case MenuItem::RELOAD:
          LoadConfig();
          Logger::MsgBox(I18N::Get(L"重新加载映射配置成功",
                                   L"Reload key mappings successfully"));
          break;
        case MenuItem::ENABLE:
          break;
        case MenuItem::ABOUT:
          Logger::MsgBox(UK_ABOUT);
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

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
  const wchar_t CLASS_NAME[] = L"UmbralKeysWindow";

  WNDCLASS wc = {};
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  HWND hWnd =
      CreateWindowEx(0, CLASS_NAME, L"UmbralKeys", 0, 0, 0, 0, 0, HWND_MESSAGE,
                     NULL, hInstance, NULL);  // HWND_MESSAGE: 无窗口后台运行

  if (!hWnd) {
    return 0;
  }

  I18N::Initialize();
  LoadConfig();
  UmbralKey::InitializeKeyboardHook();
  wstring info = I18N::Get(
      L"影键（UmbralKeys）启动成功！可以右键点击系统托盘图标查看菜单\n\n",
      L"UmbralKeys is launched! Right click the tray icon to view menu\n\n");
  Logger::MsgBox(info.append(UmbralKey::ViewUmbras()).c_str());

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}
