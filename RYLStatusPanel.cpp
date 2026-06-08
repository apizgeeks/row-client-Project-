#include <winsock2.h>
#include "RYLStatusPanel.h"
#include "RYLButton.h"
#include "RYLLabel.h"
#include "RYLSpriteEX.h"

#include "RYLGameData.h"
#include "RYLStringTable.h"

#include "RYLImage.h"
#include "RYLImageManager.h"
#include "GMImageDib.h"
#include "RYLImageCabinet.h"
#include "RYLSpriteList.h"
#include "RYLSceneManager.h"
#include "RYLRankingDlg.h"
#include "RYLGameScene.h"
#include "RYLGuildFrameDlg.h"
#include "RYLGuildInfoPanel.h"

#include "RYLStatusDlg.h"
#include "RYLNetworkData.h"


#include <Network/ClientSocket/Send/SendCommunity.h>
#include <Network/ClientSocket/Send/SendEtc.h>
#include "GMMemory.h"

CRYLStatusPanel*	g_pStatusPanel = NULL ;

CRYLStatusPanel::CRYLStatusPanel( CGMUICustomDlg *pParentDlg ) : CGMUICustomPanel( pParentDlg )
{
	m_pGMUIParentDlg = pParentDlg ;

	m_pNameLabel					= NULL ;	// 이름	
	m_pNameValueLabel				= NULL ;	// 이름 값
	m_pGuildLabel					= NULL ;	// 길드
	m_pGuildValueLabel				= NULL ;	// 길드 값
	m_pClassLabel					= NULL ;	// 클레스
	m_pClassValueLabel				= NULL ;	// 클레스 값
	m_pMarkLabek					= NULL ;	// 마크
	m_pLevelLabel					= NULL ;	// 레벨
	m_pLevelValueLabel				= NULL ;	// 레벨 값
	m_pExperienceLabel				= NULL ;	// 경험점
	m_pExperienceBalueLabel			= NULL ;	// 경험점 값
	m_pFameLabel					= NULL ;	// 명성
	m_pFameValueLabel				= NULL ;	// 명성 값
	m_pServiceMedalLabel			= NULL ;	// 공헌 매달
	m_pServiceMedalValueLabel		= NULL ;	// 공헌 매달 값
	m_pScoreLabel					= NULL ;	// 스코어
	m_pScoreValueLabel				= NULL ;	// 스코어 값
	m_pSTRLabel						= NULL ;	// STR
	m_pSTRValueLabel				= NULL ;	// STR 값
	m_pDEXLabel						= NULL ;	// DEX
	m_pDEXValueLabel				= NULL ;	// DEX 값
	m_pCONLabel						= NULL ;	// CON
	m_pCONValueLabel				= NULL ;	// CON 값
	m_pINTLabel						= NULL ;	// INT
	m_pINTValueLabel				= NULL ;	// INT 값
	m_pWISLabel						= NULL ;	// WIS
	m_pWISValueLabel				= NULL ;	// WIS 값

	m_pStrikingPowerLabel			= NULL ;	// 공격력		
	m_pStrikingPowerValueLabel		= NULL ;	// 공력력 값
	m_pDefensivePowerLabel			= NULL ;	// 방어력
	m_pDefensivePowerValueLabel		= NULL ;	// 방어력 값
	m_pArmorRateValueLabel			= NULL ;	// 방어율 값

	m_pHPLabel						= NULL ;	// HP
	m_pNowHPValueLabel				= NULL ;	// 현재 HP 값
	m_pMaxHPValueLabel				= NULL ;	// 최대 HP 값

	m_pMPLabel						= NULL ;	// MP
	m_pNowMPValueLabel				= NULL ;	// 현재 MP 값
	m_pMaxMPValueLabel				= NULL ;	// 최대 MP 값

	m_pBonusPointLabel				= NULL ;	// 보너스 포인트
	m_pBonusPointValueLabel			= NULL ;	// 보너스 포인트 값

	m_pHitRatioLabel				= NULL ;	// 명중
	m_pHitRatioValueLabel			= NULL ;	// 명중 값		
	m_pEvasionLabel					= NULL ;	// 회피
	m_pEvasionValueLabel			= NULL ;	// 회피 값

	m_pMagicPowerLabel				= NULL ;	// 마법력
	m_pMagicPowerValueLabel			= NULL ;	// 마법력 값

	m_pBlockPowerLabel				= NULL ;	// 블록률
	m_pBlockPowerValueLabel			= NULL ;	// 블록률 값

	m_pCriticalLabel				= NULL ;	// 크리티컬
	m_pCriticalValueLabel			= NULL ;	// 크리티컬 값

	m_pMagicResistLabel				= NULL ;	// 마법저항
	m_pMagicResistValueLabel		= NULL ;	// 마법저항 값

	m_pRankingButton				= NULL ;	// 랭킹 버튼
	m_pHPMPButton					= NULL ;	// mp / hp;
	m_pEnchantButton				= NULL ;	// 인첸트 창

	m_pNationLabel					= NULL ;	// 국적
	m_pNationValueLabel				= NULL ;	// 국적 값

	m_pGuildMarkSprite				= NULL ;	// 길드 마크


	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;
	RECT			rt ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 395, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	int i = 0;
	for (  ; i < 74 ; i ++ )
	{
		SetRect( &rt, 0, 38, 256, 43 ) ;
		pImageDIB->DrawRect( &rt, 0, 1 + (i * 5), CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ) ;
	}
	SetRect( &rt, 0, 42, 256, 52 ) ;
	pImageDIB->DrawRect( &rt, 0, ( 1 +(i-1) * 5 ), CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ) ;
	
	pImageDIB->MakeFrameRect( 7,     2, 243, 146, 0xffB99E6B, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 7,   153, 243, 109, 0xffB99E6B, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 7,   265, 243,  69, 0xffB99E6B, 0xc0000000,  1 ) ;

	for ( i = 0 ; i < 3 ; i ++ )
	{
		pImageDIB->MakeFrameRect( 67, 6 + ( i * 20 ), 101, 19, 0xff959595, 0xc0000000,  1 ) ;
		pImageDIB->MakeFrameRect( 67, 86 + ( i * 20 ), 101, 19, 0xff959595, 0xc0000000,  1 ) ;
	}
	pImageDIB->MakeFrameRect( 205, 6, 37, 19, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 67, 66, 175, 19, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 205, 26, 37, 19, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 205, 46, 37, 19, 0xff959595, 0xc0000000,  1 ) ;

	for ( i = 0 ; i < 4 ; i ++ )
	{
		pImageDIB->MakeFrameRect( 48, 156 + ( i * 21 ), 37, 19, 0xff959595, 0xc0000000,  1 ) ;
		if( i != 1 )
			pImageDIB->MakeFrameRect( 159, 156+ ( i * 21 ), 85, 19, 0xff959595, 0xc0000000,  1 ) ;
	}
	pImageDIB->MakeFrameRect( 159, 177, 42, 18, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 203, 177, 41, 18, 0xff959595, 0xc0000000,  1 ) ;

	pImageDIB->MakeFrameRect( 48, 240, 37, 19, 0xff959595, 0xc0000000,  1 ) ;
	pImageDIB->MakeFrameRect( 207, 240, 37, 19, 0xff959595, 0xc0000000,  1 ) ;

	for ( i = 0 ; i < 3 ; i ++ )
	{
		pImageDIB->MakeFrameRect( 65, 269+ ( i * 21 ), 37, 19, 0xff959595, 0xc0000000,  1 ) ;
		pImageDIB->MakeFrameRect( 164, 269+ ( i * 21 ), 78, 19, 0xff959595, 0xc0000000,  1 ) ;
	}

	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	
	AttachPanelImage( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;
	
	SetWidth( 256 ) ;
	SetHeight( 395 ) ;

	g_pStatusPanel	= this ;
}

CRYLStatusPanel::~CRYLStatusPanel() 
{
	FinalCtrl() ;
}

VOID	CRYLStatusPanel::InitCtrl() 
{
	RECT rt ;
	// 이름
	m_pNameLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pNameLabel->SetLeft( 39 ) ;
	m_pNameLabel->SetTop( 10 ) ;
	m_pNameLabel->SetFontColor (0xffffffff);
	m_pNameLabel->SetCaption( CRYLStringTable::m_strString[ 1926 ] ) ;
	SetRect( &rt, 14, 10, 63, 22 ) ;
	m_pNameLabel->SetAlignRect( rt ) ;
	m_pNameLabel->SetAlign( RIGHT ) ;
	m_pNameLabel->SetAutoSize( TRUE ) ;
	m_pNameLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager,  CRYLStringTable::m_strString[ 1792 ], 1792, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pNameLabel ) ;

	// 이름 값
	m_pNameValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pNameValueLabel->SetLeft( 78 ) ;
	m_pNameValueLabel->SetTop( 10 ) ;
	m_pNameValueLabel->SetFontColor (0xffffffff);
	m_pNameValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 10, 166, 22 ) ;
	m_pNameValueLabel->SetAlignRect( rt ) ;
	m_pNameValueLabel->SetAlign( CENTER ) ;
	m_pNameValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pNameValueLabel ) ;

	// 국적
	m_pNationLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pNationLabel->SetLeft( 179 ) ;
	m_pNationLabel->SetTop( 10 ) ;
	m_pNationLabel->SetFontColor (0xffffffff);
	m_pNationLabel->SetCaption( CRYLStringTable::m_strString[ 2022 ] ) ;
	SetRect( &rt, 180, 10, 202, 22 ) ;
	m_pNationLabel->SetAlignRect( rt ) ;
	m_pNationLabel->SetAlign( RIGHT ) ;
	m_pNationLabel->SetAutoSize( TRUE ) ;
	m_pNationLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 2278 ], 2278, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pNationLabel ) ;

	// 국적 값
	m_pNationValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pNationValueLabel->SetLeft( 216 ) ;
	m_pNationValueLabel->SetTop( 10 ) ;
	m_pNationValueLabel->SetFontColor (0xffffffff);
	m_pNationValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 206, 10, 241, 22 ) ;
	m_pNationValueLabel->SetAlignRect( rt ) ;
	m_pNationValueLabel->SetAlign( CENTER ) ;
	m_pNationValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pNationValueLabel ) ;

	// 길드
	m_pGuildLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pGuildLabel->SetLeft( 39 ) ;
	m_pGuildLabel->SetTop( 30 ) ;
	m_pGuildLabel->SetFontColor (0xffffffff);
	m_pGuildLabel->SetCaption( CRYLStringTable::m_strString[ 1927 ] ) ;
	SetRect( &rt, 14, 30, 63, 42 ) ;
	m_pGuildLabel->SetAlignRect( rt ) ;
	m_pGuildLabel->SetAlign( RIGHT ) ;
	m_pGuildLabel->SetAutoSize( TRUE ) ;
	m_pGuildLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1793 ], 1793, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pGuildLabel ) ;

	// 길드 값
	m_pGuildValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pGuildValueLabel->SetLeft( 78 ) ;
	m_pGuildValueLabel->SetTop( 30 ) ;
	m_pGuildValueLabel->SetFontColor (0xffffffff);
	m_pGuildValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 30, 166, 42 ) ;
	m_pGuildValueLabel->SetAlignRect( rt ) ;
	m_pGuildValueLabel->SetAlign( CENTER ) ;
	m_pGuildValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pGuildValueLabel ) ;

	// 클레스
	m_pClassLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pClassLabel->SetLeft( 27 ) ;
	m_pClassLabel->SetTop( 50 ) ;
	m_pClassLabel->SetFontColor (0xffffffff);
	m_pClassLabel->SetCaption( CRYLStringTable::m_strString[ 1928 ]	) ;
	SetRect( &rt, 14, 50, 63, 62 ) ;
	m_pClassLabel->SetAlignRect( rt ) ;
	m_pClassLabel->SetAlign( RIGHT ) ;
	m_pClassLabel->SetAutoSize( TRUE ) ;
	m_pClassLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1795 ], 1795, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pClassLabel ) ;

	// 클레스 값
	m_pClassValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pClassValueLabel->SetLeft( 78 ) ;
	m_pClassValueLabel->SetTop( 50 ) ;
	m_pClassValueLabel->SetFontColor (0xffffffff);
	m_pClassValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 50, 166, 62 ) ;
	m_pClassValueLabel->SetAlignRect( rt ) ;
	m_pClassValueLabel->SetAlign( CENTER ) ;
	m_pClassValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pClassValueLabel ) ;

	// 마크
	m_pMarkLabek = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMarkLabek->SetLeft( 179 ) ;
	m_pMarkLabek->SetTop( 30 ) ;
	m_pMarkLabek->SetFontColor (0xffffffff);
	m_pMarkLabek->SetCaption( CRYLStringTable::m_strString[ 1929 ] ) ;
	SetRect( &rt, 180, 30, 202, 42 ) ;
	m_pMarkLabek->SetAlignRect( rt ) ;
	m_pMarkLabek->SetAlign( RIGHT ) ;
	m_pMarkLabek->SetAutoSize( TRUE ) ;
	m_pMarkLabek->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1794 ], 1794, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pMarkLabek ) ;

	// 레벨
	m_pLevelLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pLevelLabel->SetLeft( 179 ) ;
	m_pLevelLabel->SetTop( 50 ) ;
	m_pLevelLabel->SetFontColor (0xffffffff);
	m_pLevelLabel->SetCaption( CRYLStringTable::m_strString[ 1930 ] ) ;
	SetRect( &rt, 180, 50, 202, 62 ) ;
	m_pLevelLabel->SetAlignRect( rt ) ;
	m_pLevelLabel->SetAlign( RIGHT ) ;
	m_pLevelLabel->SetAutoSize( TRUE ) ;
	m_pLevelLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1796 ], 1796, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pLevelLabel ) ;

	// 레벨 값
	m_pLevelValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pLevelValueLabel->SetLeft( 211 ) ;
	m_pLevelValueLabel->SetTop( 50 ) ;
	m_pLevelValueLabel->SetFontColor (0xffffffff);
	m_pLevelValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 206, 50, 241, 62 ) ;
	m_pLevelValueLabel->SetAlignRect( rt ) ;
	m_pLevelValueLabel->SetAlign( CENTER ) ;
	m_pLevelValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pLevelValueLabel ) ;

	// 경험점
	m_pExperienceLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pExperienceLabel->SetLeft( 27 ) ;
	m_pExperienceLabel->SetTop( 70 ) ;
	m_pExperienceLabel->SetFontColor (0xffffffff);
	m_pExperienceLabel->SetCaption( CRYLStringTable::m_strString[ 1931 ] ) ;
	SetRect( &rt, 14, 70, 63, 82 ) ;
	m_pExperienceLabel->SetAlignRect( rt ) ;
	m_pExperienceLabel->SetAlign( RIGHT ) ;
	m_pExperienceLabel->SetAutoSize( TRUE ) ;
	m_pExperienceLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1797 ], 1797, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pExperienceLabel ) ;

	// 경험점 값
	m_pExperienceBalueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pExperienceBalueLabel->SetLeft( 78 ) ;
	m_pExperienceBalueLabel->SetTop( 70 ) ;
	m_pExperienceBalueLabel->SetFontColor (0xffffffff);
	m_pExperienceBalueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 70, 241, 82 ) ;
	m_pExperienceBalueLabel->SetAlignRect( rt ) ;
	m_pExperienceBalueLabel->SetAlign( CENTER ) ;
	m_pExperienceBalueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pExperienceBalueLabel ) ;

	// 명성
	m_pFameLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pFameLabel->SetLeft( 39 ) ;
	m_pFameLabel->SetTop( 90 ) ;
	m_pFameLabel->SetFontColor (0xffffffff);
	m_pFameLabel->SetCaption( CRYLStringTable::m_strString[ 1932 ] ) ;
	SetRect( &rt, 14, 90, 63, 102 ) ;
	m_pFameLabel->SetAlignRect( rt ) ;
	m_pFameLabel->SetAlign( RIGHT ) ;
	m_pFameLabel->SetAutoSize( TRUE ) ;
	m_pFameLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1798 ], 1798, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pFameLabel ) ;

	// 명성 값
	m_pFameValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pFameValueLabel->SetLeft( 78 ) ;
	m_pFameValueLabel->SetTop( 90 ) ;
	m_pFameValueLabel->SetFontColor (0xffffffff);
	m_pFameValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 90, 166, 102 ) ;
	m_pFameValueLabel->SetAlignRect( rt ) ;
	m_pFameValueLabel->SetAlign( CENTER ) ;
	m_pFameValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pFameValueLabel ) ;

	// 공헌 메달
	m_pServiceMedalLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pServiceMedalLabel->SetLeft( 15 ) ;
	m_pServiceMedalLabel->SetTop( 110 ) ;
	m_pServiceMedalLabel->SetFontColor (0xffffffff);
	m_pServiceMedalLabel->SetCaption( CRYLStringTable::m_strString[ 1933 ] ) ;
	SetRect( &rt, 14, 110, 63, 122 ) ;
	m_pServiceMedalLabel->SetAlignRect( rt ) ;
	m_pServiceMedalLabel->SetAlign( RIGHT ) ;
	m_pServiceMedalLabel->SetAutoSize( TRUE ) ;
	m_pServiceMedalLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1799 ], 1799, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pServiceMedalLabel ) ;

	// 공헌 메달 값
	m_pServiceMedalValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pServiceMedalValueLabel->SetLeft( 78 ) ;
	m_pServiceMedalValueLabel->SetTop( 110 ) ;
	m_pServiceMedalValueLabel->SetFontColor (0xffffffff);
	m_pServiceMedalValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 110, 166, 122 ) ;
	m_pServiceMedalValueLabel->SetAlignRect( rt ) ;
	m_pServiceMedalValueLabel->SetAlign( CENTER ) ;
	m_pServiceMedalValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pServiceMedalValueLabel ) ;

	// 스코어
	m_pScoreLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pScoreLabel->SetLeft( 27 ) ;
	m_pScoreLabel->SetTop( 130 ) ;
	m_pScoreLabel->SetFontColor (0xffffffff);
	m_pScoreLabel->SetCaption( CRYLStringTable::m_strString[ 1934 ] ) ;
	SetRect( &rt, 14, 130, 63, 142 ) ;
	m_pScoreLabel->SetAlignRect( rt ) ;
	m_pScoreLabel->SetAlign( RIGHT ) ;
	m_pScoreLabel->SetAutoSize( TRUE ) ;
	m_pScoreLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1800 ], 1800, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pScoreLabel ) ;

	// 스코어 값
	m_pScoreValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pScoreValueLabel->SetLeft( 78 ) ;
	m_pScoreValueLabel->SetTop( 130 ) ;
	m_pScoreValueLabel->SetFontColor (0xffffffff);
	m_pScoreValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 68, 130, 166, 142 ) ;
	m_pScoreValueLabel->SetAlignRect( rt ) ;
	m_pScoreValueLabel->SetAlign( CENTER ) ;
	m_pScoreValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pScoreValueLabel ) ;

	// STR
	m_pSTRLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pSTRLabel->SetLeft( 21 ) ;
	m_pSTRLabel->SetTop( 160 ) ;
	m_pSTRLabel->SetFontColor (0xffffffff);
	m_pSTRLabel->SetCaption( "STR" ) ;
	SetRect( &rt, 14, 160, 44, 172 ) ;
	m_pSTRLabel->SetAlignRect( rt ) ;
	m_pSTRLabel->SetAlign( RIGHT ) ;
	m_pSTRLabel->SetAutoSize( TRUE ) ;
	m_pSTRLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1801 ], 1801, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pSTRLabel ) ;

	// STR 값
	m_pSTRValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pSTRValueLabel->SetLeft( 54 ) ;
	m_pSTRValueLabel->SetTop( 160 ) ;
	m_pSTRValueLabel->SetFontColor (0xffffffff);
	m_pSTRValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 49, 160, 83, 172 ) ;
	m_pSTRValueLabel->SetAlignRect( rt ) ;
	m_pSTRValueLabel->SetAlign( CENTER ) ;
	m_pSTRValueLabel->SetAutoSize( TRUE ) ;
	m_pSTRValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1801 ], 1801, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pSTRValueLabel ) ;

	// DEX
	m_pDEXLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pDEXLabel->SetLeft( 20 ) ;
	m_pDEXLabel->SetTop( 181 ) ;
	m_pDEXLabel->SetFontColor (0xffffffff);
	m_pDEXLabel->SetCaption( "DEX" ) ;
	SetRect( &rt, 14, 181, 44, 193 ) ;
	m_pDEXLabel->SetAlignRect( rt ) ;
	m_pDEXLabel->SetAlign( RIGHT ) ;
	m_pDEXLabel->SetAutoSize( TRUE ) ;
	m_pDEXLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1802 ], 1802, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pDEXLabel ) ;

	// DEX 값
	m_pDEXValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pDEXValueLabel->SetLeft( 54 ) ;
	m_pDEXValueLabel->SetTop( 181 ) ;
	m_pDEXValueLabel->SetFontColor (0xffffffff);
	m_pDEXValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 49, 181, 83, 193 ) ;
	m_pDEXValueLabel->SetAlignRect( rt ) ;
	m_pDEXValueLabel->SetAlign( CENTER ) ;
	m_pDEXValueLabel->SetAutoSize( TRUE ) ;
	m_pDEXValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1802 ], 1802, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pDEXValueLabel ) ;

	// CON
	m_pCONLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pCONLabel->SetLeft( 18 ) ;
	m_pCONLabel->SetTop( 202 ) ;
	m_pCONLabel->SetFontColor (0xffffffff);
	m_pCONLabel->SetCaption( "CON" ) ;
	SetRect( &rt, 14, 202, 44, 214 ) ;
	m_pCONLabel->SetAlignRect( rt ) ;
	m_pCONLabel->SetAlign( RIGHT ) ;
	m_pCONLabel->SetAutoSize( TRUE ) ;
	m_pCONLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1803 ], 1803, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pCONLabel ) ;

	// CON 값
	m_pCONValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pCONValueLabel->SetLeft( 54 ) ;
	m_pCONValueLabel->SetTop( 202 ) ;
	m_pCONValueLabel->SetFontColor (0xffffffff);
	m_pCONValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 49, 202, 83, 214 ) ;
	m_pCONValueLabel->SetAlignRect( rt ) ;
	m_pCONValueLabel->SetAlign( CENTER ) ;
	m_pCONValueLabel->SetAutoSize( TRUE ) ;
	m_pCONValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1803 ], 1803, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pCONValueLabel ) ;

	// INT	
	m_pINTLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pINTLabel->SetLeft( 25 ) ;
	m_pINTLabel->SetTop( 223 ) ;
	m_pINTLabel->SetFontColor (0xffffffff);
	m_pINTLabel->SetCaption( "INT" ) ;
	SetRect( &rt, 14, 223, 44, 235 ) ;
	m_pINTLabel->SetAlignRect( rt ) ;
	m_pINTLabel->SetAlign( RIGHT ) ;
	m_pINTLabel->SetAutoSize( TRUE ) ;
	m_pINTLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1804 ], 1804, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pINTLabel ) ;

	// INT 값
	m_pINTValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pINTValueLabel->SetLeft( 54 ) ;
	m_pINTValueLabel->SetTop( 223 ) ;
	m_pINTValueLabel->SetFontColor (0xffffffff);
	m_pINTValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 49, 223, 83, 235 ) ;
	m_pINTValueLabel->SetAlignRect( rt ) ;
	m_pINTValueLabel->SetAlign( CENTER ) ;
	m_pINTValueLabel->SetAutoSize( TRUE ) ;
	m_pINTValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1804 ], 1804, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pINTValueLabel ) ;

	// WIS
	m_pWISLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pWISLabel->SetLeft( 22 ) ;
	m_pWISLabel->SetTop( 244 ) ;
	m_pWISLabel->SetFontColor (0xffffffff);
	m_pWISLabel->SetCaption( "WIS" ) ;
	SetRect( &rt, 14, 244, 44, 256 ) ;
	m_pWISLabel->SetAlignRect( rt ) ;
	m_pWISLabel->SetAlign( RIGHT ) ;
	m_pWISLabel->SetAutoSize( TRUE ) ;
	m_pWISLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1805 ], 1805, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pWISLabel ) ;

	// WIS 값
	m_pWISValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pWISValueLabel->SetLeft( 54 ) ;
	m_pWISValueLabel->SetTop( 243 ) ;
	m_pWISValueLabel->SetFontColor (0xffffffff);
	m_pWISValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 49, 244, 83, 256 ) ;
	m_pWISValueLabel->SetAlignRect( rt ) ;
	m_pWISValueLabel->SetAlign( CENTER ) ;
	m_pWISValueLabel->SetAutoSize( TRUE ) ;
	m_pWISValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1805 ], 1805, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pWISValueLabel ) ;

	// 공격력		
	m_pStrikingPowerLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pStrikingPowerLabel->SetLeft( 121 ) ;
	m_pStrikingPowerLabel->SetTop( 160 ) ;
	m_pStrikingPowerLabel->SetFontColor (0xffffffff);
	m_pStrikingPowerLabel->SetCaption( CRYLStringTable::m_strString[ 1935 ] ) ;
	SetRect( &rt, 120, 160, 155, 172 ) ;
	m_pStrikingPowerLabel->SetAlignRect( rt ) ; 
	m_pStrikingPowerLabel->SetAlign( RIGHT ) ;
	m_pStrikingPowerLabel->SetAutoSize( TRUE ) ;
	m_pStrikingPowerLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1807 ], 1807, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pStrikingPowerLabel ) ;

	// 공력력 값
	m_pStrikingPowerValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pStrikingPowerValueLabel->SetLeft( 165 ) ;
	m_pStrikingPowerValueLabel->SetTop( 160 ) ;
	m_pStrikingPowerValueLabel->SetFontColor (0xffffffff);
	m_pStrikingPowerValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 160, 160, 242, 172 ) ;
	m_pStrikingPowerValueLabel->SetAlignRect( rt ) ;
	m_pStrikingPowerValueLabel->SetAlign( CENTER ) ;
	m_pStrikingPowerValueLabel->SetAutoSize( TRUE ) ;
	m_pStrikingPowerValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1807 ], 1807, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pStrikingPowerValueLabel ) ;

	// 방어력
	m_pDefensivePowerLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pDefensivePowerLabel->SetLeft( 121 ) ;
	m_pDefensivePowerLabel->SetTop( 181 ) ;
	m_pDefensivePowerLabel->SetFontColor (0xffffffff);
	m_pDefensivePowerLabel->SetCaption( CRYLStringTable::m_strString[ 1936 ] ) ;
	SetRect( &rt, 120, 181, 155, 193 ) ;
	m_pDefensivePowerLabel->SetAlignRect( rt ) ;
	m_pDefensivePowerLabel->SetAlign( RIGHT ) ;
	m_pDefensivePowerLabel->SetAutoSize( TRUE ) ;
	m_pDefensivePowerLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1808 ], 1808, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pDefensivePowerLabel ) ;

	// 방어력 값
	m_pDefensivePowerValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pDefensivePowerValueLabel->SetLeft( 169 ) ;
	m_pDefensivePowerValueLabel->SetTop( 181 ) ;
	m_pDefensivePowerValueLabel->SetFontColor (0xffffffff);
	m_pDefensivePowerValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 160, 181, 199, 193 ) ;
	m_pDefensivePowerValueLabel->SetAlignRect( rt ) ;
	m_pDefensivePowerValueLabel->SetAlign( CENTER ) ;
	m_pDefensivePowerValueLabel->SetAutoSize( TRUE ) ;
	m_pDefensivePowerValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1808 ], 1808, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pDefensivePowerValueLabel ) ;

	// 방어율 값
	m_pArmorRateValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pArmorRateValueLabel->SetLeft( 206 ) ;
	m_pArmorRateValueLabel->SetTop( 181 ) ;
	m_pArmorRateValueLabel->SetFontColor (0xffffffff);
	m_pArmorRateValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 204, 181, 242, 193 ) ;
	m_pArmorRateValueLabel->SetAlignRect( rt ) ;
	m_pArmorRateValueLabel->SetAlign( CENTER ) ;
	m_pArmorRateValueLabel->SetAutoSize( TRUE ) ;
	m_pArmorRateValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 2279 ], 2279, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pArmorRateValueLabel ) ;

	

	// HP
	m_pHPLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pHPLabel->SetLeft( 141 ) ;
	m_pHPLabel->SetTop( 202 ) ;
	m_pHPLabel->SetFontColor (0xffffffff);
	m_pHPLabel->SetCaption( "HP" ) ;
	SetRect( &rt, 120, 202, 155, 212 ) ;
	m_pHPLabel->SetAlignRect( rt ) ;
	m_pHPLabel->SetAlign( RIGHT ) ;
	m_pHPLabel->SetAutoSize( TRUE ) ;
	m_pHPLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1809 ], 1809, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pHPLabel ) ;

	//HP 값
	m_pNowHPValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pNowHPValueLabel->SetLeft( 165 ) ;
	m_pNowHPValueLabel->SetTop( 202 ) ;
	m_pNowHPValueLabel->SetFontColor (0xffffffff);
	m_pNowHPValueLabel->SetCaption( "" ) ;
