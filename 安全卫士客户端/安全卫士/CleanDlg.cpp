// CleanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "CleanDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include "MD5.h"
#include <WinSock2.h>
#include <TlHelp32.h>
#pragma comment(lib, "Ws2_32.lib")


// CCleanDlg 对话框

IMPLEMENT_DYNAMIC(CCleanDlg, CDialogEx)

CCleanDlg::CCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGCLEAN, pParent)
	, m_CleanEdit(_T(""))
	, m_postfixEdit(_T(""))
{
}

CCleanDlg::~CCleanDlg()
{
}

void CCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_cleanEdit);
	//  DDX_Text(pDX, IDC_EDIT1, m_editClean);
	DDX_Text(pDX, IDC_EDIT1, m_CleanEdit);
	DDX_Text(pDX, IDC_EDIT2, m_postfixEdit);
	DDX_Control(pDX, IDC_LISTCLEAN, m_listClean);
}


BEGIN_MESSAGE_MAP(CCleanDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CCleanDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON___, &CCleanDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDOK, &CCleanDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONMEM, &CCleanDlg::OnBnClickedButtonmem)
	ON_BN_CLICKED(IDC_BUTTONMD5, &CCleanDlg::OnBnClickedButtonmd5)
	ON_BN_CLICKED(IDC_BUTTONWHITE, &CCleanDlg::OnBnClickedButtonwhite)
	ON_BN_CLICKED(IDC_BUTTONCLOUDE, &CCleanDlg::OnBnClickedButtoncloude)
	ON_BN_CLICKED(IDC_BUTTONDEMO, &CCleanDlg::OnBnClickedButtondemo)
	ON_BN_CLICKED(IDC_BUTTONmd, &CCleanDlg::OnBnClickedButtonmd)
END_MESSAGE_MAP()


// CCleanDlg 消息处理程序


void CCleanDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	EndDialog(0);
}


void CCleanDlg::OnBnClickedButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog fileDlg;
	fileDlg.DoModal();
	//获取文件夹路径
	CString path;
	path = fileDlg.GetPathName();
	if (wcslen(path))
	{
		m_CleanEdit = path;
		UpdateData(FALSE);
	}
}


void CCleanDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	if (!wcslen(m_CleanEdit))
	{
		MessageBox(L"请选择需要清理的文件夹!");
		return;
	}
	//else
	//{
	//	CString str;
	//	str.Format(L"确定要清理:%s", m_CleanEdit);
	//	int nRes = MessageBox(str + L"?", L"警告", MB_YESNO);
	//	if (IDNO == nRes)
	//		return;
	//}
	cleanFile(m_CleanEdit);
	CString str;
	str.Format(L"确定要清理:%s", m_CleanEdit);
	int nRes = MessageBox(str + L"?", L"警告", MB_YESNO);
	if (IDNO == nRes)
		return;
	for (unsigned int i = 0; i < m_vecFileName.size(); i++)
	{
		DeleteFile(m_vecFileName[i]);
	}
	MessageBox(L"清理成功!");
	m_listClean.DeleteAllItems();
}

