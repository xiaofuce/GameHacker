// CWndMouleList.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndMouleList.h"
#include <TlHelp32.h>  //拍摄快照的头文件

// CWndMouleList 对话框

IMPLEMENT_DYNAMIC(CWndMouleList, CDialogEx)

CWndMouleList::CWndMouleList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_LIST, pParent)
{

}

CWndMouleList::~CWndMouleList()
{
}

BOOL CWndMouleList::OnInitDialog()
{
	CDialogEx::OnInitDialog();// 使用父类的基本初始化功能

	///*
	//代码实现，我们的功能
	//*/
	//调列表样式
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ListMoule.m_hWnd, GWL_STYLE);// 传入程序列表list Control 的句柄，返回 样式数据/对象
	lStyle |= LVS_REPORT;   // 指定报表视图样式
	SetWindowLongPtr(ListMoule.m_hWnd, GWL_STYLE, lStyle);// 设置/应用 样式

	DWORD dStyle = ListMoule.GetExtendedStyle(); // 获得程序类别的 拓展样式数据/对象
	dStyle |= LVS_EX_FULLROWSELECT; // 选中某个项后，将突出显示该项及其所有子项。 此样式仅与 LVS_REPORT 样式结合使用。
	dStyle |= LVS_EX_GRIDLINES;     // 显示项和子项周围的 网格线。 此样式仅与 LVS_REPORT 样式结合使用。
	ListMoule.SetExtendedStyle(dStyle);// 设置/应用 样式

	//在列表的第一行的每一列插入列明
	ListMoule.InsertColumn(0, L"模块名称", 0, 100);
	ListMoule.InsertColumn(1, L"基址", 0, 200);
	ListMoule.InsertColumn(2, L"大小", 0, 200);
	ListMoule.InsertColumn(3, L"文件位置", 0, 200);

	return TRUE;
}

void CWndMouleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ListMoule);
}

//获取进程模块列表
void CWndMouleList::GetModList(DWORD Pid)
{
	//获得指定进程含有的所有模块信息
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid);
	if (hSnap != INVALID_HANDLE_VALUE) //如果成功获得了进程模块信息，句柄不为空
	{
		MODULEENTRY32 modl{ sizeof(MODULEENTRY32) };//存储模块信息的结构体
		BOOL BMORE = Module32First(hSnap, &modl); //获得第一个模块的信息
		while (BMORE)//循环：处理当前模块信息，获取下一个的模块。估计也是链表结构
		{
			CString txt;
			txt.Format(L"%X", modl.hModule);//处理、获得16进制的模块地址
			ListMoule.InsertItem(0, modl.szModule);//在第一行的第一列插入模块名
			ListMoule.SetItemText(0, 1,txt);       //第一行第二列设置模块地址
			txt.Format(L"%X", modl.modBaseSize);//处理、获得16进制的模块大小
			ListMoule.SetItemText(0, 2,txt);       //第一行第三列设置模块基址
			ListMoule.SetItemText(0, 3, modl.szExePath);//第一行第四列设置模块文件路径
			BMORE = Module32Next(hSnap, &modl);   //获得下一个模块的信息
		}
		//注意：因为模块名是插入，所以插入时，新的一列都空出来的、上一个模块信息整体下移
	}
	//关闭快照
	CloseHandle(hSnap);
}


BEGIN_MESSAGE_MAP(CWndMouleList, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWndMouleList 消息处理程序


//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
  //这两个函数定义在基类中，而基类默认截取了ESC和回车等键的消息，然后映射到OK或cancer函数上，从而出现按键退出现象
void CWndMouleList::OnOK()
{
}

void CWndMouleList::OnCancel()
{
}



//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
void CWndMouleList::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();
	CDialogEx::OnCancel();//直接调用基类的OnCancel函数，关闭对话框并返回IDCANCEL
	//因为OnClose最后会间接调用到OnCancel或OnOK，而我们已经重载了，所以会优先调用我们的，但我们重载的
   //函数没有代码，即调用我们的函数不能正常关闭窗口
  //所以我们要直接调用基类的OnCancel来关闭窗口

}