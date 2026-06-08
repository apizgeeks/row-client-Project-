//================================================================
//
//	Name : RYLCompleteQuestPanel.h 
//	Desc : ¿Ï·áµÈ Äù½ºÆ® ÆÇ³Ú
//  Date : 2003. 12. 10
//
//================================================================
#ifndef __RYL_COMPLETEQUESTPANEL_H__
#define __RYL_COMPLETEQUESTPANEL_H__

#include "GMUICustomPanel.h"
#include <D3d8.h>
#include <list>

class CRYLLabel ;
class CRYLButton ;
struct QuestInstance ;

class CRYLCompleteQuestSlot : public CGMUICustomPanel
{
private :
	CRYLLabel*			m_pQuestTitleLabel ;			// Äù½ºÆ® Á¦¸ñ
	CRYLLabel*			m_pQuestLevelLabel ;			// Äù½ºÆ® ³­ÀÌµµ
	INT					m_iQuestSlotIndex ;
    //-- 2004. 9. 20. Zergra From. --//
    unsigned long       m_ulCompleteQuestID;            // ¿Ï·áµÈ Äù½ºÆ® ID
    //-- Zergra To. --// 

public :
	CRYLCompleteQuestSlot( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLCompleteQuestSlot() ;

	VOID	MakeCompleteQuestSlot() ;
	VOID	CreateCompleteQuestTitle( unsigned long ulQuestID, LPSTR lpQuestTitle, LPSTR lpQuestLevel ) ;
	
	VOID	SetQuestSlotIndex( INT iIndex )				{ m_iQuestSlotIndex = iIndex ; }
	INT		GetQuestSlotIndex()							{ return m_iQuestSlotIndex ; }

    unsigned long GetQuestID()                          { return m_ulCompleteQuestID; }
	
} ;

namespace RYLCOMPLETEQUESTDLG
{
	const DWORD		PREV_BUTTON				= 0x00000000 ;
	const DWORD		NEXT_BUTTON				= 0x00000001 ;
} ;


class CRYLCompleteQuestPanel : public CGMUICustomPanel
{
private :
	std::list< CRYLCompleteQuestSlot* >	m_vecQuestSlot ;

	CRYLButton*			m_pPrevButton ;
	CRYLButton*			m_pNextButton ;
	CRYLLabel*			m_pPageState ;
	INT					m_iQuestYMargin ;
	INT					m_dwStartSlot ;
	
public :
	CRYLCompleteQuestPanel( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLCompleteQuestPanel() ;

	VOID	InitCtrl() ;
	VOID	FinalCtrl() ;
	
	HRESULT		Update( BOOL &bClick, BOOL &bEdge ) ;
	HRESULT		Render( LPDIRECT3DDEVICE8 lpD3DDevice )  ;
	HRESULT		FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) ;

	static VOID DlgClick( CGMUIObject* Sender, INT X, INT Y ) ;
	
	VOID	SetStartSlot( INT iStart )			{ m_dwStartSlot = iStart ; }
	INT		GetStartSlot()						{ return m_dwStartSlot ; }

	VOID	PrevQuestSlotPage() ;
	VOID	NextQuestSlotPage() ;
		
	VOID	InsertQuestSlot( QuestInstance* lpQuest ) ;
	VOID	UpdateQuestSlotPosition() ;
	VOID	QuestSlotVisible() ;
	VOID	ClearSlot() ;

	std::list< CRYLCompleteQuestSlot* >::iterator FindIter( INT iIndex ) ;

    //-- 2004. 9. 20. Zergra From. --//
    bool    GetIsCompleteQuest( unsigned long ulQuestID );
    //-- Zergra To. --// 
} ;

#endif //__RYL_COMPLETEQUESTPANEL_H__
