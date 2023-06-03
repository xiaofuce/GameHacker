// CWndINC.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndINC.h"


// CWndINC 对话框

IMPLEMENT_DYNAMIC(CWndINC, CDialogEx)

CWndINC::CWndINC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_1, pParent)
	, UExe(_T(""))
	, UExeRoot(_T(""))
{

}

CWndINC::~CWndINC()
{
}

BOOL CWndINC::OnInitDialog()
{
	CDialogEx::OnInitDialog();// 使用父类的基本初始化功能

	////改变Tab的栏名称 （不用了，因为代码整合到了 “安装选项卡” 里）
	//mTab.InsertItem(0, L"正常进程");
	//mTab.InsertItem(1, L"感染进程");
	//mTab.InsertItem(2, L"分析结果");

	//安装选项卡（即让INC的Tab能显示窗口内容）
	InstallPage(&modPage[0], IDD_MOD_LIST, L"正常进程", TRUE);
	InstallPage(&modPage[1], IDD_MOD_LIST, L"感染进程", TRUE);
	InstallPage(&modAnly, IDD_MOD_ANLY, L"分析对比", TRUE);


	return TRUE;
}

void CWndINC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UExe);
	DDX_Text(pDX, IDC_EDIT2, UExeRoot);
	DDX_Control(pDX, IDC_TAB1, mTab);
}


BEGIN_MESSAGE_MAP(CWndINC, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndINC::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CWndINC::OnBnClickedButton7)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWndINC::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON3, &CWndINC::OnBnClickedButton3)
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CWndINC 消息处理程序

// 编写“打开进程” 的按钮的函数
void CWndINC::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//wndProcess.Init(this);//INJECTCHEK窗体中执行，会把当前窗体的指针，传给 进程列表的窗体中的 实例化的INJECTCHEK窗体类对象 wndINC 
	if (wndProcess.DoModal() == IDOK)
	{
		UExe = wndProcess.wExe;//读取的是正常进程(第一个)的数据
		modPage[0].GetModList(wndProcess.dPid); //INC窗体，在正常进程的页面，显示双击的进程的模块信息
		UpdateData(FALSE);//控制变量向控件传递数据
	}
}

// 编写“打开感染进程” 的按钮的函数
void CWndINC::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	//wndProcess.Init(this,FALSE);//FALSE表示要读取(SetProcess)的是"感染进程"的数据
	if (wndProcess.DoModal() == IDOK)
	{
		UExe = wndProcess.wExe;//读取的是正常进程(第一个)的数据
		modPage[1].GetModList(wndProcess.dPid); //INC窗体，在正常进程的页面，显示双击的进程的模块信息
		UpdateData(FALSE);//控制变量向控件传递数据
	}
}


////用于读取进程列表窗口中的数据
//void CWndINC::SetProcess(unsigned Pid, CString& wExe, BOOL bFirst)//传入的是选中的进程的PID
//{
//	//判断读取的是哪一个进程的数据
//	if (bFirst)
//	{
//		UExe = wExe;//读取的是正常进程(第一个)的数据
//		modPage[0].GetModList(Pid); //INC窗体，在正常进程的页面，显示双击的进程的模块信息
//	}
//	else 
//	{
//		UExeRoot = wExe;//读取的是感染进程的数据
//		modPage[1].GetModList(Pid);//INC窗体，在感染进程的页面，显示双击的进程的模块信息
//	}
//
//	//向窗口传递读取到的进程数据
//	UpdateData(FALSE);
//}


//实现Tab控件的页面切换 功能
void CWndINC::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int n = mTab.GetCurSel(); //用于取得组合框中当前选中的项的下标,即获得当前选中的选项卡的下标
	for (int i = 0; i < CurPage; i++)
	{
		Pages[i]->ShowWindow(i == n);
	}
}

//安装子选项卡（要进行绑定的类对象+要绑定的子对话框+设置选项卡名字+是否显示选项卡内容）
bool CWndINC::InstallPage(CDialogEx* _Wnd, int IDD_WND, CString&& _Name, BOOL IsShow)
{
	if (CurPage >= MAX_PAGE) return false;
	Pages[CurPage] = _Wnd;               //将当前 数据显示窗口 的类对象，加入到管理选项卡的指针数组里去
	Pages[CurPage]->Create(IDD_WND);      //让 数据显示窗口的类对象，和子窗口  绑定。 有Parent参数，以指明显示在哪里上，不过我们用下面的那个函数
	Pages[CurPage]->SetParent(&mTab);     //设置数据显示窗口的父窗口(即承载窗口) mTab 【让子窗口在mTab控件上显示】
	Pages[CurPage]->ShowWindow(IsShow);   //显示 数据显示窗口

	CRect rect;
	mTab.GetClientRect(&rect);         //获得 选项卡控件 位于客户区的坐标，存到rect中去
	rect.top += 0;
	rect.left += 0;
	rect.right += 5;
	rect.bottom += 2;					   //不断调节选项卡控件的坐标，以和其所显示的子对话框贴合
	Pages[CurPage]->MoveWindow(&rect);  //设置子对话框的位置坐标，以贴合选项卡控件所能显示的区域
	mTab.InsertItem(CurPage, _Name);   //为选项卡设置名字

	CurPage++;                        //指向下一个选项卡的下标
	return true;
}

//让数据显示窗口（Tab控件承载着）可以随INC窗口的大小变化而变化
void CWndINC::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//nType是1时，为最小化窗口，不做处理
	if (nType == 1)return;

	//nType是其他值时，才做处理
	
	if (Pages[CurPage]) {//必须在这里判断选项卡初始化了没有，不然会报错
		CRect rect;
		mTab.GetClientRect(&rect);         //获得 选项卡控件 位于客户区的坐标，存到rect中去
		rect.top += 0;
		rect.left += 0;
		rect.right -= 3;
		rect.bottom -= 23;				   //不断调节选项卡控件的坐标，以和其所显示的子对话框贴合
		for (int i = 0; i < CurPage; i++)//每个选项卡页面都进行设置
		{
			Pages[i]->MoveWindow(&rect);  //如果当前数组下标的页面有指针，即页面初始化了，则为true
		}
	}
}