//	SetRect( &rt, 160, 202, 242, 212 ) ;
//	m_pNowHPValueLabel->SetAlignRect( rt ) ;
//	m_pNowHPValueLabel->SetAlign( CENTER ) ;
	m_pNowHPValueLabel->SetAutoSize( TRUE ) ;
	m_pNowHPValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1809 ], 1809, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pNowHPValueLabel ) ;

	m_pMaxHPValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMaxHPValueLabel->SetLeft( 165 ) ;
	m_pMaxHPValueLabel->SetTop( 202 ) ;
	m_pMaxHPValueLabel->SetFontColor (0xffffffff);
	m_pMaxHPValueLabel->SetCaption( "" ) ;
	m_pMaxHPValueLabel->SetAutoSize( TRUE ) ;
	m_pMaxHPValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1809 ], 1809, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pMaxHPValueLabel ) ;

	// MP
	m_pMPLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMPLabel->SetLeft( 140 ) ;
	m_pMPLabel->SetTop( 223 ) ;
	m_pMPLabel->SetFontColor (0xffffffff);
	m_pMPLabel->SetCaption( "MP" ) ;
	SetRect( &rt, 120, 223, 155, 235 ) ;
	m_pMPLabel->SetAlignRect( rt ) ;
	m_pMPLabel->SetAlign( RIGHT ) ;
	m_pMPLabel->SetAutoSize( TRUE ) ;
	m_pMPLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1810 ], 1810, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pMPLabel ) ;

	// MP 값
	m_pNowMPValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pNowMPValueLabel->SetLeft( 165 ) ;
	m_pNowMPValueLabel->SetTop( 223 ) ;
	m_pNowMPValueLabel->SetFontColor (0xffffffff);
	m_pNowMPValueLabel->SetCaption( "" ) ;
