#pragma once
#include "afxcmn.h"


// CPagePE �Ի���

class CPagePE : public CDialogEx
{
	DECLARE_DYNAMIC(CPagePE)

public:
	CPagePE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPagePE();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPe();
	CString m_editPE;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonpe2();
public:
	void showHeadInfo(IMAGE_NT_HEADERS* pNt);
	static DWORD rvaToFoa(IMAGE_NT_HEADERS* pNt, DWORD rva);
	afx_msg void OnBnClickedButtonExp();
public:
	IMAGE_NT_HEADERS* m_pNt = NULL;
	char* m_pBuff = NULL;
	
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedButtonrsc();
	afx_msg void OnBnClickedButtonreloc();
	afx_msg void OnBnClickedButtontls();
	afx_msg void OnBnClickedButtontdelay();
};
