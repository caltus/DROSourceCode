// GameLauncher.h : main header file for the GAMELAUNCHER application
//

#if !defined(AFX_GAMELAUNCHER_H__0D665E64_0213_41E6_9777_CFAA6F7BB574__INCLUDED_)
#define AFX_GAMELAUNCHER_H__0D665E64_0213_41E6_9777_CFAA6F7BB574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGameLauncherApp:
// See GameLauncher.cpp for the implementation of this class
//

class CGameLauncherApp : public CWinApp
{
public:
	CGameLauncherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameLauncherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGameLauncherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMELAUNCHER_H__0D665E64_0213_41E6_9777_CFAA6F7BB574__INCLUDED_)
