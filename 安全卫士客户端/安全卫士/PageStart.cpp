// PageStart.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "PageStart.h"
#include "afxdialogex.h"


// CPageStart �Ի���

IMPLEMENT_DYNAMIC(CPageStart, CDialogEx)

CPageStart::CPageStart(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGSTATR, pParent)
{

}

CPageStart::~CPageStart()
{
}

void CPageStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTSTART, m_listStart);
}


BEGIN_MESSAGE_MAP(CPageStart, CDialogEx)
	
	ON_BN_CLICKED(IDC_BUTTONxiezai, &CPageStart::OnBnClickedButtonxiezai)
	ON_BN_CLICKED(IDC_BUTTONziqidong, &CPageStart::OnBnClickedButtonziqidong)
END_MESSAGE_MAP()


// CPageStart ��Ϣ�������


BOOL CPageStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listStart.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listStart.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 180);
	m_listStart.InsertColumn(1, _T("�汾��"), LVCFMT_CENTER, 60);
	m_listStart.InsertColumn(2, _T("ж��·��"), LVCFMT_CENTER, 260);
	vector<LPCTSTR> V;
	m_softInfo.GetUninstallPth(V);
	vector<SoftInfo> vecInfo = m_softInfo.GetSoftInfo();
	for (unsigned int i = 0; i < vecInfo.size(); i++)
	{
		m_listStart.InsertItem(i, L"");
		m_listStart.SetItemText(i, 0, vecInfo[i].m_strSoftName);
		m_listStart.SetItemText(i, 1, vecInfo[i].m_strSoftVersion);
		m_listStart.SetItemText(i, 2, vecInfo[i].m_strUninstallPth);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//void CPageStart::OnLvnItemchangedListstart(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CPageStart::OnBnClickedButtonxiezai()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listStart.GetFirstSelectedItemPosition();
	int nIndex = (int)pos - 1;
	CString str = m_listStart.GetItemText(nIndex, 2);
	CStringA temp(str);
	WinExec(temp, 0);
}

//�����ļ�·����ȡ�ļ�����
string CPageStart::getFileNameByFilePath(string filePath) {
	int pos = filePath.find_last_of('\\');
	return filePath.substr(pos + 1);
}
void CPageStart::addToAuto(string filePath) {
	string name = getFileNameByFilePath(filePath);
	createStringReg(HKEY_LOCAL_MACHINE, (char *)"Software\\Microsoft\\Windows\\CurrentVersion\\Run", (char *)name.c_str(), (char *)filePath.c_str());
}

void CPageStart::createStringReg(HKEY hRoot, char *szSubKey, char* valueName, char *Data)
{
	HKEY hKey;
	//��ע�������������򴴽��� 
	long lRet = RegCreateKeyExA(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (lRet != ERROR_SUCCESS)
	{
		MessageBox(L"RegCreateKeyEx error");
		return;
	}

	//�޸�ע����ֵ��û���򴴽��� 
	lRet = RegSetValueExA(hKey, valueName, 0, REG_SZ, (BYTE*)Data, strlen(Data));
	if (lRet != ERROR_SUCCESS)
	{
		MessageBox(L"RegSetValueEx error");
		return;
	}
	RegCloseKey(hKey);
}


string CPageStart::getFilePathByWindows() {
	char szFileName[MAX_PATH] = {};
	OPENFILENAMEA openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.nMaxFile = MAX_PATH;//����������ã������õĻ�������ִ��ļ��Ի���    
	openFileName.lpstrFilter = "��ִ���ļ���*.exe*��\0*.exe\0�κ��ļ���*.*��\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&openFileName))// �����������GetSaveFileName()  
	{
		return openFileName.lpstrFile;
	}
	return "";
}
bool isFile(string path) {
	struct _stat buf = { 0 };
	_stat(path.c_str(), &buf);
	bool b = ((buf.st_mode & _S_IFREG) > 0);
	return b;
}
void CPageStart::OnBnClickedButtonziqidong()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//POSITION pos = m_listStart.GetFirstSelectedItemPosition();
	//int nIndex = (int)pos - 1;
	//vector<SoftInfo> vecInfo = m_softInfo.GetSoftInfo();
	//CString path = vecInfo[nIndex].m_strMainProPath;
	//string temp;
	//memcpy((void*)temp.c_str(), path.GetBuffer(), path.GetLength());
	//int ii = 0;
	//addToAuto(temp);
	string filePath = getFilePathByWindows();//Ҫ��ӿ����������ļ�·��
	
	if (!isFile(filePath)) {
		MessageBox(L"�ļ���ַ��Ч!");
		return;
	}
	addToAuto(filePath);
	MessageBox(L"�ɹ���ӵ���������!");
}
