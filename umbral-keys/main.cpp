#include <windows.h>

#include "resource.h"
#include "tray.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
  const wchar_t* CLASS_NAME = L"UmbralKeysWindow";

  WNDCLASS wc = {};
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  HWND hWnd =
      CreateWindowEx(0,              // 扩展窗口样式，0 表示没有扩展样式
                     CLASS_NAME,     // 窗口类名，之前注册的窗口类名
                     L"UmbralKeys",  // 窗口标题文本
                     0,              // 窗口样式，0 表示没有指定样式
                     0,              // x 坐标，0 表示由操作系统自动计算
                     0,              // y 坐标，0 表示由操作系统自动计算
                     0,              // 宽度，0 表示默认大小
                     0,              // 高度，0 表示默认大小
                     HWND_MESSAGE,   // 父窗口句柄，这里是消息窗口，没有父窗口
                     NULL,           // 菜单句柄，NULL 表示没有菜单
                     hInstance,      // 应用程序实例句柄
                     NULL);          // 初始化数据，NULL 表示没有初始化数据

  if (!hWnd) {
    return 0;
  }

  setIcon(hWnd);

  MSG msg = {};
  try {
    InitializeUmbras();
    while (GetMessage(&msg, nullptr, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  } catch (const exception& e) {
    Logger::SaveError("failed: " + string(e.what()));
  }
  return (int)msg.wParam;
}
