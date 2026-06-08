
#include <winsock2.h>

#include <Item/ItemMgr.h>
#include <Item/ItemFactory.h>

#include <Network/ClientSocket/Send/SendItem.h>
#include <Network/Packet/PacketStruct/CharCommunityPacket.h>
#include <Network/Packet/PacketStruct/ServerInfo.h>

#include "..\\GUITooltipItem.h"
#include "..\\ItemInstance.h"

#include "WinInput.h"
#include "GMUITooltipBox.h"
#include "GMUICustomDlg.h"
#include "GMImageDib.h"

#include "SoundMgr.h"

#include "RYLCheckBox.h"
#include "RYLButton.h"
#include "RYLLabel.h"
#include "RYLStringTable.h"
#include "RYLImage.h"
#include "RYLImageManager.h"
#include "RYLImageCabinet.h"
#include "RYLSpriteList.h"
#include "RYLMessageBox.h"
#include "RYLMessageBoxManager.h"

#include "RYLGameData.h"
#include "RYLNetworkData.h"
#include "RYLClientMain.h"
#include "RYLRaceBase.h"

#include "RYLGameScene.h"
#include "RYLSceneManager.h"

#include "RYLCampShopDlg.h"
#include "RYLQuestOpeningDlg.h"
#include "RYLGuildFrameDlg.h"
#include "RYLGuildInfoPanel.h"
#include "RYLMiningAmountProcessDlg.h"
#include "RYLAkhanInventoryPanel.h"

#include "RYLCastle.h"
#include "RYLCastleManager.h"
#include "GMMemory.h"

CRYLAkhanInventoryPanel*	g_pAkhanInventoryPanel = NULL ;

CRYLAkhanInventoryPanel::CRYLAkhanInventoryPanel( CGMUICustomDlg *pParentDlg ) 
:	CRYLInventoryPanel( pParentDlg )
{
	RECT rt ;
	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;

	CRYLSpriteList::Instance()->CreateGMImage( 256, 442, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;
	SetRect( &rt, 0, 0, 256, 256 ) ;
	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_INVEN_AKHAN01_256x256 ) ) ;
	SetRect( &rt, 0, 0, 256, 185 ) ;
	pImageDIB->DrawRect( &rt, 0, 256, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_INVEN_HUMAN02_256x256 ) ) ;
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
    AttachPanelImage( ( CGMTexture*)pImage ) ;
	GM_DELETE( pImageDIB ) ;
	SetWidth( 256 ) ;
	SetHeight( 440 ) ;


	m_pTabButton		    = NULL ;
	m_dwBodyInven			= 0L ;

	m_dwRideFlag			= 0L ;
	
	m_pStreetStallButton	= NULL ;

	m_pGMUIParentDlg = pParentDlg ;
	g_pAkhanInventoryPanel = this ;
}

CRYLAkhanInventoryPanel::~CRYLAkhanInventoryPanel() 
{
	FinalCtrl() ;	
}


VOID	CRYLAkhanInventoryPanel::InitCtrl() 
{
	RECT rt ;
	CGMImageDib* pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_INVEN_HUMAN03_256x256 ) ;
