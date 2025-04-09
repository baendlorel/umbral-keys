#pragma once
#include <windows.h>
#include "resource.h"

#define IDD_CONFIG_DIALOG 101
#define IDC_EDIT_INPUT 1001
#define IDC_BUTTON_SAVE 1002

void ShowConfigEditorWindow(HINSTANCE hInstance);

INT_PTR CALLBACK ConfigEditorProc(HWND hwnd, UINT message, WPARAM wParam,
                                  LPARAM lParam);