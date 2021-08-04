// mapEditor.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "main.h"
#include "mapEditor.h"
#include "wndControl.h"
#include "tileSetEditor.h"
#include "pointVector.h"

#define MAX_LOADSTRING 100

using namespace Gdiplus;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

bool mapLoaded = false;
bool tilesetLoaded = false; 

TilesetEditor* tilesetEditor;
MapEditor* mapEditor; 
std::wstring path;

static HWND hwndClient;
HWND mainWnd;
HWND tileWnd;

int selectedLayer = 1;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Procedures
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MapEditFrameProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MapEditMainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MapEditTileSetProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TileSetEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TileInfoProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MapSettingDlg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;

	if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) return 0;

	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	bool check = true;
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
	check = RegisterClass(&WndClass);
	WndClass.lpfnWndProc = MapEditFrameProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("mapEditor");
	check = RegisterClass(&WndClass);
	WndClass.lpfnWndProc = MapEditMainProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("mapEditMain");
	check = RegisterClass(&WndClass);
	WndClass.lpfnWndProc = MapEditTileSetProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("mapEditTile");
	check = RegisterClass(&WndClass);
	WndClass.lpfnWndProc = TileSetEditProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("tileSetEditor");
	check = RegisterClass(&WndClass);
	WndClass.lpfnWndProc = TileInfoProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("tileInfoEditor");
	check = RegisterClass(&WndClass);

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

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CLIENTCREATESTRUCT clientcreate;
	static HWND mapEditWnd;
	static HWND tileSetWnd;

    switch (message)
    {
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(
			_T("MDICLIENT"),
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, 0, 0,
			hWnd,
			NULL,
			hInst,
			(LPSTR)&clientcreate);

		ShowWindow(hwndClient, SW_SHOW);
		SetTimer(hWnd, 1, 100, NULL);
		return 0;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case ID_MAP_NEW:
			if (!mapLoaded)
			{
				path = loadFile(hWnd, L"mapdata");
				if (path != L"")
				{
					mapLoaded = true;
					mapEditWnd = CreateMDIChild(hwndClient, L"mapEditor");
				}
			}
			break;
			case ID_TILESET_NEW:
			if (!tilesetLoaded)
			{
				path = loadFile(hWnd, L"png");
				if (path != L"")
				{
					tileSetWnd = CreateMDIChild(hwndClient, L"tileSetEditor");
					tilesetEditor = new TilesetEditor(tileSetWnd, path.c_str());
					tilesetLoaded = true;
				}
			}
				break;
			case ID_TILESET_SAVE:
			{
				if (tilesetLoaded)
				{
					tilesetEditor->save();
				}
			}

			case ID_MAP_SAVE:
			{
				if (mapLoaded)
				{
					mapEditor->save();
				}
			}
			break;

		}
		break;


    case WM_DESTROY:
		KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    }
	return DefFrameProc(hWnd, hwndClient, message, wParam, lParam);
}
// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK MapEditFrameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT clientRect;
	static HWND hLayerBtn[3];
	int i;
	switch (message)
	{
	case WM_CREATE:
	{
		ShowWindow(hWnd, SW_SHOW);
		GetClientRect(hWnd, &clientRect);
		SetClientSize(hWnd, 640, 480);
		mainWnd = CreateWindow(L"mapEditMain", L"map editor",
			WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL, 10, 50,
			clientRect.right * 0.7, clientRect.bottom - 50, hWnd, NULL, hInst, NULL);
		tileWnd = CreateWindow(L"mapEditTile", L"tileSet",
			WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL, clientRect.right * 0.7 + 20, 50,
			clientRect.right * 0.3 - 20, clientRect.bottom - 50, hWnd, NULL, hInst, NULL);
		hLayerBtn[0] = CreateWindow(L"button", L"layer1",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			10, 10, 100, 20, hWnd, (HMENU)100, hInst, NULL);
		hLayerBtn[1] = CreateWindow(L"button", L"layer2",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			120, 10, 100, 20, hWnd, (HMENU)101, hInst, NULL);
		hLayerBtn[2] = CreateWindow(L"button", L"layer3",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			230, 10, 100, 20, hWnd, (HMENU)102, hInst, NULL);
		CheckRadioButton(hWnd, 100, 102, 100);
		ShowWindow(tileWnd, SW_SHOW);
		ShowWindow(mainWnd, SW_SHOW);
		mapEditor = new MapEditor(mainWnd, tileWnd, path);
		SetTimer(hWnd, 1, 100, NULL);

	}
		break;

	case WM_TIMER:
		mapEditor->render();
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			mapEditor->setViewPoint(mapEditor->getViewPoint() + POINT({ -10, 0 }));
			break;
		case VK_RIGHT:
			mapEditor->setViewPoint(mapEditor->getViewPoint() + POINT({ 10, 0 }));
			break;
		case VK_UP:
			mapEditor->setViewPoint(mapEditor->getViewPoint() + POINT({ 0, -10 }));
			break;
		case VK_DOWN:
			mapEditor->setViewPoint(mapEditor->getViewPoint() + POINT({ 0, 10 }));
			break;
		}
		break;


	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case 100:
				mapEditor->setLayer(0);
				break;
			case 101:
				mapEditor->setLayer(1);
				break;
			case 102:
				mapEditor->setLayer(2);
				break;
			}
		}
		break;
		
	case WM_RBUTTONDOWN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_MAPSETTING), hWnd, MapSettingDlg);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &clientRect);
		MoveWindow(mainWnd, 10, 50, clientRect.right * 0.7, clientRect.bottom - 50, true);
		MoveWindow(tileWnd, clientRect.right * 0.7 + 20, 50, 
			clientRect.right * 0.3 - 20, clientRect.bottom - 50, true);
		break;

	case WM_DESTROY:
		delete mapEditor;
		KillTimer(hWnd, 1);
		mapLoaded = false;
		break;
	}
	return DefMDIChildProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK MapEditMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Rect clientRect;
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_HSCROLL:
		break;

	case WM_SIZE:
		break;

	case WM_VSCROLL:
		break;

	case WM_LBUTTONDOWN:
		mapEditor->drawTile(POINT({ LOWORD(lParam), HIWORD(lParam) }));
		break;

	case WM_RBUTTONDOWN:
		break;

	case WM_DESTROY:
		break;
	}

	return DefMDIChildProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK MapEditTileSetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TilesetEditor* editor;
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_MOUSEMOVE:
		if (mapEditor != NULL)
		{
			TilesetEditor* temp = mapEditor->getTileWnd();
			temp->setCurrentPos(POINT({ LOWORD(lParam), HIWORD(lParam) }));
		}
		break;

	case WM_LBUTTONDOWN:
		if (mapEditor != NULL)
		{
			TilesetEditor* temp = mapEditor->getTileWnd();
			temp->setSelectPos(POINT({ LOWORD(lParam), HIWORD(lParam) }));
		}
		break;

	case WM_DESTROY:
		break;
	}
	return DefMDIChildProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK TileSetEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hTileInfo;

	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_MOUSEMOVE:
		if (tilesetEditor != NULL)
			tilesetEditor->setCurrentPos(POINT({ LOWORD(lParam), HIWORD(lParam) }));
		break;

	case WM_RBUTTONDOWN:
		hTileInfo = CreateMDIChild(hwndClient, L"tileInfoEditor");
		break;

	case WM_DESTROY:
		delete tilesetEditor;
		tilesetEditor = NULL;
		tilesetLoaded = false;
		break;
	}
	return DefMDIChildProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK TileInfoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT targetPos;

	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_SHOW);
		SetClientSize(hWnd, 32*5, 32*5);
		SetTimer(hWnd, 0, 100, NULL);
		targetPos = tilesetEditor->getCurrentPos();
		tilesetEditor->drawTileInfo(hWnd, targetPos);
		break;

	case WM_TIMER:
		tilesetEditor->drawTileInfo(hWnd, targetPos);
		break;

	case WM_LBUTTONDOWN:
	{
		POINT pos = POINT({ LOWORD(lParam), HIWORD(lParam) });
		pos.x /= 32*5/4; pos.y /= 32 * 5 / 4;
		tilesetEditor->setTileData(targetPos, pos);
	}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		break;
	}

	return DefMDIChildProc(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK MapSettingDlg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static WCHAR sizeX[32];
	static WCHAR sizeY[32];
	switch (message)
	{
	case WM_INITDIALOG:
		swprintf_s(sizeX, L"%d", mapEditor->getNx());
		swprintf_s(sizeY, L"%d", mapEditor->getNy());
		SetDlgItemText(hWnd, IDC_NX, sizeX);
		SetDlgItemText(hWnd, IDC_NY, sizeY);
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hWnd, IDC_NX, sizeX, 32);
			GetDlgItemText(hWnd, IDC_NY, sizeY, 32);
			mapEditor->setSize(_wtoi(sizeX), _wtoi(sizeY));
			EndDialog(hWnd, 0);
			break;

		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;
		}
	}
	}

	return 0;
}