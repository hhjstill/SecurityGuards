// ThreadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "ThreadDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CThreadDlg 对话框

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ThreadDlg, pParent)
{

}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTTHREAD, m_threadList);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadDlg 消息处理程序


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_threadList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_threadList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 80);
	m_threadList.InsertColumn(1, _T("优先级"), LVCFMT_CENTER, 300);
	m_threadList.InsertColumn(2, _T("线程ID"), LVCFMT_CENTER, 140);
	
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hSnap)
	{
		MessageBox(L"线程快照句柄创建失败");
		return FALSE;
	}
	THREADENTRY32 te{ sizeof(THREADENTRY32) };
	BOOL bSucceed = Thread32First(hSnap, &te);
	if (!bSucceed)
	{
		MessageBox(L"找不到第一个线程");
		return FALSE;
	}
	int nIndex = 0;
	CString str;
	do {
		if (te.th32OwnerProcessID == m_pId)
		{
			str.Format(L"%d", nIndex + 1);
			m_threadList.InsertItem(nIndex, _T(""));
			m_threadList.SetItemText(nIndex, 0, str);
			str.Format(L"%d", te.tpBasePri);
			m_threadList.SetItemText(nIndex, 1, str);
			str.Format(L"%d", te.th32ThreadID);
			m_threadList.SetItemText(nIndex, 2, str);
			nIndex++;
		}
	} while (Thread32Next(hSnap, &te));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
