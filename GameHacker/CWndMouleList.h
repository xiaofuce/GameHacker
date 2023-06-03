#pragma once
#include "afxdialogex.h"


// CWndMouleList 对话框

class CWndMouleList : public CDialogEx
{
	DECLARE_DYNAMIC(CWndMouleList)

public:
	CWndMouleList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndMouleList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOD_LIST };
#endif

protected:
	//重载初始化函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 数据显示窗口的列表控件
	CListCtrl ListMoule;

	//获取进程模块列表
	void GetModList(DWORD Pid);//传入进程ID，就能获得各模块信息

	//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
	void OnOK();
	void OnCancel();
	//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
	afx_msg void OnClose();
};
