#pragma once
#include "afxdialogex.h"
#include "CWndAddGame.h"
#include "INJCET.h"

// CWndINJ 对话框

class CWndINJ : public CDialogEx
{
	DECLARE_DYNAMIC(CWndINJ)

public:
	CWndINJ(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndINJ();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_0 };
#endif

protected:
	//声明一个注入类对象
	INJCET  m_INJECT;          
	//重载初始化函数
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 程序列表
	CListCtrl ExeLst;
	// 注入
	BOOL B_INJECT;
	// 暂停
	BOOL B_PAUSE;
	// 调试	
	BOOL B_DEBUG;
	// 添加游戏
	CButton B_ADDGAME;
	afx_msg void OnBnClickedButton5();

	//声明添加游戏窗口的类
	CWndAddGame wndAddGame;

	//接收主对话框传来的文件夹路径
	CString AppPath;

	//配置文件的文件路径
	CString GamesIni;


	//获得文件夹路径（在主对话框中执行）
	void GiveAppPath(CString& _wAppPath);//传入主对话框中 存储文件夹路径的变量的引用

	//保存“添加游戏”后所获得的信息到 配置文件 中去【即写文件】
	void AddGames(
		CString& _GameName,//程序名
		CString& _GamePath,//程序所在文件路径
		CString& _GameFullPath,//完整程序文件路径
		CString& _GameCmds,//程序的命令行参数
		CString& _DLLPath//注入模块的路径
	);

	//添加配置文件中的游戏信息，到程序列表
	void LoadGamesImfo();

	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);

	//记录右键菜单 选的是哪一个选项
	int lstSel;
	afx_msg void OnMenuSet();
	afx_msg void OnMenuDel();

	//删除列表项后，将列表上的游戏数据 重新覆盖写到配置文件中
	void SaveLstToIni();

	//保存要重新“设置”的目标项 的数据
	void SaveSetting(
		CString& _GameName,//程序名
		CString& _GamePath,//程序所在文件路径
		CString& _GameFullPath,//完整程序文件路径
		CString& _GameCmds,//程序的命令行参数
		CString& _DLLPath,//注入模块的路径
		int index
	);

	//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
	void OnOK();
	void OnCancel();
	//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
	afx_msg void OnClose();
};
