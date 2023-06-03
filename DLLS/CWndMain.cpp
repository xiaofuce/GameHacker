// CWndMain.cpp: 实现文件
//

#include "pch.h"
#include "DLLS.h"
#include "afxdialogex.h"
#include "CWndMain.h"

// CWndMain 对话框

IMPLEMENT_DYNAMIC(CWndMain, CDialogEx)

CWndMain::CWndMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WNDMAIN, pParent)
{

}

CWndMain::~CWndMain()
{
}

void CWndMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndMain::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWndMain 消息处理程序

//“隐藏当前模块”按钮的响应函数
void CWndMain::OnBnClickedButton1()
{
	htdModule* p{};//实例化一个类对象
	p->HideDll(L"DLLS.dll");//传入要隐藏的模块名称
	AfxMessageBox(L"ok");
}
