#pragma once

#include "Utils.h"

class CLogger
{
private:
	DWORD m_CacheSize;
	PWCHAR m_WriteBuffer, m_CacheBuffer;	
public:
	CLogger();
	~CLogger();
private:
	BOOL WriteBuffers(LPCVOID lpBuffer1, DWORD nNumberOfBytesToWrite1, LPCVOID lpBuffer2, DWORD nNumberOfBytesToWrite2);
public:	
	BOOL WriteSymbol(WCHAR Symbol);
	BOOL WriteTitle(PWCHAR Title);
	BOOL WriteKeyName(PWCHAR KeyName);
public:
	BOOL Flush(void);
};
