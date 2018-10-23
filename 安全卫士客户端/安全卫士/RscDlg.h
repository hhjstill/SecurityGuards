#pragma once
#include "afxcmn.h"


// CRscDlg 对话框

class CRscDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRscDlg)

public:
	CRscDlg(CWnd* pParent = NULL);   // 标准构造函数
	CRscDlg(IMAGE_NT_HEADERS* pNt, char* pBuff, CWnd* pParent = NULL):m_pNt(pNt), m_pBuff(pBuff){}
	virtual ~CRscDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRSC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void showResource(LPBYTE pRscRoot, IMAGE_RESOURCE_DIRECTORY* pRescDir, DWORD nLevel = 1);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listRsc;
	IMAGE_NT_HEADERS* m_pNt;
	char* m_pBuff;
	virtual BOOL OnInitDialog();
};
