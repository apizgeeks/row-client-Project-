
#include <winsock2.h>
#include <RenderOption.h>

#include "BaseGraphicsLayer.h"
#include "WinInput.h"
#include "GMImageDib.h"

#include "RYLStringTable.h"
#include "RYLImage.h"
#include "RYLImageManager.h"
#include "RYLImageCabinet.h"
#include "RYLSpriteList.h"

#include "RYLGameData.h"
#include "RYLGameScene.h"
#include "RYLSceneManager.h"
#include "RYLClientMain.h"

#include "RYLGuildInfoPanel.h"
#include "RYLGuildFrameDlg.h"
#include "RYLEnchantDlg.h"
#include "GMMemory.h"


CRYLEnchantDlg*	g_pEnchantDlg = NULL ;

CRYLEnchantDlg::CRYLEnchantDlg( INT nLeft, INT nTop ) : CRYLDialog()
{
	m_pHideButton		= NULL ;				// Hide 버튼

	CGMImageDib*	pImageDIB = NULL ;
	CRYLImage*		pImage	  = NULL ;
	
	CRYLSpriteList::Instance()->CreateGMImage( 200, 80, &pImageDIB ) ;
	pImageDIB->ClearbyColor( 0x00000000 ) ;

//	RECT rt ;
//	SetRect( &rt, 140, 214, 152, 226 ) ;
//	pImageDIB->DrawRect( &rt, 0, 0, CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI_256x256 ) ) ;
		
	pImage = CRYLImageManager::Instance()->CreateRYLImage() ;
	pImage->LoadFromImage( pImageDIB ) ;
	CRYLImageManager::s_pDefaultImageCabinet->AddRYLImage( pImage ) ;
	GM_DELETE( pImageDIB ) ;

	AttachWindowTexture( ( CGMTexture*)pImage ) ;
	
	SetWidth( 200 ) ;
	SetHeight( 80 ) ;
	
	SetLeft( nLeft ) ;
	SetTop( nTop ) ;
	
	SetFormType( RYLDLG_TYPE_NONE ) ;
	
	m_lpTooltip = NULL;
	m_bShowTooltip = FALSE;

	g_pEnchantDlg = this ;	
	InitCtrl() ;
}

CRYLEnchantDlg::~CRYLEnchantDlg ()
{
	FinalCtrl() ;
}