//	SetRect( &rt, 160, 223, 242, 235 ) ;
//	m_pNowMPValueLabel->SetAlignRect( rt ) ;
//	m_pNowMPValueLabel->SetAlign( CENTER ) ;
	m_pNowMPValueLabel->SetAutoSize( TRUE ) ;
	m_pNowMPValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1810 ], 1810, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pNowMPValueLabel ) ;

	m_pMaxMPValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMaxMPValueLabel->SetLeft( 165 ) ;
	m_pMaxMPValueLabel->SetTop( 223 ) ;
	m_pMaxMPValueLabel->SetFontColor (0xffffffff);
	m_pMaxMPValueLabel->SetCaption( "" ) ;
	m_pMaxMPValueLabel->SetAutoSize( TRUE ) ;
	m_pMaxMPValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1810 ], 1810, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pMaxMPValueLabel ) ;

	// 보너스 포인트
	m_pBonusPointLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pBonusPointLabel->SetLeft( 132 ) ;
	m_pBonusPointLabel->SetTop( 244 ) ;
	m_pBonusPointLabel->SetFontColor (0xffffffff);
	m_pBonusPointLabel->SetCaption( CRYLStringTable::m_strString[ 1910 ] ) ;
	SetRect( &rt, 120, 244, 202, 256 ) ;
	m_pBonusPointLabel->SetAlignRect( rt ) ;
	m_pBonusPointLabel->SetAlign( RIGHT ) ;
	m_pBonusPointLabel->SetAutoSize( TRUE ) ;
	m_pBonusPointLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1811 ], 1811, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pBonusPointLabel ) ;

	// 보너스 포인트 값
	m_pBonusPointValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pBonusPointValueLabel->SetLeft( 213 ) ;
	m_pBonusPointValueLabel->SetTop( 244 ) ;
	m_pBonusPointValueLabel->SetFontColor (0xffffffff);
	m_pBonusPointValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 208, 244, 242, 256 ) ;
	m_pBonusPointValueLabel->SetAlignRect( rt ) ;
	m_pBonusPointValueLabel->SetAlign( CENTER ) ;
	m_pBonusPointValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pBonusPointValueLabel ) ;

	// 명중
	m_pHitRatioLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pHitRatioLabel->SetLeft( 38 ) ;
	m_pHitRatioLabel->SetTop( 273 ) ;
	m_pHitRatioLabel->SetFontColor (0xffffffff);
	m_pHitRatioLabel->SetCaption( CRYLStringTable::m_strString[ 1937 ] ) ;
	SetRect( &rt, 14, 273, 61, 285 ) ; 
	m_pHitRatioLabel->SetAlignRect( rt ) ;
	m_pHitRatioLabel->SetAlign( RIGHT ) ;
	m_pHitRatioLabel->SetAutoSize( TRUE ) ;
	m_pHitRatioLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1812 ], 1812, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pHitRatioLabel ) ;

	// 명중 값		
	m_pHitRatioValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pHitRatioValueLabel->SetLeft( 72 ) ;
	m_pHitRatioValueLabel->SetTop( 273 ) ;
	m_pHitRatioValueLabel->SetFontColor (0xffffffff);
	m_pHitRatioValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 66, 273, 100, 285 ) ;
	m_pHitRatioValueLabel->SetAlignRect( rt ) ;
	m_pHitRatioValueLabel->SetAlign( CENTER ) ;
	m_pHitRatioValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pHitRatioValueLabel ) ;

	// 회피
	m_pEvasionLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pEvasionLabel->SetLeft( 39 ) ;
	m_pEvasionLabel->SetTop( 294 ) ;
	m_pEvasionLabel->SetFontColor (0xffffffff);
	m_pEvasionLabel->SetCaption( CRYLStringTable::m_strString[ 1938 ] ) ;
	SetRect( &rt, 14, 294, 61, 306 ) ; 
	m_pEvasionLabel->SetAlignRect( rt ) ;
	m_pEvasionLabel->SetAlign( RIGHT ) ;
	m_pEvasionLabel->SetAutoSize( TRUE ) ;
	m_pEvasionLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1813 ], 1813, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pEvasionLabel ) ;

	// 회피 값
	m_pEvasionValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pEvasionValueLabel->SetLeft( 72 ) ;
	m_pEvasionValueLabel->SetTop( 294 ) ;
	m_pEvasionValueLabel->SetFontColor (0xffffffff);
	m_pEvasionValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 66, 294, 100, 306 ) ;
	m_pEvasionValueLabel->SetAlignRect( rt ) ;
	m_pEvasionValueLabel->SetAlign( CENTER ) ;
	m_pEvasionValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pEvasionValueLabel ) ;

	// 마법력
	m_pMagicPowerLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMagicPowerLabel->SetLeft( 27 ) ;
	m_pMagicPowerLabel->SetTop( 315 ) ;
	m_pMagicPowerLabel->SetFontColor (0xffffffff);
	m_pMagicPowerLabel->SetCaption( CRYLStringTable::m_strString[ 1939 ] ) ;
	SetRect( &rt, 14, 315, 61, 327 ) ; 
	m_pMagicPowerLabel->SetAlignRect( rt ) ;
	m_pMagicPowerLabel->SetAlign( RIGHT ) ;
	m_pMagicPowerLabel->SetAutoSize( TRUE ) ;
	m_pMagicPowerLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1816 ], 1816, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pMagicPowerLabel ) ;

	// 마법력 값
	m_pMagicPowerValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMagicPowerValueLabel->SetLeft( 72 ) ;
	m_pMagicPowerValueLabel->SetTop( 315 ) ;
	m_pMagicPowerValueLabel->SetFontColor (0xffffffff);
	m_pMagicPowerValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 66, 315, 100, 327 ) ;
	m_pMagicPowerValueLabel->SetAlignRect( rt ) ;
	m_pMagicPowerValueLabel->SetAlign( CENTER ) ;
	m_pMagicPowerValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pMagicPowerValueLabel ) ;


	// 블록률
	m_pBlockPowerLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pBlockPowerLabel->SetLeft( 129 ) ;
	m_pBlockPowerLabel->SetTop( 294 ) ;
	m_pBlockPowerLabel->SetFontColor (0xffffffff);
	m_pBlockPowerLabel->SetCaption( CRYLStringTable::m_strString[ 1940 ] ) ;
	SetRect( &rt, 108, 294, 159, 306 ) ;
	m_pBlockPowerLabel->SetAlignRect( rt ) ;
	m_pBlockPowerLabel->SetAlign( RIGHT ) ;
	m_pBlockPowerLabel->SetAutoSize( TRUE ) ;
	m_pBlockPowerLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1814 ], 1814, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pBlockPowerLabel ) ;

	// 블록률 값
	m_pBlockPowerValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pBlockPowerValueLabel->SetLeft( 193 ) ;
	m_pBlockPowerValueLabel->SetTop( 294 ) ;
	m_pBlockPowerValueLabel->SetFontColor (0xffffffff);
	m_pBlockPowerValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 165, 294, 240, 306 ) ;
	m_pBlockPowerValueLabel->SetAlignRect( rt ) ;
	m_pBlockPowerValueLabel->SetAlign( CENTER ) ;
	m_pBlockPowerValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pBlockPowerValueLabel ) ;

	// 크리티컬
	m_pCriticalLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pCriticalLabel->SetLeft( 114 ) ;
	m_pCriticalLabel->SetTop( 273 ) ;
	m_pCriticalLabel->SetFontColor (0xffffffff);
	m_pCriticalLabel->SetCaption( CRYLStringTable::m_strString[ 1941 ] ) ;
	SetRect( &rt, 108, 273, 159, 285 ) ;
	m_pCriticalLabel->SetAlignRect( rt ) ;
	m_pCriticalLabel->SetAlign( RIGHT ) ; 
	m_pCriticalLabel->SetAutoSize( TRUE ) ;
	m_pCriticalLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1815 ], 1815, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pCriticalLabel ) ;

	// 크리티컬 값
	m_pCriticalValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pCriticalValueLabel->SetLeft( 196 ) ;
	m_pCriticalValueLabel->SetTop( 273 ) ;
	m_pCriticalValueLabel->SetFontColor (0xffffffff);
	m_pCriticalValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 165, 273, 240, 285 ) ;
	m_pCriticalValueLabel->SetAlignRect( rt ) ;
	m_pCriticalValueLabel->SetAlign( CENTER ) ;
	m_pCriticalValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pCriticalValueLabel ) ;


	// 마법저항
	m_pMagicResistLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMagicResistLabel->SetLeft( 114 ) ;
	m_pMagicResistLabel->SetTop( 315 ) ;
	m_pMagicResistLabel->SetFontColor (0xffffffff);
	m_pMagicResistLabel->SetCaption( CRYLStringTable::m_strString[ 1942 ] ) ;
	SetRect( &rt, 108, 315, 159, 327 ) ;
	m_pMagicResistLabel->SetAlignRect( rt ) ;
	m_pMagicResistLabel->SetAlign( RIGHT ) ;
	m_pMagicResistLabel->SetAutoSize( TRUE ) ;
	m_pMagicResistLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1817 ], 1817, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pMagicResistLabel ) ;

	// 마법저항 값
	m_pMagicResistValueLabel = new CRYLLabel( m_pGMUIParentDlg ) ;		
	m_pMagicResistValueLabel->SetLeft( 193 ) ;
	m_pMagicResistValueLabel->SetTop( 315 ) ;
	m_pMagicResistValueLabel->SetFontColor (0xffffffff);
	m_pMagicResistValueLabel->SetCaption( "" ) ;
	SetRect( &rt, 165, 315, 240, 327 ) ;
	m_pMagicResistValueLabel->SetAlignRect( rt ) ;
	m_pMagicResistValueLabel->SetAlign( CENTER ) ;
	m_pMagicResistValueLabel->SetAutoSize( TRUE ) ;
	CGMUICustomPanel::AddChild( m_pMagicResistValueLabel ) ;

	CGMImageDib* pSprite			= CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList* pRYLButtonRect	= CRYLSpriteList::Instance() ;
	// 랭킹
	m_pRankingButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pRankingButton->SetLeft( 98 ) ;
	m_pRankingButton->SetTop( 343 ) ;
	m_pRankingButton->SetCaption( CRYLStringTable::m_strString[ 1943 ] ) ;
	m_pRankingButton->SetFontColor( 0xffffffff) ;
	m_pRankingButton->SetColor( 0xffffffff) ;
	m_pRankingButton->OnClickEvent = DlgClick ;
	m_pRankingButton->SetInstance( RYLSTATUSPANEL::RANKING_BUTTON ) ;
	m_pRankingButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pRankingButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pRankingButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pRankingButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pRankingButton->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 2250 ], 2250, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pRankingButton ) ;

	// MP/HP
	m_pHPMPButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pHPMPButton->SetLeft( 173 ) ;
	m_pHPMPButton->SetTop( 343 ) ;
	m_pHPMPButton->SetCaption( CRYLStringTable::m_strString[ 1944 ] ) ;
	m_pHPMPButton->SetFontColor( 0xffffffff) ;
	m_pHPMPButton->SetColor( 0xffffffff) ;
	m_pHPMPButton->OnClickEvent = DlgClick ;
	m_pHPMPButton->SetInstance( RYLSTATUSPANEL::HP_MP_BUTTON ) ;
	m_pHPMPButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pHPMPButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pHPMPButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pHPMPButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pHPMPButton->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1818 ], 1818, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pHPMPButton ) ;
	
	// 인챈트창
	m_pEnchantButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pEnchantButton->SetLeft( 11 ) ;
	m_pEnchantButton->SetTop( 343 ) ;
	m_pEnchantButton->SetCaption( CRYLStringTable::m_strString[ 1945 ] ) ;
	m_pEnchantButton->SetFontColor( 0xffffffff) ;
	m_pEnchantButton->SetColor( 0xffffffff) ;
	m_pEnchantButton->OnClickEvent = DlgClick ;
	m_pEnchantButton->SetInstance( RYLSTATUSPANEL::ENCHANT_BUTTON ) ;
	m_pEnchantButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pEnchantButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pEnchantButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pEnchantButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pEnchantButton->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1820 ], 1820, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pEnchantButton ) ;


	pSprite	= CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) ;
	// STR 포인트 증가,감소 버튼
	m_pStrPointIncBtn = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pStrPointIncBtn->SetLeft( 88 ) ;
	m_pStrPointIncBtn->SetTop( 155 ) ;
	m_pStrPointIncBtn->OnClickEvent = DlgClick ;
	m_pStrPointIncBtn->SetInstance( RYLSTATUSPANEL::STRPOINTINC_BUTTON ) ;
	SetRect( &rt, 29, 81, 49, 101 ) ;
	m_pStrPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 29, 130, 49, 150) ;
	m_pStrPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 29, 106, 49, 126) ;
	m_pStrPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 29, 56, 49, 76) ;
	m_pStrPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pStrPointIncBtn->SetVisible( FALSE ) ;
	m_pStrPointIncBtn->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1806 ], 1806, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pStrPointIncBtn ) ;

	// DEX 포인트 증가,감소 버튼
	m_pDexPointIncBtn = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pDexPointIncBtn->SetLeft( 88 ) ;
	m_pDexPointIncBtn->SetTop( 176 ) ;
	m_pDexPointIncBtn->OnClickEvent = DlgClick ;
	m_pDexPointIncBtn->SetInstance( RYLSTATUSPANEL::DEXPOINTINC_BUTTON ) ;
	SetRect( &rt, 29, 81, 49, 101 ) ;
	m_pDexPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 29, 130, 49, 150) ;
	m_pDexPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 29, 106, 49, 126) ;
	m_pDexPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 29, 56, 49, 76) ;
	m_pDexPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pDexPointIncBtn->SetVisible( FALSE ) ;
	m_pDexPointIncBtn->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1806 ], 1806, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pDexPointIncBtn ) ;

	// CON 포인트 증가,감소 버튼
	m_pConPointIncBtn = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pConPointIncBtn->SetLeft( 88 ) ;
	m_pConPointIncBtn->SetTop( 197 ) ;
	m_pConPointIncBtn->OnClickEvent = DlgClick ;
	m_pConPointIncBtn->SetInstance( RYLSTATUSPANEL::CONPOINTINC_BUTTON ) ;
	SetRect( &rt, 29, 81, 49, 101 ) ;
	m_pConPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 29, 130, 49, 150) ;
	m_pConPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 29, 106, 49, 126) ;
	m_pConPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 29, 56, 49, 76) ;
	m_pConPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pConPointIncBtn->SetVisible( FALSE ) ;
	m_pConPointIncBtn->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1806 ], 1806, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pConPointIncBtn ) ;

	// INT 포인트 증가,감소 버튼
	m_pIntPointIncBtn = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pIntPointIncBtn->SetLeft( 88 ) ;
	m_pIntPointIncBtn->SetTop( 218 ) ;
	m_pIntPointIncBtn->OnClickEvent = DlgClick ;
	m_pIntPointIncBtn->SetInstance( RYLSTATUSPANEL::INTPOINTINC_BUTTON ) ;
	SetRect( &rt, 29, 81, 49, 101 ) ;
	m_pIntPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 29, 130, 49, 150) ;
	m_pIntPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 29, 106, 49, 126) ;
	m_pIntPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 29, 56, 49, 76) ;
	m_pIntPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pIntPointIncBtn->SetVisible( FALSE ) ;
	m_pIntPointIncBtn->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1806 ], 1806, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pIntPointIncBtn ) ;

	// WIS 포인트 증가,감소 버튼
	m_pWisPointIncBtn = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pWisPointIncBtn->SetLeft( 88 ) ;
	m_pWisPointIncBtn->SetTop( 239 ) ;
	m_pWisPointIncBtn->OnClickEvent = DlgClick ;
	m_pWisPointIncBtn->SetInstance( RYLSTATUSPANEL::WISPOINTINC_BUTTON ) ;
	SetRect( &rt, 29, 81, 49, 101 ) ;
	m_pWisPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 29, 130, 49, 150) ;
	m_pWisPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 29, 106, 49, 126) ;
	m_pWisPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 29, 56, 49, 76) ;
	m_pWisPointIncBtn->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pWisPointIncBtn->SetVisible( FALSE ) ;
	m_pWisPointIncBtn->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1806 ], 1806, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pWisPointIncBtn ) ;

	// 길드 마크
	m_pGuildMarkSprite = new CRYLSpriteEx( m_pGMUIParentDlg );
	m_pGuildMarkSprite->SetLeft( 213 );
	m_pGuildMarkSprite->SetTop( 29 );
	m_pGuildMarkSprite->SetWidth( 12 );
	m_pGuildMarkSprite->SetHeight( 12 );
	CGMUICustomPanel::AddChild( m_pGuildMarkSprite );
}

