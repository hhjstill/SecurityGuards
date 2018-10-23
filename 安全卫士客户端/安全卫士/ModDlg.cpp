// ModDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "ModDlg.h"
#include "afxdialogex.h"
#include <PSAPI.H>

// CModDlg 对话框

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


// CModDlg 消息处理程序


BOOL CModDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_moduleList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_moduleList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 80);
	m_moduleList.InsertColumn(1, _T("模块路径"), LVCFMT_CENTER, 300);
	m_moduleList.InsertColumn(2, _T("模块大小"), LVCFMT_CENTER, 140);
	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_pId);
	if (!hProcess)
	{
		MessageBox(_T("获取进程句柄失败"));
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
				  // 异常: OCX 属性页应返回 FALSE
}
