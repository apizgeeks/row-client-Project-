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
#include "RYLChattingDlg.h"

#include "RYLGameScene.h"
#include "RYLSceneManager.h"
#include "RYLMessageBox.h"
#include "RYLMessageBoxManager.h"

#include "RYLTempInventoryDlg.h"
#include "GMMemory.h"

const unsigned int srcItemL = 12 +1;
const unsigned int srcItemR = srcItemL + 77;
const unsigned int srcItemT = 48 + 1;
const unsigned int srcItemB = srcItemT + 102 ;

CRYLTempInventoryDlg*		g_pTempInventoryDlg = NULL;

CRYLTempInventoryDlg::CRYLTempInventoryDlg() 
{
	m_pDlgLabel = NULL;								// 다이얼로그 라벨

	m_pDescriptionLabel1 = NULL ;
	m_pDescriptionLabel2 = NULL ;
	m_pDescriptionLabel3 = NULL ;
	m_pDescriptionLabel4 = NULL ;
	m_pDescriptionLabel5 = NULL ;

	m_pRemainingLabel = NULL ;
	m_pLimitTimeLabel = NULL ;
	m_pLimitTimeValueLabel = NULL ;

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

	CRYLSpriteList::Instance()->CreateGMImage( 256, 192, &pImageDIB ) ;

	int i;
	CGMImageDib*	pTempImage;
	pTempImage = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_GUILD_256x256 );
	for ( i = 0 ; i < 37 ; i ++ )
	{
		SetRect( &rt, 0, 38, 184, 43 ) ;
		pImageDIB->DrawRect( &rt, 0, i * 5, pTempImage ) ;
		SetRect( &rt, 72, 53, 144, 58 ) ;
		pImageDIB->DrawRect( &rt, 184, i * 5, pTempImage ) ;
	}
	SetRect( &rt, 0, 43, 184, 53 ) ;
	pImageDIB->DrawRect( &rt, 0, i * 5 - 3, pTempImage ) ;
	SetRect( &rt, 144, 53, 216, 63 ) ;
	pImageDIB->DrawRect( &rt, 184, i * 5 - 3, pTempImage  ) ;


	pImageDIB->MakeFrameRect( 2,    20, 254,   1, 0xffB99E6B, 0x80000000,  1 ) ;

	SetRect( &rt, 8, 74, 87, 179 ) ;
	pImageDIB->DrawRect( &rt, srcItemL-1 , srcItemT-1 - 15, 
		CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BLACKSMITH_256x256 ) ) ;	// 원본 아이템 슬롯

	pImageDIB->MakeFrameRect( 32,  145,  41,  18, 0xffB99E6B, 0x80000000,  1 ) ;				// 수량
	pImageDIB->MakeFrameRect( 108,   136,  130,  32, 0xffB99E6B, 0x80000000,  1 ) ;				// 제한 시간
	pImageDIB->MakeFrameRect( 180,   142,  47,  20, 0xffB99E6B, 0x80000000,  1 ) ;				// 제한 시간
	

	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	
	AttachWindowTexture( ( CGMTexture*)pImage ) ;

	GM_DELETE( pImageDIB ) ;
	
	SetWidth( 256 ) ;
	SetHeight( 192 ) ;
	SetLeft( 200 ) ;
	SetTop( 0 ) ;


	SetFormType( RYLDLG_TYPE_CLOSEBUTTON ) ;
	
	g_pTempInventoryDlg = this;

	InitCtrl() ;
}

CRYLTempInventoryDlg::~CRYLTempInventoryDlg() 
{
	FinalCtrl() ;
	g_pTempInventoryDlg = NULL;
}