VOID	CRYLEnchantDlg::InitCtrl ()
{
	CGMImageDib* pSprite = CRYLSpriteList::Instance()->FindSprite( ERYLSPRITENAME::RYL_MAINGUI_256x256 ) ;
/*
	RECT rt ;
	// Hide 버튼
	m_pHideButton = new CRYLButton( this ) ;
	m_pHideButton->SetLeft( 1 ) ;
	m_pHideButton->SetTop( 2 ) ;
	m_pHideButton->OnClickEvent = DlgClick ;
	m_pHideButton->SetInstance( RYLENCHANTDLG::HIDE_BUTTON ) ;
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
*/

	// edith 2008.07.07 툴팁추가
	m_lpTooltip = new CGMUITooltipBox ;
	( ( CGMUITooltipBox* )m_lpTooltip )->InitTooltip( "Enchant Info", 0, 0 ) ;

	INT	iTop	= GetTop() ;
	INT iBottom = iTop + 24 ;
	( ( CGMUITooltipBox* )m_lpTooltip )->SetRect( GetLeft(), iTop, GetLeft() + 24, iBottom ) ;





	m_lpChantIconBig = new CTexture;
	m_lpChantIconBig->Load("Chanticon01.dds");

	m_lpChantIcon = new CTexture;
	m_lpChantIcon->Load("Chanticon02.dds");
	
	for (unsigned long i = 0; i < 59; i++)
	{
		m_lpChant[i] = NULL;
		m_lpChantBig[i] = NULL;
	}

	m_lpChant[0] = new CRYLSprite;		
	m_lpChant[0]->Create(0, 0, 0,0,18,18, m_lpChantIcon);
	m_lpChant[1] = new CRYLSprite;		
	m_lpChant[1]->Create(0, 0, 18,0,36,18, m_lpChantIcon);
	m_lpChant[2] = new CRYLSprite;		
	m_lpChant[2]->Create(0, 0, 36,0,54,18, m_lpChantIcon);
	m_lpChant[3] = new CRYLSprite;		
	m_lpChant[3]->Create(0, 0, 54,0,72,18, m_lpChantIcon);
    m_lpChant[4] = new CRYLSprite;		
	m_lpChant[4]->Create(0, 0, 72,0,90,18, m_lpChantIcon);
    m_lpChant[5] = new CRYLSprite;		
	m_lpChant[5]->Create(0, 0, 90,0,108,18, m_lpChantIcon);
	m_lpChant[6] = new CRYLSprite;		
	m_lpChant[6]->Create(0, 0, 108,0,126,18, m_lpChantIcon);	
	m_lpChant[7] = new CRYLSprite;		
	m_lpChant[7]->Create(0, 0, 126,0,144,18, m_lpChantIcon);	
	m_lpChant[8] = new CRYLSprite;		
	m_lpChant[8]->Create(0, 0, 144,0,162,18, m_lpChantIcon);	
	m_lpChant[9] = new CRYLSprite;		
	m_lpChant[9]->Create(0, 0, 162,0,180,18, m_lpChantIcon);
	m_lpChant[10] = new CRYLSprite;		
	m_lpChant[10]->Create(0, 0, 180,0,198,18, m_lpChantIcon);
	m_lpChant[11] = new CRYLSprite;		
	m_lpChant[11]->Create(0, 0, 198,0,216,18, m_lpChantIcon);
	m_lpChant[12] = new CRYLSprite;		
	m_lpChant[12]->Create(0, 0, 216,0,234,18, m_lpChantIcon);
	m_lpChant[13] = new CRYLSprite;		
	m_lpChant[13]->Create(0, 0, 234,0,252,18, m_lpChantIcon);

	m_lpChant[14] = new CRYLSprite;		
	m_lpChant[14]->Create(0, 0, 0,18,18,36, m_lpChantIcon);
	m_lpChant[15] = new CRYLSprite;		
	m_lpChant[15]->Create(0, 0, 18,18,36,36, m_lpChantIcon);
	m_lpChant[16] = new CRYLSprite;		
	m_lpChant[16]->Create(0, 0, 36,18,54,36, m_lpChantIcon);
	m_lpChant[17] = new CRYLSprite;		
	m_lpChant[17]->Create(0, 0, 54,18,72,36, m_lpChantIcon);
    m_lpChant[18] = new CRYLSprite;		
	m_lpChant[18]->Create(0, 0, 72,18,90,36, m_lpChantIcon);
    m_lpChant[19] = new CRYLSprite;		
	m_lpChant[19]->Create(0, 0, 90,18,108,36, m_lpChantIcon);
	m_lpChant[20] = new CRYLSprite;		
	m_lpChant[20]->Create(0, 0, 108,18,126,36, m_lpChantIcon);	
	m_lpChant[21] = new CRYLSprite;		
	m_lpChant[21]->Create(0, 0, 126,18,144,36, m_lpChantIcon);	
	m_lpChant[22] = new CRYLSprite;		
	m_lpChant[22]->Create(0, 0, 144,18,162,36, m_lpChantIcon);	
	m_lpChant[23] = new CRYLSprite;		
	m_lpChant[23]->Create(0, 0, 162,18,180,36, m_lpChantIcon);
	m_lpChant[24] = new CRYLSprite;		
	m_lpChant[24]->Create(0, 0, 180,18,198,36, m_lpChantIcon);
	m_lpChant[25] = new CRYLSprite;		
	m_lpChant[25]->Create(0, 0, 198,18,216,36, m_lpChantIcon);
	m_lpChant[26] = new CRYLSprite;		
	m_lpChant[26]->Create(0, 0, 216,18,234,36, m_lpChantIcon);
	m_lpChant[27] = new CRYLSprite;		
	m_lpChant[27]->Create(0, 0, 234,18,252,36, m_lpChantIcon);

	m_lpChant[28] = new CRYLSprite;		
	m_lpChant[28]->Create(0, 0, 0,36,18,54, m_lpChantIcon);
	m_lpChant[29] = new CRYLSprite;		
	m_lpChant[29]->Create(0, 0, 18,36,36,54, m_lpChantIcon);
	m_lpChant[30] = new CRYLSprite;		
	m_lpChant[30]->Create(0, 0, 36,36,54,54, m_lpChantIcon);
	m_lpChant[31] = new CRYLSprite;		
	m_lpChant[31]->Create(0, 0, 54,36,72,54, m_lpChantIcon);
    m_lpChant[32] = new CRYLSprite;		
	m_lpChant[32]->Create(0, 0, 72,36,90,54, m_lpChantIcon);
    m_lpChant[33] = new CRYLSprite;		
	m_lpChant[33]->Create(0, 0, 90,36,108,54, m_lpChantIcon);


	m_lpChant[34] = new CRYLSprite;		
	m_lpChant[34]->Create(0, 0, 0,54,18,72, m_lpChantIcon);
	m_lpChant[35] = new CRYLSprite;		
	m_lpChant[35]->Create(0, 0, 18,54,36,72, m_lpChantIcon);
	m_lpChant[36] = new CRYLSprite;		
	m_lpChant[36]->Create(0, 0, 36,54,54,72, m_lpChantIcon);
	m_lpChant[37] = new CRYLSprite;		
	m_lpChant[37]->Create(0, 0, 54,54,72,72, m_lpChantIcon);
    m_lpChant[38] = new CRYLSprite;		
	m_lpChant[38]->Create(0, 0, 72,54,90,72, m_lpChantIcon);
    m_lpChant[39] = new CRYLSprite;		
	m_lpChant[39]->Create(0, 0, 90,54,108,72, m_lpChantIcon);
	m_lpChant[40] = new CRYLSprite;		
	m_lpChant[40]->Create(0, 0, 108,54,126,72, m_lpChantIcon);	
	m_lpChant[41] = new CRYLSprite;		
	m_lpChant[41]->Create(0, 0, 126,54,144,72, m_lpChantIcon);	
	m_lpChant[42] = new CRYLSprite;		
	m_lpChant[42]->Create(0, 0, 144,54,162,72, m_lpChantIcon);	
	m_lpChant[43] = new CRYLSprite;		
	m_lpChant[43]->Create(0, 0, 162,54,180,72, m_lpChantIcon);
	m_lpChant[44] = new CRYLSprite;		
	m_lpChant[44]->Create(0, 0, 180,54,198,72, m_lpChantIcon);
	m_lpChant[45] = new CRYLSprite;		
	m_lpChant[45]->Create(0, 0, 198,54,216,72, m_lpChantIcon);
	m_lpChant[46] = new CRYLSprite;		
	m_lpChant[46]->Create(0, 0, 216,54,234,72, m_lpChantIcon);
	m_lpChant[47] = new CRYLSprite;		
	m_lpChant[47]->Create(0, 0, 234,54,252,72, m_lpChantIcon);

	m_lpChant[48] = new CRYLSprite;		
	m_lpChant[48]->Create(0, 0, 0,72,18,90, m_lpChantIcon);
	m_lpChant[49] = new CRYLSprite;		
	m_lpChant[49]->Create(0, 0, 18,72,36,90, m_lpChantIcon);
	m_lpChant[50] = new CRYLSprite;		
	m_lpChant[50]->Create(0, 0, 36,72,54,90, m_lpChantIcon);
	m_lpChant[51] = new CRYLSprite;		
	m_lpChant[51]->Create(0, 0, 54,72,72,90, m_lpChantIcon);
    m_lpChant[52] = new CRYLSprite;		
	m_lpChant[52]->Create(0, 0, 72,72,90,90, m_lpChantIcon);
    m_lpChant[53] = new CRYLSprite;		
	m_lpChant[53]->Create(0, 0, 90,72,108,90, m_lpChantIcon);

	m_lpChant[54] = new CRYLSprite;			// 보호막
	m_lpChant[54]->Create(0, 0, 0,72,18,90, m_lpChantIcon);

	m_lpChant[55] = new CRYLSprite;		// 스틸핸드
	m_lpChant[55]->Create(0, 0, 144,36,162,54, m_lpChantIcon);
	m_lpChant[56] = new CRYLSprite;		// 미하엘의 가호
	m_lpChant[56]->Create(0, 0, 162,36,180,54, m_lpChantIcon);


	// Big 챈트
	m_lpChantBig[0] = new CRYLSprite;		
	m_lpChantBig[0]->Create(0, 0,  0, 0, 24, 24, m_lpChantIconBig);
	m_lpChantBig[1] = new CRYLSprite;		
	m_lpChantBig[1]->Create(0, 0, 24, 0, 48, 24, m_lpChantIconBig);
	m_lpChantBig[2] = new CRYLSprite;		
	m_lpChantBig[2]->Create(0, 0, 48, 0, 72, 24, m_lpChantIconBig);
	m_lpChantBig[3] = new CRYLSprite;		
	m_lpChantBig[3]->Create(0, 0, 72, 0, 96, 24, m_lpChantIconBig);
    m_lpChantBig[4] = new CRYLSprite;		
	m_lpChantBig[4]->Create(0, 0, 96, 0,120, 24, m_lpChantIconBig);
    m_lpChantBig[5] = new CRYLSprite;		
	m_lpChantBig[5]->Create(0, 0,120, 0,144, 24, m_lpChantIconBig);
	m_lpChantBig[6] = new CRYLSprite;		
	m_lpChantBig[6]->Create(0, 0,144, 0,168, 24, m_lpChantIconBig);	
	m_lpChantBig[7] = new CRYLSprite;		
	m_lpChantBig[7]->Create(0, 0,168, 0,192, 24, m_lpChantIconBig);	
	m_lpChantBig[8] = new CRYLSprite;		
	m_lpChantBig[8]->Create(0, 0,192, 0,216, 24, m_lpChantIconBig);	
	m_lpChantBig[9] = new CRYLSprite;		
	m_lpChantBig[9]->Create(0, 0,216, 0,240, 24, m_lpChantIconBig);

	m_lpChantBig[10] = new CRYLSprite;		
	m_lpChantBig[10]->Create(0, 0,  0, 24, 24, 48, m_lpChantIconBig);
	m_lpChantBig[11] = new CRYLSprite;		
	m_lpChantBig[11]->Create(0, 0, 24, 24, 48, 48, m_lpChantIconBig);
	m_lpChantBig[12] = new CRYLSprite;		
	m_lpChantBig[12]->Create(0, 0, 48, 24, 72, 48, m_lpChantIconBig);
	m_lpChantBig[13] = new CRYLSprite;		
	m_lpChantBig[13]->Create(0, 0, 72, 24, 96, 48, m_lpChantIconBig);
    m_lpChantBig[14] = new CRYLSprite;		
	m_lpChantBig[14]->Create(0, 0, 96, 24,120, 48, m_lpChantIconBig);
    m_lpChantBig[15] = new CRYLSprite;		
	m_lpChantBig[15]->Create(0, 0,120, 24,144, 48, m_lpChantIconBig);
	m_lpChantBig[16] = new CRYLSprite;		
	m_lpChantBig[16]->Create(0, 0,144, 24,168, 48, m_lpChantIconBig);	
	m_lpChantBig[17] = new CRYLSprite;		
	m_lpChantBig[17]->Create(0, 0,168, 24,192, 48, m_lpChantIconBig);	
	m_lpChantBig[18] = new CRYLSprite;		
	m_lpChantBig[18]->Create(0, 0,192, 24,216, 48, m_lpChantIconBig);	
	m_lpChantBig[19] = new CRYLSprite;		
	m_lpChantBig[19]->Create(0, 0,216, 24,240, 48, m_lpChantIconBig);

	m_lpChantBig[20] = new CRYLSprite;		
	m_lpChantBig[20]->Create(0, 0,  0, 48, 24, 72, m_lpChantIconBig);
	m_lpChantBig[21] = new CRYLSprite;		
	m_lpChantBig[21]->Create(0, 0, 24, 48, 48, 72, m_lpChantIconBig);
	m_lpChantBig[22] = new CRYLSprite;		
	m_lpChantBig[22]->Create(0, 0, 48, 48, 72, 72, m_lpChantIconBig);
	m_lpChantBig[23] = new CRYLSprite;		
	m_lpChantBig[23]->Create(0, 0, 72, 48, 96, 72, m_lpChantIconBig);
    m_lpChantBig[24] = new CRYLSprite;		
	m_lpChantBig[24]->Create(0, 0, 96, 48,120, 72, m_lpChantIconBig);
    m_lpChantBig[25] = new CRYLSprite;		
	m_lpChantBig[25]->Create(0, 0,120, 48,144, 72, m_lpChantIconBig);
	m_lpChantBig[26] = new CRYLSprite;		
	m_lpChantBig[26]->Create(0, 0,144, 48,168, 72, m_lpChantIconBig);	
	m_lpChantBig[27] = new CRYLSprite;		
	m_lpChantBig[27]->Create(0, 0,168, 48,192, 72, m_lpChantIconBig);	
	m_lpChantBig[28] = new CRYLSprite;		
	m_lpChantBig[28]->Create(0, 0,192, 48,216, 72, m_lpChantIconBig);	
	m_lpChantBig[29] = new CRYLSprite;		
	m_lpChantBig[29]->Create(0, 0,216, 48,240, 72, m_lpChantIconBig);

	m_lpChantBig[30] = new CRYLSprite;		
	m_lpChantBig[30]->Create(0, 0,  0, 72, 24, 96, m_lpChantIconBig);
	m_lpChantBig[31] = new CRYLSprite;		
	m_lpChantBig[31]->Create(0, 0, 24, 72, 48, 96, m_lpChantIconBig);
	m_lpChantBig[32] = new CRYLSprite;		
	m_lpChantBig[32]->Create(0, 0, 48, 72, 72, 96, m_lpChantIconBig);
	m_lpChantBig[33] = new CRYLSprite;		
	m_lpChantBig[33]->Create(0, 0, 72, 72, 96, 96, m_lpChantIconBig);

	m_lpChantBig[34] = new CRYLSprite;		
	m_lpChantBig[34]->Create(0, 0,  0, 96, 24, 120, m_lpChantIconBig);
	m_lpChantBig[35] = new CRYLSprite;		
	m_lpChantBig[35]->Create(0, 0, 24, 96, 48, 120, m_lpChantIconBig);
	m_lpChantBig[36] = new CRYLSprite;		
	m_lpChantBig[36]->Create(0, 0, 48, 96, 72, 120, m_lpChantIconBig);
	m_lpChantBig[37] = new CRYLSprite;		
	m_lpChantBig[37]->Create(0, 0, 72, 96, 96, 120, m_lpChantIconBig);
    m_lpChantBig[38] = new CRYLSprite;		
	m_lpChantBig[38]->Create(0, 0, 96, 96,120, 120, m_lpChantIconBig);
    m_lpChantBig[39] = new CRYLSprite;		
	m_lpChantBig[39]->Create(0, 0,120, 96,144, 120, m_lpChantIconBig);
	m_lpChantBig[40] = new CRYLSprite;		
	m_lpChantBig[40]->Create(0, 0,144, 96,168, 120, m_lpChantIconBig);	
	m_lpChantBig[41] = new CRYLSprite;		
	m_lpChantBig[41]->Create(0, 0,168, 96,192, 120, m_lpChantIconBig);	
	m_lpChantBig[42] = new CRYLSprite;		
	m_lpChantBig[42]->Create(0, 0,192, 96,216, 120, m_lpChantIconBig);	
	m_lpChantBig[43] = new CRYLSprite;		
	m_lpChantBig[43]->Create(0, 0,216, 96,240, 120, m_lpChantIconBig);

	m_lpChantBig[44] = new CRYLSprite;		
	m_lpChantBig[44]->Create(0, 0,  0, 120, 24, 144, m_lpChantIconBig);
	m_lpChantBig[45] = new CRYLSprite;		
	m_lpChantBig[45]->Create(0, 0, 24, 120, 48, 144, m_lpChantIconBig);
	m_lpChantBig[46] = new CRYLSprite;		
	m_lpChantBig[46]->Create(0, 0, 48, 120, 72, 144, m_lpChantIconBig);
	m_lpChantBig[47] = new CRYLSprite;		
	m_lpChantBig[47]->Create(0, 0, 72, 120, 96, 144, m_lpChantIconBig);

	m_lpChantBig[48] = new CRYLSprite;		
	m_lpChantBig[48]->Create(0, 0,  0, 144, 24, 168, m_lpChantIconBig);
	m_lpChantBig[49] = new CRYLSprite;		
	m_lpChantBig[49]->Create(0, 0, 24, 144, 48, 168, m_lpChantIconBig);
	m_lpChantBig[50] = new CRYLSprite;		
	m_lpChantBig[50]->Create(0, 0, 48, 144, 72, 168, m_lpChantIconBig);
	m_lpChantBig[51] = new CRYLSprite;		
	m_lpChantBig[51]->Create(0, 0, 72, 144, 96, 168, m_lpChantIconBig);
    m_lpChantBig[52] = new CRYLSprite;		
	m_lpChantBig[52]->Create(0, 0, 96, 144,120, 168, m_lpChantIconBig);
    m_lpChantBig[53] = new CRYLSprite;		
	m_lpChantBig[53]->Create(0, 0,120, 144,144, 168, m_lpChantIconBig);
	m_lpChantBig[54] = new CRYLSprite;			// 보호막
	m_lpChantBig[54]->Create(0, 0, 96, 120, 120, 144, m_lpChantIconBig);

	// 스틸핸드 챈트
	m_lpChantBig[55] = new CRYLSprite;		
	m_lpChantBig[55]->Create(0, 0, 120, 72, 144, 96, m_lpChantIconBig);
	// 미하엘의 가호
	m_lpChantBig[56] = new CRYLSprite;		
	m_lpChantBig[56]->Create(0, 0, 144, 72, 168, 96, m_lpChantIconBig);

	// 골드, 실버박스
	m_lpChantBig[57] = new CRYLSprite;		
	m_lpChantBig[57]->Create(0, 0,  0, 168, 24, 192, m_lpChantIconBig);
	m_lpChantBig[58] = new CRYLSprite;		
	m_lpChantBig[58]->Create(0, 0, 24, 168, 48, 192, m_lpChantIconBig);

}

