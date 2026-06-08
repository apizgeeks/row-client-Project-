//================================================================
//
//	Name : RYLExecutionQuestPanel.h 
//	Desc : ¼öÇàÁßÀÎ Äù½ºÆ® ÆÇ³Ú
//  Date : 2003. 12. 10
//
//================================================================
#ifndef __RYL_EXECUTIONQUESTPALEL_H__
#define __RYL_EXECUTIONQUESTPALEL_H__

#include "GMUICustomPanel.h"
#include "RYLButton.h"
#include "..\\CharStatus.h"
#include <d3d8.h>
#include <list>

class CRYLLabel ;

class CRYLExeQuestMapButton : public CRYLButton
{
private :
	INT			m_iQuestIndex ;
	BOOL		m_bAlwaysHide ;				// Ç×»ó ¼û°ÜÁ®¾ß ÇÏ´ÂÁö ¿©ºÎ

public :
	CRYLExeQuestMapButton( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLExeQuestMapButton() ;

	INT		GetQuestIndex()					{ return m_iQuestIndex ; }
	VOID	SetQuestIndex( INT iIndex )		{ m_iQuestIndex = iIndex ; }

	VOID	SetAlwaysHide( BOOL bAlwaysHide )	{ m_bAlwaysHide = bAlwaysHide; }
	BOOL	GetAlwaysHide( )					{ return m_bAlwaysHide ; }

	virtual VOID SetVisible( BOOL bVisible ) ;
} ;

class CRYLExecutionQuestSlot : public CGMUICustomPanel
{
public :
	CRYLExeQuestMapButton*	m_pQuestMapButton ;			// Äù½ºÆ® ¸ã
	CRYLExeQuestMapButton*	m_pQuestTextButton ;		// Äù½ºÆ® ¼³¸í ¹öÆ°
	CRYLExeQuestMapButton*  m_pQuestQuitButton ;		// Äù½ºÆ®Á¾·á ¹öÆ°

private :
	CRYLLabel*			m_pQuestTitleLabel ;			// Äù½ºÆ® Á¦¸ñ
	CRYLLabel*			m_pQuestLevelLabel ;			// Äù½ºÆ® ·¹º§
	
	INT					m_iQuestSlotIndex ;				// Äù½ºÆ®½½·Ô ÀÎµ¦½º
public :
	CRYLExecutionQuestSlot( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLExecutionQuestSlot() ;

	VOID		MakeExeQuestSlot() ;
	VOID		CreateExeQuestTitle( LPSTR lpQuestTitle ) ;
	VOID		CreateQuestLevelAndPhase( char* szLevel, INT iCurPage, INT iTotalPage ) ;
	VOID		SetQuestLevelAndPhase( char* szLevel, INT iCurPage, INT iTotalPage ) ;
	
	VOID		SetQuestSlotIndex( INT iIndex )				{ m_iQuestSlotIndex = iIndex ; }
	INT			GetQuestSlotIndex()							{ return m_iQuestSlotIndex ; }

	const CHAR*	GetQuestTitle() ;
} ;

namespace RYLEXECUTIONQUESTDLG
{
	const DWORD		PREV_BUTTON				= 0x00000000 ;
	const DWORD		NEXT_BUTTON				= 0x00000001 ;
	const DWORD		SLOT_PANEL				= 0x00000002 ;
	const DWORD		MAP_BUTTON				= 0x00000003 ;
	const DWORD		QUIT_BUTTON				= 0x00000004 ;
	const DWORD		TEXT_BUTTON				= 0x00000005 ;
} ;

typedef struct Message
{
	CHAR	strMessage[ MAX_PATH ] ;
} Message, *LPMessage ;

class CRYLExecutionQuestPanel : public CGMUICustomPanel
{
private :
	std::list< CRYLExecutionQuestSlot* >	m_vecQuestSlot ;
	CRYLButton*			m_pPrevButton ;
	CRYLButton*			m_pNextButton ;
	CRYLLabel*			m_pPageState ;
	INT					m_iQuestYMargin ;
	DWORD				m_dwProcessQuestTextIndex ;		// ÇöÀç ´­·¯Áø Äù½ºÆ® ¼³¸íÀÇ Äù½ºÆ® ½½·Ô ÀÎµ¦½º
	DWORD				m_dwProcessQuestMapIndex ;		// ÇöÀç ´­·¯Áø Äù½ºÆ® ¸ÊÀÇ Äù½ºÆ® ½½·Ô ÀÎµ¦½º
	INT					m_iQuestButtonIndex ;			// Äù½ºÆ® ¹öÆ° ÀÎµ¦½º
	DWORD				m_dwStartSlot ;
	

	vector<LPMessage>	m_lstMessage ;

	LONG				m_lStartQuest ;
	LONG				m_lStartDesc ;
	LONG				m_lStartDone ;

	DWORD				m_dwSelectQuestSlot ;
	DWORD				m_dwQuestWindowMode ;

public :
	QuestInstance*		m_lpDeleteQuest ;
	QuestInstance*		m_lpSelectQuest ;
	
	
public :
	DWORD				m_dwDeleteQuestResult ;

public :
	CRYLExecutionQuestPanel( CGMUICustomDlg *pParentDlg ) ;
	virtual ~CRYLExecutionQuestPanel() ;

	VOID	InitCtrl() ;
	VOID	FinalCtrl() ;
	
	HRESULT		Update( BOOL &bClick, BOOL &bEdge ) ;
	HRESULT		Render( LPDIRECT3DDEVICE8 lpD3DDevice )  ;
	HRESULT		FinalRender( LPDIRECT3DDEVICE8 lpD3DDevice ) ;

	static VOID DlgClick( CGMUIObject* Sender, INT X, INT Y ) ;

	std::list< CRYLExecutionQuestSlot* >::iterator	FindIter( INT iIndex ) ;
	std::list< CRYLExecutionQuestSlot* >::iterator	FindIter( const CHAR* strTitle ) ;
	CRYLExecutionQuestSlot*							FindPtr( INT iIndex ) ;
	CRYLExecutionQuestSlot*							FindPtr( const CHAR* strTitle ) ;
	VOID	DeleteIter( INT iIndex ) ;

	VOID	QuestSlotVisible() ;
	VOID	UpdateQuestSlotPosition() ;

	VOID	PrevQuestSlotPage() ;
	VOID	NextQuestSlotPage() ;
	
	VOID	SetStartSlot( INT iStart )			{ m_dwStartSlot = iStart ; }
	INT		GetStartSlot()						{ return m_dwStartSlot ; }

	VOID	SetProcessQuestTextIndex( INT iIndex ) ;
	VOID	SetProcessQuestMapIndex( INT iIndex ) ;
	VOID	SetProcessQuestQuitIndex( INT iIndex ) ;
	DWORD	GetProcessQuestTextIndex()			{ return m_dwProcessQuestTextIndex ; }
	DWORD	GetProcessQuestMapIndex()			{ return m_dwProcessQuestMapIndex ; }

	VOID	InitDesc() ;
	VOID	InitValue() ;
	VOID	InsertExecutionQuest( QuestInstance* lpQuest ) ;
	VOID	ClearSlot() ;
	VOID	ShowDetailDataDlg() ;
	VOID	HideDetailDataDlg( QuestInstance* lpQuest ) ;
} ;

#endif //__RYL_EXECUTIONQUESTPALEL_H__
