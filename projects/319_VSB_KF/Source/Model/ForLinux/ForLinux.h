
// ForLinux.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CForLinuxApp:
// О реализации данного класса см. ForLinux.cpp
//

class CForLinuxApp : public CWinAppEx
{
public:
	CForLinuxApp();

// Переопределение
	public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CForLinuxApp theApp;