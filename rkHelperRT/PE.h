#pragma once

#ifndef PE_h__
#define PE_h__

FARPROC MyGetProcAddress(HMODULE hModule, LPCSTR lpFunction);
HMODULE MyGetModuleHandle(LPCWSTR lpModule);

typedef HMODULE(WINAPI* PLOADLIBRARYEXW) (
	_In_ LPCWSTR lpLibFileName,
	_Reserved_ HANDLE hFile,
	_In_ DWORD dwFlags
	);
#define GetLoadLibrary() ((PLOADLIBRARYEXW)MyGetProcAddress(MyGetModuleHandle(L"kernelbase.dll"), "LoadLibraryExW"));
#define LoadLibrary(_x_) (GetLoadLibrary()(_x_, NULL, 0));

#endif // PE_h__
