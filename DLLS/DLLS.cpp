// DLLS.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "DLLS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CDLLSApp

BEGIN_MESSAGE_MAP(CDLLSApp, CWinApp)
END_MESSAGE_MAP()


// CDLLSApp 构造

CDLLSApp::CDLLSApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CDLLSApp 对象  【整个程序的入口类】

CDLLSApp theApp;
CDLLSApp* PtheApp; //将创建的对象地址，存储在PtheApp指针中，便于后续使用对象

// CDLLSApp 初始化

HHOOK keyHook; //键盘钩子，用以监控键盘的输入
LRESULT CALLBACK KeyCallBack(int nCode, WPARAM w, LPARAM l);//键盘事件触发时，会调用该函数进行处理

BOOL CDLLSApp::InitInstance()
{
	CWinApp::InitInstance();

	PtheApp = this;//PtheApp接收当前对象地址
	keyHook = SetWindowsHook(WH_KEYBOARD, KeyCallBack);// SetWindowsHook 函数来安装钩子，
	//该函数的第一个参数指定钩子类型，第二个参数是钩子过程的回调函数。
	//回调函数是由操作系统调用的函数，用于处理钩子事件。在键盘钩子中，回调函数的作用是在键盘事件发生时进行处理，例如记录按键信息或者阻止某些按键事件。
	AfxMessageBox(L"OK");
	return TRUE;
}

//按下Home键时，创建并展示DLL窗口
LRESULT CALLBACK KeyCallBack(int nCode, WPARAM w, LPARAM l)
{
	if ((l & (l << 31)) == 0)// 判断参数 l 是否是正数
	{
		switch (w)
		{
		case VK_HOME:   //如果按键消息中，w参数触发的按键为Home键
			//1.如果还没创建DLL窗口，就先创建
			if (PtheApp->wndMain == NULL) 
			{
				PtheApp->wndMain = new CWndMain();//实例化对象
				PtheApp->wndMain->Create(IDD_WNDMAIN);//创建窗口
			}
			//2.展示DLL窗口
			PtheApp->wndMain->ShowWindow(TRUE);
		}
	}
	return CallNextHookEx(keyHook, nCode, w, l);
	//将钩子事件传递给下一个钩子或者默认的钩子过程。
	//这是因为，当系统中安装了多个钩子时，它们是以链表的形式进行连接的
}