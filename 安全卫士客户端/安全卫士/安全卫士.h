
// ��ȫ��ʿ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C��ȫ��ʿApp: 
// �йش����ʵ�֣������ ��ȫ��ʿ.cpp
//

class C��ȫ��ʿApp : public CWinApp
{
public:
	C��ȫ��ʿApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C��ȫ��ʿApp theApp;