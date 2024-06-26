/***

Copyright SV Foster, 2023-2024. All rights reserved.

License:
    This program is free for personal, educational and/or non-profit usage    

Revision History:
	Rev 0, DEC 2023
	   First revision

	Rev 1, MAR 2024
	   Added check for the number of directories in the PE/PE+ header, like documentation suggests
	   Code refactoring

***/

#include <Windows.h>
#include <ImageHlp.h>
#include <stdio.h>
#include <tchar.h>
#include "SharedHeaders.h"
#include "resource.h"
#include "GlobalOptions.h"
#include "LanguageRes.h"
#include "CommandLineInterface.h"
#include "Main.h"

#pragma comment(lib, "Imagehlp.lib")
#pragma comment(lib, "Crypt32.lib")

#pragma warning( disable : 6255 )


DWORD _tmain(DWORD argc, LPCTSTR argv[], LPCTSTR envp[])
{
	TGlobalOptions GlobalOptions = { 0 };


	CLISetModeUTF16();
	SetErrorMode(SEM_FAILCRITICALERRORS); // Don't popup on floppy query and etc.
	CLIWriteLN();

	// set options
	CLIWorkModeGet(argc, argv, &GlobalOptions);
	if (GlobalOptions.OperatingMode == OperatingModeHelp)
		return ModeHelp();
	if (!CLISwitchesGet(argc, argv, &GlobalOptions))
		return ExitCodeErrorInit;
	if (!CLIPathsGet(argc, argv, &GlobalOptions))
		return ExitCodeErrorInit;

	if (!GlobalOptions.NoCopyrightLogo)
		CLILogoPrint();
	_tprintf_s(LangGet(UIMSG_101_PROCESSINGFILE), GlobalOptions.PEFileName);

	return ModeUnsigh(&GlobalOptions);
}

DWORD ModeHelp()
{
	CLILogoPrint();
	CLIHelpPrint();

	return ExitCodeOK;
}

DWORD ModeUnsigh(PGlobalOptions GlobalOptions)
{
	DWORD Result = ExitCodeOK;
	HANDLE hFile;
	DWORD CertificateCount;
	PDWORD Indices = NULL;
	DWORD LastError;


	// open the file
	hFile = CreateFile(GlobalOptions->PEFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(LangGet(UIMSG_102_CANT_OPEN_FILE), GetLastError());
		return ExitCodeErrorNoFiles;
	}

	// get all ACEs and check for curruption at the same time
	// ACE = attribute certificate entry
	if (!ACEsFind(hFile, &CertificateCount, &Indices, &LastError))
		if (LastError == ERROR_INVALID_PARAMETER)
		{
			// PE file is corrupted
			_tprintf_s(LangGet(UIMSG_103_FILE_CORRUPTED));

			// apply fix
			if (!DirectoryEntrySecZero(hFile))
				ExitFunction(ExitCodeErrorIO);

			_tprintf_s(LangGet(UIMSG_104_APP_SUCCESS));
			ExitFunction(ExitCodeOK);
		}
		else
			ExitFunction(ExitCodeErrorIO);

	if (!CertificateCount)
	{
		// no sign found
		_tprintf_s(LangGet(UIMSG_105_PE_NOT_SIGNED));
		ExitFunction(ExitCodeErrorNotSigned);
	}
	_tprintf_s(LangGet(UIMSG_116_FOUND_IMAGECERTS_CNT), CertificateCount);

	// print certs to the console
	if (!GlobalOptions->DontPrintCerificates)
		if (!ImageCertificatesPrintAll(hFile, CertificateCount, Indices))
			_tprintf_s(LangGet(UIMSG_120_ERR_ENUM_CERTS));

	// remove all
	if (!ACEsRemoveAll(hFile, CertificateCount, Indices))
		ExitFunction(ExitCodeErrorIO);

	_tprintf_s(LangGet(UIMSG_104_APP_SUCCESS));


function_end:
	free(Indices);

	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);

	if (Result != ExitCodeOK)
		_tprintf_s(LangGet(UIMSG_106_APP_ERROR));

	return Result;
}

