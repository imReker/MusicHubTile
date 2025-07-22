#include "pch.h"
#include <winnt.h>
#include "Shell.h"

using namespace Reker::RTHelper;
using namespace Platform;

int Shell::Launch() {
	HMODULE hMod = GetModuleHandle(L"TaskHostSvcs.dll");
	PLAUNCHSESSION LaunchSession = (PLAUNCHSESSION)GetProcAddress(hMod, "LaunchSession");

	hMod = GetModuleHandle(L"EMCLIENT.dll");
	PGETTASKINSTANCEIDS GetTaskInstanceIDs = (PGETTASKINSTANCEIDS)GetProcAddress(hMod, "GetTaskInstanceIDs");

	uint32 ret;

	INSTANCEID iid = {0};
	DWORD dwTaskId = 0;
	iid.count = 1;
	ret = GetTaskInstanceIDs(&dwTaskId, &iid);
	if (ret < 0)
		return ret;

	WCHAR szUri[60] = L"app://5B04B775-356B-4AA0-AAF8-6491FFEA5630/MusicAndVideoHub";
	ret = LaunchSession(dwTaskId, szUri, NULL, 0, NULL);
	return ret;
}