/*
	// 무기 I
	m_pWeaponButton[ 0 ] = new CRYLCheckBox( m_pGMUIParentDlg ) ;
	m_pWeaponButton[ 0 ]->SetLeft( 18 ) ;
	m_pWeaponButton[ 0 ]->SetTop( 119 ) ;
	m_pWeaponButton[ 0 ]->SetInstance( RYLAKHANINVENTORYPANEL::WEAPON_CHECKBOX1 ) ;
	m_pWeaponButton[ 0 ]->OnClickEvent = DlgClick ;
	SetRect( &rt, 27, 0, 54, 21 ) ;
	m_pWeaponButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 0, 0, 27, 21 ) ;
	m_pWeaponButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 0, 0, 27, 21 ) ;
	m_pWeaponButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 27, 0, 54, 21 ) ;
	m_pWeaponButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pWeaponButton[ 0 ]->SetChecked( TRUE ) ;
	m_pWeaponButton[ 0 ]->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1837 ], 1837, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pWeaponButton[ 0 ] ) ;

	// 무기 II
	m_pWeaponButton[ 1 ] = new CRYLCheckBox( m_pGMUIParentDlg ) ;
	m_pWeaponButton[ 1 ]->SetLeft( 45 ) ;
	m_pWeaponButton[ 1 ]->SetTop( 119 ) ;
	m_pWeaponButton[ 1 ]->SetInstance( RYLAKHANINVENTORYPANEL::WEAPON_CHECKBOX2 ) ;
	m_pWeaponButton[ 1 ]->OnClickEvent = DlgClick ;
	SetRect( &rt, 26, 21, 52, 42 ) ;
	m_pWeaponButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 0, 21, 26, 42 ) ;
	m_pWeaponButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 0, 21, 26, 42 ) ;
	m_pWeaponButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 26, 21, 52, 42 ) ;
	m_pWeaponButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pWeaponButton[ 1 ]->SetChecked( FALSE ) ;
	m_pWeaponButton[ 1 ]->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1838 ], 1838, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pWeaponButton[ 1 ] ) ;
*/
	// 인벤 1
	m_pInvenButton[ 0 ] = new CRYLCheckBox( m_pGMUIParentDlg ) ;
	m_pInvenButton[ 0 ]->SetLeft( 193 ) ;
	m_pInvenButton[ 0 ]->SetTop( 251 ) ;
	m_pInvenButton[ 0 ]->SetInstance( RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX1 ) ;
	m_pInvenButton[ 0 ]->OnClickEvent = DlgClick ;
	SetRect( &rt, 31, 63, 62, 102 ) ;
	m_pInvenButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 0, 63, 31, 102 ) ;
	m_pInvenButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 0, 63, 31, 102 ) ;
	m_pInvenButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 31, 63, 62, 102 ) ;
	m_pInvenButton[ 0 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pInvenButton[ 0 ]->SetChecked( TRUE ) ;
	m_pInvenButton[ 0 ]->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1842 ], 1842, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pInvenButton[ 0 ] ) ;

	// 인벤 2
	m_pInvenButton[ 1 ] = new CRYLCheckBox( m_pGMUIParentDlg ) ;
	m_pInvenButton[ 1 ]->SetLeft( 193 ) ;
	m_pInvenButton[ 1 ]->SetTop( 290 ) ;
	m_pInvenButton[ 1 ]->SetInstance( RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX2 ) ;
	m_pInvenButton[ 1 ]->OnClickEvent = DlgClick ;
	SetRect( &rt, 31, 102, 62, 141 ) ;
	m_pInvenButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 0, 102, 31, 141 ) ;
	m_pInvenButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 0, 102, 31, 141 ) ;
	m_pInvenButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 31, 102, 62, 141 ) ;
	m_pInvenButton[ 1 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pInvenButton[ 1 ]->SetChecked( FALSE ) ;
	m_pInvenButton[ 1 ]->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1843 ], 1843, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pInvenButton[ 1 ] ) ;

	// 인벤 3
	m_pInvenButton[ 2 ] = new CRYLCheckBox( m_pGMUIParentDlg ) ;
	m_pInvenButton[ 2 ]->SetLeft( 193 ) ;
	m_pInvenButton[ 2 ]->SetTop( 329 ) ;
	m_pInvenButton[ 2 ]->SetInstance( RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX3 ) ;
	m_pInvenButton[ 2 ]->OnClickEvent = DlgClick ;
	SetRect( &rt, 31, 141, 62, 180 ) ;
	m_pInvenButton[ 2 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 0, 141, 31, 180 ) ;
	m_pInvenButton[ 2 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 0, 141, 31, 180 ) ;
	m_pInvenButton[ 2 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 31, 141, 62, 180 ) ;
	m_pInvenButton[ 2 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pInvenButton[ 2 ]->SetChecked( FALSE ) ;
	m_pInvenButton[ 2 ]->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1844 ], 1844, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pInvenButton[ 2 ] ) ;

	// 인벤 퀘스트
	m_pInvenButton[ 3 ] = new CRYLCheckBox( m_pGMUIParentDlg ) ;
	m_pInvenButton[ 3 ]->SetLeft( 193 ) ;
	m_pInvenButton[ 3 ]->SetTop( 368 ) ;
	m_pInvenButton[ 3 ]->SetInstance( RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX4 ) ;
	m_pInvenButton[ 3 ]->OnClickEvent = DlgClick ;
	SetRect( &rt, 31, 180, 62, 219 ) ;
	m_pInvenButton[ 3 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	SetRect( &rt, 0, 180, 31, 219 ) ;
	m_pInvenButton[ 3 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	SetRect( &rt, 0, 180, 31, 219 ) ;
	m_pInvenButton[ 3 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	SetRect( &rt, 31, 180, 62, 219 ) ;
	m_pInvenButton[ 3 ]->AttachButtonImage( pSprite, &rt, CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pInvenButton[ 3 ]->SetChecked( FALSE ) ;
	m_pInvenButton[ 3 ]->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1845 ], 1845, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pInvenButton[ 3 ] ) ;

	pSprite			= CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList* pRYLButtonRect	= CRYLSpriteList::Instance() ;
	// 노점상 버튼
	m_pStreetStallButton = new CRYLButton( m_pGMUIParentDlg ) ;
	m_pStreetStallButton->SetLeft( 159 ) ;
	m_pStreetStallButton->SetTop( 12 ) ;
	m_pStreetStallButton->SetCaption( CRYLStringTable::m_strString[ 2084 ] ) ;     
	m_pStreetStallButton->SetFontColor( 0xffffffff) ;
	m_pStreetStallButton->SetColor( 0xffffffff) ;
	m_pStreetStallButton->OnClickEvent = DlgClick ;
	m_pStreetStallButton->SetInstance( RYLAKHANINVENTORYPANEL::STREETSTALL_BUTTON ) ;
	m_pStreetStallButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonNormalRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_NORMAL ) ;
	m_pStreetStallButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDownRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_DOWN ) ;
	m_pStreetStallButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonUpRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_UP ) ;
	m_pStreetStallButton->AttachButtonImage( pSprite, pRYLButtonRect->GetDefaultButtonDisableRect( 5 ), CRYLImageCabinet::EDCT_BUTTON_DISIBLE ) ;
	m_pStreetStallButton->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1836 ], 1836, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pStreetStallButton ) ;

	// 현재 가진 돈
	m_pCurrentGoldLabel = new CRYLLabel( m_pGMUIParentDlg ) ;
	m_pCurrentGoldLabel->SetLeft( 90 ) ;
	m_pCurrentGoldLabel->SetTop( 419 ) ;
	m_pCurrentGoldLabel->SetFontColor( 0xffffffff ) ;
	m_pCurrentGoldLabel->SetAutoSize( TRUE ) ;
	SetRect( &rt, 61, 419, 193, 430 ) ;
	m_pCurrentGoldLabel->SetAlignRect( rt ) ;
	m_pCurrentGoldLabel->SetAlign( CENTER ) ;
	m_pCurrentGoldLabel->SetTooltip( &m_pGMUIParentDlg->m_TooltipManager, CRYLStringTable::m_strString[ 1846 ], 1846, TOOLTIP_PANEL ) ;
	CGMUICustomPanel::AddChild( m_pCurrentGoldLabel ) ;

	/*
	// 반지1
	CGMUITooltip*	lpTooltip = NULL ;
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 56 ], 56, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 212, 77, 236, 101 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 반지2
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 56 ], 56, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 186, 77, 210, 101 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 헬멧
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 75 ], 75, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 90, 51, 140, 101 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 목걸이
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 57 ], 57, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 160, 77, 184, 101 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 무기 
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 58 ], 58, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 20, 177, 70, 279 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 속옷/겉옷/갑옷
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 59 ], 59, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 90, 114, 166, 216 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 망토
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 62 ], 62, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 186, 229, 236, 279 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 프로텍트암
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 76 ], 76, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 20, 52, 70, 154 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 어택암
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 77 ], 77, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 186, 114, 236, 216 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;

	// 거들
	lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )lpTooltip )->InitTooltip( CRYLStringTable::m_strUITooltip[ 78 ], 78, TOOLTIP_PANEL ) ;
	( ( CGMUITooltipBox* )lpTooltip )->SetRect( 104, 229, 154, 279 ) ;
	m_pGMUIParentDlg->m_TooltipManager.AddTooltip( lpTooltip ) ;
	*/
}

VOID	CRYLAkhanInventoryPanel::FinalCtrl() 
{
	GM_DELETE( m_pTabButton ) ;
//	GM_DELETE( m_pWeaponButton[ 0 ] ) ;
//	GM_DELETE( m_pWeaponButton[ 1 ] ) ;
	GM_DELETE( m_pInvenButton[ 0 ] ) ;
	GM_DELETE( m_pInvenButton[ 1 ] ) ;
	GM_DELETE( m_pInvenButton[ 2 ] ) ;
	GM_DELETE( m_pInvenButton[ 3 ] ) ;

	GM_DELETE( m_pStreetStallButton ) ;
	GM_DELETE( m_pCurrentGoldLabel ) ;
}


