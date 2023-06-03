#pragma once

//�洢 �����Ѽ��ص�ģ����Ϣ
typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

//_LDR_DATA_TABLE_ENTRYA �ṹ����Ҫ��ǰ�� �� PEB�ṹ�� ǰ�õ�ǰ��
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;



//PEB�ṹ���ǰ��
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

//PEB�����ݽṹ
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


//ʵ���ϵ� ����ģ����Ϣ �Ľṹ��(�޸Ĺ�)
typedef struct _LDR_DATA_TABLE_ENTRYA {
    LIST_ENTRY InLoadOrderModuleList;            //�����⿪�ŵġ��洢ģ����Ϣ��
    LIST_ENTRY InMemoryOrderLinks;               //�洢ģ����Ϣ���������⿪�ŵģ�
    LIST_ENTRY InInitializationOrderModuleList;  //�����⿪�ŵġ��洢ģ����Ϣ��

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


//��������ģ�����
class htdModule
{
    void HideDll(HMODULE _hMod);//ʵ��ģ�����ع��ܵ��ڲ�������ֻ����˽�е��ã�
public:
    void HideDll(wchar_t* dllName);//�����ṩ�Ľӿڣ�����ģ�����ƣ�Ȼ���ڲ�����ʵ�����أ�
};

