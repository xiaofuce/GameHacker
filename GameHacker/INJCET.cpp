#include "pch.h"
#include "INJCET.h"
#include <securitybaseapi.h >

//加载进程的PE文件
void* INJCET::ImageLoad(const wchar_t* filename)
{
	//打开文件
	std::ifstream streamReader(filename,std::ios::binary);//要打开的文件为filename，以二进制形式打开
	
	//获得文件大小(移动文件流到文件末尾，然后读取偏移量，进而间接获得文件大小)
	streamReader.seekg(0, std::ios::end);     // 即文件指针跳到末尾。偏移量为0，文件流从尾部开始  【偏移量，操作的起始点】
	unsigned filesize = streamReader.tellg(); // 报告文件流的当前读取位置，即 获得文件指针的偏移量 
	
	//读取PE文件数据
	char* _data = new char[filesize];  // 申请内存，存储PE文件的数据
	streamReader.seekg(0, std::ios::beg);// 文件指针跳到开头
	streamReader.read(_data, filesize);  //存储到数组_data中,读取指定数目filesize

	//关闭文件
	streamReader.close();

	return _data;
}

//释放加载PE文件的内存
void INJCET::Unloadimage(void* _data)
{
	delete[] _data;
}

//获得入口点
DWORD INJCET::GetEntryPoint(const wchar_t* filename)
{
	// 1.加载PE文件
	void* image = ImageLoad(filename); //传入要加载的PE文件(路径)

	// 2.转为DOS头的类型（IMAGE_DOS_HEADER）
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)image;

	// 3.获得PE头地址
	unsigned PEAddress = dosHeader->e_lfanew + (unsigned)image;//PE头的地址=e_lfanew的值(存储PE头相对于DOS头的距离/偏移量，又称基址)+DOS头的地址

	// 4.转为PE头的类型（IMAGE_NT_HEADERS）
	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)PEAddress;
	
	// 5.获得注入点
	DWORD dEntryPoint = ntHeader->OptionalHeader.AddressOfEntryPoint;

	// 6.释放加载PE文件的内存
	Unloadimage(image);

	return dEntryPoint;//返回入口点
}

//编码远程参数（传入一个结构体和模块名，在函数内进行装载后，将模块名和装载函数存到该结构体中传出）
void INJCET::CodeRemoteData(PREMOTE_DATA _data, const wchar_t* dllName)
{
	//1.获得DLLName的长度
	short lenth;
	for (lenth = 0; dllName[lenth];lenth++)
	{ }

	//2.通过内核动态库，获得装载模块的函数等 的地址，并传入 远程参数结构体_data 内
	  //因为 装载模块的函数 的地址在不同电脑上是不一样的，所以我们要从系统动态库里获得，以能适应不同机器
	HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));//获得内核动态库kernel32.dll的句柄 【装载模块的函数在kernel32.dll中】
	_data->f_LoadLibrary = (_LoadLibrary)GetProcAddress(hKernel, "LoadLibraryW");//从指定的动态链接库 (hKernel) 检索导出函数（"LoadLibrayW"）,成功则返回其地址
	_data->f_GetModuleHandleA = (_GetModuleHandleA)GetProcAddress(hKernel, "GetModuleHandleA");//从指定的动态链接库 (hKernel) 检索导出函数（"GetModuleHandleA"）,成功则返回其地址
	_data->f_VirtualProtect = (_VirtualProtect)GetProcAddress(hKernel, "VirtualProtect");//从指定的动态链接库 (hKernel) 检索导出函数（"LoadLibrayW"）,成功则返回其地址	
	
	//3.将传入的DLLName，保存到 远程数据模块_data 的名称变量DLLName内
	memcpy(_data->DLLName, dllName, (lenth + 1) * 2);//之所以要*2，是因为 1个wchar_t是两字节
}

