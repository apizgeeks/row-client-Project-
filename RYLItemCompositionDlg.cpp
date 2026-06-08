#include <winsock2.h>
#include "RYLMiniMapDlg.h"
#include "RYLButton.h"
#include "RYLLabel.h"
#include "RYLImageManager.h"
#include "RYLImageCabinet.h"
#include "RYLSpriteList.h"
#include "WinInput.h"
#include "RYLImage.h"
#include "RYLGameData.h"
#include "RYLSceneManager.h"
#include "RYLGameScene.h"
#include "RYLNetworkData.h"
#include "RYLStringTable.h"

#include "GMImageDib.h"
#include "BaseDataDefine.h"
#include "BaseGraphicsLayer.h"
#include <RenderOption.h>

#include "RYLGameScene.h"
#include "RYLSceneManager.h"
#include "RYLMessageBox.h"
#include "RYLStringGrid.h"
#include "RYLPicture.h"
#include "RYLMessageBoxManager.h"
#include "RYLChattingDlg.h"

#include "RYLItemCompositionDlg.h"
#include "item/ItemConstants.h"
#include "GMMemory.h"


CRYLItemCompositionDlg*		g_pItemCompositionDlg = NULL;
CRYLItemCompo1Panel*		g_pItemCompo1Panel = NULL;
CRYLItemCompo2Panel*		g_pItemCompo2Panel = NULL;
CRYLItemCompo3Panel*		g_pItemCompo3Panel = NULL;
CRYLItemCompo4Panel*		g_pItemCompo4Panel = NULL;

const DWORD		PHASE_1	=	0;
const DWORD		PHASE_2	=	1;
const DWORD		PHASE_3	=	2;
const DWORD		PHASE_4	=	3;

const unsigned int srcItemL = 12 +1;
const unsigned int srcItemR = srcItemL + 77;
const unsigned int srcItemT = 64 - 15 +1;
const unsigned int srcItemB = srcItemT + 102 ;


CRYLItemCompositionDlg::CRYLItemCompositionDlg() 
{
	m_dwPhaseState = PHASE_1;

	m_cAttribute = -1 ;
	m_pDlgLabel = NULL ;							// 다이얼로그 라벨

	m_pPhase1Panel = NULL ;							// Phase 1 판넬
	m_pPhase2Panel = NULL ;							// Phase 2 판넬
	m_pPhase4Panel = NULL ;							// Phase 4 판넬

	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 15, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 0, 0, 256, 15 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BLACKSMITH_256x256 ) ) ;
	
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	AttachCaptionTexture( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 222 -15, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	
	AttachWindowTexture( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;
	
	SetWidth( 256 ) ;
	SetHeight( 222 ) ;
	SetLeft( ( CGMUIScreen::GetInstance()->GetWidth() - GetWidth() ) / 2 ) ;
	SetTop( ( CGMUIScreen::GetInstance()->GetHeight() - GetHeight() ) / 2 ) ;
	SetFormType( RYLDLG_TYPE_CLOSEBUTTON ) ;

	g_pItemCompositionDlg = this;

	InitCtrl() ;
}

CRYLItemCompositionDlg::~CRYLItemCompositionDlg() 
{
	FinalCtrl() ;
	g_pItemCompositionDlg = NULL;
}

VOID	CRYLItemCompositionDlg::InitCtrl() 
{
	m_pPhase1Panel = new CRYLItemCompo1Panel( this );
	m_pPhase1Panel->SetLeft( 0 ) ;
	m_pPhase1Panel->SetTop( 15 ) ;
	m_pPhase1Panel->SetWidth( 256 ) ;
	m_pPhase1Panel->SetHeight( 222 - 15) ;
	((CRYLItemCompo1Panel*)m_pPhase1Panel)->InitCtrl();
	m_pPhase1Panel->SetVisible( FALSE ) ;
	AttachGMUIControl( m_pPhase1Panel  ) ;

	m_pPhase2Panel = new CRYLItemCompo2Panel( this );
	m_pPhase2Panel->SetLeft( 0 ) ;
	m_pPhase2Panel->SetTop( 15 ) ;
	m_pPhase2Panel->SetWidth( 256 ) ;
	m_pPhase2Panel->SetHeight( 266 - 15 ) ;
	((CRYLItemCompo2Panel*)m_pPhase2Panel)->InitCtrl();
	m_pPhase2Panel->SetVisible( FALSE ) ;
	AttachGMUIControl( m_pPhase2Panel  ) ;

	m_pPhase4Panel = new CRYLItemCompo4Panel( this );
	m_pPhase4Panel->SetLeft( 0 ) ;
	m_pPhase4Panel->SetTop( 15 ) ;
	m_pPhase4Panel->SetWidth( 256 ) ;
	m_pPhase4Panel->SetHeight( 222 - 15 ) ;
	((CRYLItemCompo4Panel*)m_pPhase4Panel)->InitCtrl();
	m_pPhase4Panel->SetVisible( FALSE ) ;
	AttachGMUIControl( m_pPhase4Panel  ) ;

	m_pDlgLabel= new CRYLLabel( this) ;
	m_pDlgLabel->SetLeft( 12 ) ;
	m_pDlgLabel->SetTop( 18 ) ;
	m_pDlgLabel->SetFontColor( 0xffffffff ) ;
	m_pDlgLabel->SetAutoSize( TRUE ) ;
	m_pDlgLabel->SetCaption( CRYLStringTable::m_strString[2305]) ;
	AttachGMUIControl( m_pDlgLabel) ;

	SetPhase();
}

HRESULT		CRYLItemCompositionDlg::Update() 
{
	return S_OK ;
}

VOID	CRYLItemCompositionDlg::FinalCtrl() 
{
	GM_DELETE( m_pDlgLabel ) ;								// 다이얼로그 라벨

	GM_DELETE( m_pPhase1Panel ) ;							// Phase 1 판넬
	GM_DELETE( m_pPhase2Panel ) ;							// Phase 2 판넬
	GM_DELETE( m_pPhase4Panel ) ;							// Phase 4 판넬

}

VOID	CRYLItemCompositionDlg::Show( CGMUIObject* Sender ) 
{
}

VOID	CRYLItemCompositionDlg::Hide( CGMUIObject* Sender ) 
{
}

VOID		
CRYLItemCompositionDlg::SetPhaseState2First()
{
	m_dwPhaseState = 0; 
}

VOID	
CRYLItemCompositionDlg::SetPhase()
{
	switch( m_dwPhaseState )
	{
		case PHASE_1:
			SetHeight( 222 ) ;
			((CRYLItemCompo1Panel*)m_pPhase1Panel)->SetVisible( TRUE );
			((CRYLItemCompo2Panel*)m_pPhase2Panel)->SetVisible( FALSE );
			((CRYLItemCompo4Panel*)m_pPhase4Panel)->SetVisible( FALSE );
			break;

		case PHASE_2:
			SetHeight( 266  ) ;
			((CRYLItemCompo1Panel*)m_pPhase1Panel)->SetVisible( FALSE );
			((CRYLItemCompo2Panel*)m_pPhase2Panel)->SetVisible( TRUE );
			((CRYLItemCompo4Panel*)m_pPhase4Panel)->SetVisible( FALSE );

			if(g_pItemCompo4Panel)
				g_pItemCompo4Panel->ResetText();
			break;

		case PHASE_3:
			SetHeight( 222 ) ;

			if(g_pItemCompo4Panel)
				g_pItemCompo4Panel->ResetText();

			((CRYLItemCompo1Panel*)m_pPhase1Panel)->SetVisible( FALSE );
			((CRYLItemCompo2Panel*)m_pPhase2Panel)->SetVisible( FALSE );
			((CRYLItemCompo4Panel*)m_pPhase4Panel)->SetVisible( TRUE );
			break;
	}
}

HRESULT		
CRYLItemCompositionDlg::Update( BOOL &bClick, BOOL &bEdge ) 
{
	if (FALSE == GetVisible())
	{
		return S_OK;
	}

	CRYLGameData* pGame			= CRYLGameData::Instance() ;
	CRYLNetworkData* pNetwork  = CRYLNetworkData::Instance() ;

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

	switch( m_dwPhaseState )
	{
		case PHASE_1:
			((CRYLItemCompo1Panel*)m_pPhase1Panel)->Update( bClick, bEdge );
			break;

		case PHASE_2:
			((CRYLItemCompo2Panel*)m_pPhase2Panel)->Update( bClick, bEdge );
			break;

		case PHASE_3:
			((CRYLItemCompo4Panel*)m_pPhase4Panel)->Update( bClick, bEdge );
			break;
	}

	return S_OK ;
}

VOID	
CRYLItemCompositionDlg::GMUIPaint( INT xx, INT yy ) 
{
	CRYLDialog::GMUIPaint( xx, yy ) ;
}

	
VOID		
// 한단계 다음 단계로. 4 단계에서 쓰면 OK()로. 
CRYLItemCompositionDlg::NextPhase()
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	switch( m_dwPhaseState )
	{
		case PHASE_1 :
		{
			if ( pGame->m_csStatus.m_lpCompositionOrigin )
			{
				m_dwPhaseState++;
				((CRYLItemCompo2Panel*)m_pPhase2Panel)->InitStringGrid( NULL );
				((CRYLItemCompo2Panel*)m_pPhase2Panel)->m_pPrevButton->SetEnable( TRUE );
			}
			break;
		}

		case PHASE_2:
		{
			if ( pGame->m_csStatus.m_lpCompositionOffering )
			{
				if ( pGame->m_csStatus.m_lpCompositionOffering->GetItemType() ==  
					pGame->m_csStatus.m_lpCompositionOrigin->GetItemType() )
				{
					if ( GetOption() == -1 )
					{
						return;
					}

					GM_DELETE( pGame->m_csStatus.m_lpCompositionTempResult );

					Item::ItemPos pos;
					pos.m_cIndex = Item::ExtraSpacePos::GRAFT_RESULT_POS ;
					pos.m_cPos = TakeType::TS_EXTRA;

					CItemInstance* pItemInstance = new CItemInstance;
					pItemInstance->SetItemInfo(pGame->m_csStatus.m_lpCompositionOrigin, pos);

					Item::CEquipment* lpItemOrigin = Item::CEquipment::DowncastToEquipment( pItemInstance->m_lpItemBase );
					Item::CEquipment* lpItemOffer = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOffering->m_lpItemBase );

					if (NULL == lpItemOrigin || NULL == lpItemOffer)
					{
						delete pItemInstance;
						return;
					}

					unsigned long dwUsedGold = 0;

					bool bUpgradeLevelLimit = false;
					unsigned char cErr = lpItemOrigin->OptionGraft( lpItemOffer, bUpgradeLevelLimit, GetOption(), pGame->m_csStatus.GetGold(), dwUsedGold );

					if ( Item::CEquipment::S_SUCCESS != cErr )
					{
						if (cErr == Item::CEquipment::EQUIPMENT_ERROR::E_NOT_UPGRADE_LEVEL)
						{
							// 제련 단계가 0 이거나 10 인 것만 옵션 이식 가능(원본)
							CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
							lpMessageBox->Create(CRYLStringTable::m_strString[3791]);
						}												

						delete pItemInstance;
						return;
					}

					delete pItemInstance;			

					++m_dwPhaseState;

					OK();
				}
			}
			break;
		}
		case PHASE_3:
		{
			break;
		}
	}

	SetPhase();
}
	
