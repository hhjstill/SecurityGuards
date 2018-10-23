#pragma once
#include "afxcmn.h"


// CDlgExp 对话框

class CDlgExp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExp)

public:
	CDlgExp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExp();
	CDlgExp(IMAGE_NT_HEADERS* pNt, char* pBuff, CWnd* pParent = NULL) :m_pNt(pNt), m_pBuff(pBuff) {}
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGEXP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListExp;
	IMAGE_NT_HEADERS* m_pNt;
	char* m_pBuff;
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	CString m_edit1;
	CString m_edit2;
	CString m_edit3;
	CString m_edit4;
	CString m_edit5;
	CString m_edit6;
	CString m_edit7;
};
