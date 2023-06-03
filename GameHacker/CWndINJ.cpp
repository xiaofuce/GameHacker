// CWndINJ.cpp: 实现文件
//

#include "pch.h"
#include "GameHacker.h"
#include "afxdialogex.h"
#include "CWndINJ.h"

// CWndINJ 对话框

IMPLEMENT_DYNAMIC(CWndINJ, CDialogEx)

CWndINJ::CWndINJ(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_0, pParent)
	, B_INJECT(FALSE)
	, B_PAUSE(FALSE)
	, B_DEBUG(FALSE)
	//, B_ADDGAME(FALSE)
{

}

CWndINJ::~CWndINJ()
{
}

BOOL CWndINJ::OnInitDialog()
{
	CDialogEx::OnInitDialog();// 使用父类的基本初始化功能

	///*
	//代码实现，我们的功能
	//*/
	//调列表样式
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ExeLst.m_hWnd, GWL_STYLE);// 传入程序列表list Control 的句柄，返回 样式数据/对象
	lStyle |= LVS_REPORT;   // 指定报表视图样式
	SetWindowLongPtr(ExeLst.m_hWnd, GWL_STYLE, lStyle);// 设置/应用 样式

	DWORD dStyle = ExeLst.GetExtendedStyle(); // 获得程序类别的 拓展样式数据/对象
	dStyle |= LVS_EX_FULLROWSELECT; // 选中某个项后，将突出显示该项及其所有子项。 此样式仅与 LVS_REPORT 样式结合使用。
	dStyle |= LVS_EX_GRIDLINES;     // 显示项和子项周围的 网格线。 此样式仅与 LVS_REPORT 样式结合使用。
	ExeLst.SetExtendedStyle(dStyle);// 设置/应用 样式

	ExeLst.InsertColumn(0, L"游戏名", 0, 200);
	ExeLst.InsertColumn(1, L"可执行文件", 0, 400);
	ExeLst.InsertColumn(2, L"文件夹", 0, 400);
	ExeLst.InsertColumn(3, L"命令行参数", 0, 400);
	ExeLst.InsertColumn(4, L"注入模块", 0, 400);

	return TRUE;
}

void CWndINJ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ExeLst);
	DDX_Check(pDX, IDC_CHECK2, B_INJECT);
	DDX_Check(pDX, IDC_CHECK3, B_PAUSE);
	DDX_Check(pDX, IDC_CHECK4, B_DEBUG);
	DDX_Control(pDX, IDC_BUTTON5, B_ADDGAME);
}


