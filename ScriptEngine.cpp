// ScriptEngine.cpp : Defines the entry point for the DLL application.
//

#include "ScriptEngine.h"
#include "SyntaxTree.h"
#include "IntermediateCode.h"
#include "VirtualMachine.h"
#include "Message.h"
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>
#include "GMMemory.h"
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}*/

SCRIPT	_SE_Create( const char * codeFile )
{
	FILE * fp = fopen( codeFile, "r" );
	if( fp == NULL )
	{
		ErrorMessage2( "Cannot open file : %s", codeFile );
		return NULL;
	}

	CVirtualMachine * pVM = new CVirtualMachine;

	char szFilename[128];
	strcpy( szFilename, codeFile );
	strlwr( szFilename );

	if( strstr( szFilename, MACHINECODEFILEEXT ) != 0 )
	{
		pVM->Create( codeFile );
	}
	else if( strstr( szFilename, SCRIPTFILEEXT ) != 0 )
	{
		CSyntaxTree			SyntaxTree;
		CIntermediateCode	IMCode;

		SyntaxTree.Create( codeFile );
		IMCode.Create( SyntaxTree );

		pVM->Create( IMCode, *SyntaxTree.GetSymbolTable() );
	}
	else
	{
		return NULL;
	}

	return pVM;
}

void		_SE_Destroy( SCRIPT Script )
{
	delete Script;
}

bool		_SE_Save( SCRIPT Script, const char * szFilename )
{
	return Script->Save( szFilename );
}

void		_SE_Execute( SCRIPT Script )
{
	Script->Execute();
}

void		_SE_RegisterFunction( SCRIPT Script, ANY_FUNCTION FuncPtr, eDataType RetType, const char * szFuncName,  ... )
{
	va_list args;
	va_start( args, szFuncName );

	Script->RegisterFunction( FuncPtr, RetType, szFuncName, args );
}

SE_FUNC		_SE_GetScriptFunction( SCRIPT Script, eDataType RetType, const char * szFuncName,  ... )
{
	va_list args;
	va_start( args, szFuncName );

	return Script->GetScriptFunction( RetType, szFuncName, args );
}

void		_SE_SetMessageFunction( MESSAGE_FUNCTION func )
{
	SetCompilerMessageFunction( func );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func )
{
	return pScript->CallScriptFunction( Func );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func, AnyData arg1 )
{
	return pScript->CallScriptFunction( Func, arg1 );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func, AnyData arg1, AnyData arg2 )
{
	return pScript->CallScriptFunction( Func, arg1, arg2 );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func, AnyData arg1, AnyData arg2, AnyData arg3 )
{
	return pScript->CallScriptFunction( Func, arg1, arg2, arg3 );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func, AnyData arg1, AnyData arg2, AnyData arg3, AnyData arg4 )
{
	return pScript->CallScriptFunction( Func, arg1, arg2, arg3, arg4 );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func, AnyData args[], int nArgs )
{
	return pScript->CallScriptFunction( Func, args, nArgs );
}

AnyData		_SE_CallScriptFunction( SCRIPT pScript, SE_FUNC Func, AnyData arg0, AnyData arg1, AnyData arg2, AnyData arg3, AnyData arg4,
																  AnyData arg5, AnyData arg6, AnyData arg7, AnyData arg8, AnyData arg9,
																  AnyData arg10, AnyData arg11, AnyData arg12, AnyData arg13, AnyData arg14,
																  AnyData arg15, AnyData arg16, AnyData arg17, AnyData arg18, AnyData arg19,
																  AnyData arg20, AnyData arg21, AnyData arg22, AnyData arg23, AnyData arg24,
																  AnyData arg25, AnyData arg26, AnyData arg27, AnyData arg28, AnyData arg29,
																  AnyData arg30, AnyData arg31, AnyData arg32, AnyData arg33, AnyData arg34 )
{
	return pScript->CallScriptFunction( Func, arg0,
											  arg1,
											  arg2,
											  arg3,
											  arg4,
											  arg5,
											  arg6,
											  arg7,
											  arg8,
											  arg9,
											  arg10,
											  arg11,
											  arg12,
											  arg13,
											  arg14,
											  arg15,
											  arg16,
											  arg17,
											  arg18,
											  arg19,
											  arg20,
											  arg21,
											  arg22,
											  arg23,
											  arg24,
											  arg25,
											  arg26,
											  arg27,
											  arg28,
											  arg29,
											  arg30,
											  arg31,
											  arg32,
											  arg33,
											  arg34 ) ;
}