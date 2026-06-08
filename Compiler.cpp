// Compiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "../ScriptEngine.h"

using namespace std;

void	ScriptMessage( const char * msg )
{
	cout << "##" << msg << "##\n";
}

int main(int argc, char* argv[])
{
	if( argv[1] != NULL )
	{
		_SE_SetMessageFunction( ScriptMessage );
		SCRIPT Script;
		Script = _SE_Create( argv[1] );
		
		char szOutFile[1024];
		strcpy( szOutFile, argv[1] );

		char * p = strstr( szOutFile, ".gsf" );
		if( p )
		{				
			strcpy( p, ".mcf" );

			_SE_Save( Script, szOutFile );
			_SE_Destroy( Script );

			cout << "**Congratulation**\n";
			
			return 0;
		}
		_SE_Destroy( Script );
	}

	cout << "usage : ScriptTest [gsf filename]" << endl;

	return 0;
}
