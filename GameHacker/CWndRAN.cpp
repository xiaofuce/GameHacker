// CWndRAN.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndRAN.h"


// CWndRAN 对话框

IMPLEMENT_DYNAMIC(CWndRAN, CDialogEx)

CWndRAN::CWndRAN(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_2, pParent)
{

}

CWndRAN::~CWndRAN()
{
}

//重写OnInitDialog函数，用于为List绘制表格
BOOL CWndRAN::OnInitDialog()
{
	CDialogEx::OnInitDialog();// 使用父类的基本初始化功能

	//调列表样式
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(LstData.m_hWnd, GWL_STYLE);// 传入程序列表list Control 的句柄，返回 样式数据/对象
	lStyle |= LVS_REPORT;   // 指定报表视图样式
	SetWindowLongPtr(LstData.m_hWnd, GWL_STYLE, lStyle);// 设置/应用 样式

	DWORD dStyle = LstData.GetExtendedStyle(); // 获得程序类别的 拓展样式数据/对象
	dStyle |= LVS_EX_FULLROWSELECT; // 选中某个项后，将突出显示该项及其所有子项。 此样式仅与 LVS_REPORT 样式结合使用。
	dStyle |= LVS_EX_GRIDLINES;     // 显示项和子项周围的 网格线。 此样式仅与 LVS_REPORT 样式结合使用。
	LstData.SetExtendedStyle(dStyle);// 设置/应用 样式

	LstData.InsertColumn(0, L"内存地址", 0, 200);
	LstData.InsertColumn(1, L"偏移量", 0, 400);
	LstData.InsertColumn(2, L"变量类型", 0, 400);
	LstData.InsertColumn(3, L"变量名称", 0, 400);
	LstData.InsertColumn(4, L"变量值", 0, 400);
	LstData.InsertColumn(5, L"注释", 0, 400);

	return TRUE;
}

void CWndRAN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, mTree);
	DDX_Control(pDX, IDC_LIST1, LstData);
}

//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
void CWndRAN::OnOK()
{
}

void CWndRAN::OnCancel()
{
}


BEGIN_MESSAGE_MAP(CWndRAN, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndRAN::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CWndRAN::OnNMRClickTree1)
	ON_COMMAND(ID_32773, &CWndRAN::OnLoadGame)
END_MESSAGE_MAP()


// CWndRAN 消息处理程序

//button的响应函数
void CWndRAN::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	auto val = mTree.InsertItem(L"剑侠", TVI_ROOT); // Tree控件中，插入根节点
	mTree.InsertItem(L"DATA", val);  // Tree控件中，在根节点下插入一个节点
}

//在Tree 控件处右键时，显示菜单
void CWndRAN::OnNMRClickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD dMenuId = 0;//用于判断要使用哪一个子菜单（菜单本质是数组）
	if (hProcess) //如果分析工具窗体 打开了目标进程，才进入
	{
		auto hSel  = mTree.GetSelectedItem(); //获取当前选中的Tree节点
		auto hRoot = mTree.GetRootItem();     //获取Tree控件的根节点
		if (hSel == hRoot) dMenuId = 1;   //如果相等，说明已经使用过 “加载游戏”子菜单了，接下来右键时只能显示“创建对象”等选项
	}

	//获得上一个消息发生的时候(即右击列表时)，光标所在的坐标;若要确定光标的当前位置，请使用 GetCursorPos 函数。
	DWORD dwPos = GetMessagePos();
	//通过宏函数，获得DWORD变量（即光标的坐标）的低位（X）和高位（Y），并作为一个无符号短整型（WORD）返回。
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;

	//加载 添加的 菜单资源(本质上是一个数组）
	menu.LoadMenuW(IDR_MENU1);

	//动态获得目标子菜单（即是我们想要的右键菜单） 【1和2是分析工具窗体的子菜单,1是“加载游戏”选项，二是“创建对象”等选项】
	CMenu* pop = menu.GetSubMenu(1 + dMenuId); //其中的值，是动态的，以适应不同选择情况
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);//将获得的子菜单弹出来（设置弹出的方式、样子和位置）

}

//Tree控件处，右键-加载游戏 的消息处理函数
void CWndRAN::OnLoadGame()
{
	// TODO: 在此添加命令处理程序代码

	//右键-加载游戏后，先弹出程序列表
	if (wndSelProcess.DoModal() == IDOK) {
		
		//打开选中的进程  1
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wndSelProcess.dPid);
		if (hProcess)
		{
			//在根节点处插入 进程名称 2
			auto tRoot = mTree.InsertItem(wndSelProcess.wExe, TVI_ROOT);
			//在根节点下插入 一些项目
			mTree.InsertItem(L"公共数据", tRoot);
			mTree.InsertItem(L"公共函数", tRoot);
			mTree.InsertItem(L"HOOK分析", tRoot);

			//拼接出配置文件ini的 文件夹路径（根据进程名，动态拼接出来的） 3
			wAnlyData = wAnlyPath + wndSelProcess.wExe;
			//判断是否创建ini配置文件的文件夹
			if (!PathIsDirectory(wAnlyData)) //如果配置文件的 文件夹不存在，就创建（一般只有第一次才会创建）
			{
				if (!CreateDirectory(wAnlyData, NULL))//创建文件夹失败时弹出的提示
				{
					AfxMessageBox(L"创建文件夹失败！");
					return; 
				}
			}
		}

		//加载配置文件  4
		  //a.先查找到配置文件
		WIN32_FIND_DATA fileData;   //存储查找到的文件的相关信息  的结构体
		HANDLE hFind = FindFirstFile(wAnlyData + L"\\*.ini", &fileData); //查找文件都是链表式的，这是第一个查找的头
		//如果链表第一个元素查找成功，就进入循环、查找下一个
		while (hFind != INVALID_HANDLE_VALUE) 
		{
			//b.查找到以后，数据加载到Tree控件面板上
			auto hRoot = mTree.InsertItem(fileData.cFileName, TVI_ROOT); //根节点 -

			auto hBase = mTree.InsertItem(L"基本信息", hRoot);  //根节点下的  基本信息项 --
			     mTree.InsertItem(L"内存地址", hBase); //基本信息项 下 的 子信息 ---
				 mTree.InsertItem(L"对象大小", hBase);
				 mTree.InsertItem(L"注释", hBase);

			mTree.InsertItem(L"成员变量", hRoot);//根节点下的  成员变量项 --
			mTree.InsertItem(L"成员函数", hRoot);//根节点下的  成员函数项 --

			//链式向下查找，直到找不到，才退出循环
			if (!FindNextFile(hFind, &fileData)) break;

		}
		FindClose(hFind);//查找结束后，关闭句柄
	}
	else 
	{
		AfxMessageBox(L"游戏加载失败！");
	}
	//再双击目标进程后，弹出 设置对象属性窗口
	if (wndAddClass.DoModal() == IDOK) {

	}
}

//接收来自主窗体的文件路径（exe文件路径），并拼接出保存配置文件 的文件夹路径
void CWndRAN::SetAppPath(const wchar_t* path)
{
	wAppPath = path;//exe文件所在的路径
	wAnlyPath = wAppPath + L"\\RAN\\";//拼接出 配置文件要保存到的文件夹  的路径
}