HRESULT		CRYLAkhanInventoryPanel::Update( BOOL &bClick, BOOL &bEdge )
{
	if ( !GetVisible() || g_MessageBoxManager.GetIsExistModal() ) 
	{
		return E_FAIL ;
	}

	POINT *ptMousePos = g_DeviceInput.GetMouseLocal();
	CRYLGameData*	 pGame = CRYLGameData::Instance() ;
	CRYLNetworkData* pNetwork = CRYLNetworkData::Instance() ;
	unsigned long dwMousePosX = ptMousePos->x - ( m_pGMUIParentDlg->GetLeft() + 38 ) ;
	unsigned long dwMousePosY = ptMousePos->y - ( m_pGMUIParentDlg->GetTop() + 290 ) ;

	list<CItemInstance *>::iterator it;

	if (true == ProcessMessageBoxResult())
	{
		return S_OK;
	}

	if ( pGame->m_lpInterface->GetIsTrade() )
	{
		// 장비 수리
		if ( pGame->m_dwKindCursor == CURSOR_REPAIR && g_DeviceInput.GetIsLeftMouseDown() )
		{
			if(CRYLGameData::Instance()->m_bGuildSafeResult)
				return S_OK;

			// 장비창
			for (int i = 0; i < Item::EquipmentPos::MAX_EQUPMENT_POS; ++i)
			{
				if ( pGame->m_csStatus.m_lstEquipment[ i ] )
				{
					if ( pGame->m_csStatus.m_lstEquipment[ i ]->IsOverMouse() && 
						 pGame->m_csStatus.m_lstEquipment[ i ]->GetIsEnableRepair() &&
						 pGame->m_csStatus.m_lstEquipment[ i ]->GetNowDurability() < pGame->m_csStatus.m_lstEquipment[ i ]->GetMaxDurability() )
					{
						unsigned long dwRepairGold = pGame->m_csStatus.m_lstEquipment[i]->m_lpItemBase->GetRepairPrice();

						// 길드 요새 상점의 세율 적용 (요새 상점창이 떠 있을때만 적용됨)
						CRYLGameScene* pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
						CRYLCampShopDlg* pDlg = ( CRYLCampShopDlg * )pScene->GetCampShopDlg();
						unsigned long dwCampShopTaxGold = dwRepairGold * (pDlg->GetTax() / 100.0f);

						// 성에 속한 NPC 라면 수리 세율 적용
						unsigned long dwCastleRepairTaxGold = 0;
						NPCNode* pNpcNode = g_NPCList.GetNPCNode( pGame->m_dwTradingNpcID );
                        if ( pNpcNode && pNpcNode->m_bBelongToCastle )
						{
							CRYLCastle* lpCastle = CRYLCastleManager::Instance()->FindCastleByNameID( pNpcNode->m_dwTownOrNameID );
							if ( lpCastle )
							{
								dwCastleRepairTaxGold = static_cast<unsigned long>( dwRepairGold * ( lpCastle->GetTax(Castle::REPAIR_TAX) / 100.f ) );
							}
						}

						// 최종 가격
						dwRepairGold = dwRepairGold + dwCampShopTaxGold + dwCastleRepairTaxGold ;

						if ( pGame->m_csStatus.m_Info.Gold > dwRepairGold )
						{
							char strString[MAX_PATH];
							if (TRUE == pDlg->GetVisible())
							{
								sprintf( strString, CRYLStringTable::m_strString[ 2999 ], 
									pDlg->GetOwnerName(), CRYLStringTable::MakePrintGold( dwRepairGold ) ) ;
							}
							else
							{
								sprintf( strString, CRYLStringTable::m_strString[ 161 ], CRYLStringTable::MakePrintGold( dwRepairGold ) ) ;
							}

							CRYLMessageBox* lpMessageBox = new CRYLMessageBox ;
							lpMessageBox->Create( strString, MB_YES | MB_NO | MB_EXIT ) ;
							lpMessageBox->SetResult( &m_dwRepairResult ) ;
							m_lpRepairItem = pGame->m_csStatus.m_lstEquipment[ i ] ;
							m_dwRepairResult = 0 ;
						} 
						else
						{
							CRYLMessageBox* lpMessageBox = new CRYLMessageBox ;
							lpMessageBox->Create( CRYLStringTable::m_strString[ 162 ] ) ;
						}

						return S_OK ;
					}
				}
			}

			// 인벤토리
			for ( it = pGame->m_csStatus.m_lstInventory.begin(); it != pGame->m_csStatus.m_lstInventory.end(); ++it )
			{
				if ( ( *it )->m_lpItemBase->GetPos().GetZIndex() != m_dwBackInven )
				{
					continue ;
				}

				if ( ( *it )->GetIsEnableRepair() &&							// 장비이고
					( *it )->IsOverMouse() &&									// 마우스로 선택
					( *it )->GetNowDurability() < (*it)->GetMaxDurability())	// 내구도가 적을 때
				{
					unsigned long dwRepairGold = (*it)->m_lpItemBase->GetRepairPrice();

					// 길드 요새 상점의 세율 적용 (요새 상점창이 떠 있을때만 적용됨)
					CRYLGameScene* pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
					CRYLCampShopDlg* pDlg = ( CRYLCampShopDlg * )pScene->GetCampShopDlg();
					unsigned long dwCampShopTaxGold = dwRepairGold * (pDlg->GetTax() / 100.0f);

					// 성에 속한 NPC 라면 수리 세율 적용
					unsigned long dwCastleRepairTaxGold = 0;
					NPCNode* pNpcNode = g_NPCList.GetNPCNode( pGame->m_dwTradingNpcID );
					if ( pNpcNode && pNpcNode->m_bBelongToCastle )
					{
						CRYLCastle* lpCastle = CRYLCastleManager::Instance()->FindCastleByNameID( pNpcNode->m_dwTownOrNameID );
						if ( lpCastle )
						{
							dwCastleRepairTaxGold = static_cast<unsigned long>( dwRepairGold * ( lpCastle->GetTax(Castle::REPAIR_TAX) / 100.f ) );
						}
					}

					// 최종 가격
					dwRepairGold = dwRepairGold + dwCampShopTaxGold + dwCastleRepairTaxGold ;

					if ( pGame->m_csStatus.m_Info.Gold > dwRepairGold )
					{
						char strString[MAX_PATH];
						if (TRUE == pDlg->GetVisible())
						{
							sprintf( strString, CRYLStringTable::m_strString[ 2999 ], 
								pDlg->GetOwnerName(), CRYLStringTable::MakePrintGold( dwRepairGold ) ) ;
						}
						else
						{
							sprintf( strString, CRYLStringTable::m_strString[ 161 ], CRYLStringTable::MakePrintGold( dwRepairGold ) ) ;
						}

						CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
						lpMessageBox->Create(strString, MB_YES | MB_NO | MB_EXIT);
						lpMessageBox->SetResult(&m_dwRepairResult);
						m_lpRepairItem = (*it);
						m_dwRepairResult = 0;
					} 
					else
					{
						CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
						lpMessageBox->Create(CRYLStringTable::m_strString[162]);
					}

					return S_OK ;
				}
			}
		}
	} 
	else
	{
		if ( g_DeviceInput.GetIsRightMouseDown())
		{
            RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
            if( pSelfCreature && !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
			{
				for (it = pGame->m_csStatus.m_lstInventory.begin(); it != pGame->m_csStatus.m_lstInventory.end(); it++)
				{
					if ((*it)->m_lpItemBase->GetPos().GetZIndex() != m_dwBackInven)
					{
						continue ;
					}

					if ( ( *it )->IsOverMouse() )
					{
						UseItem(*it);
						return S_OK;
					}
				}
			}
		}
	}

	// 장비 및 인벤토리 아이템 선택
	if ( SelectItem() )  
		return S_OK ;

	if (g_DeviceInput.GetIsLeftMouseDown())
	{
		// 손에 있는 아이템 처리
		if ( pGame->m_lpPickItem &&
			pGame->m_lpPickItem->GetItemPos() != TakeType::TS_STALL &&
			pGame->m_lpPickItem->GetItemPos() != TakeType::TS_QSLOT &&
			pGame->m_lpPickItem->GetItemPos() != TakeType::TS_SSLOT)		
		{
			unsigned long dwInvenX = dwMousePosX / 26;
			unsigned long dwInvenY = dwMousePosY / 26;

			__int64 ddItem;

			if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 160, m_pGMUIParentDlg->GetTop() + 77, 
				m_pGMUIParentDlg->GetLeft() + 184, m_pGMUIParentDlg->GetTop() + 101))					
			{
				if (true == SetEquip(Item::EquipmentPos::RINGL))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 212, m_pGMUIParentDlg->GetTop() + 77, 
				m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 101))						
			{
				if (true == SetEquip(Item::EquipmentPos::RINGR))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 90, m_pGMUIParentDlg->GetTop() + 51, 
				m_pGMUIParentDlg->GetLeft() + 140, m_pGMUIParentDlg->GetTop() + 101))			
			{
				if (true == SetEquip(Item::EquipmentPos::HEAD))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 77, 
				m_pGMUIParentDlg->GetLeft() + 210, m_pGMUIParentDlg->GetTop() + 101))	
			{
				if (true == SetEquip(Item::EquipmentPos::NECKLACE))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 20, m_pGMUIParentDlg->GetTop() + 177, 
				m_pGMUIParentDlg->GetLeft() + 70, m_pGMUIParentDlg->GetTop() + 279))			
			{
				if (true == SetEquip(Item::EquipmentPos::WEAPON))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 90, m_pGMUIParentDlg->GetTop() + 114, 
				m_pGMUIParentDlg->GetLeft() + 166, m_pGMUIParentDlg->GetTop() + 216))		
			{
				if (true == SetEquip(Item::EquipmentPos::BODY))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 114, 
				m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 216))		
			{
				if (true == SetEquip(Item::EquipmentPos::SKILL_ARM))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 20, m_pGMUIParentDlg->GetTop() + 52, 
				m_pGMUIParentDlg->GetLeft() + 70, m_pGMUIParentDlg->GetTop() + 154))			
			{
				if (true == SetEquip(Item::EquipmentPos::PROTECT_ARM))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 229, 
				m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 279))		
			{
				if (true == SetEquip(Item::EquipmentPos::AVATA))
				{
					return S_OK;
				}
			} 
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 104, m_pGMUIParentDlg->GetTop() + 229, 
				m_pGMUIParentDlg->GetLeft() + 154, m_pGMUIParentDlg->GetTop() + 279))		
			{
				if (true == SetEquip(Item::EquipmentPos::PELVIS))
				{
					return S_OK;
				}
			} 
			// 인벤
			else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 38, m_pGMUIParentDlg->GetTop() + 290, m_pGMUIParentDlg->GetLeft() + 193, m_pGMUIParentDlg->GetTop() + 445))		
			{
				// 리네임 아이템이 설정중이면 아이템을 선택하지 못한다.
				if(pGame->m_lpRenameWarrantItem)
					return S_OK;

				ddItem = pGame->m_csStatus.IsCheckItem(pGame->m_lpPickItem, TakeType::TS_INVEN, dwInvenX, dwInvenY, m_dwBackInven);

				if (ddItem == ITEM_QUEST_TYPE)
				{
					CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
					lpMessageBox->Create(CRYLStringTable::m_strString[ 3575 ]);
					return S_OK;
				}

				if (ddItem == ITEM_NOT_QUEST_TYPE)
				{
					CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
					lpMessageBox->Create(CRYLStringTable::m_strString[ 3576 ]);
					return S_OK;
				}

				if (ddItem != ITEM_FAILED)
				{
					Item::ItemPos pItemIndex;
					pItemIndex.SetPos(dwInvenX, dwInvenY, m_dwBackInven);
					pItemIndex.m_cPos = TakeType::TS_INVEN;

					if (pGame->m_lpPickItem->m_lpItemBase->GetPos() == pItemIndex)
					{
						CItemInstance *lpExistItem = pGame->m_csStatus.GetInventoryItem(pGame->m_lpPickItem->m_lpItemBase->GetPos());

						if (lpExistItem)
						{
							if (pGame->m_lpPickItem->GetIsEnableStack())
							{
								unsigned long dwDurability = lpExistItem->GetNowDurability() + pGame->m_lpPickItem->GetNowDurability();
								lpExistItem->SetNowDurability(dwDurability);
								pGame->m_csStatus.SetQuickSlot(lpExistItem);
								delete pGame->m_lpPickItem;
								pGame->SetPickItem(NULL);
							}
						} 
						else
						{
							pGame->m_csStatus.AddInventoryItem(pGame->m_lpPickItem);
							pGame->SetPickItem(NULL);
						}

						return S_OK ;
					}

					if (ddItem == ITEM_EMPTY)
					{
						if (pGame->m_lpPickItem->GetIsEnableStack())
						{
							CItemInstance *lpExistItem = pGame->m_csStatus.GetItem(pGame->m_lpPickItem->m_lpItemBase->GetPos());
							if (NULL != lpExistItem && lpExistItem->GetProtoTypeID() == pGame->m_lpPickItem->GetProtoTypeID())
							{
								// 기존의 자리에 아이템이 남아있으면 아이템이 나눠진걸로 생각, 아이템을 스택한다.
								pNetwork->SendSplitItem(pGame->m_lpPickItem, &pItemIndex, pGame->m_lpPickItem->GetNowDurability());
							} 
							else
							{
								pNetwork->SendTakeItem(pGame->m_lpPickItem, &pItemIndex, pGame->m_lpPickItem->GetNowDurability());
							}
						} 
						else
						{
							// 아이템 이동
							pNetwork->SendTakeItem(pGame->m_lpPickItem, &pItemIndex, pGame->m_lpPickItem->GetNowDurability());
						}
					} 
					else
					{
						if (true == ProcessItemOverlapEvent(ddItem, pItemIndex))
						{
							return S_OK ;
						}
					}

					return S_OK ;
				}
			}
		}
	}

	m_pCurrentGoldLabel->SetCaption( CRYLStringTable::MakePrintGold( pGame->m_csStatus.m_Info.Gold) ) ;
	return S_OK ;
}

