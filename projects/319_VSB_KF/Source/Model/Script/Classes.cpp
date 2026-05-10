#include "stdafx.h"
#include "Script.h"
#include "Err.h"
#include "Common.h"


CScriptVar * CScriptVar::pFirstVar = NULL;
CScriptVar * CScriptVar::pLastVar = NULL;

///////////////////////////////////////////////////////////////////////////////////////////
CScriptVar::CScriptVar()
  {
  if ( pFirstVar == NULL )
    pFirstVar = this;
  if ( pLastVar )
    pLastVar->pNextVar = this;
  pLastVar = this;
  pNextVar = NULL;
  Ref = NULL;
  pValue = NULL;
  if ( CScript::pFirstScriptVar == NULL )
    CScript::pFirstScriptVar = this;
  CScript::pLastScriptVar = this;
  }

///////////////////////////////////////////////////////////////////////////////////////////
CScriptDouble::CScriptDouble( char * _Ref ) : CScriptVar()
  {
  Type = Dbl;
  Ref = _Ref;
  Name = _Ref;
  pValue = &Value;
  }


///////////////////////////////////////////////////////////////////////////////////////////
CScriptInt::CScriptInt( char * _Ref ) : CScriptVar()
  {
  Type = Int;
  Ref = _Ref;
  Name = _Ref;
  pValue = &Value;
  }

CScriptRsu::CScriptRsu( char * _Ref ) : CScriptVar()
{
	Type = Rsu;
	Ref = _Ref;
	Name = _Ref;
}

///////////////////////////////////////////////////////////////////////////////////////////
CScriptDouble::CScriptDouble( ) : CScriptVar()
  {
  Type = Dbl;
  pValue = &Value;
  Ref = NULL;
  Name = NULL;
  }


///////////////////////////////////////////////////////////////////////////////////////////
CScriptInt::CScriptInt( ) : CScriptVar()
  {
  Type = Int;
  pValue = &Value;
  Ref = NULL;
  Name = NULL;
  }

CScriptRsu::CScriptRsu( ) : CScriptVar()
{
	Type = Rsu;
	Ref = NULL;
	Name = NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////

CScript * CScript::pFirstScript = NULL;
CScript * CScript::pLastScript = NULL;
CScriptVar * CScript::pFirstScriptVar = NULL;
CScriptVar * CScript::pLastScriptVar = NULL;


CScript::CScript( char * _ObjName ) :	IBaseModel ( _ObjName ) 
  {
  if ( pFirstScript == NULL )
    pFirstScript = this;
  if ( pLastScript )
    pLastScript->pNextScript = this;
  pLastScript = this;
  ObjName = _ObjName;
  Model = "Скрипт";
  pFirstVar = pFirstScriptVar;
  pLastVar = pLastScriptVar;
  pFirstScriptVar = NULL;
  pLastScriptVar = NULL;
  memset(pAnalog, 0, MAX_ANALOG * sizeof(CAnalog*));
  nAnalogCount = 0;
  }

int CScript::Init( int Regim )
  {
  CScriptVar * pVar = pFirstVar;
  while ( pVar )
  {
	  if ( pVar->Name == NULL )
      {
		  pVar = pVar->pNextVar;
		  continue;
      }
	  if(strchr(pVar->Name, '.'))
	  {
		  pVar = pVar->pNextVar;
		  continue;
      }
	  if(nAnalogCount > MAX_ANALOG)	//Сделать больше MAX_ANALOG
		  ASS(false);
	  if(pVar->Type == CScriptVar::Int || pVar->Type == CScriptVar::Rsu)
	  {
		  pVar = pVar->pNextVar;
		  continue;
      }
	  pAnalog[nAnalogCount] = new CAnalog(this, pVar->Name, "AO");
	  pAnalog[nAnalogCount]->pSrcAddr = (double*)pVar->pValue;
	  nAnalogCount++;
	  pVar = pVar->pNextVar;
  }

  return 0;
  }
int CScript::Step0( )
  {
  return 0;
  }
int CScript::Step1( )
  {
  return 0;
  }
//int StepT(double dt);
int CompVar ( const void * p1, const void * p2 )
  {
  CScriptVar ** pV1 = (CScriptVar **)p1;
  CScriptVar ** pV2 = (CScriptVar **)p2;
  return lstrcmp ( (*pV1)->Name, (*pV2)->Name );
  }
static char NoFind[64] = "НЕ НАЙДЕН";
int CScript::GetParams( char * )
  {
#include "IO_Parms.h"
  int kVar = 0;
#define MAX 1000
  CScriptVar * nVar[MAX];
  CScriptVar * pVar = pFirstVar;
  while ( pVar )
    {
    if ( pVar->Name == NULL )
      {
      pVar = pVar->pNextVar;
      continue;
      }
    if ( pVar->Type != pVar->Rsu && pVar->Type != pVar->Dbl && pVar->Type != pVar->Int )
      continue;
    nVar[kVar++] = pVar;
    pVar = pVar->pNextVar;
    if ( kVar >= MAX )
      break;
    }
//
  qsort ( nVar, kVar, sizeof( CScriptVar *), CompVar );
  pVar = pFirstVar;
  for ( int n = 0; n < kVar; n++ )
    {
    CScriptVar * pVar = nVar[n];
    if ( pVar->Type == pVar->Rsu )
      {
      CScriptRsu * pRSU = (CScriptRsu*)pVar;
      PARM( pRSU->Show, pVar->Name );
      continue;
      }
    if ( pVar->Type == pVar->Dbl )
      {
      if ( pVar->pValue == NULL )
        {
        PARM( NoFind, pVar->Name );
        }
      else
        {
        PARM( *(double*)(pVar->pValue), pVar->Name );
        }
      continue;
      }
    if ( pVar->Type == pVar->Int )
      {
      if ( pVar->pValue == NULL )
        {
        PARM( NoFind, pVar->Name );
        }
      else
        {
        PARM( *(int*)(pVar->pValue), pVar->Name );
        }
      continue;
      }
    ASS(0)
    }
  ACS_Params( "АСУ", this );
  return 0;
  }
int CScript::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
  return 0;
  }
