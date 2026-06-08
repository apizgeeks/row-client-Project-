#include <winsock2.h>
#include "RYLEffectOptionPanel.h"
#include "RYLLabel.h"
#include "RYLButton.h"
#include "RYLCheckBox.h"
#include "RYLStringTable.h"
#include "RYLSpriteList.h"
#include "RYLImageCabinet.h"
#include "GMImageDib.h"
#include "RYLImage.h"
#include "RYLImageManager.h"
#include "RYLGameData.h"
#include "RYLOptionDlg.h"

#include "RYLClientMain.h"
#include "RYLGameScene.h"
#include "RYLSceneManager.h"
#include "RYLOptionDlg.h"
#include "RYLNetworkData.h"
#include <Network/ClientSocket/Send/SendEtc.h>
#include "GMMemory.h"

CRYLEffectOptionPanel*	g_pEffectOption = NULL;

CRYLEffectOptionPanel::CRYLEffectOptionPanel( CGMUICustomDlg *pParentDlg ): CGMUICustomPanel( pParentDlg )
{
	m_pSkillEffectLabel			= NULL;		// 스킬 이펙트
	m_pChantLabel				= NULL;		// 챈트, 인챈트 효과
	m_pEquipEffectLabel			= NULL;		// 장비품 효과	

	m_pOkButton					= NULL;		// 확인
	m_pDefaultButton			= NULL;		// 기본 설정
	m_pAlloffButton				= NULL;		// 다 끄기

	m_pSkillEffect01Label 		= NULL;		// 전체 보기
	m_pSkillEffect02Label 		= NULL;		// 파티것만 보기
	m_pSkillEffect03Label 		= NULL;		// 자기것만 보기
	m_pSkillEffect04Label 		= NULL;		// 자기것만 끄기

	m_pChantEffectGoodLabel 	= NULL;		// 긍정
	m_pChantEffectBadLabel		= NULL;		// 부정
	m_pChantEffect01Label		= NULL;		// 파티 
	m_pChantEffect02Label		= NULL;		// 아군 
	m_pChantEffect03Label		= NULL;		// 중립 
	m_pChantEffect04Label		= NULL;		// 적군 

	m_pEquipGradeEffectLabel	= NULL;		// 그레이드별 효과 보기

	m_pSkillEffect01Button 		= NULL;		// 전체 보기
	m_pSkillEffect02Button 		= NULL;		// 파티것만 보기
	m_pSkillEffect03Button 		= NULL;		// 자기것만 보기
	m_pSkillEffect04Button 		= NULL;		// 자기것만 끄기

	m_pChantEffect01_1Button 	= NULL;		// 파티 긍정
	m_pChantEffect02_1Button 	= NULL;		// 아군 긍정
	m_pChantEffect03_1Button 	= NULL;		// 중립 긍정
	m_pChantEffect04_1Button 	= NULL;		// 적군 긍정

	m_pChantEffect01_2Button 	= NULL;		// 파티 부정
	m_pChantEffect02_2Button 	= NULL;		// 아군 부정
	m_pChantEffect03_2Button 	= NULL;		// 중립 부정
	m_pChantEffect04_2Button 	= NULL;		// 적군 부정

	m_pEquipGradeEffectButton	= NULL;		// 그레이드별 효과 보기

	m_pGMUIParentDlg = pParentDlg;
	
	CGMImageDib*	pImageDIB = NULL;
	CRYLImage*		pImage	  = NULL;
	RECT			rt;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 395, &pImageDIB );
	pImageDIB->ClearbyColor( 0x00000000 );
	int i = 0;
	for ( ; i < 74; i ++ )
	{
		SetRect( &rt, 0, 38, 256, 43 );
		pImageDIB->DrawRect( &rt, 0, i * 5, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) );
	}
	SetRect( &rt, 0, 42, 256, 52 );
	pImageDIB->DrawRect( &rt, 0, ( i - 1 ) * 5, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 ) );

	pImageDIB->MakeFrameRect( 24, 24, 195, 88, 0xff6e6857, 0xc0000000, 1 );		
	pImageDIB->MakeFrameRect( 24, 137, 195, 138, 0xff6e6857, 0xc0000000, 1 );		
	pImageDIB->MakeFrameRect( 24, 299, 195, 30, 0xff6e6857, 0xc0000000, 1 );		
	
	pImage = CRYLImageManager::Instance()->CreateRYLImage();
	pImage->LoadFromImage( pImageDIB );
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage );
	
	AttachPanelImage( ( CGMTexture*)pImage );

	GM_DELETE( pImageDIB );
	
	SetWidth( 256 );
	SetHeight( 395 );

	g_pEffectOption = this;
}

