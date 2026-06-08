//================================================================
//
//	Name : RYLHumanInventoryPanel.h 
//	Desc : 휴먼 인벤토리 페널
//  Date : 2003. 12. 29
//
//================================================================
#ifndef __RYL_HUMANINVENTORYPANEL_H__
#define __RYL_HUMANINVENTORYPANEL_H__

#include "RYLInventoryPanel.h"
#include <d3d8.h>

class CRYLLabel ;
class CRYLCheckBox ;
class CRYLButton ;
class CRYLTabButton ;

class CItemInstance ;

namespace RYLHUMANINVENTORYPANEL
{
	const DWORD		WEAPON_CHECKBOX1			= 0x00000000 ;
	const DWORD		WEAPON_CHECKBOX2			= 0x00000001 ;
	const DWORD		SHIELD_CHECKBOX1			= 0x00000002 ;
	const DWORD		SHIELD_CHECKBOX2			= 0x00000003 ;
	const DWORD		ARMOR_CHECKBOX1				= 0x00000004 ;
	const DWORD		ARMOR_CHECKBOX2				= 0x00000005 ;
	const DWORD		ARMOR_CHECKBOX3				= 0x00000006 ;
	const DWORD		INVEN_CHECKBOX1				= 0x00000007 ;
	const DWORD		INVEN_CHECKBOX2				= 0x00000008 ;
	const DWORD		INVEN_CHECKBOX3				= 0x00000009 ;
	const DWORD		INVEN_CHECKBOX4				= 0x00000010 ;
	const DWORD		STREETSTALL_BUTTON			= 0x00000011 ;
} ;

class CRYLHumanInventoryPanel : public CRYLInventoryPanel
{
private :

	CRYLTabButton*			m_pTabButton ;
	CRYLCheckBox*			m_pWeaponButton[ 2 ] ;
	CRYLCheckBox*			m_pShieldButton[ 2 ] ;
	CRYLCheckBox*			m_pArmorButton[ 3 ] ;
	CRYLCheckBox*			m_pInvenButton[ 4 ] ;

	CRYLButton*				m_pStreetStallButton ;			// 노점상 버튼
	CRYLLabel*				m_pCurrentGoldLabel ;

	DWORD					m_dwBodyInven ;

	DWORD					m_dwRideFlag ;

public :

	CRYLHumanInventoryPanel( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLHumanInventoryPanel() ;

	VOID	InitCtrl() ;
	VOID	FinalCtrl() ;
	
	HRESULT		Update( BOOL &bClick, BOOL &bEdge ) ;
	HRESULT		Render( LPDIRECT3DDEVICE8 lpD3DDevice )  ;
	HRESULT		FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) ;

	static VOID DlgClick( CGMUIObject* Sender, INT X, INT Y ) ;

	CRYLCheckBox*		GetWeaponCheckButton( INT iIndex )		{ return m_pWeaponButton[ iIndex ] ; }
	CRYLCheckBox*		GetArmorCheckButton( INT iIndex )		{ return m_pArmorButton[ iIndex ] ; }
	CRYLCheckBox*		GetShieldCheckButton( INT iIndex )		{ return m_pShieldButton[ iIndex ] ; }
	CRYLCheckBox*		GetInvenCheckButton( INT iIndex )		{ return m_pInvenButton[ iIndex ] ; }

	VOID	UpdateRideFlag( INT iIndex );

	// 체크 버튼 업데이트
	VOID	UpdateWeaponCheckButton( INT iIndex ) ;
	VOID	UpdateArmorCheckButton( INT iIndex ) ;
	VOID	UpdateInvenCheckButton( INT iIndex ) ;

	VOID	SetArmorIndex( DWORD dwIndex )					{ m_dwBodyInven = dwIndex ; }
	BOOL	SelectItem() ;

	// 사용 무기 선택
	VOID	InventorySelectWeapon( INT iIndex ) ;

	VOID	UpdateInvenItemPos(VOID);

	ULONG	GetAllRepairMoney() ;
} ;

#endif //__RYL_HUMANINVENTORYPANEL_H__