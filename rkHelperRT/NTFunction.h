#ifndef NTFunction__h_
#define NTFunction__h_
#include <winternl.h>

//
// Define Address of User Shared Data
//

#define MM_SHARED_USER_DATA_VA      0x7FFE0000

#define USER_SHARED_DATA ((PCKUSER_SHARED_DATA)MM_SHARED_USER_DATA_VA)

#pragma warning (disable:4035)        // disable 4035 (function must return something)
//#define PcTeb 0x18
//#define PcPeb 0x30
//_inline PTEB NtCurrentTeb( void ) { __asm mov eax, fs:[PcTeb] }
//_inline PPEB NtCurrentPeb( void ) { __asm mov eax, fs:[PcPeb] }
#pragma warning (default:4035)        // reenable it

#define NtCurrentPeb() (NtCurrentTeb()->ProcessEnvironmentBlock)
#define RtlProcessHeap() (NtCurrentPeb()->ProcessHeap)

#ifndef NtCurrentProcess
#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )
#endif /* NtCurrentProcess */
#ifndef NtCurrentThread
#define NtCurrentThread() ( (HANDLE)(LONG_PTR) -2 )
#endif /* NtCurrentThread */

//#define NTAPI __stdcall
#define PAGE_SIZE 0x1000
//#define DECLSPEC_IMPORT __declspec(dllimport)

#define NT_SUCCESS(Status)				((NTSTATUS)(Status)>=0)
#define NT_INFORMATION(Status)			((ULONG)(Status) >> 30 == 1)
#define NT_WARNING(Status)				((ULONG)(Status) >> 30 == 2)
#define NT_ERROR(Status)				((ULONG)(Status) >> 30 == 3)

//#define STATUS_INVALID_PARAMETER		((NTSTATUS)0xC000000DL)
#define STATUS_ACCESS_DENIED			((NTSTATUS)0xC0000022L)
#define STATUS_PRIVILEGE_NOT_HELD		((NTSTATUS)0xC0000061L)
#define STATUS_DRIVER_BLOCKED_CRITICAL	((NTSTATUS)0xC000036BL)
#define STATUS_OBJECT_NAME_NOT_FOUND    ((NTSTATUS)0xC0000034L)
#define STATUS_IMAGE_ALREADY_LOADED     ((NTSTATUS)0xC000010EL)

#define MINCHAR     0x80        // winnt
#define MAXCHAR     0x7f        // winnt
#define MINSHORT    0x8000      // winnt
#define MAXSHORT    0x7fff      // winnt
#define MINLONG     0x80000000  // winnt
#define MAXLONG     0x7fffffff  // winnt
#define MAXUCHAR    0xff        // winnt
#define MAXUSHORT   0xffff      // winnt
#define MAXULONG    0xffffffff  // winnt

//
// Define the highest user address and user probe address.
//

#define MM_HIGHEST_USER_ADDRESS (PVOID)0x7ffeffff

//
// Define the create/open option flags
//

#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008

#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080

#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
//UNUSED                                        0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800

#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000

//
// Define the I/O status information return values for NtCreateFile/NtOpenFile
//

#define FILE_SUPERSEDED                 0x00000000
#define FILE_OPENED                     0x00000001
#define FILE_CREATED                    0x00000002
#define FILE_OVERWRITTEN                0x00000003
#define FILE_EXISTS                     0x00000004
#define FILE_DOES_NOT_EXIST             0x00000005

//
// Define the file attributes values
//
// Note:  0x00000008 is reserved for use for the old DOS VOLID (volume ID)
//        and is therefore not considered valid in NT.
//
// Note:  0x00000010 is reserved for use for the old DOS SUBDIRECTORY flag
//        and is therefore not considered valid in NT.  This flag has
//        been disassociated with file attributes since the other flags are
//        protected with READ_ and WRITE_ATTRIBUTES access to the file.
//
// Note:  Note also that the order of these flags is set to allow both the
//        FAT and the Pinball File Systems to directly set the attributes
//        flags in attributes words without having to pick each flag out
//        individually.  The order of these flags should not be changed!
//

