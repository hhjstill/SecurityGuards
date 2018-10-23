// DlgImp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "DlgImp.h"
#include "afxdialogex.h"
#include "PagePE.h"


// CDlgImp �Ի���

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


// CDlgImp ��Ϣ�������


BOOL CDlgImp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�������б�ı�ͷ
	m_listUP.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listUP.InsertColumn(0, _T("Dll����"), LVCFMT_CENTER, 240);
	m_listUP.InsertColumn(1, _T("OriginalFirstThunk"), LVCFMT_CENTER, 120);
	m_listUP.InsertColumn(2, _T("FirstThunk"), LVCFMT_CENTER, 140);
	m_listDOWN.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listDOWN.InsertColumn(0, _T("API����"), LVCFMT_CENTER, 240);
	m_listDOWN.InsertColumn(1, _T("ThunkRva"), LVCFMT_CENTER, 120);
	m_listDOWN.InsertColumn(2, _T("��ʾ"), LVCFMT_CENTER, 140);
	
	IMAGE_IMPORT_DESCRIPTOR* impDir = (IMAGE_IMPORT_DESCRIPTOR*)(CPagePE::rvaToFoa(m_pNt ,m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress) + m_pBuff);
	IMAGE_THUNK_DATA* Int = (IMAGE_THUNK_DATA*)(CPagePE::rvaToFoa(m_pNt, impDir->OriginalFirstThunk) + m_pBuff);
	int nIndex = 0;
	while (impDir->Name != 0)
	{
		//NAME��һ��RVAֵ
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
				//�ж�����ŷ�ʽ���뻹�����ֵ���
				if (IMAGE_SNAP_BY_ORDINAL(Int->u1.Ordinal)) {
					//��ŵ���
					CString orStr;
					orStr.Format(L"%d", Int->u1.Ordinal & 0xffff);
					m_listDOWN.InsertItem(nIndex2, L"");
					m_listDOWN.SetItemText(nIndex2, 0, orStr);
				}
				else {
					//���Ƶ���
					//impNameΪһ�����ƽṹ�������RVA;
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
		
		//printf("���������:%s\n", pName);
		//IMAGE_THUNK_DATA* Int = NULL;
		//Int = (IMAGE_THUNK_DATA*)(CPagePE::rvaToFoa(m_pNt, impDir->OriginalFirstThunk) + m_pBuff);
		//while (Int->u1.Function)
		//{
		//	//�ж�����ŷ�ʽ���뻹�����ֵ���
		//	if (IMAGE_SNAP_BY_ORDINAL(Int->u1.Ordinal)) {
		//		//��ŵ���
		//		printf("\t���:[%x]\n", Int->u1.Ordinal & 0xFFFF);
		//	}
		//	else {
		//		//���Ƶ���
		//		//impNameΪһ�����ƽṹ�������RVA;
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}





//void CDlgImp::OnLvnItemchangingListimp2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CDlgImp::OnNMClickListimp2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = pNMItemActivate->iItem;
	IMAGE_IMPORT_DESCRIPTOR* impDir = (IMAGE_IMPORT_DESCRIPTOR*)(CPagePE::rvaToFoa(m_pNt, m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress) + m_pBuff);
	impDir += nIndex;
	IMAGE_THUNK_DATA* Int = (IMAGE_THUNK_DATA*)(CPagePE::rvaToFoa(m_pNt, impDir->OriginalFirstThunk) + m_pBuff);
	m_listDOWN.DeleteAllItems();
	int nIndex2 = 0;
	while (Int->u1.Function)
	{
		//�ж�����ŷ�ʽ���뻹�����ֵ���
		if (IMAGE_SNAP_BY_ORDINAL(Int->u1.Ordinal)) {
			//��ŵ���
			CString orStr;
			orStr.Format(L"%d", Int->u1.Ordinal & 0xffff);
			m_listDOWN.InsertItem(nIndex2, L"");
			m_listDOWN.SetItemText(nIndex2, 0, orStr);
		}
		else {
			//���Ƶ���
			//impNameΪһ�����ƽṹ�������RVA;
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
