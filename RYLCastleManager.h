//=================================================================
// Name : RYLCastleManager.h
// Data : 2004. 05. 18
// Desc : Castle Manager Class
//=================================================================
#ifndef __RYL_CASTLE_MANAGER_H__
#define __RYL_CASTLE_MANAGER_H__

#include <map>
#include <windows.h>
#include <Castle/CastleConstants.h>

class CRYLCastle ;
struct vector3 ;

class CRYLCastleManager
{
private : 
	enum eSiegeTime
	{
		TYPE_CHANGE_SIEGE_TIME		= 0,			// 공성 시간대 변경
		TYPE_ENABLE_CASTLE_SIEGE	= 1,			// 특정 성의 공성 가능 정보 변경 
		TYPE_CHARACTER_LOGIN		= 2,			// 캐릭터 로그인시 공성 시간 정보
	};

	std::map< unsigned long, CRYLCastle* >		m_mapCastle ;	// map::< Castle ID, Castle Infomation >
	CRYLCastle* m_pCurrentCastle ;

public :
	CRYLCastleManager() ;
	virtual ~CRYLCastleManager() ;

	VOID		ClearAllCastle() ;

	HRESULT		InsertCastle( CRYLCastle* pCastle ) ;
	CRYLCastle*	FindCastle( unsigned long dwCastleID ) ;
	CRYLCastle*	FindCastleByNameID( unsigned long dwNameID ) ;
	CRYLCastle*	FindCastleInBlessArea( const vector3& vecPos ) ;
	CRYLCastle*	GetCurrentCastle()						{ return m_pCurrentCastle ;}

// Signton
public :
	static CRYLCastleManager*	s_pInstance ;
	static CRYLCastleManager*	Instance()			{ return s_pInstance ; } ;
} ;

#endif //__RYL_CASTLE_MANAGER_H__