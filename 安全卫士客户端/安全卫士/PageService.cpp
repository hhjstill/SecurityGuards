// PageService.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "PageService.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CPageService 对话框

IMPLEMENT_DYNAMIC(CPageService, CDialogEx)

CPageService::CPageService(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGSERVICE, pParent)
{

}

CPageService::~CPageService()
{
}

void CPageService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTSERVICE, m_listService);
}


BEGIN_MESSAGE_MAP(CPageService, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LISTSERVICE, &CPageService::OnNMRClickListservice)
	ON_COMMAND(ID_32776, &CPageService::OnClickStartSvc)
	ON_COMMAND(ID_32777, &CPageService::OnClickCloseSvc)
END_MESSAGE_MAP()


// CPageService 消息处理程序


BOOL CPageService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listService.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listService.InsertColumn(0, _T("服务名"), LVCFMT_CENTER, 120);
	m_listService.InsertColumn(1, _T("服务状态"), LVCFMT_CENTER, 100);
	m_listService.InsertColumn(2, _T("启动类型"), LVCFMT_CENTER, 100);
	m_listService.InsertColumn(3, _T("路径"), LVCFMT_CENTER, 180);

	showAllService();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CPageService::showAllService()
{
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE);
	
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (PBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);
	for (DWORD i = 0; i < dwServiceNum; i++)
	{

		
		m_listService.InsertItem(0, L"");
		m_listService.SetItemText(0, 0, pEnumService[i].lpServiceName);
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case 1:
			m_listService.SetItemText(0, 1, L"已停止");
			break;
		case 4:
			m_listService.SetItemText(0, 1, L"正在运行");
			break;
		case 7:
			m_listService.SetItemText(0, 1, L"暂停中");
			break;
		default:
			break;
		}
		SC_HANDLE hService = OpenService(hSCM, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		switch (pServiceConfig->dwStartType)
		{
		case 2:
			m_listService.SetItemText(0, 2, L"自启动");
			break;
		case 3:
			m_listService.SetItemText(0, 2, L"手动启动");
			break;
		case 4:
			m_listService.SetItemText(0, 2, L"已禁用");
			break;
		default:
			break;
		}
		m_listService.SetItemText(0, 3, pServiceConfig->lpBinaryPathName);
	}
}

void CPageService::OnNMRClickListservice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu* menu = new CMenu;
	menu->LoadMenu(IDR_MENU2);
	CMenu *pPopup = menu->GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
	CPoint point1;//定义一个用于确定光标位置的位置 
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标 
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);//在指定位置显示弹出菜单
	if (pNMItemActivate->iItem >= 0)
	{
		m_svcName = m_listService.GetItemText(pNMItemActivate->iItem, 0);
		m_svcStatus = m_listService.GetItemText(pNMItemActivate->iItem, 1);
		m_nIndex = pNMItemActivate->iItem;
	}


	//
	//SERVICE_STATUS status;
	//BOOL isSuccess = QueryServiceStatus(service, &status);
	//if (!isSuccess)
	//{
	//	Printf("QueryServiceStatus error!/n");
	//}
	////启动服务
	//if (status.dwCurrentState == SERVICE_STOPPED)
	//{
	//	isSuccess = StartService(service, NULL, NULL);

	//	if (!isSuccess)
	//	{
	//		Printf("启动服务失败！");
	//	}

	//}
	////下面是停止服务的实现代码：

	//	if (status.dwCurrentState != SERVICE_STOPPED)
	//	{
	//		isSuccess = ControlService(service, SERVICE_CONTROL_STOP, &status);
	//		if (!isSuccess)
	//		{
	//			Printf("停止服务失败！");
	//		}
	//	}
	*pResult = 0;
}


void CPageService::OnClickStartSvc()
{
	// TODO: 在此添加命令处理程序代码
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM, m_svcName, SERVICE_ALL_ACCESS);

	BOOL isSuccess = FALSE;
	if(m_svcStatus == CString("已停止"))
		isSuccess = StartService(hService, NULL, NULL);
	if (!isSuccess)
	{
		MessageBox(L"启动服务失败！");
		return;
	}
	m_listService.SetItemText(m_nIndex, 1, L"正在运行");
	
}


void CPageService::OnClickCloseSvc()
{
	// TODO: 在此添加命令处理程序代码
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM, m_svcName, SERVICE_ALL_ACCESS);
	SERVICE_STATUS status;
	BOOL isSuccess = QueryServiceStatus(hService, &status);
	if (m_svcStatus == CString("正在运行"))
		isSuccess = ControlService(hService, SERVICE_CONTROL_STOP, &status);
	if (!isSuccess)
	{
		MessageBox(L"停止服务失败！");
		return;
	}
	m_listService.SetItemText(m_nIndex, 1, L"已停止");
}
