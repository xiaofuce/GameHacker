// CWndAddGame.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndAddGame.h"
#include "CWndINJ.h"

// CWndAddGame 对话框

IMPLEMENT_DYNAMIC(CWndAddGame, CDialogEx)

CWndAddGame::CWndAddGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDGAME, pParent)
	, GameName(_T(""))
	, GamePath(_T(""))
	, GameFullPath(_T(""))
	, GameCmds(_T(""))
	, DLLPath(_T(""))
{

}

CWndAddGame::~CWndAddGame()
{
}

void CWndAddGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, GameName);
	DDX_Text(pDX, IDC_EDIT2, GamePath);
	DDX_Text(pDX, IDC_EDIT3, GameFullPath);
	DDX_Text(pDX, IDC_EDIT4, GameCmds);
	DDX_Text(pDX, IDC_EDIT5, DLLPath);
}


BEGIN_MESSAGE_MAP(CWndAddGame, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndAddGame::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWndAddGame::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CWndAddGame::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CWndAddGame::OnBnClickedButton4)
END_MESSAGE_MAP()


// CWndAddGame 消息处理程序

//"打开文件"（三个点）的消息处理函数
void CWndAddGame::OnBnClickedButton2()
{
	//点击“三个点”以弹出小窗前，先保存编辑栏的数据到对应的控制变量中去,防止输入的数据被清空
	UpdateData(TRUE);
	//“打开文件”时的，可筛选的文件类型
	TCHAR szFilters[] = _T("exe(*.exe)|*.exe|data (*.data)|*.data|All Files (*.*)|*.*||");
	//打开对话框 不追加拓展名 不显示初始文件名 
	CFileDialog dialog(TRUE, NULL, NULL, 6, szFilters);//定义一个用于文件打开操作的对话框类对象
	//如果选择了文件并确定
	if (dialog.DoModal() == IDOK)
	{
		GameName = dialog.GetFileTitle();   //获得游戏进程名称
		GamePath = dialog.GetFolderPath();  //获得游戏进程所在文件夹的路径
		GameFullPath = dialog.GetPathName();//获得游戏进程的完整路径
		GameCmds = L"";						//接收游戏的命令行参数
		UpdateData(FALSE);                  //控制变量向控件传数据，以显示数据
	}
}

//"打开注入模块"（三个点）的消息处理函数
void CWndAddGame::OnBnClickedButton3()
{
	//点击“三个点”以弹出小窗前，先保存编辑栏的数据到对应的控制变量中去,防止输入的数据被清空
	UpdateData(TRUE);
	//“打开文件”时的，可筛选的文件类型
	TCHAR szFilters[] = _T("动态链接库(*.dll)|*.dll");
	//打开对话框 不追加拓展名 不显示初始文件名 
	CFileDialog dialog(TRUE, NULL, NULL, 6, szFilters);//定义一个用于文件打开操作的对话框类对象
	//如果选择了文件并确定
	if (dialog.DoModal() == IDOK)
	{
		DLLPath = dialog.GetPathName();//获得注入模块的完整路径
		UpdateData(FALSE);             //控制变量向控件传数据，以显示数据
	}
}

//"取消退出"的消息处理函数
void CWndAddGame::OnBnClickedButton6()
{
	CDialog::OnCancel();//退出，关闭当前窗口
}

//"确认添加"的消息处理函数【保存数据到配置文件】
void CWndAddGame::OnBnClickedButton4()
{
	CWndINJ* p = (CWndINJ*)father;
	UpdateData(TRUE);

	if (IsCreate)//如果是添加新游戏的数据
	{
		p->AddGames(GameName, GamePath, GameFullPath, GameCmds, DLLPath);//利用定义在父控件中的保存数据的函数，保存游戏数据到配置文件
	}
	else //如果是重新设置原游戏条目的数据
	{
		p->SaveSetting(GameName, GamePath, GameFullPath, GameCmds, DLLPath,index);
	}

	CDialog::OnCancel();//完成后退出、关闭当前窗口
}

//清除保存的变量数据（防止每次设置目标项后，添加新游戏时编辑框仍然会保持上次设置的数据）
//void cleanData()
//{
//	GameName=null;
//	GamePath;//程序所在文件路径
//	GameFullPath;//完整程序文件路径
//	GameCmds;//程序的命令行参数
//	DLLPath;//注入模块的路径
//}

//初始化AddGame窗体的数据
void CWndAddGame::Init(void* _father, bool Create, int _index)
{
	father = _father; //获得父控件的对象指针
	IsCreate = Create;//获得是 要添加新游戏数据条目，还是在原有条目上重新设置 的讯息
	index = _index;   //获得要设置的目标列表项的编号
}

//将要重新设置的数据传给AddGame窗体(此函数在INJECT窗体调用)
void CWndAddGame::SetData(CString& _GameName, CString& _GamePath, CString& _GameFullPath, CString& _GameCmds, CString& _DLLPath)
{
	GameName =_GameName;
	GamePath =  _GamePath;
	GameFullPath = _GameFullPath;
	GameCmds = _GameCmds;
	DLLPath = _DLLPath;

}