//入口点注入函数
DWORD _stdcall RemoteThreadProc(PREMOTE_DATA p)
{
	//1.获得基址
	unsigned base = p->f_GetModuleHandleA(0);
	p->EntryPoint += base;//计算程序的PE头
	//2.改变进程入口点的内存属性
	DWORD dRet;
	p->f_VirtualProtect((LPVOID)p->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dRet);

	//3.保存入口点的代码
	char* entryCode = (char*)p->EntryPoint;

	p->oldCode[0] = entryCode[0];
	p->oldCode[1] = entryCode[1];
	p->oldCode[2] = entryCode[2];
	p->oldCode[3] = entryCode[3];
	p->oldCode[4] = entryCode[4];

	//4.修改入口点 内存指向
	entryCode[0] = 0xE9;//JMP
	int Distance = p->HOOKfuction - p->EntryPoint - 5; //要JMP到的实际内存地址 = 要跳转到的目标内存地址 - 执行jmp指令的内存地址 - 5
	
	int* entryDis = (int*)(p->EntryPoint + 1);//临时指针 指向入口点起始地址的下一块内存（即指向JMP后面的距离的内存）
	*entryDis = Distance;//修改入口点处的内存，指向 我们申请的内存空间

	return 0;
}

//启动程序
BOOL INJCET::StartProcess(const wchar_t* GameFullPath, const wchar_t* GamePath, wchar_t* GameCmds, PROCESS_INFORMATION* lpInfo,bool Pause)
{
	STARTUPINFO si{};//指定创建时进程的主窗口的窗口工作站、桌面、标准句柄和外观
	si.cb = sizeof(si);  //一定要执行这步，否则可能会出错

	DWORD dFlags = 0;
	if (Pause)
	{
		dFlags = CREATE_SUSPENDED;//如果开启了暂停，则创建进程后暂停进程
		//AfxMessageBox(L"YY");
	}
	//创建游戏进程
	return CreateProcess(
		GameFullPath,//进程(exe文件)的完整路径
		GameCmds,//命令行参数   .GetBuffer()- 获得指向CSimpleString 中字符串 的指针
		NULL, NULL, FALSE,  //固定这样写，不继承
		dFlags,   //选择进程的创建标志
		NULL,
		GamePath,//进程所在的文件夹的完整路径
		&si,
		lpInfo   //创建进程后，信息存到变量prinfo中去
	);
}

/* 内存区域的使用规划：
	 0-0x200 注入模块
	 0x500   入口点注入函数
	 0x2000  远程参数区
*/

//注入模块       要解决的问题：如何把“远程参数”的内存地址（在游戏进程中分配的）告诉INJIECTCode
void _stdcall INJIECTCode()
{
	//1.将远程参数（存储 执行我们的功能所需的各种数据）的地址，用CC暂时占位adrRemoteData的地址，在CreateRemoteData函数中进行修正。
	unsigned address = 0xCCCCCCCC;       //注意：注入到内存中的INJIECTCode，已经是对地址进行修正过的了

	//2.加载要注入的模块
	PREMOTE_DATA p = (PREMOTE_DATA)address; //地址的内存转为 远程参数 结构体指针
	p->f_LoadLibrary(p->DLLName); //用远程参数中的装载函数，装载我们的dll模块。 p->f__LoadLibrary就是装载模块的函数，p->DLLName是要装载的模块名
	//AfxMessageBox(L"加载要注入的模块成功！");

	//3.恢复入口点的代码
	unsigned dEntry = p->EntryPoint;

	char* entryCode = (char*)p->EntryPoint;//临时指针指向入口点的内存位置，用以恢复入口点以前的代码
	entryCode[0] = p->oldCode[0];
	entryCode[1] = p->oldCode[1];
	entryCode[2] = p->oldCode[2];
	entryCode[3] = p->oldCode[3];
	entryCode[4] = p->oldCode[4];

	//4.CPU跳回入口点，正常执行游戏进程的代码
	_asm
	{
		mov eax, dEntry
		jmp eax
	}
}

