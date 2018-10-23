#pragma once
#include "afxcmn.h"


// CModDlg 对话框

class CModDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModDlg)

public:
	CModDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModDlg();
	CModDlg(int pId, CWnd* pParent = NULL):m_pId(pId){}
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGMOD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_pId;
	CListCtrl m_moduleList;
	virtual BOOL OnInitDialog();
};
