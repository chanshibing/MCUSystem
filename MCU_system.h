
// MCU_system.h : MCU_system Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMCU_systemApp:
// �йش����ʵ�֣������ MCU_system.cpp
//

class CMCU_systemApp : public CWinAppEx
{
public:
	CMCU_systemApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
};

extern CMCU_systemApp theApp;
