#include "framework.h"
#include "mapEditor.h"
#include "gridMap.h"
#include <commdlg.h>
#include <string>
#include <map>

using namespace std;

using namespace Gdiplus;

HINSTANCE hInst;

extern map<HWND, TileSetEditor*> tileSetEditorMap;

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TileWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND hEditWnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{

	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;

	if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) return 0;

	
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCEW(IDC_MAPEDITOR);
	WndClass.lpszClassName = _T("Frame Window");
	RegisterClass(&WndClass);
	WndClass.lpfnWndProc = EditWndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Edit Window");
	RegisterClass(&WndClass);
	WndClass.lpfnWndProc = TileWndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Tile Window");
	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Frame Window"),
		_T("MDI Programming"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gpToken);

	return (int)msg.wParam;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	HWND hwndChild;

	switch (message)
	{
	case WM_CREATE:
	{
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(
			_T("MDICLIENT"),
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd,
			NULL,
			hInst,
			(LPSTR)&clientcreate);

		ShowWindow(hwndClient, SW_SHOW);
		return 0;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_NEW:
		{
			CreateMDIChild(hwndClient, L"Edit Window");
		}
			return 0;

		case ID_NEWTILE:
			loadTileSet(hwnd, hwndClient);
			return 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefFrameProc(hwnd, hwndClient, message, wParam, lParam);
}

LRESULT CALLBACK EditWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_LBUTTONDOWN:
		i = 1234;
		break;

	case WM_MOVE:
		break;

	case WM_DESTROY:
		return 0;

	}

	return DefMDIChildProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK TileWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_MOVE:
	{
		if (tileSetEditorMap.find(hwnd) != tileSetEditorMap.end())
		{
			HDC hdc = GetDC(hwnd);
			tileSetEditorMap[hwnd]->renderTileSet(hdc);
			ReleaseDC(hwnd, hdc);
		}
	}
		break;

	case WM_MOUSEMOVE:

		if (tileSetEditorMap.find(hwnd) != tileSetEditorMap.end())
		{
			HDC hdc = GetDC(hwnd);
			tileSetEditorMap[hwnd]->renderSelection(hdc, POINT({ LOWORD(lParam), HIWORD(lParam) }));
			ReleaseDC(hwnd, hdc);

		}
		break;

	case WM_LBUTTONDOWN:
		if (tileSetEditorMap.find(hwnd) != tileSetEditorMap.end())
		{
			tileSetEditorMap[hwnd]->selectTile(POINT({ LOWORD(lParam), HIWORD(lParam) }));
		}
		break;

	case WM_DESTROY:
		tileSetEditorMap.erase(hwnd);
		return 0;

	}

	return DefMDIChildProc(hwnd, message, wParam, lParam);
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