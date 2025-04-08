#include "TrayManager.h"

#include <shellapi.h>

void TrayManager::init(HWND hwnd) {
  hWnd = hwnd;
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hWnd;
  nid.uID = 1;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_USER + 1;
  nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // Ĭ��ͼ��
  lstrcpyn(nid.szTip, TEXT("umbralkeys ��������"), sizeof(nid.szTip));

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

  add(m, MenuItem::VIEW_CONFIG, L"�鿴��ǰӳ��", L"View current key mappings");
  add(m, MenuItem::EDIT_CONFIG, L"�༭ӳ��", L"Edit key mappings");
  add(m, MenuItem::RELOAD, L"���¼���ӳ������", L"Reload key mappings");
  add(m, MenuItem::ENABLE, L"����", L"Enable");
  addSeparator(m);
  add(m, MenuItem::ABOUT, L"����Ӱ��", L"About UmbralKeys");
  add(m, MenuItem::EXIT, L"�˳�", L"Exit");

  SetForegroundWindow(hWnd);  // ����˵��������ʧ������
  TrackPopupMenu(m, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
  DestroyMenu(m);
}
