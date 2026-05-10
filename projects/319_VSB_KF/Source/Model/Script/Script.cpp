#include "stdafx.h"
#include "Script.h"
#include "Err.h"
#include "Common.h"

extern CScript * pFirstScript;

CMainScript::CMainScript( char * _ObjName ) :	IBaseModel ( _ObjName ) 
	{
  ObjName = _ObjName;
  Model = "Скрипты";
	#include "IO_Clear.h"
	#include "Points.h"
	}

CMainScript::~CMainScript()
	{
	}

int CMainScript::Init( int Regim )
	{
	int Res = 0;
	return 0;
	}

int CMainScript::Step0( )
	{
	return 0;
	}

int CMainScript::Step1( )
	{
  CScriptVar * pVar = CScriptVar::pFirstVar;
  while ( pVar )
    {
    pVar->Connect( );
    pVar = pVar->pNextVar;
    }
	return 0;
	}

int CMainScript::StepT ( double dt )
	{
  CScript * pScript = CScript::pFirstScript;
  while ( pScript )
    {
    pScript->StepT( dt );
    pScript = pScript->pNextScript;
    }
	return 0;
	}

int CMainScript::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CMainScript::SaveState( )
  {
	  return 0;
  }

int CMainScript::RestoreState( char * StrName )
  {
	  return 0;
  }

