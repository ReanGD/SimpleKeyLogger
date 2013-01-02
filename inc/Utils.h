#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void RunAsAdmin(PTCHAR Params);
void AddToAutoload(void);
BOOL IsFisrtRun(void);

template<class T> inline void Delete(T** Obj)
{
	if(*Obj)
	{
		delete (*Obj);
		(*Obj) = NULL;
	}
}

template<class T> inline void DeleteArr(T** Obj)
{
	if(*Obj)
	{
		delete[] (*Obj);
		(*Obj) = NULL;
	}
}