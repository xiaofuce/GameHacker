
// GameHackerDlg.h: 头文件
//

#pragma once
#include "CWndINC.h"
#include "CWndINJ.h"
#define MAX_PAGE 2

// CGameHackerDlg 对话框
class CGameHackerDlg : public CDialogEx
{
// 构造
public:
	CGameHackerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEHACKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	
	CWndINC PageINC;
	CWndINJ PageINJ;

	CDialogEx* Pages[MAX_PAGE];//管理页面的指针
	short CurPage = 0;         //当前页面下标
	//安装子选项卡（要进行绑定的类对象+要绑定的子对话框+设置选项卡名字+是否显示选项卡内容）
	bool InstallPage(CDialogEx* _Wnd, int IDD_WND, CString&& _Name, BOOL IsShow=FALSE);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 选项卡
	CTabCtrl mTab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
