//=================================================================
// Name : RYLStringTable.h
// Data : 2003. 11. 13
// Desc : RYL String Table 
//			- Client에서 문자열 배열
//=================================================================

#ifndef __RYL_STRINGTABLE_H__
#define __RYL_STRINGTABLE_H__

#include <Utility/Setup/GlobalConstants.h>
#include <windows.h>

class CRYLStringTable
{
public :

    enum
    {
        MAX_GAMEGUARD_CODE_LEN = 64
    };

	static unsigned long	m_dwStringCount ;
	static char				m_strString[ 8192 ][ 1024 ] ;
	static unsigned long	m_dwTextSize[ 129 ] ;

	static GameRYL::ServiceNation	m_eInternationalCode;						// 국가 코드
    static GameRYL::ServerType		m_eServerType;								// 서버 타입
    static char             		m_szGameGuardCode[MAX_GAMEGUARD_CODE_LEN];  // 게임 가드 초기화 코드

private :

	CRYLStringTable()	{} ;
	virtual ~CRYLStringTable()  {} ;

public :

	static bool		Load(const char *strScriptFile, const char *strMacroFile) ;

	static char*	MakeWordBreak( const char *strString, unsigned long dwWidth ) ;
	static char*	MakePrintGold( unsigned long dwGold ) ;
	static char*	MakePrintGoldAbleMinus( long lGold ) ;

	static VOID		CreateTextSize() ;
	static INT		GetStringWidth( LPCTSTR lpStr ) ;

    // 문자열의 길이가 dwWidth 이상인지 체크해서 넘어가면 넘어간 부분에 대해서 ... 처리한다. 
	static char*	CheckStringWidth( LPCTSTR lpStr, unsigned long dwWidth ) ;	

	static DWORD	GetChatColor( DWORD dwCommand ) ;

	static char*	GetJobName( unsigned long dwClass ) ;

	// 주의 : strResult 의 크기가 strSource 보다 크거나 같아야 한다!!
	static VOID		EraseToken( char *strResult, char *strSource, char *szToken ) ;
	static VOID		GetFirstStringByToken( char *strResult, char *strSource, char *szToken ) ;
} ;

#endif // __RYL_STRINGTABLE_H__
