// CWndAddClass.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndAddClass.h"


// CWndAddClass 对话框

IMPLEMENT_DYNAMIC(CWndAddClass, CDialogEx)

CWndAddClass::CWndAddClass(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDCLASS, pParent)
{

}

CWndAddClass::~CWndAddClass()
{
}

void CWndAddClass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndAddClass, CDialogEx)
END_MESSAGE_MAP()


// CWndAddClass 消息处理程序
