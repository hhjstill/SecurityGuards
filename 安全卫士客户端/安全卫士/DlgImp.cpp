// DlgImp.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "DlgImp.h"
#include "afxdialogex.h"
#include "PagePE.h"


// CDlgImp 对话框

IMPLEMENT_DYNAMIC(CDlgImp, CDialogEx)

CDlgImp::CDlgImp(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_IMP, pParent)
{

}

CDlgImp::~CDlgImp()
{
}

void CDlgImp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTIMP2, m_listUP);
	DDX_Control(pDX, IDC_LISTIMP1, m_listDOWN);
}


BEGIN_MESSAGE_MAP(CDlgImp, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_CLICK, IDC_LISTIMP2, &CDlgImp::OnNMClickListimp2)
END_MESSAGE_MAP()


// CDlgImp 消息处理程序


BOOL CDlgImp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化两个列表的表头
	m_listUP.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listUP.InsertColumn(0, _T("Dll名称"), LVCFMT_CENTER, 240);
	m_listUP.InsertColumn(1, _T("OriginalFirstThunk"), LVCFMT_CENTER, 120);
	m_listUP.InsertColumn(2, _T("FirstThunk"), LVCFMT_CENTER, 140);
	m_listDOWN.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listDOWN.InsertColumn(0, _T("API名称"), LVCFMT_CENTER, 240);
	m_listDOWN.InsertColumn(1, _T("ThunkRva"), LVCFMT_CENTER, 120);
	m_listDOWN.InsertColumn(2, _T("提示"), LVCFMT_CENTER, 140);
	
	IMAGE_IMPORT_DESCRIPTOR* impDir = (IMAGE_IMPORT_DESCRIPTOR*)(CPagePE::rvaToFoa(m_pNt ,m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress) + m_pBuff);
	IMAGE_THUNK_DATA* Int = (IMAGE_THUNK_DATA*)(CPagePE::rvaToFoa(m_pNt, impDir->OriginalFirstThunk) + m_pBuff);
	int nIndex = 0;
	while (impDir->Name != 0)
	{
		//NAME是一个RVA值
		char* pName = (char*)(CPagePE::rvaToFoa(m_pNt, impDir->Name) + m_pBuff);
		CString nameStr(pName);
		m_listUP.InsertItem(nIndex, L"");
		m_listUP.SetItemText(nIndex, 0, nameStr);
		nameStr.Format(L"%X", impDir->OriginalFirstThunk);
		m_listUP.SetItemText(nIndex, 1, nameStr);
		nameStr.Format(L"%X", impDir->FirstThunk);
		m_listUP.SetItemText(nIndex, 2, nameStr);
		if (nIndex == 0)
		{
			int nIndex2 = 0;
			while (Int->u1.Function)
			{
				//判断以序号方式导入还是名字导入
				if (IMAGE_SNAP_BY_ORDINAL(Int->u1.Ordinal)) {
					//序号导入
					CString orStr;
					orStr.Format(L"%d", Int->u1.Ordinal & 0xffff);
					m_listDOWN.InsertItem(nIndex2, L"");
					m_listDOWN.SetItemText(nIndex2, 0, orStr);
				}
				else {
					//名称导入
					//impName为一个名称结构体数组的RVA;
					IMAGE_IMPORT_BY_NAME* impName = (IMAGE_IMPORT_BY_NAME*)Int->u1.AddressOfData;
					impName = (IMAGE_IMPORT_BY_NAME*)(m_pBuff + CPagePE::rvaToFoa(m_pNt, (DWORD)impName));
					CString nameStr(impName->Name);
					m_listDOWN.InsertItem(nIndex2, L"");
					m_listDOWN.SetItemText(nIndex2, 0, nameStr);
					nameStr.Format(L"%x", (DWORD)Int);
					m_listDOWN.SetItemText(nIndex2, 1, nameStr);
					nameStr.Format(L"%d", impName->Hint);
					m_listDOWN.SetItemText(nIndex2, 2, nameStr);
					//printf("\t%s\n", impName->Name);
				}
				Int++;
				nIndex2++;
			}
		}
		
		//printf("导入表名称:%s\n", pName);
		//IMAGE_THUNK_DATA* Int = NULL;
		//Int = (IMAGE_THUNK_DATA*)(CPagePE::rvaToFoa(m_pNt, impDir->OriginalFirstThunk) + m_pBuff);
		//while (Int->u1.Function)
		//{
		//	//判断以序号方式导入还是名字导入
		//	if (IMAGE_SNAP_BY_ORDINAL(Int->u1.Ordinal)) {
		//		//序号导入
		//		printf("\t序号:[%x]\n", Int->u1.Ordinal & 0xFFFF);
		//	}
		//	else {
		//		//名称导入
		//		//impName为一个名称结构体数组的RVA;
		//		IMAGE_IMPORT_BY_NAME* impName = (IMAGE_IMPORT_BY_NAME*)Int->u1.AddressOfData;
		//		impName = (IMAGE_IMPORT_BY_NAME*)(m_pBuff + CPagePE::rvaToFoa(m_pNt, (DWORD)impName));
		//		printf("\t%s\n", impName->Name);
		//	}
		//	Int++;
		//}
		nIndex++;
		impDir++;
	}
	//int nIndex = 0;
	//CString str;
	//m_proListCtrl.DeleteAllItems();
	//if (b)
	//{
	//	do {
	//		str.Format(L"%d", nIndex + 1);
	//		m_proListCtrl.InsertItem(nIndex, _T(""));
	//		m_proListCtrl.SetItemText(nIndex, 0, str);
	//		m_proListCtrl.SetItemText(nIndex, 1, pe.szExeFile);
	//		str.Format(L"%d", pe.th32ProcessID);
	//		m_proListCtrl.SetItemText(nIndex, 2, str);
	//		nIndex++;
	//	} while (Process32Next(hSnap, &pe));
	//}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





