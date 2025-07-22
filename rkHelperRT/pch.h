//
// pch.h
// 标准系统包含文件的标头。
//

#pragma once
#define _ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE 1
#define WINAPI_FAMILY WINAPI_FAMILY_DESKTOP_APP
//#define WINAPI_FAMILY WINAPI_FAMILY_APP

#define _WIN32_WINNT 0x0603

#include <windows.h>
//#define LoadLibraryExW __LoadLibraryExW
#undef LoadLibrary
#undef LoadLibraryEx

#include "PE.h"
