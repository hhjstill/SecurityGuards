#pragma once
#include "afxcmn.h"
#include <vector>


// CCleanDlg �Ի���

class CCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanDlg)

public:
	CCleanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCleanDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGCLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void cleanFile(const CString& path);
	bool isInWhiteList(char* szName);
	BOOL isRubish(const CString& path);
	void md5CleanFile(const CString& path,char* md5Str);
	void whiteListCleanFile(const CString& path, char* listStr);
	BOOL cmpMd5(const CString& path, char* allMd5);

	BOOL InitWinSock();
	int conn(char* ipAddr, int port);
	void mySend(const CString& path, int nType);
	void mySendInfo(CString& path, int nType);
	DECLARE_MESSAGE_MAP()
public:

	CString m_CleanEdit;
	// ��׺
	CString m_postfixEdit;
	SOCKET m_sock = 0;
	std::vector<CString> m_vecFileName;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedOk();
	
	
	afx_msg void OnBnClickedButtonmem();
	afx_msg void OnBnClickedButtonmd5();
	afx_msg void OnBnClickedButtonwhite();
	afx_msg void OnBnClickedButtoncloude();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtondemo();
	CListCtrl m_listClean;
	afx_msg void OnBnClickedButtonmd();
};
