// DlgExp.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "DlgExp.h"
#include "afxdialogex.h"
#include "PagePE.h"


// CDlgExp 对话框

IMPLEMENT_DYNAMIC(CDlgExp, CDialogEx)

CDlgExp::CDlgExp(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGEXP, pParent)
	, m_edit1(_T(""))
	, m_edit2(_T(""))
	, m_edit3(_T(""))
	, m_edit4(_T(""))
	, m_edit5(_T(""))
	, m_edit6(_T(""))
	, m_edit7(_T(""))
{

}

CDlgExp::~CDlgExp()
{
}

void CDlgExp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTEXP, m_ListExp);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT3, m_edit2);
	DDX_Text(pDX, IDC_EDIT2, m_edit3);
	DDX_Text(pDX, IDC_EDIT4, m_edit4);
	DDX_Text(pDX, IDC_EDIT5, m_edit5);
	DDX_Text(pDX, IDC_EDIT6, m_edit6);
	DDX_Text(pDX, IDC_EDIT7, m_edit7);
}


BEGIN_MESSAGE_MAP(CDlgExp, CDialogEx)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgExp 消息处理程序


//void CDlgExp::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnLButtonUp(nFlags, point);
//}


BOOL CDlgExp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListExp.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListExp.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 120);
	m_ListExp.InsertColumn(1, _T("RVA"), LVCFMT_CENTER, 120);
	m_ListExp.InsertColumn(2, _T("函数名"), LVCFMT_CENTER, 260); 
	DWORD expTabRva = m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	IMAGE_EXPORT_DIRECTORY* expTabFoa = (IMAGE_EXPORT_DIRECTORY*)(CPagePE::rvaToFoa(m_pNt, expTabRva) + m_pBuff);
	DWORD* Ent = (DWORD*)(CPagePE::rvaToFoa(m_pNt, expTabFoa->AddressOfNames) + m_pBuff);
	WORD* Eot = (WORD*)(CPagePE::rvaToFoa(m_pNt, expTabFoa->AddressOfNameOrdinals) + m_pBuff);
	DWORD* Eat = (DWORD*)(CPagePE::rvaToFoa(m_pNt, expTabFoa->AddressOfFunctions) + m_pBuff);
	CString str;
	for (DWORD i = 0; i < expTabFoa->NumberOfNames; i++)
	{
		m_ListExp.InsertItem(i, L"");
		char* name = (char*)(CPagePE::rvaToFoa(m_pNt, Ent[i]) + m_pBuff);
		//printf("%s\n", name);
		CString tempStr(name);
		m_ListExp.SetItemText(i, 2, tempStr);
		WORD index = Eot[i];
		str.Format(L"%d", index + 1);
		m_ListExp.SetItemText(i, 0, str);
		DWORD addr = Eat[index];
		str.Format(L"%x", addr);
		m_ListExp.SetItemText(i, 1, str);
	}
	
	str.Format(L"%d", expTabFoa->NumberOfFunctions);
	m_edit1 = str;
	str.Format(L"%d", expTabFoa->NumberOfNames);
	m_edit2 = str;
	str.Format(L"%x", expTabFoa->AddressOfFunctions);
	m_edit3 = str;
	str.Format(L"%x", expTabFoa->AddressOfNames);
	m_edit4 = str;
	str.Format(L"%x", expTabFoa->AddressOfNameOrdinals);
	m_edit5 = str;
	str.Format(L"%x", (DWORD)expTabFoa);
	m_edit6 = str;
	char* dllName = (char*)(CPagePE::rvaToFoa(m_pNt, expTabFoa->Name) + m_pBuff);
	CString tempStr(dllName);
	m_edit7 = tempStr;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
