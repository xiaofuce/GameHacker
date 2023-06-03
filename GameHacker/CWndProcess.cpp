// CWndProcess.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndProcess.h"
#include <TlHelp32.h>
#include "CWndINC.h"

CWndINC* wndINC;//实例化一个INJECTCHEK窗体类对象 wndINC，用于数据传递

// CWndProcess 对话框

IMPLEMENT_DYNAMIC(CWndProcess, CDialogEx)

CWndProcess::CWndProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_LIST, pParent)
{

}

CWndProcess::~CWndProcess()
{
}

//重载 进程列表窗口 的初始化函数
BOOL CWndProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();// 使用父类的基本初始化功能

	///*
	//代码实现，我们的功能
	//*/
	
	//调整列表样式
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ProcessLst.m_hWnd, GWL_STYLE);// 传入程序列表list Control 的句柄，返回 样式数据/对象
	lStyle |= LVS_REPORT;   // 指定报表视图样式
	SetWindowLongPtr(ProcessLst.m_hWnd, GWL_STYLE, lStyle);// 设置/应用 样式

	DWORD dStyle = ProcessLst.GetExtendedStyle(); // 获得程序类别的 拓展样式数据/对象
	dStyle |= LVS_EX_FULLROWSELECT; // 选中某个项后，将突出显示该项及其所有子项。 此样式仅与 LVS_REPORT 样式结合使用。
	dStyle |= LVS_EX_GRIDLINES;     // 显示项和子项周围的 网格线。 此样式仅与 LVS_REPORT 样式结合使用。
	ProcessLst.SetExtendedStyle(dStyle);// 设置/应用 样式

	ProcessLst.InsertColumn(0, L"PID", 0, 200);
	ProcessLst.InsertColumn(1, L"文件名", 0, 400);

	//刷新出进程列表
	RefreshProcess();


	return TRUE;
}

void CWndProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ProcessLst);
}

//刷新出进程列表
void CWndProcess::RefreshProcess()
{
	//清空进程列表项记录
	ProcessLst.DeleteAllItems();
	//创建快照，并做异常处理
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//快照类型：获取系统中的所有进程的快照；第二个参数是获取指定进程的快照，如果为0时，会获取系统中所有进程的详细信息，如果是指定进程，就只获取该进程的信息
	if (hSnap == INVALID_HANDLE_VALUE) //如果快照句柄无效
	{
		AfxMessageBox(L"获取进程列表失败！请检查是否具备管理员权限");
		return;
	}

	//这个结构体是用来存储进程的详细信息的，包括进程 ID、父进程 ID、进程路径、线程数等；结构体大小必须初始化
	PROCESSENTRY32 pe{ sizeof(pe) };
	BOOL BMORE = Process32First(hSnap, &pe);//获取第一个系统快照的进程信息，存储到pe结构体中去
	while (BMORE)//通过循环，不断获取进程信息，直到没有；进程应该是链表存储的，即可以
	{
		CString txt;
		txt.Format(L"%d", pe.th32ProcessID);//获取进程ID
		ProcessLst.InsertItem(0, txt);//将获取到的进程PID,插入到进程列表第一行的第一列
		ProcessLst.SetItemText(0, 1, pe.szExeFile);//将获取到的进程文件名，设置到进程列表第一行的第二列
		BMORE = Process32Next(hSnap, &pe); //获取下一个进程的信息，并覆盖存储到结构体pe里
	}
	CloseHandle(hSnap);
}


BEGIN_MESSAGE_MAP(CWndProcess, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndProcess::OnNMDblclkList1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWndProcess 消息处理程序

//双击选中进程列表里的目标进程
void CWndProcess::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//NM_LISTVIEW是一个存储 列表视图控件的通知消息 参数 的结构体
	//pNMHDR是一个指针，指向一个包含了通知消息的结构体
	NM_LISTVIEW* View = (NM_LISTVIEW*)pNMHDR;//简单来说，就是让 进程列表窗口，获得来自 双击 的消息信息。
	//当用户在进程列表窗口中双击列表视图控件时，会触发一个NM_DBLCLK通知消息，该消息会被发送到进程列表窗口的消息队列中
	
	if (View->iItem != -1) //如果选中了列表项 （没选中=-1.选中则!=-1）
	{
		wPid = ProcessLst.GetItemText(View->iItem, 0);//获得选中的进程项的ID
		wExe = ProcessLst.GetItemText(View->iItem, 1);//获得选中的进程项的Exe文件名
		dPid = wcstoul(wPid.GetBuffer(), 0,10);       //将宽字符字符串的PID转换为无符号长整数
		OnOK();//函数执行后，会调用EndDialog函数，关闭对话框，并将 IDOK 作为返回值传递给操作系统
		       //我们就通过系统的IDOK值，判断当前函数已经执行完，以此在检测窗体中获得当前窗体的数据
		         
		//wndINC->SetProcess(dPid, wExe, IsFirst);//在进程列表的窗体中, 通过wndINC.SetProcess()，
		//                                        //就成功把 打开的进程的信息通过指针传递给了INJECTCHEK窗体
		//                                        //IsFirst是判断读取哪一个进程的数据（正常进程，还是感染进程），由调用Init函数时传入
	}
	//OnCancel();//双击以后就退出。让程序能够正确地处理取消操作，例如关闭对话框、停止当前操作等
}
////初始化函数，主要用于传入INC窗口的指针
//void CWndProcess::Init(LPVOID lPtr, BOOL _FIRST)
//{
//	wndINC = (CWndINC*)lPtr;//把INJECTCHEK窗体的指针，传给 进程列表的窗体中的 实例化的INJECTCHEK窗体类对象 wndINC 
//	IsFirst = _FIRST;
//}

