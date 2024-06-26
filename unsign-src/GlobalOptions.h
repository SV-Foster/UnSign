/***

Copyright SV Foster, 2023. All rights reserved.

License:
    This program is free for personal, educational and/or non-profit usage    

Revision History:
	Rev 0, DEC 2023
	   First revision

***/

#pragma once


typedef enum _OperatingMode
{
	OperatingModeHelp,
	OperatingModeUnsigh
} TOperatingMode;

typedef struct _GlobalOptions
{
	TOperatingMode OperatingMode;	
	LPTSTR PEFileName;
	BOOL DontPrintCerificates;
	BOOL NoCopyrightLogo;
} TGlobalOptions, *PGlobalOptions;