CRYLEffectOptionPanel::~CRYLEffectOptionPanel() 
{
	FinalCtrl();
}

VOID	CRYLEffectOptionPanel::InitCtrl() 
{
	// 스킬 이펙트
	m_pSkillEffectLabel = new CRYLLabel( m_pGMUIParentDlg );
	m_pSkillEffectLabel->SetLeft( 27 );
	m_pSkillEffectLabel->SetTop( 9 );
	m_pSkillEffectLabel->SetFontColor (0xffffffff);
	m_pSkillEffectLabel->SetAutoSize( TRUE );
	m_pSkillEffectLabel->SetCaption( CRYLStringTable::m_strString[ 2171 ] );
	CGMUICustomPanel::AddChild( m_pSkillEffectLabel );

	// 챈트, 인챈트 효과
	m_pChantLabel = new CRYLLabel( m_pGMUIParentDlg );
	m_pChantLabel->SetLeft( 27 );
	m_pChantLabel->SetTop( 122 );
	m_pChantLabel->SetFontColor (0xffffffff);
	m_pChantLabel->SetAutoSize( TRUE );
	m_pChantLabel->SetCaption( CRYLStringTable::m_strString[ 2172 ] );
	CGMUICustomPanel::AddChild( m_pChantLabel );

	// 장비품 효과
	m_pEquipEffectLabel = new CRYLLabel( m_pGMUIParentDlg );
	m_pEquipEffectLabel->SetLeft( 27 );
	m_pEquipEffectLabel->SetTop( 284 );
	m_pEquipEffectLabel->SetFontColor (0xffffffff);
	m_pEquipEffectLabel->SetAutoSize( TRUE );
	m_pEquipEffectLabel->SetCaption( CRYLStringTable::m_strString[ 971 ] );
	CGMUICustomPanel::AddChild( m_pEquipEffectLabel );

	m_pChantEffectGoodLabel = new CRYLLabel( m_pGMUIParentDlg );
	m_pChantEffectGoodLabel->SetLeft( 82 );
	m_pChantEffectGoodLabel->SetTop( 148 );
	m_pChantEffectGoodLabel->SetFontColor (0xffffffff);
	m_pChantEffectGoodLabel->SetAutoSize( TRUE );
	m_pChantEffectGoodLabel->SetCaption( CRYLStringTable::m_strString[ 3148 ] );
	CGMUICustomPanel::AddChild( m_pChantEffectGoodLabel );

	m_pChantEffectBadLabel = new CRYLLabel( m_pGMUIParentDlg );
	m_pChantEffectBadLabel->SetLeft( 150 );
	m_pChantEffectBadLabel->SetTop( 148 );
	m_pChantEffectBadLabel->SetFontColor (0xffffffff);
	m_pChantEffectBadLabel->SetAutoSize( TRUE );
	m_pChantEffectBadLabel->SetCaption( CRYLStringTable::m_strString[ 3149 ] );
	CGMUICustomPanel::AddChild( m_pChantEffectBadLabel );

	// 전체 보기
	m_pSkillEffect01Label = new CRYLLabel( m_pGMUIParentDlg );
	m_pSkillEffect01Label->SetLeft( 78 );
	m_pSkillEffect01Label->SetTop( 34 );
	m_pSkillEffect01Label->SetFontColor (0xffffffff);
	m_pSkillEffect01Label->SetAutoSize( TRUE );
	m_pSkillEffect01Label->SetCaption( CRYLStringTable::m_strString[ 3150 ] );		
	CGMUICustomPanel::AddChild( m_pSkillEffect01Label );

	// 파티것만 보기
	m_pSkillEffect02Label = new CRYLLabel( m_pGMUIParentDlg );
	m_pSkillEffect02Label->SetLeft( 78 );
	m_pSkillEffect02Label->SetTop( 53 );
	m_pSkillEffect02Label->SetFontColor (0xffffffff);
	m_pSkillEffect02Label->SetAutoSize( TRUE );
	m_pSkillEffect02Label->SetCaption( CRYLStringTable::m_strString[ 3151 ] );		
	CGMUICustomPanel::AddChild( m_pSkillEffect02Label );

	// 자기것만 보기
	m_pSkillEffect03Label = new CRYLLabel( m_pGMUIParentDlg );
	m_pSkillEffect03Label->SetLeft( 78 );
	m_pSkillEffect03Label->SetTop( 72 );
	m_pSkillEffect03Label->SetFontColor (0xffffffff);
	m_pSkillEffect03Label->SetAutoSize( TRUE );
	m_pSkillEffect03Label->SetCaption( CRYLStringTable::m_strString[ 3152 ] );		
	CGMUICustomPanel::AddChild( m_pSkillEffect03Label );

	// 자기것만 끄기
	m_pSkillEffect04Label = new CRYLLabel( m_pGMUIParentDlg );
	m_pSkillEffect04Label->SetLeft( 78 );
	m_pSkillEffect04Label->SetTop( 91 );
	m_pSkillEffect04Label->SetFontColor (0xffffffff);
	m_pSkillEffect04Label->SetAutoSize( TRUE );
	m_pSkillEffect04Label->SetCaption( CRYLStringTable::m_strString[ 3153 ] );		
	CGMUICustomPanel::AddChild( m_pSkillEffect04Label );

	// 파티
	m_pChantEffect01Label= new CRYLLabel( m_pGMUIParentDlg );
	m_pChantEffect01Label->SetLeft( 41 );
	m_pChantEffect01Label->SetTop( 165 );
	m_pChantEffect01Label->SetFontColor (0xffffffff);
	m_pChantEffect01Label->SetAutoSize( TRUE );
	m_pChantEffect01Label->SetCaption( CRYLStringTable::m_strString[ 1962 ] ); 
	CGMUICustomPanel::AddChild( m_pChantEffect01Label);

	// 아군
	m_pChantEffect02Label= new CRYLLabel( m_pGMUIParentDlg );
	m_pChantEffect02Label->SetLeft( 41 );
	m_pChantEffect02Label->SetTop( 186 );
	m_pChantEffect02Label->SetFontColor (0xffffffff);
	m_pChantEffect02Label->SetAutoSize( TRUE );
	m_pChantEffect02Label->SetCaption( CRYLStringTable::m_strString[ 3154 ] ); 
	CGMUICustomPanel::AddChild( m_pChantEffect02Label);

	// 중립
	m_pChantEffect03Label= new CRYLLabel( m_pGMUIParentDlg );
	m_pChantEffect03Label->SetLeft( 41 );
	m_pChantEffect03Label->SetTop( 205 );
	m_pChantEffect03Label->SetFontColor (0xffffffff);
	m_pChantEffect03Label->SetAutoSize( TRUE );
	m_pChantEffect03Label->SetCaption( CRYLStringTable::m_strString[ 3155 ] ); 
	CGMUICustomPanel::AddChild( m_pChantEffect03Label);

	// 적군
	m_pChantEffect04Label= new CRYLLabel( m_pGMUIParentDlg );
	m_pChantEffect04Label->SetLeft( 41 );
	m_pChantEffect04Label->SetTop( 224 );
	m_pChantEffect04Label->SetFontColor (0xffffffff);
	m_pChantEffect04Label->SetAutoSize( TRUE );
	m_pChantEffect04Label->SetCaption( CRYLStringTable::m_strString[ 3156 ] ); 
	CGMUICustomPanel::AddChild( m_pChantEffect04Label);

	// 그레이드별 효과 보기
	m_pEquipGradeEffectLabel= new CRYLLabel( m_pGMUIParentDlg );
	m_pEquipGradeEffectLabel->SetLeft( 78 );
	m_pEquipGradeEffectLabel->SetTop( 307 );
	m_pEquipGradeEffectLabel->SetFontColor (0xffffffff);
	m_pEquipGradeEffectLabel->SetAutoSize( TRUE );
	m_pEquipGradeEffectLabel->SetCaption( CRYLStringTable::m_strString[ 972 ] ); 
	CGMUICustomPanel::AddChild( m_pEquipGradeEffectLabel);

	RECT rt;
	CGMImageDib* pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_COMPONENT01_256x256 );

	// 전체 보기
	m_pSkillEffect01Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pSkillEffect01Button->SetLeft( 42 );
	m_pSkillEffect01Button->SetTop( 32 );
	m_pSkillEffect01Button->SetInstance( SKILLEFFECT01_BUTTON );
	m_pSkillEffect01Button->SetSecondParent( this );
	m_pSkillEffect01Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect01Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect01Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect01Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect01Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	m_pSkillEffect01Button->SetChecked( TRUE );
	CGMUICustomPanel::AddChild( m_pSkillEffect01Button );

	// 파티것만 보기
	m_pSkillEffect02Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pSkillEffect02Button->SetLeft( 42 );
	m_pSkillEffect02Button->SetTop( 51 );
	m_pSkillEffect02Button->SetInstance( SKILLEFFECT02_BUTTON );
	m_pSkillEffect02Button->SetSecondParent( this );
	m_pSkillEffect02Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect02Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect02Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect02Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect02Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pSkillEffect02Button );

	// 자기것만 보기
	m_pSkillEffect03Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pSkillEffect03Button->SetLeft( 42 );
	m_pSkillEffect03Button->SetTop( 70 );
	m_pSkillEffect03Button->SetInstance( SKILLEFFECT03_BUTTON );
	m_pSkillEffect03Button->SetSecondParent( this );
	m_pSkillEffect03Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect03Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect03Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect03Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect03Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pSkillEffect03Button );

	// 자기것만 끄기
	m_pSkillEffect04Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pSkillEffect04Button->SetLeft( 42 );
	m_pSkillEffect04Button->SetTop( 89 );
	m_pSkillEffect04Button->SetInstance( SKILLEFFECT04_BUTTON );
	m_pSkillEffect04Button->SetSecondParent( this );
	m_pSkillEffect04Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect04Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect04Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pSkillEffect04Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pSkillEffect04Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pSkillEffect04Button );

	// -------------------------------------------------------------------------------------------------
	// 파티

	// 긍정
	m_pChantEffect01_1Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect01_1Button->SetLeft( 97 );
	m_pChantEffect01_1Button->SetTop( 165 );
	m_pChantEffect01_1Button->SetInstance( CHANTEFFECT01_1BUTTON );
	m_pChantEffect01_1Button->SetSecondParent( this );
	m_pChantEffect01_1Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect01_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect01_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect01_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect01_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	m_pChantEffect01_1Button->SetChecked( TRUE );
	CGMUICustomPanel::AddChild( m_pChantEffect01_1Button );
	
	// 부정
	m_pChantEffect01_2Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect01_2Button->SetLeft( 165 );
	m_pChantEffect01_2Button->SetTop( 165 );
	m_pChantEffect01_2Button->SetInstance( CHANTEFFECT01_2BUTTON );
	m_pChantEffect01_2Button->SetSecondParent( this );
	m_pChantEffect01_2Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect01_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect01_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect01_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect01_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	m_pChantEffect01_2Button->SetChecked( TRUE );
	CGMUICustomPanel::AddChild( m_pChantEffect01_2Button );

	// -------------------------------------------------------------------------------------------------
	// 아군

	// 긍정
	m_pChantEffect02_1Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect02_1Button->SetLeft( 97 );
	m_pChantEffect02_1Button->SetTop( 184 );
	m_pChantEffect02_1Button->SetInstance( CHANTEFFECT02_1BUTTON );
	m_pChantEffect02_1Button->SetSecondParent( this );
	m_pChantEffect02_1Button->OnMouseUpEvent = EffectOptionMouseUp;

	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect02_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect02_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect02_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect02_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pChantEffect02_1Button );
		
	// 부정
	m_pChantEffect02_2Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect02_2Button->SetLeft( 165 );
	m_pChantEffect02_2Button->SetTop( 184 );
	m_pChantEffect02_2Button->SetInstance( CHANTEFFECT02_2BUTTON );
	m_pChantEffect02_2Button->SetSecondParent( this );
	m_pChantEffect02_2Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect02_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect02_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect02_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect02_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pChantEffect02_2Button );

	// -------------------------------------------------------------------------------------------------
	// 중립

	// 긍정
	m_pChantEffect03_1Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect03_1Button->SetLeft( 97 );
	m_pChantEffect03_1Button->SetTop( 203 );
	m_pChantEffect03_1Button->SetInstance( CHANTEFFECT03_1BUTTON );
	m_pChantEffect03_1Button->SetSecondParent( this );
	m_pChantEffect03_1Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect03_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect03_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect03_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect03_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pChantEffect03_1Button );
		
	// 부정
	m_pChantEffect03_2Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect03_2Button->SetLeft( 165 );
	m_pChantEffect03_2Button->SetTop( 203 );
	m_pChantEffect03_2Button->SetInstance( CHANTEFFECT03_2BUTTON );
	m_pChantEffect03_2Button->SetSecondParent( this );
	m_pChantEffect03_2Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect03_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect03_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect03_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect03_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pChantEffect03_2Button );

	// -------------------------------------------------------------------------------------------------
	// 적군

	// 긍정
	m_pChantEffect04_1Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect04_1Button->SetLeft( 97 );
	m_pChantEffect04_1Button->SetTop( 222 );
	m_pChantEffect04_1Button->SetInstance( CHANTEFFECT04_1BUTTON );
	m_pChantEffect04_1Button->SetSecondParent( this );
	m_pChantEffect04_1Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect04_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect04_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect04_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect04_1Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pChantEffect04_1Button );
		
	// 부정
	m_pChantEffect04_2Button = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pChantEffect04_2Button->SetLeft( 165 );
	m_pChantEffect04_2Button->SetTop( 222 );
	m_pChantEffect04_2Button->SetInstance( CHANTEFFECT04_2BUTTON );
	m_pChantEffect04_2Button->SetSecondParent( this );
	m_pChantEffect04_2Button->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect04_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect04_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pChantEffect04_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pChantEffect04_2Button->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pChantEffect04_2Button );

	// 그레이드별 효과 보기
	m_pEquipGradeEffectButton = new CRYLCheckBox( m_pGMUIParentDlg );
	m_pEquipGradeEffectButton->SetLeft( 42 );
	m_pEquipGradeEffectButton->SetTop( 305 );
	m_pEquipGradeEffectButton->SetInstance( EQUIPGRADEEFFECT_BUTTON );
	m_pEquipGradeEffectButton->SetSecondParent( this );
	m_pEquipGradeEffectButton->OnMouseUpEvent = EffectOptionMouseUp;
	SetRect( &rt, 181, 220, 197, 236 );
	m_pEquipGradeEffectButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pEquipGradeEffectButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN );
	SetRect( &rt, 181, 238, 197, 254 );
	m_pEquipGradeEffectButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP );
	SetRect( &rt, 181, 220, 197, 236 );
	m_pEquipGradeEffectButton->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pEquipGradeEffectButton );

	pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 );
	CRYLSpriteList* pRYLButtonRect	= CRYLSpriteList::Instance();

	// 확인
	m_pOkButton = new CRYLButton( m_pGMUIParentDlg );
	m_pOkButton->SetLeft( 154 );
	m_pOkButton->SetTop( 345 );
	m_pOkButton->SetCaption( CRYLStringTable::m_strString[ 2164 ] );
	m_pOkButton->SetFontColor( 0xffffffff);
	m_pOkButton->SetColor( 0xffffffff);
	m_pOkButton->OnClickEvent = DlgClick;
	m_pOkButton->SetInstance( OK_BUTTON );
	m_pOkButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	m_pOkButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN );
	m_pOkButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP );
	m_pOkButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pOkButton );
	
	// 기본 설정
	m_pDefaultButton = new CRYLButton( m_pGMUIParentDlg );
	m_pDefaultButton->SetLeft( 69 );
	m_pDefaultButton->SetTop( 244 );
	m_pDefaultButton->SetCaption( CRYLStringTable::m_strString[ 3157 ] );
	m_pDefaultButton->SetFontColor( 0xffffffff);
	m_pDefaultButton->SetColor( 0xffffffff);
	m_pDefaultButton->OnClickEvent = DlgClick;
	m_pDefaultButton->SetInstance( DEFAULT_BUTTON );
	m_pDefaultButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	m_pDefaultButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN );
	m_pDefaultButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP );
	m_pDefaultButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pDefaultButton );
	
	// 다 끄기
	m_pAlloffButton = new CRYLButton( m_pGMUIParentDlg );
	m_pAlloffButton->SetLeft( 139 );
	m_pAlloffButton->SetTop( 244 );
	m_pAlloffButton->SetCaption( CRYLStringTable::m_strString[ 3158 ] );
	m_pAlloffButton->SetFontColor( 0xffffffff);
	m_pAlloffButton->SetColor( 0xffffffff);
	m_pAlloffButton->OnClickEvent = DlgClick;
	m_pAlloffButton->SetInstance( ALLOFF_BUTTON );
	m_pAlloffButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL );
	m_pAlloffButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN );
	m_pAlloffButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP );
	m_pAlloffButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE );
	CGMUICustomPanel::AddChild( m_pAlloffButton );
}


