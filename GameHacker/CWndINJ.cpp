// CWndINJ.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndINJ.h"


// CWndINJ 对话框

IMPLEMENT_DYNAMIC(CWndINJ, CDialogEx)

CWndINJ::CWndINJ(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_0, pParent)
{

}

CWndINJ::~CWndINJ()
{
}

void CWndINJ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndINJ, CDialogEx)
END_MESSAGE_MAP()


// CWndINJ 消息处理程序
