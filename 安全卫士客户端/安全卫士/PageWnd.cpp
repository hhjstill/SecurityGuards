// PageWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "PageWnd.h"
#include "afxdialogex.h"


// CPageWnd �Ի���

IMPLEMENT_DYNAMIC(CPageWnd, CDialogEx)

CPageWnd::CPageWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PAGEWND, pParent)
{

}

CPageWnd::~CPageWnd()
{
}

void CPageWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTWND, m_listWnd);
}


BEGIN_MESSAGE_MAP(CPageWnd, CDialogEx)
END_MESSAGE_MAP()


// CPageWnd ��Ϣ�������


BOOL CPageWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listWnd.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listWnd.InsertColumn(0, _T("���"), LVCFMT_CENTER, 80);
	m_listWnd.InsertColumn(1, _T("������"), LVCFMT_CENTER, 300);
	m_listWnd.InsertColumn(2, _T("���ֵ"), LVCFMT_CENTER, 140);
	showAllWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CALLBACK wndProc(HWND hWnd, LPARAM lParam)
{
	CListCtrl* pCtr = (CListCtrl*)(lParam);
	static UINT nIndex = 0;
	TCHAR tBuf[64]{};
	GetWindowText(hWnd, tBuf, _countof(tBuf));
	BOOL isShow = IsWindowVisible(hWnd);
	int nLen = wcslen(tBuf);
	if (isShow == TRUE && nLen != 0)
	{
		CString str;
		str.Format(L"%d", nIndex + 1);
		pCtr->InsertItem(nIndex, _T(""));
		pCtr->SetItemText(nIndex, 0, str);
		pCtr->SetItemText(nIndex, 1, tBuf);
		str.Format(L"%X", hWnd);
		pCtr->SetItemText(nIndex, 2, str);
		nIndex++;
	}

	return TRUE;
}
void CPageWnd::showAllWnd()
{
	EnumWindows(wndProc, (LPARAM)&m_listWnd);
}