#define FILE_ATTRIBUTE_READONLY         0x00000001  // winnt
#define FILE_ATTRIBUTE_HIDDEN           0x00000002  // winnt
#define FILE_ATTRIBUTE_SYSTEM           0x00000004  // winnt
#define FILE_ATTRIBUTE_DIRECTORY        0x00000010  // winnt
#define FILE_ATTRIBUTE_ARCHIVE          0x00000020  // winnt
#define FILE_ATTRIBUTE_NORMAL           0x00000080  // winnt
#define FILE_ATTRIBUTE_TEMPORARY        0x00000100  // winnt
#define FILE_ATTRIBUTE_RESERVED0        0x00000200
#define FILE_ATTRIBUTE_RESERVED1        0x00000400
#define FILE_ATTRIBUTE_COMPRESSED       0x00000800  // winnt
#define FILE_ATTRIBUTE_OFFLINE          0x00001000  // winnt
#define FILE_ATTRIBUTE_PROPERTY_SET     0x00002000
#define FILE_ATTRIBUTE_VALID_FLAGS      0x00003fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS  0x00003fa7

//
// Define the create disposition values
//

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

#define OBJ_CASE_INSENSITIVE					0x00000040L

#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080

//
// These must be converted to LUIDs before use.
//

#define SE_MIN_WELL_KNOWN_PRIVILEGE       (2L)
#define SE_CREATE_TOKEN_PRIVILEGE         (2L)
#define SE_ASSIGNPRIMARYTOKEN_PRIVILEGE   (3L)
#define SE_LOCK_MEMORY_PRIVILEGE          (4L)
#define SE_INCREASE_QUOTA_PRIVILEGE       (5L)
#define SE_MACHINE_ACCOUNT_PRIVILEGE      (6L)
#define SE_TCB_PRIVILEGE                  (7L)
#define SE_SECURITY_PRIVILEGE             (8L)
#define SE_TAKE_OWNERSHIP_PRIVILEGE       (9L)
#define SE_LOAD_DRIVER_PRIVILEGE          (10L)
#define SE_SYSTEM_PROFILE_PRIVILEGE       (11L)
#define SE_SYSTEMTIME_PRIVILEGE           (12L)
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE  (13L)
#define SE_INC_BASE_PRIORITY_PRIVILEGE    (14L)
#define SE_CREATE_PAGEFILE_PRIVILEGE      (15L)
#define SE_CREATE_PERMANENT_PRIVILEGE     (16L)
#define SE_BACKUP_PRIVILEGE               (17L)
#define SE_RESTORE_PRIVILEGE              (18L)
#define SE_SHUTDOWN_PRIVILEGE             (19L)
#define SE_DEBUG_PRIVILEGE                (20L)
#define SE_AUDIT_PRIVILEGE                (21L)
#define SE_SYSTEM_ENVIRONMENT_PRIVILEGE   (22L)
#define SE_CHANGE_NOTIFY_PRIVILEGE        (23L)
#define SE_REMOTE_SHUTDOWN_PRIVILEGE      (24L)
#define SE_UNDOCK_PRIVILEGE               (25L)
#define SE_SYNC_AGENT_PRIVILEGE           (26L)
#define SE_ENABLE_DELEGATION_PRIVILEGE    (27L)
//xp and later
#define SE_MANAGE_VOLUME_PRIVILEGE        (28L)
#define SE_IMPERSONATE_PRIVILEGE          (29L)
#define SE_CREATE_GLOBAL_PRIVILEGE        (30L)
#define SE_MAX_WELL_KNOWN_PRIVILEGE       (30L)

//
// The following values for the RelativeTo parameter determine what the
// Path parameter to RtlQueryRegistryValues is relative to.
//

#define RTL_REGISTRY_ABSOLUTE     0   // Path is a full path
#define RTL_REGISTRY_SERVICES     1   // \Registry\Machine\System\CurrentControlSet\Services
#define RTL_REGISTRY_CONTROL      2   // \Registry\Machine\System\CurrentControlSet\Control
#define RTL_REGISTRY_WINDOWS_NT   3   // \Registry\Machine\Software\Microsoft\Windows NT\CurrentVersion
#define RTL_REGISTRY_DEVICEMAP    4   // \Registry\Machine\Hardware\DeviceMap
#define RTL_REGISTRY_USER         5   // \Registry\User\CurrentUser
#define RTL_REGISTRY_MAXIMUM      6
#define RTL_REGISTRY_HANDLE       0x40000000    // Low order bits are registry handle
#define RTL_REGISTRY_OPTIONAL     0x80000000    // Indicates the key node is optional