VOID	CRYLEffectOptionPanel::FinalCtrl() 
{
	GM_DELETE( m_pSkillEffectLabel );			// 스킬 이펙트
	GM_DELETE( m_pChantLabel );					// 챈트, 인챈트 효과
	GM_DELETE( m_pEquipEffectLabel );			// 장비품 효과

	GM_DELETE( m_pSkillEffect01Label );			// 전체 보기
	GM_DELETE( m_pSkillEffect02Label );			// 파티것만 보기
	GM_DELETE( m_pSkillEffect03Label );			// 자기것만 보기
	GM_DELETE( m_pSkillEffect04Label );			// 자기것만 끄기

	// 챈트 이펙트 옵션
	GM_DELETE( m_pChantEffectGoodLabel );		// 긍정
	GM_DELETE( m_pChantEffectBadLabel );		// 부정
	GM_DELETE( m_pChantEffect01Label);			// 전체 보기
	GM_DELETE( m_pChantEffect02Label);			// 긍정효과만 보기
	GM_DELETE( m_pChantEffect03Label);			// 부정효과만 보기
	GM_DELETE( m_pChantEffect04Label);			// 전체 보기 않음

	// 장비품 효과
	GM_DELETE( m_pEquipGradeEffectLabel );		// 그레이드별 효과 보기

	// 스킬 이펙트 옵션
	GM_DELETE( m_pSkillEffect01Button );		// 전체 보기
	GM_DELETE( m_pSkillEffect02Button );		// 파티것만 보기
	GM_DELETE( m_pSkillEffect03Button );		// 자기것만 보기
	GM_DELETE( m_pSkillEffect04Button );		// 자기것만 끄기

	// 챈트 이펙트 옵션
	GM_DELETE( m_pChantEffect01_1Button );		// 파티 긍정
	GM_DELETE( m_pChantEffect02_1Button );		// 아군 긍정
	GM_DELETE( m_pChantEffect03_1Button );		// 중립 긍정
	GM_DELETE( m_pChantEffect04_1Button );		// 적군 긍정
	GM_DELETE( m_pChantEffect01_2Button );		// 파티 부정
	GM_DELETE( m_pChantEffect02_2Button );		// 아군 부정
	GM_DELETE( m_pChantEffect03_2Button );		// 중립 부정
	GM_DELETE( m_pChantEffect04_2Button );		// 적군 부정

	// 장비품 효과
	GM_DELETE( m_pEquipGradeEffectButton );		// 그레이드별 효과 보기

	GM_DELETE( m_pOkButton );					// 확인
	GM_DELETE( m_pDefaultButton );				// 기본 설정
	GM_DELETE( m_pAlloffButton );				// 다 끄기
}

