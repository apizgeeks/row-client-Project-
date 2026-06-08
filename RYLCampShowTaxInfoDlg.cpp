
#include <winsock2.h>
#include "WinInput.h"
#include "GMImageDib.h"
#include "RYLImageManager.h"
#include "RYLImageCabinet.h"
#include "RYLImage.h"
#include "RYLSpriteList.h"
#include "RYLButton.h"
#include "RYLLabel.h"
#include "RYLMessageBox.h"
#include "RYLStringTable.h"

#include "RYLCastle.h"
#include "RYLCastleManager.h"
#include <Castle/CastleBlessMgr.h>

#include "RYLCampShowTaxInfoDlg.h"
#include "GMMemory.h"

CRYLCampShowTaxInfoDlg*	g_pRYLCampShowTaxInfoDlg = NULL ;

CRYLCampShowTaxInfoDlg::CRYLCampShowTaxInfoDlg() 
{
	m_pTitleLabel		= NULL ;	// 성의 축복 / 세율 정보
	m_pCastleLabel		= NULL ;	// 축복을 주는 성
	m_pCastleValue		= NULL ;	// 성 이름 (칭호 + 성이름)
	m_pCastleBonusLabel	= NULL ;	// 축복 보너스
	m_pCastleBonusValue	= NULL ;	// 축복 보너스 값
	m_pCastleTaxLabel	= NULL ;	// 요새 상점 수익 세율 (채굴기 광물 세율)
	m_pCastleTaxValue	= NULL ;	// 세율 값
	m_pOKButton			= NULL ;	// 확인 버튼

	m_vecPos = vector3( 0.0f, 0.0f, 0.0f ) ;	// 요새 위치
	m_eCampType			= E_NONE ;				// 요새 타입

	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 15, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 0, 0, 256, 15 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ) ;

	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	AttachCaptionTexture( ( CGMTexture*)pImage ) ;
	GM_DELETE( pImageDIB ) ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 180, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	int i = 0 ;
	for ( ; i < 33 ; i ++ )
	{
		SetRect( &rt, 0, 38, 256, 43 ) ;
		pImageDIB->DrawRect( &rt, 0, ( i * 5 ), CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ) ;
	}

	SetRect( &rt, 0, 42, 256, 52 ) ;
	pImageDIB->DrawRect( &rt, 0, ( ( i - 1 ) * 5 ), CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ) ;

	pImageDIB->MakeFrameRect( 2, 20, 252, 1, 0xff946b42, 0xc0000000,  1 ) ;	

	pImageDIB->MakeFrameRect(  17,  64 - 15, 194, 19, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 173,  94 - 15,  38, 19, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 173, 124 - 15,  38, 19, 0xff959595, 0xc0000000,  1 ) ;

	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	AttachWindowTexture( ( CGMTexture*)pImage ) ;
	GM_DELETE( pImageDIB ) ;

	SetWidth( 256 ) ;
	SetHeight( 191 ) ;
	SetLeft( ( CGMUIScreen::GetInstance()->GetWidth() - GetWidth() ) / 2 ) ;
	SetTop( ( CGMUIScreen::GetInstance()->GetHeight() - GetHeight() ) / 2 ) ;
	SetFormType( RYLDLG_TYPE_CLOSEBUTTON ) ;

	g_pRYLCampShowTaxInfoDlg = this ;
	InitCtrl() ;
}

CRYLCampShowTaxInfoDlg::~CRYLCampShowTaxInfoDlg() 
{
	FinalCtrl() ;
}

