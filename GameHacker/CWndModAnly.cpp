// CWndModAnly.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndModAnly.h"


// CWndModAnly 对话框

IMPLEMENT_DYNAMIC(CWndModAnly, CDialogEx)

CWndModAnly::CWndModAnly(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_ANLY, pParent)
{

}

CWndModAnly::~CWndModAnly()
{
}

//重写列表控件 初始化的虚函数
BOOL CWndModAnly::OnInitDialog()
{
	CDialogEx::OnInitDialog();// 使用父类的基本初始化功能

	///*
	//代码实现，我们的功能
	//*/
	//调列表样式
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(LstAnly.m_hWnd, GWL_STYLE);// 传入程序列表list Control 的句柄，返回 样式数据/对象
	lStyle |= LVS_REPORT;   // 指定报表视图样式
	SetWindowLongPtr(LstAnly.m_hWnd, GWL_STYLE, lStyle);// 设置/应用 样式

	DWORD dStyle = LstAnly.GetExtendedStyle(); // 获得程序类别的 拓展样式数据/对象
	dStyle |= LVS_EX_FULLROWSELECT; // 选中某个项后，将突出显示该项及其所有子项。 此样式仅与 LVS_REPORT 样式结合使用。
	dStyle |= LVS_EX_GRIDLINES;     // 显示项和子项周围的 网格线。 此样式仅与 LVS_REPORT 样式结合使用。
	LstAnly.SetExtendedStyle(dStyle);// 设置/应用 样式

	//在列表的第一行的每一列插入列明
	LstAnly.InsertColumn(0, L"模块名称", 0, 100);
	LstAnly.InsertColumn(1, L"基址", 0, 200);
	LstAnly.InsertColumn(2, L"大小", 0, 200);
	LstAnly.InsertColumn(3, L"文件位置", 0, 200);
	LstAnly.InsertColumn(4, L"状态", 0, 200);

	return TRUE;
}

void CWndModAnly::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, LstAnly);
}


BEGIN_MESSAGE_MAP(CWndModAnly, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CWndModAnly::OnNMCustomdrawList2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWndModAnly 消息处理程序

//重写绘制消息,实现列表框重绘
void CWndModAnly::OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLVCUSTOMDRAW* lp = (NMLVCUSTOMDRAW*)pNMHDR;//NMLVCUSTOMDRAW结构体中，包含 列表控件发送的信息
	auto& cmd = lp->nmcd;  //nmcd就是存储列表控件信息的变量，返回值类型由当前绘图阶段dwDrawStage决定，所以要用auto自动判断类型
	switch (cmd.dwDrawStage)
	{
	case CDDS_PREPAINT://如果绘图阶段是在  绘画周期开始之前。
		*pResult = CDRF_NOTIFYITEMDRAW;//控件将通知父项相关的任何绘图操作
		break;
	case CDDS_ITEMPREPAINT://如果绘图阶段是在  绘制项之前。
	{
		DWORD dIndex = LstAnly.GetItemData(cmd.dwItemSpec);//dwItemSpec是项编号。即获得为目标项设置的属性值（0为缺少，1为增多，2为正常）
		COLORREF color[3]{ RGB(0,0,255),RGB(255,0,0),RGB(0,0,0) };//缺少是蓝色，增多是红色，正常是黑色
		lp->clrText = color[dIndex];//根据目标项的属性值(属性值标识模块状态)，设置不同颜色
	}
	*pResult = CDRF_DODEFAULT;//控件将自行绘制
	break;
	}

}


//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
  //这两个函数定义在基类中，而基类默认截取了ESC和回车等键的消息，然后映射到OK或cancer函数上，从而出现按键退出现象
void CWndModAnly::OnOK()
{
}

void CWndModAnly::OnCancel()
{
}



//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
void CWndModAnly::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();
	CDialogEx::OnCancel();//直接调用基类的OnCancel函数，关闭对话框并返回IDCANCEL
	//因为OnClose最后会间接调用到OnCancel或OnOK，而我们已经重载了，所以会优先调用我们的，但我们重载的
   //函数没有代码，即调用我们的函数不能正常关闭窗口
  //所以我们要直接调用基类的OnCancel来关闭窗口

}