//
// Object Manager Directory Specific Access Rights.
//

#define DIRECTORY_QUERY                 (0x0001)
#define DIRECTORY_TRAVERSE              (0x0002)
#define DIRECTORY_CREATE_OBJECT         (0x0004)
#define DIRECTORY_CREATE_SUBDIRECTORY   (0x0008)

#define DIRECTORY_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0xF)

//
// Determine if an argument is present by testing the value of the pointer
// to the argument value.
//

#define ARGUMENT_PRESENT(ArgumentPointer)    (\
    (CHAR *)(ArgumentPointer) != (CHAR *)(NULL) )

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

//
// This works "generically" for Unicode and Ansi/Oem strings.
// Usage:
//   const static UNICODE_STRING FooU = RTL_CONSTANT_STRING(L"Foo");
//   const static         STRING Foo  = RTL_CONSTANT_STRING( "Foo");
// instead of the slower:
//   UNICODE_STRING FooU;
//           STRING Foo;
//   RtlInitUnicodeString(&FooU, L"Foo");
//          RtlInitString(&Foo ,  "Foo");
//
// Or:
//   const static char szFoo[] = "Foo";
//   const static STRING sFoo = RTL_CONSTANT_STRING(szFoo);
//
// This will compile without error or warning in C++. C will get a warning.
//
#ifdef __cplusplus
	extern "C++"
	{
	char _RTL_CONSTANT_STRING_type_check(const char *s);
	char _RTL_CONSTANT_STRING_type_check(const WCHAR *s);
	// __typeof would be desirable here instead of sizeof.
	template <size_t N> class _RTL_CONSTANT_STRING_remove_const_template_class;
	template <> class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(char)>  {public: typedef  char T; };
	template <> class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(WCHAR)> {public: typedef WCHAR T; };
	#define _RTL_CONSTANT_STRING_remove_const_macro(s) \
		(const_cast<_RTL_CONSTANT_STRING_remove_const_template_class<sizeof((s)[0])>::T*>(s))
	}
#else
	char _RTL_CONSTANT_STRING_type_check(const void *s);
	#define _RTL_CONSTANT_STRING_remove_const_macro(s) (s)
#endif
#define RTL_CONSTANT_STRING(s) \
{ \
    sizeof( s ) - sizeof( (s)[0] ), \
    sizeof( s ) / sizeof(_RTL_CONSTANT_STRING_type_check(s)), \
    _RTL_CONSTANT_STRING_remove_const_macro(s) \
}

//
// Valid values for the Attributes field
//

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_VALID_ATTRIBUTES    0x000001F2L


typedef LONG NTSTATUS;
typedef ULONG ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR KAFFINITY;
typedef ULONG KPRIORITY;

/*typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;//长度 18h
	HANDLE RootDirectory;// 00000000
	PUNICODE_STRING ObjectName;//指向对象名的指针
	ULONG Attributes;//对象属性00000040h
	PVOID SecurityDescriptor; // Points to type SECURITY_DESCRIPTOR，0
	PVOID SecurityQualityOfService; // Points to type SECURITY_QUALITY_OF_SERVICE，0
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;*/

//
// Event type
//
typedef enum _EVENT_TYPE {
    NotificationEvent,
    SynchronizationEvent
} EVENT_TYPE;

//
// Process/Thread System and User Time
//  NtQueryInformationProcess using ProcessTimes
//  NtQueryInformationThread using ThreadTimes
//

typedef struct _KERNEL_USER_TIMES {
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER ExitTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
} KERNEL_USER_TIMES;
typedef KERNEL_USER_TIMES *PKERNEL_USER_TIMES;

/*typedef struct _SYSTEM_PROCESS_INFORMATION {
	DWORD NextEntryOffset;
	DWORD dThreadCount;
	DWORD dReserved01;
	DWORD dReserved02;
	DWORD dReserved03;
	DWORD dReserved04;
	DWORD dReserved05;
	DWORD dReserved06;
	FILETIME ftCreateTime;			//relative to 01-01-1601
	FILETIME ftUserTime;			//100 nsec units
	FILETIME ftKernelTime;			//100 nsec units
	UNICODE_STRING ProcessName;		//进程名
	DWORD BasePriority;
	DWORD dUniqueProcessId;			//进程ID
	DWORD dParentProcessID;
	DWORD dHandleCount;
	DWORD dReserved07;
	DWORD dReserved08;
	DWORD VmCounters;
	DWORD dCommitCharge;
	PVOID ThreadInfos[1];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;*/

