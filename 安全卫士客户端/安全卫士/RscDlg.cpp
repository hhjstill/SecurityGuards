// RscDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ.h"
#include "RscDlg.h"
#include "afxdialogex.h"
#include "PagePE.h"


// CRscDlg �Ի���

IMPLEMENT_DYNAMIC(CRscDlg, CDialogEx)

CRscDlg::CRscDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGRSC, pParent)
{

}

CRscDlg::~CRscDlg()
{
}

void CRscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTRSC, m_listRsc);
}


BEGIN_MESSAGE_MAP(CRscDlg, CDialogEx)
END_MESSAGE_MAP()


// CRscDlg ��Ϣ�������


BOOL CRscDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listRsc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRsc.InsertColumn(0, _T("��Դ����"), LVCFMT_CENTER, 200);
	m_listRsc.InsertColumn(1, _T("��ԴID"), LVCFMT_CENTER, 100);
	m_listRsc.InsertColumn(2, _T("RVA"), LVCFMT_CENTER, 100);
	m_listRsc.InsertColumn(3, _T("��С"), LVCFMT_CENTER, 100);

	DWORD pRscRva = m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	DWORD pRscFoa = CPagePE::rvaToFoa(m_pNt, pRscRva);
	//�ҵ���Դͷ���ļ��е�λ��.
	IMAGE_RESOURCE_DIRECTORY* pResource = (IMAGE_RESOURCE_DIRECTORY*)(m_pBuff + pRscFoa);
	showResource((LPBYTE)pResource, pResource);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
void CRscDlg::showResource(LPBYTE pRscRoot, IMAGE_RESOURCE_DIRECTORY* pRescDir, DWORD nLevel)
{
	//ͨ��Ŀ¼�ṹ��õ���Դ��ڵ�����
	DWORD dwNumOfEntry = pRescDir->NumberOfIdEntries + pRescDir->NumberOfNamedEntries;
	//���ҵ���ڵ��׵�ַ,����Ŀ¼֮����ǵ�һ�����
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRescDir + 1);
	int testCode = sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
	if (nLevel == 1)
	{
		//�����������Ŀ¼�ǵ�һ��,������һ�����Ϣ
		//1.Ŀ¼��������Ϣ��Ҫ����Դ�������,�ڽ�������ʱ���Ѿ��õ���
		//2.ѭ���������
		
		for (DWORD i = 0; i < dwNumOfEntry; i++)
		{
			//���������Ҫ����ʲô��Ϣ��?
			//��Ҫ��ID,�͵��ڶ���Ŀ¼��ƫ��(��ƫ��Ϊ��Ը�Ŀ¼��ַ��ƫ��)
			//1.ID(�ж�ID�����λ����ַ������͵�),NameIsString����ֶ�ֵΪ1Ϊ�ַ�����
			int nIndex = m_listRsc.GetItemCount();
			m_listRsc.InsertItem(nIndex, L"");
			if (pEntry[i].NameIsString)//�ַ������͵�ID
			{
				IMAGE_RESOURCE_DIR_STRING_U* pName = (IMAGE_RESOURCE_DIR_STRING_U*)(pEntry[i].NameOffset + pRscRoot);
				m_listRsc.SetItemText(nIndex, 0, pName->NameString);
				//printf("%S\n", pName->NameString);
			}
			else//����ID
			{
				char* pIdName[] =
				{
					"",
					"���ָ�루Cursor��",
					"λͼ��Bitmap��",
					"ͼ�꣨Icon��",
					"�˵���Menu��",
					"�Ի���Dialog��",
					"�ַ����б�String Table��",
					"����Ŀ¼��Font Directory��",
					"���壨Font��",
					"��ݼ���Accelerators��",
					"�Ǹ�ʽ����Դ��Unformatted��",
					"��Ϣ�б�Message Table��",
					"���ָ���飨Group Cursor��",
					"",
					"ͼ���飨Group Icon��",
					"",
					"�汾��Ϣ��Version Information��"
				};
				WORD dwId = pEntry[i].Id;
				CString str;
				if (dwId >= _countof(pIdName))
				{
					str.Format(L"%d", dwId);
					m_listRsc.SetItemText(nIndex, 0, str);
					//printf("%d\n", dwId);
				}
				else
				{
					CString tempStr(pIdName[dwId]);
					m_listRsc.SetItemText(nIndex, 0, tempStr);
					//printf("%s\n", pIdName[dwId]);
				}

			}
			//2.ƫ��
			//��λ���ڶ���Ŀ¼���ļ��еĵ�ַ
			IMAGE_RESOURCE_DIRECTORY* p2RscDir = (IMAGE_RESOURCE_DIRECTORY*)(pRscRoot + pEntry[i].OffsetToDirectory);
			showResource(pRscRoot, p2RscDir, 2);
		}


	}
	else if (nLevel == 2)
	{
		//�ڶ����ID��Ϣ����Դ������ID,Ҳ���ַ���������
		for (DWORD i = 0; i < dwNumOfEntry; i++)
		{
			int nIndex = m_listRsc.GetItemCount();
			m_listRsc.InsertItem(nIndex, L"");
			if (pEntry[i].NameIsString)//�ַ�����
			{
				IMAGE_RESOURCE_DIR_STRING_U* pName = (IMAGE_RESOURCE_DIR_STRING_U*)(pEntry[i].NameOffset + (LPBYTE)pRscRoot);
				m_listRsc.SetItemText(nIndex, 1, pName->NameString);
				//printf("\t%S\n", pName->NameString);
			}
			else
			{
				CString str;
				str.Format(L"%d", pEntry[i].Id);
				m_listRsc.SetItemText(nIndex, 1, str);
				//printf("\t%d\n", pEntry[i].Id);
			}
			//�ڶ����ƫ����ϢҲ�Ƕ�λ������Ŀ¼
			IMAGE_RESOURCE_DIRECTORY* p3RscDir = (IMAGE_RESOURCE_DIRECTORY*)(pRscRoot + pEntry[i].OffsetToDirectory);
			showResource(pRscRoot, p3RscDir, 3);
		}
	}
	else if (nLevel == 3)
	{
		//������ID��Ϣ����
		for (DWORD i = 0; i < dwNumOfEntry; i++)
		{
			int nIndex = m_listRsc.GetItemCount();
			IMAGE_RESOURCE_DATA_ENTRY* pData = (IMAGE_RESOURCE_DATA_ENTRY*)(pEntry[i].OffsetToData + pRscRoot);
			m_listRsc.InsertItem(nIndex, L"");
			CString str;
			str.Format(L"%X", pData->OffsetToData);
			m_listRsc.SetItemText(nIndex, 2, str);
			str.Format(L"%dKB", pData->Size);
			m_listRsc.SetItemText(nIndex, 3, str);
			//printf("\tSIZE:%x  \tRVA:%08x\n", pData->Size, pData->OffsetToData);
		}
	}
}