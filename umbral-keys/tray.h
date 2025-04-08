#pragma once
#include <windows.h>

#include "I18N.h"
#include "TrayManager.h"
#include "UmbralKey.h"
#include "config.h"
#include "info.h"
#include "utils.h"

void LoadConfig();

void InitializeUmbras();


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);