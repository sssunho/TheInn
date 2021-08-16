#pragma once
#ifndef __WNDCTRL__
#define __WNDCTRL__
#include "framework.h"
#include <string>
#include <vector>

#define L_KEY_STATE (GetAsyncKeyState(0x41))
#define R_KEY_STATE (GetAsyncKeyState(0x44))
#define U_KEY_STATE (GetAsyncKeyState(0x57))
#define D_KEY_STATE (GetAsyncKeyState(0x53))

enum class DIRECTION;

void SetClientSize(HWND hWnd, int nDestClientWidth, int nDestClientHeight);

HWND CreateMDIChild(HWND hWndClient, const WCHAR * className);

std::wstring loadFile(HWND owner, const WCHAR type[]);

std::wstring saveFile(HWND owner, const WCHAR type[]);

void GetFiles(std::vector<std::wstring>& vList, const std::wstring sPath, bool bAllDirectories);

DIRECTION getDirectionKeyState();

#endif
