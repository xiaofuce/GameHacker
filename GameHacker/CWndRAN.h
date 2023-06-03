#pragma once
#include "afxdialogex.h"

#include "CWndAddClass.h" //用于弹出 设置对象窗口的
#include "CWndProcess.h"  //用于弹出 进程列表窗口的

// CWndRAN 对话框

class CWndRAN : public CDialogEx
{
	DECLARE_DYNAMIC(CWndRAN)

public:
	CWndRAN(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndRAN();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_2 };
#endif

protected:
	virtual BOOL OnInitDialog();//重写OnInitDialog函数，用于为List绘制表格
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
	void OnOK();
	void OnCancel();
public:
	CWndProcess wndSelProcess;//用于弹出 进程列表窗口的
	CWndAddClass wndAddClass; //用于弹出 设置对象窗口的
	// Tree控件
	CTreeCtrl mTree;
	// List控件，显示节点的数据
	CListCtrl LstData;
	afx_msg void OnBnClickedButton1();

	//在Tree 控件处右键时，显示菜单
	afx_msg void OnNMRClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//菜单-加载游戏
	afx_msg void OnLoadGame();

	//保存选中、打开的进程句柄
	HANDLE hProcess{};

	//存储来自主窗体的文件夹路径（exe的文件夹路径）
	CString wAppPath;
	//存储用wAppPath拼接出的、保存配置文件的 文件夹RAN 的路径
	CString wAnlyPath;
	//接收来自主窗体的文件夹路径（exe的文件夹路径），并拼接出保存配置文件的文件夹RAN 的路径
	void SetAppPath(const wchar_t* path);

	//存储配置文件ini的文件夹路径（根据进程名，动态拼接出来的）
	CString wAnlyData;
};
