#pragma once
#include <fstream>
#include <ImageHlp.h>  
#pragma comment(lib,"imagehlp.lib")
#include <windows.h>


//自定义函数指针，用于传入DLL模块的路径
typedef unsigned int(WINAPI* _LoadLibrary)(wchar_t* DLLName);
//自定义函数指针，用于获得基址
typedef unsigned int(WINAPI* _GetModuleHandleA)(char* modName);
//自定义函数指针，用于改变进程的内存属性
typedef unsigned int(WINAPI* _VirtualProtect)(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flNewProtect, _Out_ PDWORD lpflOldProtect);

//记录 要远程注入的数据 的结构体 （模块名 和 装载模块的函数）
typedef struct _REMOTE_DATA
{
	wchar_t DLLName[0xFF];       //记录 DLL模块的名字
	//变量区
	unsigned EntryPoint;         //记录入口点
	unsigned HOOKfuction;        //存储注入函数的地址
	char     oldCode[5];         //存储注入前游戏进程原本的代码

	_LoadLibrary      f_LoadLibrary;     //记录 装载模块的函数LoadLibrary 的地址
	_GetModuleHandleA f_GetModuleHandleA; //记录 获得基址的函数 的地址
	_VirtualProtect   f_VirtualProtect;   //记录 改变进程的内存属性 的地址

}*PREMOTE_DATA;

//注入类
class INJCET
{
public:
	/*
	基于入口点注入
	*/

	// 启动程序：进程(exe文件)的完整路径、 GameCmds.GetBuffer()- 获得指向CString 中字符串 的指针、创建进程后的信息存到变量prinfo中去
	BOOL StartProcess(
		const wchar_t* GameFullPath, 
		const wchar_t* GamePath, 
		wchar_t* GameCmds, 
		PROCESS_INFORMATION* lpInfo,
		bool Pause //用于表示是否需要暂停进程
	);

	// 加载游戏进程的 PE文件数据 到内存
	void* ImageLoad(const wchar_t* filename); //传入要加载的PE文件(路径)

	// 释放加载PE文件的内存
	void Unloadimage(void* _data);

	//从PE文件数据中，获取注入的入口点
	DWORD GetEntryPoint(const wchar_t* filename); //传入要加载的PE文件(路径)

	//分配远程空间内存
	BOOL CreateRemoteData(HANDLE hProcee, wchar_t* GameExe,const wchar_t* DLLName);
	
	//编码远程参数（传入一个结构体和模块名，在函数内进行装载后，将模块名和装载函数存到该结构体中传出）
	void CodeRemoteData(PREMOTE_DATA _data, const wchar_t* DLLName);

};