BOOL ACEsFind(CONST HANDLE hFile, PDWORD PCertificateCount, PDWORD* Indices, PDWORD LastError)
{
	*LastError = 0;

	if (!ImageEnumerateCertificates(hFile, CERT_SECTION_TYPE_ANY, PCertificateCount, NULL, 0))
	{
		*LastError = GetLastError();
		return FALSE;
	}

	if (!*PCertificateCount)
		return TRUE;

	*Indices = malloc(*PCertificateCount * sizeof(DWORD));
	if (!*Indices)
		return FALSE;

	if (!ImageEnumerateCertificates(hFile, CERT_SECTION_TYPE_ANY, PCertificateCount, *Indices, *PCertificateCount))
		return FALSE;

	return TRUE;
}

BOOL ImageCertificatesPrintAll(CONST HANDLE hFile, CONST DWORD CertificateCount, PDWORD Indices)
{
	DWORD Result = TRUE;
	LPWIN_CERTIFICATE PCertificate = NULL;
	DWORD RequiredLength = 0;
	CRYPT_DATA_BLOB p7Data;


	for (SIZE_T i = 0; i < CertificateCount; ++i)
	{
#pragma warning( push )
#pragma warning( disable : 6387 )
		ImageGetCertificateData(hFile, Indices[i], NULL, &RequiredLength);
		if (!RequiredLength)
			ExitFunction(FALSE);
#pragma warning( pop )

		PCertificate = malloc(RequiredLength);
		if (!PCertificate)
			ExitFunction(FALSE);

		if (!ImageGetCertificateData(hFile, Indices[i], PCertificate, &RequiredLength))
			ExitFunction(FALSE);

		p7Data.cbData = PCertificate->dwLength - sizeof(DWORD) - sizeof(WORD) - sizeof(WORD);
		p7Data.pbData = PCertificate->bCertificate;

		if (!StoreCertificatesPrintAll(Indices[i], p7Data))
			ExitFunction(FALSE);
	}


function_end:
	free(PCertificate);

	return Result;
}

BOOL StoreCertificatesPrintAll(CONST DWORD Index, CONST CRYPT_DATA_BLOB p7Data)
{
	HCERTSTORE CertStore;	
	CHAR signingOID[] = szOID_PKIX_KP_CODE_SIGNING;
	CERT_ENHKEY_USAGE keyUsage;
	keyUsage.cUsageIdentifier = 1;
	keyUsage.rgpszUsageIdentifier = _alloca(sizeof(LPSTR));
	keyUsage.rgpszUsageIdentifier[0] = &signingOID[0];	
	PCCERT_CONTEXT certContext = NULL;	
	int CertCount = 0;


	_tprintf_s(LangGet(UIMSG_107_EXAM_CERTSTORE), Index);
	CertStore = CertOpenStore(CERT_STORE_PROV_PKCS7, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, 0, &p7Data);
	if (!CertStore)
		return FALSE;

	do
	{
		certContext = CertFindCertificateInStore
		(
			CertStore,
			X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			CERT_FIND_EXT_ONLY_ENHKEY_USAGE_FLAG,
			CERT_FIND_ENHKEY_USAGE,
			&keyUsage,
			certContext
		);

		if (certContext)
		{			
			CertificatePrintSingle(CertCount, certContext);
			CertCount++;
		}
	} while (certContext);
	// only non-NULL CERT_CONTEXT that CertFindCertificateInStore() returns must be freed by CertFreeCertificateContext()

	if (CertCount == 0)
		_tprintf_s(LangGet(UIMSG_108_NO_CODESIGN));
	CLIWriteLN();
	
	CertCloseStore(CertStore, CERT_CLOSE_STORE_FORCE_FLAG);

	return TRUE;
}

VOID CertificatePrintSingle(CONST DWORD IndexStore, PCCERT_CONTEXT cert)
{
	DWORD CertType = CERT_X500_NAME_STR;
	DWORD StrSubjSize;
	LPTSTR StrSubj;


	_tprintf_s(LangGet(UIMSG_109_CODESIGN_CERTID), IndexStore);

	StrSubjSize = CertGetNameString(cert, CERT_NAME_RDN_TYPE, 0, &CertType, 0, 0);
	if (!StrSubjSize)
		return;

	StrSubj = _alloca(StrSubjSize * sizeof(TCHAR));
	StrSubjSize = CertGetNameString(cert, CERT_NAME_RDN_TYPE, 0, &CertType, StrSubj, StrSubjSize);
	if (StrSubjSize)
		_tprintf_s(TEXT("%s\n"), StrSubj);
}