VOID		
// 한단계 전 페이즈로. 1 단계에서 쓰면 Cancel()로
CRYLItemCompositionDlg::PrevPhase()
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	if ( pGame->m_csStatus.m_lpCompositionResult )
		return;

	switch( m_dwPhaseState )
	{
		case PHASE_2:
			m_dwPhaseState--;
			break;

		case PHASE_3:
			m_dwPhaseState--;
			break;
	}

	SetPhase();
}
	
VOID		
// 다이얼로그를 닫고 페이즈를 1 단계로!
CRYLItemCompositionDlg::Cancel()
{
	m_dwPhaseState = 0;
	SetPhase();
	SetVisible( FALSE );
}

VOID		
// 이식 시도 하고 다이얼로그는 그대로. 페이즈 변함 없음
CRYLItemCompositionDlg::OK()
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;

	if ( pGame->m_csStatus.m_lpCompositionOffering && pGame->m_csStatus.m_lpCompositionOrigin )
	{
		g_pClientSocket->SetStatusFlag(NS_ITEMUPGRADE);

		pGame->m_lpItemComposition = new CEffScript;
		pGame->m_lpItemComposition->SetInterfaceSet(true);
		pGame->m_lpItemComposition->GetScriptBinData("inter_enhance_start.esf");
		
		pGame->m_lpItemComposition->m_InterfacePos[0] = GetLeft() + 47 ;
		pGame->m_lpItemComposition->m_InterfacePos[1] = GetTop() + 250  ;

		CSceneManager::m_EffectManager.AddInterfaceScript(pGame->m_lpItemComposition);
	}	

}

VOID		
CRYLItemCompositionDlg::SetVisible( BOOL bVisible )
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	m_cAttribute = -1;

	if ( bVisible )
	{
		if ( pGame->m_csStatus.m_lpCompositionResult )
		{
			m_dwPhaseState = PHASE_3;
			SetPhase();
		}
		else if ( pGame->m_csStatus.m_lpCompositionOffering )
		{
			m_dwPhaseState = PHASE_2;
			SetPhase();
		}
		else
		{
			m_dwPhaseState = PHASE_1;
			GM_DELETE( pGame->m_csStatus.m_lpCompositionTempResult );
			SetPhase();
		}
	}

	CRYLDialog::SetVisible( bVisible );
}