VOID	CRYLStatusPanel::FinalCtrl() 
{
	GM_DELETE( m_pNameLabel ) ;							// 이름	
	GM_DELETE( m_pNameValueLabel ) ;					// 이름 값
	GM_DELETE( m_pGuildLabel ) ;						// 길드
	GM_DELETE( m_pGuildValueLabel ) ;					// 길드 값
	GM_DELETE( m_pClassLabel ) ;						// 클레스
	GM_DELETE( m_pClassValueLabel ) ;					// 클레스 값
	GM_DELETE( m_pMarkLabek ) ;							// 마크
	GM_DELETE( m_pLevelLabel ) ;						// 레벨
	GM_DELETE( m_pLevelValueLabel ) ;					// 레벨 값
	GM_DELETE( m_pExperienceLabel ) ;					// 경험점
	GM_DELETE( m_pExperienceBalueLabel ) ;				// 경험점 값
	GM_DELETE( m_pFameLabel ) ;							// 명성
	GM_DELETE( m_pFameValueLabel ) ;					// 명성 값
	GM_DELETE( m_pServiceMedalLabel ) ;					// 공헌 매달
	GM_DELETE( m_pServiceMedalValueLabel ) ;			// 공헌 매달 값
	GM_DELETE( m_pScoreLabel ) ;						// 스코어
	GM_DELETE( m_pScoreValueLabel ) ;					// 스코어 값
	GM_DELETE( m_pSTRLabel ) ;							// STR
	GM_DELETE( m_pSTRValueLabel ) ;						// STR 값
	GM_DELETE( m_pDEXLabel ) ;							// DEX
	GM_DELETE( m_pDEXValueLabel ) ;						// DEX 값
	GM_DELETE( m_pCONLabel ) ;							// CON
	GM_DELETE( m_pCONValueLabel ) ;						// CON 값
	GM_DELETE( m_pINTLabel ) ;							// INT
	GM_DELETE( m_pINTValueLabel ) ;						// INT 값
	GM_DELETE( m_pWISLabel ) ;							// WIS
	GM_DELETE( m_pWISValueLabel ) ;						// WIS 값

	GM_DELETE( m_pStrikingPowerLabel ) ;				// 공격력		
	GM_DELETE( m_pStrikingPowerValueLabel ) ;			// 공력력 값
	GM_DELETE( m_pDefensivePowerLabel ) ;				// 방어력
	GM_DELETE( m_pDefensivePowerValueLabel ) ;			// 방어력 값
	GM_DELETE( m_pArmorRateValueLabel );				// 방어율 값

	GM_DELETE( m_pHPLabel ) ;							// HP
	GM_DELETE( m_pNowHPValueLabel ) ;					// 현재 HP 값
	GM_DELETE( m_pMaxHPValueLabel ) ;					// 최대 HP 값

	GM_DELETE( m_pMPLabel ) ;							// MP
	GM_DELETE( m_pNowMPValueLabel ) ;					// 현재 MP 값
	GM_DELETE( m_pMaxMPValueLabel ) ;					// 최대 MP 값

	GM_DELETE( m_pBonusPointLabel ) ;					// 보너스 포인트
	GM_DELETE( m_pBonusPointValueLabel ) ;				// 보너스 포인트 값

	GM_DELETE( m_pHitRatioLabel ) ;				// 공격보정
	GM_DELETE( m_pHitRatioValueLabel ) ;			// 공격보정 값		
	GM_DELETE( m_pEvasionLabel ) ;				// 방어보정
	GM_DELETE( m_pEvasionValueLabel ) ;			// 방어보정 값

	GM_DELETE( m_pMagicPowerLabel ) ;					// 마법력
	GM_DELETE( m_pMagicPowerValueLabel ) ;				// 마법력 값

	GM_DELETE( m_pBlockPowerLabel ) ;					// 블록률
	GM_DELETE( m_pBlockPowerValueLabel ) ;				// 블록률 값

	GM_DELETE( m_pCriticalLabel ) ;						// 크리티컬
	GM_DELETE( m_pCriticalValueLabel ) ;				// 크리티컬 값

	GM_DELETE( m_pMagicResistLabel ) ;					// 마법저항
	GM_DELETE( m_pMagicResistValueLabel ) ;				// 마법저항 값

	GM_DELETE( m_pRankingButton ) ;						// 랭킹 버튼
	GM_DELETE( m_pHPMPButton ) ;						// HP/MP
	GM_DELETE( m_pEnchantButton ) ;						// 인챈트창
	// STR 포인트 증가,감소 버튼
	GM_DELETE( m_pStrPointIncBtn ) ;
	// DEX 포인트 증가,감소 버튼
	GM_DELETE( m_pDexPointIncBtn ) ;
	// CON 포인트 증가,감소 버튼
	GM_DELETE( m_pConPointIncBtn ) ;
	// INT 포인트 증가,감소 버튼
	GM_DELETE( m_pIntPointIncBtn ) ;
	// WIS 포인트 증가,감소 버튼
	GM_DELETE( m_pWisPointIncBtn ) ;

	GM_DELETE( m_pNationLabel );						// 국적
	GM_DELETE( m_pNationValueLabel );					// 국적값

	GM_DELETE( m_pGuildMarkSprite );					// 길드 마크
}


