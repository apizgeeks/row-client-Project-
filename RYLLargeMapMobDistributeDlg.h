//================================================================
//
//	Name : RYLLargeMapMobDistributeDlg.h 
//	Desc : 라지 맵 다이얼로그
//  Date : 2004. 04. 20
//
//================================================================
#ifndef __RYL_LARGEMAPMOBDISTRIBUTEDLG_H__
#define __RYL_LARGEMAPMOBDISTRIBUTEDLG_H__

#include "GMUICustomPanel.h"
#include <d3dx8.h>


typedef union {
	struct {
		byte	b;
		byte	g;
		byte	r;
		byte	a;
	};
	DWORD color;
} COLOR_ARGB;

namespace RYLMOBDISTRIBUTEDLG
{
	enum	COLOR_DEFINE
	{
		DARKRED				=		0x80420000,					// 128,  66,   0,   0
		RED					=		0x80980000,					// 128, 152,   0,   0
		BRIGHTRED			=		0x80FF1400,					// 128, 255,  20,   0
		ORANGE				=		0x80FF8B36,					// 128, 255, 139,  54
		YELLOW				=		0x80F2E668,					// 128, 242, 230, 104
		GREEN				=		0x8095D887,					// 128, 149, 216, 135
		BLACK				=		0x80323232					// 128,  50,  50,  50
	};
};

class CRYLLabel ;

class CRYLMobDistributeDlg : public CGMUICustomPanel
{
private:
	CRYLSpriteEx*			m_pMobDistributeSprite[ 4 ] ;
	COLOR_ARGB*				m_pOriginSprite[ 4 ];					//현재 Zone의 원본 이미지
	CRYLLabel*				m_pIndexLabel[ 6 ];							//좌측 하단의 툴팁용 
	unsigned char			m_cLevel;								//현재 캐릭의 레벨
		

protected:
	VOID					UpdateDistributeMap();

public :
	CRYLMobDistributeDlg( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLMobDistributeDlg() ;

	VOID					InitCtrl() ;
	VOID					FinalCtrl() ;

	virtual VOID			Show( CGMUIObject* Sender ) ;
	virtual VOID			Hide( CGMUIObject* Sender ) ;

	virtual VOID			GMUIPaint( INT xx, INT yy ) ;

	virtual	HRESULT			Render( LPDIRECT3DDEVICE8 lpD3DDevice )		 { return S_OK ; } ;
	virtual HRESULT			FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) { return S_OK ; } ;
	virtual HRESULT			Update( BOOL &bClick, BOOL &bEdge )  ;


	static VOID				DlgClick( CGMUIObject* Sender, INT X, INT Y ) ;

	VOID					HideControl() ;
	VOID					LoadMobDistributeTexture();
	VOID					PlayerLevelChanged();					//분포도 색깔 변경

} ;







#endif