VOID	
CRYLItemCompositionDlg::DlgClick( CGMUIObject* Sender, INT X, INT Y )
{
	if ( !g_pItemCompositionDlg )
		return ;

	switch( Sender->GetInstance() )
	{
		case RYLITEMCOMPOSITIONDLG::MEMBER_BASE_STRINGGRID:			break;
		case RYLITEMCOMPOSITIONDLG::MEMBER_OFFERING_STRINGGRID:		break;
		case RYLITEMCOMPOSITIONDLG::MEMBER_COMPOSITION_STRINGGRID:	break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//	Phase1
///////////////////////////////////////////////////////////////////////////////

CRYLItemCompo1Panel::CRYLItemCompo1Panel( CGMUICustomDlg *pParentDlg ) : CGMUICustomPanel( pParentDlg )
{
	m_pGMUIParentDlg = pParentDlg;
	g_pItemCompo1Panel = this;

	m_pCancelButton = NULL;
	m_pNextButton = NULL;
	
	m_pPhaseLabel = NULL;
	m_pDescription1Label = NULL;
	m_pDescription2Label = NULL;
	m_pItemSlotPicture = NULL;

	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;
	CRYLSpriteList::Instance()->CreateGMImage( 256, 222 -15, &pImageDIB ) ;

	int i;
	CGMImageDib*	pTempImage;
	RECT rt ;
	pTempImage = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_GUILD_256x256 );
	for ( i = 0 ; i < 40 ; i ++ )
	{
		SetRect( &rt, 0, 38, 184, 43 ) ;
		pImageDIB->DrawRect( &rt, 0, i * 5, pTempImage ) ;
		SetRect( &rt, 72, 53, 144, 58 ) ;
		pImageDIB->DrawRect( &rt, 184, i * 5, pTempImage ) ;
	}
	SetRect( &rt, 0, 43, 184, 53 ) ;
	pImageDIB->DrawRect( &rt, 0, (i * 5) -3, pTempImage ) ;
	SetRect( &rt, 144, 53, 216, 63 ) ;
	pImageDIB->DrawRect( &rt, 184, (i * 5) -3, pTempImage  ) ;

	pImageDIB->MakeFrameRect( 2,    20, 252,   1, 0xffB99E6B, 0x80000000,  1 ) ;

	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	
	AttachPanelImage( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;
}

CRYLItemCompo1Panel::~CRYLItemCompo1Panel()
{
	FinalCtrl();
	g_pItemCompo1Panel = NULL;
}

VOID
CRYLItemCompo1Panel::InitCtrl()
{
	SetWidth( 256 ) ;
	SetHeight( 222 -15 ) ;

	CGMImageDib*			pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList*			pRYLButtonRect	= CRYLSpriteList::Instance() ;

	// 취소 버튼
	m_pCancelButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pCancelButton->SetLeft( 99 ) ;
	m_pCancelButton->SetTop( 189  - 15) ;
	m_pCancelButton->SetCaption( CRYLStringTable::m_strString[ 1897 ] ) ;
	m_pCancelButton->SetFontColor( 0xffffffff) ;
	m_pCancelButton->SetColor( 0xffffffff) ;
	m_pCancelButton->OnClickEvent = DlgClick ;
	m_pCancelButton->SetInstance( RYLITEMCOMPO1PANEL::CANCEL_BUTTON ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pCancelButton ) ;	

	// 다음 버튼
	m_pNextButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pNextButton->SetLeft( 173 ) ;
	m_pNextButton->SetTop( 189  - 15) ;
	m_pNextButton->SetCaption( CRYLStringTable::m_strString[2299]) ;
	m_pNextButton->SetFontColor( 0xffffffff) ;
	m_pNextButton->SetColor( 0xffffffff) ;
	m_pNextButton->OnClickEvent = DlgClick ;
	m_pNextButton->SetInstance( RYLITEMCOMPO1PANEL::NEXT_BUTTON ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pNextButton ) ;

	// 페이즈 라벨
	m_pPhaseLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pPhaseLabel->SetLeft( 21 ) ;
	m_pPhaseLabel->SetTop( 48 - 15 ) ;
	m_pPhaseLabel->SetFontColor( 0xffffffff ) ;
	m_pPhaseLabel->SetAutoSize( TRUE ) ;
	m_pPhaseLabel->SetCaption( CRYLStringTable::m_strString[2302] ) ;
	CGMUICustomPanel::AddChild( m_pPhaseLabel ) ;

	// 설명 첫째줄
	m_pDescription1Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription1Label->SetLeft( 102 ) ;
	m_pDescription1Label->SetTop( 64 - 15 ) ;
	m_pDescription1Label->SetFontColor( 0xffffffff ) ;
	m_pDescription1Label->SetAutoSize( TRUE ) ;
	m_pDescription1Label->SetCaption( CRYLStringTable::m_strString[2303] ) ;
	CGMUICustomPanel::AddChild( m_pDescription1Label ) ;

	// 설명 둘째줄
	m_pDescription2Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription2Label->SetLeft( 102 ) ;
	m_pDescription2Label->SetTop( 81 - 15 ) ;
	m_pDescription2Label->SetFontColor( 0xffffffff ) ;
	m_pDescription2Label->SetAutoSize( TRUE ) ;
	m_pDescription2Label->SetCaption( CRYLStringTable::m_strString[2304] ) ;
	CGMUICustomPanel::AddChild( m_pDescription2Label ) ;

	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 80, 106, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 8, 74, 87, 179 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BLACKSMITH_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	m_pItemSlotPicture = new CRYLPicture( m_pGMUIParentDlg ) ;
	m_pItemSlotPicture->SetLeft( 12 ) ;
	m_pItemSlotPicture->SetTop( 64 -15 ) ;
	m_pItemSlotPicture->SetWidth( 80 ) ;
	m_pItemSlotPicture->SetHeight( 106 ) ;
	m_pItemSlotPicture->AttachImageTexture( (CGMTexture* )pImage ) ;
	
	GM_DELETE( pImageDIB ) ;
	CGMUICustomPanel::AddChild( m_pItemSlotPicture ) ;
}

VOID
CRYLItemCompo1Panel::FinalCtrl()
{
	GM_DELETE( m_pCancelButton ) ;
	GM_DELETE( m_pNextButton ) ;
	
	GM_DELETE( m_pPhaseLabel ) ;
	GM_DELETE( m_pDescription1Label ) ;
	GM_DELETE( m_pDescription2Label ) ;
	GM_DELETE( m_pItemSlotPicture );
}

VOID 
CRYLItemCompo1Panel::GMUIPaint( INT xx, INT yy ) 
{
	CGMUICustomPanel::GMUIPaint( xx, yy ) ;
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	LPDIRECT3DDEVICE8   lpD3DDevice = BaseGraphicsLayer::GetDevice() ;

	unsigned int left = m_pGMUIParentDlg->GetLeft() + GetLeft();
	unsigned int top = m_pGMUIParentDlg->GetTop() + GetTop();

	if (NULL != pGame->m_lpPickItem)
	{
		if (g_DeviceInput.InRect( left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB))
		{
			if (true == pGame->m_lpPickItem->GetIsEnableAddOption())
			{
				pGame->RenderRect(lpD3DDevice, left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB, 0xFF000080, 0x80);
			} 
			else
			{
				pGame->RenderRect(lpD3DDevice, left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB, 0xFF800000, 0x80);
			}
		} 
	}

	if ( pGame->m_csStatus.m_lpCompositionOrigin )
	{
		pGame->m_csStatus.m_lpCompositionOrigin->m_wPutX = left + srcItemL +
			((77 / 2) - (pGame->m_csStatus.m_lpCompositionOrigin->m_lpSprite->GetSizeX() / 2));
		pGame->m_csStatus.m_lpCompositionOrigin->m_wPutY = top + srcItemT +
			((103 / 2) - (pGame->m_csStatus.m_lpCompositionOrigin->m_lpSprite->GetSizeY() / 2));
		pGame->m_csStatus.m_lpCompositionOrigin->Render(lpD3DDevice);
	}
}

HRESULT
CRYLItemCompo1Panel::Update( BOOL &bClick, BOOL &bEdge )
{
	CRYLGameData*		pGame		= CRYLGameData::Instance() ;
	POINT*				ptMousePos	= g_DeviceInput.GetMouseLocal();
	CRYLNetworkData*	pNetwork	= CRYLNetworkData::Instance() ;

	unsigned int left = m_pGMUIParentDlg->GetLeft() + GetLeft();
	unsigned int top = m_pGMUIParentDlg->GetTop() + GetTop();

	if (g_DeviceInput.InRect( left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB))
	{
		if (g_DeviceInput.GetIsLeftMouseDown())
		{
			if (NULL != pGame->m_lpPickItem)
			{
				if (true == pGame->m_lpPickItem->GetIsEnableAddOption())
				{
					Item::ItemPos pItemIndex;
					pItemIndex.m_cIndex = Item::ExtraSpacePos::GRAFT_ORIGINAL_POS;
					pItemIndex.m_cPos = TakeType::TS_EXTRA;

					if (NULL == pGame->m_csStatus.m_lpCompositionOrigin)
					{
						// 아이템 이동
						if (pGame->m_lpPickItem->m_lpItemBase->GetPos() == pItemIndex)
						{
							pGame->m_csStatus.m_lpCompositionOrigin = pGame->m_lpPickItem;
							pGame->SetPickItem(NULL);
						} 
						else
						{
							pNetwork->SendTakeItem( pGame->m_lpPickItem, &pItemIndex, 1);
						}
					} 
					else
					{
						// 스왑하기
						if ( pGame->m_csStatus.m_lpCompositionOffering )
						{
							return S_OK;
						}

						pNetwork->SendSwapItem(pGame->m_lpPickItem, &pItemIndex, pGame->m_csStatus.m_lpCompositionOrigin );
					}
				}
			}
			else
			{
				if (pGame->m_dwKindCursor == CURSOR_NORMAL && pGame->m_csStatus.m_lpCompositionOrigin )
				{
					if ( pGame->m_csStatus.m_lpCompositionOffering )
					{
						return S_OK;
					}

					pGame->SetPickItem(pGame->m_csStatus.m_lpCompositionOrigin);
					pGame->m_csStatus.m_lpCompositionOrigin = NULL;
				}

			}

			return S_OK;
		}
	}

	return S_OK;
}

VOID	
CRYLItemCompo1Panel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pItemCompo1Panel )
		return ;

	switch( Sender->GetInstance() )
	{
		case RYLITEMCOMPO1PANEL::NEXT_BUTTON :
			((CRYLItemCompositionDlg*)g_pItemCompo1Panel->m_pGMUIParentDlg)->NextPhase();
			break;

		case RYLITEMCOMPO1PANEL::CANCEL_BUTTON :
			((CRYLItemCompositionDlg*)g_pItemCompo1Panel->m_pGMUIParentDlg)->Cancel();
			break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//	Phase2
///////////////////////////////////////////////////////////////////////////////

CRYLItemCompo2Panel::CRYLItemCompo2Panel( CGMUICustomDlg *pParentDlg ) : CGMUICustomPanel( pParentDlg )
{
	m_pGMUIParentDlg = pParentDlg;
	g_pItemCompo2Panel = this;

	m_pPrevButton = NULL;
	m_pNextButton = NULL;
	
	m_pPhaseLabel = NULL;
	m_pItemSlotPicture = NULL;

	m_pCostLabel = NULL ;
	m_pOptionLabel = NULL ;
	m_pOptionOptLabel = NULL ;							// 옵션
	m_pOptionOriginLabel  = NULL ;						// 원본
	m_pOptionBonusLabel = NULL ;						// 보너스

	m_nOfferingCount = 0;
	m_nOfferingSelected = -1;
	memset( m_aryOffering, 0, sizeof(int) * 8 );
	memset( m_aryCanComposition, 0, sizeof( bool ) * 8 );

	m_dwCheckLimitResult = 0;
}

CRYLItemCompo2Panel::~CRYLItemCompo2Panel()
{
	FinalCtrl();
	g_pItemCompo2Panel = NULL;
}

VOID
CRYLItemCompo2Panel::InitCtrl()
{
	SetWidth( 256 ) ;
	SetHeight( 266 - 15 ) ;

	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 266 -15, &pImageDIB ) ;
	int i;
	CGMImageDib*	pTempImage;
	pTempImage = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_GUILD_256x256 );
	for ( i = 0 ; i < 49 ; i ++ )
	{
		SetRect( &rt, 0, 38, 184, 43 ) ;
		pImageDIB->DrawRect( &rt, 0, i * 5, pTempImage ) ;
		SetRect( &rt, 72, 53, 144, 58 ) ;
		pImageDIB->DrawRect( &rt, 184, i * 5, pTempImage ) ;
	}
	SetRect( &rt, 0, 43, 184, 53 ) ;
	pImageDIB->DrawRect( &rt, 0, (i * 5) -4 , pTempImage ) ;
	SetRect( &rt, 144, 53, 216, 63 ) ;
	pImageDIB->DrawRect( &rt, 184, (i * 5) -4, pTempImage  ) ;

	pImageDIB->MakeFrameRect( 2,    20, 252,   1, 0xffB99E6B, 0x80000000,  1 ) ;

	pImageDIB->MakeFrameRect( 97, 64-15, 146, 127, 0xffB99E6B, 0x80000000, 1 );		// line box
	pImageDIB->DrawFrameRect( 102, 87 -15, 136, 2, 0xffB99E6B );					// line

	SetRect( &rt, 126, 74, 245, 94 ) ;
	pImageDIB->DrawRect( &rt, 124, 196-15, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BLACKSMITH_256x256 ) ) ;			// 돈 아이콘
	
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	
	AttachPanelImage( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;

	CGMImageDib*			pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList*			pRYLButtonRect	= CRYLSpriteList::Instance() ;

	// 이전 버튼
	m_pPrevButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pPrevButton->SetLeft( 61 ) ;
	m_pPrevButton->SetTop( 231 - 15 ) ;
	m_pPrevButton->SetCaption( CRYLStringTable::m_strString[2298] ) ;
	m_pPrevButton->SetFontColor( 0xffffffff) ;
	m_pPrevButton->SetColor( 0xffffffff) ;
	m_pPrevButton->OnClickEvent = DlgClick ;
	m_pPrevButton->SetInstance( RYLITEMCOMPO2PANEL::PREV_BUTTON ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pPrevButton ) ;
		
	// 다음 버튼
	m_pNextButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pNextButton->SetLeft( 135 ) ;
	m_pNextButton->SetTop( 231 - 15 ) ;
	m_pNextButton->SetCaption( CRYLStringTable::m_strString[2300]) ;
	m_pNextButton->SetFontColor( 0xffffffff) ;
	m_pNextButton->SetColor( 0xffffffff) ;
	m_pNextButton->OnClickEvent = DlgClick ;
	m_pNextButton->SetInstance( RYLITEMCOMPO2PANEL::NEXT_BUTTON ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pNextButton ) ;

	// 페이즈 라벨
	m_pPhaseLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pPhaseLabel->SetLeft( 22 ) ;
	m_pPhaseLabel->SetTop( 48 - 15 ) ;
	m_pPhaseLabel->SetFontColor( 0xffffffff ) ;
	m_pPhaseLabel->SetAutoSize( TRUE ) ;
	m_pPhaseLabel->SetCaption( CRYLStringTable::m_strString[2301] ) ;
	CGMUICustomPanel::AddChild( m_pPhaseLabel ) ;

	// 옵션 비용
	m_pCostLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pCostLabel->SetLeft( 161 ) ;
	m_pCostLabel->SetTop( 201 - 15 ) ;
	m_pCostLabel->SetFontColor( 0xffffffff ) ;
	m_pCostLabel->SetAutoSize( TRUE ) ;
	m_pCostLabel->SetCaption( "" ) ;
	CGMUICustomPanel::AddChild( m_pCostLabel ) ;
	
	// 옵션 선택
	m_pOptionLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pOptionLabel->SetLeft( 146 ) ;
	m_pOptionLabel->SetTop( 48 - 15 ) ;
	m_pOptionLabel->SetFontColor( 0xffffffff ) ;
	m_pOptionLabel->SetAutoSize( TRUE ) ;
	m_pOptionLabel->SetCaption( CRYLStringTable::m_strString[2700] ) ;
	CGMUICustomPanel::AddChild( m_pOptionLabel ) ;

	// 옵션
	m_pOptionOptLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pOptionOptLabel->SetLeft( 121 ) ;
	m_pOptionOptLabel->SetTop( 71 - 15 ) ;
	m_pOptionOptLabel->SetFontColor( 0xffffffff ) ;
	m_pOptionOptLabel->SetAutoSize( TRUE ) ;
	m_pOptionOptLabel->SetCaption( CRYLStringTable::m_strString[2701] ) ;
	CGMUICustomPanel::AddChild( m_pOptionOptLabel ) ;

	// 원본
	m_pOptionOriginLabel   = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pOptionOriginLabel->SetLeft( 173 ) ;
	m_pOptionOriginLabel->SetTop( 71 - 15 ) ;
	m_pOptionOriginLabel->SetFontColor( 0xffffffff ) ;
	m_pOptionOriginLabel->SetAutoSize( TRUE ) ;
	m_pOptionOriginLabel->SetCaption( CRYLStringTable::m_strString[2702] ) ;
	CGMUICustomPanel::AddChild( m_pOptionOriginLabel ) ;

	// 보너스
	m_pOptionBonusLabel   = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pOptionBonusLabel->SetLeft( 205 ) ;
	m_pOptionBonusLabel->SetTop( 71 - 15 ) ;
	m_pOptionBonusLabel->SetFontColor( 0xffffffff ) ;
	m_pOptionBonusLabel->SetAutoSize( TRUE ) ;
	m_pOptionBonusLabel->SetCaption( CRYLStringTable::m_strString[2703] ) ;
	CGMUICustomPanel::AddChild( m_pOptionBonusLabel ) ;

	// 제물 아이템 리스트
	m_pOfferingStringGrid = new CRYLStringGrid( m_pGMUIParentDlg, FALSE ) ;
	m_pOfferingStringGrid->SetLeft( 105 ) ;
	m_pOfferingStringGrid->SetTop( 93 - 15 ) ;
	m_pOfferingStringGrid->SetWidth( 144 ) ;
	m_pOfferingStringGrid->SetHeight( 112 ) ;
	m_pOfferingStringGrid->SetColCount( 3 ) ;
	m_pOfferingStringGrid->SetItemHeight( 16 ) ;
	m_pOfferingStringGrid->SetFontColor( 0xffffffff ) ;
	m_pOfferingStringGrid->SetInstance( RYLITEMCOMPOSITIONDLG::MEMBER_COMPOSITION_STRINGGRID ) ;
	m_pOfferingStringGrid->OnClickEvent = DlgClick ;
	CGMUICustomPanel::AddChild( m_pOfferingStringGrid ) ;

	CRYLSpriteList::Instance()->CreateGMImage( 80, 106, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 8, 74, 87, 179 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BLACKSMITH_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	m_pItemSlotPicture = new CRYLPicture( m_pGMUIParentDlg ) ;
	m_pItemSlotPicture->SetLeft( 12 ) ;
	m_pItemSlotPicture->SetTop( 64 -15 ) ;
	m_pItemSlotPicture->SetWidth( 80 ) ;
	m_pItemSlotPicture->SetHeight( 106 ) ;
	m_pItemSlotPicture->AttachImageTexture( (CGMTexture* )pImage ) ;
	
	GM_DELETE( pImageDIB ) ;
	CGMUICustomPanel::AddChild( m_pItemSlotPicture ) ;
}

VOID
CRYLItemCompo2Panel::FinalCtrl()
{
	GM_DELETE( m_pPrevButton ) ;
	GM_DELETE( m_pNextButton ) ;
	
	GM_DELETE( m_pPhaseLabel ) ;
	GM_DELETE( m_pCostLabel );
	GM_DELETE( m_pOptionLabel );
	GM_DELETE( m_pOptionOptLabel );							// 옵션
	GM_DELETE( m_pOptionOriginLabel );						// 원본
	GM_DELETE( m_pOptionBonusLabel );						// 보너스

	GM_DELETE( m_pItemSlotPicture  ) ;
	GM_DELETE( m_pOfferingStringGrid ) ;
}

int		
CRYLItemCompo2Panel::SelectStringGrid()
{
	// edith 옵션이식 비용.
	int index = m_pOfferingStringGrid->GetItemIndex();
	if ( index == -1 )
	{
		m_pOfferingStringGrid->SelectItemIndex( -1 );
		m_nOfferingSelected = -1; 
	}
	else if ( !m_aryCanComposition[index] )
	{
		m_pOfferingStringGrid->SelectItemIndex( m_nOfferingSelected );
	}
	else
	{
		m_nOfferingSelected = index ;
	}

	CRYLGameData* pGame = CRYLGameData::Instance() ; 


	// 최소데미지 값일떄 최대 데미지 값보다 커지며 안된다.
	if(m_nOfferingSelected == 0 && m_aryOffering[0] == Item::Attribute::MIN_DAMAGE)
	{
		Item::CEquipment *lpItem = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOffering->m_lpItemBase );
		if ( !lpItem )
		{
			m_pOfferingStringGrid->SelectItemIndex( -1 );
			m_nOfferingSelected = -1; 
			return -1;
		}

		Item::CEquipment *lpItemOrigin = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOrigin->m_lpItemBase );
		if ( !lpItemOrigin )
		{
			m_pOfferingStringGrid->SelectItemIndex( -1 );
			m_nOfferingSelected = -1; 
			return -1;
		}

		int	origin;
		int	originmax;
		int offering;
		int addattr;
		
		origin = lpItemOrigin->GetAttribute( Item::Attribute::Type( Item::Attribute::MIN_DAMAGE ) );
		originmax = lpItemOrigin->GetAttribute( Item::Attribute::Type( Item::Attribute::MAX_DAMAGE ) );
		offering = lpItem->GetAttribute( Item::Attribute::Type( Item::Attribute::MIN_DAMAGE )) ;
		addattr = (offering - origin) / 2;

		if (origin+addattr > originmax)
		{
			m_pOfferingStringGrid->SelectItemIndex( -1 );
			m_nOfferingSelected = -1; 
		}
	}

	if ( m_nOfferingSelected == -1 )
	{
		return -1;
	}

	Item::ItemPos pos;
	pos.m_cIndex = Item::ExtraSpacePos::GRAFT_RESULT_POS ;
	pos.m_cPos = TakeType::TS_EXTRA;

	CItemInstance* pItemInstance = new CItemInstance;
	pItemInstance->SetItemInfo(pGame->m_csStatus.m_lpCompositionOrigin, pos);

	Item::CEquipment *lpItemOrigin = Item::CEquipment::DowncastToEquipment( pItemInstance->m_lpItemBase );
	Item::CEquipment *lpItemOffer = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOffering->m_lpItemBase );

	unsigned long dwUsedGold = 0;
	char buff[MAX_PATH] = { 0, }; 

	// 업그레이드레벨의 제한. true면 제한을 건다 구USA버젼에 포함.
	bool bUpgradeLevelLimit = false;

	// 아이템 옵션이식이의 가격 책정.
	if ( Item::CEquipment::S_SUCCESS != lpItemOrigin->OptionGraft( lpItemOffer, bUpgradeLevelLimit, 
		m_aryOffering[m_nOfferingSelected], pGame->m_csStatus.GetGold(), dwUsedGold ) )
	{
		sprintf( buff,"%d", 0 );

		CRYLMessageBox*lpMessageBox = new CRYLMessageBox;
		lpMessageBox->Create(CRYLStringTable::m_strString[ 4043 ]);
	}
	else
	{
		sprintf( buff,"%d", dwUsedGold );
	}

	m_pCostLabel->SetCaption( buff );
	delete pItemInstance;

	return m_aryOffering[m_nOfferingSelected];
}

