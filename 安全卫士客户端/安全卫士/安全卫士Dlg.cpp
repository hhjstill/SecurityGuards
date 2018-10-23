
// 安全卫士Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "安全卫士Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C安全卫士Dlg 对话框



C安全卫士Dlg::C安全卫士Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C安全卫士Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabWin);
}

BEGIN_MESSAGE_MAP(C安全卫士Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_RBUTTONUP()
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C安全卫士Dlg::OnTcnSelchangeTab1)
ON_WM_TIMER()
ON_COMMAND(ID_32781, &C安全卫士Dlg::OnLock)
ON_COMMAND(ID_32779, &C安全卫士Dlg::OnRestart)
ON_COMMAND(ID_32778, &C安全卫士Dlg::OnShutDown)
ON_COMMAND(ID_32780, &C安全卫士Dlg::OnLogOff)
END_MESSAGE_MAP()


// C安全卫士Dlg 消息处理程序

BOOL C安全卫士Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	BOOL bb = RegisterHotKey(GetSafeHwnd(), 0x6789, MOD_CONTROL, 'Q');
	m_menu.LoadMenu(IDR_MENU3);
	SetMenu(&m_menu);
	m_statusBar.Create(this);
	UINT idicator[] = { 0x4444, 0x5555 };
	m_statusBar.SetIndicators(idicator, _countof(idicator));
	CRect rect;
	GetWindowRect(rect);
	m_statusBar.SetPaneInfo(0, idicator[0], SBPS_NORMAL, rect.Width() / 3);
	m_statusBar.SetPaneInfo(1, idicator[1], SBPS_NORMAL, rect.Width() / 3);


	MEMORYSTATUS ms{};
	GlobalMemoryStatus(&ms);
	//内存占用率
	DWORD dwLoad = ms.dwMemoryLoad;
	CString str;
	str.Format(L"%d", dwLoad);
	//MessageBox(L"当前内存占用率为:" + str + L"%", L"提示", 0);
	m_statusBar.SetPaneText(0, L"内存:  " + str + L"%");


	int cpuLoad = getCpuLoad();
	str.Format(L"%d", cpuLoad);
	//MessageBox(L"当前CPU占用率为:" + str + L"%", L"提示", 0);
	m_statusBar.SetPaneText(1, L"CPU:  " + str + L"%");
	


	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);
	SetTimer(1, 20000, NULL);
	m_tabWin.InsertItem(0, L"进程");
	m_tabWin.InsertItem(1, L"窗口");
	m_tabWin.InsertItem(2, L"PE");
	m_tabWin.InsertItem(3, L"垃圾清理");
	m_tabWin.InsertItem(4, L"服务");
	m_tabWin.InsertItem(5, L"软件");
	m_pageProcess.Create(IDD_pageprocess,&m_tabWin);
	m_pageWnd.Create(IDD_PAGEWND, &m_tabWin);
	m_pagePE.Create(IDD_pagePE, &m_tabWin);
	m_pageClean.Create(IDD_DIALOGCLEAN, &m_tabWin);
	m_pageSvc.Create(IDD_DIALOGSERVICE, &m_tabWin);
	m_pageStart.Create(IDD_DIALOGSTATR, &m_tabWin);
	//CRect rect;
	m_tabWin.GetClientRect(&rect);
	rect.top += 25;
	//rect.bottom -= 4;
	//rect.left += 10;
	//rect.right -= 4;
	m_pageProcess.MoveWindow(&rect);
	m_pageWnd.MoveWindow(&rect);
	m_pagePE.MoveWindow(&rect);
	m_pageClean.MoveWindow(&rect);
	m_pageSvc.MoveWindow(&rect);
	m_pageStart.MoveWindow(&rect);
	m_pageProcess.ShowWindow(TRUE);
	m_tabWin.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C安全卫士Dlg::OnPaint()
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
HCURSOR C安全卫士Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void C安全卫士Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_tabWin.GetCurSel())
	{
	case 0:
		m_pageProcess.ShowWindow(SW_SHOW);
		m_pagePE.ShowWindow(SW_HIDE);
		m_pageWnd.ShowWindow(SW_HIDE);
		m_pageClean.ShowWindow(SW_HIDE);
		m_pageSvc.ShowWindow(SW_HIDE);
		m_pageStart.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pageProcess.ShowWindow(SW_HIDE);
		m_pageWnd.ShowWindow(SW_SHOW);
		m_pagePE.ShowWindow(SW_HIDE);
		m_pageClean.ShowWindow(SW_HIDE);
		m_pageSvc.ShowWindow(SW_HIDE);
		m_pageStart.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pageProcess.ShowWindow(SW_HIDE);
		m_pageWnd.ShowWindow(SW_HIDE);
		m_pagePE.ShowWindow(SW_SHOW);
		m_pageClean.ShowWindow(SW_HIDE);
		m_pageSvc.ShowWindow(SW_HIDE);
		m_pageStart.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pageProcess.ShowWindow(SW_HIDE);
		m_pageWnd.ShowWindow(SW_HIDE);
		m_pagePE.ShowWindow(SW_HIDE);
		m_pageClean.ShowWindow(SW_SHOW);
		m_pageSvc.ShowWindow(SW_HIDE);
		m_pageStart.ShowWindow(SW_HIDE);
		break;
	case 4:
		m_pageProcess.ShowWindow(SW_HIDE);
		m_pageWnd.ShowWindow(SW_HIDE);
		m_pagePE.ShowWindow(SW_HIDE);
		m_pageClean.ShowWindow(SW_HIDE);
		m_pageSvc.ShowWindow(SW_SHOW);
		m_pageStart.ShowWindow(SW_HIDE);
		break;
	case 5:
		m_pageProcess.ShowWindow(SW_HIDE);
		m_pageWnd.ShowWindow(SW_HIDE);
		m_pagePE.ShowWindow(SW_HIDE);
		m_pageClean.ShowWindow(SW_HIDE);
		m_pageSvc.ShowWindow(SW_HIDE);
		m_pageStart.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
}


