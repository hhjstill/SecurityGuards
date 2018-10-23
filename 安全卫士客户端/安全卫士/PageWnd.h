#pragma once
#include "afxcmn.h"


// CPageWnd 对话框

class CPageWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CPageWnd)

public:
	CPageWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGEWND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void showAllWnd();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listWnd;
	virtual BOOL OnInitDialog();
};