//void CDlgImp::OnLvnItemchangingListimp2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CDlgImp::OnNMClickListimp2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = pNMItemActivate->iItem;
	IMAGE_IMPORT_DESCRIPTOR* impDir = (IMAGE_IMPORT_DESCRIPTOR*)(CPagePE::rvaToFoa(m_pNt, m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress) + m_pBuff);
	impDir += nIndex;
	IMAGE_THUNK_DATA* Int = (IMAGE_THUNK_DATA*)(CPagePE::rvaToFoa(m_pNt, impDir->OriginalFirstThunk) + m_pBuff);
	m_listDOWN.DeleteAllItems();
	int nIndex2 = 0;
	while (Int->u1.Function)
	{
		//判断以序号方式导入还是名字导入
		if (IMAGE_SNAP_BY_ORDINAL(Int->u1.Ordinal)) {
			//序号导入
			CString orStr;
			orStr.Format(L"%d", Int->u1.Ordinal & 0xffff);
			m_listDOWN.InsertItem(nIndex2, L"");
			m_listDOWN.SetItemText(nIndex2, 0, orStr);
		}
		else {
			//名称导入
			//impName为一个名称结构体数组的RVA;
			IMAGE_IMPORT_BY_NAME* impName = (IMAGE_IMPORT_BY_NAME*)Int->u1.AddressOfData;
			impName = (IMAGE_IMPORT_BY_NAME*)(m_pBuff + CPagePE::rvaToFoa(m_pNt, (DWORD)impName));
			CString nameStr(impName->Name);
			m_listDOWN.InsertItem(nIndex2, L"");
			m_listDOWN.SetItemText(nIndex2, 0, nameStr);
			nameStr.Format(L"%x", (DWORD)Int);
			m_listDOWN.SetItemText(nIndex2, 1, nameStr);
			nameStr.Format(L"%d", impName->Hint);
			m_listDOWN.SetItemText(nIndex2, 2, nameStr);
			//printf("\t%s\n", impName->Name);
		}
		Int++;
		nIndex2++;
	}
	*pResult = 0;
}