VOID	CRYLCampShowTaxInfoDlg::InitCtrl() 
{
	RECT rt ;
	CGMImageDib* pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ;
	CRYLSpriteList* pRYLButtonRect	= CRYLSpriteList::Instance() ;

	// 성의 축복 / 세율 정보
	m_pTitleLabel = new CRYLLabel( this ) ;
	m_pTitleLabel->SetLeft( 11 ) ;
	m_pTitleLabel->SetTop( 18 ) ;
	m_pTitleLabel->SetFontColor (0xffffffff);
	m_pTitleLabel->SetAutoSize( TRUE ) ;
	m_pTitleLabel->SetCaption( CRYLStringTable::m_strString[3994] ) ;
	AttachGMUIControl( m_pTitleLabel ) ;

	// 축복을 주는 성
	m_pCastleLabel = new CRYLLabel( this ) ;
	m_pCastleLabel->SetLeft( 17 ) ;
	m_pCastleLabel->SetTop( 44 ) ;
	m_pCastleLabel->SetFontColor (0xffffffff);
	m_pCastleLabel->SetAutoSize( TRUE ) ;
	m_pCastleLabel->SetCaption( CRYLStringTable::m_strString[3995] ) ;
	AttachGMUIControl( m_pCastleLabel ) ;

	// 성 이름 (칭호 + 성이름)
	m_pCastleValue = new CRYLLabel( this ) ;
	SetRect( &rt, 18, 69, 209, 79 ) ;
	m_pCastleValue->SetAlignRect( rt ) ;
	m_pCastleValue->SetAlign( CENTER ) ;
	m_pCastleValue->SetFontColor( 0xffffffff ) ;
	m_pCastleValue->SetAutoSize( TRUE ) ;
	m_pCastleValue->SetCaption( "---------------------" ) ;
	AttachGMUIControl( m_pCastleValue ) ;

	// 축복 보너스
	m_pCastleBonusLabel = new CRYLLabel( this ) ;
	m_pCastleBonusLabel->SetLeft( 17 ) ;
	m_pCastleBonusLabel->SetTop( 98 ) ;
	m_pCastleBonusLabel->SetFontColor (0xffffffff);
	m_pCastleBonusLabel->SetAutoSize( TRUE ) ;
	m_pCastleBonusLabel->SetCaption( CRYLStringTable::m_strString[3996] ) ;
	AttachGMUIControl( m_pCastleBonusLabel ) ;

	// 축복 보너스 값
	m_pCastleBonusValue = new CRYLLabel( this ) ;
	SetRect( &rt, 174, 99, 209, 109 ) ;
	m_pCastleBonusValue->SetAlignRect( rt ) ;
	m_pCastleBonusValue->SetAlign( CENTER ) ;
	m_pCastleBonusValue->SetFontColor( 0xffffffff ) ;
	m_pCastleBonusValue->SetAutoSize( TRUE ) ;
	m_pCastleBonusValue->SetCaption( "---" ) ;
	AttachGMUIControl( m_pCastleBonusValue ) ;

	// 요새 상점 수익 세율 (채굴기 광물 세율)
	m_pCastleTaxLabel = new CRYLLabel( this ) ;
	m_pCastleTaxLabel->SetLeft( 17 ) ;
	m_pCastleTaxLabel->SetTop( 128 ) ;
	m_pCastleTaxLabel->SetFontColor (0xffffffff);
	m_pCastleTaxLabel->SetAutoSize( TRUE ) ;
	m_pCastleTaxLabel->SetCaption( CRYLStringTable::m_strString[3997] ) ;
	AttachGMUIControl( m_pCastleTaxLabel ) ;

	// 세율 값
	m_pCastleTaxValue = new CRYLLabel( this ) ;
	SetRect( &rt, 174, 129, 209, 139 ) ;
	m_pCastleTaxValue->SetAlignRect( rt ) ;
	m_pCastleTaxValue->SetAlign( CENTER ) ;
	m_pCastleTaxValue->SetFontColor( 0xffffffff ) ;
	m_pCastleTaxValue->SetAutoSize( TRUE ) ;
	m_pCastleTaxValue->SetCaption( "---" ) ;
	AttachGMUIControl( m_pCastleTaxValue ) ;


	pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;

	// 확인 버튼
	m_pOKButton = new CRYLButton( this ) ;
	m_pOKButton->SetLeft( 173 ) ;
	m_pOKButton->SetTop( 154 ) ;
	m_pOKButton->SetCaption( CRYLStringTable::m_strString[ 2164 ] ) ;
	m_pOKButton->SetFontColor( 0xffffffff) ;
	m_pOKButton->SetColor( 0xffffffff) ;
	m_pOKButton->OnClickEvent = DlgClick ;
	m_pOKButton->SetInstance( RYLCAMPSHOWTAXINFODLG::OK_BUTTON ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	AttachGMUIControl( m_pOKButton ) ;
}

VOID	CRYLCampShowTaxInfoDlg::FinalCtrl() 
{
	GM_DELETE( m_pTitleLabel ) ;			// 성의 축복 / 세율 정보
	GM_DELETE( m_pCastleLabel ) ;			// 축복을 주는 성
	GM_DELETE( m_pCastleValue ) ;			// 성 이름 (칭호 + 성이름)
	GM_DELETE( m_pCastleBonusLabel ) ;		// 축복 보너스
	GM_DELETE( m_pCastleBonusValue ) ;		// 축복 보너스 값
	GM_DELETE( m_pCastleTaxLabel ) ;		// 요새 상점 수익 세율 (채굴기 광물 세율)
	GM_DELETE( m_pCastleTaxValue ) ;		// 세율 값
	GM_DELETE( m_pOKButton ) ;				// 확인 버튼
}

VOID	CRYLCampShowTaxInfoDlg::InitValue() 
{
}

VOID	CRYLCampShowTaxInfoDlg::Show( CGMUIObject* Sender ) 
{
}

VOID	CRYLCampShowTaxInfoDlg::Hide( CGMUIObject* Sender ) 
{
	m_eCampType = E_NONE ;
	m_pCastleValue->SetCaption( "---------------------" ) ;
	m_pCastleBonusValue->SetCaption( "---" ) ;
	m_pCastleTaxValue->SetCaption( "---" ) ;
}

VOID	CRYLCampShowTaxInfoDlg::GMUIPaint( INT xx, INT yy ) 
{
	CRYLDialog::GMUIPaint( xx, yy ) ;
}

HRESULT		CRYLCampShowTaxInfoDlg::Render( LPDIRECT3DDEVICE8 lpD3DDevice )  
{
	return S_OK ;
}

HRESULT		CRYLCampShowTaxInfoDlg::FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) 
{
	return S_OK ;
}

