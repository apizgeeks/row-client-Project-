#include <winsock2.h>
#include "RYLClientMain.h"
#include "RYLStatusDlg.h"
#include "RYLButton.h"
#include "RYLLabel.h"
#include "RYLProgressBar.h"
#include "RYLImageManager.h"
#include "RYLImageCabinet.h"
#include "RYLSpriteList.h"
#include "WinInput.h"
#include "GMUIObject.h"

#include "GMImageDib.h"
#include "RYLImage.h"
#include "RYLStringTable.h"

#include "RYLGameData.h"
#include <RenderOption.h>
#include "RYLStatusExtensionSlotOne.h"
#include "RYLStatusExtensionSlotTwo.h"

#include "RYLGameScene.h"
#include "RYLSceneManager.h"
#include "GMMemory.h"

CRYLStatusDlg*	g_pStatusDlg = NULL ;

CRYLStatusDlg::CRYLStatusDlg( INT nLeft, INT nTop, INT nLevel )	
{
	m_pHideButton		= NULL ;				// Hide 버튼
	m_pExtensionButton	= NULL ;				// 확장 버튼
	m_nExtensionLevel	= 0;
	m_pSlotOne			= NULL;
	m_pSlotTwo			= NULL;

	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;
	
	CRYLSpriteList::Instance()->CreateGMImage( 238, 41, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 0, 0, 147, 41 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI_256x256 ) ) ;
	SetRect( &rt, 0, 41, 90, 81 ) ;
	pImageDIB->DrawRect( &rt, 147, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI_256x256 ) ) ;
	
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	GM_DELETE( pImageDIB ) ;

	AttachWindowTexture( ( CGMTexture*)pImage ) ;
	
	SetWidth( 238 ) ;
	SetHeight( 41 ) ;
	
	SetLeft( nLeft ) ;
	SetTop( nTop ) ;
	m_nHeight = GetHeight();

	SetFormType( RYLDLG_TYPE_NONE ) ;
	
	g_pStatusDlg = this ;	
	InitCtrl() ;

	m_nExtensionLevel = nLevel;
	UpdateExtension();
}

CRYLStatusDlg::~CRYLStatusDlg ()
{
	FinalCtrl() ;
}

