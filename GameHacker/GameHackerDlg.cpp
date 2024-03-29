﻿
// GameHackerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GameHacker.h"
#include "GameHackerDlg.h"
#include "afxdialogex.h"
#include "CWndAddGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGameHackerDlg 对话框



CGameHackerDlg::CGameHackerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAMEHACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameHackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
}

BEGIN_MESSAGE_MAP(CGameHackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CGameHackerDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGameHackerDlg 消息处理程序

//安装子选项卡（要进行绑定的类对象+要绑定的子对话框+设置选项卡名字+是否显示选项卡）
bool CGameHackerDlg::InstallPage(CDialogEx* _Wnd, int IDD_WND, CString&& _Name, BOOL IsShow)
{
	if (CurPage >= MAX_PAGE)return false;
	Pages[CurPage] = _Wnd;               //将当前子对话框类实例，加入到管理选项卡的指针数组里去
	Pages[CurPage]->Create(IDD_WND);    //让子对话框类实例，和子对话框 绑定
	Pages[CurPage]->SetParent(this);    //设置子对话框的父窗口 为主对话框（让子对话框在主对话框上显示）
	Pages[CurPage]->ShowWindow(IsShow); //显示子对话框

	CRect rect;
	mTab.GetClientRect(&rect);         //获得 选项卡控件 位于客户区的坐标，存到rect中去
	rect.top += 23;
	rect.left += 0;
	rect.right += 5;
	rect.bottom += 2;				   //不断调节选项卡控件的坐标，以和其所显示的子对话框贴合
	Pages[CurPage]->MoveWindow(&rect);  //设置子对话框的位置坐标，以贴合选项卡控件所能显示的区域
	mTab.InsertItem(CurPage, _Name);   //为选项卡设置名字

	CurPage++;                        //指向下一个选项卡的下标
	return true;
}

BOOL CGameHackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//获得文件夹路径
	TCHAR FileModile[0x100]{};//接收文件夹路径（可执行文件的路径 ）
	GetModuleFileName(NULL, FileModile, 0x100);//检索当前进程的可执行文件的路径  指针接收 接收的大小
	  //将可执行文件的程序名部分截断，只留文件夹路径部分
	int i;
	for (i = 0x100;FileModile[i] != '\\';i--);
	FileModile[i] = '\0';
	 //将获得的路径，用全局变量wAppPath保存，因为后面还会用到
	wAppPath.Format(L"%s", FileModile);
	//AfxMessageBox(FileModile);//测试
	

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	 //安装选项卡
	InstallPage(&PageINJ, IDD_PAGE_0, L"注入", TRUE);
	InstallPage(&PageINC, IDD_PAGE_1, L"注入检测");
	InstallPage(&PageRAN, IDD_PAGE_2, L"分析工具");

	 //将exe文件夹路径传入子窗口-注入页面
	PageINJ.GiveAppPath(wAppPath);
	// 将exe文件夹路径传入子窗口-分析工具页面
	PageRAN.SetAppPath(wAppPath);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGameHackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGameHackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGameHackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//选项卡切换：切换之后显示对应选项卡的画面
void CGameHackerDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int n = mTab.GetCurSel(); //用于取得组合框中当前选中的项的下标,即获得当前选中的选项卡的下标
	for (int i = 0;i < CurPage;i++)
	{
		Pages[i]->ShowWindow(i == n);
	}
}

//动态调整主对话框大小
void CGameHackerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//nType是1时，为最小化窗口，不做处理
	if (nType == 1)return;

	//nType是其他值时，才做处理
	if (Pages[CurPage]) {//必须在这里判断选项卡初始化了没有，不然会报错
		CRect rect;
		mTab.GetClientRect(&rect);         //获得 选项卡控件 位于客户区的坐标，存到rect中去
		rect.top += 23;
		rect.left += 0;
		rect.right += 5;
		rect.bottom += 2;				   //不断调节选项卡控件的坐标，以和其所显示的子对话框贴合
		for (int i = 0;i < CurPage;i++)//每个选项卡页面都进行设置
		{
			Pages[i]->MoveWindow(&rect);  //新设置选项卡控件所能显示的区域，以和子对话框贴合
		}
	}
}

//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
  //这两个函数定义在基类中，而基类默认截取了ESC和回车等键的消息，然后映射到OK或cancer函数上，从而出现按键退出现象
void CGameHackerDlg::OnOK()
{
}

void CGameHackerDlg::OnCancel()
{
}



//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
void CGameHackerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();
	CDialogEx::OnCancel();//直接调用基类的OnCancel函数，关闭对话框并返回IDCANCEL
	 //因为OnClose最后会间接调用到OnCancel或OnOK，而我们已经重载了，所以会优先调用我们的，但我们重载的
	//函数没有代码，即调用我们的函数不能正常关闭窗口
	//所以我们要直接调用基类的OnCancel来关闭窗口
}
