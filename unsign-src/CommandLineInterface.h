/***

Copyright SV Foster, 2023. All rights reserved.

License:
    This program is free for personal, educational and/or non-profit usage    

Revision History:
	See CommandLineInterface.c for details

***/

#pragma once


// File were processed without error
#define ExitCodeOK 0
// No file was found to process
#define ExitCodeErrorNoFiles 1
// File is not signed
#define ExitCodeErrorNotSigned 2
// Initialization error occurred. There is not enough memory or disk space, or you entered 
// an invalid file name or invalid syntax on the command line
#define ExitCodeErrorInit 4
//	File read/write error occurred or file has corrupted or incompatible format
#define ExitCodeErrorIO 5

#if defined(ENV64BIT)
	#define ArchString TEXT("x64 (x86-64)")
#elif defined (ENV32BIT)
	#define ArchString TEXT("x86 (IA-32)")
#else
	#error "Must define either ENV32BIT or ENV64BIT"
#endif


VOID  CLIWorkModeGet(CONST DWORD argc, LPCTSTR argv[], PGlobalOptions glo);
BOOL  CLISwitchesGet(CONST DWORD argc, LPCTSTR argv[], PGlobalOptions glo);
BOOL  CLIPathsGet(CONST DWORD argc, LPCTSTR argv[], PGlobalOptions glo);
VOID  CLILogoPrint();
VOID  CLIHelpPrint();
VOID  CLIWriteLN();
BOOL  CLISetModeUTF16();
