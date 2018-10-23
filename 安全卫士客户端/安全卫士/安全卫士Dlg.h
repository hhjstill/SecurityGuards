
// ��ȫ��ʿDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "PageProcess.h"
#include "PagePE.h"
#include "PageWnd.h"
#include "CleanDlg.h"
#include "PageService.h"
#include "PageStart.h"


// C��ȫ��ʿDlg �Ի���
class C��ȫ��ʿDlg : public CDialogEx
{
// ����
public:
	C��ȫ��ʿDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	void adjustPrivilege();
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_isShow = TRUE;
	CMenu m_menu;
	CTabCtrl m_tabWin;
	CPageProcess m_pageProcess;
	CPageWnd m_pageWnd;
	CPagePE m_pagePE;
	CCleanDlg m_pageClean;
	CStatusBar m_statusBar;
	CPageService m_pageSvc;
	CPageStart m_pageStart;
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	int getCpuLoad();
	double FILETME2Double(const _FILETIME& fileTime);
	afx_msg void OnLock();
	afx_msg void OnRestart();
	afx_msg void OnShutDown();
	afx_msg void OnLogOff();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