HRESULT		CRYLCampShowTaxInfoDlg::Update( BOOL &bClick, BOOL &bEdge ) 
{
	if (FALSE == GetVisible())
	{
		return S_OK;
	}

	if (g_DeviceInput.InRect( GetLeft(), GetTop(), GetLeft() + GetWidth(), GetTop() + GetHeight() ) )
	{
		bEdge = TRUE;

		if(g_DeviceInput.GetIsLeftMouseDown() || g_DeviceInput.GetIsMiddleMouseDown() || g_DeviceInput.GetIsRightMouseDown() ||
			g_DeviceInput.GetIsLeftMousePress() || g_DeviceInput.GetIsMiddleMousePress() || g_DeviceInput.GetIsRightMousePress() ||
			g_DeviceInput.GetIsLeftMouseUp() || g_DeviceInput.GetIsMiddleMouseUp() || g_DeviceInput.GetIsRightMouseUp())
		{
			bClick = TRUE;
		}
	}

	if ( E_NONE != m_eCampType )
	{
		CRYLCastle* pBlessCastle = CRYLCastleManager::Instance()->FindCastleInBlessArea( m_vecPos ) ;
		if ( pBlessCastle )
		{
			char szBuff[ MAX_PATH ] ;
			unsigned char cTitle = g_CastleBlessMgr.GetTitle( pBlessCastle->GetTotalGainTaxCount(), pBlessCastle->GetUpgradeStep() ) ;
			sprintf( szBuff, "%s %s성 +%d", pBlessCastle->GetTitleName( cTitle ), pBlessCastle->GetCastleName(), pBlessCastle->GetUpgradeStep() ) ;
			m_pCastleValue->SetCaption( szBuff ) ;

			unsigned char cBonus = g_CastleBlessMgr.GetBonusPercent( pBlessCastle->GetTotalGainTaxCount(), pBlessCastle->GetUpgradeStep() ) ;
			sprintf( szBuff, "%d%%", cBonus ) ;
			m_pCastleBonusValue->SetCaption( szBuff ) ;

			switch ( m_eCampType )
			{
				case E_CAMP_SHOP :
					sprintf( szBuff, "%d%%", pBlessCastle->GetTax( Castle::CAMP_GOLD_TAX ) ) ;
					m_pCastleTaxValue->SetCaption( szBuff ) ;
					break ;

				case E_MINING_CAMP :
					sprintf( szBuff, "%d%%", pBlessCastle->GetTax( Castle::CAMP_MINERAL_TAX ) ) ;
					m_pCastleTaxValue->SetCaption( szBuff ) ;
					break ;
			}
		}
	}

	return S_OK ;
}

VOID	CRYLCampShowTaxInfoDlg::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pRYLCampShowTaxInfoDlg )
		return ;

	switch ( Sender->GetInstance() )
	{
	case RYLCAMPSHOWTAXINFODLG::OK_BUTTON :
		{
			g_pRYLCampShowTaxInfoDlg->SetVisible( FALSE ) ;
		}
		break ;
	}
}

VOID	CRYLCampShowTaxInfoDlg::SetCampShop( const vector3& vecPos )
{
	m_eCampType = E_CAMP_SHOP ;
	m_vecPos = vecPos ;
	m_pCastleTaxLabel->SetCaption( CRYLStringTable::m_strString[3998] ) ;
}

VOID	CRYLCampShowTaxInfoDlg::SetMiningCamp( const vector3& vecPos )
{
	m_eCampType = E_MINING_CAMP ;
	m_vecPos = vecPos ;
	m_pCastleTaxLabel->SetCaption( CRYLStringTable::m_strString[3999] ) ;
}