void C安全卫士Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MEMORYSTATUS ms{};
	GlobalMemoryStatus(&ms);
	//内存占用率
	DWORD dwLoad = ms.dwMemoryLoad;
	CString str;
	str.Format(L"%d", dwLoad);
	//MessageBox(L"当前内存占用率为:" + str + L"%", L"提示", 0);
	m_statusBar.SetPaneText(0, L"内存:  " + str + L"%");
	int cpuLoad = getCpuLoad();
	str.Format(L"%d", cpuLoad);
	//MessageBox(L"当前CPU占用率为:" + str + L"%", L"提示", 0);
	m_statusBar.SetPaneText(1, L"CPU:  " + str + L"%");
	CDialogEx::OnTimer(nIDEvent);
}
int C安全卫士Dlg::getCpuLoad()
{
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(0, FALSE, FALSE, 0);
	WaitForSingleObject(hEvent, 100);
	CloseHandle(hEvent);
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	double idle_Time = FILETME2Double(idleTime);
	double new_IdleTime = FILETME2Double(newIdleTime);
	double kernel_Time = FILETME2Double(kernelTime);
	double new_KernelTime = FILETME2Double(newKernelTime);
	double user_Time = FILETME2Double(userTime);
	double new_UserTime = FILETME2Double(newUserTime);

	return int(100.0 - (new_IdleTime - idle_Time) / (new_KernelTime + new_UserTime - kernel_Time - user_Time) * 100);
}
double C安全卫士Dlg::FILETME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime * 4.294967296e9) + double(fileTime.dwLowDateTime);
}
//void C任务管理器Dlg::OnBnClickedButton8()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	MEMORYSTATUS ms{};
//	GlobalMemoryStatus(&ms);
//	//内存占用率
//	DWORD dwLoad = ms.dwMemoryLoad;
//	CString str;
//	str.Format(L"%d", dwLoad);
//	MessageBox(L"当前内存占用率为:" + str + L"%", L"提示", 0);
//}
//
//
//void C任务管理器Dlg::OnBnClickedButton9()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	int cpuLoad = getCpuLoad();
//	CString str;
//	str.Format(L"%d", cpuLoad);
//	MessageBox(L"当前CPU占用率为:" + str + L"%", L"提示", 0);
//}
//
//double C任务管理器Dlg::FILETME2Double(const _FILETIME& fileTime)
//{
//	return double(fileTime.dwHighDateTime * 4.294967296e9) + double(fileTime.dwLowDateTime);
//}
//
//int C任务管理器Dlg::getCpuLoad()
//{
//	_FILETIME idleTime, kernelTime, userTime;
//	GetSystemTimes(&idleTime, &kernelTime, &userTime);
//	HANDLE hEvent = CreateEvent(0, FALSE, FALSE, 0);
//	WaitForSingleObject(hEvent, 1000);
//	CloseHandle(hEvent);
//	_FILETIME newIdleTime, newKernelTime, newUserTime;
//	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
//	double idle_Time = FILETME2Double(idleTime);
//	double new_IdleTime = FILETME2Double(newIdleTime);
//	double kernel_Time = FILETME2Double(kernelTime);
//	double new_KernelTime = FILETME2Double(newKernelTime);
//	double user_Time = FILETME2Double(userTime);
//	double new_UserTime = FILETME2Double(newUserTime);
//
//	return int(100.0 - (new_IdleTime - idle_Time) / (new_KernelTime + new_UserTime - kernel_Time - user_Time) * 100);
//}

void C安全卫士Dlg::OnLock()
{
	// TODO: 在此添加命令处理程序代码
	adjustPrivilege();
	LockWorkStation();
}
void C安全卫士Dlg::adjustPrivilege()
{
	HANDLE hToken = NULL;
	HANDLE hPro = GetCurrentProcess();
	OpenProcessToken(hPro, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp{};
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}



void C安全卫士Dlg::OnRestart()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	adjustPrivilege();
	int nRes = MessageBox(L"确定要重启计算机?", L"警告", MB_YESNO);
	if (IDNO == nRes)
		return;
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}


void C安全卫士Dlg::OnShutDown()
{
	// TODO: 在此添加命令处理程序代码
	int nRes = MessageBox(L"确定要关闭计算机?", L"警告", MB_YESNO);
	if (IDNO == nRes)
		return;
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
}


void C安全卫士Dlg::OnLogOff()
{
	// TODO: 在此添加命令处理程序代码
	adjustPrivilege();
	int nRes = MessageBox(L"确定要注销吗?", L"警告", MB_YESNO);
	if (IDNO == nRes)
		return;
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
}


BOOL C安全卫士Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_HOTKEY && pMsg->wParam == 0x6789)
	{
		if (m_isShow == TRUE)
		{
			ShowWindow(SW_HIDE);
			m_isShow = FALSE;
		}
		else
		{
			ShowWindow(SW_SHOW);
			m_isShow = TRUE;
		}
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}