VOID	CRYLTempInventoryDlg::InitCtrl() 
{
	CGMImageDib*			pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_BUTTON_256x256 ) ;
	CRYLSpriteList* pRYLButtonRect	= CRYLSpriteList::Instance() ;

	m_pDlgLabel = new CRYLLabel( this ) ;								// 다이얼로그 라벨
	m_pDlgLabel->SetLeft( 12 ) ;
	m_pDlgLabel->SetTop( 18 ) ;
	m_pDlgLabel->SetFontColor (0xffffffff);
	m_pDlgLabel->SetAutoSize( TRUE ) ;
	m_pDlgLabel->SetCaption( CRYLStringTable::m_strString[ 2753 ] ) ;
	AttachGMUIControl( m_pDlgLabel ) ;

	m_pDescriptionLabel1 = new CRYLLabel( this ) ;						// 설명 첫째줄
	m_pDescriptionLabel1->SetLeft( 98 ) ;
	m_pDescriptionLabel1->SetTop( 48 ) ;
	m_pDescriptionLabel1->SetFontColor (0xffffffff);
	m_pDescriptionLabel1->SetAutoSize( TRUE ) ;
	m_pDescriptionLabel1->SetCaption( CRYLStringTable::m_strString[ 2747 ] ) ;
	AttachGMUIControl( m_pDescriptionLabel1 ) ;

	m_pDescriptionLabel2 = new CRYLLabel( this ) ;						// 설명 둘째줄
	m_pDescriptionLabel2->SetLeft( 98 ) ;
	m_pDescriptionLabel2->SetTop( 65 ) ;
	m_pDescriptionLabel2->SetFontColor (0xffffffff);
	m_pDescriptionLabel2->SetAutoSize( TRUE ) ;
	m_pDescriptionLabel2->SetCaption( CRYLStringTable::m_strString[ 2748 ] ) ;
	AttachGMUIControl( m_pDescriptionLabel2 ) ;

	m_pDescriptionLabel3 = new CRYLLabel( this ) ;						// 설명 셋째줄
	m_pDescriptionLabel3->SetLeft( 98 ) ;
	m_pDescriptionLabel3->SetTop( 82 ) ;
	m_pDescriptionLabel3->SetFontColor (0xffffffff);
	m_pDescriptionLabel3->SetAutoSize( TRUE ) ;
	m_pDescriptionLabel3->SetCaption( CRYLStringTable::m_strString[ 2749 ] ) ;
	AttachGMUIControl( m_pDescriptionLabel3 ) ;

	m_pDescriptionLabel4 = new CRYLLabel( this ) ;						// 설명 넷째줄
	m_pDescriptionLabel4->SetLeft( 98 ) ;
	m_pDescriptionLabel4->SetTop( 108 ) ;
	m_pDescriptionLabel4->SetFontColor (0xffffffff);
	m_pDescriptionLabel4->SetAutoSize( TRUE ) ;
	m_pDescriptionLabel4->SetCaption( CRYLStringTable::m_strString[ 2750 ] ) ;
	AttachGMUIControl( m_pDescriptionLabel4 ) ;

	m_pDescriptionLabel5 = new CRYLLabel( this ) ;						// 설명 다섯째줄
	m_pDescriptionLabel5->SetLeft( 98 ) ;
	m_pDescriptionLabel5->SetTop( 125 ) ;
	m_pDescriptionLabel5->SetFontColor (0xffffffff);
	m_pDescriptionLabel5->SetAutoSize( TRUE ) ;
	m_pDescriptionLabel5->SetCaption( CRYLStringTable::m_strString[ 2751 ] ) ;
	AttachGMUIControl( m_pDescriptionLabel5 ) ;

	m_pRemainingLabel = new CRYLLabel( this ) ;							// 비용
	m_pRemainingLabel->SetLeft( 38 ) ;
	m_pRemainingLabel->SetTop( 163 ) ;
	m_pRemainingLabel->SetFontColor (0xffffffff);
	m_pRemainingLabel->SetAutoSize( TRUE ) ;
	m_pRemainingLabel->SetCaption( "" ) ;
	AttachGMUIControl( m_pRemainingLabel ) ;

	m_pLimitTimeLabel = new CRYLLabel( this ) ;							// 제한 시간
	m_pLimitTimeLabel->SetLeft( 119 ) ;
	m_pLimitTimeLabel->SetTop( 162 ) ;
	m_pLimitTimeLabel->SetFontColor (0xffffffff);
	m_pLimitTimeLabel->SetAutoSize( TRUE ) ;
	m_pLimitTimeLabel->SetCaption( CRYLStringTable::m_strString[ 2752 ] ) ;
	AttachGMUIControl( m_pLimitTimeLabel ) ;

	m_pLimitTimeValueLabel = new CRYLLabel( this ) ;					// 제한 시간 값
	m_pLimitTimeValueLabel->SetLeft( 186 ) ;
	m_pLimitTimeValueLabel->SetTop( 162 ) ;
	m_pLimitTimeValueLabel->SetFontColor (0xffffffff);
	m_pLimitTimeValueLabel->SetAutoSize( TRUE ) ;
	m_pLimitTimeValueLabel->SetCaption( "" ) ;
	AttachGMUIControl( m_pLimitTimeValueLabel ) ;
}

