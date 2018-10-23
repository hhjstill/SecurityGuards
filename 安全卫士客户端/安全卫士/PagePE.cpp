// PagePE.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "PagePE.h"
#include "afxdialogex.h"
#include "DlgImp.h"
#include "DlgExp.h"
#include "RscDlg.h"
#include "RelocDlg.h"


// CPagePE 对话框

IMPLEMENT_DYNAMIC(CPagePE, CDialogEx)

CPagePE::CPagePE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_pagePE, pParent)
	, m_editPE(_T(""))
{

}

CPagePE::~CPagePE()
{
}

void CPagePE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PE, m_editPE);
	//  DDX_Control(pDX, IDC_LISTPE, m_listPE);
}


BEGIN_MESSAGE_MAP(CPagePE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PE, &CPagePE::OnBnClickedButtonPe)
	ON_BN_CLICKED(IDC_BUTTONPE2, &CPagePE::OnBnClickedButtonpe2)
	ON_BN_CLICKED(IDC_BUTTON_EXP, &CPagePE::OnBnClickedButtonExp)
	ON_BN_CLICKED(IDC_BUTTON__exp, &CPagePE::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTONRSC, &CPagePE::OnBnClickedButtonrsc)
	ON_BN_CLICKED(IDC_BUTTONRELOC, &CPagePE::OnBnClickedButtonreloc)
	ON_BN_CLICKED(IDC_BUTTONTLS, &CPagePE::OnBnClickedButtontls)
	ON_BN_CLICKED(IDC_BUTTONTDELAY, &CPagePE::OnBnClickedButtontdelay)
END_MESSAGE_MAP()


// CPagePE 消息处理程序


void CPagePE::OnBnClickedButtonPe()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();
	CString filename = fileDlg.GetPathName();
	m_editPE = filename;
	UpdateData(FALSE);
}


BOOL CPagePE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//**********************************
//自定义方法                       * 
//**********************************
DWORD CPagePE::rvaToFoa(IMAGE_NT_HEADERS* pNt, DWORD rva)
{
	IMAGE_SECTION_HEADER* pScn = (IMAGE_SECTION_HEADER*)IMAGE_FIRST_SECTION(pNt);
	DWORD dwScnCnt = pNt->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwScnCnt; i++)
	{
		if (rva >= pScn[i].VirtualAddress && rva <= pScn[i].VirtualAddress + pScn[i].SizeOfRawData)
		{
			return rva - pScn[i].VirtualAddress + pScn[i].PointerToRawData;
		}
	}
	printf("超出范围的虚拟地址\n");
	return 0;
}
void CPagePE::showHeadInfo(IMAGE_NT_HEADERS* pNt)
{
	CString infoStr;
	infoStr.Format(L"%s%d\n%s%x\n%s%x\n%s%x\n%s%x\n%s%x\n%s%x\n\n%s\n",
		L"时间戳:", pNt->FileHeader.TimeDateStamp,
		L"入口点:", pNt->OptionalHeader.AddressOfEntryPoint, 
		L"镜像基址:", pNt->OptionalHeader.ImageBase,
		L"代码基址:", pNt->OptionalHeader.BaseOfCode,
		L"区段数量:",pNt->FileHeader.NumberOfSections,
		L"文件对齐:",pNt->OptionalHeader.FileAlignment,
		L"内存对齐:", pNt->OptionalHeader.SectionAlignment,
		L"区段信息:"
		);
	IMAGE_SECTION_HEADER* pScn = (IMAGE_SECTION_HEADER*)(IMAGE_FIRST_SECTION(pNt));
	
	for (DWORD i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		CString nameStr(pScn[i].Name);
		CString temp;
		temp.Format(L"%s%s\n%s%x\n%s%x\n%s%x\n",
			L"区段名:", nameStr,
			L"数据大小:", pScn[i].SizeOfRawData,
			L"区段基址:",pScn[i].VirtualAddress,
			L"区段文件基址:", pScn[i].PointerToRawData
			);
		infoStr += temp;
	}
	GetDlgItem(IDC_STATICPE)->SetWindowText(infoStr);
}
//**********************************

