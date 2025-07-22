/************************************************************************
*                                                                       *
*   winternl.h -- This module defines the internal NT APIs and data     *
*       structures that are intended for the use only by internal core  *
*       Windows components.  These APIs and data structures may change  *
*       at any time.                                                    *
*                                                                       *
*   These APIs and data structures are subject to changes from one      *
*       Windows release to another Windows release.  To maintain the    *
*       compatiblity of your application, avoid using these APIs and    *
*       data structures.                                                *
*                                                                       *
*   The appropriate mechanism for accessing the functions defined in    *
*       this header is to use LoadLibrary() for ntdll.dll and           *
*       GetProcAddress() for the particular function.  By using this    *
*       approach, your application will be more resilient to changes    *
*       for these functions between Windows releases.  If a function    *
*       prototype does change, then GetProcAddress() for that function  *
*       might detect the change and fail the function call, which your  *
*       application will be able to detect.  GetProcAddress() may not   *
*       be able to detect all signature changes, thus avoid using these *
*       internal functions.  Instead, your application should use the   *
*       appropriate Win32 function that provides equivalent or similiar *
*       functionality.                                                  *
*                                                                       *
*   Copyright (c) Microsoft Corp. All rights reserved.                  *
*                                                                       *
************************************************************************/

#ifndef _WINTERNL_
#define _WINTERNL_

#if (_WIN32_WINNT >= 0x0500)

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// These data structures and type definitions are needed for compilation and
// use of the internal Windows APIs defined in this header.
//
typedef LONG NTSTATUS;

typedef CONST char *PCSZ;

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;
typedef PSTRING PCANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;
typedef CONST STRING* PCOEM_STRING;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

//
// The PEB and TEB structures are subject to changes between Windows
// releases, thus the fields offsets may change as well as the Reserved
// fields.  The Reserved fields are reserved for use only by the Windows
// operating systems.  Do not assume a maximum size for the structures.
//

//
// Instead of using the BeingDebugged field, use the Win32 APIs
//     IsDebuggerPresent, CheckRemoteDebuggerPresent
// Instead of using the SessionId field, use the Win32 APIs
//     GetCurrentProcessId and ProcessIdToSessionId
// Sample x86 assembly code that gets the SessionId (subject to change
//     between Windows releases, use the Win32 APIs to make your application
//     resilient to changes)
//     mov     eax,fs:[00000018]
//     mov     eax,[eax+0x30]
//     mov     eax,[eax+0x1d4]
//

typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef
VOID
(NTAPI* PPS_POST_PROCESS_INIT_ROUTINE)(
    VOID
);

typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOLEAN Initialized;
	HANDLE SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

//
//  Image loading functions
//

#define DOS_MAX_COMPONENT_LENGTH 255
#define DOS_MAX_PATH_LENGTH (DOS_MAX_COMPONENT_LENGTH + 5 )

typedef struct _CURDIR {
    UNICODE_STRING DosPath;
    HANDLE Handle;
} CURDIR, *PCURDIR;

//
// These bits are always on for console handles and are used for routing
// by windows.
//

#define CONSOLE_HANDLE_SIGNATURE 0x10000003

#define CONSOLE_HANDLE(HANDLE) (((ULONG)(HANDLE) & CONSOLE_HANDLE_SIGNATURE) == CONSOLE_HANDLE_SIGNATURE)


#define CONSOLE_DETACHED_PROCESS -1
#define CONSOLE_NEW_CONSOLE -2
#define CONSOLE_CREATE_NO_WINDOW -3

BOOL WINAPI
CloseConsoleHandle(
    HANDLE hConsole
    );

typedef BOOL
(WINAPI* PCloseConsoleHandle) (
    HANDLE hConsole
    );

//
// Low order 2 bits of handle value used as flag bits.
//

#define RTL_USER_PROC_CURDIR_CLOSE      0x00000002
#define RTL_USER_PROC_CURDIR_INHERIT    0x00000003