VOID	CRYLEnchantDlg::FinalCtrl ()
{
	GM_DELETE(m_lpTooltip);
	GM_DELETE( m_pHideButton ) ;
	for ( INT i = 0; i < 59; i++)
	{
		if ( m_lpChant[ i ] ) 
		{ 
			GM_DELETE( m_lpChant[ i ] ) ;
		}
		if ( m_lpChantBig[ i ] ) 
		{ 
			GM_DELETE( m_lpChantBig[ i ] ) ;
		}

	}

	GM_DELETE(  m_lpChantIcon) ;
	GM_DELETE(  m_lpChantIconBig) ;
}

VOID	CRYLEnchantDlg::Show( CGMUIObject* Sender )
{
	
}
	
VOID	CRYLEnchantDlg::Hide( CGMUIObject* Sender )
{
	
}

VOID	CRYLEnchantDlg::GMUIPaint( INT xx, INT yy )
{
	LPDIRECT3DDEVICE8   lpD3DDevice = BaseGraphicsLayer::GetDevice() ;
	CRYLDialog::GMUIPaint( xx, yy ) ;

	INT		iLeft = GetLeft() ;
	INT		iTop  = GetTop() ;
	INT		iWidth = GetWidth();
	INT		iHeight = GetHeight();

	CRYLGameData*	pGame = CRYLGameData::Instance() ;
	RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
	if( !pSelfCreature )
		return;

	CharDataNode* pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );
	if( !pSelfData )
		return;

	int iRow = 0;
	int iLine = 0;