void CPagePE::OnBnClickedButtonpe2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_editPE.GetLength())
	{
		MessageBox(L"路径不能为空!", L"提示", 0);
		return;
	}
	HANDLE hFile = CreateFile(m_editPE, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"文件不存在!", L"提示", 0);
		return;
	}
	int fileSize = GetFileSize(hFile, 0);
	DWORD fileRead = 0;
	char* pBuff = new char[fileSize];
	ReadFile(hFile, pBuff, fileSize, &fileRead, 0);
	IMAGE_DOS_HEADER* pDos = (IMAGE_DOS_HEADER*)pBuff;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(L"不是有效PE文件!", L"提示", 0);
		return;
	}
	IMAGE_NT_HEADERS* pNt = (IMAGE_NT_HEADERS*)(pDos->e_lfanew + pBuff);
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"不是有效PE文件!", L"提示", 0);
		return;
	}
	m_pNt = pNt;
	m_pBuff = pBuff;
	showHeadInfo(pNt);
	CloseHandle(hFile);
	//GetDlgItem(IDC_STATICPE)->SetWindowText(L"123");
}


void CPagePE::OnBnClickedButtonExp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pNt)
	{
		MessageBox(L"找不到文件!", L"提示", 0);
		return;
	}
	CDlgImp* dlg = new CDlgImp(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOG_IMP, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pNt)
	{
		MessageBox(L"找不到文件!", L"提示", 0);
		return;
	}
	CDlgExp* dlg = new CDlgExp(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOGEXP, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButtonrsc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pNt)
	{
		MessageBox(L"找不到文件!", L"提示", 0);
		return;
	}
	CRscDlg* dlg = new CRscDlg(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOGRSC, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButtonreloc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pNt)
	{
		MessageBox(L"找不到文件!", L"提示", 0);
		return;
	}
	CRelocDlg* dlg = new CRelocDlg(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOGRELOC, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButtontls()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pNt)
	{
		MessageBox(L"找不到文件!", L"提示", 0);
		return;
	}
	DWORD TlsTableRva = m_pNt->OptionalHeader.DataDirectory[9].VirtualAddress;
	//获取TLS表的结构体在内存中的位置
	IMAGE_TLS_DIRECTORY * pTLSDir = (IMAGE_TLS_DIRECTORY*)(rvaToFoa(m_pNt, TlsTableRva) + m_pBuff);
	CString startStr, endStr, addrStr;
	startStr.Format(L"源数据的起始位置：%x\r\n", pTLSDir->StartAddressOfRawData);
	endStr.Format(L"源数据的结束位置：%x\r\n", pTLSDir->EndAddressOfRawData);
	addrStr.Format(L"TLS回调函数地址表位置：%x", pTLSDir->AddressOfCallBacks);
	MessageBox(startStr + endStr + addrStr, L"TLS信息:", 0);
	//printf("源数据的起始位置：%x\n", pTLSDir->StartAddressOfRawData);
	//printf("源数据的终止位置: %x\n", pTLSDir->EndAddressOfRawData);
	//printf("TLS回调函数地址表位置：%x\n", pTLSDir->AddressOfCallBacks);

}


void CPagePE::OnBnClickedButtontdelay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pNt)
	{
		MessageBox(L"找不到文件!", L"提示", 0);
		return;
	}
	DWORD delayImp = m_pNt->OptionalHeader.DataDirectory[13].VirtualAddress;
	if (!delayImp)
	{
		MessageBox(L"无相关信息!");
		return;
	}
	IMAGE_DELAYLOAD_DESCRIPTOR* delayImpFoa = (IMAGE_DELAYLOAD_DESCRIPTOR*)(rvaToFoa(m_pNt, delayImp) + m_pBuff);
	CString str;
	while (delayImpFoa->DllNameRVA)
	{
		char* pNameFoa = (char*)(rvaToFoa(m_pNt, delayImpFoa->DllNameRVA) + m_pBuff);
		CString temp;
		temp.Format(L"%s\r\n", pNameFoa);
		str += temp;
	}
	CString resStr("被延迟加载的DLL为:");
	resStr += str;
	MessageBox(resStr, L"提示", 0);
}