//“分析”按钮的代码
void CWndINC::OnBnClickedButton3()
{
	//1.开始对比
	int CountNormal = modPage[0].ListMoule.GetItemCount(); //获取正常进程的模块数量
	int CountRoot   = modPage[1].ListMoule.GetItemCount(); //获取感染进程的模块数量
	if ((CountNormal) && (CountRoot)) //如果正常进程和感染模块是正常显示模块信息的，才进行下面的分析
	{
		modAnly.LstAnly.DeleteAllItems();//在比对分析之前，把分析页面上的数据清空，防止堆积
		for (int i = 0; i < CountNormal; i++)
		{
			CString txt = modPage[0].ListMoule.GetItemText(i, 3);//获取正常进程的每个模块的文件路径，用它来和感染进程比较
			for (int j = 0; j < CountRoot; j++)//用正常进程的一个模块 和 感染进程的每个模块的文件路径 比较
			{
				if (modPage[1].ListMoule.GetItemText(j, 3).MakeUpper() == txt.MakeUpper())//这里要都转化为大写再比较，因为win大小写不分，但我们的比对是分的，都都统一后就好比了，也防止模块重复
				{
					modPage[1].ListMoule.SetItemData(j, 1);//如果对得上，即正常进程的模块，感染进程也有
					modPage[0].ListMoule.SetItemData(i, 1);//则两个页面的两个比较的模块的属性值设为1（正常）
					break; //完成当前模块的比较
				}
			}
		}

		//2.完成对比，开始在 分析窗口的页面上的“状态”处，设置模块的情况
		CString wTxt[2]{ L"减少",L"正常" };  //用于在分析页面显示相应文字
		DWORD  dState[2]{ 0,2 };          //减少为0，正常为2，1是增多。便于后面对不同模块情况选择突出的颜色
		  //a.将正常进程中，正常的模块 和 减少的模块 的信息，设置到 “分析比较”页面中以显示
		for (int i = 0; i < CountNormal; i++)
		{
			//获得当前模块的属性值(0就是减少，1就是正常)
			DWORD state = modPage[0].ListMoule.GetItemData(i);

			//将模块信息，插入到分析窗口中去
			modAnly.LstAnly.InsertItem(0, modPage[0].ListMoule.GetItemText(i, 0));//将正常进程中，当前模块的模块名，插入到分析窗口中
			modAnly.LstAnly.SetItemText(0,1, modPage[0].ListMoule.GetItemText(i, 1));//将正常进程中，当前模块的基址，插入到分析窗口中
			modAnly.LstAnly.SetItemText(0, 2, modPage[0].ListMoule.GetItemText(i, 2));//将正常进程中，当前模块的大小，插入到分析窗口中
			modAnly.LstAnly.SetItemText(0, 3, modPage[0].ListMoule.GetItemText(i, 3));//将正常进程中，当前模块的文件位置，插入到分析窗口中
			modAnly.LstAnly.SetItemText(0, 4, wTxt[state]);//将正常进程中，当前模块的状态，插入到分析窗口中
			modAnly.LstAnly.SetItemData(0, dState[state]); //根据当前模块的属性值，在dState中选择并设置新的属性值，以便于后面的设置条目颜色
		}

		//b.将感染进程中，增加的模块的信息，设置到 “分析比较”页面中以显示
		for (int i = 0; i < CountRoot; i++)
		{
			//获得当前模块的属性值
			DWORD state = modPage[1].ListMoule.GetItemData(i);
			//如果当前模块的属性值为0，即是增加的模块，则将模块信息，插入到分析窗口中去
			if (state == 0)
			{
				modAnly.LstAnly.InsertItem(0, modPage[1].ListMoule.GetItemText(i, 0));//将正常进程中，当前模块的模块名，插入到分析窗口中
				modAnly.LstAnly.SetItemText(0, 1, modPage[1].ListMoule.GetItemText(i, 1));//将正常进程中，当前模块的基址，插入到分析窗口中
				modAnly.LstAnly.SetItemText(0, 2, modPage[1].ListMoule.GetItemText(i, 2));//将正常进程中，当前模块的大小，插入到分析窗口中
				modAnly.LstAnly.SetItemText(0, 3, modPage[1].ListMoule.GetItemText(i, 3));//将正常进程中，当前模块的文件位置，插入到分析窗口中
				modAnly.LstAnly.SetItemText(0, 4, L"增加");//将正常进程中，当前模块的状态，插入到分析窗口中
				modAnly.LstAnly.SetItemData(0, 1); //如果当前模块的属性值为0，则设置新的属性值为1（表示为“增加模块”），以便于后面的设置条目颜色

			}
			
		}
	}

}

//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
  //这两个函数定义在基类中，而基类默认截取了ESC和回车等键的消息，然后映射到OK或cancer函数上，从而出现按键退出现象
void CWndINC::OnOK()
{
}

void CWndINC::OnCancel()
{
}



//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
void CWndINC::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();
	CDialogEx::OnCancel();//直接调用基类的OnCancel函数，关闭对话框并返回IDCANCEL
	//因为OnClose最后会间接调用到OnCancel或OnOK，而我们已经重载了，所以会优先调用我们的，但我们重载的
   //函数没有代码，即调用我们的函数不能正常关闭窗口
  //所以我们要直接调用基类的OnCancel来关闭窗口

}