/*
	if ( pSelfData->m_bPeaceMode )
	{
		if(m_lpChant[ 48 ])
			m_lpChant[ 48 ]->Render( lpD3DDevice, 0xFF, iLeft + (iRow * 20), iTop + (iLine * 20 ) ) ;
	} 
	else
	{
		if(m_lpChant[ 49 ])
			m_lpChant[ 49 ]->Render( lpD3DDevice, 0xFF, iLeft + (iRow * 20), iTop + (iLine * 20 ) ) ;
	}

	iRow++;
*/
	// 프리미엄박스처리
	if(pGame->m_iPremiumType != 0)
	{
		if(pGame->m_iPremiumType == 1)
		{
			if(m_lpChantBig[ 57 ])
				m_lpChantBig[ 57 ]->Render( lpD3DDevice, 0xFF, iLeft + (iRow * 26), iTop + (iLine * 26 ) ) ;

			iRow++;
		}
		else if(pGame->m_iPremiumType == 2)
		{
			if(m_lpChantBig[ 58 ])
				m_lpChantBig[ 58 ]->Render( lpD3DDevice, 0xFF, iLeft + (iRow * 26), iTop + (iLine * 26 ) ) ;

			iRow++;
		}
	}


	// edith 2008.02.22 인첸트 다이얼로그 표시 로직
	for ( ULONG i = 1; i < Skill::SpellID::MAX_SPELL_TYPE ; i++)
	{
		
		if ( i < Skill::SpellID::TasteWine && pGame->m_csStatus.m_aryEnchantLevel[ i ] )	// 석상전 버프 출력
//		if ( i < Skill::SpellID::PowerStatue && pGame->m_csStatus.m_aryEnchantLevel[ i ] )
		{
			if ( m_lpChantBig[ i-1 ] ) 
			{
				m_lpChantBig[ i-1 ]->Render( lpD3DDevice, 0xFF, iLeft + (iRow * 26), iTop + (iLine * 26 ) ) ;
				
				++iRow;
				if(iRow >= 10)
				{
					iRow = 0;
					iLine++;
				}
			}
		}
	}

	//===================================================
	// 국적 변경이 들어갈 경우 혼란을 가져올 수 있기때문에
	// 앨리트보너스 표기를 없셈.
	/*
	if ( 0 != pGame->m_cEliteBonus )
	{
		if ( 0 < pGame->m_cEliteBonus )
		{
			if ( m_lpChant[ 42 + ( pGame->m_cEliteBonus - 1 ) ] )
			{
				m_lpChant[ 42 + ( pGame->m_cEliteBonus - 1 ) ]->Render( lpD3DDevice, 0xFF, 3 + iLeft, 66 + iTop ) ;
			}
		} 
		else
		{
			if ( m_lpChant[ 32 + ( abs(pGame->m_cEliteBonus) - 1 ) ] )
			{
				m_lpChant[ 32 + ( abs(pGame->m_cEliteBonus) - 1 ) ]->Render( lpD3DDevice, 0xFF, 3 + iLeft, 66 + iTop ) ;
			}
		}
	}
	*/



	//CRYLGameScene*		 pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
	//CRYLGuildFrameDlg*	 pDlg   = ( CRYLGuildFrameDlg* )pScene->GetGuildFrameDlg() ;
	//CRYLGuildInfoPanel*  pPanel = ( CRYLGuildInfoPanel* )pDlg->GetGuildInfoPanel() ;

	//LPGUILDLARGEINFOEX pMyGuildInfo = pPanel->FindGuild( pSelfData->m_ulGuildID );
	//by Hades Kang - 평화 길드는 없어짐.
	//bool bPeaceMode = ( NULL != pMyGuildInfo ) ? pMyGuildInfo->IsPeaceMode() : pSelfData->m_bPeaceMode;

	// by Vincent - 카나번 길드전 시간이 아닐땐 길드 가입자도 평화모드가 될수 있다.
	//bool bPeaceMode = ( NULL != pMyGuildInfo ) ? false : pSelfData->m_bPeaceMode;


	if(m_bShowTooltip && m_lpTooltip)
	{
		m_lpTooltip->Render(lpD3DDevice);
	}
}

