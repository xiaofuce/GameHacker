#pragma once
#include "afxdialogex.h"


// CWndModAnly 对话框

class CWndModAnly : public CDialogEx
{
	DECLARE_DYNAMIC(CWndModAnly)

public:
	CWndModAnly(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndModAnly();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOD_ANLY };
#endif

protected:
	//重写列表控件 初始化的虚函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 分析页面的列表
	CListCtrl LstAnly;
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);

	//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
	void OnOK();
	void OnCancel();
	//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
	afx_msg void OnClose();
};
