#pragma once

using namespace Platform;
namespace Reker {
	namespace RTHelper {
		public ref class Shell sealed {
		public:
			static int Launch();
		};
	}
}

typedef enum tagAPPLICATIONTYPE {
	UNKNOW,
	NATIVE,
	SLM,
	XNA
} APPLICATIONTYPE;

typedef struct tagINSTANCEID{
	DWORD count;
	DWORD b;
	DWORD c;
} INSTANCEID, *LPINSTANCEID;

typedef uint32(WINAPI* PLAUNCHSESSION)(UINT taskId, LPCWSTR taskUri, LPBYTE parameterData, UINT parameterDataSize, LPVOID launchSessionCallback);
typedef uint32(WINAPI* PGETTASKINSTANCEIDS)(LPDWORD taskId, LPINSTANCEID lpInstanceID);
