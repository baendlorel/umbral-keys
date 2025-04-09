#pragma once
#include <windows.h>
#include <stdio.h>

#include "I18N.h"
#include "TrayManager.h"
#include "UmbralKey.h"
#include "config.h"
#include "consts.h"
#include "utils.h"
#include "ConfigEditor.h"

inline void LoadConfig() { UmbralKey::ApplyConfig(Config::Load()); }

void InitializeUmbras();


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);