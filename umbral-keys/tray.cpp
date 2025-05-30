#include "tray.h"

void LoadConfig() { UmbralKey::ApplyConfig(Config::Load()); }

void InitializeUmbras() {
  I18N::Initialize();
  LoadConfig();
  UmbralKey::InitializeKeyboardHook();

  wstring info = I18N::Get(
      L"影键（UmbralKeys）启动成功！可以右键点击系统托盘图标查看菜单\n\n",
      L"UmbralKeys is launched! Right click the tray icon to view menu\n\n");
  info.append(UmbralKey::ViewUmbras());
  Logger::MsgBox(info.c_str());
}

// 菜单项回调函数
void UpdateMenuItemState(HMENU hMenu, UINT itemID, bool isDisabled) {
  // 更新菜单项勾选状态
  CheckMenuItem(hMenu, itemID,
                MF_BYCOMMAND | (isDisabled ? MF_UNCHECKED : MF_CHECKED));

  // 更新菜单项的文字（可以显示“启用”或“禁用”）
  ModifyMenu(hMenu, itemID, MF_BYCOMMAND | MF_STRING, itemID,
             isDisabled ? L"影键已启用" : L"影键已禁用");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  static TrayManager tray;
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
          Config::OpenConfigFile();
          break;
        case MenuItem::RELOAD:
          LoadConfig();
          msg.append(I18N::Get(L"重新加载映射配置成功！\n\n",
                               L"Reload key mappings successfully!\n\n"));
          msg.append(UmbralKey::ViewUmbras());
          Logger::MsgBox(msg.c_str());
          break;
        case MenuItem::ENABLE:
          UmbralKey::ToggleAll();
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