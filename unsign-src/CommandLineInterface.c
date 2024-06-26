/***

Copyright SV Foster, 2023-2024. All rights reserved.

License:
    This program is free for personal, educational and/or non-profit usage    

Revision History:
	Rev 0, DEC 2023
	   First revision

	Rev 1, MAR 2024
	   Code refactoring

***/

#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>
#include "SharedHeaders.h"
#include "resource.h"
#include "GlobalOptions.h"
#include "LanguageRes.h"
#include "CommandLineInterface.h"

#pragma comment(lib, "Version.lib")

#pragma warning( disable : 6255 )


VOID CLIWorkModeGet(CONST DWORD argc, LPCTSTR argv[], PGlobalOptions glo)
{
	glo->OperatingMode = OperatingModeHelp;

	if (argc <= 1)
	{
		return;
	}

	for (DWORD i = 1; i < argc; ++i)
	{
		if (_tcsicmp(argv[i], TEXT("/help")) == 0)
		{
			return;
		}

		if (_tcsicmp(argv[i], TEXT("/?")) == 0)
		{
			return;
		}
	}

	glo->OperatingMode = OperatingModeUnsigh;
}

BOOL CLISwitchesGet(CONST DWORD argc, LPCTSTR argv[], PGlobalOptions glo)
{
	for (DWORD i = 1; i < argc; ++i)
	{
		if (_tcsicmp(argv[i], TEXT("/NoLogo")) == 0)
		{
			glo->NoCopyrightLogo = TRUE;
			continue;
		}

		if (_tcsicmp(argv[i], TEXT("/NoCertPrint")) == 0)
		{
			glo->DontPrintCerificates = TRUE;
			continue;
		}

		if (argv[i][0] == TEXT('/'))
		{
			CLILogoPrint();
			_tprintf_s(LangGet(UIMSG_121_ERR_PARAMS_BAD_SW), argv[i]);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CLIPathsGet(CONST DWORD argc, LPCTSTR argv[], PGlobalOptions glo)
{
	for (DWORD i = 1; i < argc; ++i)
	{		
		if (argv[i][0] == TEXT('/'))
		{
			continue;
		}

		if (glo->PEFileName == NULL)
		{
			glo->PEFileName = (LPTSTR)argv[i];
			continue;
		}

		CLILogoPrint();
		_tprintf_s(LangGet(UIMSG_119_ERR_PARAMS_TOO_MANY_AGRS), argv[i]);
		return FALSE;
	}

	return TRUE;
}

VOID CLILogoPrint()
{
	TCHAR FileName[MAX_PATH + 1];
	DWORD FileNameSize;
	DWORD Handle;
	DWORD FileVersionInfoSize;
	LPVOID BufferData;
	UINT len;
	LPVOID CopyrightString;
	LPVOID ProductNameString;
	LPVOID versionInfo;
	VS_FIXEDFILEINFO* fileInfo;


	FileNameSize = GetModuleFileName(NULL, (LPTSTR)&FileName, MAX_PATH);
	if (!FileNameSize)
		return;
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		return;
	FileName[FileNameSize] = TEXT('\0');

	FileVersionInfoSize = GetFileVersionInfoSize((LPTSTR)&FileName, &Handle);
	if (!FileVersionInfoSize)
		return;

	BufferData = _alloca(FileVersionInfoSize);

	if (!GetFileVersionInfo((LPTSTR)&FileName, 0, FileVersionInfoSize, BufferData))
		return;

	if (!VerQueryValue(BufferData, TEXT("\\"), &versionInfo, &len))
		return;

	fileInfo = (VS_FIXEDFILEINFO*)versionInfo;
	if (fileInfo->dwSignature != 0xfeef04bd)
		return;

	if (!VerQueryValue(BufferData, TEXT("\\StringFileInfo\\040904B0\\FileDescription"), &ProductNameString, &len))
		return;

	if (!VerQueryValue(BufferData, TEXT("\\StringFileInfo\\040904B0\\LegalCopyright"), &CopyrightString, &len))
		return;

	_tprintf_s(LangGet(UIMSG_118_LOGO_TEXT),
		ProductNameString,
		HIWORD(fileInfo->dwProductVersionMS),
		LOWORD(fileInfo->dwProductVersionMS),
		ArchString,
		CopyrightString
	);
}

VOID CLIHelpPrint()
{
	_tprintf_s(LangGet(UIMSG_117_HELP_TEXT));
}

VOID CLIWriteLN()
{
	_tprintf_s(TEXT("\n"));
}

BOOL CLISetModeUTF16()
{
	if (_setmode(_fileno(stdout), _O_U16TEXT) == -1)
		return FALSE;

	if (_setmode(_fileno(stdin), _O_U16TEXT) == -1)
		return FALSE;

	if (_setmode(_fileno(stderr), _O_U16TEXT) == -1)
		return FALSE;

	return TRUE;
}
