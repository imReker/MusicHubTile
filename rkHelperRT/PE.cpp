#include "pch.h"
#include <winnt.h>
#include "PE.h"
#include "winternl.h"
#include "ntfunction.h"

FARPROC MyGetProcAddress(HMODULE hModule, LPCSTR lpFunction) {
	if (hModule == NULL || lpFunction == 0) {
		return 0;
	}

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;

	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return 0;
	}

	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((PBYTE)hModule + pDosHeader->e_lfanew);

	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return 0;
	}

	IMAGE_DATA_DIRECTORY DataDirectory = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	if (DataDirectory.VirtualAddress == 0 || DataDirectory.Size == 0) {
		return 0;
	}

	IMAGE_EXPORT_DIRECTORY* pExportDirectory = (IMAGE_EXPORT_DIRECTORY*)((PBYTE)hModule + DataDirectory.VirtualAddress);
	PDWORD pAddressOfNames = (PDWORD)((PBYTE)hModule + pExportDirectory->AddressOfNames);
	PWORD pAddressOfNameOrdinals = (PWORD)((PBYTE)hModule + pExportDirectory->AddressOfNameOrdinals);
	PDWORD pAddressOfFunctions = (PDWORD)((PBYTE)hModule + pExportDirectory->AddressOfFunctions);

	for (unsigned i = 0; i < pExportDirectory->NumberOfNames; i++) {
		LPCSTR pszName = (LPCSTR)((PBYTE)hModule + pAddressOfNames[i]);
		WORD nIndex = pAddressOfNameOrdinals[i];

		if (strcmp(pszName, lpFunction) == 0) {
			return (FARPROC)(pAddressOfFunctions[nIndex] + (DWORD)hModule);
		}
	}

	return 0;
}

HMODULE MyGetModuleHandle(LPCWSTR lpModule) {
	PPEB pPEB = NtCurrentPeb();
	PPEB_LDR_DATA pLdr = pPEB->Ldr;
	WCHAR lpS[250] = {0};
	PLIST_ENTRY pflink = (PLIST_ENTRY)(pLdr->InLoadOrderModuleList.Flink);
	PLIST_ENTRY pblink = (PLIST_ENTRY)(pLdr->InLoadOrderModuleList.Blink);

	if (NULL == lpModule) {
		PLDR_DATA_TABLE_ENTRY pModuleTab = (PLDR_DATA_TABLE_ENTRY)pflink;
		return (HMODULE)pModuleTab->DllBase;
	}

	for (PLIST_ENTRY ptemp = pflink; ptemp != pblink; ptemp = ptemp->Flink) {
		PLDR_DATA_TABLE_ENTRY pModuleTab = (PLDR_DATA_TABLE_ENTRY)ptemp;

		wcscpy_s(lpS, sizeof(lpS) / sizeof(lpS[0]), pModuleTab->BaseDllName.Buffer);
		_wcslwr_s(lpS, sizeof(lpS) / sizeof(lpS[0]));
		if (_wcsicmp(lpS, lpModule) == 0) {
			return (HMODULE)pModuleTab->DllBase;
		}
	}

	return 0;
}
