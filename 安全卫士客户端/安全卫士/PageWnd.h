#pragma once
#include "afxcmn.h"


// CPageWnd �Ի���

class CPageWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CPageWnd)

public:
	CPageWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageWnd();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGEWND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void showAllWnd();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listWnd;
	virtual BOOL OnInitDialog();
};
