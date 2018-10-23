// PageProcess.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "PageProcess.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "ThreadDlg.h"
#include "ModDlg.h"


// CPageProcess �Ի���

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


// CPageProcess ��Ϣ�������


BOOL CPageProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//m_menu.LoadMenu(IDR_MENU1);
	//SetMenu(&m_menu);
	m_proListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_proListCtrl.InsertColumn(0, _T("���"), LVCFMT_CENTER, 80);
	m_proListCtrl.InsertColumn(1, _T("������"), LVCFMT_CENTER, 300);
	m_proListCtrl.InsertColumn(2, _T("PID"), LVCFMT_CENTER, 140);
	showAllProcess();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
void CPageProcess::showAllProcess()
{
	//��������32λ��,û��64 ��.���ǵ�.
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu* menu = new CMenu;
	menu->LoadMenu(IDR_MENU1);
	CMenu *pPopup = menu->GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵�
	CPoint point1;//����һ������ȷ�����λ�õ�λ�� 
	GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����� 
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);//��ָ��λ����ʾ�����˵�
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
	// TODO: �ڴ���������������
	HANDLE hPro = OpenProcess(PROCESS_TERMINATE, FALSE, m_pId);
	if (hPro == NULL)
	{
		MessageBox(L"���ܽ����ý���", L"��ʾ:", 0);
		return;
	}
	TerminateProcess(hPro, 0);
	showAllProcess();
}


void CPageProcess::OnClickThread()
{
	// TODO: �ڴ���������������
	CThreadDlg* dlg = new CThreadDlg(m_pId);
	dlg->Create(IDD_ThreadDlg,this);
	dlg->ShowWindow(SW_SHOW);
}


void CPageProcess::OnClickMod()
{
	// TODO: �ڴ���������������
	CModDlg* dlg = new CModDlg(m_pId);
	dlg->Create(IDD_DIALOGMOD, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPageProcess::OnBnClickedButtonprotect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL b = Process32First(hSnap, &pe);
	int pId = -1;
	if (b)
	{
		do {
			if (wcscmp(pe.szExeFile, L"���������.exe") == 0)
			{
				pId = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnap, &pe));
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	if (!hProcess)
	{
		printf("��ȡ���̾��ʧ��!");
		return;
	}
	//64λDLL·��
	//char path[] = "D:\\c����\\HOOK\\x64\\Debug\\hookDLL.dll";
	//32λDLL·��
	char path[] = "D:\\c����\\HOOK\\Debug\\hookDLL.dll";
	int nLen = sizeof(path);
	//VirtualAllocEx(hProcess, pBuff, 0x1000, MEM_COMMIT, PAGE_READWRITE);
	LPVOID pBuff = VirtualAllocEx(hProcess, 0, nLen, MEM_COMMIT, PAGE_READWRITE);
	SIZE_T beWriten = 0;
	WriteProcessMemory(hProcess, pBuff, path, sizeof(path), &beWriten);
	LPTHREAD_START_ROUTINE pStartAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	//DWORD iiiiiiiiid;
	HANDLE hThr = CreateRemoteThread(hProcess, 0, 0, pStartAddr, pBuff, 0, 0);
	WaitForSingleObject(hThr, -1);
	MessageBox(L"ע��ɹ�!", L"��ʾ:", 0);
	//hThr = CreateRemoteThread(hProcess, 0, 0, LPTHREAD_START_ROUTINE(FreeLibrary), pBuff, 0, 0);
}