BOOL ACEsRemoveAll(CONST HANDLE hFile, CONST DWORD CertificateCount, PDWORD Indices)
{
	_tprintf_s(LangGet(UIMSG_110_REMOVING_ACE));

	for (SIZE_T i = 0; i < CertificateCount; ++i)
	{
		_tprintf_s(LangGet(UIMSG_111_REMOVING_ACE_ID), Indices[i]);
		if (!ImageRemoveCertificate(hFile, Indices[i]))
			return FALSE;
	}

	CLIWriteLN();
	return TRUE;
}

BOOL DirectoryEntrySecZero(CONST HANDLE hFile)
{
	BOOL Result = TRUE;
	DWORD fileSize;
	HANDLE hFileMapping = NULL;
	LPVOID fileBase = NULL;
	PIMAGE_DOS_HEADER HeaderDOS;
	PIMAGE_NT_HEADERS32 ntHeader32;
	PIMAGE_NT_HEADERS64 ntHeader64;


	_tprintf_s(LangGet(UIMSG_112_REMOVING_IDES));

	// Get the file size
	fileSize = GetFileSize(hFile, NULL);
	if (!fileSize)
		ExitFunction(FALSE);

	// Create a file mapping object
	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMapping == NULL) 
		ExitFunction(FALSE);

	// Map the file into memory
	fileBase = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (fileBase == NULL) 
		ExitFunction(FALSE);

	// check DOS header
	HeaderDOS = (PIMAGE_DOS_HEADER)fileBase;
	if (HeaderDOS->e_magic != IMAGE_DOS_SIGNATURE) {
		_tprintf_s(LangGet(UIMSG_122_PE_BAD_HEADERS));
		ExitFunction(FALSE);
	}
	if (HeaderDOS->e_lfanew == 0) {
		_tprintf_s(LangGet(UIMSG_122_PE_BAD_HEADERS));
		ExitFunction(FALSE);
	}

	// set variables for PE headers
	ntHeader64 = (PIMAGE_NT_HEADERS64)((LPBYTE)fileBase + HeaderDOS->e_lfanew);
	ntHeader32 = (PIMAGE_NT_HEADERS32)ntHeader64;

	// signatures for PE32 and PE32+ headers are on the same place, single check in enough
	if (ntHeader64->Signature != IMAGE_NT_SIGNATURE) {
		_tprintf_s(LangGet(UIMSG_122_PE_BAD_HEADERS));
		ExitFunction(FALSE);
	}

	// fix
	switch (ntHeader64->OptionalHeader.Magic)
	{
	// PE executable
	case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
		// Note that the number of directories is not fixed. Before looking for a specific directory,
		// check the NumberOfRvaAndSizes field in the optional header
		// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
		if (ntHeader32->OptionalHeader.NumberOfRvaAndSizes < (IMAGE_DIRECTORY_ENTRY_SECURITY + 1))
		{
			_tprintf_s(LangGet(UIMSG_123_PE_BAD_DIRECTORY_COUNT));
			ExitFunction(FALSE);
		}

		_tprintf_s(LangGet(UIMSG_113_IDES_SIZE), ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
		_tprintf_s(LangGet(UIMSG_114_IDES_VA), ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);

		ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size = 0;
		ntHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress = 0;
		ntHeader32->OptionalHeader.CheckSum = 0;

		break;

	// PE+ executable
	case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
		// Note that the number of directories is not fixed. Before looking for a specific directory,
		// check the NumberOfRvaAndSizes field in the optional header
		// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
		if (ntHeader64->OptionalHeader.NumberOfRvaAndSizes < (IMAGE_DIRECTORY_ENTRY_SECURITY + 1))
		{
			_tprintf_s(LangGet(UIMSG_123_PE_BAD_DIRECTORY_COUNT));
			ExitFunction(FALSE);
		}

		_tprintf_s(LangGet(UIMSG_113_IDES_SIZE), ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
		_tprintf_s(LangGet(UIMSG_114_IDES_VA), ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);

		ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size = 0;
		ntHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress = 0;
		ntHeader64->OptionalHeader.CheckSum = 0;

		break;

	// unknown
	default:
		_tprintf_s(LangGet(UIMSG_122_PE_BAD_HEADERS));
		ExitFunction(FALSE);

		break;
	}

	_tprintf_s(LangGet(UIMSG_115_IDES_ZEROED));


function_end:
	// Unmap the file and close handles
	if (fileBase)
		UnmapViewOfFile(fileBase);

	if (hFileMapping)
		CloseHandle(hFileMapping);

	return Result;
}
