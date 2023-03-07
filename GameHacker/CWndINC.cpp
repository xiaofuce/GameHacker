// CWndINC.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndINC.h"


// CWndINC 对话框

IMPLEMENT_DYNAMIC(CWndINC, CDialogEx)

CWndINC::CWndINC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_1, pParent)
{

}

CWndINC::~CWndINC()
{
}

void CWndINC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndINC, CDialogEx)
END_MESSAGE_MAP()


// CWndINC 消息处理程序
