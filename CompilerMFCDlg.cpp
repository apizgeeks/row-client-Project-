// CompilerMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CompilerMFC.h"
#include "CompilerMFCDlg.h"
#include "../ScriptEngine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompilerMFCDlg dialog

CCompilerMFCDlg::CCompilerMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompilerMFCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompilerMFCDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCompilerMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompilerMFCDlg)
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutputFile);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInputFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompilerMFCDlg, CDialog)
	//{{AFX_MSG_MAP(CCompilerMFCDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE1, OnButtonBrowse1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, OnButtonBrowse2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompilerMFCDlg message handlers

BOOL CCompilerMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCompilerMFCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCompilerMFCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCompilerMFCDlg::OnButtonBrowse1() 
{
	static char szFilter[] = "Gama Script File(*.gsf)|*.gsf|All Files(*.*)|*.*||";

	CFileDialog	fileDlg( TRUE, "gsf", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this );

	char szCurPath[1024];
	GetCurrentDirectory( 1024, szCurPath );
	fileDlg.m_ofn.lpstrInitialDir = szCurPath;

	if( fileDlg.DoModal() == IDOK )
	{
		CString inputFile = fileDlg.GetPathName();
		m_editInputFile.SetWindowText( inputFile );

		CString outputFile = inputFile;
		outputFile.MakeLower();
		outputFile.Replace( "gsf", "mcf" );
		
		m_editOutputFile.SetWindowText( outputFile );		
	}
}

void CCompilerMFCDlg::OnButtonBrowse2() 
{
	static char szFilter[] = "Machine Code File(*.mcf)|*.mcf|All Files(*.*)|*.*||";
	
	CFileDialog fileDlg( FALSE, "mcf", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this );

	char szCurPath[1024];
	GetCurrentDirectory( 1024, szCurPath );
	fileDlg.m_ofn.lpstrInitialDir = szCurPath;

	if( fileDlg.DoModal() == IDOK )
	{
		m_editOutputFile.SetWindowText( fileDlg.GetPathName() );
	}
}

void	ScriptMessage( const char * msg )
{
	MessageBox( NULL, msg, "Error!!", MB_OK );
}

void CCompilerMFCDlg::OnOK() 
{
	CString inputFile, outputFile;
	m_editInputFile.GetWindowText( inputFile );
	m_editOutputFile.GetWindowText( outputFile );

	if( inputFile == "" )
	{
		MessageBox( "gsf 화일을 입력해주세요." );
		return;
	}

	if( outputFile == "" )
	{
		MessageBox( "mcf 화일을 입력해주세요." );
		return;
	}

	_SE_SetMessageFunction( ScriptMessage );

	SCRIPT Script;
	Script = _SE_Create( inputFile );
	
	_SE_Save( Script, outputFile );
	_SE_Destroy( Script );

	MessageBox( "Done!!" );
}
