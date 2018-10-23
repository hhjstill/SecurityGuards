#pragma once
#include "afxcmn.h"
#include "regedit.h"


// CPageStart �Ի���

class CPageStart : public CDialogEx
{
	DECLARE_DYNAMIC(CPageStart)

public:
	CPageStart(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageStart();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGSTATR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listStart;
	CSoftInfo m_softInfo;
	string getFileNameByFilePath(string filePath);
	void addToAuto(string filePath);
	void createStringReg(HKEY hRoot, char *szSubKey, char* valueName, char *Data);
	string getFilePathByWindows();
	virtual BOOL OnInitDialog();
//	afx_msg void OnLvnItemchangedListstart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonxiezai();
	afx_msg void OnBnClickedButtonziqidong();
};