HRESULT		CRYLStatusPanel::Update()
{
	static char strString[MAX_PATH];
	static INT iLeft = 160, iRight = 242;
	CRYLGameData *pGame = CRYLGameData::Instance() ;

	if(!pGame)
		return S_OK;

	// 이름 값
	m_pNameValueLabel->SetCaption( pGame->m_csStatus.m_Info.Name ) ;
	
	// 길드 값
	if ( 0 != pGame->m_csStatus.m_Info.GID )
	{
		CRYLGameScene*			pScene		= ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
		CRYLGuildFrameDlg*		pDlg		= ( CRYLGuildFrameDlg* )pScene->GetGuildFrameDlg() ;
		CRYLGuildInfoPanel*		pPanel		= ( CRYLGuildInfoPanel* )pDlg->GetGuildInfoPanel() ;
		LPGUILDLARGEINFOEX		lpGuildLargeInfo = pPanel->FindGuild( pGame->m_csStatus.m_Info.GID ) ;

		if ( lpGuildLargeInfo )	m_pGuildValueLabel->SetCaption( lpGuildLargeInfo->m_szName ) ;

		// 길드 마크 출력
		CRYLSprite* pSprite = pPanel->GetGuildSprite( pGame->m_csStatus.m_Info.GID );
		if ( pSprite )
		{
			if(m_pGuildMarkSprite)
			{
				m_pGuildMarkSprite->SetVisible( TRUE );
				m_pGuildMarkSprite->AttachImageTexture( pSprite );
			}
		}
		else
		{
			if(m_pGuildMarkSprite)
				m_pGuildMarkSprite->SetVisible( FALSE );
		}
	}
	else
	{
		m_pGuildValueLabel->SetCaption( "" ) ;
		m_pGuildMarkSprite->SetVisible( FALSE );
	}

	// 국가 값
	if ( Creature::MERKADIA == pGame->m_cAccountNation )
	{
		m_pNationValueLabel->SetCaption( "MK" ) ;
		m_pNationValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 2286 ], 1812, TOOLTIP_PANEL ) ;
	}
	else if ( Creature::KARTERANT == pGame->m_cAccountNation )
	{
		m_pNationValueLabel->SetCaption( "KR" ) ;
		m_pNationValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 2285 ], 1812, TOOLTIP_PANEL ) ;
	}
	else if ( Creature::ALMIGHTY_PIRATE == pGame->m_cAccountNation )	// 신의 해적단
	{
		m_pNationValueLabel->SetCaption( "AP" ) ;
		m_pNationValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 2287 ], 1812, TOOLTIP_PANEL ) ;
	}


	// 클레스 값
	m_pClassValueLabel->SetCaption( pGame->GetClassName( pGame->m_csStatus.m_Info.Class ) ) ;
		
	// 작위 값
	int nFame = pGame->m_csStatus.m_Info.Fame ;
	int nFameNameField = 0;

	if( 1 <= nFame && nFame <= 2000 )					nFameNameField = 1;
	else if( 2001 <= nFame && nFame <= 10000 )			nFameNameField = 2;
	else if( 10001 <= nFame && nFame <= 25000 )			nFameNameField = 3;
	else if( 25001 <= nFame && nFame <= 50000 )			nFameNameField = 4;
	else if( 50001 <= nFame )							nFameNameField = 5;
	
	int nClassInfo = 0;		

	switch( pGame->m_csStatus.m_Info.Class )
	{	
		// Defender
		case 5:			nClassInfo = 1;			break;
		// Warrior	
		case 6:			nClassInfo = 2;			break;		
		// Assasin
		case 7:			nClassInfo = 3;			break;
		// Archer
		case 8:			nClassInfo = 4;			break;		
		// Sorcerer	
		case 9:			nClassInfo = 5;			break;
		// Enchanter
		case 10:		nClassInfo = 6;			break;
		// Priest
		case 11:		nClassInfo = 7;			break;
		// Cleric
		case 12:		nClassInfo = 8;			break;			
		// Templer
		case 19:		nClassInfo = 9;			break;
		// Attacker
		case 20:		nClassInfo = 10;		break;
		// Gunner
		case 21:		nClassInfo = 11;		break;		
		// Rune Ope	
		case 22:		nClassInfo = 12;		break;
		// Life Ope	
		case 23:		nClassInfo = 13;		break;
		// Shadow Ope
		case 24:		nClassInfo = 14;		break;
	}

	/*
	if( nFameNameField == 0 || nClassInfo == 0)
	{
		m_pTitleValueLabel->SetCaption( "None" ) ;
	} else
	{
		int nStringTable = (nClassInfo - 1) * 5 + 1000 + nFameNameField - 1 ;

		strcpy( strString, CRYLStringTable::m_strString[ nStringTable ] );
		m_pTitleValueLabel->SetCaption( strString ) ;
	}
	*/
	
	// 명성 값
