#include "pch.h"
#include "INJCET.h"
#include <securitybaseapi.h >

//���ؽ��̵�PE�ļ�
void* INJCET::ImageLoad(const wchar_t* filename)
{
	//���ļ�
	std::ifstream streamReader(filename,std::ios::binary);//Ҫ�򿪵��ļ�Ϊfilename���Զ�������ʽ��
	
	//����ļ���С(�ƶ��ļ������ļ�ĩβ��Ȼ���ȡƫ������������ӻ���ļ���С)
	streamReader.seekg(0, std::ios::end);     // ���ļ�ָ������ĩβ��ƫ����Ϊ0���ļ�����β����ʼ  ��ƫ��������������ʼ�㡿
	unsigned filesize = streamReader.tellg(); // �����ļ����ĵ�ǰ��ȡλ�ã��� ����ļ�ָ���ƫ���� 
	
	//��ȡPE�ļ�����
	char* _data = new char[filesize];  // �����ڴ棬�洢PE�ļ�������
	streamReader.seekg(0, std::ios::beg);// �ļ�ָ��������ͷ
	streamReader.read(_data, filesize);  //�洢������_data��,��ȡָ����Ŀfilesize

	//�ر��ļ�
	streamReader.close();

	return _data;
}

//�ͷż���PE�ļ����ڴ�
void INJCET::Unloadimage(void* _data)
{
	delete[] _data;
}

//�����ڵ�
DWORD INJCET::GetEntryPoint(const wchar_t* filename)
{
	// 1.����PE�ļ�
	void* image = ImageLoad(filename); //����Ҫ���ص�PE�ļ�(·��)

	// 2.תΪDOSͷ�����ͣ�IMAGE_DOS_HEADER��
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)image;

	// 3.���PEͷ��ַ
	unsigned PEAddress = dosHeader->e_lfanew + (unsigned)image;//PEͷ�ĵ�ַ=e_lfanew��ֵ(�洢PEͷ�����DOSͷ�ľ���/ƫ�������ֳƻ�ַ)+DOSͷ�ĵ�ַ

	// 4.תΪPEͷ�����ͣ�IMAGE_NT_HEADERS��
	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)PEAddress;
	
	// 5.���ע���
	DWORD dEntryPoint = ntHeader->OptionalHeader.AddressOfEntryPoint;

	// 6.�ͷż���PE�ļ����ڴ�
	Unloadimage(image);

	return dEntryPoint;//������ڵ�
}

//����Զ�̲���������һ���ṹ���ģ�������ں����ڽ���װ�غ󣬽�ģ������װ�غ����浽�ýṹ���д�����
void INJCET::CodeRemoteData(PREMOTE_DATA _data, const wchar_t* dllName)
{
	//1.���DLLName�ĳ���
	short lenth;
	for (lenth = 0; dllName[lenth];lenth++)
	{ }

	//2.ͨ���ں˶�̬�⣬���װ��ģ��ĺ����� �ĵ�ַ�������� Զ�̲����ṹ��_data ��
	  //��Ϊ װ��ģ��ĺ��� �ĵ�ַ�ڲ�ͬ�������ǲ�һ���ģ���������Ҫ��ϵͳ��̬�����ã�������Ӧ��ͬ����
	HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));//����ں˶�̬��kernel32.dll�ľ�� ��װ��ģ��ĺ�����kernel32.dll�С�
	_data->f_LoadLibrary = (_LoadLibrary)GetProcAddress(hKernel, "LoadLibraryW");//��ָ���Ķ�̬���ӿ� (hKernel) ��������������"LoadLibrayW"��,�ɹ��򷵻����ַ
	_data->f_GetModuleHandleA = (_GetModuleHandleA)GetProcAddress(hKernel, "GetModuleHandleA");//��ָ���Ķ�̬���ӿ� (hKernel) ��������������"GetModuleHandleA"��,�ɹ��򷵻����ַ
	_data->f_VirtualProtect = (_VirtualProtect)GetProcAddress(hKernel, "VirtualProtect");//��ָ���Ķ�̬���ӿ� (hKernel) ��������������"LoadLibrayW"��,�ɹ��򷵻����ַ	
	
	//3.�������DLLName�����浽 Զ������ģ��_data �����Ʊ���DLLName��
	memcpy(_data->DLLName, dllName, (lenth + 1) * 2);//֮����Ҫ*2������Ϊ 1��wchar_t�����ֽ�
}

