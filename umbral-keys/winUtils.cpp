#include "winUtils.h"

void centralize(HWND hwnd) {
  RECT rc;
  GetWindowRect(hwnd, &rc);

  int winWidth = rc.right - rc.left;
  int winHeight = rc.bottom - rc.top;

  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  int x = (screenWidth - winWidth) / 2;
  int y = (screenHeight - winHeight) / 2;

  MoveWindow(hwnd, x, y, winWidth, winHeight, TRUE);
}

void setIcon(HWND hwnd) {
  static HINSTANCE hInstance =
      (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
  static HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));

  SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);  // 大图标（Alt+Tab）
  SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);  // 小图标（标题栏）
}

void initWindow(HWND hwnd) { 
    centralize(hwnd);
    setIcon(hwnd); 
}
