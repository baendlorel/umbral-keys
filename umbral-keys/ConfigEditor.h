#pragma once
#include <windows.h>
#include "resource.h"
#include "utils.h"

//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 ConfigEditor.rc 使用
//
#define IDD_CONFIG_EDITOR 101
#define IDC_TEXT_EDITOR 1001
#define IDC_HELP_TEXT 1002

// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE 104
#define _APS_NEXT_COMMAND_VALUE 40001
#define _APS_NEXT_CONTROL_VALUE 1005
#define _APS_NEXT_SYMED_VALUE 101
#endif
#endif
//void ShowConfigEditorWindow(HINSTANCE hInstance);
//
INT_PTR CALLBACK ConfigEditorProc(HWND hwnd, UINT message, WPARAM wParam,
                                  LPARAM lParam);