VOID	
CRYLItemCompo2Panel::InitStringGrid( CItemInstance * pItem )
{
	// 선택된 옵션종류를 초기화 시킨다.
	((CRYLItemCompositionDlg*)g_pItemCompo2Panel->m_pGMUIParentDlg)->SetOption( -1 );

	char buff[256]; 
	CRYLGameData* pGame = CRYLGameData::Instance() ; 
	m_nOfferingSelected = -1;
	m_pOfferingStringGrid->ClearItem();
	m_pCostLabel->SetCaption( "" );

	if ( !pItem ) 
		return;

	Item::CEquipment *lpItem = Item::CEquipment::DowncastToEquipment( pItem->m_lpItemBase );
	if ( !lpItem )
		return;
	Item::CEquipment *lpItemOrigin = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOrigin->m_lpItemBase );
	if ( !lpItemOrigin )
		return;
	
	m_nOfferingCount = 0;
	m_nOfferingSelected = -1;
	memset( m_aryOffering, 0, sizeof(int)*8 );
	memset( m_aryCanComposition, 0, sizeof( bool ) * 8 );

	switch( pItem->GetItemType() )
	{
		case Item::ItemType::CON_ARMOUR:
		case Item::ItemType::CON_BODY:
		case Item::ItemType::DEX_ARMOUR:
		case Item::ItemType::DEX_BODY:

		// edith 2008.01.14 옵션이식 추가작업
		case Item::ItemType::CON_HELM:
		case Item::ItemType::DEX_HELM:
		case Item::ItemType::CON_HEAD:
		case Item::ItemType::DEX_HEAD:

		case Item::ItemType::CON_GLOVE:
		case Item::ItemType::DEX_GLOVE:
		case Item::ItemType::CON_BOOTS:
		case Item::ItemType::DEX_BOOTS:
		case Item::ItemType::CON_PELVIS:
		case Item::ItemType::DEX_PELVIS:
		case Item::ItemType::CON_PROTECT_A:
		case Item::ItemType::DEX_PROTECT_A:
			m_nOfferingCount = 6;
			m_aryOffering[0] = Item::Attribute::ARMOR;
			m_aryOffering[1] = Item::Attribute::EVADE;
			m_aryOffering[2] = Item::Attribute::MAX_HP;
			m_aryOffering[3] = Item::Attribute::HP_REGEN;
			m_aryOffering[4] = Item::Attribute::MAX_MP;
			m_aryOffering[5] = Item::Attribute::MAGIC_RESIST;
			break;

		case Item::ItemType::DAGGER:
		case Item::ItemType::OPP_TALON:
		case Item::ItemType::SKILL_A_KNIFE:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::ONEHANDED_AXE:
		case Item::ItemType::ONEHANDED_SWORD:
		case Item::ItemType::ONEHANDED_BLUNT:
		case Item::ItemType::OPP_HAMMER:
		case Item::ItemType::OPP_AXE:
		case Item::ItemType::OPP_SLUSHER:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::SKILL_A_ATTACK:
		case Item::ItemType::SKILL_A_GUN:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::COM_BLUNT:
		case Item::ItemType::COM_SWORD:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::TWOHANDED_AXE:
		case Item::ItemType::TWOHANDED_BLUNT:
		case Item::ItemType::TWOHANDED_SWORD:
		case Item::ItemType::BOW:
		case Item::ItemType::CROSSBOW:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::STAFF:
		case Item::ItemType::OPP_SYTHE:
			m_nOfferingCount = 6;
			m_aryOffering[0] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[1] = Item::Attribute::HIT_RATE;
			m_aryOffering[2] = Item::Attribute::MAX_MP;
			m_aryOffering[3] = Item::Attribute::MP_REGEN;
			m_aryOffering[4] = Item::Attribute::CRITICAL;
			m_aryOffering[5] = Item::Attribute::MAGIC_POWER;
			break;

		case Item::ItemType::SHIELD:
		case Item::ItemType::SKILL_A_GUARD:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MAX_HP;;
			m_aryOffering[1] = Item::Attribute::HP_REGEN;
			m_aryOffering[2] = Item::Attribute::MAX_MP ;
			m_aryOffering[3] = Item::Attribute::MP_REGEN;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;
	}
	
	int			nValue;
	for( int i = 0; i < m_nOfferingCount ; i++ )
	{
		nValue = 1;
		switch( Item::Attribute::Type( m_aryOffering[i] ) )
		{
			case Item::Attribute::MIN_DAMAGE:	strcpy( buff, CRYLStringTable::m_strString[ 2289 ] );					break;
			case Item::Attribute::MAX_DAMAGE:	strcpy( buff, CRYLStringTable::m_strString[ 2290 ] );					break;
			case Item::Attribute::MAGIC_POWER:	strcpy( buff, CRYLStringTable::m_strString[ 1939 ] );					break;
			case Item::Attribute::HIT_RATE:		strcpy( buff, CRYLStringTable::m_strString[ 1937 ] );					break;
			case Item::Attribute::CRITICAL:		strcpy( buff, CRYLStringTable::m_strString[ 1941 ] );					break;
			case Item::Attribute::EVADE:		strcpy( buff, CRYLStringTable::m_strString[ 1938 ] );					break;
			case Item::Attribute::ARMOR:		strcpy( buff, CRYLStringTable::m_strString[ 1936 ] );					break;
			case Item::Attribute::MAX_HP:		strcpy( buff, CRYLStringTable::m_strString[ 2291 ] );	nValue = 10;	break;
			case Item::Attribute::MAX_MP:		strcpy( buff, CRYLStringTable::m_strString[ 2293 ] );	nValue = 10;	break;
			case Item::Attribute::HP_REGEN:		strcpy( buff, CRYLStringTable::m_strString[ 2292 ] );					break;
			case Item::Attribute::MP_REGEN:		strcpy( buff, CRYLStringTable::m_strString[ 2294 ] );					break;
			case Item::Attribute::BLOCK:		strcpy( buff, CRYLStringTable::m_strString[ 1940 ] );					break;
			case Item::Attribute::MAGIC_RESIST:	strcpy( buff, CRYLStringTable::m_strString[ 1942 ] );					break;

			default:
				strcpy( buff, "UNDEFINE" );
		}
			
		m_pOfferingStringGrid->AddItem( 0, buff, 70, 0xffffffff );
	
		int	origin;
		int offering;

		origin = lpItemOrigin->GetAttribute( Item::Attribute::Type( m_aryOffering[i] ) );
		sprintf( buff,"%4d", origin * nValue );
		m_pOfferingStringGrid->AddItem( 1, buff, 28, 0xffffffff );

		// 계산을 위해 아이템에서 룬 속성을 삭제한다.
		Item::CEquipment::RuneApplyType	enRuneApplyType	= 
			static_cast<Item::CEquipment::RuneApplyType>(Item::CEquipment::RUNE_INDEX | Item::CEquipment::RUNE_REMOVE);
		unsigned char ucRuneIndexAttribuate = m_aryOffering[i];

		lpItem->SetRuneAttribute(enRuneApplyType, ucRuneIndexAttribuate);
		lpItemOrigin->SetRuneAttribute(enRuneApplyType, ucRuneIndexAttribuate);

		origin = lpItemOrigin->GetAttribute( Item::Attribute::Type( m_aryOffering[i] ) );

		offering = lpItem->GetAttribute( Item::Attribute::Type( m_aryOffering[i] )) ;
		offering = (offering - origin + 1) / 2;
		offering *= nValue ;		
		
		
		// edith 2009.03.15 min 데미지가 max보다 높아도 옵션이식 성공 추가작업된것  (로직 빠짐)
//*
		if(m_aryOffering[i] == Item::Attribute::MIN_DAMAGE)
		{
			int originmax = lpItemOrigin->GetAttribute( Item::Attribute::Type( Item::Attribute::MAX_DAMAGE ) );
			if (origin+offering > originmax)
			{
				offering = 0;
			}
		}
//*/

		if ( offering > 0 )
		{
			sprintf( buff," %+4d",  offering);
			m_pOfferingStringGrid->AddItem( 2, buff, 40, 0xff00ff00 );
			m_aryCanComposition[i] = true;
		}
		else
		{
			if ( offering == 0 )
			{
				sprintf( buff," %+4d",  offering);
			}
			else
			{
				sprintf( buff," %+4d",  offering);
			}
			m_pOfferingStringGrid->AddItem( 2, buff, 40, 0xffff0000 );
		}

		// 삭제했던 룬 속성을 아이템에 추가한다.
		enRuneApplyType			= static_cast<Item::CEquipment::RuneApplyType>(Item::CEquipment::RUNE_INDEX | Item::CEquipment::RUNE_APPLY);
		ucRuneIndexAttribuate	= m_aryOffering[i];

		lpItem->SetRuneAttribute(enRuneApplyType, ucRuneIndexAttribuate);
		lpItemOrigin->SetRuneAttribute(enRuneApplyType, ucRuneIndexAttribuate);
	}	
}

