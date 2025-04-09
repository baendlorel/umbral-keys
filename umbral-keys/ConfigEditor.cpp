#include "ConfigEditor.h"

// void ShowConfigEditorWindow(HINSTANCE hInstance) {
//   const wchar_t CLASS_NAME[] = L"ConfigEditorWindow";
//
//   static bool registered = false;
//   if (!registered) {
//     WNDCLASS wc = {};
//     wc.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam,
//                         LPARAM lParam) -> LRESULT {
//       static HWND hEdit;
//       switch (msg) {
//         case WM_CREATE:
//           hEdit = CreateWindowW(
//               L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER |
//               ES_AUTOHSCROLL, 20, 20, 240, 25, hwnd, (HMENU)1, NULL, NULL);
//
//           CreateWindowW(L"BUTTON", L"保存",
//                         WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 100, 60,
//                         80, 25, hwnd, (HMENU)2, NULL, NULL);
//           break;
//
//         case WM_COMMAND:
//           if (LOWORD(wParam) == 2) {
//             wchar_t buffer[256];
//             GetWindowTextW(hEdit, buffer, 256);
//             MessageBoxW(hwnd, buffer, L"你输入的是", MB_OK);
//           }
//           break;
//
//         case WM_CLOSE:
//           DestroyWindow(hwnd);
//           break;
//
//         case WM_DESTROY:
//           return 0;
//       }
//       return DefWindowProc(hwnd, msg, wParam, lParam);
//     };
//     wc.hInstance = hInstance;
//     wc.lpszClassName = CLASS_NAME;
//     RegisterClass(&wc);
//     registered = true;
//   }
//
//   HWND hwnd = CreateWindowExW(
//       0, CLASS_NAME, L"编辑配置", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
//       CW_USEDEFAULT, CW_USEDEFAULT, 300, 150, NULL, NULL, hInstance, NULL);
//
//   HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
//
//   SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);  //
//   大图标（Alt+Tab） SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
//   // 小图标（标题栏）
//
//   ShowWindow(hwnd, SW_SHOWNORMAL);
//   UpdateWindow(hwnd);
// }

INT_PTR CALLBACK ConfigEditorProc(HWND hDlg, UINT message, WPARAM wParam,
                                  LPARAM lParam) {
  static HINSTANCE hInstance =
      (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE);
  static HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
  switch (message) {
    case WM_INITDIALOG:
      MessageBox(NULL, L"centerWindow", L"centerWindow", MB_OK);
      centerWindow(hDlg);
      //SetDlgItemTextW(hDlg, IDC_EDIT_INPUT, L"默认配置内容...");
      SendMessage(hDlg, WM_SETICON, ICON_BIG,
                  (LPARAM)hIcon);  // 大图标（Alt+Tab）
      SendMessage(hDlg, WM_SETICON, ICON_SMALL,
                  (LPARAM)hIcon);  // 小图标（标题栏）
      // 初始化控件等
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}