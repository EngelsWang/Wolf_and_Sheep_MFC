
// TestOne.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include"control.h"
#include "resource.h"		// 主符号


// CTestOneApp:
// 有关此类的实现，请参阅 TestOne.cpp
//

class CTestOneApp : public CWinApp
{
public:
	CTestOneApp();

// 重写
public:
	virtual BOOL InitInstance();
	World* w;
	CBitmap bitmap0, bitmap1, bitmap2, bitmap3;
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTestOneApp theApp;

