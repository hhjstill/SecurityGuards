// ThreadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "ThreadDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CThreadDlg �Ի���

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


// CThreadDlg ��Ϣ�������


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_threadList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_threadList.InsertColumn(0, _T("���"), LVCFMT_CENTER, 80);
	m_threadList.InsertColumn(1, _T("���ȼ�"), LVCFMT_CENTER, 300);
	m_threadList.InsertColumn(2, _T("�߳�ID"), LVCFMT_CENTER, 140);
	
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hSnap)
	{
		MessageBox(L"�߳̿��վ������ʧ��");
		return FALSE;
	}
	THREADENTRY32 te{ sizeof(THREADENTRY32) };
	BOOL bSucceed = Thread32First(hSnap, &te);
	if (!bSucceed)
	{
		MessageBox(L"�Ҳ�����һ���߳�");
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}
