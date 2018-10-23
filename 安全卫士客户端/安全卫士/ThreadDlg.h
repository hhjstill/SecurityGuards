#pragma once
#include "afxcmn.h"


// CThreadDlg 对话框

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThreadDlg();
	CThreadDlg(int pId, CWnd* pParent = NULL):m_pId(pId){}
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ThreadDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_threadList;
	int m_pId;
};
