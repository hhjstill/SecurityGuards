#pragma once
#include "afxcmn.h"


// CPageService 对话框

class CPageService : public CDialogEx
{
	DECLARE_DYNAMIC(CPageService)

public:
	CPageService(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageService();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGSERVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void showAllService();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listService;
	CString m_svcName;
	CString m_svcStatus;
	int m_nIndex = -1;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickListservice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickStartSvc();
	afx_msg void OnClickCloseSvc();
};