HRESULT		CRYLEffectOptionPanel::Update()
{
	return S_OK;
}

VOID	CRYLEffectOptionPanel::ReLoadOption() 
{
	m_pSkillEffect01Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwSkillEffect & 1 ) ? TRUE : FALSE );
	m_pSkillEffect02Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwSkillEffect & 2 ) ? TRUE : FALSE );
	m_pSkillEffect03Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwSkillEffect & 4 ) ? TRUE : FALSE );
	m_pSkillEffect04Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwSkillEffect & 8 ) ? TRUE : FALSE );

	m_pChantEffect01_1Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 1 ) ? TRUE : FALSE );
	m_pChantEffect02_1Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 2 ) ? TRUE : FALSE );
	m_pChantEffect03_1Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 4 ) ? TRUE : FALSE );
	m_pChantEffect04_1Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 8 ) ? TRUE : FALSE );
	m_pChantEffect01_2Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 16 ) ? TRUE : FALSE );
	m_pChantEffect02_2Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 32 ) ? TRUE : FALSE );
	m_pChantEffect03_2Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 64 ) ? TRUE : FALSE );
	m_pChantEffect04_2Button->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwChantEffect & 128 ) ? TRUE : FALSE );

	m_pEquipGradeEffectButton->SetChecked( ( CRYLOptionDlg::m_coOption.m_dwEquipEffect & 1 ) ? TRUE : FALSE );
}