HRESULT		CRYLEnchantDlg::Update( BOOL &bClick, BOOL &bEdge ) 
{
	m_bShowTooltip = FALSE;
	if (FALSE == GetVisible())
	{
		return S_OK;
	}

	INT		iLeft = GetLeft() ;
	INT		iTop  = GetTop() ;
	INT		iWidth = GetWidth()+52;
	INT		iHeight = GetHeight()+26;

	char strTooltop[512];
	char strTemp[256];

	if( g_DeviceInput.InRect( iLeft, iTop, iLeft + iWidth, iTop + iHeight ) )
	{
		CRYLGameData*	pGame = CRYLGameData::Instance() ;

		int iRow = 0;
		int iLine = 0;
		int iX, iY;

		// 프리미엄박스처리
		if(pGame->m_iPremiumType != 0)
		{
			if(pGame->m_iPremiumType == 1)
			{
				strcpy(strTooltop, CRYLStringTable::m_strString[ 4710 ]);
				iRow++;
			}
			else if(pGame->m_iPremiumType == 2)
			{
				strcpy(strTooltop, CRYLStringTable::m_strString[ 4711 ]);
				iRow++;
			}
			if(iRow == 1)
			{
				iX = iLeft;
				iY = iTop;
				if(g_DeviceInput.InRect(iX, iY, iX+24, iY+24))
				{
					if(pGame->m_lPremiumTime > 0)
					{
						if(pGame->m_lPremiumTime <= 60)	// Min
							wsprintf(strTemp, "\\%d %s", pGame->m_lPremiumTime, CRYLStringTable::m_strString[ 4702 ]);
						else if(pGame->m_lPremiumTime <= 1440)	 // Hour
							wsprintf(strTemp, "\\%d %s", pGame->m_lPremiumTime/60, CRYLStringTable::m_strString[ 4701 ]);
						else if(pGame->m_lPremiumTime > 1440)	 // Day
							wsprintf(strTemp, "\\%d %s", pGame->m_lPremiumTime/1440, CRYLStringTable::m_strString[ 4700 ]);
						else 
							wsprintf(strTemp, "\\%d %s", pGame->m_lPremiumTime, CRYLStringTable::m_strString[ 4702 ]);

						strcat(strTooltop, strTemp);
					}

					( ( CGMUITooltipBox* )m_lpTooltip )->InitTooltip( strTooltop, 0, 0 ) ;
					m_bShowTooltip = TRUE;
				}
			}
		}

		int add;
		for ( ULONG i = 1; i < Skill::SpellID::MAX_SPELL_TYPE ; i++)
		{			
			if ( i < Skill::SpellID::TasteWine && pGame->m_csStatus.m_aryEnchantLevel[ i ] )	// 석상전 버프 출력
			{
				if ( !m_lpChantBig[ i-1 ] ) 
					continue;

				iX = iLeft + (iRow * 26);
				iY = iTop + (iLine * 26);
				if(g_DeviceInput.InRect(iX, iY, iX+24, iY+24))
				{
					int iLevel = pGame->m_csStatus.m_aryEnchantLevel[ i ];
					int iTime = pGame->m_csStatus.m_aryEnchantTime[ i ]-1;
					if(iTime < 0)
						iTime = 0;

					BOOL bShowTime = TRUE;

					switch(i)
					{
					case Skill::SpellID::CD_8D12:
					case Skill::SpellID::CD_8D14:
					case Skill::SpellID::CD_8D16:
					case Skill::SpellID::CD_99A1:
					case Skill::SpellID::CD_99A2:
					case Skill::SpellID::CD_99A3:
						continue;

					case Skill::SpellID::Slow:			    
					case Skill::SpellID::ManaShell:				
					case Skill::SpellID::Flexibility:		    
					case Skill::SpellID::Hold:
					case Skill::SpellID::Stun:
					case Skill::SpellID::Frozen:		    
					case Skill::SpellID::Poisoned:
					case Skill::SpellID::LowerStrength:	 
					case Skill::SpellID::Invincible:
					case Skill::SpellID::Hide:
					case Skill::SpellID::StoneForm:		   		
					case Skill::SpellID::CounterAttack:			
					case Skill::SpellID::Envenom:		   		
					case Skill::SpellID::BombSet:				
					case Skill::SpellID::Fired:
					case Skill::SpellID::ExpOrb:
					case Skill::SpellID::LuckyOrb:
					case Skill::SpellID::ExpStatue:			// 경험의 석상
					case Skill::SpellID::Concentration:
					case Skill::SpellID::Stealth:		
					case Skill::SpellID::StealHand:		
					case Skill::SpellID::MichaelBless:		
						strcpy(strTooltop, CRYLStringTable::m_strString[ 4600 + i ]);
						break;

					case Skill::SpellID::BattleSong:
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel * 2, iLevel );
						bShowTime = FALSE;
						break;

					case Skill::SpellID::MaintenanceChant:	
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel, static_cast<short>(iLevel * 0.1f), static_cast<short>(iLevel * 0.2f) );
						bShowTime = FALSE;
						break;

					case Skill::SpellID::AccelerationChant:
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel * 2, iLevel, static_cast<short>(iLevel * 0.2f) );
						bShowTime = FALSE;
						break;

					case Skill::SpellID::LifeAura:	     
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], static_cast<short>(iLevel * 0.4f), static_cast<short>(iLevel * 0.4f));
						bShowTime = FALSE;
						break;

					case Skill::SpellID::SpeedBuff:
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel * 2);
						break;

					case Skill::SpellID::Regeneration:	   
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], static_cast<short>(iLevel * 0.4f));
						break;

					case Skill::SpellID::ArmorBroken:		    
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel);
						break;

					case Skill::SpellID::Blaze:			    
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], 50, static_cast<short>(iLevel * 0.5f), static_cast<short>(iLevel * 0.1f), static_cast<short>(iLevel * 0.1f));
						break;

					case Skill::SpellID::Charging:		    
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], 50, static_cast<short>(iLevel * 0.5f), static_cast<short>(iLevel * 0.1f));
						break;

					case Skill::SpellID::Encourage:		  
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel, iLevel, static_cast<short>(iLevel * 0.5f), static_cast<short>(iLevel * 0.25f));
						break;

					case Skill::SpellID::EnchantWeapon:	    
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel * 4, iLevel);
						break;

					case Skill::SpellID::BrightArmor:	
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], static_cast<short>(iLevel * 2.5f));
						break;

					case Skill::SpellID::HardenSkin:		    
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], static_cast<short>(iLevel * 2.5f));
						break;

					case Skill::SpellID::Guard:			
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel);
						break;

					case Skill::SpellID::CurseOfBlind:
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel * 4, iLevel, static_cast<short>(iLevel * 0.25f), iLevel);
						break;

					case Skill::SpellID::DeCoolDown:
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel * 2);
						break;

					case Skill::SpellID::DemagePotion:		    
						add = (int)pGame->m_csStatus.GetBaseMinDamage()*((float)iLevel/100);	
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::ArmorPotion:
						add = (int)pGame->m_csStatus.GetBaseArmor()*((float)iLevel/100);	
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::HitRatePotion:		
						add = (int)pGame->m_csStatus.GetBaseHitRate()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::EvadePotion:								
						add = (int)pGame->m_csStatus.GetBaseEvade()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::MaxHPPotion:
						add = (int)pGame->m_csStatus.GetBaseMaxHP()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::MaxMPPotion:
						add = (int)pGame->m_csStatus.GetBaseMaxMP()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::HPRegenPotion:
						add = (int)pGame->m_csStatus.GetBaseHPRegen()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::MPRegenPotion:
						add = (int)pGame->m_csStatus.GetBaseMPRegen()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::CriticalPotion:
						add = (int)pGame->m_csStatus.GetBaseCritical()*((float)iLevel/100);				
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::BlockPotion:
						add = (int)pGame->m_csStatus.GetBaseBlock()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::SpeedPotion:
						add = (int)pGame->m_csStatus.GetBaseSpeed()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::DeCoolDownPotion:
						add = (int)pGame->m_csStatus.GetBaseCoolDownRevision()*((float)iLevel/100);			
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::MagicPowerPotion:
						add = (int)pGame->m_csStatus.GetMagicPower()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;
					case Skill::SpellID::MagicResistPotion:
						add = (int)pGame->m_csStatus.GetMagicResist()*((float)iLevel/100);					
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], add);
						break;

					case Skill::SpellID::WealthStatue:		// 부의 석상
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], iLevel);
						break;
					
					case Skill::SpellID::PowerStatue:		// 힘의 석상
					case Skill::SpellID::IntStatue:			// 경험의 석상
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], (int)(iLevel * 1));
						break;

					case Skill::SpellID::RealmHP:			// 렐름 HP
					case Skill::SpellID::RealmMP:			// 렐름 MP
						wsprintf(strTooltop, CRYLStringTable::m_strString[ 4600 + i ], (int)(iLevel * 5));
						bShowTime = FALSE;
						break;
					}

					if(bShowTime && iTime != 0)
					{
						if(iTime >= 60 && iTime < 3600)	// Min
							wsprintf(strTemp, "\\%d %s", iTime/60, CRYLStringTable::m_strString[ 4702 ]);
						else if(iTime >= 3600)	// Hour
							wsprintf(strTemp, "\\%d %s", iTime/3600, CRYLStringTable::m_strString[ 4701 ]);
						else 
							wsprintf(strTemp, "\\%d %s", iTime, CRYLStringTable::m_strString[ 4703 ]);

						strcat(strTooltop, strTemp);
					}

					( ( CGMUITooltipBox* )m_lpTooltip )->InitTooltip( strTooltop, 0, 0 ) ;
					m_bShowTooltip = TRUE;
					break;
				}
				
				++iRow;
				if(iRow >= 10)
				{
					iRow = 0;
					iLine++;
				}
			}
		}

		bEdge = TRUE;

		if(g_DeviceInput.GetIsLeftMouseDown() || g_DeviceInput.GetIsMiddleMouseDown() || g_DeviceInput.GetIsRightMouseDown() ||
			g_DeviceInput.GetIsLeftMousePress() || g_DeviceInput.GetIsMiddleMousePress() || g_DeviceInput.GetIsRightMousePress() ||
			g_DeviceInput.GetIsLeftMouseUp() || g_DeviceInput.GetIsMiddleMouseUp() || g_DeviceInput.GetIsRightMouseUp())
		{
			bClick = TRUE;
		}
	}
	return S_OK ;
}