typedef struct _RTL_DRIVE_LETTER_CURDIR {
    USHORT Flags;
    USHORT Length;
    ULONG TimeStamp;
    STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

#define RTL_MAX_DRIVE_LETTERS 32
#define RTL_DRIVE_LETTER_VALID (USHORT)0x0001

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG MaximumLength;
    ULONG Length;

    ULONG Flags;
    ULONG DebugFlags;

    HANDLE ConsoleHandle;
    ULONG  ConsoleFlags;
    HANDLE StandardInput;
    HANDLE StandardOutput;
    HANDLE StandardError;

    CURDIR CurrentDirectory;        // ProcessParameters
    UNICODE_STRING DllPath;         // ProcessParameters
    UNICODE_STRING ImagePathName;   // ProcessParameters
    UNICODE_STRING CommandLine;     // ProcessParameters
    PVOID Environment;              // NtAllocateVirtualMemory

    ULONG StartingX;
    ULONG StartingY;
    ULONG CountX;
    ULONG CountY;
    ULONG CountCharsX;
    ULONG CountCharsY;
    ULONG FillAttribute;

    ULONG WindowFlags;
    ULONG ShowWindowFlags;
    UNICODE_STRING WindowTitle;     // ProcessParameters
    UNICODE_STRING DesktopInfo;     // ProcessParameters
    UNICODE_STRING ShellInfo;       // ProcessParameters
    UNICODE_STRING RuntimeData;     // ProcessParameters
    RTL_DRIVE_LETTER_CURDIR CurrentDirectores[ RTL_MAX_DRIVE_LETTERS ];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_FREE_BLOCK
{
     struct _PEB_FREE_BLOCK *Next;
     ULONG Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef struct _PEB {
     BOOLEAN InheritedAddressSpace;
     BOOLEAN ReadImageFileExecOptions;
     BOOLEAN BeingDebugged;
	 union {
		 BOOLEAN BitField;
		 struct {
			 BOOLEAN ImageUsesLargePages: 1;
			 BOOLEAN IsProtectedProcess: 1;
			 BOOLEAN IsLegacyProcess: 1;
			 BOOLEAN IsImageDynamicallyRelocated: 1;
			 BOOLEAN SpareBits: 4;
		 };
	 };
     HANDLE Mutant;
     PVOID ImageBaseAddress;
     PPEB_LDR_DATA Ldr;
     PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
     PVOID SubSystemData;
     PVOID ProcessHeap;
     PRTL_CRITICAL_SECTION FastPebLock;
     PVOID AtlThunkSListPtr;     // Used only for AMD64
     PVOID IFEOKey;
	 union {
		 ULONG CrossProcessFlags;
		 struct {
			 ULONG ProcessInJob: 1;
			 ULONG ProcessInitializing: 1;
			 ULONG ReservedBits0: 30;
		 };
	 };
     union
     {
          PVOID KernelCallbackTable;
          PVOID UserSharedInfoPtr;
     };
     ULONG SystemReserved[1];
     ULONG SpareUlong;
     PPEB_FREE_BLOCK FreeList;
     ULONG TlsExpansionCounter;
     PVOID TlsBitmap;
     ULONG TlsBitmapBits[2];
     PVOID ReadOnlySharedMemoryBase;
     PVOID HotpatchInformation; //ReadOnlySharedMemoryHeap?
     PVOID *ReadOnlyStaticServerData;
     PVOID AnsiCodePageData;
     PVOID OemCodePageData;
     PVOID UnicodeCaseTableData;
     ULONG NumberOfProcessors;
     ULONG NtGlobalFlag;
     LARGE_INTEGER CriticalSectionTimeout;
     SIZE_T HeapSegmentReserve;
     SIZE_T HeapSegmentCommit;
     SIZE_T HeapDeCommitTotalFreeThreshold;
     SIZE_T HeapDeCommitFreeBlockThreshold;
     ULONG NumberOfHeaps;
     ULONG MaximumNumberOfHeaps;
     PVOID *ProcessHeaps;
     PVOID GdiSharedHandleTable;
     PVOID ProcessStarterHelper;
     ULONG GdiDCAttributeList;
     PRTL_CRITICAL_SECTION LoaderLock;
     ULONG OSMajorVersion;
     ULONG OSMinorVersion;
     WORD OSBuildNumber;
     WORD OSCSDVersion;
     ULONG OSPlatformId;
     ULONG ImageSubsystem;
     ULONG ImageSubsystemMajorVersion;
     ULONG ImageSubsystemMinorVersion;
     ULONG ImageProcessAffinityMask;
     ULONG GdiHandleBuffer[34];
     PVOID PostProcessInitRoutine;
     PVOID TlsExpansionBitmap;
     ULONG TlsExpansionBitmapBits[32];
     ULONG SessionId;
     ULARGE_INTEGER AppCompatFlags;
     ULARGE_INTEGER AppCompatFlagsUser;
     PVOID pShimData;
     PVOID AppCompatInfo;
     UNICODE_STRING CSDVersion;
     PVOID ActivationContextData;
     PVOID ProcessAssemblyStorageMap;
     PVOID SystemDefaultActivationContextData;
     PVOID SystemAssemblyStorageMap;
     ULONG MinimumStackCommit;
     PVOID FlsCallback;
     LIST_ENTRY FlsListHead;
     PVOID FlsBitmap;
     ULONG FlsBitmapBits[4];
     ULONG FlsHighIndex;
     PVOID WerRegistrationData;
     PVOID WerShipAssertPtr;
} PEB, *PPEB;

//
// Instead of using the Tls fields, use the Win32 TLS APIs
//     TlsAlloc, TlsGetValue, TlsSetValue, TlsFree
//
// Instead of using the ReservedForOle field, use the COM API
//     CoGetContextToken
//

//
//  Fusion/sxs thread state information
//

typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME
{
     struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME *Previous;
     PVOID ActivationContext;
     ULONG Flags;
} RTL_ACTIVATION_CONTEXT_STACK_FRAME, *PRTL_ACTIVATION_CONTEXT_STACK_FRAME;

typedef struct _GDI_TEB_BATCH
{
     ULONG Offset;
     ULONG HDC;
     ULONG Buffer[310];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

#define ACTIVATION_CONTEXT_STACK_FLAG_QUERIES_DISABLED (0x00000001)

typedef struct _ACTIVATION_CONTEXT_STACK
{
     PRTL_ACTIVATION_CONTEXT_STACK_FRAME ActiveFrame;
     LIST_ENTRY FrameListCache;
     ULONG Flags;
     ULONG NextCookieSequenceNumber;
     ULONG StackId;
} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT
{
     ULONG Flags;
     CHAR * FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;
typedef const TEB_ACTIVE_FRAME_CONTEXT *PCTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME
{
     ULONG Flags;
     struct _TEB_ACTIVE_FRAME *Previous;
     PCTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

typedef struct _TEB {
	NT_TIB NtTib;
	PVOID EnvironmentPointer;
	CLIENT_ID ClientId;
	PVOID ActiveRpcHandle;
	PVOID ThreadLocalStoragePointer;
	PPEB ProcessEnvironmentBlock;
	ULONG LastErrorValue;
	ULONG CountOfOwnedCriticalSections;
	PVOID CsrClientThread;
	PVOID Win32ThreadInfo;
	ULONG User32Reserved[26];
	ULONG UserReserved[5];
	PVOID WOW32Reserved;
	ULONG CurrentLocale;
	ULONG FpSoftwareStatusRegister;
	PVOID SystemReserved1[54];
	LONG ExceptionCode;
	PACTIVATION_CONTEXT_STACK ActivationContextStackPointer;
	UCHAR SpareBytes1[36];
	ULONG TxFsContext;
	GDI_TEB_BATCH GdiTebBatch;
	CLIENT_ID RealClientId;
	HANDLE GdiCachedProcessHandle;
	ULONG GdiClientPID;
	ULONG GdiClientTID;
	PVOID GdiThreadLocalInfo;
	ULONG_PTR Win32ClientInfo[62];
	PVOID glDispatchTable[233];
	ULONG glReserved1[29];
	PVOID glReserved2;
	PVOID glSectionInfo;
	PVOID glSection;
	PVOID glTable;
	PVOID glCurrentRC;
	PVOID glContext;
	ULONG LastStatusValue;
	UNICODE_STRING StaticUnicodeString;
	WCHAR StaticUnicodeBuffer[261];
	PVOID DeallocationStack;
	PVOID TlsSlots[64];
	LIST_ENTRY TlsLinks;
	PVOID Vdm;
	PVOID ReservedForNtRpc;
	PVOID DbgSsReserved[2];
	ULONG HardErrorMode;
	PVOID Instrumentation[9];
	GUID ActivityId;
	PVOID SubProcessTag;
	PVOID EtwLocalData;
	PVOID EtwTraceData;
	PVOID WinSockData;
	ULONG GdiBatchCount;
	BOOLEAN InDbgPrint;
    BOOLEAN FreeStackOnTermination;
    BOOLEAN HasFiberData;
	UCHAR IdealProcessor;
	ULONG GuaranteedStackBytes;
	PVOID ReservedForPerf;
    PVOID ReservedForOle;
	ULONG WaitingOnLoaderLock;
	// Windows 2000 end
	PVOID SavedPriorityState;
	ULONG SoftPatchPtr1;
	PVOID ThreadPoolData;
    PVOID TlsExpansionSlots;
	LCID ImpersonationLocale;
	ULONG IsImpersonating;
	PVOID NlsCache;
	PVOID pShimData;
	ULONG HeapVirtualAffinity;
	HANDLE CurrentTransactionHandle;
	PTEB_ACTIVE_FRAME ActiveFrame;
	PVOID FlsData;
	PVOID PreferredLanguages;
	PVOID UserPrefLanguages;
	PVOID MergedPrefLanguages;
	ULONG MuiImpersonation;
	WORD CrossTebFlags;
	ULONG SpareCrossTebBits: 16;
	WORD SameTebFlags;
	ULONG DbgSafeThunkCall: 1;
	ULONG DbgInDebugPrint: 1;
	ULONG DbgHasFiberData: 1;
	ULONG DbgSkipThreadAttach: 1;
	ULONG DbgWerInShipAssertCode: 1;
	ULONG DbgRanProcessInit: 1;
	ULONG DbgClonedThread: 1;
	ULONG DbgSuppressDebugMsg: 1;
	ULONG SpareSameTebBits: 8;
	PVOID TxnScopeEnterCallback;
	PVOID TxnScopeExitCallback;
	PVOID TxnScopeContext;
	ULONG LockCount;
	ULONG ProcessRundown;
	UINT64 LastSwitchTime;
	UINT64 TotalSwitchOutTime;
	LARGE_INTEGER WaitReasonBitMap;
} TEB;
typedef TEB *PTEB;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );

#if defined(_M_IA64)

typedef struct _FRAME_POINTERS {
    ULONGLONG MemoryStackFp;
    ULONGLONG BackingStoreFp;
} FRAME_POINTERS, *PFRAME_POINTERS;

#define UNWIND_HISTORY_TABLE_SIZE 12

typedef struct _RUNTIME_FUNCTION {
    ULONG BeginAddress;
    ULONG EndAddress;
    ULONG UnwindInfoAddress;
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;

typedef struct _UNWIND_HISTORY_TABLE_ENTRY {
    ULONG64 ImageBase;
    ULONG64 Gp;
    PRUNTIME_FUNCTION FunctionEntry;
} UNWIND_HISTORY_TABLE_ENTRY, *PUNWIND_HISTORY_TABLE_ENTRY;

typedef struct _UNWIND_HISTORY_TABLE {
    ULONG Count;
    UCHAR Search;
    ULONG64 LowAddress;
    ULONG64 HighAddress;
    UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
} UNWIND_HISTORY_TABLE, *PUNWIND_HISTORY_TABLE;

#endif // _M_IA64

typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS	ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR	AffinityMask;
	ULONG_PTR	BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR	InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER Reserved1[2];
    ULONG Reserved2;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    BYTE Reserved1[52];
    PVOID Reserved2[3];
    HANDLE UniqueProcessId;
    PVOID Reserved3;
    ULONG HandleCount;
    BYTE Reserved4[4];
    PVOID Reserved5[11];
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER Reserved6[6];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION {
    ULONG RegistryQuotaAllowed;
    ULONG RegistryQuotaUsed;
    PVOID Reserved1;
} SYSTEM_REGISTRY_QUOTA_INFORMATION, *PSYSTEM_REGISTRY_QUOTA_INFORMATION;

typedef struct _SYSTEM_BASIC_INFORMATION {
    BYTE Reserved1[24];
    PVOID Reserved2[4];
    CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_TIMEOFDAY_INFORMATION {
    BYTE Reserved1[48];
} SYSTEM_TIMEOFDAY_INFORMATION, *PSYSTEM_TIMEOFDAY_INFORMATION;

typedef struct _SYSTEM_PERFORMANCE_INFORMATION {
    BYTE Reserved1[312];
} SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_EXCEPTION_INFORMATION {
    BYTE Reserved1[16];
} SYSTEM_EXCEPTION_INFORMATION, *PSYSTEM_EXCEPTION_INFORMATION;

typedef struct _SYSTEM_LOOKASIDE_INFORMATION {
    BYTE Reserved1[32];
} SYSTEM_LOOKASIDE_INFORMATION, *PSYSTEM_LOOKASIDE_INFORMATION;

typedef struct _SYSTEM_INTERRUPT_INFORMATION {
    BYTE Reserved1[24];
} SYSTEM_INTERRUPT_INFORMATION, *PSYSTEM_INTERRUPT_INFORMATION;

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
    ProcessWow64Information = 26
} PROCESSINFOCLASS;

typedef enum _THREADINFOCLASS {
    ThreadIsIoPending = 16
} THREADINFOCLASS;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation = 0,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3,
    SystemProcessThreadInfo = 5,
    SystemProcessorPerformanceInformation = 8,
    SystemInterruptInformation = 23,
    SystemExceptionInformation = 33,
    SystemRegistryQuotaInformation = 37,
    SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

#if (_WIN32_WINNT >= 0x0501)
//
// use the WTS API instead
//     WTSGetActiveConsoleSessionId
// The active console id is cached as a volatile ULONG in a constant
// memory location.  This x86 memory location is subject to changes between
// Windows releases.  Use the WTS API to make your application resilient to
// changes.
//
#define INTERNAL_TS_ACTIVE_CONSOLE_ID ( *((volatile ULONG*)(0x7ffe02d8)) )
#endif // (_WIN32_WINNT >= 0x0501)

//
// These functions are intended for use by internal core Windows components
// since these functions may change between Windows releases.
//

#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))

//
// use the Win32 API instead
//     CloseHandle
//
NTSTATUS
NtClose (
    IN HANDLE Handle
    );

//
// use the Win32 API instead
//     CreateFile
//
NTSTATUS
NtCreateFile (
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
    );

//
// use the Win32 API instead
//     CreateFile
//
NTSTATUS
NtOpenFile (
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess,
    IN ULONG OpenOptions
    );

//
// use the Win32 API instead
//     DeviceIoControl
//
NTSTATUS
NtDeviceIoControlFile (
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
    );

//
// use the Win32 API instead
//     WaitForSingleObjectEx
//
NTSTATUS
NtWaitForSingleObject (
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

//
// use the Win32 API instead
//     CheckNameLegalDOS8Dot3
//
BOOLEAN
RtlIsNameLegalDOS8Dot3 (
    IN PUNICODE_STRING Name,
    IN OUT POEM_STRING OemName OPTIONAL,
    IN OUT PBOOLEAN NameContainsSpaces OPTIONAL
    );

//
// use the Win32 APIs instead
//     GetProcessHandleCount
//     GetProcessId
//
NTSTATUS
NtQueryInformationProcess (
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

//
// use the Win32 API instead
//     GetThreadIOPendingFlag
//
NTSTATUS
NtQueryInformationThread (
    IN HANDLE ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

//
// use the Win32 APIs instead
//     GetSystemRegistryQuota
//     GetSystemTimes
// use the CryptoAPIs instead for generating random data
//     CryptGenRandom
//
NTSTATUS
NtQuerySystemInformation (
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

//
// use the Win32 API instead
//     GetSystemTimeAsFileTime
//
NTSTATUS
NtQuerySystemTime (
    OUT PLARGE_INTEGER SystemTime
    );

//
// use the Win32 API instead
//     LocalFileTimeToFileTime
//
NTSTATUS
RtlLocalTimeToSystemTime (
    IN PLARGE_INTEGER LocalTime,
    OUT PLARGE_INTEGER SystemTime
    );

//
// use the Win32 API instead
//     SystemTimeToFileTime to convert to FILETIME structures
//     copy the resulting FILETIME structures to ULARGE_INTEGER structures
//     perform the calculation
//
BOOLEAN
RtlTimeToSecondsSince1970 (
    PLARGE_INTEGER Time,
    PULONG ElapsedSeconds
    );

//
// These APIs might be need for some of the internal Windows functions,
// defined in this header file.
//
VOID
RtlFreeAnsiString (
    PANSI_STRING AnsiString
    );

VOID
RtlFreeUnicodeString (
    PUNICODE_STRING UnicodeString
    );

VOID
RtlFreeOemString(
    POEM_STRING OemString
    );

VOID
RtlInitString (
    PSTRING DestinationString,
    PCSZ SourceString
    );

VOID
RtlInitAnsiString (
    PANSI_STRING DestinationString,
    PCSZ SourceString
    );

VOID
RtlInitUnicodeString (
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

NTSTATUS
RtlAnsiStringToUnicodeString (
    PUNICODE_STRING DestinationString,
    PCANSI_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

NTSTATUS
RtlUnicodeStringToAnsiString (
    PANSI_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

NTSTATUS
RtlUnicodeStringToOemString(
    POEM_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

//
// Use the Win32 API instead
//     WideCharToMultiByte
//     set CodePage to CP_ACP
//     set cbMultiByte to 0
//
NTSTATUS
RtlUnicodeToMultiByteSize(
    PULONG BytesInMultiByteString,
    IN PWSTR UnicodeString,
    ULONG BytesInUnicodeString
    );

//
// Use the C runtime function instead
//     strtol
//
NTSTATUS
RtlCharToInteger (
    PCSZ String,
    ULONG Base,
    PULONG Value
    );

//
// use the Win32 API instead
//     ConvertSidToStringSid
//
NTSTATUS
RtlConvertSidToUnicodeString (
    PUNICODE_STRING UnicodeString,
    PSID Sid,
    BOOLEAN AllocateDestinationString
    );

//
// use the CryptoAPIs instead
//     CryptGenRandom
//
ULONG
RtlUniform (
    PULONG Seed
    );

#define LOGONID_CURRENT     ((ULONG)-1)
#define SERVERNAME_CURRENT  ((HANDLE)NULL)

typedef enum _WINSTATIONINFOCLASS {
    WinStationInformation = 8
} WINSTATIONINFOCLASS;


typedef struct _WINSTATIONINFORMATIONW {
    BYTE Reserved2[70];
    ULONG LogonId;
    BYTE Reserved3[1140];
} WINSTATIONINFORMATIONW, * PWINSTATIONINFORMATIONW;

//
// this function is implemented in winsta.dll (you need to loadlibrary to call this function)
// this internal function retrives the LogonId (also called SessionId) for the current process
// You should avoid using this function as it can change. you can retrieve the same information 
// Using public api WTSQuerySessionInformation. Pass WTSSessionId as the WTSInfoClass parameter
// 
typedef BOOLEAN (WINAPI * PWINSTATIONQUERYINFORMATIONW)(
    HANDLE, ULONG, WINSTATIONINFOCLASS, PVOID, ULONG, PULONG );

typedef enum _NT_PRODUCT_TYPE {
    NtProductWinNt = 1,
    NtProductLanManNt,
    NtProductServer
} NT_PRODUCT_TYPE, *PNT_PRODUCT_TYPE;

typedef struct _KSYSTEM_TIME {
    ULONG LowPart;
    LONG High1Time;
    LONG High2Time;
} KSYSTEM_TIME, *PKSYSTEM_TIME;

typedef struct _KUSER_SHARED_DATA {
    ULONG TickCountLowDeprecated;
    ULONG TickCountMultiplier;
    volatile KSYSTEM_TIME InterruptTime;
    volatile KSYSTEM_TIME SystemTime;
    volatile KSYSTEM_TIME TimeZoneBias;
    USHORT ImageNumberLow;
    USHORT ImageNumberHigh;
    WCHAR NtSystemRoot[260];
    ULONG MaxStackTraceDepth;
    ULONG CryptoExponent;
    ULONG TimeZoneId;
    ULONG LargePageMinimum;
    ULONG Reserved2[7];
    NT_PRODUCT_TYPE NtProductType;
    BOOLEAN ProductTypeIsValid;
    ULONG NtMajorVersion;
    ULONG NtMinorVersion;
    BOOLEAN ProcessorFeatures[64];
    ULONG Reserved1;
    ULONG Reserved3;
    volatile ULONG TimeSlip;
    ULONG AlternativeArchitecture;
    ULONG AltArchitecturePad[1];
    LARGE_INTEGER SystemExpirationDate;
    ULONG SuiteMask;
    BOOLEAN KdDebuggerEnabled;
    UCHAR NXSupportPolicy;
    volatile ULONG ActiveConsoleId;
    volatile ULONG DismountCount;
    ULONG ComPlusPackage;
    ULONG LastSystemRITEventTickCount;
    ULONG NumberOfPhysicalPages;
    BOOLEAN SafeBootMode;
    union {
        UCHAR TscQpcData;
        struct {
            UCHAR TscQpcEnabled   : 1;
            UCHAR TscQpcSpareFlag : 1;
            UCHAR TscQpcShift     : 6;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
    UCHAR TscQpcPad[2];
    union {
        ULONG SharedDataFlags;
        struct {
            ULONG DbgErrorPortPresent       : 1;
            ULONG DbgElevationEnabled       : 1;
            ULONG DbgVirtEnabled            : 1;
            ULONG DbgInstallerDetectEnabled : 1;
            ULONG DbgSystemDllRelocated     : 1;
            ULONG DbgDynProcessorEnabled    : 1;
            ULONG DbgSEHValidationEnabled   : 1;
            ULONG SpareBits                 : 25;
        } DUMMYSTRUCTNAME2;
    } DUMMYUNIONNAME2;
    ULONG DataFlagsPad[1];
    ULONGLONG TestRetInstruction;
    ULONG SystemCall;
    ULONG SystemCallReturn;
    ULONGLONG SystemCallPad[3];
    union {
        volatile ULONG64 TickCount;
        volatile KSYSTEM_TIME TickCountQuad;
        struct {
            ULONG ReservedTickCountOverlay[3];
            ULONG TickCountPad[1];
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME3;
    ULONG Cookie;
    ULONG CookiePad[1];
    LONGLONG ConsoleSessionForegroundProcessId;
    ULONG Wow64SharedInformation[16];
    USHORT UserModeGlobalLogger[16];
    ULONG ImageFileExecutionOptions;
    ULONG LangGenerationCount;
    ULONGLONG Reserved5;
    volatile ULONG64 InterruptTimeBias;
    volatile ULONG64 TscQpcBias;
    volatile ULONG ActiveProcessorCount;
    volatile USHORT ActiveGroupCount;
    USHORT Reserved4;
    volatile ULONG AitSamplingValue;
    volatile ULONG AppCompatFlag;
    ULONGLONG SystemDllNativeRelocation;
    ULONG SystemDllWowRelocation;
    ULONG XStatePad[1];
    PVOID XState;
} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;
typedef const KUSER_SHARED_DATA *PCKUSER_SHARED_DATA;

#ifdef __cplusplus
}
#endif

#endif // (_WIN32_WINNT >= 0x0500)

#endif // _WINTERNL_