//分配远程注入的内存空间
BOOL INJCET::CreateRemoteData(HANDLE hProcee, wchar_t* GameExe, const wchar_t* DLLName)
{
	//1.分配内存空间，以用来注入我们的代码
	LPVOID adrRemote = VirtualAllocEx(hProcee, NULL, 0x3000, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);//内存大小为0x3000
	/* 内存区域的使用规划：
	 0-0x200 注入模块
	 0x500   入口点注入函数
	 0x2000  远程参数区
*/
	SIZE_T lwt;    //记录写入了多少字节，用于判断错误用。可忽略
	LPVOID adrRemoteData = (LPVOID)((unsigned)adrRemote + 0x2000); //这处内存，用来存 远程参数
	LPVOID adrRemoteProc = (LPVOID)((unsigned)adrRemote + 0x500);  //这处内存，用来存 入口点注入的函数
	
	//2.写入远程执行的代码
	  //(1)编码 远程参数 （远程参数：存储 要装载的函数模块、入口点数据等）
	_REMOTE_DATA remoteData{};//用于存储 远程编码 的数据（模块名和装载模块的函数）
	remoteData.HOOKfuction = (unsigned)adrRemote;             //将分配的空间的起始地址，写入远程参数的结构体里
	remoteData.EntryPoint = (unsigned)GetEntryPoint(GameExe); //将入口点地址，写入远程参数的结构体里
	CodeRemoteData(&remoteData, DLLName);  //向远程参数的结构体里，传入 模块名 和 要装载模块的函数
	
	  //(2)将 远程参数，写入对应的内存区域
	WriteProcessMemory(hProcee, adrRemoteData, &remoteData, sizeof(remoteData), &lwt);//将远程参数remoteData，写入内存
	WriteProcessMemory(hProcee, adrRemoteProc, RemoteThreadProc, 0x1400, &lwt);        //入口点注入函数RemoteThreadProc，写入内存
	
	//3.修正远程代码
	  //(1)拷贝一份 函数“注入模块”的代码数据，用于进行代码修正
	char _code[0x200]; 
	memcpy(_code, INJIECTCode, sizeof(_code));
	
	  //(2)进行代码修正（将占位处的内存地址替换为真正的内存地址）
	for (int i = 0;i < 0x100;i++)//这里的0x100要根据INJIECTCode中的代码量来调
	{
		unsigned* pcode = (unsigned*)(&_code[i]);//临时指针，指向 函数“加载注入模块”的某块代码数据（每次指向内存空间的4个字节，因为为unsigned类型）
		//AfxMessageBox(L"开始修正！");
		if (*pcode == 0xCCCCCCCC)//判断该块内存里的数据是不是我们想要修正的内容
		{
			*pcode = (unsigned)adrRemoteData; //将占位“远程参数”地址的cc，修正为真实的内存地址（写入pcode指向的_code的内存中）【修改的是_code[]中的数据】
			break;
		}
	}
	//(3)将修正后的 注入模块 的数据(存在_code[]中),写到分配的内存中去
	WriteProcessMemory(hProcee, adrRemote, _code, 0x200, &lwt);
	

	//4.分配一个线程,来执行 我们分配的内存中 “修改程序入口点” 的代码【这步失败了】
	DWORD dwThreadId{};
	HANDLE remoteHdl = CreateRemoteThread(hProcee, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemoteProc, adrRemoteData, 0, &dwThreadId);//创建线程，执行 我们分配的内存中，“入口点注入”的代码
	int x = GetLastError();
	WaitForSingleObject(remoteHdl,INFINITE);//等待线程执行结束
	CloseHandle(remoteHdl);

	//线程注入
	//DWORD dwThreadId;
	//HANDLE remoteHdl = CreateRemoteThread(hProcee, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemoteProc, adrRemoteData, 0, &dwThreadId);//创建线程，执行 我们分配的内存中，修改程序入口点的代码
	//DWORD dw = WaitForSingleObject(remoteHdl, INFINITE);//等待线程执行结束
	//CloseHandle(remoteHdl);

	return 0;
}