BEGIN_MESSAGE_MAP(CWndINJ, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CWndINJ::OnBnClickedButton5)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndINJ::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CWndINJ::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CWndINJ::OnMenuSet)
	ON_COMMAND(ID_32772, &CWndINJ::OnMenuDel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWndINJ 消息处理程序

//“添加游戏”的消息处理函数
void CWndINJ::OnBnClickedButton5()
{
	//弹窗，以选择要添加的游戏和要注入的模块等
	wndAddGame.Init(this);//传入父控件指针，以能在后面使用父控件的变量-配置文件路径，保存数据
	wndAddGame.DoModal();

}



//获得文件夹路径（在主对话框中执行）
void CWndINJ::GiveAppPath(CString& _wAppPath)
{
	// 获得文件夹路径
	AppPath = _wAppPath;
	GamesIni.Format(L"%s\\Setting\\AddGames.ini",AppPath); // 像sprintf一样，设置字符串格式

	// 从主对话框获得文件夹路径的同时，顺便将配置文件中的游戏信息 添加到程序列表
	LoadGamesImfo();
}

//保存“添加游戏”后所获得的信息到 配置文件 中去【即写文件】
void CWndINJ::AddGames(CString& _GameName, CString& _GamePath, CString& _GameFullPath, CString& _GameCmds, CString& _DLLPath)
{
	//1.打开INI文件（此函数会返回整数值）
	int count=GetPrivateProfileIntW(L"main",L"count",0, GamesIni);//配置文件中的节名  键名  找不到键名时的返回值 文件路径
	   //找到了指定的键名，就会返回对应的值


	//2.将游戏的信息写入INI文件
	 //为每一条加入的记录，设置节名
	CString keyName;
	keyName.Format(L"count_%d", count);
	 //开始写入
	WritePrivateProfileStringW(keyName, L"GameName", _GameName, GamesIni);//节名 键名 键值 配置文件路径 
	WritePrivateProfileStringW(keyName, L"GamePath", _GamePath, GamesIni);
	WritePrivateProfileStringW(keyName, L"GameFullPath", _GameFullPath, GamesIni); 
	WritePrivateProfileStringW(keyName, L"GameCmds", _GameCmds, GamesIni);
	WritePrivateProfileStringW(keyName, L"DLLPath", _DLLPath, GamesIni);


	//3.更新节“main”中、记录条数的键“count”的数值（只要执行了本函数，就说明已经添加了一个游戏）
	count++;//键"count"是用来记录配置文件内所添加的游戏记录条数的（初始值为0）。只要添加了1条，count值就++
	CString wCount;
	wCount.Format(L"%d", count);
	WritePrivateProfileStringW(L"main", L"count", wCount, GamesIni);

	//4.立即将添加的游戏信息同时加载到程序列表里
	int icount = ExeLst.GetItemCount();//获得程序列表的条目数，以将信息插入最后一行
	ExeLst.InsertItem(icount, _GameName);
	ExeLst.SetItemText(icount, 1, _GameFullPath);
	ExeLst.SetItemText(icount, 2, _GamePath);
	ExeLst.SetItemText(icount, 3, _GameCmds);
	ExeLst.SetItemText(icount, 4, _DLLPath);
}

//将配置文件中的游戏信息，加载到程序列表
void CWndINJ::LoadGamesImfo()
{
	//1.打开INI文件（此函数会返回整数值）
	int count = GetPrivateProfileIntW(L"main", L"count", 0, GamesIni);//配置文件中的节名  键名  找不到键名时的返回值 文
	//2.读取数据，并写入程序列表中
	for (int i = 0;i < count;i++)//如果count返回0，即没有游戏配置信息，则不会继续执行
	{
		// 临时变量，暂时接收INI文件中的数据
		CString _GameName, _GamePath, _GameFullPath, _GameCmds, _DLLPath, _AppName;
		_AppName.Format(L"count_%d", i);//这里构造INI文件中的节名，因为检索数据要用到

		// 从INI文件中,检索/获取指定节的字符串
		wchar_t wRead[0xFF];
		GetPrivateProfileStringW(_AppName,L"GameName", NULL, wRead, 0xFF, GamesIni);//节名 键名 找不到时的返回值 存储返回值的指针 指针大小 文件名
		_GameName.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GamePath", NULL, wRead, 0xFF, GamesIni);
		_GamePath.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GameFullPath", NULL, wRead, 0xFF, GamesIni);
		_GameFullPath.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GameCmds", NULL, wRead, 0xFF, GamesIni);
		_GameCmds.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"DLLPath", NULL, wRead, 0xFF, GamesIni);
		_DLLPath.Format(L"%s", wRead);

		// 数据加载进程序列表
		ExeLst.InsertItem(i, _GameName);
		ExeLst.SetItemText(i, 1, _GameFullPath);
		ExeLst.SetItemText(i, 2, _GamePath);
		ExeLst.SetItemText(i, 3, _GameCmds);
		ExeLst.SetItemText(i, 4, _DLLPath);
	}

}

