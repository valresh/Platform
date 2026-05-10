#include "stdafx.h"
#define DLL_IntReg
#include "IntReg.h"
#include "math.h"
#include "Err.h"
#include "Connect.h"
#include "SetData.h"


CSetComp::CSetComp( IBaseModel * pMain, char * Name ) : IBaseModel ( Name, pMain ), ISet ( this )
{
#include "IO_Clear.h"
  //
  //
  Model = "SetComp";
  pMain->AddSet( this );
  W = NULL;
  CLEAR(TestCompName)
  }

CSetComp_W::CSetComp_W()
  {
  memset ( this, 0, sizeof ( CSetComp_W ));
  }

CWComp::CWComp()
  {
  memset ( this, 0, sizeof ( CWComp ));
  }

int CSetComp::GetParams( char * )
  {
  #include "IO_Parms.h"
  char Name[128];
  char * pName = ObjName;
  while ( 1 )
    {
    char * pNext = strchr ( pName, '/' );
    if ( pNext == NULL )
      break;
    pName = pNext + 1;
    }
  sprintf_s ( Name, 128, "%s", pName );
//
  if ( W )
    lstrcpy ( TestCompName, W->CompName );
  else
    lstrcpy ( TestCompName, "Не задан" );
  PARM( TestCompName, Name );
  return 0;
  }


void CSetComp::Create( )
  {
  if ( lstrcmp ( TestCompName, "Не задан" ) != 0 )
    {
  // Состав задан в параметрах
    if ( TestCompName[0] == ' ' || TestCompName[0] == 0 )
      {
      W = NULL;
      Use = false;
      return;
      }
    Use = true;
    if ( W == NULL )
      W = new CWComp;
    if ( lstrlen ( TestCompName ) <= 31 )
      {
      lstrcpy ( W->CompName, TestCompName );
      W->Comp.Read ( W->CompName );
      }
    else
      Use = false;
    }
  }

int CSetComp::UpdateParam( struct CParams & Param )
  {
  if ( Param.Addr == TestCompName )
    {
    if ( lstrcmp ( TestCompName, "Не задан" ) == 0 )
      {
      Use = false;
      return 0;
      }
// Состав поменялся
    if ( TestCompName[0] == ' ' || TestCompName[0] == 0 )
      {
      W = NULL;
      Use = false;
      return 0;
      }
    Use = true;
    if ( W == NULL )
      W = new CWComp;
    if ( lstrlen ( TestCompName ) <= 31 )
      {
      lstrcpy ( W->CompName, TestCompName );
      if ( !W->Comp.Read ( W->CompName ))
        Use = false;
      }
    else
      Use = false;
    }
  return 0;
  }

void CSetComp::OutValue ( char Txt[64] )
  {
  if ( W )
    {
    lstrcpy ( Txt, W->CompName );
    }
  else
    {
    lstrcpy ( Txt, "Не задан" );
    }
  }

int CSetComp::SaveState( )
  {
  S_CLASS("W",CSetComp_W)
  if ( W )
    {
    ASS(pSRFile->WStruct( "Comp", sizeof ( CComp_W ), W ));
    }
  return 0;
  }

int CSetComp::RestoreState( char * StrName )
  {
  R_CLASS("W",CSetComp_W )
  if ( lstrcmp ( StrName, "Comp" ) == 0 )
    {
    if ( W == NULL )
      W = new CWComp;
    ASS(pSRFile->RStruct( sizeof ( CComp_W ), W ));
    return 0;
    }
  return 1;
  }

bool CSetComp::Set( CComp * pComp )
  {
  if ( Use && W == NULL )
    {
    W = new CWComp;
    W->Comp.Set ( pComp );
    lstrcpy ( W->CompName, "Безимянный" );
    }
  if ( Use )
    {
    pComp->Set ( &W->Comp );
    return true;
    }
//  if ( W )
//    W->Comp.Set ( pComp );
  return false;
  }

int CSetComp::SetData( int TypeData, void * pData )
  {
  if ( TypeData == sd_GetCompInfo )
    {
    if ( W == NULL )
      return 0;
    CExtCompInfo * pInfo = (CExtCompInfo*)pData;
    pInfo->Cmol = W->Comp.Cmol;
    pInfo->IsComp = W->Comp.IsComp;
    pInfo->P = NULL;
    pInfo->T = NULL;
    pInfo->pLAST_GAS = &LAST_GAS;
    pInfo->pGas = pGas;
    pInfo->CompName = W->CompName;
    return sd_GetCompInfo;
    }
  if ( TypeData == sd_SetCompData )
    {
    if ( W == NULL )
      return 0;
    CExtCompData * pInfo = (CExtCompData*)pData;
    W->Comp.Set( pInfo->IsComp, pInfo->Cmol );
    return sd_SetCompData;
    }
  return 0;
  }


