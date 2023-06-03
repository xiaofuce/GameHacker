#pragma once
#include "afxdialogex.h"
#include "htdModule.h"

// CWndMain 对话框

class CWndMain : public CDialogEx
{
	DECLARE_DYNAMIC(CWndMain)

public:
	CWndMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WNDMAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
