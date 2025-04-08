#include "TrayManager.h"

#include <shellapi.h>

void TrayManager::init(HWND hwnd) {
  hWnd = hwnd;
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hWnd;
  nid.uID = 1;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_USER + 1;
  HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
  nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
  (void)lstrcpyn(nid.szTip, TEXT("umbralkeys 正在运行"), _countof(nid.szTip));

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
  menu = CreatePopupMenu();

  if (UmbralKey::GetDisabledAll()) {
    AppendMenu(menu, MF_UNCHECKED, MenuItem::ENABLE,
               I18N::Get(L"当前是禁用", L"Disabled"));
  } else {
    AppendMenu(menu, MF_CHECKED, MenuItem::ENABLE, I18N::Get(L"当前是启用", L"Enabled"));
  }
  add(menu, MenuItem::VIEW_CONFIG, L"查看当前映射", L"View current key mappings");
  add(menu, MenuItem::EDIT_CONFIG, L"编辑映射", L"Edit key mappings");
  add(menu, MenuItem::RELOAD, L"重新加载映射配置", L"Reload key mappings");
  //add(m, MenuItem::ENABLE, L"启用", L"Enable");
  addSeparator(menu);
  add(menu, MenuItem::ABOUT, L"关于影键", L"About UmbralKeys");
  add(menu, MenuItem::EXIT, L"退出", L"Exit");

  SetForegroundWindow(hWnd);  // 解决菜单点击后不消失的问题
  TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
  DestroyMenu(menu);
}
