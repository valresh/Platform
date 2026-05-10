#include "stdafx.h"
#if 0
#define ALL_DATA 
#include "Data.h"
#include "CommProc.h"
//#include "Names.h"
//#include "XML.h"
#include "Err.h"
#include <stdlib.h>

static int kErr;

#undef SAVE_VAR
#define SAVE_VAR(V) { if ( !pSRFile->WF( &V, sizeof( V ))) kErr++; }
#undef SAVE_STR
#define SAVE_STR(V) { if ( !pSRFile->WS( V )) kErr++; }
#undef SAVE_DATA
#define SAVE_DATA(V,L) { if ( !pSRFile->WF( V, L )) kErr++; }


CFlow_Ext * CreateExt ( CFlow_Ext::eType Type );
int SizeExt ( CFlow_Ext::eType Type );

void SaveExt( CFlow * pFlow )
  {
  if ( pFlow->TYPE_EXT == CFlow_Ext::NoExt )
    return;
  ASS( pFlow ->pExt )
//  char Name[256];
//  int L = sprintf ( Name, "%s¦%s", pFlow->ObjName, pFlow->PipeName );
//  SAVE_VAR( L );
//  SAVE_DATA((void*)Name, L )
  pFlow->pExt->SaveData();
  }

void SkipExt( CFlow * pFlow )
  {
  if ( pFlow ->TYPE_EXT == CFlow_Ext::NoExt )
    return;
  int L = SizeExt ( pFlow->TYPE_EXT );
  if ( L > 0 )
    pSRFile->Skip( L );
  }

void RestExt( CFlow * pFlow )
  {
  if ( pFlow ->TYPE_EXT == CFlow_Ext::NoExt )
    {
    pFlow->pExt = NULL;
    return;
    }
  CFlow_Ext * pExt = CreateExt ( pFlow ->TYPE_EXT );
  pFlow->pExt = pExt;
//  pFlow->TYPE_EXT = CFlow_Ext::NoExt; // Для последующего подтверждения 
//  return;pExt
  pExt->RestData();
//  pFlow->pExt = pExt;
//  pFlow->TYPE_EXT = CFlow_Ext::NoExt; // Для последующего подтверждения 
  }

CFlow_Ext * CreateExt_Solid ( );

CFlow_Ext * CreateExt ( CFlow_Ext::eType Type )
  {
  switch ( Type )
    {
    case CFlow_Ext::Solid:
    return CreateExt_Solid( );
    default:
      ASS(0)
    return NULL;
    }
  return NULL;
  }

int SizeExt_Solid ( );

int SizeExt ( CFlow_Ext::eType Type )
  {
  switch ( Type )
    {
    case CFlow_Ext::Solid:
    return SizeExt_Solid( );
    default:
      ASS(0)
    return -1;
    }
  return -1;
  }
#endif
