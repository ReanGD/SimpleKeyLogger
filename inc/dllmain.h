#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#ifdef _USRDLL
#	define API_FUNC __declspec(dllexport)
#else
#	define API_FUNC __declspec(dllimport)
#endif

extern "C" {
	__declspec(dllexport) BOOL SetHook(HWND hWnd);
	__declspec(dllexport) void ClearHook(void);
}

#define UWM_KEYBOARD_MSG _T("UWM_KEYBOARD_MSG-5bd6ecc9-e6c2-4065-9b9a-36d18bd64cb4")
#define UWM_CBT_MSG _T("UWM_CBT_MSG-a011810e-b4ff-4f35-80db-9dac7256156a")