//双击鼠标左键，启动游戏
void CWndINJ::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR); //确认消息句柄
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//选中了列表项，才执行启动游戏的功能
	int index = pNMItemActivate->iItem; //获得列表视图项的索引(行号)。如果项目索引不用于通知（没选中），则返回-1。
	if (index < 0)return;//没选择，就不执行后续代码

	// 先获取必要信息，以用于创建进程
	CString GameExe = ExeLst.GetItemText(index, 1);//完整程序文件路径(exe文件路径)
	CString GamePath = ExeLst.GetItemText(index,2);		//程序所在文件夹路径
	CString GameCmds = ExeLst.GetItemText(index,3);		//程序的命令行参数
	CString GameDLLs = ExeLst.GetItemText(index,4);		//注入模块的路径

	//将选中的按钮的值先传递给程序，用以判断是否开启相应功能
	UpdateData(TRUE);

	//1.启动游戏进程
		//a.判断是否点击了“暂停”按钮
	bool pause = B_PAUSE;
	if ((B_PAUSE) && (GameDLLs.GetLength() > 1))//光暂停，不设置好dll，程序不会暂停的
	{
		AfxMessageBox(L"已暂停游戏进程！");
		pause = true;
	}
		//b.启动进程
	PROCESS_INFORMATION prinfo{};//存储进程的各种信息
	m_INJECT.StartProcess(GameExe, GamePath, GameCmds.GetBuffer(), &prinfo,pause);//游戏进程会根据pause判断是否暂停

	//2.判断是否进行注入  L"D:\\Git\\GameHacker\\GameHacker\\Debug\\DLLS.dll"
	if ((B_INJECT) && (GameDLLs.GetLength() > 1)) //如果选中注入，并设置好了dll路径，就会进行注入
	{
		AfxMessageBox(L"开始注入！");
		m_INJECT.CreateRemoteData(prinfo.hProcess, GameExe.GetBuffer(), GameDLLs);
	}
	

	//3.判断是否进入调试模式（启动dbg进程）
	if (B_DEBUG)
	{
		PROCESS_INFORMATION odinfo{};//存储dbg进程的各种信息
		//设置dbg参数
		CString dbgExe, dbgPath, dbgCmds;
		dbgExe = L"C:\\Users\\Administrator\\Desktop\\编程\\逆向工具\\OllyDbg\\Ollydbg.exe";//od的完整路径
		dbgPath = L"C:\\Users\\Administrator\\Desktop\\编程\\逆向工具\\OllyDbg\\";//od的文件夹路径
		dbgCmds.Format(L"%s -p %d", dbgExe, prinfo.dwProcessId); //拼接命令行参数（需要进程id，od才能打开特定进程以进行调试）
		//启动dbg
		AfxMessageBox(L"开始调试！");
		m_INJECT.StartProcess(dbgExe, dbgPath, dbgCmds.GetBuffer(), &odinfo,false);//false是为了直接启动OD进程、不暂停OD进程
	}

	
	 //4.进行完以上操作、且是取消了暂停的，才会恢复游戏的主线程运行
	ResumeThread(prinfo.hThread);
	//UpdateData(TRUE);
	//if(!B_PAUSE)  ResumeThread(prinfo.hThread);
}



//右键时，打开菜单
void CWndINJ::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pActive = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//判断是否选中了右键菜单的项
	if (pActive->iItem != -1)// ！=-1，就说明选中了列表
	{
		lstSel = pActive->iItem; //pActive->iItem是NMITEMACTIVATE结构体中的一个成员变量，表示被激活的项在列表控件中的索引。通过这个索引，程序可以得知哪个项被激活了，从而进行相应的处理。
		DWORD dwPos = GetMessagePos();//获得上一个消息发生的时候(即右击列表时)，光标所在的坐标;若要确定光标的当前位置，请使用 GetCursorPos 函数。
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));//通过宏函数，获得DWORD变量（即光标的坐标）的低位（X）和高位（Y），并作为一个无符号短整型（WORD）返回。
		
		CMenu menu;
		menu.LoadMenuW(IDR_MENU1);//加载 添加的 菜单资源(本质上是一个数组）
		CMenu* pop = menu.GetSubMenu(0);//获得其下的第一个子菜单（即是我们编辑的右键菜单）
		pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);//将获得的子菜单弹出来（设置弹出的方式、样子和位置）
	}
}

//设置目标项
void CWndINJ::OnMenuSet()
{
	// TODO: 在此添加命令处理程序代码

	//重新设置数据
	CString GameName = ExeLst.GetItemText(lstSel, 0);//获得程序名
	CString GameExe = ExeLst.GetItemText(lstSel, 1);//完整程序文件路径(exe文件路径)
	CString GamePath = ExeLst.GetItemText(lstSel, 2);		//程序所在文件夹路径
	CString GameCmds = ExeLst.GetItemText(lstSel, 3);		//程序的命令行参数
	CString GameDLLs = ExeLst.GetItemText(lstSel, 4);		//注入模块的路径

	//重新初始化AddGame窗体的信息 （函数在INJECT窗体执行的）
	wndAddGame.Init(this, false, lstSel);
	//将重新设置的数据传入AddGame窗体 （函数在INJECT窗体执行的）
	wndAddGame.SetData(GameName, GameExe, GamePath, GameCmds, GameDLLs);
	//打开AddGame窗口，然后让用户在该窗口中进行操作，直到用户关闭对话框窗口为止；而在对话框被关闭之前，程序会一直停留在dlg.DoModal()这一行代码处，不会执行后续的代码。
	wndAddGame.DoModal();
}

