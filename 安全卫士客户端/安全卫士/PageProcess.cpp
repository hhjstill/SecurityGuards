// PageProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "PageProcess.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "ThreadDlg.h"
#include "ModDlg.h"


// CPageProcess 对话框

IMPLEMENT_DYNAMIC(CPageProcess, CDialogEx)

CPageProcess::CPageProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_pageprocess, pParent)
{

}

CPageProcess::~CPageProcess()
{
}

void CPageProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESSLIST, m_proListCtrl);
}


BEGIN_MESSAGE_MAP(CPageProcess, CDialogEx)
//	ON_WM_RBUTTONUP()
//ON_WM_RBUTTONUP()
ON_NOTIFY(NM_RCLICK, IDC_PROCESSLIST, &CPageProcess::OnNMRClickProcesslist)
ON_COMMAND(ID_32771, &CPageProcess::OnClickTerminate)
ON_COMMAND(ID_32772, &CPageProcess::OnClickThread)
ON_COMMAND(ID_32773, &CPageProcess::OnClickMod)
ON_BN_CLICKED(IDC_BUTTONPROTECT, &CPageProcess::OnBnClickedButtonprotect)
END_MESSAGE_MAP()


// CPageProcess 消息处理程序


BOOL CPageProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_menu.LoadMenu(IDR_MENU1);
	//SetMenu(&m_menu);
	m_proListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_proListCtrl.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 80);
	m_proListCtrl.InsertColumn(1, _T("进程名"), LVCFMT_CENTER, 300);
	m_proListCtrl.InsertColumn(2, _T("PID"), LVCFMT_CENTER, 140);
	showAllProcess();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CPageProcess::showAllProcess()
{
	//好像是有32位的,没的64 的.不是的.
	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL b = Process32First(hSnap, &pe);
	int nIndex = 0;
	CString str;
	m_proListCtrl.DeleteAllItems();
	if (b)
	{
		do {
			str.Format(L"%d", nIndex + 1);
			m_proListCtrl.InsertItem(nIndex, _T(""));
			m_proListCtrl.SetItemText(nIndex, 0, str);
			m_proListCtrl.SetItemText(nIndex, 1, pe.szExeFile);
			str.Format(L"%d", pe.th32ProcessID);
			m_proListCtrl.SetItemText(nIndex, 2, str);
			nIndex++;
		} while (Process32Next(hSnap, &pe));
	}
}





void CPageProcess::OnNMRClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu* menu = new CMenu;
	menu->LoadMenu(IDR_MENU1);
	CMenu *pPopup = menu->GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
	CPoint point1;//定义一个用于确定光标位置的位置 
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标 
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);//在指定位置显示弹出菜单
	if (pNMItemActivate->iItem >= 0)
	{
		CString pidStr = m_proListCtrl.GetItemText(pNMItemActivate->iItem, 2);
		CStringA str(pidStr);
		sscanf_s(str, "%d", &m_pId);
	}
	*pResult = 0;
}


void CPageProcess::OnClickTerminate()
{
	// TODO: 在此添加命令处理程序代码
	HANDLE hPro = OpenProcess(PROCESS_TERMINATE, FALSE, m_pId);
	if (hPro == NULL)
	{
		MessageBox(L"不能结束该进程", L"提示:", 0);
		return;
	}
	TerminateProcess(hPro, 0);
	showAllProcess();
}


void CPageProcess::OnClickThread()
{
	// TODO: 在此添加命令处理程序代码
	CThreadDlg* dlg = new CThreadDlg(m_pId);
	dlg->Create(IDD_ThreadDlg,this);
	dlg->ShowWindow(SW_SHOW);
}


void CPageProcess::OnClickMod()
{
	// TODO: 在此添加命令处理程序代码
	CModDlg* dlg = new CModDlg(m_pId);
	dlg->Create(IDD_DIALOGMOD, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPageProcess::OnBnClickedButtonprotect()
{
	// TODO: 在此添加控件通知处理程序代码
	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL b = Process32First(hSnap, &pe);
	int pId = -1;
	if (b)
	{
		do {
			if (wcscmp(pe.szExeFile, L"任务管理器.exe") == 0)
			{
				pId = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnap, &pe));
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	if (!hProcess)
	{
		printf("获取进程句柄失败!");
		return;
	}
	//64位DLL路径
	//char path[] = "D:\\c代码\\HOOK\\x64\\Debug\\hookDLL.dll";
	//32位DLL路径
	char path[] = "D:\\c代码\\HOOK\\Debug\\hookDLL.dll";
	int nLen = sizeof(path);
	//VirtualAllocEx(hProcess, pBuff, 0x1000, MEM_COMMIT, PAGE_READWRITE);
	LPVOID pBuff = VirtualAllocEx(hProcess, 0, nLen, MEM_COMMIT, PAGE_READWRITE);
	SIZE_T beWriten = 0;
	WriteProcessMemory(hProcess, pBuff, path, sizeof(path), &beWriten);
	LPTHREAD_START_ROUTINE pStartAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	//DWORD iiiiiiiiid;
	HANDLE hThr = CreateRemoteThread(hProcess, 0, 0, pStartAddr, pBuff, 0, 0);
	WaitForSingleObject(hThr, -1);
	MessageBox(L"注入成功!", L"提示:", 0);
	//hThr = CreateRemoteThread(hProcess, 0, 0, LPTHREAD_START_ROUTINE(FreeLibrary), pBuff, 0, 0);
}
