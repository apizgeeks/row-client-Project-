// CompilerMFC.h : main header file for the COMPILERMFC application
//

#if !defined(AFX_COMPILERMFC_H__F873C597_051F_4752_8D3D_6716757FB259__INCLUDED_)
#define AFX_COMPILERMFC_H__F873C597_051F_4752_8D3D_6716757FB259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCompilerMFCApp:
// See CompilerMFC.cpp for the implementation of this class
//

class CCompilerMFCApp : public CWinApp
{
public:
	CCompilerMFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompilerMFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCompilerMFCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILERMFC_H__F873C597_051F_4752_8D3D_6716757FB259__INCLUDED_)
