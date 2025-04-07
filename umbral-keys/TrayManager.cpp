#include "TrayManager.h"

#include <shellapi.h>

void TrayManager::init(HWND hwnd) {
  hWnd = hwnd;
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hWnd;
  nid.uID = 1;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_USER + 1;
  nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // 默认图标
  lstrcpyn(nid.szTip, TEXT("umbralkeys 正在运行"), sizeof(nid.szTip));

  Shell_NotifyIcon(NIM_ADD, &nid);
}

void TrayManager::cleanup() { Shell_NotifyIcon(NIM_DELETE, &nid); }

void add(HMENU m, int id, const WCHAR *zh, const WCHAR *en) {
  AppendMenu(m, MF_STRING, id, I18N::Get(zh, en));
}
void addSeparator(HMENU m) { AppendMenu(m, MF_SEPARATOR, 0, L""); }

void TrayManager::showMenu() {
  POINT pt;
  GetCursorPos(&pt);
  HMENU m = CreatePopupMenu();

  add(m, MenuItem::EDIT_CONFIG, L"编辑键盘映射", L"Edit key mapping");
  add(m, MenuItem::ENABLE, L"启用", L"Enable");
  addSeparator(m);
  add(m, MenuItem::ABOUT, L"关于", L"About");
  add(m, MenuItem::EXIT, L"退出影键", L"Exit");

  SetForegroundWindow(hWnd);  // 解决菜单点击后不消失的问题
  TrackPopupMenu(m, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
  DestroyMenu(m);
}