//	sprintf(strString, "%10d", pGame->m_csStatus.m_Info.Fame);
//	m_pFameValueLabel->SetCaption( strString ) ;
	m_pFameValueLabel->SetCaption( CRYLStringTable::MakePrintGold( pGame->m_csStatus.m_Info.Fame ) ) ;

	// 공헌 메달 값
//	sprintf( strString, "%10d", pGame->m_csStatus.m_Info.Mileage ) ;
//	m_pServiceMedalValueLabel->SetCaption( strString ) ;
	m_pServiceMedalValueLabel->SetCaption( CRYLStringTable::MakePrintGold( pGame->m_csStatus.m_Info.Mileage ) ) ;

	// 레벨 값
	sprintf( strString, "%3d", pGame->m_csStatus.m_Info.Level ) ;
	m_pLevelValueLabel->SetCaption( strString ) ;

	unsigned long dwExp, dwExpMax;
	char strExp[2], strExpMax[2];
	if(1000000 < pGame->m_csStatus.m_Info.Exp && pGame->m_csStatus.m_Info.Exp <= 1000000000)
	{
		dwExp = pGame->m_csStatus.m_Info.Exp / 1000;
		strcpy(strExp, "K");
	} else if(1000000000 < pGame->m_csStatus.m_Info.Exp && pGame->m_csStatus.m_Info.Exp <= 1000000000000)
	{
		dwExp = pGame->m_csStatus.m_Info.Exp / 1000000;
		strcpy(strExp, "M");
	} else if(1000000000000 < pGame->m_csStatus.m_Info.Exp && pGame->m_csStatus.m_Info.Exp <= 1000000000000000)
	{
		dwExp = pGame->m_csStatus.m_Info.Exp / 1000000000;
		strcpy(strExp, "G");
	} else
	{
		dwExp = pGame->m_csStatus.m_Info.Exp;
		strcpy(strExp, "");
	}

	if(1000000 < pGame->m_csStatus.m_llExpMax && pGame->m_csStatus.m_llExpMax <= 1000000000)
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax / 1000;
		strcpy(strExpMax, "K");
	} else if(1000000000 < pGame->m_csStatus.m_llExpMax && pGame->m_csStatus.m_llExpMax <= 1000000000000)
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax / 1000000;
		strcpy(strExpMax, "M");
	} else if(1000000000000 < pGame->m_csStatus.m_llExpMax && pGame->m_csStatus.m_llExpMax <= 1000000000000000)
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax / 1000000000;
		strcpy(strExpMax, "G");
	} else
	{
		dwExpMax = pGame->m_csStatus.m_llExpMax;
		strcpy(strExpMax, "");
	}

	sprintf(strString, "%8d%s / %8d%s(%4.2f%%)", dwExp, strExp, dwExpMax, strExpMax, (float)((double)pGame->m_csStatus.m_Info.Exp / (double)pGame->m_csStatus.m_llExpMax * 100.0f));
	// 경험점 값
	//sprintf(strString, "%ld / %ld(%4.2f%%)", pGame->m_csStatus.m_Info.Exp, pGame->m_csStatus.m_llExpMax, (float)((double)pGame->m_csStatus.m_Info.Exp / (double)pGame->m_csStatus.m_llExpMax * 100.0f));
	m_pExperienceBalueLabel->SetCaption( strString ) ;

	// 스코어 값
