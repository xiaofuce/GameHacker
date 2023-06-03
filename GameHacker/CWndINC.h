#pragma once
#include "afxdialogex.h"
#include "CWndProcess.h"
#include "CWndMouleList.h"
#include "CWndModAnly.h"
#define MAX_PAGE 3 //要安装的页面数量

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
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	//声明一个进程列表窗口的类对象
	CWndProcess wndProcess;
	// “打开进程”所打开的进程名
	CString UExe;
	// “打开感染进程”所打开的进程名
	CString UExeRoot;

	//用于读取进程列表窗口中的数据
	void SetProcess(unsigned Pid, CString& wExe, BOOL bFirst = FALSE);
	afx_msg void OnBnClickedButton7();

	// 注入检测窗口的tab
	CTabCtrl mTab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	
	//用一个数组，来管理类对象：声明2个窗口，一个“正常进程”用，一个“感染进程”用
	CWndMouleList modPage[2];
	//实例化 “分析结果”的窗口页面
	CWndModAnly modAnly;

	//管理选项卡（即tab页/声明的窗口）的指针  （因为我们是在tab页里嵌套窗口）
	CDialogEx* Pages[MAX_PAGE];//当下标没有保存指针时，为null

	//记录当前选项卡的下标，即当前是哪一个tab页（的窗口）
	short CurPage = 0;         
	//安装子选项卡（要进行绑定的类对象+要绑定的子对话框+设置选项卡名字+是否显示选项卡内容）
	bool InstallPage(CDialogEx* _Wnd, int IDD_WND, CString&& _Name, BOOL IsShow);


	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton3();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
	void OnOK();
	void OnCancel();
	//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
	afx_msg void OnClose();
};
