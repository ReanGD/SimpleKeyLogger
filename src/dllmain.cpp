#include "../inc/dllmain.h"

#pragma data_seg(".SHRD")
HWND hWndServer = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.SHRD,rws")

HINSTANCE hInst;
UINT UWM_KEYBOARD, UWM_CBT;
HHOOK hHookKeyboard = NULL, hHookCBT = NULL;

static LRESULT CALLBACK MsgHookKeyboard(UINT nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK MsgHookCBT(UINT nCode, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	lpReserved;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hInst = static_cast<HINSTANCE>(hModule);
			UWM_KEYBOARD = RegisterWindowMessage(UWM_KEYBOARD_MSG);
			UWM_CBT = RegisterWindowMessage(UWM_CBT_MSG);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			if(hWndServer != NULL)
				ClearHook();
			break;
	}
	return TRUE;
}

__declspec(dllexport) BOOL SetHook(HWND hWnd)
{
	if(hWndServer != NULL)
		return FALSE;

	if(hHookKeyboard == NULL)
		hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)MsgHookKeyboard, hInst, 0);
	if(hHookCBT == NULL)
		hHookCBT = SetWindowsHookEx(WH_CBT, (HOOKPROC)MsgHookCBT, hInst, 0);

	if((hHookKeyboard != NULL) && (hHookCBT != NULL))
	{
		hWndServer = hWnd;
		return TRUE;
	}
	return FALSE;
}

__declspec(dllexport) void ClearHook(void)
{
	if(hHookKeyboard != NULL)
		if(UnhookWindowsHookEx(hHookKeyboard))
			hHookKeyboard = NULL;
	if(hHookCBT != NULL)
		if(UnhookWindowsHookEx(hHookCBT))
			hHookCBT = NULL;

	if((hHookKeyboard == NULL) && (hHookCBT == NULL))
		hWndServer = NULL;
}

static LRESULT CALLBACK MsgHookKeyboard(UINT nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		bool CurStateIsDown = ((lParam & (1<<31)) == 0);
		bool PreviousStateIsUp = ((lParam & (1<<30)) == 0);

		if(CurStateIsDown && PreviousStateIsUp)
		{
			WCHAR wChar;
			UINT ScanCode = ((BYTE*)&lParam)[2];
			BYTE KeyState[256] = {0};

			if(GetKeyboardState(KeyState))
				if(ToUnicode((UINT)wParam, ScanCode, KeyState, &wChar, 1, 0) == 1)
					PostMessage(hWndServer, UWM_KEYBOARD, WPARAM(wChar), lParam);
		}
	}
	return CallNextHookEx(hHookKeyboard, nCode, wParam, lParam);
}

static LRESULT CALLBACK MsgHookCBT(UINT nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HCBT_ACTIVATE)
		PostMessage(hWndServer, UWM_CBT, wParam, lParam);
    return CallNextHookEx(hHookCBT, nCode, wParam, lParam); 
}