//	m_pScoreValueLabel

	// STR 값
	sprintf( strString, "%4d", pGame->m_csStatus.m_Info.STR ) ;
	m_pSTRValueLabel->SetCaption( strString ) ;
	
	// DEX 값
	sprintf( strString, "%4d", pGame->m_csStatus.m_Info.DEX ) ;
	m_pDEXValueLabel->SetCaption( strString ) ;
	
	// CON 값
	sprintf( strString, "%4d", pGame->m_csStatus.m_Info.CON ) ;
	m_pCONValueLabel->SetCaption( strString ) ;

	// INT 값
	sprintf( strString, "%4d", pGame->m_csStatus.m_Info.INT ) ;
	m_pINTValueLabel->SetCaption( strString ) ;
	
	// WIS 값
	sprintf( strString, "%4d", pGame->m_csStatus.m_Info.WIS ) ;
	m_pWISValueLabel->SetCaption( strString ) ;


	// 보너스 포인트 값
	sprintf( strString, "%4d", pGame->m_csStatus.m_Info.IP ) ;
	m_pBonusPointValueLabel->SetCaption( strString ) ;
	if ( pGame->m_csStatus.m_Info.IP && ( pGame->m_csStatus.m_Info.HP != 0 ) )
	{
		m_pStrPointIncBtn->SetVisible( TRUE ) ;
		m_pDexPointIncBtn->SetVisible( TRUE ) ;
		m_pConPointIncBtn->SetVisible( TRUE ) ;
		m_pIntPointIncBtn->SetVisible( TRUE ) ;
		m_pWisPointIncBtn->SetVisible( TRUE ) ;
	}
	else
	{
		m_pStrPointIncBtn->SetVisible( FALSE ) ;
		m_pDexPointIncBtn->SetVisible( FALSE ) ;
		m_pConPointIncBtn->SetVisible( FALSE ) ;
		m_pIntPointIncBtn->SetVisible( FALSE ) ;
		m_pWisPointIncBtn->SetVisible( FALSE ) ;
	}

	
	// 공력력 값
	sprintf(strString, "%4d ~ %4d", pGame->m_csStatus.GetMinDamage(), pGame->m_csStatus.GetMaxDamage());
	long m_lDamage = pGame->m_csStatus.GetMinDamage() + ( pGame->m_csStatus.GetMaxDamage() - pGame->m_csStatus.GetMinDamage()) / 2;
	long m_lRealDamage = pGame->m_csStatus.GetBaseMinDamage() + ( pGame->m_csStatus.GetBaseMaxDamage() - pGame->m_csStatus.GetBaseMinDamage()) / 2;
	if(m_lDamage < m_lRealDamage)
	{
		m_pStrikingPowerValueLabel->SetCaption( strString ) ;
		m_pStrikingPowerValueLabel->SetFontColor( 0xffff0000 ) ;
	} else if(m_lDamage > m_lRealDamage)
	{
		m_pStrikingPowerValueLabel->SetCaption( strString ) ;
		m_pStrikingPowerValueLabel->SetFontColor( 0xff00ff00 ) ;
	} else
	{
		m_pStrikingPowerValueLabel->SetCaption( strString ) ;
		m_pStrikingPowerValueLabel->SetFontColor( 0xffffffff ) ;
	}

	// 방어력 값
	sprintf(strString, "%4d", pGame->m_csStatus.GetArmor());
	if ( pGame->m_csStatus.GetArmor() < pGame->m_csStatus.GetBaseArmor() )
	{
		m_pDefensivePowerValueLabel->SetCaption( strString ) ;
		m_pDefensivePowerValueLabel->SetFontColor( 0xffff0000 ) ;

		m_pArmorRateValueLabel->SetFontColor( 0xffff0000 ) ;
	} 
	else if( pGame->m_csStatus.GetArmor() > pGame->m_csStatus.GetBaseArmor() )
	{
		m_pDefensivePowerValueLabel->SetCaption( strString ) ;
		m_pDefensivePowerValueLabel->SetFontColor( 0xff00ff00 ) ;

		m_pArmorRateValueLabel->SetFontColor( 0xff00ff00 ) ;
	} 
	else
	{
		m_pDefensivePowerValueLabel->SetCaption( strString ) ;
		m_pDefensivePowerValueLabel->SetFontColor( 0xffffffff ) ;

		m_pArmorRateValueLabel->SetFontColor( 0xffffffff ) ;
	}

	sprintf(strString, "%2.2f%%", pGame->m_csStatus.GetDefenceRate() );
	m_pArmorRateValueLabel->SetCaption( strString );

	// HP 값
	sprintf( strString, "%d / ", static_cast<unsigned short>(pGame->m_csStatus.m_Info.HP) );
	INT iNowHPWidth = CRYLStringTable::GetStringWidth( strString ) ;
	m_pNowHPValueLabel->SetCaption( strString );
	
	sprintf( strString, "%d", static_cast<unsigned short>(pGame->m_csStatus.m_MaxHP) );
	INT iMaxHPWidth = CRYLStringTable::GetStringWidth( strString ) ;
	m_pMaxHPValueLabel->SetCaption( strString );

	m_pNowHPValueLabel->SetLeft( iLeft + (iRight - iLeft) / 2 - (iNowHPWidth + iMaxHPWidth) / 2 ) ;
	m_pMaxHPValueLabel->SetLeft( m_pNowHPValueLabel->GetLeft() + iNowHPWidth ) ;
	
	if (pGame->m_csStatus.GetBaseMaxHP() < pGame->m_csStatus.GetMaxHP())
	{
		m_pMaxHPValueLabel->SetFontColor( 0xff00ff00 ) ;
	}
	else if (pGame->m_csStatus.GetBaseMaxHP() > pGame->m_csStatus.GetMaxHP())
	{
		m_pMaxHPValueLabel->SetFontColor( 0xffff0000 ) ;
	}
	else
	{
		m_pMaxHPValueLabel->SetFontColor( 0xffffffff ) ;
	}

	// MP 값
	sprintf( strString, "%d / ", static_cast<unsigned short>(pGame->m_csStatus.m_Info.MP) );
	INT iNowMPWidth = CRYLStringTable::GetStringWidth( strString ) ;
	m_pNowMPValueLabel->SetCaption( strString );

	sprintf( strString, "%d", static_cast<unsigned short>(pGame->m_csStatus.m_MaxMP) );
	INT iMaxMPWidth = CRYLStringTable::GetStringWidth( strString ) ;
	m_pMaxMPValueLabel->SetCaption( strString );

	m_pNowMPValueLabel->SetLeft( iLeft + (iRight - iLeft) / 2 - (iNowMPWidth + iMaxMPWidth) / 2 ) ;
	m_pMaxMPValueLabel->SetLeft( m_pNowMPValueLabel->GetLeft() + iNowMPWidth ) ;

	if (pGame->m_csStatus.GetBaseMaxMP() < pGame->m_csStatus.GetMaxMP())
	{
		m_pMaxMPValueLabel->SetFontColor( 0xff00ff00 ) ;
	}
	else if (pGame->m_csStatus.GetBaseMaxMP() > pGame->m_csStatus.GetMaxMP())
	{
		m_pMaxMPValueLabel->SetFontColor( 0xffff0000 ) ;
	}
	else
	{
		m_pMaxMPValueLabel->SetFontColor( 0xffffffff ) ;
	}
	
	// 명중률 값		
	sprintf( strString, "%4d", pGame->m_csStatus.GetHitRate() ) ;
	m_pHitRatioValueLabel->SetCaption( strString ) ;

	sprintf( strString,  CRYLStringTable::m_strString[ 2280 ], pGame->m_csStatus.GetHitRate(), pGame->m_csStatus.GetHitRate()/2, pGame->m_csStatus.GetHitRate()/3 ) ;
	m_pHitRatioValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, strString, 1812, TOOLTIP_PANEL ) ;

	if( pGame->m_csStatus.GetHitRate() < pGame->m_csStatus.GetBaseHitRate() )		
	{
		m_pHitRatioValueLabel->SetFontColor( 0xffff0000 );
	}
	else if ( pGame->m_csStatus.GetHitRate() > pGame->m_csStatus.GetBaseHitRate() )	
	{
		m_pHitRatioValueLabel->SetFontColor( 0xff00ff00 );
	}
	else
	{
		m_pHitRatioValueLabel->SetFontColor( 0xffffffff );
	}
	
	// 회피 값
	sprintf( strString, "%4d", pGame->m_csStatus.GetEvade() ) ;
	m_pEvasionValueLabel->SetCaption( strString ) ;

	sprintf( strString,  CRYLStringTable::m_strString[ 2281 ], pGame->m_csStatus.GetEvade(), pGame->m_csStatus.GetEvade()*2, pGame->m_csStatus.GetEvade()*3 ) ;
	m_pEvasionValueLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, strString, 1812, TOOLTIP_PANEL ) ;

	if( pGame->m_csStatus.GetEvade() < pGame->m_csStatus.GetBaseEvade() )			
	{
		m_pEvasionValueLabel->SetFontColor( 0xffff0000 );
	}
	else if ( pGame->m_csStatus.GetEvade() > pGame->m_csStatus.GetBaseEvade() )		
	{
		m_pEvasionValueLabel->SetFontColor( 0xff00ff00 );
	}
	else
	{
		m_pEvasionValueLabel->SetFontColor( 0xffffffff );
	}

	// 마법력 값
	sprintf( strString, "%4d", pGame->m_csStatus.GetMagicPower() );
	m_pMagicPowerValueLabel->SetCaption( strString );

	if( pGame->m_csStatus.GetMagicPower() < pGame->m_csStatus.GetBaseMagicPower() )	
	{
		m_pMagicPowerValueLabel->SetFontColor( 0xffff0000 );
	}
	else if ( pGame->m_csStatus.GetMagicPower() > pGame->m_csStatus.GetBaseMagicPower() )	
	{
		m_pMagicPowerValueLabel->SetFontColor( 0xff00ff00 );
	}
	else
	{
		m_pMagicPowerValueLabel->SetFontColor( 0xffffffff );
	}

	// 블록률 값
	sprintf(strString, "%4d", pGame->m_csStatus.GetBlock() );
	m_pBlockPowerValueLabel->SetCaption( strString ) ;

	if ( pGame->m_csStatus.GetBlock() < pGame->m_csStatus.GetBaseBlock() )
	{
		m_pBlockPowerValueLabel->SetFontColor( 0xffff0000 );
	} 
	else if( pGame->m_csStatus.GetBlock() > pGame->m_csStatus.GetBaseBlock() )
	{
		m_pBlockPowerValueLabel->SetFontColor( 0xff00ff00 );
	} 
	else
	{
		m_pBlockPowerValueLabel->SetFontColor( 0xffffffff );
	}

	sprintf(strString, CRYLStringTable::m_strString[ 2283 ], pGame->m_csStatus.GetBlockRate());
	m_pBlockPowerValueLabel->SetTooltip(  &m_pGMUIParentDlg->m_TooltipManager,  strString, 1792, TOOLTIP_PANEL ) ;

	// 크리티컬 확률
	sprintf(strString, "%3d", pGame->m_csStatus.GetCritical() ) ;
	m_pCriticalValueLabel->SetCaption( strString ) ;
	sprintf(strString,  CRYLStringTable::m_strString[ 2282 ], pGame->m_csStatus.GetCriticalRate(), pGame->m_csStatus.GetCriticalMagnification() ) ;
	m_pCriticalValueLabel->SetTooltip(  &m_pGMUIParentDlg->m_TooltipManager,  strString, 1792, TOOLTIP_PANEL ) ;

	if( pGame->m_csStatus.GetCritical() < pGame->m_csStatus.GetBaseCritical() )	
	{
		m_pCriticalValueLabel->SetFontColor( 0xffff0000 );
	}
	else if ( pGame->m_csStatus.GetCritical() > pGame->m_csStatus.GetBaseCritical() )	
	{
		m_pCriticalValueLabel->SetFontColor( 0xff00ff00 );
	}
	else
	{
		m_pCriticalValueLabel->SetFontColor( 0xffffffff );
	}

	// 마법저항 값
	sprintf(strString, "%4d", pGame->m_csStatus.GetMagicResist() ) ;
	m_pMagicResistValueLabel->SetCaption( strString ) ;

	sprintf(strString, CRYLStringTable::m_strString[ 2284 ], pGame->m_csStatus.GetMagicResistRate() ) ;
	m_pMagicResistValueLabel->SetTooltip(  &m_pGMUIParentDlg->m_TooltipManager,  strString, 1792, TOOLTIP_PANEL ) ;

	if( pGame->m_csStatus.GetMagicResist() < pGame->m_csStatus.GetBaseMagicResist() )	
	{
		m_pMagicResistValueLabel->SetFontColor( 0xffff0000 );
	}
	else if ( pGame->m_csStatus.GetMagicResist() > pGame->m_csStatus.GetBaseMagicResist() )	
	{
		m_pMagicResistValueLabel->SetFontColor( 0xff00ff00 );
	}
	else
	{
		m_pMagicResistValueLabel->SetFontColor( 0xffffffff );
	}

	return S_OK ;
}

