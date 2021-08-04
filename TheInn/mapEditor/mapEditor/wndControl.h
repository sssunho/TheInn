#pragma once
#ifndef __WNDCTRL__
#define __WNDCTRL__
#include "framework.h"
#include <string>

void SetClientSize(HWND hWnd, int nDestClientWidth, int nDestClientHeight);

HWND CreateMDIChild(HWND hWndClient, const WCHAR * className);

std::wstring loadFile(HWND owner, const WCHAR type[]);

std::wstring saveFile(HWND owner, const WCHAR type[]);

#endif