HRESULT		CRYLAkhanInventoryPanel::Render( LPDIRECT3DDEVICE8 lpD3DDevice )  
{
	return S_OK ;
}

BOOL	CRYLAkhanInventoryPanel::SelectItem()
{
	BOOL bPickItem = FALSE;
	CItemInstance* lpInstance = NULL;

	CRYLGameData* pGame = CRYLGameData::Instance() ;
	CRYLNetworkData* pNetwork = CRYLNetworkData::Instance() ;


	// 장비 아이템 선택
	for (int i = 0; i < Item::EquipmentPos::MAX_EQUPMENT_POS; i++)
	{
		if (pGame->m_csStatus.m_lstEquipment[i])
		{
			lpInstance = pGame->m_csStatus.m_lstEquipment[i];
			if (pGame->m_dwKindCursor == CURSOR_NORMAL && !pGame->m_lpPickItem && g_DeviceInput.GetIsLeftMouseDown() && lpInstance->IsOverMouse())
			{
				//--//	start..
				if (Item::EquipmentPos::RINGL == i || Item::EquipmentPos::RINGR == i ||	Item::EquipmentPos::NECKLACE == i ||
					Item::EquipmentPos::WEAPON_HAND1 == i || Item::EquipmentPos::WEAPON_HAND2 == i || Item::EquipmentPos::ARMOUR == i ||
					Item::EquipmentPos::SHIELD_HAND1 == i || Item::EquipmentPos::SHIELD_HAND2 == i)
				{
					short aryEquipmentAttr[Item::Attribute::MAX_ATTRIBUTE_NUM] = { 0, };
					Item::CEquipment* lpEquip = reinterpret_cast<Item::CEquipment*>(pGame->m_csStatus.m_lstEquipment[ i ]->m_lpItemBase);
					if (lpEquip)
					{
						lpEquip->GetAttribute(aryEquipmentAttr, Item::Attribute::MAX_ATTRIBUTE_NUM);
					}

					if (aryEquipmentAttr[Item::Attribute::SKILL_POINT] > 0)
					{
						unsigned short wDiff = pGame->m_csStatus.GetSkillPoint() - pGame->m_csStatus.m_Skill.wSkillNum;

						// 남아있는 스킬 포인트가 반지로 인해 증가한 수치보다 작을때
						if (wDiff < aryEquipmentAttr[Item::Attribute::SKILL_POINT])
						{
							char strMessage[MAX_PATH];
							unsigned short wDeleteSkillNum = aryEquipmentAttr[Item::Attribute::SKILL_POINT] - wDiff;
							sprintf(strMessage, CRYLStringTable::m_strString[ 122 ], wDeleteSkillNum);
							CRYLMessageBox* lpMessageBox = new CRYLMessageBox;
							lpMessageBox->Create(strMessage);

							return FALSE;
						}
					}
				}

				// 5단계 스킬 각성 아이템
				if (Item::EquipmentPos::PROTECT_ARM == i)
				{
					Item::CEquipment* lpEquip = reinterpret_cast<Item::CEquipment*>(pGame->m_csStatus.m_lstEquipment[ i ]->m_lpItemBase);

					// 각성 스킬이 5단계 0스킬 보다 큰 경우 선택이 되지 않도록 막는다.
					switch(lpEquip->GetPrototypeID())
					{
						case 5351 : if(false == IsSelectableFifthItem(0x9302)) { return false; } break;
						case 5352 : if(false == IsSelectableFifthItem(0x9402)) { return false; } break;
						case 5353 : if(false == IsSelectableFifthItem(0x9502)) { return false; } break;
						case 5354 : if(false == IsSelectableFifthItem(0x9606)) { return false; } break;
						case 5355 : if(false == IsSelectableFifthItem(0x9603)) { return false; } break;
						case 5356 : if(false == IsSelectableFifthItem(0x9706)) { return false; } break;
						case 5357 : if(false == IsSelectableFifthItem(0x9804)) { return false; } break;
						default	 : break;
					}
				}

				pGame->SetPickItem(lpInstance);
				pGame->m_csStatus.m_lstEquipment[i] = NULL;
				pNetwork->SendCharShape();
				pGame->m_csStatus.StatusUpdate(STATUS_UPDATE_BASE, true, true);
				bPickItem = TRUE;

				if (i == Item::EquipmentPos::WEAPON_HAND1 + pGame->m_csStatus.m_cWeaponPos ||
					i == Item::EquipmentPos::SHIELD_HAND1 + pGame->m_csStatus.m_cWeaponPos)
				{
					g_CharacterData.m_bAutoTargetAttack = false;
					g_CharacterData.m_dwAutoTargetID = 0xFFFFFFFF;
				}

				Item::ItemPos pItemIndex;
				pItemIndex.m_cPos = TakeType::TS_TEMP;
				pItemIndex.m_cIndex = Item::ExtraSpacePos::HOLDITEM_POS;
				pNetwork->SendTakeItem(lpInstance, &pItemIndex, lpInstance->GetNowDurability());
			}
		}
	}

	// 리네임 아이템이 설정중이면 아이템을 선택하지 못한다.
	if(pGame->m_lpRenameWarrantItem)
		return FALSE;

	list<CItemInstance *>::iterator it ;
	///// 인벤 아이템 선택
	for (it = pGame->m_csStatus.m_lstInventory.begin(); it != pGame->m_csStatus.m_lstInventory.end();)
	{
		if ( ( *it )->m_lpItemBase->GetPos().GetZIndex() != m_dwBackInven )
		{
			it++ ;
			continue ;
		}

		// edith 2008.01.25 아이템선택 인벤토리에 있는 아이템을 팔기위해서 처리하는 방법
		// 여기가 이이템을 왼클릭 했을때 아이템을 마우스에 붙이는 부분이다.
		// 여기서 특정 버튼이 눌렸을때 아이템을 바로 판매하게 만든다.
		
		// 아이템을 판매할때 다음과 같은 형식으로 진행된다.
		// 1. 일반 우클릭 : 현재선택한 아이템을 1개 판매하게 한다. 팝업창 출력
		// 2. Shift 우클릭 : 현재선택한 아이템을 전부 판매하게 한다. 팝업창 출력
		// 3. Ctrl 우클릭 : 현재선택한 아이템을 1개 판매한다. 팝업창 없이

		// 트레이드 모드일때 처리한다.
		if ( pGame->m_bRYLTradeMode && pGame->m_dwKindCursor == CURSOR_NORMAL && !pGame->m_lpPickItem && 
			!pGame->m_lpTradeItem &&
			g_DeviceInput.GetIsRightMouseDown() && (*it)->IsOverMouse() )
		{
			pGame->m_lpTradeItem = ( *it );
			pGame->m_bRYLAutoTrade = TRUE;
			return TRUE;
		}


		if ( pGame->m_dwKindCursor == CURSOR_NORMAL && !pGame->m_lpPickItem && 
			g_DeviceInput.GetIsLeftMouseDown() && (*it)->IsOverMouse() )
		{
			CItemInstance *lpStallItem = pGame->m_csStatus.GetStreetTradeItem((*it)->m_dwItemUID);
			if (lpStallItem)
			{
				TakeType pTakeType;
				pTakeType.m_srcPos.m_cPos	= TakeType::TS_STALL ;
				pTakeType.m_srcPos.m_cIndex = 0 ;
				pTakeType.m_dstPos			= lpStallItem->m_lpItemBase->GetPos() ;
				
				if ( lpStallItem->GetIsEnableStack() )
				{
					pTakeType.m_cNum = lpStallItem->GetNowDurability() ;
				} 
				else
				{
					pTakeType.m_cNum = 1 ;
				}
			
				pGame->m_bStallRegFlag = TRUE;
//				g_pClientSocket->SetStatusFlag( NS_STALLREGISTERITEM ) ;
				SendPacket::CharStallRegisterItem( g_GameSession, pNetwork->m_dwMyChrID, pNetwork->m_dwMyChrID, 
					pTakeType, 0, PktStRI::SC_CANCEL ) ;
			}

			if ( ( *it )->GetIsEnableStack() && g_DeviceInput.KeyHold(VK_SHIFT) && ( *it )->GetNowDurability() > 1 )
			{
				CRYLMessageBox *lpMessageBox ;
				lpMessageBox = new CRYLMessageBox ;
				lpMessageBox->Create( CRYLStringTable::m_strString[ 170 ], MB_NUMBER | MB_YES | MB_NO | MB_EXIT ) ;
				lpMessageBox->SetResult( &m_dwSplitResult ) ;
				lpMessageBox->SetValue( &m_dwSplitValue ) ;

				m_lpSplitItem	= ( *it ) ;
				m_dwSplitResult = 0 ;
				m_dwSplitValue	= 0 ;

				char Temp[MAX_PATH]; 
				sprintf( Temp, "%s\\Sound\\Interface\\click.mnd", g_ClientMain.m_strClientPath ) ;
				CSound* pSound; 
				CSoundMgr::_GetInstance()->GetSound( pSound, Temp ); 
				pSound->Play(); 

				++it;
			} 
            else if (!ClientSocket::IsNSFlagChainChecked(CmdCharUseItem))
			{
				pGame->SetPickItem( ( *it ) ) ;
				pGame->m_csStatus.CheckInvenField( CCharStatus::REMOVE_FIELD, pGame->m_lpPickItem ) ;
				it = pGame->m_csStatus.m_lstInventory.erase( it ) ;
				pGame->m_csStatus.SetQuickSlot( pGame->m_lpPickItem ) ;

				bPickItem = TRUE ;
			}
            else
            {
                ++it;
            }
		} 
		else
		{
			++it ;
		}
	}

	if ( bPickItem )
	{
		return TRUE ;
	}

	return FALSE;
}


