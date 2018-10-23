#pragma once
#include "afxcmn.h"


// CRelocDlg 对话框

class CRelocDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocDlg)

public:
	CRelocDlg(CWnd* pParent = NULL);   // 标准构造函数
	CRelocDlg(IMAGE_NT_HEADERS* pNt,char* pBuff,CWnd* pParent = NULL):m_pNt(pNt), m_pBuff(pBuff){}
	virtual ~CRelocDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRELOC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	IMAGE_NT_HEADERS* m_pNt;
	char* m_pBuff;
	CListCtrl m_listReloc;
	virtual BOOL OnInitDialog();
};
