//================================================================
//
//	Name : RYLEmblemUpgrade.h 
//	Desc : 상징물의 업그레이드 다이얼로그
//  Date : 2004. 06. 01
//
//================================================================
#ifndef __RYL_EMBLEMUPGRADEDLG_H__
#define __RYL_EMBLEMUPGRADEDLG_H__

#include "RYLDialog.h"

namespace RYLEMBLEMUPGRADEDLG
{
	const DWORD		OK_BUTTON				= 0x00000000 ;// 확인 버튼
	const DWORD		CANCEL_BUTTON			= 0x00000001 ;// 취소 버튼
};

class CRYLButton ;
class CRYLLabel ;
class CRYLCastle ;

class CRYLEmblemUpgradeDlg : public CRYLDialog
{
public:
	enum eUpgradeError
	{
		S_ALL_READY		= 0,
		E_NOT_RIGHT,
		E_MAX_LEVEL,
		E_NOT_ENOUGH_NUM,
		E_DIFFERENT_JEWEL
	};

private :
	CRYLLabel*		m_pEmblemUpgradeLabel ;				// 상징물의 업그레이드
	CRYLLabel*		m_pJewelLabel ;						// [보석조각]
	CRYLLabel*		m_pEmblemUpgradeExpla1Label ;		// 업그레이드 설명 1
	CRYLLabel*		m_pEmblemUpgradeExpla2Label ;		// 업그레이드 설명 2
	CRYLLabel*		m_pEmblemUpgradeExpla3Label ;		// 업그레이드 설명 3
	CRYLLabel*		m_pEmblemUpgradeExpla4Label ;		// 업그레이드 설명 4
	CRYLButton*		m_pOKButton ;
	CRYLButton*		m_pCancelButton ;

	unsigned long	m_dwCastleID ;						// 상징물이 속해 있는 성 ID
	unsigned long	m_dwEmblemCID ;						// 상징물의 크리쳐 ID

	eUpgradeError	CheckUpgradable() ;					// 업그레이드가 가능한지 체크

public :
	CRYLEmblemUpgradeDlg() ;
	virtual ~CRYLEmblemUpgradeDlg() ;

	VOID	InitCtrl() ;
	VOID	FinalCtrl() ;

	VOID	InitValue() ;

	virtual VOID Show( CGMUIObject* Sender ) ;
	virtual VOID Hide( CGMUIObject* Sender ) ;

	virtual VOID GMUIPaint( INT xx, INT yy ) ;

	virtual	HRESULT		Render( LPDIRECT3DDEVICE8 lpD3DDevice )  ;
	virtual HRESULT		FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) ;
	virtual HRESULT		Update( BOOL &bClick, BOOL &bEdge ) ;

	static VOID	DlgClick( CGMUIObject* Sender, INT X, INT Y ) ;
	
	VOID	SetCastleAndEmblemID( unsigned long dwCastleID, unsigned long dwEmblemID )
	{
		m_dwCastleID = dwCastleID ;
		m_dwEmblemCID = dwEmblemID ;
	}

	unsigned long	GetCastleID()		{ return m_dwCastleID ; }
	unsigned long	GetEmblemCID()		{ return m_dwEmblemCID;	}

	void	SetEmblemInfo( unsigned long dwEmblemID ) ;
} ;

#endif // __RYL_EMBLEMUPGRADEDLG_H__