HRESULT		CRYLAkhanInventoryPanel::FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) 
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;

	for (int i = 0; i < 15; i++)
	{
		if (pGame->m_csStatus.m_lstEquipment[i])
		{
			if(pGame->m_csStatus.m_lstEquipment[i]->IsZeroPos())
				continue;

			switch(i)
			{
				case Item::EquipmentPos::AVATA:
					pGame->m_csStatus.m_lstEquipment[i]->Render(lpD3DDevice);
					if(m_dwRideFlag == 0)
						pGame->RenderRect(lpD3DDevice, m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 229, m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 279, 0xFF282828, 0x80);
					break;

				case Item::EquipmentPos::WEAPON_HAND1:
				case Item::EquipmentPos::SHIELD_HAND1:
					if (pGame->m_csStatus.m_cWeaponPos == 0)
					{
						if (i == Item::EquipmentPos::WEAPON_HAND1 && pGame->m_csStatus.m_lstEquipment[i]->GetTwoHandItem())
						{
							if ((pGame->m_csStatus.m_lstEquipment[i]->GetItemType() == Item::ItemType::OPP_SYTHE) && 
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1])
							{
								pGame->m_csStatus.m_lstEquipment[i]->Render(lpD3DDevice);
							}
							else
							{
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_wPutX =
									m_pGMUIParentDlg->GetLeft() + 20 + ((50 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_lpSprite->GetSizeX()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_wPutY =
									m_pGMUIParentDlg->GetTop() + 177 + ((102 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_lpSprite->GetSizeY()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->Render(lpD3DDevice);

								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_wPutX =
									m_pGMUIParentDlg->GetLeft() + 186 + ((50 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_lpSprite->GetSizeX()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_wPutY =
									m_pGMUIParentDlg->GetTop() + 114 + ((102 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_lpSprite->GetSizeY()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->Render(lpD3DDevice);

								pGame->RenderRect(lpD3DDevice, m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 114, m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 216, 0xFFFF0000, 0x80);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_wPutX =
									m_pGMUIParentDlg->GetLeft() + 20 + ((50 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_lpSprite->GetSizeX()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_wPutY =
									m_pGMUIParentDlg->GetTop() + 177 + ((102 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->m_lpSprite->GetSizeY()) / 2);
							}
						} 
						else
						{
							pGame->m_csStatus.m_lstEquipment[i]->Render(lpD3DDevice);
						}
					}
					break;
/*
				case Item::EquipmentPos::WEAPON_HAND2:
				case Item::EquipmentPos::SHIELD_HAND2:
					if (pGame->m_csStatus.m_cWeaponPos == 1)
					{
						if (i == Item::EquipmentPos::WEAPON_HAND2 && pGame->m_csStatus.m_lstEquipment[i]->GetTwoHandItem())
						{
							if ((pGame->m_csStatus.m_lstEquipment[i]->GetItemType() == Item::ItemType::OPP_SYTHE) && 
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::SHIELD_HAND2])
							{
								pGame->m_csStatus.m_lstEquipment[i]->Render(lpD3DDevice);
							} 
							else
							{
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_wPutX =
									m_pGMUIParentDlg->GetLeft() + 20 + ((50 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_lpSprite->GetSizeX()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_wPutY =
									m_pGMUIParentDlg->GetTop() + 177 + ((102 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_lpSprite->GetSizeY()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->Render(lpD3DDevice);

								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_wPutX =
									m_pGMUIParentDlg->GetLeft() + 186 + ((50 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_lpSprite->GetSizeX()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_wPutY =
									m_pGMUIParentDlg->GetTop() + 114 + ((102 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_lpSprite->GetSizeY()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->Render(lpD3DDevice);

								pGame->RenderRect(lpD3DDevice, m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 114, m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 216, 0xFFFF0000, 0x80);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_wPutX =
									m_pGMUIParentDlg->GetLeft() + 20 + ((50 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_lpSprite->GetSizeX()) / 2);
								pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_wPutY =
									m_pGMUIParentDlg->GetTop() + 177 + ((102 - pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::WEAPON_HAND2]->m_lpSprite->GetSizeY()) / 2);
							}
						} 
						else
						{
							pGame->m_csStatus.m_lstEquipment[i]->Render(lpD3DDevice);
						}
					}
					break;
*/
				default:
					pGame->m_csStatus.m_lstEquipment[i]->Render(lpD3DDevice);
					break;
			}
		}
	}

	list<CItemInstance *>::iterator it;
	for (it = pGame->m_csStatus.m_lstInventory.begin(); it != pGame->m_csStatus.m_lstInventory.end(); it++)
	{
		if (m_dwBackInven == (*it)->m_lpItemBase->GetPos().GetZIndex() && !(*it)->IsZeroPos()) 
			(*it)->Render(lpD3DDevice);
	}

	if (pGame->m_lpPickItem && g_MessageBoxManager.m_lstMessageBox.empty())
	{
		if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 160, m_pGMUIParentDlg->GetTop() + 77, 
			m_pGMUIParentDlg->GetLeft() + 184, m_pGMUIParentDlg->GetTop() + 101))				
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::RINGL, 160, 77, 184, 101);
		}
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 212, m_pGMUIParentDlg->GetTop() + 77, 
			m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 101))			
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::RINGR, 212, 77, 236, 101);
		}
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 90, m_pGMUIParentDlg->GetTop() + 51, 
			m_pGMUIParentDlg->GetLeft() + 140, m_pGMUIParentDlg->GetTop() + 101))			
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::HEAD, 90, 51, 140, 101);
		}
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 77, 
			m_pGMUIParentDlg->GetLeft() + 210, m_pGMUIParentDlg->GetTop() + 101))			
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::NECKLACE, 186, 77, 210, 101);
		} 
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 20, m_pGMUIParentDlg->GetTop() + 177, 
			m_pGMUIParentDlg->GetLeft() + 70, m_pGMUIParentDlg->GetTop() + 279))			
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::WEAPON, 20, 177, 70, 279);
		} 
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 90, m_pGMUIParentDlg->GetTop() + 114, 
			m_pGMUIParentDlg->GetLeft() + 166, m_pGMUIParentDlg->GetTop() + 216))		
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::BODY, 90, 114, 166, 216);
		} 
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 114, 
			m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 216))		
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::SKILL_ARM, 186, 114, 236, 216);
		} 
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 20, m_pGMUIParentDlg->GetTop() + 52, 
			m_pGMUIParentDlg->GetLeft() + 70, m_pGMUIParentDlg->GetTop() + 154))			
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::PROTECT_ARM, 20, 52, 70, 154);
		} 
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 186, m_pGMUIParentDlg->GetTop() + 229, 
			m_pGMUIParentDlg->GetLeft() + 236, m_pGMUIParentDlg->GetTop() + 279))		
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::AVATA, 186, 229, 236, 279);
		} 
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 104, m_pGMUIParentDlg->GetTop() + 229, 
			m_pGMUIParentDlg->GetLeft() + 154, m_pGMUIParentDlg->GetTop() + 279))		
		{
			RenderRectBGColor(lpD3DDevice, Item::EquipmentPos::PELVIS, 104, 229, 154, 279);
		} 
		// 인벤
		else if (g_DeviceInput.InRect(m_pGMUIParentDlg->GetLeft() + 38, m_pGMUIParentDlg->GetTop() + 290, m_pGMUIParentDlg->GetLeft() + 193, m_pGMUIParentDlg->GetTop() + 445))		
		{
			Item::ItemPos pItemIndex;
			POINT *ptMousePos = g_DeviceInput.GetMouseLocal();
			pItemIndex.SetPos((ptMousePos->x - (m_pGMUIParentDlg->GetLeft() + 38)) / 26, (ptMousePos->y - (m_pGMUIParentDlg->GetTop() + 290)) / 26, m_dwBackInven);

			unsigned char res;
			int pos;	
			
			__int64 ddItem = pGame->m_csStatus.IsCheckItem(pGame->m_lpPickItem, TakeType::TS_INVEN, pItemIndex.GetXIndex(), pItemIndex.GetYIndex(), m_dwBackInven);
			if (ddItem != ITEM_FAILED && ddItem != ITEM_QUEST_TYPE && ddItem != ITEM_NOT_QUEST_TYPE)
			{
				CItemInstance* lpExistItem = pGame->m_csStatus.GetInventoryItem(ddItem);
				CItemInstance* lpStallItem = NULL;

				if (lpExistItem)
				{
					lpStallItem = pGame->m_csStatus.GetStreetTradeItem(lpExistItem->m_dwItemUID);
				}

				if (!lpStallItem && pGame->m_csStatus.GetIsInstallSocket(lpExistItem, pGame->m_lpPickItem))
				{
					pGame->RenderRect(lpD3DDevice,
						m_pGMUIParentDlg->GetLeft() + 38 + lpExistItem->m_lpItemBase->GetPos().GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + lpExistItem->m_lpItemBase->GetPos().GetYIndex() * 26,
						m_pGMUIParentDlg->GetLeft() + 38 + lpExistItem->GetItemInfo().m_DetailData.m_cXSize * 26 + lpExistItem->m_lpItemBase->GetPos().GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + lpExistItem->GetItemInfo().m_DetailData.m_cYSize * 26 + lpExistItem->m_lpItemBase->GetPos().GetYIndex() * 26,
						0xFFFFFFFF, 0x80);
				} 
				// 아이템 합성			
				else if ( pGame->m_csStatus.GetIsChemicalitem(lpExistItem, pGame->m_lpPickItem, res ) )
				{
					pGame->RenderRect(lpD3DDevice,
						m_pGMUIParentDlg->GetLeft() + 38 + lpExistItem->m_lpItemBase->GetPos().GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + lpExistItem->m_lpItemBase->GetPos().GetYIndex() * 26,
						m_pGMUIParentDlg->GetLeft() + 38 + lpExistItem->GetItemInfo().m_DetailData.m_cXSize * 26 + lpExistItem->m_lpItemBase->GetPos().GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + lpExistItem->GetItemInfo().m_DetailData.m_cYSize * 26 + lpExistItem->m_lpItemBase->GetPos().GetYIndex() * 26,
						0xFFFFFFFF, 0x80);
				}
				else if( pGame->m_csStatus.GetIsRuneSocket( lpExistItem, pGame->m_lpPickItem, pos ) )
				{
					pGame->RenderRect(lpD3DDevice,
						m_pGMUIParentDlg->GetLeft() + 38 + lpExistItem->m_lpItemBase->GetPos().GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + lpExistItem->m_lpItemBase->GetPos().GetYIndex() * 26,
						m_pGMUIParentDlg->GetLeft() + 38 + lpExistItem->GetItemInfo().m_DetailData.m_cXSize * 26 + lpExistItem->m_lpItemBase->GetPos().GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + lpExistItem->GetItemInfo().m_DetailData.m_cYSize * 26 + lpExistItem->m_lpItemBase->GetPos().GetYIndex() * 26,
						0xFFFFFFFF, 0x80);
				}
				else
				{
					pGame->RenderRect(lpD3DDevice,
						m_pGMUIParentDlg->GetLeft() + 38 + pItemIndex.GetXIndex() * 26, m_pGMUIParentDlg->GetTop() + 290 + pItemIndex.GetYIndex() * 26,
						m_pGMUIParentDlg->GetLeft() + 38 + pGame->m_lpPickItem->GetItemInfo().m_DetailData.m_cXSize * 26 + pItemIndex.GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + pGame->m_lpPickItem->GetItemInfo().m_DetailData.m_cYSize * 26 + pItemIndex.GetYIndex() * 26,
						0xFF0000FF, 0x80);
				}
			} 
			else
			{
				if (pGame->m_lpPickItem->GetItemInfo().m_DetailData.m_cXSize + pItemIndex.GetXIndex() <= 6 &&
					pGame->m_lpPickItem->GetItemInfo().m_DetailData.m_cYSize + pItemIndex.GetYIndex() <= 6)
				{
					pGame->RenderRect(lpD3DDevice,
						m_pGMUIParentDlg->GetLeft() + 38 + pItemIndex.GetXIndex() * 26, m_pGMUIParentDlg->GetTop() + 290 + pItemIndex.GetYIndex() * 26,
						m_pGMUIParentDlg->GetLeft() + 38 + pGame->m_lpPickItem->GetItemInfo().m_DetailData.m_cXSize * 26 + pItemIndex.GetXIndex() * 26,
						m_pGMUIParentDlg->GetTop() + 290 + pGame->m_lpPickItem->GetItemInfo().m_DetailData.m_cYSize * 26 + pItemIndex.GetYIndex() * 26,
						0xFFFF0000, 0x80);
				}
			}
		}
	}
	return S_OK ;
}

