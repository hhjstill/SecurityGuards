
// 安全卫士Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "PageProcess.h"
#include "PagePE.h"
#include "PageWnd.h"
#include "CleanDlg.h"
#include "PageService.h"
#include "PageStart.h"


// C安全卫士Dlg 对话框
class C安全卫士Dlg : public CDialogEx
{
// 构造
public:
	C安全卫士Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	void adjustPrivilege();
	// 生成的消息映射函数
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