//��ڵ�ע�뺯��
DWORD _stdcall RemoteThreadProc(PREMOTE_DATA p)
{
	//1.��û�ַ
	unsigned base = p->f_GetModuleHandleA(0);
	p->EntryPoint += base;//��������PEͷ
	//2.�ı������ڵ���ڴ�����
	DWORD dRet;
	p->f_VirtualProtect((LPVOID)p->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dRet);

	//3.������ڵ�Ĵ���
	char* entryCode = (char*)p->EntryPoint;

	p->oldCode[0] = entryCode[0];
	p->oldCode[1] = entryCode[1];
	p->oldCode[2] = entryCode[2];
	p->oldCode[3] = entryCode[3];
	p->oldCode[4] = entryCode[4];

	//4.�޸���ڵ� �ڴ�ָ��
	entryCode[0] = 0xE9;//JMP
	int Distance = p->HOOKfuction - p->EntryPoint - 5; //ҪJMP����ʵ���ڴ��ַ = Ҫ��ת����Ŀ���ڴ��ַ - ִ��jmpָ����ڴ��ַ - 5
	
	int* entryDis = (int*)(p->EntryPoint + 1);//��ʱָ�� ָ����ڵ���ʼ��ַ����һ���ڴ棨��ָ��JMP����ľ�����ڴ棩
	*entryDis = Distance;//�޸���ڵ㴦���ڴ棬ָ�� ����������ڴ�ռ�

	return 0;
}

//��������
BOOL INJCET::StartProcess(const wchar_t* GameFullPath, const wchar_t* GamePath, wchar_t* GameCmds, PROCESS_INFORMATION* lpInfo,bool Pause)
{
	STARTUPINFO si{};//ָ������ʱ���̵������ڵĴ��ڹ���վ�����桢��׼��������
	si.cb = sizeof(si);  //һ��Ҫִ���ⲽ��������ܻ����

	DWORD dFlags = 0;
	if (Pause)
	{
		dFlags = CREATE_SUSPENDED;//�����������ͣ���򴴽����̺���ͣ����
		//AfxMessageBox(L"YY");
	}
	//������Ϸ����
	return CreateProcess(
		GameFullPath,//����(exe�ļ�)������·��
		GameCmds,//�����в���   .GetBuffer()- ���ָ��CSimpleString ���ַ��� ��ָ��
		NULL, NULL, FALSE,  //�̶�����д�����̳�
		dFlags,   //ѡ����̵Ĵ�����־
		NULL,
		GamePath,//�������ڵ��ļ��е�����·��
		&si,
		lpInfo   //�������̺���Ϣ�浽����prinfo��ȥ
	);
}

/* �ڴ������ʹ�ù滮��
	 0-0x200 ע��ģ��
	 0x500   ��ڵ�ע�뺯��
	 0x2000  Զ�̲�����
*/

//ע��ģ��       Ҫ��������⣺��ΰѡ�Զ�̲��������ڴ��ַ������Ϸ�����з���ģ�����INJIECTCode
void _stdcall INJIECTCode()
{
	//1.��Զ�̲������洢 ִ�����ǵĹ�������ĸ������ݣ��ĵ�ַ����CC��ʱռλadrRemoteData�ĵ�ַ����CreateRemoteData�����н���������
	unsigned address = 0xCCCCCCCC;       //ע�⣺ע�뵽�ڴ��е�INJIECTCode���Ѿ��ǶԵ�ַ��������������

	//2.����Ҫע���ģ��
	PREMOTE_DATA p = (PREMOTE_DATA)address; //��ַ���ڴ�תΪ Զ�̲��� �ṹ��ָ��
	p->f_LoadLibrary(p->DLLName); //��Զ�̲����е�װ�غ�����װ�����ǵ�dllģ�顣 p->f__LoadLibrary����װ��ģ��ĺ�����p->DLLName��Ҫװ�ص�ģ����
	//AfxMessageBox(L"����Ҫע���ģ��ɹ���");

	//3.�ָ���ڵ�Ĵ���
	unsigned dEntry = p->EntryPoint;

	char* entryCode = (char*)p->EntryPoint;//��ʱָ��ָ����ڵ���ڴ�λ�ã����Իָ���ڵ���ǰ�Ĵ���
	entryCode[0] = p->oldCode[0];
	entryCode[1] = p->oldCode[1];
	entryCode[2] = p->oldCode[2];
	entryCode[3] = p->oldCode[3];
	entryCode[4] = p->oldCode[4];

	//4.CPU������ڵ㣬����ִ����Ϸ���̵Ĵ���
	_asm
	{
		mov eax, dEntry
		jmp eax
	}
}