HRESULT		CRYLTempInventoryDlg::Update() 
{
	return S_OK ;
}

VOID	CRYLTempInventoryDlg::FinalCtrl() 
{
	GM_DELETE( m_pDlgLabel );								// 다이얼로그 라벨

	GM_DELETE( m_pDescriptionLabel1) ;						// 설명 첫째줄
	GM_DELETE( m_pDescriptionLabel2) ;						// 설명 둘째줄
	GM_DELETE( m_pDescriptionLabel3) ;						// 설명 셋째줄
	GM_DELETE( m_pDescriptionLabel4) ;						// 설명 넷째줄
	GM_DELETE( m_pDescriptionLabel5) ;						// 설명 다섯째줄

	GM_DELETE( m_pRemainingLabel ) ;
	GM_DELETE( m_pLimitTimeLabel ) ;
	GM_DELETE( m_pLimitTimeValueLabel ) ;
}

VOID	CRYLTempInventoryDlg::Show( CGMUIObject* Sender ) 
{
/*
	CRYLGameScene* pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
	if ( pScene->GetMiniMapDlg()->GetVisible() )
	{
		SetLeft( pScene->GetMiniMapDlg()->GetWidth() ) ;
		SetTop( 0 ) ;
	}
	else
	{
		SetLeft( 0 ) ;
		SetTop( 0 ) ;
	}
*/
}

VOID	CRYLTempInventoryDlg::Hide( CGMUIObject* Sender ) 
{
}

VOID	CRYLTempInventoryDlg::GMUIPaint( INT xx, INT yy ) 
{
	CRYLDialog::GMUIPaint( xx, yy ) ;
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	LPDIRECT3DDEVICE8   lpD3DDevice = BaseGraphicsLayer::GetDevice() ;

	CItemInstance* pItem;
	pItem = pGame->m_csStatus.GetTempInventory();
	if ( pItem )
	{
		pItem->m_wPutX = GetLeft() + srcItemL + ((77 / 2) - (pItem->m_lpSprite->GetSizeX() / 2));
		pItem->m_wPutY = GetTop() + srcItemT + ((103 / 2) - (pItem->m_lpSprite->GetSizeY() / 2));
		pItem->Render(lpD3DDevice);
	}
}

HRESULT		CRYLTempInventoryDlg::FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) 
{
	return S_OK ;
}