VOID	CRYLAkhanInventoryPanel::InventorySelectWeapon( INT iIndex )
{
	return;

	// 아칸은 아직 지원하지 않는다.
	CRYLGameData*	 pGame = CRYLGameData::Instance() ;
	CRYLNetworkData* pNetwork = CRYLNetworkData::Instance() ;

    RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if( !pSelfCreature )
        return;

	switch( iIndex )
	{
		case 1 :
		{
			if (pGame->m_csStatus.m_cWeaponPos != 0 && !g_CharacterData.GetAttacking())
			{
				pGame->m_csStatus.m_cWeaponPos = 0;
                pSelfCreature->m_ulAttackCombo = 0;
				pNetwork->SendCharSwitchHand();
				pNetwork->SendCharShape();
				pGame->m_csStatus.StatusUpdate(STATUS_UPDATE_BASE, true, true);
			}
		}
		break ;

		case 2 :
		{
			if (pGame->m_csStatus.m_cWeaponPos != 1 && !g_CharacterData.GetAttacking())
			{
				pGame->m_csStatus.m_cWeaponPos = 1;
                pSelfCreature->m_ulAttackCombo = 0;
				pNetwork->SendCharSwitchHand();
				pNetwork->SendCharShape();
				pGame->m_csStatus.StatusUpdate(STATUS_UPDATE_BASE, true, true);
			}
		}
		break ;
	}	
}

