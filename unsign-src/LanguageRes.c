/***

Copyright SV Foster, 2023. All rights reserved.

License:
    This program is free for personal, educational and/or non-profit usage    

Revision History:
	Rev 0, DEC 2023
	   First revision

***/

#include <Windows.h>
#include "LanguageRes.h"


static CONST TCHAR MessageErrorNoString[] = LanguageResErrorNoString;

LPCTSTR LangGet(CONST UINT uID)
{
	LPCTSTR Result;
	int Length;

	Length = LoadString(GetModuleHandle(NULL), uID, (LPTSTR)&Result, 0);
	if (!Length)
		return (LPCTSTR)&MessageErrorNoString;

	return Result;
}