void CCleanDlg::cleanFile(const CString& path)
{
	WIN32_FIND_DATA wd{};
	MD5_CTX ctx{};
	HANDLE hFile = FindFirstFile(path + "\\*", &wd);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	do {
		if (wcscmp(wd.cFileName, L".") == 0 || wcscmp(wd.cFileName, L"..") == 0)
			continue;
		if (isRubish(wd.cFileName))
		{
			if (!(wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				m_vecFileName.push_back(path + L"\\" + wd.cFileName);
				m_listClean.InsertItem(0, L"");
				m_listClean.SetItemText(0, 0, wd.cFileName);
				CStringA str(path + L"\\" + wd.cFileName);
				unsigned char* pPath = (unsigned char*)str.GetBuffer();
				MD5Init(&ctx);
				MD5Update(&ctx, pPath, str.GetLength());
				MD5Final(&ctx);
				char* resStr = (char*)Hex2ASC(ctx.digest, 16);
				CString temp(resStr);
				m_listClean.SetItemText(0, 1, temp);
			}
			//DeleteFile(path + L"\\" + wd.cFileName);
		}
		if (wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			cleanFile(path + L"\\" + wd.cFileName);
	} while (FindNextFile(hFile, &wd));
}

void CCleanDlg::md5CleanFile(const CString& path, char* md5Str)
{
	MD5_CTX ctx{};
	WIN32_FIND_DATA wd{};
	HANDLE hFile = FindFirstFile(path + "\\*", &wd);
	int md5Num = strlen(md5Str) / 32;
	
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	do {
		if (wcscmp(wd.cFileName, L".") == 0 || wcscmp(wd.cFileName, L"..") == 0)
			continue;
		if (!(wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString szPath = path + "\\" + wd.cFileName;
			CStringA str(szPath);
			int nLen = str.GetLength();
			unsigned char* pPath = new unsigned char[nLen + 1]{};
			for (int i = 0; i < nLen; i++)
			{
				pPath[i] = str[i];
			}
			MD5Init(&ctx);
			MD5Update(&ctx, pPath, nLen);
			MD5Final(&ctx);
			char* resStr = (char*)Hex2ASC(ctx.digest, 16);
			for (int i = 0; i < md5Num; i++)
			{
				char* eachMd5 = new char[33]{};
				memcpy(eachMd5, md5Str + i * 32, 32);
				if (strcmp(resStr, md5Str) == 0)
				{
					CString temp;
					temp.Format(L"发现病毒文件:%s, 是否删除?", path + L"\\" + wd.cFileName);
					int nRes = MessageBox(temp, L"警告:", MB_YESNO);
					if(IDYES == nRes)
						DeleteFile(path + L"\\" + wd.cFileName);
				}
			}
			//printf(Hex2ASC(ctx.digest, 16));
		}
		if (wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			md5CleanFile(path + L"\\" + wd.cFileName,md5Str);
	} while (FindNextFile(hFile, &wd));
}
void CCleanDlg::whiteListCleanFile(const CString& path, char* listStr)
{
	WIN32_FIND_DATA wd{};
	HANDLE hFile = FindFirstFile(path + "\\*", &wd);
	WHITELIST* wl = (WHITELIST*)listStr;

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	do {
		if (wcscmp(wd.cFileName, L".") == 0 || wcscmp(wd.cFileName, L"..") == 0)
			continue;
		if (!(wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString szPath = path + "\\" + wd.cFileName;
			CStringA str(szPath);
			int nLen = str.GetLength();
			char* pPath = str.GetBuffer();
			bool isWhite = false;
			char* pTemp = wl->allPath;
			for (int i = 0; i < wl->nNum; i++)
			{
				
				char* eachPath = new char[strlen(pTemp) + 1]{};
				memcpy(eachPath, pTemp, strlen(pTemp));
				pTemp += strlen(pTemp) + 1;
				if (strcmp(eachPath, str) == 0)
				{
					isWhite = true;
					break;
				}
			}
			if(isWhite == false)
				DeleteFile(path + L"\\" + wd.cFileName);
		}
		if (wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			whiteListCleanFile(path + L"\\" + wd.cFileName, listStr);
	} while (FindNextFile(hFile, &wd));
}
BOOL CCleanDlg::isRubish(const CString& path)
{
	wchar_t tBuf[MAX_PATH]{};

	int nIndex = path.GetLength() - m_postfixEdit.GetLength();
	int i = 0;
	while (i != m_postfixEdit.GetLength())
	{
		tBuf[i] = path[nIndex];
		i++;
		nIndex++;
	}
	if (wcscmp(tBuf, m_postfixEdit) == 0)
		return TRUE;
	return FALSE;
}


void CCleanDlg::OnBnClickedButtonmem()
{
	// TODO: 在此添加控件通知处理程序代码
	MEMORYSTATUSEX stcMemStatusEx{};
	stcMemStatusEx.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	unsigned int preMem = (unsigned int)((stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys) * 100 / stcMemStatusEx.ullTotalPhys);
	
	DWORD dwPIDlist[1000]{};
	DWORD buffSize = sizeof(dwPIDlist);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDlist, buffSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hPro = OpenProcess(PROCESS_SET_QUOTA, FALSE, dwPIDlist[i]);
		SetProcessWorkingSetSize(hPro, -1, -1);
	}
	GlobalMemoryStatusEx(&stcMemStatusEx);
	unsigned int afterMem = (unsigned int)((stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys) * 100 / stcMemStatusEx.ullTotalPhys);
	CString preStr,afterStr;
	preStr.Format(L"%d", preMem);
	afterStr.Format(L"%d", afterMem);
	MessageBox(L"清理前内存占用率:" + preStr + L"%" + L"\r\n" + L"当前内存占用率:" + afterStr + L"%", L"提示:", 0);
}


void CCleanDlg::OnBnClickedButtonmd5()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog fileDlg;
	fileDlg.DoModal();
	//获取文件夹路径
	CString path;
	path = fileDlg.GetPathName();
	if (!path.GetLength())return;
	
	HANDLE hFile = CreateFileA("MD5.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"文件不存在!");
		return;
	}
	DWORD dwSize = GetFileSize(hFile, 0);
	DWORD dwRead = 0;
	char* pBuff = new char[dwSize + 1]{};
	ReadFile(hFile, pBuff, dwSize, &dwRead, 0);
	CloseHandle(hFile);
	md5CleanFile(path, pBuff);
	MessageBox(L"完成清理!", L"提示:", 0);
	//HINSTANCE hDLL = NULL;
	//if ((hDLL = LoadLibrary(L"advapi32.dll")) > 0)
	//{
	//	MD5Init = (PMD5Init)GetProcAddress(hDLL, "MD5Init");
	//	MD5Update = (PMD5Update)GetProcAddress(hDLL, "MD5Update");
	//	MD5Final = (PMD5Final)GetProcAddress(hDLL, "MD5Final");
	//	md5CleanFile(path,pBuff);
	//}
}

bool CCleanDlg::isInWhiteList(char* szName)
{
	FILE* fp = NULL;
	if (fopen_s(&fp, "D:\\c代码\\TSET\\白名单进程\\白名单.txt", "rb"))return true;
	char buf[128]{};
	while (!feof(fp))
	{
		fread(buf, 1, 128, fp);
		if (strcmp(szName, buf) == 0)
		{
			return true;
		}
	}
	fclose(fp);
	fp = NULL;
	return false;
}
void CCleanDlg::OnBnClickedButtonwhite()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFolderPickerDialog fileDlg;
	//fileDlg.DoModal();
	////获取文件夹路径
	//CString path;
	//path = fileDlg.GetPathName();
	//if (!path.GetLength())return;

	//HANDLE hFile = CreateFileA("WhiteList.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (hFile == INVALID_HANDLE_VALUE)
	//{
	//	MessageBox(L"文件不存在!");
	//	return;
	//}
	//DWORD dwSize = GetFileSize(hFile, 0);
	//DWORD dwRead = 0;
	//char* pBuff = new char[dwSize + 1]{};
	//ReadFile(hFile, pBuff, dwSize, &dwRead, 0);
	//CloseHandle(hFile);
	//whiteListCleanFile(path, pBuff);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnap)
	{
		MessageBox(L"创建快照失败!");
		return;
	}
	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };

	BOOL bSuccess = Process32First(hSnap, &pe);
	if (!bSuccess)
	{
		MessageBox(L"找不到第一个进程!");
		return;
	}
	do {
		CStringA temp(pe.szExeFile);
		char buff[128]{};
		memcpy(buff, temp.GetBuffer(), 128);
		if (!isInWhiteList(buff))
		{
			HANDLE hPro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
			if (!hPro)return;
			CString str;
			str.Format(L"即将结束进程(%s)?", pe.szExeFile);
			if(MessageBox(str, L"警告", MB_YESNO) == IDYES)
				TerminateProcess(hPro, 0);
		}
	} while (Process32Next(hSnap, &pe));
	CloseHandle(hSnap);
	
}

void CCleanDlg::mySend(const CString& path, int nType)
{
	MD5_CTX ctx{};
	SENDSTC ss{};
	ss.type = nType;
	WIN32_FIND_DATA wd{};
	HANDLE hFile = FindFirstFile(path + "\\*", &wd);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	do {
		if (wcscmp(wd.cFileName, L".") == 0 || wcscmp(wd.cFileName, L"..") == 0)
			continue;
		if (!(wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString szPath = path + "\\" + wd.cFileName;
			CStringA str(szPath);
			memcpy(ss.filePath, str, str.GetLength());
			int nLen = str.GetLength();
			unsigned char* pPath = new unsigned char[nLen + 1]{};
			for (int i = 0; i < nLen; i++)
			{
				pPath[i] = str[i];
			}
			MD5Init(&ctx);
			MD5Update(&ctx, pPath, nLen);
			MD5Final(&ctx);
			char* resStr = (char*)Hex2ASC(ctx.digest, 16);
			memcpy(ss.md5, resStr, 33);
			//发送
			send(m_sock, (char*)&ss, sizeof(ss), 0);
			//printf(Hex2ASC(ctx.digest, 16));
		}
		if (wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			mySend(path + L"\\" + wd.cFileName, nType);
	} while (FindNextFile(hFile, &wd));
}

void CCleanDlg::OnBnClickedButtoncloude()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_sock)
	{
		MessageBox(L"未连接服务器!");
		return;
	}
	CFolderPickerDialog fileDlg;
	fileDlg.DoModal();
	//获取文件夹路径
	CString path;
	path = fileDlg.GetPathName();
	if (!path.GetLength())return;
	mySend(path, CLOUDEKILL);
}
void CCleanDlg::mySendInfo(CString& path, int nType)
{
	MD5_CTX ctx{};
	SENDSTC ss{};
	ss.type = nType;
	CStringA str(path);
	memcpy(ss.filePath, str, str.GetLength());
	int nLen = str.GetLength();
	unsigned char* pPath = new unsigned char[nLen + 1]{};
	for (int i = 0; i < nLen; i++)
	{
		pPath[i] = str[i];
	}
	MD5Init(&ctx);
	MD5Update(&ctx, pPath, nLen);
	MD5Final(&ctx);
	char* resStr = (char*)Hex2ASC(ctx.digest, 16);
	memcpy(ss.md5, resStr, 32);
	send(m_sock, (char*)&ss, sizeof(ss), 0);
}
void CCleanDlg::OnBnClickedButtondemo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_sock)
	{
		MessageBox(L"未连接服务器!");
		return;
	}
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();
	CString path;
	path = fileDlg.GetPathName();
	if (!path.GetLength())return;
	mySendInfo(path, MD5INFO);
	MessageBox(L"提交样本完成!", L"提示:", 0);
}
DWORD CALLBACK ThreadProc(LPVOID lp)
{
	//接收服务端发来的消息
	while (1)
	{
		SENDSTC ss{};
		recv(SOCKET(lp), (char*)&ss, sizeof(ss), 0);
		if (ss.type == CLOUDEKILL)
		{
			CStringA str;
			str.Format("发现疑似病毒文件:%s!是否删除?", ss.filePath);
			if(MessageBoxA(0, str, "警告!", MB_YESNO) == IDYES)
				DeleteFileA(ss.filePath);
		}
	}
	return 0;
}
BOOL CCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listClean.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listClean.InsertColumn(0, _T("路径"), LVCFMT_CENTER, 250);
	m_listClean.InsertColumn(1, _T("MD5"), LVCFMT_CENTER, 250);
	
	HINSTANCE hDLL = NULL;
	if ((hDLL = LoadLibrary(L"advapi32.dll")) > 0)
	{
		MD5Init = (PMD5Init)GetProcAddress(hDLL, "MD5Init");
		MD5Update = (PMD5Update)GetProcAddress(hDLL, "MD5Update");
		MD5Final = (PMD5Final)GetProcAddress(hDLL, "MD5Final");
	}
	if (InitWinSock() == SOCKET_ERROR)
	{
		MessageBox(L"套接字初始化失败!");
		return FALSE;
	}
	//connectSvr();
	if (conn("127.0.0.1", 9999) == SOCKET_ERROR)
	{
		MessageBox(L"服务器未启动");
		closesocket(m_sock);
		m_sock = NULL;
		WSACleanup();
		return FALSE;
	}
	CreateThread(NULL, 0, ThreadProc, (void*)m_sock, 0, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
BOOL CCleanDlg::InitWinSock() 
{
	WSADATA stcData;
	int nResult = 0;
	nResult = WSAStartup(MAKEWORD(2, 2), &stcData);
	if (nResult == SOCKET_ERROR) {
		return FALSE;
	}
	return TRUE;
}
int CCleanDlg::conn(char* ipAddr, int port)
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	//in_addr s = { 0 };
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ipAddr);
	return connect(m_sock, (sockaddr*)&addr, sizeof(sockaddr_in));
}




