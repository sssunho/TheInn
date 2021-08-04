#include "ui.h"

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

	//�ػ� ���ϱ�
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);

	//�˸°� ��ȯ�� �����츦 ȭ�� ���߾ӿ� ��ġ�Ѵ�.
	::SetWindowPos(hWnd, NULL,
		nResolutionX / 2 - nWindowWidth / 2,
		nResolutionY / 2 - nWindowHeight / 2,
		nWindowWidth, nWindowHeight, NULL);
}