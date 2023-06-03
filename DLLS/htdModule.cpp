#include "pch.h"
//#include <Windows.h>
#include "htdModule.h"

//实现模块隐藏功能的内部函数（只允许私有调用）
void htdModule::HideDll(HMODULE _hMod) //传入目标模块的基址
{
   //一.删除链表中的数据
	//1.获得PEB（存储进程的各种信息）
	PPEB _peb;
	_asm
	{
		mov eax, fs: [0x30] ; //TEB就放在fs段寄存器开头，而TEB往后偏移48个字节(16进制为30)，就是PEB
		mov _peb, eax;
	}
	//2.获得 进程已加载的模块信息
	PPEB_LDR_DATA Pldr = _peb->Ldr;
	//3.获得 存储模块信息的链表 的头指针
	PLIST_ENTRY FIRST = &(Pldr->InMemoryOrderModuleList);
	PLIST_ENTRY CURST = FIRST->Flink; //用一个当前指针，指向链表的第一个元素

	//4.获得 链表元素 在 具体模块信息结构体中的相对偏移量 （原理和详情看笔记 “模块链表的示意图”）
	PLDR_DATA_TABLE_ENTRYA lDis{};
	int StrctDiff = (int)&lDis->InMemoryOrderLinks;//因为新生成的结构体，大多数编译器的起始地址为0

	//5.遍历模块双向链表，直到找到目标模块
	PLDR_DATA_TABLE_ENTRYA lModule;
	do {
		lModule = (PLDR_DATA_TABLE_ENTRYA)((unsigned)CURST - StrctDiff);//链表元素的指针减去相对偏移量，就是具体模块信息的结构体的起始地址
		if (lModule->DllBase == _hMod) //如果是目标模块的基址
		{
			//在三个表中，将当前模块元素“断链”
			lModule->InLoadOrderModuleList.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
			lModule->InLoadOrderModuleList.Flink->Blink = lModule->InLoadOrderModuleList.Blink;

			lModule->InMemoryOrderLinks.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
			lModule->InMemoryOrderLinks.Flink->Blink = lModule->InLoadOrderModuleList.Blink;

			lModule->InInitializationOrderModuleList.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
			lModule->InInitializationOrderModuleList.Flink->Blink = lModule->InLoadOrderModuleList.Blink;
		}
		//如果不是目标模块，就让当前元素指针 指向下一个元素
		CURST = CURST->Flink; 

	} while (CURST != FIRST);//只要当前元素指针 没有轮转回来，就一直遍历

	//二.删除内存中的PE头和dos头
	//1.获得PE头和dos头
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)_hMod; //模块的 DOS 头，就位于模块的基址（起始地址）处
														     //PE 头是紧跟在 DOS 头后面的一个结构体，它的位置由 DOS 头的一个字段 e_lfanew 指定
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(dosHeader->e_lfanew + (unsigned)_hMod);//e_lfanew 表示 PE 头的文件偏移量，即它在文件中的位置 距离文件起始地址的字节数。
	                                                              //由于模块已经被加载到内存中，因此只需要 将PE头的偏移量 + 模块的基址 _hMod，就能得到 PE 头在内存中的地址。

	//2.删除它们
	IMAGE_DOS_HEADER dosH{};
	IMAGE_NT_HEADERS ntH{};

	DWORD dOld;
	VirtualProtect(dosHeader, sizeof(dosH), PAGE_EXECUTE_READWRITE, &dOld); //修改内存属性，以能修改内存
	VirtualProtect(ntHeader, sizeof(ntH), PAGE_EXECUTE_READWRITE, &dOld);
	
	memcpy(dosHeader, &dosH, sizeof(dosH)); //用c++的内存函数，更快捷地清空dos头和PE头（全部设置为0）
	memcpy(ntHeader,   &ntH, sizeof(ntH));
}

//对外提供的接口（传入模块名称，然后内部代码实现隐藏）
void htdModule::HideDll(wchar_t* dllName)
{
	//1.获得传入的模块的句柄
	HMODULE hMod = GetModuleHandleW(dllName);//获得目标模块的句柄（也就是模块的基址，因为在Windows操作系统中，模块句柄实际上就是模块的基址，
	                                         //可以通过将模块句柄强制转换为HMODULE类型的指针来获取模块的基址。）
	//2.将模块句柄传入内部函数，以实现模块隐藏功能
	if(hMod)  HideDll(hMod);
}
