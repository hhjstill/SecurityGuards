// PagePE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "PagePE.h"
#include "afxdialogex.h"
#include "DlgImp.h"
#include "DlgExp.h"
#include "RscDlg.h"
#include "RelocDlg.h"


// CPagePE �Ի���

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


// CPagePE ��Ϣ�������


void CPagePE::OnBnClickedButtonPe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();
	CString filename = fileDlg.GetPathName();
	m_editPE = filename;
	UpdateData(FALSE);
}


BOOL CPagePE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
//**********************************
//�Զ��巽��                       * 
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
	printf("������Χ�������ַ\n");
	return 0;
}
void CPagePE::showHeadInfo(IMAGE_NT_HEADERS* pNt)
{
	CString infoStr;
	infoStr.Format(L"%s%d\n%s%x\n%s%x\n%s%x\n%s%x\n%s%x\n%s%x\n\n%s\n",
		L"ʱ���:", pNt->FileHeader.TimeDateStamp,
		L"��ڵ�:", pNt->OptionalHeader.AddressOfEntryPoint, 
		L"�����ַ:", pNt->OptionalHeader.ImageBase,
		L"�����ַ:", pNt->OptionalHeader.BaseOfCode,
		L"��������:",pNt->FileHeader.NumberOfSections,
		L"�ļ�����:",pNt->OptionalHeader.FileAlignment,
		L"�ڴ����:", pNt->OptionalHeader.SectionAlignment,
		L"������Ϣ:"
		);
	IMAGE_SECTION_HEADER* pScn = (IMAGE_SECTION_HEADER*)(IMAGE_FIRST_SECTION(pNt));
	
	for (DWORD i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		CString nameStr(pScn[i].Name);
		CString temp;
		temp.Format(L"%s%s\n%s%x\n%s%x\n%s%x\n",
			L"������:", nameStr,
			L"���ݴ�С:", pScn[i].SizeOfRawData,
			L"���λ�ַ:",pScn[i].VirtualAddress,
			L"�����ļ���ַ:", pScn[i].PointerToRawData
			);
		infoStr += temp;
	}
	GetDlgItem(IDC_STATICPE)->SetWindowText(infoStr);
}
//**********************************

void CPagePE::OnBnClickedButtonpe2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_editPE.GetLength())
	{
		MessageBox(L"·������Ϊ��!", L"��ʾ", 0);
		return;
	}
	HANDLE hFile = CreateFile(m_editPE, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"�ļ�������!", L"��ʾ", 0);
		return;
	}
	int fileSize = GetFileSize(hFile, 0);
	DWORD fileRead = 0;
	char* pBuff = new char[fileSize];
	ReadFile(hFile, pBuff, fileSize, &fileRead, 0);
	IMAGE_DOS_HEADER* pDos = (IMAGE_DOS_HEADER*)pBuff;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(L"������ЧPE�ļ�!", L"��ʾ", 0);
		return;
	}
	IMAGE_NT_HEADERS* pNt = (IMAGE_NT_HEADERS*)(pDos->e_lfanew + pBuff);
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"������ЧPE�ļ�!", L"��ʾ", 0);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pNt)
	{
		MessageBox(L"�Ҳ����ļ�!", L"��ʾ", 0);
		return;
	}
	CDlgImp* dlg = new CDlgImp(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOG_IMP, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pNt)
	{
		MessageBox(L"�Ҳ����ļ�!", L"��ʾ", 0);
		return;
	}
	CDlgExp* dlg = new CDlgExp(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOGEXP, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButtonrsc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pNt)
	{
		MessageBox(L"�Ҳ����ļ�!", L"��ʾ", 0);
		return;
	}
	CRscDlg* dlg = new CRscDlg(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOGRSC, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButtonreloc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pNt)
	{
		MessageBox(L"�Ҳ����ļ�!", L"��ʾ", 0);
		return;
	}
	CRelocDlg* dlg = new CRelocDlg(m_pNt, m_pBuff);
	dlg->Create(IDD_DIALOGRELOC, this);
	dlg->ShowWindow(SW_SHOW);
}


void CPagePE::OnBnClickedButtontls()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pNt)
	{
		MessageBox(L"�Ҳ����ļ�!", L"��ʾ", 0);
		return;
	}
	DWORD TlsTableRva = m_pNt->OptionalHeader.DataDirectory[9].VirtualAddress;
	//��ȡTLS��Ľṹ�����ڴ��е�λ��
	IMAGE_TLS_DIRECTORY * pTLSDir = (IMAGE_TLS_DIRECTORY*)(rvaToFoa(m_pNt, TlsTableRva) + m_pBuff);
	CString startStr, endStr, addrStr;
	startStr.Format(L"Դ���ݵ���ʼλ�ã�%x\r\n", pTLSDir->StartAddressOfRawData);
	endStr.Format(L"Դ���ݵĽ���λ�ã�%x\r\n", pTLSDir->EndAddressOfRawData);
	addrStr.Format(L"TLS�ص�������ַ��λ�ã�%x", pTLSDir->AddressOfCallBacks);
	MessageBox(startStr + endStr + addrStr, L"TLS��Ϣ:", 0);
	//printf("Դ���ݵ���ʼλ�ã�%x\n", pTLSDir->StartAddressOfRawData);
	//printf("Դ���ݵ���ֹλ��: %x\n", pTLSDir->EndAddressOfRawData);
	//printf("TLS�ص�������ַ��λ�ã�%x\n", pTLSDir->AddressOfCallBacks);

}


void CPagePE::OnBnClickedButtontdelay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pNt)
	{
		MessageBox(L"�Ҳ����ļ�!", L"��ʾ", 0);
		return;
	}
	DWORD delayImp = m_pNt->OptionalHeader.DataDirectory[13].VirtualAddress;
	if (!delayImp)
	{
		MessageBox(L"�������Ϣ!");
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
	CString resStr("���ӳټ��ص�DLLΪ:");
	resStr += str;
	MessageBox(resStr, L"��ʾ", 0);
}
