#pragma once
#include "afxdialogex.h"


// CWndAddGame 对话框

class CWndAddGame : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAddGame)

public:
	CWndAddGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndAddGame();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDGAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CString GameName;//程序名
	CString GamePath;//程序所在文件路径
	CString GameFullPath;//完整程序文件路径
	CString GameCmds;//程序的命令行参数
	CString DLLPath;//注入模块的路径
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
public:
	void* father;//用于接受父控件的指针
	bool IsCreate;//获得 是要添加新游戏数据条目，还是在原有条目上重新设置 的讯息
	int index;   //存储要右键-设置的列表项的编号
	void Init(void* _father, bool IsCreate=true,int _index=0);//初始化AddGame窗体的数据：获得父控件的对象指针、判断是否是在添加新游戏数据条目、要设置的目标列表项的编号
	void SetData( //将要重新设置的数据传给AddGame窗体(此函数在INJECT窗体调用)
		CString& _GameName,//程序名
		CString& _GamePath,//程序所在文件路径
		CString& _GameFullPath,//完整程序文件路径
		CString& _GameCmds,//程序的命令行参数
		CString& _DLLPath//注入模块的路径
	);
};
