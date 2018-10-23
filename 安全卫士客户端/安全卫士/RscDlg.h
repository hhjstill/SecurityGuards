#pragma once
#include "afxcmn.h"


// CRscDlg �Ի���

class CRscDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRscDlg)

public:
	CRscDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CRscDlg(IMAGE_NT_HEADERS* pNt, char* pBuff, CWnd* pParent = NULL):m_pNt(pNt), m_pBuff(pBuff){}
	virtual ~CRscDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRSC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void showResource(LPBYTE pRscRoot, IMAGE_RESOURCE_DIRECTORY* pRescDir, DWORD nLevel = 1);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listRsc;
	IMAGE_NT_HEADERS* m_pNt;
	char* m_pBuff;
	virtual BOOL OnInitDialog();
};
