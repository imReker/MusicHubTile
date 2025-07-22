#pragma once
#ifndef ZMediaQueueClient_h__
#define ZMediaQueueClient_h__

#include "PE.h"
#include <zmediatypes.h>

typedef HRESULT(WINAPI* _ZMediaQueue_ConnectToService)();
typedef HRESULT(WINAPI* _ZMediaQueue_DisconnectFromService)();
typedef HRESULT(WINAPI* _ZMediaQueue_GetCurrentItem)(DWORD unknow, int *songIndex);
typedef HRESULT(WINAPI* _ZMediaQueue_GetCurrentItemMedia)(DWORD unknow, ZMEDIAITEM *song);
typedef HRESULT(WINAPI* _ZMediaQueue_GetPlayState)(DWORD unknow, ZMEDIAQUEUE_PLAYSTATE *state);
typedef HRESULT(WINAPI* _ZMediaQueue_GetVolume)(DWORD unknow, float *volume);
typedef HRESULT(WINAPI* _ZMediaQueue_IsMuted)(DWORD unknow, BOOL *muted);
typedef HRESULT(WINAPI* _ZMediaQueue_Mute)(DWORD unknow, BOOL mute);
typedef HRESULT(WINAPI* _ZMediaQueue_SetPlayState)(DWORD unknow, ZMEDIAQUEUE_PLAYSTATE state);
typedef HRESULT(WINAPI* _ZMediaQueue_SetVolume)(DWORD unknow, float volume);

struct ZMediaQueue {
	HMODULE m_hLib;
	_ZMediaQueue_ConnectToService ZMediaQueue_ConnectToService;
	_ZMediaQueue_DisconnectFromService ZMediaQueue_DisconnectFromService;
	_ZMediaQueue_GetCurrentItem ZMediaQueue_GetCurrentItem;
	_ZMediaQueue_GetCurrentItemMedia ZMediaQueue_GetCurrentItemMedia;
	_ZMediaQueue_GetPlayState ZMediaQueue_GetPlayState;
	_ZMediaQueue_GetVolume ZMediaQueue_GetVolume;
	_ZMediaQueue_IsMuted ZMediaQueue_IsMuted;
	_ZMediaQueue_Mute ZMediaQueue_Mute;
	_ZMediaQueue_SetPlayState ZMediaQueue_SetPlayState;
	_ZMediaQueue_SetVolume ZMediaQueue_SetVolume;

	BOOL m_fRequired;

	ZMediaQueue() : m_fRequired(FALSE) {}
	~ZMediaQueue() {
		Release();
	}

	HRESULT Require() {
		m_hLib = LoadLibrary(L"ZMediaLib_Client.dll");
		ZMediaQueue_ConnectToService = (_ZMediaQueue_ConnectToService)GetProcAddress(m_hLib, "ZMediaQueue_ConnectToService");
		ZMediaQueue_DisconnectFromService = (_ZMediaQueue_DisconnectFromService)GetProcAddress(m_hLib, "ZMediaQueue_DisconnectFromService");
		ZMediaQueue_GetCurrentItem = (_ZMediaQueue_GetCurrentItem)GetProcAddress(m_hLib, "ZMediaQueue_GetCurrentItem");
		ZMediaQueue_GetCurrentItemMedia = (_ZMediaQueue_GetCurrentItemMedia)GetProcAddress(m_hLib, "ZMediaQueue_GetCurrentItemMedia");
		ZMediaQueue_GetPlayState = (_ZMediaQueue_GetPlayState)GetProcAddress(m_hLib, "ZMediaQueue_GetPlayState");
		ZMediaQueue_GetVolume = (_ZMediaQueue_GetVolume)GetProcAddress(m_hLib, "ZMediaQueue_GetVolume");
		ZMediaQueue_IsMuted = (_ZMediaQueue_IsMuted)GetProcAddress(m_hLib, "ZMediaQueue_IsMuted");
		ZMediaQueue_Mute = (_ZMediaQueue_Mute)GetProcAddress(m_hLib, "ZMediaQueue_Mute");
		ZMediaQueue_SetPlayState = (_ZMediaQueue_SetPlayState)GetProcAddress(m_hLib, "ZMediaQueue_SetPlayState");
		ZMediaQueue_SetVolume = (_ZMediaQueue_SetVolume)GetProcAddress(m_hLib, "ZMediaQueue_SetVolume");

		HRESULT hr = S_OK;
		if (!m_fRequired) {
			hr = ZMediaQueue_ConnectToService();
			if (SUCCEEDED(hr)) {
				m_fRequired = TRUE;
			}
		}
		return hr;
	}