VOID 
CRYLItemCompo2Panel::GMUIPaint( INT xx, INT yy ) 
{
	CGMUICustomPanel::GMUIPaint( xx, yy ) ;
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	LPDIRECT3DDEVICE8   lpD3DDevice = BaseGraphicsLayer::GetDevice() ;

	unsigned int left = m_pGMUIParentDlg->GetLeft() + GetLeft();
	unsigned int top = m_pGMUIParentDlg->GetTop() + GetTop();

	if (pGame->m_lpPickItem)
	{
		if (g_DeviceInput.InRect( left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB))
		{
			if (true == pGame->m_lpPickItem->GetIsEnableAddOption())
			{
				if ( pGame->m_csStatus.m_lpCompositionOrigin &&										
					pGame->m_lpPickItem->GetItemType() ==  pGame->m_csStatus.m_lpCompositionOrigin->GetItemType() )
				{
					pGame->RenderRect(lpD3DDevice, left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB, 0xFF000080, 0x80);
				}
				else
				{
					pGame->RenderRect(lpD3DDevice, left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB, 0xFF800000, 0x80);
				}
			} 
			else
			{
				pGame->RenderRect(lpD3DDevice, left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB, 0xFF800000, 0x80);
			}
		} 
	}

	if ( pGame->m_csStatus.m_lpCompositionOffering )
	{
		pGame->m_csStatus.m_lpCompositionOffering->m_wPutX = left + srcItemL +
			((77 / 2) - (pGame->m_csStatus.m_lpCompositionOffering->m_lpSprite->GetSizeX() / 2));
		pGame->m_csStatus.m_lpCompositionOffering->m_wPutY = top + srcItemT +
			((103 / 2) - (pGame->m_csStatus.m_lpCompositionOffering->m_lpSprite->GetSizeY() / 2));
		pGame->m_csStatus.m_lpCompositionOffering->Render(lpD3DDevice);
	}
}

HRESULT
CRYLItemCompo2Panel::Update( BOOL &bClick, BOOL &bEdge )
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	POINT *ptMousePos = g_DeviceInput.GetMouseLocal();
	CRYLNetworkData* pNetwork  = CRYLNetworkData::Instance() ;

	if (m_dwCheckLimitResult & MB_YES)
	{
		((CRYLItemCompositionDlg*)g_pItemCompo2Panel->m_pGMUIParentDlg)->NextPhase();

		m_dwCheckLimitResult = 0;
		return true;
	} 
	else if (m_dwCheckLimitResult & MB_NO || m_dwCheckLimitResult & MB_EXIT)
	{
		m_dwCheckLimitResult = 0;
		return true;
	}

	unsigned int left = m_pGMUIParentDlg->GetLeft() + GetLeft();
	unsigned int top = m_pGMUIParentDlg->GetTop() + GetTop();

	if (g_DeviceInput.InRect( left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB))
	{
		if (g_DeviceInput.GetIsLeftMouseDown())
		{
			if (NULL != pGame->m_lpPickItem)
			{
				if (true == pGame->m_lpPickItem->GetIsEnableAddOption())
				{
					if (pGame->m_csStatus.m_lpCompositionOrigin &&										
						pGame->m_lpPickItem->GetProtoTypeID() ==  pGame->m_csStatus.m_lpCompositionOrigin->GetProtoTypeID())
					{
						Item::CEquipment *lpItem = Item::CEquipment::DowncastToEquipment( pGame->m_lpPickItem->m_lpItemBase );
						if ( !lpItem )
							return S_OK;

						Item::CEquipment *lpItemOrigin = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOrigin->m_lpItemBase );
						if ( !lpItemOrigin )
							return S_OK;

						// 그레이드가 높을수록 값이 작다! AAA = 0, F= 6
						if ( lpItem->GetItemGrade().m_eItemGrade > lpItemOrigin->GetItemGrade().m_eItemGrade )			
						{
							CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
							lpMessageBox->Create( CRYLStringTable::m_strString[2295]);
							return S_OK;
						}

						Item::ItemPos pItemIndex;
						pItemIndex.m_cIndex = Item::ExtraSpacePos::GRAFT_SACRIFICE_POS;
						pItemIndex.m_cPos = TakeType::TS_EXTRA;

						if (NULL == pGame->m_csStatus.m_lpCompositionOffering)
						{
							// 아이템 이동
							if (pGame->m_lpPickItem->m_lpItemBase->GetPos() == pItemIndex)
							{
								pGame->m_csStatus.m_lpCompositionOffering = pGame->m_lpPickItem;
								pGame->SetPickItem(NULL);
								InitStringGrid( pGame->m_csStatus.m_lpCompositionOffering );
							} 
							else
							{
								pNetwork->SendTakeItem( pGame->m_lpPickItem, &pItemIndex, 1);
								InitStringGrid( pGame->m_lpPickItem );
							}
						} 
						else
						{
							// 스왑하기
							pNetwork->SendSwapItem(pGame->m_lpPickItem, &pItemIndex, pGame->m_csStatus.m_lpCompositionOffering );
							InitStringGrid(pGame->m_lpPickItem);
						}
						
						m_pPrevButton->SetEnable( FALSE );
					}
					else			
					{
						// 종류가 맞지 않는다면...
						CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
						lpMessageBox->Create( CRYLStringTable::m_strString[2296]);
					}
				}
			}
			else
			{
				if (pGame->m_dwKindCursor == CURSOR_NORMAL && pGame->m_csStatus.m_lpCompositionOffering )
				{
					pGame->SetPickItem(pGame->m_csStatus.m_lpCompositionOffering);
					pGame->m_csStatus.m_lpCompositionOffering = NULL;
					InitStringGrid( NULL );
					m_pPrevButton->SetEnable( TRUE );
				}
			}

			return S_OK;
		}
	}

	return S_OK;
}

