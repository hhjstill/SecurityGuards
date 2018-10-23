// ModDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "ModDlg.h"
#include "afxdialogex.h"
#include <PSAPI.H>

// CModDlg �Ի���

IMPLEMENT_DYNAMIC(CModDlg, CDialogEx)

CModDlg::CModDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGMOD, pParent)
{

}

CModDlg::~CModDlg()
{
}

void CModDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTMOD, m_moduleList);
}


BEGIN_MESSAGE_MAP(CModDlg, CDialogEx)
END_MESSAGE_MAP()


// CModDlg ��Ϣ�������


BOOL CModDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_moduleList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_moduleList.InsertColumn(0, _T("���"), LVCFMT_CENTER, 80);
	m_moduleList.InsertColumn(1, _T("ģ��·��"), LVCFMT_CENTER, 300);
	m_moduleList.InsertColumn(2, _T("ģ���С"), LVCFMT_CENTER, 140);
	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_pId);
	if (!hProcess)
	{
		MessageBox(_T("��ȡ���̾��ʧ��"));
		return FALSE;
	}
	HMODULE hModules[0x2000]{};
	DWORD dwNeed = 0;
	BOOL bRet = EnumProcessModulesEx(hProcess, hModules, sizeof(hModules), &dwNeed, LIST_MODULES_ALL);
	DWORD dwModuleCnt = dwNeed / sizeof(HMODULE);
	MODULEINFO mi{};
	TCHAR tBuf[128]{};
	CString	str;
	m_moduleList.DeleteAllItems();
	for (UINT i = 0; i < dwModuleCnt; i++)
	{
		GetModuleFileNameEx(hProcess, hModules[i], tBuf, _countof(tBuf));
		GetModuleInformation(hProcess, hModules[i], &mi, sizeof(MODULEINFO));
		str.Format(L"%d", i + 1);
		m_moduleList.InsertItem(i, _T(""));
		m_moduleList.SetItemText(i, 0, str);
		m_moduleList.SetItemText(i, 1, tBuf);
		str.Format(L"%dKB", mi.SizeOfImage / 1024);
		m_moduleList.SetItemText(i, 2, str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