VOID	CRYLStatusDlg::InitCtrl ()
{
	RECT rt ;
	CGMImageDib* pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI_256x256 ) ;

	// 레벨
	m_pLevelLabel = new CRYLLabel( this ) ;
	m_pLevelLabel->SetLeft( 212 ) ;
	m_pLevelLabel->SetTop( 26 ) ;
	m_pLevelLabel->SetFontColor (0xffffffff);
	m_pLevelLabel->SetAutoSize( TRUE ) ;
	m_pLevelLabel->SetCaption( "" ) ;
	AttachGMUIControl( m_pLevelLabel ) ;

	// Hide 버튼
	m_pHideButton = new CRYLButton( this ) ;
	m_pHideButton->SetLeft( 224 ) ;
	m_pHideButton->SetTop( 3 ) ;
	m_pHideButton->OnClickEvent = DlgClick ;
	m_pHideButton->SetInstance( RYLSTATUSDLG::HIDE_BUTTON ) ;
	SetRect( &rt, 202, 0, 213, 9 ) ;
	m_pHideButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 202, 18, 213, 27 ) ;
	m_pHideButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 202, 9, 213, 18 ) ;
	m_pHideButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 202, 9, 213, 18 ) ;
	m_pHideButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pHideButton->SetTooltip( &m_TooltipManager, CRYLStringTable::m_strString[ 1868], 1868 ) ;
	AttachGMUIControl( m_pHideButton ) ;
	
	// 확장 버튼
	pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI02_256x256 ) ;
	m_pExtensionButton = new CRYLButton( this ) ;
	m_pExtensionButton->SetLeft( 212 ) ;
	m_pExtensionButton->SetTop( 3 ) ;
	m_pExtensionButton->OnClickEvent = DlgClick ;
	m_pExtensionButton->SetInstance( RYLSTATUSDLG::EXTENSION_BUTTON ) ;
	SetRect( &rt, 153, 126, 163, 135 ) ;
	m_pExtensionButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 153, 135, 163, 144 ) ;
	m_pExtensionButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 153, 144, 163, 153 ) ;
	m_pExtensionButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 153, 144, 163, 153 ) ;
	m_pExtensionButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pExtensionButton->SetTooltip( &m_TooltipManager, CRYLStringTable::m_strString[2307], 1868 ) ;
	AttachGMUIControl( m_pExtensionButton ) ;

	// Exp 프로그레스바
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	m_pExpProgressBar = new CRYLProgressBar( this ) ;
	m_pExpProgressBar->SetLeft( 20 ) ;
	m_pExpProgressBar->SetTop( 35 ) ;
	m_pExpProgressBar->SetWidth( 182 ) ;
	m_pExpProgressBar->SetHeight( 1 ) ;
	m_pExpProgressBar->SetMax( 10 ) ;
	
	CRYLSpriteList::Instance()->CreateGMImage( 182, 1, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt,166, 120, 255, 121 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI02_256x256 ) ) ;
	SetRect( &rt,153, 124, 244, 125 ) ;
	pImageDIB->DrawRect( &rt, 89, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI02_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	m_pExpProgressBar->AttachBarImageTexture( ( CGMTexture*)pImage ) ;
	m_pExpProgressBar->SetPosition( 10 ) ;
	AttachGMUIControl( m_pExpProgressBar ) ;
	GM_DELETE( pImageDIB ) ;

	// HP 프로그레스바
	m_pHPProgressBar = new CRYLProgressBar( this ) ;
	m_pHPProgressBar->SetLeft( 18 ) ;
	m_pHPProgressBar->SetTop( 16 ) ;
	m_pHPProgressBar->SetWidth( 81 ) ;
	m_pHPProgressBar->SetHeight( 13 ) ;
	m_pHPProgressBar->SetMax( 10 ) ;
	
	CRYLSpriteList::Instance()->CreateGMImage( 81, 13, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 166, 106, 247, 119 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI02_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	m_pHPProgressBar->AttachBarImageTexture( ( CGMTexture*)pImage ) ;
	m_pHPProgressBar->SetPosition( 10 ) ;
	AttachGMUIControl( m_pHPProgressBar ) ;
	GM_DELETE( pImageDIB ) ;

	// MP 프로그레스바
	m_pMPProgressBar = new CRYLProgressBar( this ) ;
	m_pMPProgressBar->SetLeft( 120 ) ;
	m_pMPProgressBar->SetTop( 16 ) ;
	m_pMPProgressBar->SetWidth( 81 ) ;
	m_pMPProgressBar->SetHeight( 13 ) ;
	m_pMPProgressBar->SetMax( 10 ) ;
	
	CRYLSpriteList::Instance()->CreateGMImage( 81, 13, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt,166, 92, 247, 105 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI02_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	m_pMPProgressBar->AttachBarImageTexture( ( CGMTexture*)pImage ) ;
	m_pMPProgressBar->SetPosition( 10 ) ;
	AttachGMUIControl( m_pMPProgressBar ) ;
	GM_DELETE( pImageDIB ) ;

	m_pHPLebel = new CRYLLabel( this ) ;
	m_pHPLebel->SetLeft( 22 ) ;
	m_pHPLebel->SetTop( 18 ) ;
	m_pHPLebel->SetFontColor (0xffffffff);
	m_pHPLebel->SetAutoSize( TRUE ) ;
	m_pHPLebel->SetCaption( "" ) ;
	AttachGMUIControl( m_pHPLebel ) ;

	m_pMPLebel = new CRYLLabel( this ) ;
	m_pMPLebel->SetLeft( 125 ) ;
	m_pMPLebel->SetTop( 18 ) ;
	m_pMPLebel->SetFontColor (0xffffffff);
	m_pMPLebel->SetAutoSize( TRUE ) ;
	m_pMPLebel->SetCaption( "" ) ;
	AttachGMUIControl( m_pMPLebel ) ;

	

	// Slot 1
	if( !m_pSlotOne )
		m_pSlotOne = new CRYLStatusExtensionSlotOne( this ) ;
	m_pSlotOne->SetLeft( 0 ) ;
	m_pSlotOne->SetTop( GetHeight() ) ;
	m_pSlotOne->SetWidth( GetWidth() ) ;
	m_pSlotOne->SetHeight( 21 ) ;
	m_pSlotOne->SetVisible( FALSE ) ;
	AttachGMUIControl( m_pSlotOne ) ;


	// Slot 2
	if( !m_pSlotTwo )
		m_pSlotTwo = new CRYLStatusExtensionSlotTwo( this ) ;
	m_pSlotTwo->SetLeft( 0 ) ;
	m_pSlotTwo->SetTop( GetHeight()+m_pSlotOne->GetHeight() ) ;
	m_pSlotTwo->SetWidth( GetWidth() ) ;
	m_pSlotTwo->SetHeight( 21 ) ;
	m_pSlotTwo->SetVisible( FALSE ) ;
	AttachGMUIControl( m_pSlotTwo ) ;
	

}

VOID	CRYLStatusDlg::FinalCtrl ()
{
	GM_DELETE( m_pLevelLabel ) ;
	GM_DELETE( m_pHideButton ) ;
	GM_DELETE( m_pExtensionButton );
	GM_DELETE( m_pHPProgressBar ) ;
	GM_DELETE( m_pMPProgressBar ) ;
	GM_DELETE( m_pExpProgressBar ) ;
	GM_DELETE( m_pHPLebel ) ;
	GM_DELETE( m_pMPLebel ) ;
	GM_DELETE( m_pSlotOne ) ;
	GM_DELETE( m_pSlotTwo ) ;
}

VOID	CRYLStatusDlg::Show( CGMUIObject* Sender )
{
	
}
	
VOID	CRYLStatusDlg::Hide( CGMUIObject* Sender )
{
	
}

VOID	CRYLStatusDlg::GMUIPaint( INT xx, INT yy )
{
	CRYLDialog::GMUIPaint( xx, yy ) ;
}

HRESULT		CRYLStatusDlg::Update( BOOL &bClick, BOOL &bEdge ) 
{
	if (FALSE == GetVisible())
	{
		return S_OK;
	}

	if (g_DeviceInput.InRect( GetLeft(), GetTop(), GetLeft() + GetWidth(), GetTop() + GetHeight() ) )
	{
		bEdge = TRUE;

		if (g_DeviceInput.GetIsLeftMouseDown() || g_DeviceInput.GetIsMiddleMouseDown() || g_DeviceInput.GetIsRightMouseDown() ||
			g_DeviceInput.GetIsLeftMousePress() || g_DeviceInput.GetIsMiddleMousePress() || g_DeviceInput.GetIsRightMousePress() ||
			g_DeviceInput.GetIsLeftMouseUp() || g_DeviceInput.GetIsMiddleMouseUp() || g_DeviceInput.GetIsRightMouseUp())
		{
			bClick = TRUE;
		}
	}
	if( m_pSlotOne->GetVisible() )	((CRYLStatusExtensionSlotOne*)m_pSlotOne)->Update( bClick, bEdge ) ;
	if( m_pSlotTwo->GetVisible() )	((CRYLStatusExtensionSlotTwo*)m_pSlotTwo)->Update( bClick, bEdge ) ;
    

	char			strString[MAX_PATH];
	CRYLGameData*	pGame = CRYLGameData::Instance() ;

	// ------------------------------------------------------------------------------------------
	// HP/MP bar

	sprintf(strString, "%d / %d", 
		static_cast<unsigned short>(pGame->m_csStatus.m_Info.HP), 
		pGame->m_csStatus.GetMaxHP() );
	m_pHPLebel->SetCaption( strString ) ;
	
	sprintf(strString, "%d / %d", 
		static_cast<unsigned short>(pGame->m_csStatus.m_Info.MP), 
		pGame->m_csStatus.GetMaxMP() );
	m_pMPLebel->SetCaption( strString ) ;

	m_pHPProgressBar->SetMax( pGame->m_csStatus.GetMaxHP() ) ;
    if( static_cast<unsigned short>(pGame->m_csStatus.m_Info.HP) < pGame->m_csStatus.GetMaxHP() )
    {   
        m_pHPProgressBar->SetPosition( static_cast<unsigned short>(pGame->m_csStatus.m_Info.HP) ) ;
    }
    else
    { 
        m_pHPProgressBar->SetPosition( pGame->m_csStatus.GetMaxHP() ) ;
    }

	m_pMPProgressBar->SetMax( pGame->m_csStatus.GetMaxMP() ) ;
    if( static_cast<unsigned short>(pGame->m_csStatus.m_Info.MP) < pGame->m_csStatus.GetMaxMP() )
    {   
        m_pMPProgressBar->SetPosition( static_cast<unsigned short>(pGame->m_csStatus.m_Info.MP) ) ;
    }
    else
    { 
        m_pMPProgressBar->SetPosition( pGame->m_csStatus.GetMaxMP() ) ;
    }

	// ------------------------------------------------------------------------------------------
	// EXP bar

	unsigned long dwExp, dwExpMax , type;


	type = 1;
	if (1000000 < pGame->m_csStatus.m_llExpMax && pGame->m_csStatus.m_llExpMax <= 1000000000)
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax / 1000;
		type = 1000;
	} 
	else if (1000000000 < pGame->m_csStatus.m_llExpMax && pGame->m_csStatus.m_llExpMax <= 1000000000000)
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax / 1000000;
		type = 1000000;
	} 
	else if (1000000000000 < pGame->m_csStatus.m_llExpMax && pGame->m_csStatus.m_llExpMax <= 1000000000000000)
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax / 1000000000;
		type = 1000000000;
	} 
	else
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax;
		type = 1;
	}

	dwExp = pGame->m_csStatus.m_Info.Exp / type;

	/*
	if (1000000 < pGame->m_csStatus.m_Info.Exp && pGame->m_csStatus.m_Info.Exp <= 1000000000)
	{
		dwExp = pGame->m_csStatus.m_Info.Exp / 1000;
	} 
	else if (1000000000 < pGame->m_csStatus.m_Info.Exp && pGame->m_csStatus.m_Info.Exp <= 1000000000000)
	{
		dwExp = pGame->m_csStatus.m_Info.Exp / 1000000;
	} 
	else if (1000000000000 < pGame->m_csStatus.m_Info.Exp && pGame->m_csStatus.m_Info.Exp <= 1000000000000000)
	{
		dwExp = pGame->m_csStatus.m_Info.Exp / 1000000000;
	} 
	else
	{
		dwExp = pGame->m_csStatus.m_Info.Exp;
	}
	*/

	m_pExpProgressBar->SetMax( dwExpMax ) ;
	m_pExpProgressBar->SetPosition( dwExp ) ;

	CHAR szBuff[ 8 ] ;
	sprintf( szBuff, "%d", pGame->m_csStatus.m_Info.Level ) ;
	m_pLevelLabel->SetCaption( szBuff ) ;

	return S_OK ;
}

