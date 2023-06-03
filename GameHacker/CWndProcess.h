#pragma once
#include "afxdialogex.h"


// CWndProcess 对话框

class CWndProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CWndProcess)

public:
	CWndProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_LIST };
#endif

protected:
	//重载初始化函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 进程列表控件
	CListCtrl ProcessLst;

	//刷新出进程列表
	void RefreshProcess();

	//双击选中进程列表里的目标进程
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);

	//在Init里初始化，用于判断是要读取哪一个进程的数据（正常进程还是感染进程）
	BOOL IsFirst;

	CString wPid ; //获得选中的进程项的ID
	CString wExe ; //获得选中的进程项的Exe文件名
	DWORD   dPid ; //将进程项的宽字符字符串的PID,转换为无符号长整数
	////初始化函数，主要用于传入INC窗口的指针
	//void Init(LPVOID lPtr, BOOL _FIRST = TRUE);
	// 

};
