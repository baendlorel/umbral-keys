#pragma once
#include <windows.h>

#include "I18N.h"
#include "TrayManager.h"
#include "UmbralKey.h"
#include "config.h"
#include "info.h"
#include "utils.h"

inline void LoadConfig() { UmbralKey::ApplyConfig(Config::Load()); }

inline void InitializeUmbras() {
  I18N::Initialize();
  LoadConfig();
  UmbralKey::InitializeKeyboardHook();

  wstring info = I18N::Get(
      L"影键（UmbralKeys）启动成功！可以右键点击系统托盘图标查看菜单\n\n",
      L"UmbralKeys is launched! Right click the tray icon to view menu\n\n");
  info.append(UmbralKey::ViewUmbras());
  Logger::MsgBox(info.c_str());
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);