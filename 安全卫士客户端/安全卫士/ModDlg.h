#pragma once
#include "afxcmn.h"


// CModDlg �Ի���

class CModDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModDlg)

public:
	CModDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModDlg();
	CModDlg(int pId, CWnd* pParent = NULL):m_pId(pId){}
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGMOD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_pId;
	CListCtrl m_moduleList;
	virtual BOOL OnInitDialog();
};