//����Զ��ע����ڴ�ռ�
BOOL INJCET::CreateRemoteData(HANDLE hProcee, wchar_t* GameExe, const wchar_t* DLLName)
{
	//1.�����ڴ�ռ䣬������ע�����ǵĴ���
	LPVOID adrRemote = VirtualAllocEx(hProcee, NULL, 0x3000, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);//�ڴ��СΪ0x3000
	/* �ڴ������ʹ�ù滮��
	 0-0x200 ע��ģ��
	 0x500   ��ڵ�ע�뺯��
	 0x2000  Զ�̲�����
*/
	SIZE_T lwt;    //��¼д���˶����ֽڣ������жϴ����á��ɺ���
	LPVOID adrRemoteData = (LPVOID)((unsigned)adrRemote + 0x2000); //�⴦�ڴ棬������ Զ�̲���
	LPVOID adrRemoteProc = (LPVOID)((unsigned)adrRemote + 0x500);  //�⴦�ڴ棬������ ��ڵ�ע��ĺ���
	
	//2.д��Զ��ִ�еĴ���
	  //(1)���� Զ�̲��� ��Զ�̲������洢 Ҫװ�صĺ���ģ�顢��ڵ����ݵȣ�
	_REMOTE_DATA remoteData{};//���ڴ洢 Զ�̱��� �����ݣ�ģ������װ��ģ��ĺ�����
	remoteData.HOOKfuction = (unsigned)adrRemote;             //������Ŀռ����ʼ��ַ��д��Զ�̲����Ľṹ����
	remoteData.EntryPoint = (unsigned)GetEntryPoint(GameExe); //����ڵ��ַ��д��Զ�̲����Ľṹ����
	CodeRemoteData(&remoteData, DLLName);  //��Զ�̲����Ľṹ������� ģ���� �� Ҫװ��ģ��ĺ���
	
	  //(2)�� Զ�̲�����д���Ӧ���ڴ�����
	WriteProcessMemory(hProcee, adrRemoteData, &remoteData, sizeof(remoteData), &lwt);//��Զ�̲���remoteData��д���ڴ�
	WriteProcessMemory(hProcee, adrRemoteProc, RemoteThreadProc, 0x1400, &lwt);        //��ڵ�ע�뺯��RemoteThreadProc��д���ڴ�
	
	//3.����Զ�̴���
	  //(1)����һ�� ������ע��ģ�顱�Ĵ������ݣ����ڽ��д�������
	char _code[0x200]; 
	memcpy(_code, INJIECTCode, sizeof(_code));
	
	  //(2)���д�����������ռλ�����ڴ��ַ�滻Ϊ�������ڴ��ַ��
	for (int i = 0;i < 0x100;i++)//�����0x100Ҫ����INJIECTCode�еĴ���������
	{
		unsigned* pcode = (unsigned*)(&_code[i]);//��ʱָ�룬ָ�� ����������ע��ģ�顱��ĳ��������ݣ�ÿ��ָ���ڴ�ռ��4���ֽڣ���ΪΪunsigned���ͣ�
		//AfxMessageBox(L"��ʼ������");
		if (*pcode == 0xCCCCCCCC)//�жϸÿ��ڴ���������ǲ���������Ҫ����������
		{
			*pcode = (unsigned)adrRemoteData; //��ռλ��Զ�̲�������ַ��cc������Ϊ��ʵ���ڴ��ַ��д��pcodeָ���_code���ڴ��У����޸ĵ���_code[]�е����ݡ�
			break;
		}
	}
	//(3)��������� ע��ģ�� ������(����_code[]��),д��������ڴ���ȥ
	WriteProcessMemory(hProcee, adrRemote, _code, 0x200, &lwt);
	

	//4.����һ���߳�,��ִ�� ���Ƿ�����ڴ��� ���޸ĳ�����ڵ㡱 �Ĵ��롾�ⲽʧ���ˡ�
	DWORD dwThreadId{};
	HANDLE remoteHdl = CreateRemoteThread(hProcee, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemoteProc, adrRemoteData, 0, &dwThreadId);//�����̣߳�ִ�� ���Ƿ�����ڴ��У�����ڵ�ע�롱�Ĵ���
	int x = GetLastError();
	WaitForSingleObject(remoteHdl,INFINITE);//�ȴ��߳�ִ�н���
	CloseHandle(remoteHdl);

	//�߳�ע��
	//DWORD dwThreadId;
	//HANDLE remoteHdl = CreateRemoteThread(hProcee, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemoteProc, adrRemoteData, 0, &dwThreadId);//�����̣߳�ִ�� ���Ƿ�����ڴ��У��޸ĳ�����ڵ�Ĵ���
	//DWORD dw = WaitForSingleObject(remoteHdl, INFINITE);//�ȴ��߳�ִ�н���
	//CloseHandle(remoteHdl);

	return 0;
}