/*typedef struct _PROCESS_BASIC_INFORMATION {
	NTSTATUS	ExitStatus;
	PPEB		PebBaseAddress;
	ULONG_PTR	AffinityMask;
	KPRIORITY	BasePriority;
	ULONG_PTR	UniqueProcessId;
	ULONG_PTR	InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;*/

typedef struct _THREAD_BASIC_INFORMATION {
	NTSTATUS	ExitStatus;
	PVOID		TebBaseAddress;
	CLIENT_ID	ClientId;
	KAFFINITY	AffinityMask;
	KPRIORITY	Priority;
	KPRIORITY	BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

/*
typedef enum _PROCESS_INFORMATION_CLASS {
	ProcessQuotaLimits = 1,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	ProcessDeviceMap,
	ProcessSessionInformation,
	ProcessForegroundInformation,
	ProcessImageFileName = 27,
	ProcessLUIDDeviceMapsEnabled,
	ProcessBreakOnTermination,
	ProcessDebugObjectHandle,
	ProcessDebugFlags,
	ProcessHandleTracing,
	ProcessIoPriority,
	ProcessExecuteFlags,
	ProcessResourceManagement,
	ProcessCookie,
	ProcessImageInformation,
	MaxProcessInfoClass              // MaxProcessInfoClass should always be the last enum
} PROCESS_INFORMATION_CLASS, *PPROCESS_INFORMATION_CLASS;*/

/*
typedef enum _THREAD_INFORMATION_CLASS {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadHideFromDebugger = 17
} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;*/

typedef enum _SYSTEM_INFO_CLASS {
	SystemProcessorInformation = 1,
	SystemPathInformation = 4,
	SystemCallCountInformation = 6,
	SystemDeviceInformation,
	SystemFlagsInformation = 9,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPagefileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemDpcBehaviorInformation = 24,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemCrashDumpStateInformation = 34,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemExtendServiceTableInformation = 38,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemTimeSlipNotification = 46,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFO_CLASS;

typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation,
    MemoryWorkingSetInformation,
    MemoryMappedFilenameInformation
} MEMORY_INFORMATION_CLASS;

typedef enum _FILE_INFORMATION_CLASS {
// end_wdm
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   // 2
    FileBothDirectoryInformation,   // 3
    FileBasicInformation,           // 4  wdm
    FileStandardInformation,        // 5  wdm
    FileInternalInformation,        // 6
    FileEaInformation,              // 7
    FileAccessInformation,          // 8
    FileNameInformation,            // 9
    FileRenameInformation,          // 10
    FileLinkInformation,            // 11
    FileNamesInformation,           // 12
    FileDispositionInformation,     // 13
    FilePositionInformation,        // 14 wdm
    FileFullEaInformation,          // 15
    FileModeInformation,            // 16
    FileAlignmentInformation,       // 17
    FileAllInformation,             // 18
    FileAllocationInformation,      // 19
    FileEndOfFileInformation,       // 20 wdm
    FileAlternateNameInformation,   // 21
    FileStreamInformation,          // 22
    FilePipeInformation,            // 23
    FilePipeLocalInformation,       // 24
    FilePipeRemoteInformation,      // 25
    FileMailslotQueryInformation,   // 26
    FileMailslotSetInformation,     // 27
    FileCompressionInformation,     // 28
    FileObjectIdInformation,        // 29
    FileCompletionInformation,      // 30
    FileMoveClusterInformation,     // 31
    FileQuotaInformation,           // 32
    FileReparsePointInformation,    // 33
    FileNetworkOpenInformation,     // 34
    FileAttributeTagInformation,    // 35
    FileTrackingInformation,        // 36
    FileIdBothDirectoryInformation, // 37
    FileIdFullDirectoryInformation, // 38
    FileValidDataLengthInformation, // 39
    FileShortNameInformation,       // 40
    FileMaximumInformation
// begin_wdm
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _FILE_POSITION_INFORMATION {
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

typedef struct _FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_STANDARD_INFORMATION {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		struct {
			ULONG TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
	PVOID* EntryPointActivationContext;

	PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef NTSTATUS (*PUSER_THREAD_START_ROUTINE)(
    PVOID ThreadParameter
    );

//
// Subroutines for dealing with the Registry
//

typedef NTSTATUS
NTAPI RTL_QUERY_REGISTRY_ROUTINE(
    IN PWSTR ValueName,
    IN ULONG ValueType,
    IN PVOID ValueData,
    IN ULONG ValueLength,
    IN PVOID Context OPTIONAL,
    IN PVOID EntryContext OPTIONAL
    );
typedef RTL_QUERY_REGISTRY_ROUTINE *PRTL_QUERY_REGISTRY_ROUTINE;

typedef struct _RTL_QUERY_REGISTRY_TABLE {
    PRTL_QUERY_REGISTRY_ROUTINE QueryRoutine;
    ULONG Flags;
    PWSTR Name;
    PVOID EntryContext;
    ULONG DefaultType;
    PVOID DefaultData;
    ULONG DefaultLength;

} RTL_QUERY_REGISTRY_TABLE, *PRTL_QUERY_REGISTRY_TABLE;


//
// Current Directory Stuff
//

typedef struct _RTLP_CURDIR_REF *PRTLP_CURDIR_REF;

typedef struct _RTL_RELATIVE_NAME {
    STRING RelativeName;
    HANDLE ContainingDirectory;
    PRTLP_CURDIR_REF CurDirRef;
} RTL_RELATIVE_NAME, *PRTL_RELATIVE_NAME;

typedef enum _SECTION_INHERIT {
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef VOID
(WINAPI* PPS_APC_ROUTINE) (
    IN PVOID ApcArgument1,
    IN PVOID ApcArgument2,
    IN PVOID ApcArgument3
    );

typedef NTSTATUS
(NTAPI* PNtQuerySystemInformation)(
	IN SYSTEM_INFORMATION_CLASS		SystemInformationClass,
	IN OUT PVOID					SystemInformation,
	IN ULONG						SystemInformationLength,
	OUT PULONG						ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI* PNtQueryInformationProcess)(
	IN HANDLE						ProcessHandle,
	IN PROCESS_INFORMATION_CLASS	ProcessInformationClass,
	OUT PVOID						ProcessInformation,
	IN ULONG						ProcessInformationLength,
	OUT PULONG						ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI* PNtQueryInformationThread)(
	IN HANDLE						ThreadHandle,
	IN THREAD_INFORMATION_CLASS		ThreadInformationClass,
	OUT PVOID						ThreadInformation,
	IN ULONG						ThreadInformationLength,
	OUT PULONG						ReturnLength OPTIONAL);

typedef NTSTATUS
(NTAPI* PNtSetInformationThread)(
	IN HANDLE							ThreadHandle,
	IN THREAD_INFORMATION_CLASS		ThreadInformationClass,
	IN PVOID							ThreadInformation,
	IN ULONG							ThreadInformationLength);

typedef NTSTATUS
(NTAPI* PNtProtectVirtualMemory)(
	IN HANDLE		ProcessHandle,
	IN OUT PVOID*	BaseAddress,
	IN OUT PULONG	NumberOfBytesToProtect,
	IN ULONG		NewAccessProtection,
	OUT PULONG		OldAccessProtection);

typedef NTSTATUS
(NTAPI* PNtAllocateVirtualMemory) (
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG ZeroBits,
    IN OUT PULONG RegionSize,
    IN ULONG AllocationType,
    IN ULONG Protect
    );

typedef NTSTATUS
(NTAPI* PNtFreeVirtualMemory) (
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PULONG RegionSize,
    IN ULONG FreeType
    );

typedef NTSTATUS
(NTAPI* PNtQueryVirtualMemory) (
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress,
    IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
    OUT PVOID MemoryInformation,
    IN ULONG MemoryInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

typedef NTSTATUS
(NTAPI* PZwOpenProcess)(
	OUT PHANDLE					ProcessHandle,
	IN ACCESS_MASK			DesiredAccess,
	IN POBJECT_ATTRIBUTES	ObjectAttributes,
	IN PCLIENT_ID			ClientId);

typedef NTSTATUS
(NTAPI* PZwOpenThread)(
	OUT PHANDLE					ThreadHandle,
	IN ACCESS_MASK			AccessMask,
	IN POBJECT_ATTRIBUTES		ObjectAttributes,
	IN PCLIENT_ID				ClientId);

typedef NTSTATUS
(NTAPI* PNtDeviceIoControlFile) (
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

typedef NTSTATUS
(NTAPI* PNtQueryInformationFile) (
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass
    );

typedef NTSTATUS
(NTAPI* PNtSetInformationFile) (
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass
    );

typedef NTSTATUS
(NTAPI* PNtQueryAttributesFile) (
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PFILE_BASIC_INFORMATION FileInformation
    );

typedef NTSTATUS
(NTAPI* PNtCreateFile) (
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

typedef NTSTATUS
(NTAPI* PNtReadFile) (
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG Key OPTIONAL
    );

typedef NTSTATUS
(NTAPI* PNtCreateEvent) (
    OUT PHANDLE EventHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN EVENT_TYPE EventType,
    IN BOOLEAN InitialState
    );

typedef NTSTATUS
(NTAPI* PNtSetEvent) (
    IN HANDLE EventHandle,
    OUT PLONG PreviousState OPTIONAL
    );

typedef NTSTATUS
(NTAPI* PNtWaitForSingleObject) (
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

typedef NTSTATUS
(NTAPI* PNtDelayExecution) (
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER DelayInterval
    );

typedef NTSTATUS
(NTAPI* PNtCreateMutant) (
    IN PHANDLE MutantHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN BOOLEAN InitialOwner
    );

typedef NTSTATUS
(NTAPI* PNtOpenMutant) (
    IN PHANDLE MutantHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

typedef NTSTATUS
(NTAPI* PNtQueueApcThread) (
    IN HANDLE ThreadHandle,
    IN PPS_APC_ROUTINE ApcRoutine,
    IN PVOID ApcArgument1,
    IN PVOID ApcArgument2,
    IN PVOID ApcArgument3
    );

typedef NTSTATUS
(NTAPI* PNtClose)(PVOID);

typedef NTSTATUS
(NTAPI* PNtLoadDriver)(PUNICODE_STRING RegPath);

typedef NTSTATUS
(NTAPI* PNtUnloadDriver)(PUNICODE_STRING RegPath);

typedef VOID 
(NTAPI* PRtlInitAnsiString)(
	IN OUT PANSI_STRING DestinationString, 
	IN PCSZ SourceString
	);

typedef VOID
(NTAPI* PRtlInitUnicodeString) (
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
	);

typedef NTSTATUS
(NTAPI* PRtlAnsiStringToUnicodeString)(
	IN OUT PUNICODE_STRING DestinationString,
	IN PANSI_STRING SourceString,
	IN BOOLEAN AllocateDestinationString
	);

typedef NTSTATUS
(NTAPI* PRtlFreeUnicodeString)(
	IN PUNICODE_STRING DestinationString
	);

typedef NTSTATUS
(NTAPI* PRtlAdjustPrivilege) ( 
	ULONG    Privilege,
	BOOLEAN  Enable,
	BOOLEAN  CurrentThread,
	PBOOLEAN Enabled
	);

typedef BOOL
(NTAPI* PLookupPrivilegeValueA) (
	IN LPCSTR lpSystemName,
	IN LPCSTR lpName,
	OUT PLUID   lpLuid
	);

typedef NTSTATUS
(NTAPI* PNtCreateKey) (
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG TitleIndex,
	IN PUNICODE_STRING Class OPTIONAL,
	IN ULONG CreateOptions,
	OUT PULONG Disposition OPTIONAL
	);

typedef NTSTATUS
(NTAPI* PNtSetValueKey) ( 
	IN HANDLE KeyHandle, 
	IN PUNICODE_STRING ValueName, 
	IN ULONG TitleIndex, 
	IN ULONG Type, 
	IN PVOID Data, 
	IN ULONG DataSize 
	);

typedef NTSTATUS
(NTAPI* PNtDeleteKey) (
	IN HANDLE KeyHandle
	);

typedef NTSTATUS
(NTAPI* PRtlCreateRegistryKey) (
	IN  ULONG RelativeTo,
	IN  PWSTR Path
	);

typedef NTSTATUS
(NTAPI* PRtlWriteRegistryValue) (
	IN ULONG RelativeTo,
	IN PCWSTR Path,
	IN PCWSTR ValueName,
	IN ULONG ValueType,
	IN PVOID ValueData OPTIONAL,
	IN ULONG ValueLength
	);

typedef NTSTATUS
(NTAPI* PRtlDeleteRegistryValue) (
	IN  ULONG RelativeTo,
	IN  PCWSTR Path,
	IN  PCWSTR ValueName
	);

typedef NTSTATUS
(NTAPI* PRtlQueryRegistryValues) (
  IN      ULONG RelativeTo,
  IN      PCWSTR Path,
  IN OUT  PRTL_QUERY_REGISTRY_TABLE QueryTable,
  IN  PVOID Context OPTIONAL,
  IN  PVOID Environment OPTIONAL
);

typedef NTSTATUS
(NTAPI* PRtlCreateUserThread) (
    IN HANDLE Process,
    IN PSECURITY_DESCRIPTOR ThreadSecurityDescriptor OPTIONAL,
    IN BOOLEAN CreateSuspended,
    IN ULONG ZeroBits OPTIONAL,
    IN SIZE_T MaximumStackSize OPTIONAL,
    IN SIZE_T CommittedStackSize OPTIONAL,
    IN PUSER_THREAD_START_ROUTINE StartAddress,
    IN PVOID Parameter OPTIONAL,
    OUT PHANDLE Thread OPTIONAL,
    OUT PCLIENT_ID ClientId OPTIONAL
    );

typedef BOOLEAN
(NTAPI* PRtlDosPathNameToNtPathName_U) (
    IN PCWSTR DosFileName,
    OUT PUNICODE_STRING NtFileName,
    OUT PWSTR *FilePart OPTIONAL,
    OUT PRTL_RELATIVE_NAME RelativeName OPTIONAL
    );

typedef ULONG
(NTAPI* PRtlIsDosDeviceName_U) (
    IN PWSTR DosFileName
    );

typedef ULONG 
(NTAPI* PRtlGetFullPathName_U) (
    IN PCWSTR lpFileName,
    IN ULONG nBufferLength,
    IN PWSTR lpBuffer,
    OUT PWSTR *lpFilePart OPTIONAL
    );

typedef ULONG
(NTAPI* PRtlNtStatusToDosError) (
   NTSTATUS Status
   );

typedef NTSTATUS
(NTAPI* PNtCreateSection) (
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER MaximumSize OPTIONAL,
    IN ULONG SectionPageProtection,
    IN ULONG AllocationAttributes,
    IN HANDLE FileHandle OPTIONAL
	);

typedef NTSTATUS
(NTAPI* PNtMapViewOfSection) (
    IN HANDLE SectionHandle,
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG ZeroBits,
    IN ULONG CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PULONG ViewSize,
    IN SECTION_INHERIT InheritDisposition,
    IN ULONG AllocationType,
    IN ULONG Protect
    );

typedef NTSTATUS
(NTAPI* PNtUnmapViewOfSection) (
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress
    );

typedef NTSTATUS
(NTAPI* PNtOpenDirectoryObject) (
    OUT PHANDLE DirectoryHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef NTSTATUS
(NTAPI* PRtlAcquirePebLock) (
    VOID
    );

typedef NTSTATUS
(NTAPI* PRtlReleasePebLock) (
    VOID
    );

typedef BOOLEAN
(NTAPI* PRtlFreeHeap) (
    IN PVOID HeapHandle,
    IN ULONG Flags,
    IN PVOID BaseAddress
    );

typedef BOOLEAN
(NTAPI* PNtFlushInstructionCache) (
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress,
    IN SIZE_T Length
    );

typedef NTSTATUS
(NTAPI* PLdrUnloadDll) (
    IN PVOID DllHandle
    );

typedef NTSTATUS
(NTAPI* PRtlIsTextUnicode) (
    IN PVOID Buffer,
    IN ULONG Size,
    IN OUT PULONG Result OPTIONAL
);

#endif