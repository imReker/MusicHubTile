#include "pch.h"
#include "ZMedia.h"

using namespace Reker::RTHelper::ZMedia;

ZMediaLib g_ZMediaLib;
ZMediaQueue g_ZMediaQueue;


/************************************************************************/
/* Zune                                                               */
/************************************************************************/
void Reker::RTHelper::ZMedia::Zune::Init() {
	g_ZMediaLib.Require();
}

void Reker::RTHelper::ZMedia::Zune::Shutdown() {
	g_ZMediaLib.Release();
}


/************************************************************************/
/* MediaQueue                                                          */
/************************************************************************/
MediaQueue^ Reker::RTHelper::ZMedia::MediaQueue::m_Instance = nullptr;

void Reker::RTHelper::ZMedia::MediaQueue::Connect() {
	g_ZMediaQueue.Require();
}

MediaQueue^ Reker::RTHelper::ZMedia::MediaQueue::CreateMediaQueue() {
	if (m_Instance == nullptr) {
		m_Instance = ref new MediaQueue();
	}
	return m_Instance;
}

void Reker::RTHelper::ZMedia::MediaQueue::Disconnect() {
	g_ZMediaQueue.Release();
}

ZMediaState Reker::RTHelper::ZMedia::MediaQueue::State::get() {
	ZMEDIAQUEUE_PLAYSTATE zPlayState = ZMEDIAQUEUE_PLAYSTATE_INVALID;
	HRESULT hr = g_ZMediaQueue.ZMediaQueue_GetPlayState(1, &zPlayState);
	if (SUCCEEDED(hr)) {
		switch (zPlayState) {
		case ZMEDIAQUEUE_PLAYSTATE_INVALID:
		case ZMEDIAQUEUE_PLAYSTATE_UNKNOWN:
		case ZMEDIAQUEUE_PLAYSTATE_STOPPED:
			return ZMediaState::Stopped;
			break;
		case ZMEDIAQUEUE_PLAYSTATE_FASTFORWARDING:
		case ZMEDIAQUEUE_PLAYSTATE_REWINDING:
		case ZMEDIAQUEUE_PLAYSTATE_LOADING:
		case ZMEDIAQUEUE_PLAYSTATE_PLAYING:
			return ZMediaState::Playing;
			break;
		case ZMEDIAQUEUE_PLAYSTATE_PAUSED:
			return ZMediaState::Paused;
			break;
		}
	}
	return ZMediaState::Stopped;
}

Song^ Reker::RTHelper::ZMedia::MediaQueue::CurrentItem::get() {
	Song^ song = nullptr;
	ZMEDIAITEM hSong = 0;

	if (SUCCEEDED(g_ZMediaQueue.ZMediaQueue_GetCurrentItemMedia(1, &hSong))) {
		song = ref new Song(hSong);
	}
	return song;
}


/************************************************************************/
/* Song                                                                 */
/************************************************************************/
Reker::RTHelper::ZMedia::Song::Song(ZMEDIAITEM handle) {
	this->m_Handle = handle;
	this->m_Artist = Reker::RTHelper::ZMedia::Artist::Empty;
	this->m_Album = Reker::RTHelper::ZMedia::Album::Empty;

	if (handle != 0) {
		WCHAR szName[MAX_PATH] = {0};

		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetItemStringAttribute(handle, ZMEDIAITEM_ATTRIBUTE_NAME, szName, MAX_PATH, NULL))) {
			this->m_Name = ref new String(szName);
		} else {
			this->m_Name = L"";
		}
	} else {
		this->m_Name = L"";
	}
}

String^ Reker::RTHelper::ZMedia::Song::Name::get() {
	return this->m_Name;
}

Reker::RTHelper::ZMedia::Artist^ Reker::RTHelper::ZMedia::Song::Artist::get() {
	if ((this->m_Handle != 0) &&
			(this->m_Artist == Reker::RTHelper::ZMedia::Artist::Empty)) {
		ZMEDIAITEM hArtist = 0;
		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetRelatedItem(this->m_Handle, ZMEDIAITEM_RELATION_ARTIST, &hArtist))) {
			this->m_Artist = ref new Reker::RTHelper::ZMedia::Artist(hArtist);
		}
	}
	return this->m_Artist;
}

Reker::RTHelper::ZMedia::Album^ Reker::RTHelper::ZMedia::Song::Album::get() {
	if ((this->m_Handle != 0) &&
			(this->m_Album == Reker::RTHelper::ZMedia::Album::Empty)) {
		ZMEDIAITEM hAlbum = 0;
		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetRelatedItem(this->m_Handle, ZMEDIAITEM_RELATION_ALBUM, &hAlbum))) {
			this->m_Album = ref new Reker::RTHelper::ZMedia::Album(hAlbum);
		}
	}
	return this->m_Album;
}