VOID	CRYLEnchantDlg::DlgClick( CGMUIObject* Sender, INT X, INT Y )
{
	if ( !g_pEnchantDlg )
		return ;

	switch( Sender->GetInstance() )
	{
		case RYLENCHANTDLG::HIDE_BUTTON :
			{
				g_pEnchantDlg->SetVisible( FALSE ) ;

				// Visible Flag Setting
				CRYLGameScene* pScene = static_cast<CRYLGameScene*>( CRYLSceneManager::Instance()->GetCurrentScene() ) ;
				if ( pScene ) pScene->SetVisibleInfo( RYLMAINWINDOW::ENCHANT_WINDOW, FALSE ) ;
			}
			break ;
	}
}

CRYLSprite *CRYLEnchantDlg::GetChantIcon(unsigned long dwChantNum)
{
	CRYLSprite *lpChantIcon = NULL;

	if(dwChantNum == 0)
		return NULL;

	switch(dwChantNum)
	{
	case Skill::SpellID::CD_8D12:
	case Skill::SpellID::CD_8D14:
	case Skill::SpellID::CD_8D16:
	case Skill::SpellID::CD_99A1:
	case Skill::SpellID::CD_99A2:
	case Skill::SpellID::CD_99A3:
		return NULL;
	}


	if(dwChantNum == Skill::SpellID::StealHand)
	{
		if( m_lpChant[ 55 ] )
		{
			lpChantIcon = m_lpChant[ 55 ];
			return lpChantIcon;
		}
	}
	else if(dwChantNum == Skill::SpellID::MichaelBless)
	{
		if( m_lpChant[ 56 ] )
		{
			lpChantIcon = m_lpChant[ 56 ];
			return lpChantIcon;
		}
	}

	// ExpOver 밑으로만 출력해준다. 
	if(dwChantNum < Skill::SpellID::ExpOrb)
	{
		if( m_lpChant[ dwChantNum-1 ] )
		{
			lpChantIcon = m_lpChant[ dwChantNum-1 ];
		}
	}

	return lpChantIcon;
}
