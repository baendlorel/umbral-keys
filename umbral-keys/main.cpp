#include <windows.h>

#include "I18N.h"
#include "TrayManager.h"
#include "UmbralKey.h"
#include "config.h"
#include "utils.h"

void LoadConfig() { UmbralKey::ApplyConfig(Config::Load()); }

TrayManager g_tray;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_CREATE:
      g_tray.init(hWnd);
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case MenuItem::EDIT_CONFIG:
          // MessageBox(hWnd, L"配置重新加载（测试用）", L"umbralkeys", MB_OK);
          Config::OpenConfigFile();
          LoadConfig();
          break;
        case MenuItem::ENABLE:
          break;
        case MenuItem::ABOUT:
          Logger::MsgBox(I18N::Get(L"影键", L"UmbralKeys"));
          break;
        case MenuItem::EXIT:
          g_tray.cleanup();
          PostQuitMessage(0);
          break;
      }
      break;

    case WM_USER + 1:
      if (lParam == WM_RBUTTONUP) {
        g_tray.showMenu();
      }
      break;

    case WM_DESTROY:
      g_tray.cleanup();
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow) {
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

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}

// int main() {
//   initialize();
//   return 0;
// }
