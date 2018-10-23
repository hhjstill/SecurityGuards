#pragma once
#include "afxcmn.h"


// CDlgImp 对话框

class CDlgImp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImp)

public:
	CDlgImp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImp();
	CDlgImp(IMAGE_NT_HEADERS* pNt, char* pBuff, CWnd* pParent = NULL):m_pNt(pNt), m_pBuff(pBuff){}
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// //显示DLL信息
	IMAGE_NT_HEADERS* m_pNt;
	char* m_pBuff;
	CListCtrl m_listUP;
	CListCtrl m_listDOWN;
	virtual BOOL OnInitDialog();
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnLvnItemchangingListimp2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListimp2(NMHDR *pNMHDR, LRESULT *pResult);
};
