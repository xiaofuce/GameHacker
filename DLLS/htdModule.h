#pragma once

//存储 进程已加载的模块信息
typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

//_LDR_DATA_TABLE_ENTRYA 结构体需要的前置 和 PEB结构体 前置的前置
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;



//PEB结构体的前置
typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;
typedef
VOID
(NTAPI* PPS_POST_PROCESS_INIT_ROUTINE) (
    VOID
    );

//PEB的数据结构
typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB, * PPEB;


//实际上的 具体模块信息 的结构体(修改过)
typedef struct _LDR_DATA_TABLE_ENTRYA {
    LIST_ENTRY InLoadOrderModuleList;            //不对外开放的、存储模块信息表单
    LIST_ENTRY InMemoryOrderLinks;               //存储模块信息的链表（对外开放的）
    LIST_ENTRY InInitializationOrderModuleList;  //不对外开放的、存储模块信息表单

    PVOID DllBase;
    PVOID Reserved3[2];
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
#pragma warning(push)
#pragma warning(disable: 4201) // we'll always use the Microsoft compiler
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    } DUMMYUNIONNAME;
#pragma warning(pop)
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRYA, * PLDR_DATA_TABLE_ENTRYA;


//用于隐藏模块的类
class htdModule
{
    void HideDll(HMODULE _hMod);//实现模块隐藏功能的内部函数（只允许私有调用）
public:
    void HideDll(wchar_t* dllName);//对外提供的接口（传入模块名称，然后内部代码实现隐藏）
};

