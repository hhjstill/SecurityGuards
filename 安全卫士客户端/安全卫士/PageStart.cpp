// PageStart.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "PageStart.h"
#include "afxdialogex.h"


// CPageStart 对话框

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


// CPageStart 消息处理程序


BOOL CPageStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listStart.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listStart.InsertColumn(0, _T("软件名"), LVCFMT_CENTER, 180);
	m_listStart.InsertColumn(1, _T("版本号"), LVCFMT_CENTER, 60);
	m_listStart.InsertColumn(2, _T("卸载路径"), LVCFMT_CENTER, 260);
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
				  // 异常: OCX 属性页应返回 FALSE
}


//void CPageStart::OnLvnItemchangedListstart(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CPageStart::OnBnClickedButtonxiezai()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listStart.GetFirstSelectedItemPosition();
	int nIndex = (int)pos - 1;
	CString str = m_listStart.GetItemText(nIndex, 2);
	CStringA temp(str);
	WinExec(temp, 0);
}

//根据文件路径获取文件名称
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
	//打开注册表键，不存在则创建它 
	long lRet = RegCreateKeyExA(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (lRet != ERROR_SUCCESS)
	{
		MessageBox(L"RegCreateKeyEx error");
		return;
	}

	//修改注册表键值，没有则创建它 
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
	openFileName.nMaxFile = MAX_PATH;//这个必须设置，不设置的话不会出现打开文件对话框    
	openFileName.lpstrFilter = "可执行文件（*.exe*）\0*.exe\0任何文件（*.*）\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&openFileName))// 如果保存则用GetSaveFileName()  
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
	// TODO: 在此添加控件通知处理程序代码
	//POSITION pos = m_listStart.GetFirstSelectedItemPosition();
	//int nIndex = (int)pos - 1;
	//vector<SoftInfo> vecInfo = m_softInfo.GetSoftInfo();
	//CString path = vecInfo[nIndex].m_strMainProPath;
	//string temp;
	//memcpy((void*)temp.c_str(), path.GetBuffer(), path.GetLength());
	//int ii = 0;
	//addToAuto(temp);
	string filePath = getFilePathByWindows();//要添加开机启动的文件路径
	
	if (!isFile(filePath)) {
		MessageBox(L"文件地址无效!");
		return;
	}
	addToAuto(filePath);
	MessageBox(L"成功添加到开机启动!");
}
