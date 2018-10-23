#pragma once
#include "afxcmn.h"


// CPageProcess �Ի���

class CPageProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CPageProcess)

public:
	CPageProcess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageProcess();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_pageprocess };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	void showAllProcess();
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_proListCtrl;
	int m_pId;
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNMRClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickTerminate();
	afx_msg void OnClickThread();
	afx_msg void OnClickMod();
	afx_msg void OnBnClickedButtonprotect();
};
