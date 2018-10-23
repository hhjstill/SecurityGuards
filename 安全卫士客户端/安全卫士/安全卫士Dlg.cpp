
// ��ȫ��ʿDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "��ȫ��ʿDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C��ȫ��ʿDlg �Ի���



C��ȫ��ʿDlg::C��ȫ��ʿDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C��ȫ��ʿDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabWin);
}

BEGIN_MESSAGE_MAP(C��ȫ��ʿDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_RBUTTONUP()
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C��ȫ��ʿDlg::OnTcnSelchangeTab1)
ON_WM_TIMER()
ON_COMMAND(ID_32781, &C��ȫ��ʿDlg::OnLock)
ON_COMMAND(ID_32779, &C��ȫ��ʿDlg::OnRestart)
ON_COMMAND(ID_32778, &C��ȫ��ʿDlg::OnShutDown)
ON_COMMAND(ID_32780, &C��ȫ��ʿDlg::OnLogOff)
END_MESSAGE_MAP()


// C��ȫ��ʿDlg ��Ϣ�������

BOOL C��ȫ��ʿDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	//�ڴ�ռ����
	DWORD dwLoad = ms.dwMemoryLoad;
	CString str;
	str.Format(L"%d", dwLoad);
	//MessageBox(L"��ǰ�ڴ�ռ����Ϊ:" + str + L"%", L"��ʾ", 0);
	m_statusBar.SetPaneText(0, L"�ڴ�:  " + str + L"%");


	int cpuLoad = getCpuLoad();
	str.Format(L"%d", cpuLoad);
	//MessageBox(L"��ǰCPUռ����Ϊ:" + str + L"%", L"��ʾ", 0);
	m_statusBar.SetPaneText(1, L"CPU:  " + str + L"%");
	


	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);
	SetTimer(1, 20000, NULL);
	m_tabWin.InsertItem(0, L"����");
	m_tabWin.InsertItem(1, L"����");
	m_tabWin.InsertItem(2, L"PE");
	m_tabWin.InsertItem(3, L"��������");
	m_tabWin.InsertItem(4, L"����");
	m_tabWin.InsertItem(5, L"���");
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C��ȫ��ʿDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C��ȫ��ʿDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void C��ȫ��ʿDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


void C��ȫ��ʿDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MEMORYSTATUS ms{};
	GlobalMemoryStatus(&ms);
	//�ڴ�ռ����
	DWORD dwLoad = ms.dwMemoryLoad;
	CString str;
	str.Format(L"%d", dwLoad);
	//MessageBox(L"��ǰ�ڴ�ռ����Ϊ:" + str + L"%", L"��ʾ", 0);
	m_statusBar.SetPaneText(0, L"�ڴ�:  " + str + L"%");
	int cpuLoad = getCpuLoad();
	str.Format(L"%d", cpuLoad);
	//MessageBox(L"��ǰCPUռ����Ϊ:" + str + L"%", L"��ʾ", 0);
	m_statusBar.SetPaneText(1, L"CPU:  " + str + L"%");
	CDialogEx::OnTimer(nIDEvent);
}
int C��ȫ��ʿDlg::getCpuLoad()
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
double C��ȫ��ʿDlg::FILETME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime * 4.294967296e9) + double(fileTime.dwLowDateTime);
}
//void C���������Dlg::OnBnClickedButton8()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	MEMORYSTATUS ms{};
//	GlobalMemoryStatus(&ms);
//	//�ڴ�ռ����
//	DWORD dwLoad = ms.dwMemoryLoad;
//	CString str;
//	str.Format(L"%d", dwLoad);
//	MessageBox(L"��ǰ�ڴ�ռ����Ϊ:" + str + L"%", L"��ʾ", 0);
//}
//
//
//void C���������Dlg::OnBnClickedButton9()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	int cpuLoad = getCpuLoad();
//	CString str;
//	str.Format(L"%d", cpuLoad);
//	MessageBox(L"��ǰCPUռ����Ϊ:" + str + L"%", L"��ʾ", 0);
//}
//
//double C���������Dlg::FILETME2Double(const _FILETIME& fileTime)
//{
//	return double(fileTime.dwHighDateTime * 4.294967296e9) + double(fileTime.dwLowDateTime);
//}
//
//int C���������Dlg::getCpuLoad()
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

void C��ȫ��ʿDlg::OnLock()
{
	// TODO: �ڴ���������������
	adjustPrivilege();
	LockWorkStation();
}
void C��ȫ��ʿDlg::adjustPrivilege()
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



void C��ȫ��ʿDlg::OnRestart()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	adjustPrivilege();
	int nRes = MessageBox(L"ȷ��Ҫ���������?", L"����", MB_YESNO);
	if (IDNO == nRes)
		return;
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}


void C��ȫ��ʿDlg::OnShutDown()
{
	// TODO: �ڴ���������������
	int nRes = MessageBox(L"ȷ��Ҫ�رռ����?", L"����", MB_YESNO);
	if (IDNO == nRes)
		return;
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
}


void C��ȫ��ʿDlg::OnLogOff()
{
	// TODO: �ڴ���������������
	adjustPrivilege();
	int nRes = MessageBox(L"ȷ��Ҫע����?", L"����", MB_YESNO);
	if (IDNO == nRes)
		return;
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
}


BOOL C��ȫ��ʿDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