HRESULT		CRYLTempInventoryDlg::Update( BOOL &bClick, BOOL &bEdge ) 
{
	if (NULL != GetCloseButton())
	{
		GetCloseButton()->SetVisible( FALSE ) ;
	}

	if (FALSE == GetVisible())
	{
		if ( CRYLGameData::Instance()->m_csStatus.m_lstTempInventoryItem.size() > 0)
		{
			SetVisible( TRUE );
		}
		return S_OK;
	}
	else
	{
		if ( CRYLGameData::Instance()->m_csStatus.m_lstTempInventoryItem.size() == 0 )
		{
			SetVisible( FALSE );
			return S_OK;
		}
	}

	CheckTime();

	CRYLGameData* pGame			= CRYLGameData::Instance() ;
	CRYLNetworkData* pNetwork	= CRYLNetworkData::Instance() ;

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

	POINT *ptMousePos = g_DeviceInput.GetMouseLocal();

	if (g_DeviceInput.InRect( GetLeft() + srcItemL, GetTop() + srcItemT, GetLeft() + srcItemR, GetTop() + srcItemB))
	{
		if (g_DeviceInput.GetIsLeftMouseDown())
		{
			if (NULL == pGame->m_lpPickItem)
			{
				CItemInstance* pItem = pGame->m_csStatus.GetTempInventory();
				if (NULL != pItem)
				{
					Item::ItemPos pItemIndex;
					unsigned char cNum = (pItem->GetIsEnableStack()) ? pItem->GetNowDurability() : 1;
					
					if (pGame->m_csStatus.GetAbleInven(
                        pItem->GetProtoTypeID(), cNum, pItemIndex, pItem->GetIsEnableStack()))
					{
						g_pClientSocket->SetStatusFlag(NS_TAKEITEM);

						pGame->SetPickItem( pItem );
						CRYLNetworkData::Instance()->SendTakeItem( pItem, &pItemIndex, cNum);
					}
					else
					{
						CRYLMessageBox *lpMessageBox = new CRYLMessageBox;
						lpMessageBox->SetWordBreak(TRUE);
						lpMessageBox->Create(CRYLStringTable::m_strString[ 138 ], MB_YES | MB_NO | MB_EXIT);
						lpMessageBox->SetResult(&pGame->m_dwTempInvenResult);
						pGame->m_dwTempInvenResult = 0;
						pGame->m_cTempInvenItemNum = cNum;
					}
				}
			}
		}
	}

	return S_OK;
}

VOID	CRYLTempInventoryDlg::DlgClick( CGMUIObject* Sender, INT X, INT Y )
{
	if ( !g_pTempInventoryDlg )
		return ;
}

VOID	CRYLTempInventoryDlg::CheckTime()
{
	unsigned int min, sec, size;
	int	ulTime;
	char strString[20];

	CRYLGameData*	pGame	= CRYLGameData::Instance() ;
	CItemInstance*	pItem	= pGame->m_csStatus.GetTempInventory();

	unsigned short wTimeUp = pGame->m_csStatus.GetAbilityValue(Skill::Type::AB_TEMPINVEN_TIMEUP);

	ulTime = ( timeGetTime() - pGame->m_csStatus.GetTempInventoryTime() ) / 1000;
	ulTime = ((60 * 3)+wTimeUp) - ulTime ;
	if ( ulTime < 0 )
		ulTime = 0;
	min = ulTime / 60;
	sec = ulTime % 60;
	
	// 시간이 초과 하면 떨어 뜨린다. (바닥에 떨굴 수 없는 아이템도 임시 인벤토리에서는 떨어질 수 있다.)
	if ( ulTime <= 0 )						
	{
		if ( NS_TAKEITEM != g_pClientSocket->GetStatusFlag() )
		{
			g_MessageBoxManager.DestroyList();

			CRYLChattingDlg::Instance()->AddMessage(CRYLStringTable::m_strString[ 2914 ], CRYLNetworkData::Instance()->m_dwMyChrID, 129);
			Item::ItemPos pItemIndex;
			pItemIndex.m_cPos = TakeType::TS_TEMPINVEN;
			pItemIndex.m_cIndex = 0;
			if ( pItem->GetIsEnableStack() )
			{
				if(CRYLNetworkData::SendFieldObjectPullDown( pItem->GetNowDurability(), pItemIndex ))
					pGame->m_bPullDownFlag = TRUE;
				else
					pGame->m_bPullDownFlag = FALSE;
			}
			else
			{
				if(CRYLNetworkData::SendFieldObjectPullDown( 1, pItemIndex ))
					pGame->m_bPullDownFlag = TRUE;
				else
					pGame->m_bPullDownFlag = FALSE;
			}
		}
		g_pClientSocket->SetStatusFlag(NS_TAKEITEM);

		return;
	}

	sprintf( strString, "%2d : %2d", min, sec );
	m_pLimitTimeValueLabel->SetCaption( strString );

	size = pGame->m_csStatus.GetTempInventorySize() ;
	sprintf( strString, "%2d/%2d", 1, size );
	m_pRemainingLabel->SetCaption( strString );
}