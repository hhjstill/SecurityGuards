// RscDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士.h"
#include "RscDlg.h"
#include "afxdialogex.h"
#include "PagePE.h"


// CRscDlg 对话框

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


// CRscDlg 消息处理程序


BOOL CRscDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	m_listRsc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRsc.InsertColumn(0, _T("资源类型"), LVCFMT_CENTER, 200);
	m_listRsc.InsertColumn(1, _T("资源ID"), LVCFMT_CENTER, 100);
	m_listRsc.InsertColumn(2, _T("RVA"), LVCFMT_CENTER, 100);
	m_listRsc.InsertColumn(3, _T("大小"), LVCFMT_CENTER, 100);

	DWORD pRscRva = m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	DWORD pRscFoa = CPagePE::rvaToFoa(m_pNt, pRscRva);
	//找到资源头在文件中的位置.
	IMAGE_RESOURCE_DIRECTORY* pResource = (IMAGE_RESOURCE_DIRECTORY*)(m_pBuff + pRscFoa);
	showResource((LPBYTE)pResource, pResource);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CRscDlg::showResource(LPBYTE pRscRoot, IMAGE_RESOURCE_DIRECTORY* pRescDir, DWORD nLevel)
{
	//通过目录结构体得到资源入口的数量
	DWORD dwNumOfEntry = pRescDir->NumberOfIdEntries + pRescDir->NumberOfNamedEntries;
	//先找到入口的首地址,紧随目录之后就是第一个入口
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRescDir + 1);
	int testCode = sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
	if (nLevel == 1)
	{
		//如果传进来的目录是第一层,遍历第一层的信息
		//1.目录包含的信息主要是资源入口数量,在进函数的时候已经得到了
		//2.循环遍历入口
		
		for (DWORD i = 0; i < dwNumOfEntry; i++)
		{
			//入口里面主要包含什么信息呢?
			//主要是ID,和到第二层目录的偏移(该偏移为相对根目录地址的偏移)
			//1.ID(判断ID是整形还是字符串类型的),NameIsString这个字段值为1为字符串型
			int nIndex = m_listRsc.GetItemCount();
			m_listRsc.InsertItem(nIndex, L"");
			if (pEntry[i].NameIsString)//字符串类型的ID
			{
				IMAGE_RESOURCE_DIR_STRING_U* pName = (IMAGE_RESOURCE_DIR_STRING_U*)(pEntry[i].NameOffset + pRscRoot);
				m_listRsc.SetItemText(nIndex, 0, pName->NameString);
				//printf("%S\n", pName->NameString);
			}
			else//整型ID
			{
				char* pIdName[] =
				{
					"",
					"鼠标指针（Cursor）",
					"位图（Bitmap）",
					"图标（Icon）",
					"菜单（Menu）",
					"对话框（Dialog）",
					"字符串列表（String Table）",
					"字体目录（Font Directory）",
					"字体（Font）",
					"快捷键（Accelerators）",
					"非格式化资源（Unformatted）",
					"消息列表（Message Table）",
					"鼠标指针组（Group Cursor）",
					"",
					"图标组（Group Icon）",
					"",
					"版本信息（Version Information）"
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
			//2.偏移
			//定位到第二层目录在文件中的地址
			IMAGE_RESOURCE_DIRECTORY* p2RscDir = (IMAGE_RESOURCE_DIRECTORY*)(pRscRoot + pEntry[i].OffsetToDirectory);
			showResource(pRscRoot, p2RscDir, 2);
		}


	}
	else if (nLevel == 2)
	{
		//第二层的ID信息是资源真正的ID,也有字符串和整形
		for (DWORD i = 0; i < dwNumOfEntry; i++)
		{
			int nIndex = m_listRsc.GetItemCount();
			m_listRsc.InsertItem(nIndex, L"");
			if (pEntry[i].NameIsString)//字符串型
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
			//第二层的偏移信息也是定位第三层目录
			IMAGE_RESOURCE_DIRECTORY* p3RscDir = (IMAGE_RESOURCE_DIRECTORY*)(pRscRoot + pEntry[i].OffsetToDirectory);
			showResource(pRscRoot, p3RscDir, 3);
		}
	}
	else if (nLevel == 3)
	{
		//第三层ID信息无用
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