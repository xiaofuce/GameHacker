#include "pch.h"
//#include <Windows.h>
#include "htdModule.h"

//ʵ��ģ�����ع��ܵ��ڲ�������ֻ����˽�е��ã�
void htdModule::HideDll(HMODULE _hMod) //����Ŀ��ģ��Ļ�ַ
{
   //һ.ɾ�������е�����
	//1.���PEB���洢���̵ĸ�����Ϣ��
	PPEB _peb;
	_asm
	{
		mov eax, fs: [0x30] ; //TEB�ͷ���fs�μĴ�����ͷ����TEB����ƫ��48���ֽ�(16����Ϊ30)������PEB
		mov _peb, eax;
	}
	//2.��� �����Ѽ��ص�ģ����Ϣ
	PPEB_LDR_DATA Pldr = _peb->Ldr;
	//3.��� �洢ģ����Ϣ������ ��ͷָ��
	PLIST_ENTRY FIRST = &(Pldr->InMemoryOrderModuleList);
	PLIST_ENTRY CURST = FIRST->Flink; //��һ����ǰָ�룬ָ������ĵ�һ��Ԫ��

	//4.��� ����Ԫ�� �� ����ģ����Ϣ�ṹ���е����ƫ���� ��ԭ������鿴�ʼ� ��ģ�������ʾ��ͼ����
	PLDR_DATA_TABLE_ENTRYA lDis{};
	int StrctDiff = (int)&lDis->InMemoryOrderLinks;//��Ϊ�����ɵĽṹ�壬���������������ʼ��ַΪ0

	//5.����ģ��˫������ֱ���ҵ�Ŀ��ģ��
	PLDR_DATA_TABLE_ENTRYA lModule;
	do {
		lModule = (PLDR_DATA_TABLE_ENTRYA)((unsigned)CURST - StrctDiff);//����Ԫ�ص�ָ���ȥ���ƫ���������Ǿ���ģ����Ϣ�Ľṹ�����ʼ��ַ
		if (lModule->DllBase == _hMod) //�����Ŀ��ģ��Ļ�ַ
		{
			//���������У�����ǰģ��Ԫ�ء�������
			lModule->InLoadOrderModuleList.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
			lModule->InLoadOrderModuleList.Flink->Blink = lModule->InLoadOrderModuleList.Blink;

			lModule->InMemoryOrderLinks.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
			lModule->InMemoryOrderLinks.Flink->Blink = lModule->InLoadOrderModuleList.Blink;

			lModule->InInitializationOrderModuleList.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
			lModule->InInitializationOrderModuleList.Flink->Blink = lModule->InLoadOrderModuleList.Blink;
		}
		//�������Ŀ��ģ�飬���õ�ǰԪ��ָ�� ָ����һ��Ԫ��
		CURST = CURST->Flink; 

	} while (CURST != FIRST);//ֻҪ��ǰԪ��ָ�� û����ת��������һֱ����

	//��.ɾ���ڴ��е�PEͷ��dosͷ
	//1.���PEͷ��dosͷ
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)_hMod; //ģ��� DOS ͷ����λ��ģ��Ļ�ַ����ʼ��ַ����
														     //PE ͷ�ǽ����� DOS ͷ�����һ���ṹ�壬����λ���� DOS ͷ��һ���ֶ� e_lfanew ָ��
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(dosHeader->e_lfanew + (unsigned)_hMod);//e_lfanew ��ʾ PE ͷ���ļ�ƫ�������������ļ��е�λ�� �����ļ���ʼ��ַ���ֽ�����
	                                                              //����ģ���Ѿ������ص��ڴ��У����ֻ��Ҫ ��PEͷ��ƫ���� + ģ��Ļ�ַ _hMod�����ܵõ� PE ͷ���ڴ��еĵ�ַ��

	//2.ɾ������
	IMAGE_DOS_HEADER dosH{};
	IMAGE_NT_HEADERS ntH{};

	DWORD dOld;
	VirtualProtect(dosHeader, sizeof(dosH), PAGE_EXECUTE_READWRITE, &dOld); //�޸��ڴ����ԣ������޸��ڴ�
	VirtualProtect(ntHeader, sizeof(ntH), PAGE_EXECUTE_READWRITE, &dOld);
	
	memcpy(dosHeader, &dosH, sizeof(dosH)); //��c++���ڴ溯��������ݵ����dosͷ��PEͷ��ȫ������Ϊ0��
	memcpy(ntHeader,   &ntH, sizeof(ntH));
}

//�����ṩ�Ľӿڣ�����ģ�����ƣ�Ȼ���ڲ�����ʵ�����أ�
void htdModule::HideDll(wchar_t* dllName)
{
	//1.��ô����ģ��ľ��
	HMODULE hMod = GetModuleHandleW(dllName);//���Ŀ��ģ��ľ����Ҳ����ģ��Ļ�ַ����Ϊ��Windows����ϵͳ�У�ģ����ʵ���Ͼ���ģ��Ļ�ַ��
	                                         //����ͨ����ģ����ǿ��ת��ΪHMODULE���͵�ָ������ȡģ��Ļ�ַ����
	//2.��ģ���������ڲ���������ʵ��ģ�����ع���
	if(hMod)  HideDll(hMod);
}