/************************************************************************/
/* Artist                                                               */
/************************************************************************/
Artist^ Reker::RTHelper::ZMedia::Artist::m_Empty = ref new Reker::RTHelper::ZMedia::Artist();

Reker::RTHelper::ZMedia::Artist::Artist() {
	this->m_Handle = 0;
	this->m_Name = L"";
}

Reker::RTHelper::ZMedia::Artist::Artist(ZMEDIAITEM handle) {
	this->m_Handle = handle;
	if (handle != 0) {
		WCHAR szName[MAX_PATH] = {0};
		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetItemStringAttribute(handle, ZMEDIAITEM_ATTRIBUTE_NAME, szName, MAX_PATH, NULL))) {
			this->m_Name = ref new String(szName);
		} else {
			this->m_Name = L"";
		}
	} else {
		this->m_Name = L"";
	}
}

String^ Reker::RTHelper::ZMedia::Artist::GetArtPath() {
	WCHAR szPath[MAX_PATH] = {0};
	HRESULT hr = g_ZMediaLib.ZMediaLib_GetItemStringAttribute(this->m_Handle, ZMEDIAITEM_ATTRIBUTE_BACKGROUND_PATH, szPath, MAX_PATH, NULL);
	if (SUCCEEDED(hr)) {
		if (this->HasArt) {
			return ref new String(szPath);
		} else {
			return nullptr;
		}
	} else {
		return nullptr;
	}
}

Artist^ Reker::RTHelper::ZMedia::Artist::Empty::get() {
	return m_Empty;
}

String^ Reker::RTHelper::ZMedia::Artist::Name::get() {
	return this->m_Name;
}

bool Reker::RTHelper::ZMedia::Artist::HasArt::get() {
	if (this->m_Handle != 0) {
		ZMEDIAITEMSTREAM pStream = NULL;
		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetItemStreamOnProperty(this->m_Handle, ZMEDIAITEM_ATTRIBUTE_BACKGROUND_PATH, &pStream))) {
			g_ZMediaLib.ZMediaLib_CloseMediaItemStream(pStream);
			return true;
		}
	}
	return false;
}


/************************************************************************/
/* Album                                                               */
/************************************************************************/
Album^ Reker::RTHelper::ZMedia::Album::m_Empty = ref new Reker::RTHelper::ZMedia::Album();

Reker::RTHelper::ZMedia::Album::Album() {
	this->m_Handle = 0;
	this->m_Name = L"";
	this->m_Artist = Reker::RTHelper::ZMedia::Artist::Empty;
}

Reker::RTHelper::ZMedia::Album::Album(ZMEDIAITEM handle) {
	this->m_Handle = handle;
	this->m_Artist = Reker::RTHelper::ZMedia::Artist::Empty;

	if (handle != 0) {
		WCHAR szName[MAX_PATH] = {0};
		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetItemStringAttribute(handle, ZMEDIAITEM_ATTRIBUTE_NAME, szName, MAX_PATH, NULL))) {
			this->m_Name = ref new String(szName);
		} else {
			this->m_Name = L"";
		}
	} else {
		this->m_Name = L"";
	}
}

String^ Reker::RTHelper::ZMedia::Album::GetArtPath() {
	WCHAR szPath[MAX_PATH] = {0};
	if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetItemStringAttribute(this->m_Handle, ZMEDIAITEM_ATTRIBUTE_ARTPATH, szPath, MAX_PATH, NULL))) {
		return ref new String(szPath);
	} else {
		return nullptr;
	}
}

Album^ Reker::RTHelper::ZMedia::Album::Empty::get() {
	return m_Empty;
}

String^ Reker::RTHelper::ZMedia::Album::Name::get() {
	return this->m_Name;
}

bool Reker::RTHelper::ZMedia::Album::HasArt::get() {
	BOOL bHasArt = FALSE;
	if (this->m_Handle != 0) {
		HRESULT hr = g_ZMediaLib.ZMediaLib_IsItemArtPresent(this->m_Handle, &bHasArt);
		if (SUCCEEDED(hr)) {
			return bHasArt != FALSE;
		}
	}
	return false;
}

Reker::RTHelper::ZMedia::Artist^ Reker::RTHelper::ZMedia::Album::Artist::get() {
	if ((this->m_Handle != 0) &&
			(this->m_Artist != Reker::RTHelper::ZMedia::Artist::Empty)) {
		ZMEDIAITEM hArtist = 0;
		if (SUCCEEDED(g_ZMediaLib.ZMediaLib_GetRelatedItem(this->m_Handle, ZMEDIAITEM_RELATION_ARTIST, &hArtist))) {
			this->m_Artist = ref new Reker::RTHelper::ZMedia::Artist(hArtist);
		}
	}
	return this->m_Artist;
}