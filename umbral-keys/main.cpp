#include <windows.h>

#include "resource.h"
#include "tray.h"


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

  HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));

  SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);  // 大图标（Alt+Tab）
  SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);  // 小图标（标题栏）

  InitializeUmbras();

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}
