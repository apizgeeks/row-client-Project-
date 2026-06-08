
#include <iostream>
#include <conio.h>			//getch

using namespace std;

#include "..\ScriptEngine.h"


void	PrintInt( int n )
{
	cout << "Print( int ) ==> " << n << endl;
}

void	PrintFloat( float f )
{
	cout << "Print( float ) ==> " << f << endl;
}

void	PrintStr( const char * str )
{
	cout << "Print( const char * ) ==> " << str << endl;
}

void	PrintBOOL( BOOL b )
{
	cout << "Print( bool ) ==> ";
	if( b )
		cout << "true";
	else
		cout << "false";
	cout << endl;
}

void	ScriptMessage( const char * msg )
{
	cout << "##" << msg << "##\n";
}

void	main( int argc, char *argv[] )
{
	//!!!!!!!!!!시간나면 테스트를 좀 더 자동화 할 것!!!!!!!!!!!//
	//지금은 각 부분의 테스트를 하고 Print해서 찍도록 한 다음에
	//직접 확인하고 있지만... assert( test == 결과값 ) 같은 형식으로 하면
	//assert의 결과가 ok인지 아닌지만 확인하면 되므로 훨씬 효율적임.

	_SE_SetMessageFunction( ScriptMessage );
	SCRIPT Script;

	if( argc != 2 )
	{		
		Script = _SE_Create( "TestCode.gsf" );
	}
	else
	{
		Script = _SE_Create( argv[1] );
	}

	if( Script == 0 )
		cout << "usage : ScriptTest [gsf filename]" << endl;

	_SE_RegisterFunction( Script, PrintInt, T_VOID, "Print", T_INT, T_VOID );
	_SE_RegisterFunction( Script, PrintFloat, T_VOID, "Print", T_FLOAT, T_VOID );
	_SE_RegisterFunction( Script, PrintStr, T_VOID, "Print", T_STRING, T_VOID );
	_SE_RegisterFunction( Script, PrintBOOL, T_VOID, "Print", T_BOOL, T_VOID );
	_SE_Execute( Script );

	SE_FUNC ScriptMain = _SE_GetScriptFunction( Script, T_VOID, "ScriptMain", 0 );
	_SE_CallScriptFunction( Script, ScriptMain );

	SE_FUNC getDoubleFunc = _SE_GetScriptFunction( Script, T_INT, "getDouble", T_INT, 0 );
	AnyData result = _SE_CallScriptFunction( Script, getDoubleFunc, 1000 );

	cout << "getDouble() ==> " << result.intValue << endl;

	SE_FUNC getDoubleFunc2 = _SE_GetScriptFunction( Script, T_FLOAT, "getDouble", T_FLOAT, 0 );
	AnyData result2 = _SE_CallScriptFunction( Script, getDoubleFunc2, 0.534f );

	cout << "getDouble(float) ==> " << result2.floatValue << endl;



	SE_FUNC TestFloat1 = _SE_GetScriptFunction( Script, T_FLOAT, "TestFloat", T_FLOAT, T_FLOAT, 0 );
	result = _SE_CallScriptFunction( Script, TestFloat1, 0.325f, 13.535f );					//4.398875
	cout << "TestFloat( float, float ) ==> " << result.floatValue << endl;

	SE_FUNC TestFloat2 = _SE_GetScriptFunction( Script, T_FLOAT, "TestFloat", T_FLOAT, T_FLOAT, T_FLOAT, 0 );
	result = _SE_CallScriptFunction( Script, TestFloat2, 0.23415f, 8412.5325f, 212.53f );	//418640.4218
	cout << "TestFloat( float, float, float ) ==> " << result.floatValue << endl;

	SE_FUNC TestFloat3 = _SE_GetScriptFunction( Script, T_FLOAT, "TestFloat", T_FLOAT, T_FLOAT, T_FLOAT, T_FLOAT, 0 );
	result = _SE_CallScriptFunction( Script, TestFloat3, 32.3f, 35.2f, 8.2f, 93.66f );
	cout << "TestFloat( float, float, float, float ) ==> " << result.floatValue << endl;	//873198.92352


	cout << "Press any key to continue...\n";
//	getch();

	_SE_Destroy( Script );
}
