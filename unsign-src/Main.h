/***

Copyright 2023, SV Foster. All rights reserved.

License:
    This program is free for personal, educational and/or non-profit usage    

Revision History:

***/

#pragma once


// ACE = attribute certificate entry
BOOL  ACEsFind(CONST HANDLE hFile, PDWORD PCertificateCount, PDWORD* Indices, PDWORD LastError);
BOOL  ImageCertificatesPrintAll(CONST HANDLE hFile, CONST DWORD CertificateCount, PDWORD Indices);
BOOL  StoreCertificatesPrintAll(CONST DWORD Index, CONST CRYPT_DATA_BLOB p7Data);
VOID  CertificatePrintSingle(CONST DWORD IndexStore, PCCERT_CONTEXT cert);
BOOL  ACEsRemoveAll(CONST HANDLE hFile, CONST DWORD CertificateCount, PDWORD Indices);
BOOL  DirectoryEntrySecZero(CONST HANDLE hFile);