//删除目标项
void CWndINJ::OnMenuDel()
{
	// TODO: 在此添加命令处理程序代码
	// 
	//删除列表上的目标项（目标项的编号在右键该项时，就已经被记录在变量lstSel中了）
	ExeLst.DeleteItem(lstSel);
	//将数据重新覆盖写一遍
	SaveLstToIni();
}

//删除列表项后，将列表上的游戏数据 重新覆盖写到配置文件中（因为删除目标项后，小工具一个个改其他项的数据太麻烦，重写一次数据性价比最高）
void CWndINJ::SaveLstToIni()
{
	int count = ExeLst.GetItemCount();//获得程序列表的条目数，以
	for (int i = 0; i < count; i++) 
	{
		//1.获得删除目标项后的列表项目数据
		CString GameName = ExeLst.GetItemText(i, 0);//获得程序名
		CString GameExe  = ExeLst.GetItemText(i, 1);//完整程序文件路径(exe文件路径)
		CString GamePath = ExeLst.GetItemText(i, 2);		//程序所在文件夹路径
		CString GameCmds = ExeLst.GetItemText(i, 3);		//程序的命令行参数
		CString GameDLLs = ExeLst.GetItemText(i, 4);		//注入模块的路径

		//2.将游戏的信息写入INI文件
			//为每一条记录，设置节名
		CString keyName;
		keyName.Format(L"count_%d", count);
		WritePrivateProfileStringW(keyName, L"GameName", GameName, GamesIni);//节名 键名 键值 配置文件路径 
		WritePrivateProfileStringW(keyName, L"GamePath", GameExe, GamesIni);
		WritePrivateProfileStringW(keyName, L"GameFullPath", GamePath, GamesIni);
		WritePrivateProfileStringW(keyName, L"GameCmds", GameCmds, GamesIni);
		WritePrivateProfileStringW(keyName, L"DLLPath", GameDLLs, GamesIni);
	}
	
	//3.最后再更新 节“main”中，记录条数的键“count”的数值
	CString wCount;
	wCount.Format(L"%d", count);
	WritePrivateProfileStringW(L"main", L"count", wCount, GamesIni);
}

//保存要重新“设置”的目标项 的数据
void CWndINJ::SaveSetting(CString& _GameName, CString& _GamePath, CString& _GameFullPath, CString& _GameCmds, CString& _DLLPath, int _index)
{
	CString key;
	key.Format(L"count_%d", _index);//获取目标项的节名

	// 在原来的条目上，重新写入目标项的数据
	WritePrivateProfileStringW(key, L"GameName", _GameName, GamesIni);//节名 键名 键值 配置文件路径 
	WritePrivateProfileStringW(key, L"GamePath", _GamePath, GamesIni);
	WritePrivateProfileStringW(key, L"GameFullPath", _GameFullPath, GamesIni);
	WritePrivateProfileStringW(key, L"GameCmds", _GameCmds, GamesIni);
	WritePrivateProfileStringW(key, L"DLLPath", _DLLPath, GamesIni);

	
	// 将重新设置的目标项数据，加载进程序列表 显示出来
	ExeLst.SetItemText(_index, 0,_GameName);
	ExeLst.SetItemText(_index, 1, _GameFullPath);
	ExeLst.SetItemText(_index, 2, _GamePath);
	ExeLst.SetItemText(_index, 3, _GameCmds);
	ExeLst.SetItemText(_index, 4, _DLLPath);
}

//重载OnOK和Cancer的函数，以能解决“ESC和回车导致窗口关闭”的问题
  //这两个函数定义在基类中，而基类默认截取了ESC和回车等键的消息，然后映射到OK或cancer函数上，从而出现按键退出现象
void CWndINJ::OnOK()
{
}

void CWndINJ::OnCancel()
{
}



//重载关闭窗口的函数，以能在解决ESC和回车导致窗口关闭的问题之后，正常关闭窗口
void CWndINJ::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();
	CDialogEx::OnCancel();//直接调用基类的OnCancel函数，关闭对话框并返回IDCANCEL
	//因为OnClose最后会间接调用到OnCancel或OnOK，而我们已经重载了，所以会优先调用我们的，但我们重载的
   //函数没有代码，即调用我们的函数不能正常关闭窗口
   //所以我们要直接调用基类的OnCancel来关闭窗口

}