VOID		CRYLEffectOptionPanel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pEffectOption )
		return;

	CRYLGameData* pGame = CRYLGameData::Instance();
	switch ( Sender->GetInstance() )
	{
		case OK_BUTTON:
		{
			CRYLGameScene*	pScene		= ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE );
			CRYLOptionDlg*  pOptionDlg	= static_cast<CRYLOptionDlg *>( pScene->GetOptionDlg() );

			pOptionDlg->OK();
			break;
		}

		case ALLOFF_BUTTON:
		{
			g_pEffectOption->m_pChantEffect01_1Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect02_1Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect03_1Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect04_1Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect01_2Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect02_2Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect03_2Button->SetChecked( FALSE );
			g_pEffectOption->m_pChantEffect04_2Button->SetChecked( FALSE );

			CRYLOptionDlg::m_coOption.m_dwChantEffect = 0;
			break;
		}

		case DEFAULT_BUTTON:
		{
			g_pEffectOption->m_pChantEffect01_1Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect02_1Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect03_1Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect04_1Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect01_2Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect02_2Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect03_2Button->SetChecked( TRUE );
			g_pEffectOption->m_pChantEffect04_2Button->SetChecked( TRUE );

			CRYLOptionDlg::m_coOption.m_dwChantEffect = 255;
			break;
		}
	}
}