void CCleanDlg::OnBnClickedButtonmd()
{
	// TODO: 在此添加控件通知处理程序代码
	MD5_CTX ctx{};
	CFolderPickerDialog dlg;
	dlg.DoModal();
	CString filePath = dlg.GetPathName();
	if (filePath.IsEmpty())
	{
		MessageBox(L"文件路径不能为空!", L"警告:", 0);
		return;
	}
	HANDLE hFile = CreateFileA("md.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(L"文件不存在!", L"警告:", 0);
		return;
	}
	DWORD dwSize = GetFileSize(hFile, 0);
	DWORD dwRead = 0;
	LPVOID pBuff = new BYTE[dwSize + 1];
	ReadFile(hFile, pBuff, dwSize, &dwRead, 0);
	CloseHandle(hFile);
	cmpMd5(filePath,(char*)pBuff);
	MessageBox(L"清理成功!", L"提示:", 0);
}
BOOL CCleanDlg::cmpMd5(const CString& path, char* allMd5)
{
	int md5Num = strlen(allMd5) / 32;
	MD5_CTX ctx{};
	WIN32_FIND_DATA wd{};
	HANDLE hFile = FindFirstFile(path + "\\*", &wd);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do {
			if(wd.cFileName == CString(".") || wd.cFileName == CString(".."))
				continue;
			if (!(wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString temp = path + "\\" + wd.cFileName;
				HANDLE hF = CreateFile(temp, GENERIC_ALL, 0, 0, OPEN_EXISTING, 0, 0);
				DWORD dwSize = GetFileSize(hF, 0);
				unsigned char* readBuf = new unsigned char[dwSize + 1]{};
				DWORD dwRead = 0;
				ReadFile(hF, readBuf, dwSize, &dwRead, 0);
				MD5Init(&ctx);
				MD5Update(&ctx, readBuf, dwSize);
				MD5Final(&ctx);
				char* resStr = (char*)Hex2ASC(ctx.digest, 16);
				for (int i = 0; i < md5Num; i++)
				{
					char* eachMd5 = new char[33]{};
					memcpy(eachMd5, allMd5 + i * 32, 32);
					if (strcmp(resStr, eachMd5) == 0)
					{
						CloseHandle(hF);
						hF = 0;
						DeleteFile(path + L"\\" + wd.cFileName);
						break;
					}
				}
				
				if (hF != NULL)
				{
					CloseHandle(hF);
					//MessageBox(L"未发现病毒文件!", L"提示:", 0);
				}
			}
			if (wd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString temp = path + "\\" + wd.cFileName;
				cmpMd5(temp, allMd5);
			}
		} while (FindNextFile(hFile, &wd));
	}
	return TRUE;
}