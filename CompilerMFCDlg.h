// CompilerMFCDlg.h : header file
//

#if !defined(AFX_COMPILERMFCDLG_H__D80E07C5_9CE0_4EC7_BD39_A658DE84A4ED__INCLUDED_)
#define AFX_COMPILERMFCDLG_H__D80E07C5_9CE0_4EC7_BD39_A658DE84A4ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCompilerMFCDlg dialog

class CCompilerMFCDlg : public CDialog
{
// Construction
public:
	CCompilerMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompilerMFCDlg)
	enum { IDD = IDD_COMPILERMFC_DIALOG };
	CEdit	m_editOutputFile;
	CEdit	m_editInputFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompilerMFCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCompilerMFCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonBrowse1();
	afx_msg void OnButtonBrowse2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILERMFCDLG_H__D80E07C5_9CE0_4EC7_BD39_A658DE84A4ED__INCLUDED_)