int CScript::UpdateParam( struct CParams & Param )
  {
  return 0;
  }
int CScript::SaveState ( )
{
	int nLen = 0;
	CScriptVar * pVar = pFirstVar;	
	while(pVar)	//Определяем размер
	{		
		nLen += lstrlen(pVar->Name) + 1;
		nLen += sizeof(pVar->Type);
		switch(pVar->Type)
		{
		case CScriptVar::Dbl:
			nLen += sizeof(double);
			break;
		case CScriptVar::Int:
			nLen += sizeof(int);
			break;
		default:
			KKK();
		}		 
		pVar = pVar->pNextVar;
	}	//
	if(nLen == 0)
		return 0;
	char* pStruct = NULL;	
	pStruct = new char[nLen];
	int nSize = 0;
	pVar = pFirstVar;	
  double C0 = 0.;
  int I0 = 0;
	while(pVar)	//Заполняем условную структуру
	{	
		
		lstrcpy(&pStruct[nSize], pVar->Name);
		nSize += lstrlen(pVar->Name) + 1;
		memcpy(&pStruct[nSize], &pVar->Type, sizeof(pVar->Type));
		nSize += sizeof(pVar->Type);
		switch(pVar->Type)
		{
		case CScriptVar::Dbl:
      if ( pVar->pValue )
			  memcpy(&pStruct[nSize], pVar->pValue, sizeof(double));
      else
        memcpy(&pStruct[nSize], &C0, sizeof(double));
			nSize += sizeof(double);
			break;
		case CScriptVar::Int:
      if ( pVar->pValue )
			  memcpy(&pStruct[nSize], pVar->pValue, sizeof(int));
      else
        memcpy(&pStruct[nSize], &I0, sizeof(int));
			nSize += sizeof(int);
			break;
		default:
			KKK();
		}		 

		pVar = pVar->pNextVar;
	}//
	ASS(nSize == nLen);
	pSRFile->WStruct(ObjName, nSize, pStruct);
	delete[] pStruct;
	/*int nLen = 0;
	static int Key = 0xE1320724;
	if ( !pSRFile->WF( &Key, sizeof ( Key ) )) 
		return 1;
	int nObjNameLen = lstrlen(ObjName);
	if ( !pSRFile->WF( &nObjNameLen, sizeof ( nObjNameLen ) )) 
		return 1;
	if ( !pSRFile->WF( ObjName, nObjNameLen )) 
		return 1;
	nLen = 64;
	char p[64];
	lstrcpy(p, "TestText");
	if ( !pSRFile->WF( &nLen, sizeof(nLen) )) 
		return 1;
	if ( !pSRFile->WF( p, nLen )) 
		return 1;*/
	return 0;
}
int CScript::RestoreState ( char * StrName )
{
	int nLen = pSRFile->Bytes;
	if(nLen == 0)
		return 0;
	char * pStruct;
	pStruct = new char[nLen];
	pSRFile->RStruct(nLen, pStruct);
	int nSize = 0;
	while(nSize < nLen)
	{
		char szVarName[256] = {0};
		lstrcpy(szVarName, &pStruct[nSize]);
		nSize += lstrlen(szVarName) + 1;
		CScriptVar::eType Type;
		memcpy(&Type, &pStruct[nSize], sizeof(Type));
		nSize += sizeof(Type);
		union Value
		{
			int Int;
			double Dbl;
		} value;
		switch(Type)
		{
		case CScriptVar::Dbl:
			memcpy(&value.Dbl, &pStruct[nSize], sizeof(double));
			nSize += sizeof(double);
			break;
		case CScriptVar::Int:
			memcpy(&value.Int, &pStruct[nSize], sizeof(double));
			nSize += sizeof(int);
			break;
		default:
			KKK();			
		}
		CScriptVar * pVar = pFirstVar;		
		while(pVar)
		{
			if(!lstrcmp(pVar->Name, szVarName))
			{
				ASS(pVar->Type == Type);
				switch(pVar->Type)
				{
				case CScriptVar::Dbl:
          if ( pVar->pValue )
					  (*(double*)pVar->pValue) = value.Dbl;
					break;
				case CScriptVar::Int:
          if ( pVar->pValue )
					  (*(int*)pVar->pValue) = value.Int;
					break;
				default:
					KKK();			
				}
				break;
			}
			pVar = pVar->pNextVar;
		}
	}
	ASS(nSize == nLen);
	delete[] pStruct;	
	return 0;
}

int CScript::SetData( int TypeData, void * pData )
{
	
	return 0;
}

