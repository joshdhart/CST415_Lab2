
// CST415_Lab2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCST415_Lab2App:
// See CST415_Lab2.cpp for the implementation of this class
//

class CCST415_Lab2App : public CWinApp
{
public:
	CCST415_Lab2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCST415_Lab2App theApp;