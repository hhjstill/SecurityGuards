#pragma once
#include "afxcmn.h"


// CThreadDlg �Ի���

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThreadDlg();
	CThreadDlg(int pId, CWnd* pParent = NULL):m_pId(pId){}
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ThreadDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_threadList;
	int m_pId;
};
