#include "stdafx.h"
#include "Script.h"
#include "Err.h"
#include "CommProc.h"
#include "Common.h"


_W_MainScript::_W_MainScript()
	{
  memset ( this, 0, sizeof ( _W_MainScript ));
	}


char NoFound[64] = "НЕ НАЙДЕН"; 
int CMainScript::GetParams( char * )
	{
#include "IO_Parms.h"
  TAB("Скрипты",1)
  CScript * pScript = CScript::pFirstScript;
  while ( pScript )
    {
    PARM((char*)pScript->ObjName, "#Скрипт");
    pScript = pScript->pNextScript;
    }
  ETAB
  TAB("Внешние переменные",1)
  CScriptVar * pVar = CScriptVar::pFirstVar;
  while ( pVar )
    {
    if ( pVar->Name == NULL )
      {
      pVar = pVar->pNextVar;
      continue;
      }
    if ( pVar->Type == pVar->Dbl )
      {
      PARM( *(double*)(pVar->pValue), pVar->Name );
      pVar = pVar->pNextVar;
      continue;
      }
    if ( pVar->Type == pVar->Int )
      {
      PARM( *(int*)(pVar->pValue), pVar->Name );
      pVar = pVar->pNextVar;
      continue;
      }
    pVar = pVar->pNextVar;
    }
  ETAB
	return 0;
	}

int CMainScript::UpdateParam( CParams & Param )
  {
  return 0;
  }


int CMainScript::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
  return 0;
  }