VOID	
CRYLItemCompo2Panel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pItemCompo2Panel )
		return ;

	switch( Sender->GetInstance() )
	{
		case RYLITEMCOMPO2PANEL::NEXT_BUTTON :
		{
			CRYLItemCompositionDlg* lpItemCompositionDlg = reinterpret_cast<CRYLItemCompositionDlg*>(g_pItemCompo2Panel->m_pGMUIParentDlg);
			lpItemCompositionDlg->NextPhase();
			break;
		}

		case RYLITEMCOMPO2PANEL::PREV_BUTTON :
			((CRYLItemCompositionDlg*)g_pItemCompo2Panel->m_pGMUIParentDlg)->PrevPhase();
			break;

		case RYLITEMCOMPOSITIONDLG::MEMBER_COMPOSITION_STRINGGRID :
			((CRYLItemCompositionDlg*)g_pItemCompo2Panel->m_pGMUIParentDlg)->SetOption( g_pItemCompo2Panel->SelectStringGrid() );
			break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//	Phase3
///////////////////////////////////////////////////////////////////////////////

CRYLItemCompo3Panel::CRYLItemCompo3Panel( CGMUICustomDlg *pParentDlg ) : CGMUICustomPanel( pParentDlg )
{
	m_pGMUIParentDlg = pParentDlg;
	g_pItemCompo3Panel = this;


	m_pPrevButton = NULL;
	m_pNextButton = NULL;
	
	m_pPhaseLabel = NULL;
	m_pDescription1Label = NULL;
	m_pDescription2Label = NULL;
	m_pDescription3Label = NULL;
	m_pDescription4Label = NULL;
	
	m_pOfferingStringGrid = NULL;

	m_nOfferingCount = 0;
	m_nOfferingSelected = -1;
	memset( m_aryOffering, 0, sizeof(int)*8 );
	memset( m_aryCanComposition, 0, sizeof( bool ) * 8 );
	

	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 180, &pImageDIB ) ;


	pImageDIB->ClearbyColor( 0x00000000 ) ;
	pImageDIB->MakeFrameRect( 12,   64-15,  149-12,  177-64, 0xffB99E6B, 0x80000000,  1 ) ;								// 테두리
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	AttachPanelImage( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;

}

CRYLItemCompo3Panel::~CRYLItemCompo3Panel()
{
	FinalCtrl();
	g_pItemCompo3Panel = NULL;
}




VOID
CRYLItemCompo3Panel::InitCtrl()
{
	SetWidth( 256 ) ;
	SetHeight( 222 -15  ) ;

	CGMImageDib*			pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList*			pRYLButtonRect	= CRYLSpriteList::Instance() ;
	
	

	// 이전 버튼
	m_pPrevButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pPrevButton->SetLeft( 99 ) ;
	m_pPrevButton->SetTop( 189 - 15 ) ;
	m_pPrevButton->SetCaption( CRYLStringTable::m_strString[ 2704 ] ) ;
	m_pPrevButton->SetFontColor( 0xffffffff) ;
	m_pPrevButton->SetColor( 0xffffffff) ;
	m_pPrevButton->OnClickEvent = DlgClick ;
	m_pPrevButton->SetInstance( RYLITEMCOMPO3PANEL::PREV_BUTTON ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pPrevButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pPrevButton ) ;
	

	// 다음 버튼
	m_pNextButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pNextButton->SetLeft( 173 ) ;
	m_pNextButton->SetTop( 189 - 15 ) ;
	m_pNextButton->SetCaption( CRYLStringTable::m_strString[ 2705 ] ) ;
	m_pNextButton->SetFontColor( 0xffffffff) ;
	m_pNextButton->SetColor( 0xffffffff) ;
	m_pNextButton->OnClickEvent = DlgClick ;
	m_pNextButton->SetInstance( RYLITEMCOMPO3PANEL::NEXT_BUTTON ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pNextButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pNextButton ) ;
	
	
	// 페이즈 라벨
	m_pPhaseLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pPhaseLabel->SetLeft( 42 ) ;
	m_pPhaseLabel->SetTop( 48 - 15 ) ;
	m_pPhaseLabel->SetFontColor( 0xffffffff ) ;
	m_pPhaseLabel->SetAutoSize( TRUE ) ;
	m_pPhaseLabel->SetCaption( CRYLStringTable::m_strString[ 2700 ] ) ;
	CGMUICustomPanel::AddChild( m_pPhaseLabel ) ;

	// 설명 첫째줄
	m_pDescription1Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription1Label->SetLeft( 163 ) ;
	m_pDescription1Label->SetTop( 64 - 15 ) ;
	m_pDescription1Label->SetFontColor( 0xffffffff ) ;
	m_pDescription1Label->SetAutoSize( TRUE ) ;
	m_pDescription1Label->SetCaption( CRYLStringTable::m_strString[ 2706 ] ) ;
	CGMUICustomPanel::AddChild( m_pDescription1Label ) ;


	// 설명 둘째줄
	m_pDescription2Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription2Label->SetLeft( 163 ) ;
	m_pDescription2Label->SetTop( 84 - 15 ) ;
	m_pDescription2Label->SetFontColor( 0xffffffff ) ;
	m_pDescription2Label->SetAutoSize( TRUE ) ;
	m_pDescription2Label->SetCaption( CRYLStringTable::m_strString[ 2707 ] ) ;
	CGMUICustomPanel::AddChild( m_pDescription2Label ) ;

	// 설명 셋째줄
	m_pDescription3Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription3Label->SetLeft( 163 ) ;
	m_pDescription3Label->SetTop( 104 - 15 ) ;
	m_pDescription3Label->SetFontColor( 0xffffffff ) ;
	m_pDescription3Label->SetAutoSize( TRUE ) ;
	m_pDescription3Label->SetCaption( CRYLStringTable::m_strString[ 2708 ] ) ;
	CGMUICustomPanel::AddChild( m_pDescription3Label ) ;

	// 설명 넷째줄
	m_pDescription4Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription4Label->SetLeft( 163 ) ;
	m_pDescription4Label->SetTop( 124 - 15 ) ;
	m_pDescription4Label->SetFontColor( 0xffffffff ) ;
	m_pDescription4Label->SetAutoSize( TRUE ) ;
	m_pDescription4Label->SetCaption( CRYLStringTable::m_strString[ 2709 ] ) ;
	CGMUICustomPanel::AddChild( m_pDescription4Label ) ;


	// 제물 아이템 리스트
	m_pOfferingStringGrid = new CRYLStringGrid( m_pGMUIParentDlg, FALSE ) ;
	m_pOfferingStringGrid->SetLeft( 21 ) ;
	m_pOfferingStringGrid->SetTop( 71 - 15 ) ;
	m_pOfferingStringGrid->SetWidth( 134 ) ;
	m_pOfferingStringGrid->SetHeight( 146 ) ;
	m_pOfferingStringGrid->SetColCount( 3 ) ;
	m_pOfferingStringGrid->SetItemHeight( 16 ) ;
	m_pOfferingStringGrid->SetFontColor( 0xffffffff ) ;
	m_pOfferingStringGrid->SetInstance( RYLITEMCOMPOSITIONDLG::MEMBER_COMPOSITION_STRINGGRID ) ;
	m_pOfferingStringGrid->OnClickEvent = DlgClick ;
	CGMUICustomPanel::AddChild( m_pOfferingStringGrid ) ;
}

VOID
CRYLItemCompo3Panel::FinalCtrl()
{
	
	GM_DELETE( m_pPrevButton ) ;
	GM_DELETE( m_pNextButton ) ;
	
	GM_DELETE( m_pPhaseLabel );
	GM_DELETE( m_pDescription1Label ) ;
	GM_DELETE( m_pDescription2Label ) ;
	GM_DELETE( m_pDescription3Label ) ;
	GM_DELETE( m_pDescription4Label ) ;
	GM_DELETE( m_pOfferingStringGrid ) ;

}

VOID 
CRYLItemCompo3Panel::GMUIPaint( INT xx, INT yy ) 
{
	CGMUICustomPanel::GMUIPaint( xx, yy ) ;
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	LPDIRECT3DDEVICE8   lpD3DDevice = BaseGraphicsLayer::GetDevice() ;
}

HRESULT
CRYLItemCompo3Panel::Update( BOOL &bClick, BOOL &bEdge )
{
	return S_OK;
}


int		
CRYLItemCompo3Panel::SelectStringGrid()
{
	int index;
	index = m_pOfferingStringGrid->GetItemIndex();
	if ( index == -1 )
	{
		m_pOfferingStringGrid->SelectItemIndex( -1 );
		m_nOfferingSelected = -1; 
	}
	else if ( !m_aryCanComposition[index] )
		m_pOfferingStringGrid->SelectItemIndex( m_nOfferingSelected );
	else
		m_nOfferingSelected = index ;

	if ( m_nOfferingSelected == -1 )
		return -1;

	return m_aryOffering[m_nOfferingSelected];
}


VOID	
CRYLItemCompo3Panel::InitStringGrid()
{
	char buff[256]; 
	CRYLGameData* pGame = CRYLGameData::Instance() ; 
	m_nOfferingSelected = -1;
	m_pOfferingStringGrid->ClearItem();
	

	if ( !pGame->m_csStatus.m_lpCompositionOffering ) 
		return;

	Item::CEquipment *lpItem = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOffering->m_lpItemBase );
	if ( !lpItem )
		return;

	Item::CEquipment *lpItemOrigin = Item::CEquipment::DowncastToEquipment( pGame->m_csStatus.m_lpCompositionOrigin->m_lpItemBase );
	if ( !lpItemOrigin )
		return;



	m_nOfferingCount = 0;
	m_nOfferingSelected = -1;
	memset( m_aryOffering, 0, sizeof(int)*8 );
	memset( m_aryCanComposition, 0, sizeof( bool ) * 8 );

	switch( pGame->m_csStatus.m_lpCompositionOffering->GetItemType() )
	{
		case Item::ItemType::CON_ARMOUR:
		case Item::ItemType::CON_BODY:
		case Item::ItemType::DEX_ARMOUR:
		case Item::ItemType::DEX_BODY:

		// edith 2008.01.14 옵션이식 추가작업
		case Item::ItemType::CON_HELM:
		case Item::ItemType::DEX_HELM:
		case Item::ItemType::CON_HEAD:
		case Item::ItemType::DEX_HEAD:

		case Item::ItemType::CON_GLOVE:
		case Item::ItemType::DEX_GLOVE:
		case Item::ItemType::CON_BOOTS:
		case Item::ItemType::DEX_BOOTS:
		case Item::ItemType::CON_PELVIS:
		case Item::ItemType::DEX_PELVIS:
		case Item::ItemType::CON_PROTECT_A:
		case Item::ItemType::DEX_PROTECT_A:
			m_nOfferingCount = 6;
			m_aryOffering[0] = Item::Attribute::ARMOR;
			m_aryOffering[1] = Item::Attribute::EVADE;
			m_aryOffering[2] = Item::Attribute::MAX_HP;
			m_aryOffering[3] = Item::Attribute::HP_REGEN;
			m_aryOffering[4] = Item::Attribute::MAX_MP;
			m_aryOffering[5] = Item::Attribute::MAGIC_RESIST;
			break;

		case Item::ItemType::DAGGER:
		case Item::ItemType::OPP_TALON:
		case Item::ItemType::SKILL_A_KNIFE:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::ONEHANDED_AXE:
		case Item::ItemType::ONEHANDED_SWORD:
		case Item::ItemType::ONEHANDED_BLUNT:
		case Item::ItemType::OPP_HAMMER:
		case Item::ItemType::OPP_AXE:
		case Item::ItemType::OPP_SLUSHER:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::SKILL_A_ATTACK:
		case Item::ItemType::SKILL_A_GUN:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::COM_BLUNT:
		case Item::ItemType::COM_SWORD:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::TWOHANDED_AXE:
		case Item::ItemType::TWOHANDED_BLUNT:
		case Item::ItemType::TWOHANDED_SWORD:
		case Item::ItemType::BOW:
		case Item::ItemType::CROSSBOW:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MIN_DAMAGE;
			m_aryOffering[1] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[2] = Item::Attribute::HIT_RATE;
			m_aryOffering[3] = Item::Attribute::CRITICAL;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;

		case Item::ItemType::STAFF:
		case Item::ItemType::OPP_SYTHE:
			m_nOfferingCount = 6;
			m_aryOffering[0] = Item::Attribute::MAX_DAMAGE;
			m_aryOffering[1] = Item::Attribute::HIT_RATE;
			m_aryOffering[2] = Item::Attribute::MAX_MP;
			m_aryOffering[3] = Item::Attribute::MP_REGEN;
			m_aryOffering[4] = Item::Attribute::CRITICAL;
			m_aryOffering[5] = Item::Attribute::MAGIC_POWER;
			break;

		case Item::ItemType::SHIELD:
		case Item::ItemType::SKILL_A_GUARD:
			m_nOfferingCount = 5;
			m_aryOffering[0] = Item::Attribute::MAX_HP;;
			m_aryOffering[1] = Item::Attribute::HP_REGEN;
			m_aryOffering[2] = Item::Attribute::MAX_MP ;
			m_aryOffering[3] = Item::Attribute::MP_REGEN;
			m_aryOffering[4] = Item::Attribute::BLOCK;
			break;
	}

	int			nValue;
	for( int i = 0; i < m_nOfferingCount ; i++ )
	{
		nValue = 1;
		switch( Item::Attribute::Type( m_aryOffering[i] ) )
		{
		case  Item::Attribute::MIN_DAMAGE:
			strcpy( buff, CRYLStringTable::m_strString[ 2289 ] );
			break;
		case  Item::Attribute::MAX_DAMAGE:
			strcpy( buff, CRYLStringTable::m_strString[ 2290 ] );
			break;
		case  Item::Attribute::MAGIC_POWER:
			strcpy( buff, CRYLStringTable::m_strString[ 1939 ] );
			break;
		case Item::Attribute::HIT_RATE:
			strcpy( buff, CRYLStringTable::m_strString[1937] );
			break;
		case Item::Attribute::CRITICAL:
			strcpy( buff, CRYLStringTable::m_strString[ 1941 ]  );
			break;
		case Item::Attribute::EVADE:
			strcpy( buff, CRYLStringTable::m_strString[1938] );
			break;
		case Item::Attribute::ARMOR:
			strcpy( buff, CRYLStringTable::m_strString[1936] );
			break;
		case Item::Attribute::MAX_HP:
			strcpy( buff, CRYLStringTable::m_strString[2291] );
			nValue = 10;
			break;
		case Item::Attribute::MAX_MP:
			strcpy( buff, CRYLStringTable::m_strString[2293] );
			nValue = 10;
			break;
		case Item::Attribute::HP_REGEN:
			strcpy( buff, CRYLStringTable::m_strString[2292] );
			break;
		case Item::Attribute::MP_REGEN:
			strcpy( buff, CRYLStringTable::m_strString[2294] );
			break;
		case Item::Attribute::BLOCK:
			strcpy( buff, CRYLStringTable::m_strString[ 1940 ] );
			break;
		case Item::Attribute::MAGIC_RESIST:
			strcpy( buff, CRYLStringTable::m_strString[1942] );
			break;
		default:
			strcpy( buff, "UNDEFINE" );

		}
			
		m_pOfferingStringGrid->AddItem( 0, buff, 60, 0xffffffff );
		int	origin;
		int offering;
		origin = lpItemOrigin->GetAttribute( Item::Attribute::Type( m_aryOffering[i] ) );
		sprintf( buff,"%4d", origin * nValue );
		m_pOfferingStringGrid->AddItem( 1, buff, 24, 0xffffffff );
		offering = lpItem->GetAttribute( Item::Attribute::Type( m_aryOffering[i] )) ;
		offering = (offering - origin) / 2;
		offering *= nValue ;
		if ( offering > 0 )
		{
			sprintf( buff," %+4d",  offering);
			m_pOfferingStringGrid->AddItem( 2, buff, 40, 0xff00ff00 );
			m_aryCanComposition[i] = true;
		}
		else
		{
			if ( offering == 0 )
			{
				sprintf( buff," %+4d",  offering);
			}
			else
			{
				sprintf( buff," %+4d",  offering);
			}
			m_pOfferingStringGrid->AddItem( 2, buff, 40, 0xffff0000 );
		}
	}
}