VOID
// EntensionLevel에 따라서 슬롯 (비)활성화 시키는 함수
CRYLStatusDlg::UpdateExtension()
{
	INT		yy, height, dy;
	yy = GetTop() ;
	height = g_ClientMain.m_iScreenHeight ;
	dy = 0;
	
	switch( m_nExtensionLevel )
	{
	case 0 :
		m_pSlotOne->SetVisible( FALSE );
		m_pSlotTwo->SetVisible( FALSE );
		dy = m_nHeight;
		break;
	case 1: 
		m_pSlotOne->SetVisible( TRUE );
		m_pSlotTwo->SetVisible( FALSE );
		dy = m_nHeight + m_pSlotOne->GetHeight() - 1;
		break;
	case 2:
		m_pSlotOne->SetVisible( TRUE );
		m_pSlotTwo->SetVisible( TRUE );
		dy = m_nHeight + m_pSlotOne->GetHeight() + m_pSlotTwo->GetHeight() - 2;
		break;
	default:
		m_pSlotOne->SetVisible( FALSE );
		m_pSlotTwo->SetVisible( FALSE );
		break;
	}
	
	SetHeight( dy ) ;
	if( height < yy + dy )
	{
		SetTop( height - dy  );
	}

}


VOID	CRYLStatusDlg::DlgClick( CGMUIObject* Sender, INT X, INT Y )
{
	if ( !g_pStatusDlg )
		return ;

	switch( Sender->GetInstance() )
	{
		case RYLSTATUSDLG::HIDE_BUTTON :
			{
				g_pStatusDlg->SetVisible( FALSE ) ;
				
				// Visible Flag Setting
				CRYLGameScene* pScene = static_cast<CRYLGameScene*>( CRYLSceneManager::Instance()->GetCurrentScene() ) ;
				if ( pScene ) pScene->SetVisibleInfo( RYLMAINWINDOW::STATUS_WINDOW, FALSE ) ;

			}
			break;
		case RYLSTATUSDLG::EXTENSION_BUTTON :
			{
				g_pStatusDlg->m_nExtensionLevel = ( g_pStatusDlg->m_nExtensionLevel + 1) % 3;
				g_pStatusDlg->UpdateExtension();
			}
			break ;
	}
}