VOID	CRYLStatusPanel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pStatusPanel )
		return ;

	switch( Sender->GetInstance() )
	{
	case RYLSTATUSPANEL::RANKING_BUTTON	:
		{
			if ( TRUE == ( ( CRYLGameScene* )CRYLSceneManager::GetCurrentScene() )->GetRankingDlg()->GetVisible() )
			{
				( ( CRYLGameScene* )CRYLSceneManager::GetCurrentScene() )->GetRankingDlg()->SetVisible( FALSE ) ;
			}
			else
			{
				SendPacket::CharRankingInfo( g_GameSession, CRYLNetworkData::Instance()->m_dwMyChrID, 0, 0 ) ;
			}
		}		
		break ;
	case RYLSTATUSPANEL::STRPOINTINC_BUTTON :
		SendPacket::CharIncreasePoint(g_GameSession, CRYLNetworkData::Instance()->m_dwMyChrID, ClientConstants::StateType_STR);
		break ;
	case RYLSTATUSPANEL::DEXPOINTINC_BUTTON :
		SendPacket::CharIncreasePoint(g_GameSession, CRYLNetworkData::Instance()->m_dwMyChrID, ClientConstants::StateType_DEX);
		break ;
	case RYLSTATUSPANEL::CONPOINTINC_BUTTON :
		SendPacket::CharIncreasePoint(g_GameSession, CRYLNetworkData::Instance()->m_dwMyChrID, ClientConstants::StateType_CON);
		break ;
	case RYLSTATUSPANEL::INTPOINTINC_BUTTON :
		SendPacket::CharIncreasePoint(g_GameSession, CRYLNetworkData::Instance()->m_dwMyChrID, ClientConstants::StateType_INT);
		break ;
	case RYLSTATUSPANEL::WISPOINTINC_BUTTON :
		SendPacket::CharIncreasePoint(g_GameSession, CRYLNetworkData::Instance()->m_dwMyChrID, ClientConstants::StateType_WIS);
		break ;
	case RYLSTATUSPANEL::HP_MP_BUTTON :
		{
			CRYLGameScene* pScene = static_cast<CRYLGameScene*>( CRYLSceneManager::GetCurrentScene() ) ;
			if ( pScene )
			{
				pScene->GetStatusDlg()->SetVisible( !pScene->GetStatusDlg()->GetVisible() ) ;

				// Visible Flag Setting
				pScene->SetVisibleInfo( RYLMAINWINDOW::STATUS_WINDOW, pScene->GetStatusDlg()->GetVisible() ) ;
			}
		}
		break ; 
	case RYLSTATUSPANEL::ENCHANT_BUTTON :
		{
			CRYLGameScene* pScene = static_cast<CRYLGameScene*>( CRYLSceneManager::GetCurrentScene() ) ;
			if ( pScene )
			{
				pScene->GetEnchantDlg()->SetVisible( !pScene->GetEnchantDlg()->GetVisible() ) ;

				// Visible Flag Setting
				pScene->SetVisibleInfo( RYLMAINWINDOW::ENCHANT_WINDOW, pScene->GetEnchantDlg()->GetVisible() ) ;
			}
		}
		break ;
	}
}