VOID CRYLItemCompo3Panel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pItemCompo3Panel )
		return ;

	switch( Sender->GetInstance() )
	{
	case RYLITEMCOMPO3PANEL::NEXT_BUTTON :
		((CRYLItemCompositionDlg*)g_pItemCompo3Panel->m_pGMUIParentDlg)->NextPhase();
		break;
	case RYLITEMCOMPO3PANEL::PREV_BUTTON :
		((CRYLItemCompositionDlg*)g_pItemCompo3Panel->m_pGMUIParentDlg)->PrevPhase();
		break;
	case RYLITEMCOMPOSITIONDLG::MEMBER_COMPOSITION_STRINGGRID :
		((CRYLItemCompositionDlg*)g_pItemCompo3Panel->m_pGMUIParentDlg)->SetOption( g_pItemCompo3Panel->SelectStringGrid() );
		break;
	}

	

}



///////////////////////////////////////////////////////////////////////////////
//	Phase4
///////////////////////////////////////////////////////////////////////////////


CRYLItemCompo4Panel::CRYLItemCompo4Panel( CGMUICustomDlg *pParentDlg ) : CGMUICustomPanel( pParentDlg )
{
	m_pGMUIParentDlg = pParentDlg;
	g_pItemCompo4Panel = this;


	m_pOKButton = NULL;
	m_pCancelButton = NULL;
//	m_pPrevButton = NULL;
//	m_pNextButton = NULL;
	
	m_pPhaseLabel = NULL;
	m_pDescription1Label = NULL;
	m_pDescription2Label = NULL;
	m_pDescription3Label = NULL;
	m_pDescription4Label = NULL;
	m_pDescription5Label = NULL;

	m_pItemSlotPicture = NULL;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;
	CRYLSpriteList::Instance()->CreateGMImage( 256, 222 -15, &pImageDIB ) ;

	int i;
	CGMImageDib*	pTempImage;
	RECT rt ;
	pTempImage = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_GUILD_256x256 );
	for ( i = 0 ; i < 40 ; i ++ )
	{
		SetRect( &rt, 0, 38, 184, 43 ) ;
		pImageDIB->DrawRect( &rt, 0, i * 5, pTempImage ) ;
		SetRect( &rt, 72, 53, 144, 58 ) ;
		pImageDIB->DrawRect( &rt, 184, i * 5, pTempImage ) ;
	}
	SetRect( &rt, 0, 43, 184, 53 ) ;
	pImageDIB->DrawRect( &rt, 0, (i * 5) -3, pTempImage ) ;
	SetRect( &rt, 144, 53, 216, 63 ) ;
	pImageDIB->DrawRect( &rt, 184, (i * 5) -3, pTempImage  ) ;

//	pImageDIB->ClearbyColor( 0x80000000 ) ;
	pImageDIB->MakeFrameRect( 2,    20, 252,   1, 0xffB99E6B, 0x80000000,  1 ) ;



/*
	pImageDIB->MakeFrameRect( 1,   0,  1,  206, 0xffB99E6B, 0x80000000,  1 ) ;								// 테두리
	pImageDIB->MakeFrameRect( 254,   0,  1,  206, 0xffB99E6B, 0x80000000,  1 ) ;							// 테두리
	pImageDIB->MakeFrameRect( 1,   206,  254,  1, 0xffB99E6B, 0x80000000,  1 ) ;							// 테두리
*/
	
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	
	AttachPanelImage( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;

}

CRYLItemCompo4Panel::~CRYLItemCompo4Panel()
{
	FinalCtrl();
	g_pItemCompo4Panel = NULL;
}

