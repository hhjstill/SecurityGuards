// PageWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "PageWnd.h"
#include "afxdialogex.h"


// CPageWnd 对话框

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


// CPageWnd 消息处理程序


BOOL CPageWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// TODO:  在此添加额外的初始化
	m_listWnd.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listWnd.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 80);
	m_listWnd.InsertColumn(1, _T("窗口名"), LVCFMT_CENTER, 300);
	m_listWnd.InsertColumn(2, _T("句柄值"), LVCFMT_CENTER, 140);
	showAllWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
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