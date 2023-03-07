#pragma once
#include "afxdialogex.h"


// CWndINC 对话框

class CWndINC : public CDialogEx
{
	DECLARE_DYNAMIC(CWndINC)

public:
	CWndINC(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndINC();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
