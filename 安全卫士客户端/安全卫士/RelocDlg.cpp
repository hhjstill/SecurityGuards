// RelocDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "RelocDlg.h"
#include "afxdialogex.h"
#include "PagePE.h"


// CRelocDlg 对话框

IMPLEMENT_DYNAMIC(CRelocDlg, CDialogEx)

CRelocDlg::CRelocDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGRELOC, pParent)
{

}

CRelocDlg::~CRelocDlg()
{
}

void CRelocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTRELOC, m_listReloc);
}


BEGIN_MESSAGE_MAP(CRelocDlg, CDialogEx)
END_MESSAGE_MAP()


// CRelocDlg 消息处理程序


BOOL CRelocDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listReloc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listReloc.InsertColumn(0, _T("类型"), LVCFMT_CENTER, 120);
	m_listReloc.InsertColumn(1, _T("需重载地址RVA"), LVCFMT_CENTER, 160);
	//m_listReloc.InsertColumn(2, _T("函数名"), LVCFMT_CENTER, 260);
	struct TypeOffset
	{
		WORD offset : 12;
		WORD type : 4;
	};
	DWORD relocTabRva = m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	if (!relocTabRva)
	{
		MessageBox(L"无重定位数据", L"提示", 0);
		return 0;
	}
	//DWORD relocTabFoa = CPagePE::rvaToFoa(m_pNt, relocTabRva);
	IMAGE_BASE_RELOCATION* pRelocTab = (IMAGE_BASE_RELOCATION*)(CPagePE::rvaToFoa(m_pNt, relocTabRva) + m_pBuff);
	while (pRelocTab->SizeOfBlock)
	{
		//printf("RVA:%08x\n", pRelocTab->VirtualAddress);
		TypeOffset* pTypeOffset = (TypeOffset*)(pRelocTab + 1);
		DWORD dwNum = (pRelocTab->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
		for (DWORD i = 0; i < dwNum; i++)
		{
			int nIndex = m_listReloc.GetItemCount();
			CString str;
			str.Format(L"%d", pTypeOffset[i].type);
			m_listReloc.InsertItem(nIndex, L"");
			m_listReloc.SetItemText(nIndex, 0, str);
			str.Format(L"%X", pTypeOffset[i].offset + pRelocTab->VirtualAddress);
			m_listReloc.SetItemText(nIndex, 1, str);
			//printf("\ttype:%d -- Offset:%x\n", pTypeOffset[i].type, pTypeOffset[i].offset);
		}
		pRelocTab = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelocTab + pRelocTab->SizeOfBlock);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

