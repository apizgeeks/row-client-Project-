// CharacterSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DummyClient.h"
#include "CharacterSelectDlg.h"
#include ".\characterselectdlg.h"

#include "MainFrm.h"
#include "GuildManager.h"

#include "CharacterData.h"
#include <Network/ClientSocket/Send/SendAuthServer.h>
#include "GMMemory.h"

// CCharacterSelectDlg dialog

IMPLEMENT_DYNAMIC(CCharacterSelectDlg, CDialog)
CCharacterSelectDlg::CCharacterSelectDlg(CCharacterData& characterData, CWnd* pParent /*=NULL*/)
:   CDialog(CCharacterSelectDlg::IDD, pParent), m_CharacterData(characterData)
{
	
}

CCharacterSelectDlg::~CCharacterSelectDlg()
{
}

void CCharacterSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CTRL_CHARSELECT, m_ctrlCharacterSelect);
}


BEGIN_MESSAGE_MAP(CCharacterSelectDlg, CDialog)
    ON_WM_NCDESTROY()
    ON_NOTIFY(NM_DBLCLK, IDC_CTRL_CHARSELECT, OnNMDblclkCtrlCharselect)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCharacterSelectDlg message handlers

BOOL CCharacterSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlCharacterSelect.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ); 

	m_ctrlCharacterSelect.InsertColumn( 0, _T( "이름" ), LVCFMT_CENTER, 80 ) ;
	m_ctrlCharacterSelect.InsertColumn( 1, _T( "레벨" ), LVCFMT_CENTER, 60 ) ;
	m_ctrlCharacterSelect.InsertColumn( 2, _T( "명성" ), LVCFMT_CENTER, 80 ) ;
	m_ctrlCharacterSelect.InsertColumn( 3, _T( "클래스" ), LVCFMT_CENTER, 80 ) ;
    m_ctrlCharacterSelect.InsertColumn( 4, _T( "UID"), LVCFMT_CENTER, 80);
    m_ctrlCharacterSelect.InsertColumn( 5, _T( "CID"), LVCFMT_CENTER, 80);

	return TRUE ;
}

void CCharacterSelectDlg::InsertData(LPCTSTR lpszName, 
                                     unsigned long dwUID, unsigned long dwCID,
                                     char cLevel, unsigned long dwFame, unsigned short wClass ) 
{
    if(0 != dwUID && 0 != dwCID)
    {
	    int iCnt = m_ctrlCharacterSelect.GetItemCount();
	    CString	str ;

	    m_ctrlCharacterSelect.InsertItem( iCnt, lpszName ) ;
	    str.Format( "%d", cLevel ) ;
	    m_ctrlCharacterSelect.SetItemText( iCnt, 1, str ) ;
	    str.Format( "%d", dwFame ) ;
	    m_ctrlCharacterSelect.SetItemText( iCnt, 2, str ) ;
	    m_ctrlCharacterSelect.SetItemText( iCnt, 3, CGuildManager::GetClassName( wClass ) ) ;

        str.Format( "%d", dwUID ) ;
        m_ctrlCharacterSelect.SetItemText( iCnt, 4, str) ;

        str.Format( "%d", dwCID ) ;
        m_ctrlCharacterSelect.SetItemText( iCnt, 5, str) ;
    }
}

void CCharacterSelectDlg::ClearData()
{
    m_ctrlCharacterSelect.DeleteAllItems();
}

void CCharacterSelectDlg::OnNcDestroy()
{
    CDialog::OnNcDestroy();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    delete this;
}

CCharacterSelectDlg* CCharacterSelectDlg::CreateSelectDlg(CCharacterData& characterData)
{
#include "GMUndefNew.h"
    CCharacterSelectDlg* lpCharSelectDlg =
        new CCharacterSelectDlg(characterData);
#include "GMRedefNew.h"

    if(0 != lpCharSelectDlg)
    {
        lpCharSelectDlg->Create(
            IDD_DLG_CHARSELECT, AfxGetMainWnd());
    }

    return lpCharSelectDlg;
}

void CCharacterSelectDlg::MoveToCenter()
{
    CRect rcDlg, rcMain;

    GetClientRect(&rcDlg);    
    AfxGetMainWnd()->GetClientRect(&rcMain);

    int x = ( rcMain.Width() - rcDlg.Width() ) / 2 ;
    int y = ( rcMain.Height() - rcDlg.Height() ) / 2 ;
    MoveWindow( x, y, rcDlg.Width(), rcDlg.Height() + 20 ) ;
}

void CCharacterSelectDlg::OnNMDblclkCtrlCharselect(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    *pResult = 0;

    // 다이얼로그를 닫고, CharSelect를 전송한다.
    
    if(NM_DBLCLK == pNMHDR->code)
    {
        POSITION pos = m_ctrlCharacterSelect.GetFirstSelectedItemPosition();

        if (pos != NULL)
        {
            while (pos)
            {
                int nItem = m_ctrlCharacterSelect.GetNextSelectedItem(pos);

                CString szUID = m_ctrlCharacterSelect.GetItemText(nItem, 4);
                CString szCID = m_ctrlCharacterSelect.GetItemText(nItem, 5);

                unsigned long dwUID = atol(szUID.GetBuffer(0));
                unsigned long dwCID = atol(szCID.GetBuffer(0));

                m_CharacterData.SetCID(dwCID);

                SendPacket::CharSelect(
                    &m_CharacterData.GetClientSocket().GetHandler(ClientSocket::AuthEventHandler), dwUID, dwCID);

                break;
            }
        }

        OnOK();
    }
}

void CCharacterSelectDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    
    POSITION pos = m_ctrlCharacterSelect.GetFirstSelectedItemPosition();

    if (pos != NULL)
    {
        while (pos)
        {
            int nItem = m_ctrlCharacterSelect.GetNextSelectedItem(pos);
            
            CString szUID = m_ctrlCharacterSelect.GetItemText(nItem, 4);
            CString szCID = m_ctrlCharacterSelect.GetItemText(nItem, 5);

            unsigned long dwUID = atol(szUID.GetBuffer(0));
            unsigned long dwCID = atol(szCID.GetBuffer(0));
            
            m_CharacterData.SetCID(dwCID);

            SendPacket::CharSelect(
                &m_CharacterData.GetClientSocket().GetHandler(ClientSocket::AuthEventHandler), 
                dwUID, dwCID);

            break;
        }
    }

    OnOK();
}