VOID	CRYLAkhanInventoryPanel::DlgClick( CGMUIObject* Sender, INT X, INT Y ) 
{
	if ( !g_pAkhanInventoryPanel )
		return ;

	CRYLCheckBox* pCheck = NULL ;

	switch( Sender->GetInstance() )
	{
		case RYLAKHANINVENTORYPANEL::WEAPON_CHECKBOX1 :
			g_pAkhanInventoryPanel->UpdateWeaponCheckButton( 0 ) ;
			g_pAkhanInventoryPanel->InventorySelectWeapon( 1 ) ;
			break ;

		case RYLAKHANINVENTORYPANEL::WEAPON_CHECKBOX2 :
			g_pAkhanInventoryPanel->UpdateWeaponCheckButton( 0 ) ;
			g_pAkhanInventoryPanel->InventorySelectWeapon( 1 ) ;
//			g_pAkhanInventoryPanel->UpdateWeaponCheckButton( 1 ) ;
//			g_pAkhanInventoryPanel->InventorySelectWeapon( 2 ) ;
			break ;

		case RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX1 :
			g_pAkhanInventoryPanel->UpdateInvenCheckButton( 0 ) ;
			g_pAkhanInventoryPanel->SetInvenIndex( 0 ) ;
			break ;

		case RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX2 :
			g_pAkhanInventoryPanel->UpdateInvenCheckButton( 1 ) ;
			g_pAkhanInventoryPanel->SetInvenIndex( 1 ) ;
			break ;

		case RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX3 :
			g_pAkhanInventoryPanel->UpdateInvenCheckButton( 2 ) ;
			g_pAkhanInventoryPanel->SetInvenIndex( 2 ) ;
			break ;

		// 퀘스트 탭
		case RYLAKHANINVENTORYPANEL::INVEN_CHECKBOX4 :
			g_pAkhanInventoryPanel->UpdateInvenCheckButton( 3 ) ;
			g_pAkhanInventoryPanel->SetInvenIndex( 3 ) ;
			break ;

		case RYLAKHANINVENTORYPANEL::STREETSTALL_BUTTON  :
		{
			if ( !CRYLGameData::Instance()->m_lpInterface->GetBusy() )
			{
				CRYLGameScene*		 pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
				
				if ( pScene->GetVaultDlg()->GetVisible() || 
					 pScene->GetBlacksmithDlg()->GetVisible() ||
					 pScene->GetExchangeDlg()->GetVisible() || 
					 pScene->GetNewTradeDlg()->GetVisible() ||
					 pScene->GetStreetStallDlg()->GetVisible() )
				{
					CRYLMessageBox *lpMessageBox = new CRYLMessageBox ;
					lpMessageBox->Create( CRYLStringTable::m_strString[ 2085 ], MB_CONFIRM | MB_EXIT ) ;
					return ;
				}

				CRYLNetworkData::SendStallLeave();
				pScene->GUIVisible( FALSE ) ;
				CRYLGameData::Instance()->m_lpInterface->InitStreetStall( CRYLNetworkData::Instance()->m_dwMyChrID, NULL ) ;
			}
		}
		break ;
	}
}

