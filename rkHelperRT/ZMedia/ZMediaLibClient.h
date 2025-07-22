#pragma once
#ifndef ZMediaLibClient_h__
#define ZMediaLibClient_h__

#include "PE.h"
#include <zmediatypes.h>
//#include <zmedialib.h>

typedef HRESULT(WINAPI* _ZMediaLib_RequireLibrary)(void);

typedef HRESULT(WINAPI* _ZMediaLib_ReleaseLibrary)(void);

typedef HRESULT(WINAPI* _ZMediaLib_GetItemIntAttribute)(
	_In_ ZMEDIAITEM item,
	     _In_ ZMEDIAITEM_INTATTRIBUTE attrib,
	     _Out_ int* piVal
);

typedef HRESULT(WINAPI* _ZMediaLib_GetItemStringAttribute)(
	_In_ ZMEDIAITEM item,
	     _In_ ZMEDIAITEM_STRINGATTRIBUTE attrib,
	     _Out_writes_opt_(cchVal) WCHAR* szVal,
	     _In_ size_t cchVal,
	     _Out_opt_ size_t* pcchValNeeded
);

typedef HRESULT(WINAPI* _ZMediaLib_GetItemThumbnail)(
	_In_ ZMEDIAITEM item,
	     _In_ ZMEDIAITEM_THUMBTYPE thumbType,
	     _Out_writes_bytes_opt_(cbVal) void* pvVal,
	     _In_ size_t cbVal,
	     _Out_opt_ size_t* pcbValNeeded
);

typedef HRESULT(WINAPI* _ZMediaLib_IsItemArtPresent)(
	_In_ ZMEDIAITEM item,
	     _Out_ BOOL* pfItemArtPresent
);

typedef HRESULT(WINAPI* _ZMediaLib_GetRelatedItem)(
	ZMEDIAITEM item,
	ZMEDIAITEM_RELATION relation,
	ZMEDIAITEM* pItemRelated
);

typedef HRESULT (WINAPI* _ZMediaLib_GetItemStreamOnProperty)(
	_In_ ZMEDIAITEM item,
	     _In_ ZMEDIAITEM_STRINGATTRIBUTE attrib,
	     _Out_ ZMEDIAITEMSTREAM* pStream
);

typedef HRESULT (WINAPI* _ZMediaLib_CloseMediaItemStream)(
	_In_ ZMEDIAITEMSTREAM stream
);


struct ZMediaLib {
	HMODULE m_hLib;
	_ZMediaLib_RequireLibrary ZMediaLib_RequireLibrary;
	_ZMediaLib_ReleaseLibrary ZMediaLib_ReleaseLibrary;
	_ZMediaLib_GetItemIntAttribute ZMediaLib_GetItemIntAttribute;
	_ZMediaLib_GetItemStringAttribute ZMediaLib_GetItemStringAttribute;
	_ZMediaLib_GetItemThumbnail ZMediaLib_GetItemThumbnail;
	_ZMediaLib_IsItemArtPresent ZMediaLib_IsItemArtPresent;
	_ZMediaLib_GetRelatedItem ZMediaLib_GetRelatedItem;
	_ZMediaLib_GetItemStreamOnProperty ZMediaLib_GetItemStreamOnProperty;
	_ZMediaLib_CloseMediaItemStream ZMediaLib_CloseMediaItemStream;

	BOOL m_fRequired;

	ZMediaLib() : m_fRequired(FALSE) {}

	~ZMediaLib() {
		Release();
	}

	HRESULT Require() {
		m_hLib = LoadLibrary(L"ZMediaLib_Client.dll");

		ZMediaLib_RequireLibrary = (_ZMediaLib_RequireLibrary)GetProcAddress(m_hLib, "ZMediaLib_RequireLibrary");
		ZMediaLib_ReleaseLibrary = (_ZMediaLib_ReleaseLibrary)GetProcAddress(m_hLib, "ZMediaLib_ReleaseLibrary");
		ZMediaLib_GetItemIntAttribute = (_ZMediaLib_GetItemIntAttribute)GetProcAddress(m_hLib, "ZMediaLib_GetItemIntAttribute");
		ZMediaLib_GetItemStringAttribute = (_ZMediaLib_GetItemStringAttribute)GetProcAddress(m_hLib, "ZMediaLib_GetItemStringAttribute");
		ZMediaLib_GetItemThumbnail = (_ZMediaLib_GetItemThumbnail)GetProcAddress(m_hLib, "ZMediaLib_GetItemThumbnail");
		ZMediaLib_IsItemArtPresent = (_ZMediaLib_IsItemArtPresent)GetProcAddress(m_hLib, "ZMediaLib_IsItemArtPresent");
		ZMediaLib_GetRelatedItem = (_ZMediaLib_GetRelatedItem)GetProcAddress(m_hLib, "ZMediaLib_GetRelatedItem");
		ZMediaLib_GetItemStreamOnProperty = (_ZMediaLib_GetItemStreamOnProperty)GetProcAddress(m_hLib, "ZMediaLib_GetItemStreamOnProperty");
		ZMediaLib_CloseMediaItemStream = (_ZMediaLib_CloseMediaItemStream)GetProcAddress(m_hLib, "ZMediaLib_CloseMediaItemStream");

		HRESULT hr = S_OK;
		if (!m_fRequired) {
			hr = ZMediaLib_RequireLibrary();
			if (SUCCEEDED(hr)) {
				m_fRequired = TRUE;
			}
		}
		return hr;
	}

	VOID Release() {
		if (m_fRequired) {
			ZMediaLib_ReleaseLibrary();
			m_fRequired = FALSE;
		}
	}
};
#endif // ZMediaLibClient_h__
