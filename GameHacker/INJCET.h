#pragma once
#include <fstream>
#include <ImageHlp.h>  
#pragma comment(lib,"imagehlp.lib")
#include <windows.h>


//�Զ��庯��ָ�룬���ڴ���DLLģ���·��
typedef unsigned int(WINAPI* _LoadLibrary)(wchar_t* DLLName);
//�Զ��庯��ָ�룬���ڻ�û�ַ
typedef unsigned int(WINAPI* _GetModuleHandleA)(char* modName);
//�Զ��庯��ָ�룬���ڸı���̵��ڴ�����
typedef unsigned int(WINAPI* _VirtualProtect)(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flNewProtect, _Out_ PDWORD lpflOldProtect);

//��¼ ҪԶ��ע������� �Ľṹ�� ��ģ���� �� װ��ģ��ĺ�����
typedef struct _REMOTE_DATA
{
	wchar_t DLLName[0xFF];       //��¼ DLLģ�������
	//������
	unsigned EntryPoint;         //��¼��ڵ�
	unsigned HOOKfuction;        //�洢ע�뺯���ĵ�ַ
	char     oldCode[5];         //�洢ע��ǰ��Ϸ����ԭ���Ĵ���

	_LoadLibrary      f_LoadLibrary;     //��¼ װ��ģ��ĺ���LoadLibrary �ĵ�ַ
	_GetModuleHandleA f_GetModuleHandleA; //��¼ ��û�ַ�ĺ��� �ĵ�ַ
	_VirtualProtect   f_VirtualProtect;   //��¼ �ı���̵��ڴ����� �ĵ�ַ

}*PREMOTE_DATA;

//ע����
class INJCET
{
public:
	/*
	������ڵ�ע��
	*/

	// �������򣺽���(exe�ļ�)������·���� GameCmds.GetBuffer()- ���ָ��CString ���ַ��� ��ָ�롢�������̺����Ϣ�浽����prinfo��ȥ
	BOOL StartProcess(
		const wchar_t* GameFullPath, 
		const wchar_t* GamePath, 
		wchar_t* GameCmds, 
		PROCESS_INFORMATION* lpInfo,
		bool Pause //���ڱ�ʾ�Ƿ���Ҫ��ͣ����
	);

	// ������Ϸ���̵� PE�ļ����� ���ڴ�
	void* ImageLoad(const wchar_t* filename); //����Ҫ���ص�PE�ļ�(·��)

	// �ͷż���PE�ļ����ڴ�
	void Unloadimage(void* _data);

	//��PE�ļ������У���ȡע�����ڵ�
	DWORD GetEntryPoint(const wchar_t* filename); //����Ҫ���ص�PE�ļ�(·��)

	//����Զ�̿ռ��ڴ�
	BOOL CreateRemoteData(HANDLE hProcee, wchar_t* GameExe,const wchar_t* DLLName);
	
	//����Զ�̲���������һ���ṹ���ģ�������ں����ڽ���װ�غ󣬽�ģ������װ�غ����浽�ýṹ���д�����
	void CodeRemoteData(PREMOTE_DATA _data, const wchar_t* DLLName);

};