VOID	CRYLEffectOptionPanel::EffectOptionMouseUp( CGMUIObject* Sender, INT Button, INT Shift, INT X, INT Y ) 
{
	if ( !g_pEffectOption )
		return;

	CRYLGameData* pGame = CRYLGameData::Instance();
	switch ( Sender->GetInstance() )
	{
		case SKILLEFFECT01_BUTTON:
		{
			if ( g_pEffectOption->m_pSkillEffect01Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwSkillEffect |= 1;
			else
				CRYLOptionDlg::m_coOption.m_dwSkillEffect ^= 1;
			pGame->PlayClickSound();
		}
		break;

		case SKILLEFFECT02_BUTTON:
		{
			if ( g_pEffectOption->m_pSkillEffect02Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwSkillEffect |= 2;
			else
				CRYLOptionDlg::m_coOption.m_dwSkillEffect ^= 2;
			pGame->PlayClickSound();
		}
		break;

		case SKILLEFFECT03_BUTTON:
		{
			if ( g_pEffectOption->m_pSkillEffect03Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwSkillEffect |= 4;
			else
				CRYLOptionDlg::m_coOption.m_dwSkillEffect ^= 4;
			pGame->PlayClickSound();
		}
		break;

		case SKILLEFFECT04_BUTTON:
		{
			if ( g_pEffectOption->m_pSkillEffect04Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwSkillEffect |= 8;
			else
				CRYLOptionDlg::m_coOption.m_dwSkillEffect ^= 8;
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT01_1BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect01_1Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 1;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 1;

			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT02_1BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect02_1Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 2;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 2;
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT03_1BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect03_1Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 4;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 4;
			
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT04_1BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect04_1Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 8;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 8;
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT01_2BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect01_2Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 16;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 16;
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT02_2BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect02_2Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 32;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 32;
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT03_2BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect03_2Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 64;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 64;
			pGame->PlayClickSound();
		}
		break;

		case CHANTEFFECT04_2BUTTON:
		{
			if ( g_pEffectOption->m_pChantEffect04_2Button->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwChantEffect |= 128;
			else
				CRYLOptionDlg::m_coOption.m_dwChantEffect ^= 128;
			pGame->PlayClickSound();
		}
		break;

		case EQUIPGRADEEFFECT_BUTTON:
		{
			if ( g_pEffectOption->m_pEquipGradeEffectButton->GetChecked() )
				CRYLOptionDlg::m_coOption.m_dwEquipEffect |= 1;
			else
				CRYLOptionDlg::m_coOption.m_dwEquipEffect ^= 1;
			pGame->PlayClickSound();
		}
		break;
	}
}