VOID	CRYLAkhanInventoryPanel::UpdateRideFlag( INT iIndex )
{
	m_dwRideFlag = iIndex;
}

VOID	CRYLAkhanInventoryPanel::UpdateWeaponCheckButton( INT iIndex )
{
	CRYLCheckBox* pCheck = NULL ;

	pCheck = g_pAkhanInventoryPanel->GetWeaponCheckButton( 0 ) ;
	pCheck->SetChecked( FALSE ) ;
	pCheck = g_pAkhanInventoryPanel->GetWeaponCheckButton( 1 ) ;
	pCheck->SetChecked( FALSE ) ;
//	pCheck = g_pAkhanInventoryPanel->GetShieldCheckButton( 0 ) ;
//	pCheck->SetChecked( FALSE ) ;
//	pCheck = g_pAkhanInventoryPanel->GetShieldCheckButton( 1 ) ;
//	pCheck->SetChecked( FALSE ) ;

	pCheck = g_pAkhanInventoryPanel->GetWeaponCheckButton( iIndex ) ;
	pCheck->SetChecked( TRUE ) ;
//	pCheck = g_pAkhanInventoryPanel->GetShieldCheckButton( iIndex ) ;
//	pCheck->SetChecked( TRUE ) ;
}


VOID	CRYLAkhanInventoryPanel::UpdateInvenCheckButton( INT iIndex )
{
	CRYLCheckBox* pCheck = NULL ;

	pCheck = g_pAkhanInventoryPanel->GetInvenCheckButton( 0 ) ;
	pCheck->SetChecked( FALSE ) ;
	pCheck = g_pAkhanInventoryPanel->GetInvenCheckButton( 1 ) ;
	pCheck->SetChecked( FALSE ) ;
	pCheck = g_pAkhanInventoryPanel->GetInvenCheckButton( 2 ) ;
	pCheck->SetChecked( FALSE ) ;
	pCheck = g_pAkhanInventoryPanel->GetInvenCheckButton( 3 ) ;
	pCheck->SetChecked( FALSE ) ;

	pCheck = g_pAkhanInventoryPanel->GetInvenCheckButton( iIndex ) ;
	pCheck->SetChecked( TRUE ) ;
}


ULONG		CRYLAkhanInventoryPanel::GetAllRepairMoney()
{
	ULONG iMoney = 0 ;
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	for ( INT i = 0 ; i < 15 ; i ++ )
	{
		if ( pGame->m_csStatus.m_lstEquipment[ i ] )
		{
			if ( pGame->m_csStatus.m_lstEquipment[ i ]->GetIsEnableRepair() &&
				pGame->m_csStatus.m_lstEquipment[ i ]->GetNowDurability() < pGame->m_csStatus.m_lstEquipment[ i ]->GetMaxDurability() )
			{
				iMoney += pGame->m_csStatus.m_lstEquipment[i]->m_lpItemBase->GetRepairPrice() ;
			}
		}
	}

	return iMoney ;
}

VOID	CRYLAkhanInventoryPanel::UpdateInvenItemPos(VOID)
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;

	unsigned short wPosX[15] = { 90, 0, 0,  90, 20, 104, 186, 212, 160, 186, 0,  20, 0,	186, 0 };
	unsigned short wPosY[15] = { 51, 0, 0, 114, 52, 229,  77,  77,  77, 114, 0, 177, 0,	229, 0 };

	unsigned short wSizeX[15] = { 50, 0, 0, 76, 50, 50, 24, 24, 24, 50, 0, 50, 0, 50, 50 };
	unsigned short wSizeY[15] = { 50, 0, 0, 102, 102, 50, 24, 24, 24, 102, 0, 102, 0, 50, 50 };

	CItemInstance* lpInstance = NULL;
	// 장비 아이템 업데이트 및 선택
	for (int i = 0; i < Item::EquipmentPos::MAX_EQUPMENT_POS; i++)
	{
		if (pGame->m_csStatus.m_lstEquipment[i])
		{
			lpInstance = pGame->m_csStatus.m_lstEquipment[i];
			lpInstance->m_wPutX	= m_pGMUIParentDlg->GetLeft() + wPosX[i] + ((wSizeX[i] - lpInstance->m_lpSprite->GetSizeX()) / 2);
			lpInstance->m_wPutY	= m_pGMUIParentDlg->GetTop() + wPosY[i] + ((wSizeY[i] - lpInstance->m_lpSprite->GetSizeY()) / 2);
		}
	}

	list<CItemInstance *>::iterator it ;
	const Item::ItemInfo* lpItemInfo = NULL;
	///// 인벤 아이템 업데이트 및 선택
	for (it = pGame->m_csStatus.m_lstInventory.begin(); it != pGame->m_csStatus.m_lstInventory.end(); it++)
	{
		lpItemInfo = &(*it)->GetItemInfo();

		( *it )->m_wPutX = m_pGMUIParentDlg->GetLeft() + 38 + 26 * ( *it )->m_lpItemBase->GetPos().GetXIndex() +  
						   ( ( ( lpItemInfo->m_DetailData.m_cXSize * 26 - 1 ) - ( *it )->m_lpSprite->GetSizeX() ) / 2 ) ;
		( *it )->m_wPutY = m_pGMUIParentDlg->GetTop() + 290 + 26 * ( *it )->m_lpItemBase->GetPos().GetYIndex() +  
						   ( ( ( lpItemInfo->m_DetailData.m_cYSize * 26 - 1 ) - ( *it )->m_lpSprite->GetSizeY() ) / 2 ) ;
	}
}


