// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DummyClient.h"
#include "LoginDlg.h"
#include ".\logindlg.h"
#include "MainFrm.h"
#include "CharacterSelectDlg.h"
#include "GMMemory.h"

// CLoginDlg dialog

const char* szSetupFileName = "./DummyClient.ini";

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)
CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_szAccount(_T(""))
	, m_szPassword(_T(""))
    , m_szCheckSum(_T(""))
    , m_dwClientVersion(0)
    , m_nLoopCount(0)
{
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_LOGIN, m_szAccount);
    DDX_Text(pDX, IDC_EDT_PASSWORD, m_szPassword);
    DDX_Text(pDX, IDC_IPADDRESS1, m_AuthServerIP);
    DDX_Text(pDX, IDC_EDT_LOGIN2, m_szCheckSum);
    DDX_Text(pDX, IDC_EDT_LOGIN3, m_dwClientVersion);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(ID_BTN_LOGIN, OnBnClickedBtnLogin)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLoginDlg message handlers


//----------------------------------------------------------------------
// Desc : 로그인 처리
//----------------------------------------------------------------------
void CLoginDlg::OnBnClickedBtnExit()
{
	// TODO: Add your control notification handler code here

	
}

//----------------------------------------------------------------------
// Desc : 종료
//----------------------------------------------------------------------
void CLoginDlg::OnBnClickedBtnLogin()
{
	// TODO: Add your control notification handler code here

    // 새 캐릭터 데이터 인스턴스 생성후에, 연결 호출

	
}

void CLoginDlg::MoveToCenter()
{
    CRect rcDlg, rcMain;

    GetClientRect(&rcDlg);    
    AfxGetMainWnd()->GetClientRect(&rcMain);

    int x = ( rcMain.Width() - rcDlg.Width() ) / 2 ;
    int y = ( rcMain.Height() - rcDlg.Height() ) / 2 ;
    MoveWindow( x, y, rcDlg.Width(), rcDlg.Height() + 20 ) ;
}

BOOL CLoginDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.

    // 레지스트리 파일에서 서버주소, 버전, 체크섬을 읽어온다.
    
    m_dwClientVersion = GetPrivateProfileInt(
        "DummyLoginSetup", "ClientVersion", 0, szSetupFileName);

    char szTemp[MAX_PATH];

    GetPrivateProfileString("DummyLoginSetup", "CheckSum", 
        "", szTemp, MAX_PATH - 1, szSetupFileName);

    szTemp[MAX_PATH - 1] = 0;
    m_szCheckSum = szTemp;

    GetPrivateProfileString("DummyLoginSetup", "AuthServerAddress", 
        "", szTemp, MAX_PATH - 1, szSetupFileName);

    szTemp[MAX_PATH - 1] = 0;
    m_AuthServerIP = szTemp;
    
    GetPrivateProfileString("DummyLoginSetup", "Account", 
        "", szTemp, MAX_PATH - 1, szSetupFileName);

    szTemp[MAX_PATH - 1] = 0;
    m_szAccount = szTemp;

    GetPrivateProfileString("DummyLoginSetup", "Password", 
        "", szTemp, MAX_PATH - 1, szSetupFileName);

    szTemp[MAX_PATH - 1] = 0;
    m_szPassword = szTemp;

    MoveToCenter();
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLoginDlg::OnDestroy()
{
    UpdateData(TRUE);

    char szClientVersion[MAX_PATH];
    itoa(m_dwClientVersion, szClientVersion, 10);

    WritePrivateProfileString("DummyLoginSetup", "ClientVersion", szClientVersion, szSetupFileName);
    WritePrivateProfileString("DummyLoginSetup", "CheckSum", m_szCheckSum, szSetupFileName);

    WritePrivateProfileString("DummyLoginSetup", "AuthServerAddress", 
        m_AuthServerIP.GetBuffer(0), szSetupFileName);

    WritePrivateProfileString("DummyLoginSetup", "Account", 
        m_szAccount.GetBuffer(0), szSetupFileName);

    WritePrivateProfileString("DummyLoginSetup", "Password", 
        m_szPassword.GetBuffer(0), szSetupFileName);

    CDialog::OnDestroy();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