	VOID Release() {
		if (m_fRequired) {
			ZMediaQueue_DisconnectFromService();
			m_fRequired = FALSE;
		}
	}
};

//	ZMediaQueue_AddItem
//		ZMediaQueue_AddItemFromByteStreams
//		ZMediaQueue_CanPlayMultiResVideo
//		ZMediaQueue_CancelApplicationRequests
//		ZMediaQueue_Clear
//		ZMediaQueue_ClearAll_Except
//		ZMediaQueue_CreateBackgroundClientQueue
//		ZMediaQueue_CreateClientQueue
//		ZMediaQueue_CreateClientQueueForURL
//		ZMediaQueue_DecrementProperty
//		ZMediaQueue_DestroyClientQueue
//		ZMediaQueue_DisableApplicationRequests
//		ZMediaQueue_EnableApplicationRequests
//		ZMediaQueue_EnableRadio
//		ZMediaQueue_EnableStationData
//		ZMediaQueue_FindItem
//		ZMediaQueue_GetBackgroundServicePid
//		ZMediaQueue_GetClientQueueType
//		ZMediaQueue_GetConnectivityPlayableItemCount
//		ZMediaQueue_GetErrorState
//		ZMediaQueue_GetFrequency
//		ZMediaQueue_GetFrequencyRange
//		ZMediaQueue_GetItemCount
//		ZMediaQueue_GetItemMediaAt
//		ZMediaQueue_GetNativeVideoRotation
//		ZMediaQueue_GetNativeVideoSize
//		ZMediaQueue_GetPlayDuration
//		ZMediaQueue_GetPlayFlags
//		ZMediaQueue_GetProgressivePlayMode
//		ZMediaQueue_GetProperty
//		ZMediaQueue_GetRadioRegion
//		ZMediaQueue_GetSignalQuality
//		ZMediaQueue_GetStationGenre
//		ZMediaQueue_GetStationText
//		ZMediaQueue_GetStringProperty
//		ZMediaQueue_GetTelemetryData
//		ZMediaQueue_GetVideoBroadcastingPort
//		ZMediaQueue_HasAudio
//		ZMediaQueue_HasVideo
//		ZMediaQueue_IncrementProperty
//		ZMediaQueue_InsertItem
//		ZMediaQueue_InsertItems
//		ZMediaQueue_IsBuffering
//		ZMediaQueue_IsSeeking
//		ZMediaQueue_IsStationDataEnabled
//		ZMediaQueue_LaunchStreamingAgent
//		ZMediaQueue_MoveNextEx
//		ZMediaQueue_MovePrevEx
//		ZMediaQueue_MoveToEx
//		ZMediaQueue_NotifyAgentCompleted
//		ZMediaQueue_NotifyServiceTermination
//		ZMediaQueue_NudgeFrequency
//		ZMediaQueue_PlayItem
//		ZMediaQueue_PlayList
//		ZMediaQueue_RebuildMediaItems
//		ZMediaQueue_RecommendQueueForUrl
//		ZMediaQueue_RegisterForNotifications
//		ZMediaQueue_RemoveItem
//		ZMediaQueue_RestoreContents
//		ZMediaQueue_Resync
//		ZMediaQueue_SaveContents
//		ZMediaQueue_SetFrequency
//		ZMediaQueue_SetPlayDuration
//		ZMediaQueue_SetProgressivePlayMode
//		ZMediaQueue_SetProperty
//		ZMediaQueue_SetRadioRegion
//		ZMediaQueue_SetRepeatSingle
//		ZMediaQueue_SetStringProperty
//		ZMediaQueue_SetVideoSessionHandle
//		ZMediaQueue_StartSeekToFrequency
//		ZMediaQueue_StartSeekToNextStation
//		ZMediaQueue_StopSeek
//		ZMediaQueue_UnregisterForNotifications
//		ZMediaQueue_UnregisterForNotificationsNoWait
//		ZMediaQueue_UpdateCurrentPlayingData
//		ZMediaQueue_WaitForSeekComplete

#endif // ZMediaQueueClient_h__