VOID
CRYLItemCompo4Panel::InitCtrl()
{
	SetWidth( 256 ) ;
	SetHeight( 222 -15  ) ;

	CGMImageDib*			pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList*			pRYLButtonRect	= CRYLSpriteList::Instance() ;
	
/*
	// 확인 버튼
	m_pOKButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pOKButton->SetLeft( 99 ) ;
	m_pOKButton->SetTop( 189 - 15 ) ;
	m_pOKButton->SetCaption( CRYLStringTable::m_strString[ 2164 ] ) ;
	m_pOKButton->SetFontColor( 0xffffffff) ;
	m_pOKButton->SetColor( 0xffffffff) ;
	m_pOKButton->OnClickEvent = DlgClick ;
	m_pOKButton->SetInstance( RYLITEMCOMPO4PANEL::OK_BUTTON ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pOKButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	
	CGMUICustomPanel::AddChild( m_pOKButton ) ;


	// 취소 버튼
	m_pCancelButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pCancelButton->SetLeft( 173 ) ;
	m_pCancelButton->SetTop( 189 - 15 ) ;
	m_pCancelButton->SetCaption( CRYLStringTable::m_strString[ 1897 ] ) ;
	m_pCancelButton->SetFontColor( 0xffffffff) ;
	m_pCancelButton->SetColor( 0xffffffff) ;
	m_pCancelButton->OnClickEvent = DlgClick ;
	m_pCancelButton->SetInstance( RYLITEMCOMPO4PANEL::CANCEL_BUTTON ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pCancelButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 4 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	CGMUICustomPanel::AddChild( m_pCancelButton ) ;	
*/

	// 페이즈 라벨
	m_pPhaseLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pPhaseLabel->SetLeft( 21 ) ;
	m_pPhaseLabel->SetTop( 48 - 15 ) ;
	m_pPhaseLabel->SetFontColor( 0xffffffff ) ;
	m_pPhaseLabel->SetAutoSize( TRUE ) ;
	m_pPhaseLabel->SetCaption( CRYLStringTable::m_strString[2306] ) ;
	CGMUICustomPanel::AddChild( m_pPhaseLabel ) ;

	// 설명 첫째줄
	m_pDescription1Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription1Label->SetLeft( 102 ) ;
	m_pDescription1Label->SetTop( 64  - 15 ) ;
	m_pDescription1Label->SetFontColor( 0xffffffff ) ;
	m_pDescription1Label->SetAutoSize( TRUE ) ;
	m_pDescription1Label->SetCaption( CRYLStringTable::m_strString[2735] ) ;
	CGMUICustomPanel::AddChild( m_pDescription1Label ) ;

	// 설명 둘째줄
	m_pDescription2Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription2Label->SetLeft( 102 ) ;
	m_pDescription2Label->SetTop( 81  - 15 ) ;
	m_pDescription2Label->SetFontColor( 0xffffffff ) ;
	m_pDescription2Label->SetAutoSize( TRUE ) ;
	m_pDescription2Label->SetCaption( CRYLStringTable::m_strString[2736] ) ;
	CGMUICustomPanel::AddChild( m_pDescription2Label ) ;

	// 설명 셋째줄
	m_pDescription3Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription3Label->SetLeft( 102 ) ;
	m_pDescription3Label->SetTop( 98  - 15 ) ;
	m_pDescription3Label->SetFontColor( 0xffffffff ) ;
	m_pDescription3Label->SetAutoSize( TRUE ) ;
	m_pDescription3Label->SetCaption( CRYLStringTable::m_strString[2737] ) ;
	CGMUICustomPanel::AddChild( m_pDescription3Label ) ;

	// 설명 넷째줄
	m_pDescription4Label = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pDescription4Label->SetLeft( 102 ) ;
	m_pDescription4Label->SetTop( 115  - 15 ) ;
	m_pDescription4Label->SetFontColor( 0xffffffff ) ;
	m_pDescription4Label->SetAutoSize( TRUE ) ;
	m_pDescription4Label->SetCaption( CRYLStringTable::m_strString[2738] ) ;
	CGMUICustomPanel::AddChild( m_pDescription4Label ) ;


	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 80, 106, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 8, 74, 87, 179 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BLACKSMITH_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	m_pItemSlotPicture = new CRYLPicture( m_pGMUIParentDlg ) ;
	m_pItemSlotPicture->SetLeft( 12 ) ;
	m_pItemSlotPicture->SetTop( 64 -15 ) ;
	m_pItemSlotPicture->SetWidth( 80 ) ;
	m_pItemSlotPicture->SetHeight( 106 ) ;
	m_pItemSlotPicture->AttachImageTexture( (CGMTexture* )pImage ) ;
	
	GM_DELETE( pImageDIB ) ;
	CGMUICustomPanel::AddChild( m_pItemSlotPicture ) ;
}

VOID
CRYLItemCompo4Panel::FinalCtrl()
{
	
	GM_DELETE( m_pOKButton ) ;
	GM_DELETE( m_pCancelButton ) ;
	//GM_DELETE( m_pPrevButton ) ;
	//GM_DELETE( m_pNextButton ) ;
	
	GM_DELETE( m_pPhaseLabel ) ;
	GM_DELETE( m_pDescription1Label ) ;
	GM_DELETE( m_pDescription2Label ) ;
	GM_DELETE( m_pDescription3Label ) ;
	GM_DELETE( m_pDescription4Label ) ;
	GM_DELETE( m_pDescription5Label ) ;
	GM_DELETE( m_pItemSlotPicture ) ;
}

VOID 
CRYLItemCompo4Panel::GMUIPaint( INT xx, INT yy ) 
{
	CGMUICustomPanel::GMUIPaint( xx, yy ) ;
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	LPDIRECT3DDEVICE8   lpD3DDevice = BaseGraphicsLayer::GetDevice() ;

	unsigned int left;
	unsigned int top;
	left = m_pGMUIParentDlg->GetLeft() + GetLeft();
	top = m_pGMUIParentDlg->GetTop() + GetTop();


	if ( pGame->m_csStatus.m_lpCompositionResult )
	{
		pGame->m_csStatus.m_lpCompositionResult->m_wPutX = left + srcItemL +
			((77 / 2) - (pGame->m_csStatus.m_lpCompositionResult->m_lpSprite->GetSizeX() / 2));
		pGame->m_csStatus.m_lpCompositionResult->m_wPutY = top + srcItemT +
			((103 / 2) - (pGame->m_csStatus.m_lpCompositionResult->m_lpSprite->GetSizeY() / 2));
		pGame->m_csStatus.m_lpCompositionResult->Render(lpD3DDevice);
	}
	else if ( pGame->m_csStatus.m_lpCompositionTempResult )
	{
		pGame->m_csStatus.m_lpCompositionTempResult->m_wPutX = left + srcItemL +
			((77 / 2) - (pGame->m_csStatus.m_lpCompositionTempResult->m_lpSprite->GetSizeX() / 2));
		pGame->m_csStatus.m_lpCompositionTempResult->m_wPutY = top + srcItemT +
			((103 / 2) - (pGame->m_csStatus.m_lpCompositionTempResult->m_lpSprite->GetSizeY() / 2));
		pGame->m_csStatus.m_lpCompositionTempResult->Render(lpD3DDevice);
	}


}

HRESULT
CRYLItemCompo4Panel::Update( BOOL &bClick, BOOL &bEdge )
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	POINT *ptMousePos = g_DeviceInput.GetMouseLocal();
	CRYLNetworkData* pNetwork  = CRYLNetworkData::Instance() ;

	// edith 2008.01.14 아이템합셩 결과값 텍스트 출력부위
	if(pGame->m_csStatus.m_iCompositionResult != -1)
	{
		if(pGame->m_csStatus.m_iCompositionResult == 0)
		{
			// 실패
			m_pDescription1Label->SetCaption( CRYLStringTable::m_strString[4260] ) ;
			m_pDescription2Label->SetCaption( CRYLStringTable::m_strString[4261] ) ;
			m_pDescription3Label->SetCaption( CRYLStringTable::m_strString[4262] ) ;
			m_pDescription4Label->SetCaption( CRYLStringTable::m_strString[4263] ) ;

			// 실패
			pGame->m_lpItemResultEft = new CEffScript;
			pGame->m_lpItemResultEft->SetInterfaceSet(true);
			pGame->m_lpItemResultEft->GetScriptBinData("inter_enhance_failed.esf");

			pGame->m_lpItemResultEft->m_InterfacePos[0] = m_pGMUIParentDlg->GetLeft() + GetLeft() + 130;
			pGame->m_lpItemResultEft->m_InterfacePos[1] = m_pGMUIParentDlg->GetTop() + GetTop() + 90;

			CSceneManager::m_EffectManager.AddInterfaceScript(pGame->m_lpItemResultEft);
		}
		else if(pGame->m_csStatus.m_iCompositionResult == 1)
		{
			m_pDescription1Label->SetCaption( CRYLStringTable::m_strString[4258] ) ;
			m_pDescription2Label->SetCaption( CRYLStringTable::m_strString[4259] ) ;
			m_pDescription3Label->SetCaption( CRYLStringTable::m_strString[4262] ) ;
			m_pDescription4Label->SetCaption( CRYLStringTable::m_strString[4263] ) ;

			//성공
			pGame->m_lpItemResultEft = new CEffScript;
			pGame->m_lpItemResultEft->SetInterfaceSet(true);
			pGame->m_lpItemResultEft->GetScriptBinData("inter_enhance_success.esf");

			pGame->m_lpItemResultEft->m_InterfacePos[0] = m_pGMUIParentDlg->GetLeft() + GetLeft() + 130;
			pGame->m_lpItemResultEft->m_InterfacePos[1] = m_pGMUIParentDlg->GetTop() + GetTop() + 90;

			CSceneManager::m_EffectManager.AddInterfaceScript(pGame->m_lpItemResultEft);
		}

		pGame->m_csStatus.m_iCompositionResult = -1;
	}


	unsigned int left;
	unsigned int top;
	left = m_pGMUIParentDlg->GetLeft() + GetLeft();
	top = m_pGMUIParentDlg->GetTop() + GetTop();
	if (g_DeviceInput.InRect( left + srcItemL, top + srcItemT, left + srcItemR, top + srcItemB))
	{
		if (g_DeviceInput.GetIsLeftMouseDown())
		{
			if ( !pGame->m_lpPickItem)
			{
				if (pGame->m_dwKindCursor == CURSOR_NORMAL && pGame->m_csStatus.m_lpCompositionResult )
				{
					// edith 옵션이식시 결과아이템을 셋팅하는 부분.
					pGame->SetPickItem(pGame->m_csStatus.m_lpCompositionResult);
					pGame->m_csStatus.m_lpCompositionResult = NULL;
					pGame->m_csStatus.m_iCompositionResult = -1;
					((CRYLItemCompositionDlg*)m_pGMUIParentDlg)->SetPhaseState2First();
					((CRYLItemCompositionDlg*)m_pGMUIParentDlg)->SetVisible( FALSE );
				}

			}
			return S_OK;
		}
	}

	return S_OK;
}

VOID CRYLItemCompo4Panel::ResetText()
{
	m_pDescription1Label->SetCaption( CRYLStringTable::m_strString[ 2735 ] ) ;
	m_pDescription2Label->SetCaption( CRYLStringTable::m_strString[ 2736 ] ) ;
	m_pDescription3Label->SetCaption( CRYLStringTable::m_strString[ 2737 ] ) ;
	m_pDescription4Label->SetCaption( CRYLStringTable::m_strString[ 2738 ] ) ;
}


VOID	
CRYLItemCompo4Panel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pItemCompo4Panel )
		return ;

	switch( Sender->GetInstance() )
	{
	case RYLITEMCOMPO4PANEL::CANCEL_BUTTON:
		((CRYLItemCompositionDlg*)g_pItemCompo4Panel->m_pGMUIParentDlg)->Cancel();
		break;
	case RYLITEMCOMPO4PANEL::OK_BUTTON:
		((CRYLItemCompositionDlg*)g_pItemCompo4Panel->m_pGMUIParentDlg)->OK();
		break;
	}
}
