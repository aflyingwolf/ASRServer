
// ASRServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CASRServerApp:
// �йش����ʵ�֣������ ASRServer.cpp
//

class CASRServerApp : public CWinAppEx
{
public:
	CASRServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CASRServerApp theApp;