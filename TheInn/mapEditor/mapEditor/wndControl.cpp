#include "wndControl.h"
#include <string>
#include <commdlg.h>
#include <fstream>

using namespace std;

extern HINSTANCE hInst;

void SetClientSize(HWND hWnd, int nDestClientWidth, int nDestClientHeight)
{
	::SetWindowPos(hWnd, NULL, 0, 0, nDestClientWidth, nDestClientHeight, SWP_NOMOVE);
	RECT stWindowRect;
	RECT stClientRect;
	GetWindowRect(hWnd, &stWindowRect);
	GetClientRect(hWnd, &stClientRect);

	int nClientWidth = stClientRect.right - stClientRect.left;
	int nClientHeight = stClientRect.bottom - stClientRect.top;
	int nWindowWidth = stWindowRect.right - stWindowRect.left;
	int nWindowHeight = stWindowRect.bottom - stWindowRect.top;

	nWindowWidth += nWindowWidth - nClientWidth;
	nWindowHeight += nWindowHeight - nClientHeight;

	//해상도 구하기
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);

	//알맞게 변환된 윈도우를 화면 정중앙에 배치한다.
	::SetWindowPos(hWnd, NULL,
		nResolutionX / 2 - nWindowWidth / 2,
		nResolutionY / 2 - nWindowHeight / 2,
		nWindowWidth, nWindowHeight, NULL);
}

HWND CreateMDIChild(HWND hWndClient, const WCHAR* className)
{

	MDICREATESTRUCT mdicreate;
	mdicreate.szClass = className;
	mdicreate.szTitle = className;
	mdicreate.hOwner = hInst;
	mdicreate.x = CW_USEDEFAULT;
	mdicreate.y = CW_USEDEFAULT;
	mdicreate.cx = CW_USEDEFAULT;
	mdicreate.cy = CW_USEDEFAULT;
	mdicreate.style = 0;
	mdicreate.lParam = 0;
	return (HWND)SendMessage(hWndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
}


wstring loadFile(HWND owner, const WCHAR type[])
{
	OPENFILENAME ofn;
	wfstream tileSetDataFile;

	WCHAR filter[128];
	swprintf_s(filter, L"%s(*.%s*)\0", type, type);
	WCHAR* last = filter + wcslen(filter) + 1;
	swprintf_s(last, 128 - wcslen(filter) - 1, L"*.%s\0", type);

	WCHAR path[512] = L"";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.nMaxFile = 512;
	ofn.Flags = OFN_NOCHANGEDIR;
	ofn.lpstrInitialDir = L"c:\\";
	if (GetOpenFileName(&ofn) == 0)
		return L"";
	else return path;
}

std::wstring saveFile(HWND owner, const WCHAR type[])
{
	OPENFILENAME ofn;
	wfstream tileSetDataFile;
	int i;

	WCHAR filter[128];
	swprintf_s(filter, L"%s(*.%s*)\0", type, type);
	WCHAR* last = filter + wcslen(filter) + 1;
	swprintf_s(last, 128 - wcslen(filter) - 1, L"*.%s\0", type);

	WCHAR path[512] = L"";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.nMaxFile = 512;
	ofn.Flags = OFN_NOCHANGEDIR;
	ofn.lpstrInitialDir = L"c:\\";
	if (GetSaveFileName(&ofn) == 0)
		return L"";

	for (i = wcslen(path) - 1; i >= 0 && path[i] != L'.'; i--);
	if (i < 0)
	{
		WCHAR exp[32] = L".";
		wcscat_s(exp, type);
		wcscat_s(path, exp);
	}

	return path;
}
