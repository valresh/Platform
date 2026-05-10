#include "Y_MainLogic.h"
#include <rsuErr.h>
//#include "Y_Draw.h"
#include <macros/StrHelps.h>
#include <ConnectY.h>

int CompYBase ( const void * pP1, const void * pP2 );
int CompDiscret ( const void * pP1, const void * pP2 );
int CompDR ( const void * pP1, const void * pP2 );

static CY_Base CY_Base_Key(false);
static CY_Base * pCY_Base_Key = &CY_Base_Key;

static CY_DI CY_DI_Key(false);
static CY_DI *pCY_DI_Key = &CY_DI_Key;

int CompDiscretNumbType( const void * pP1, const void * pP2 )
{
  CY_DI * pB1 = *(CY_DI**)pP1;
  CY_DI * pB2 = *(CY_DI**)pP2;
  if( pB1->Y_Type==pB2->Y_Type )
  {
    if( pB1->Numb == pB2->Numb )
    {
      if( pB1->domain < pB2->domain )
        return -1;
      if( pB1->domain > pB2->domain )
        return 1;

      if( pB1->NumbFCS < pB2->NumbFCS )
        return -1;
      if( pB1->NumbFCS > pB2->NumbFCS )
        return 1;
      return 0;
    }
    if( pB1->Numb < pB2->Numb )
      return -1;
    return 1;
  }
  if( pB1->Y_Type < pB2->Y_Type )
    return -1;
  return 1;
}

CY_Base * FindYObjN( char * ObjName, int NumbFcs, int domain )
{
  return CY_MainLogic::FindYObjN( ObjName, NumbFcs, domain );
}

CY_Base * CY_MainLogic::FindYObjN( char * ObjName, int NumbFcs, int domain )
{
  if( domain < 0 )
  {
    if( !s_bFcsMultiDomain )
      return NULL;
    int skipDomain = -domain;
    for( int i=0; i<CY_MainLogic::kY_Obj; i++ )
    {
      if( CY_MainLogic::pY_Obj[i]->pDR->domain==skipDomain )
        continue;
      if( !strcmp( CY_MainLogic::pY_Obj[i]->Name, ObjName ) )
      {
        //if( !NumbFcs || NumbFcs==CY_MainLogic::pY_Obj[i]->pDR->NumbFCS )
          return CY_MainLogic::pY_Obj[i];
      }
    }
  }

  if( NumbFcs < 0 )
  {
    return CY_MainLogic::FindYObj( ObjName, false, -NumbFcs, domain );
  }
  static CY_DR tDR;
  tDR.NumbFCS = 0;
  tDR.domain = domain;
  CY_Base_Key.pDR = NULL;

  LPCSTR pSep = strchr(ObjName, ':');
  if( pSep )
  {
    tDR.NumbFCS = atoi(ObjName);
    ASS( tDR.NumbFCS );
    CY_Base_Key.pDR = &tDR;
  }

  if( NumbFcs>0 )
  {
    if( CY_Base_Key.pDR  )
      ASS( CY_Base_Key.pDR->NumbFCS==NumbFcs );
    CY_Base_Key.pDR = &tDR;
    tDR.NumbFCS = NumbFcs;
  }
  
  if( pSep)
    strcpy_s( CY_Base_Key.Name, pSep+1 );
  else
    strcpy_s( CY_Base_Key.Name, ObjName );
  CY_Base_Key.Y_Type = 0;
  CY_Base ** ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pY_Obj, CY_MainLogic::kY_Obj, sizeof ( CY_Base *), CompYBase );
	if ( ppBlk )
		return *ppBlk;

  strcpy_s( CY_DI_Key.Name, CY_Base_Key.Name );
  CY_DI_Key.NumbFCS = tDR.NumbFCS;
  CY_DI_Key.domain = tDR.domain;
  CY_DI ** ppDI = (CY_DI**)bsearch ( &pCY_DI_Key, CY_MainLogic::pD_Obj, CY_MainLogic::kD_Obj, sizeof ( CY_DI *), CompDiscret );
	if ( ppDI )
    return *ppDI;
  //
  for( int i=0; i<CY_MainLogic::kY_Obj; i++ )
  {
    if( CY_MainLogic::pY_Obj[i]->pDR->domain!=domain )
      continue;
    if( !strcmp( CY_MainLogic::pY_Obj[i]->Name, ObjName ) )
    {
      if( !NumbFcs || NumbFcs==CY_MainLogic::pY_Obj[i]->pDR->NumbFCS )
        return CY_MainLogic::pY_Obj[i];
    }
  }
  for( int i=0; i<CY_MainLogic::kD_Obj; i++ )
  {
    if( CY_MainLogic::pD_Obj[i]->domain!=domain )
      continue;
    if( !strcmp( CY_MainLogic::pD_Obj[i]->Name, ObjName ) )
    {
      if( !NumbFcs || NumbFcs==CY_MainLogic::pD_Obj[i]->NumbFCS )
        return CY_MainLogic::pD_Obj[i];
    }
  }

  CY_Base *pObjN = CY_MainLogic::GetObj( ObjName, NumbFcs, domain );
  if( pObjN )
    return pObjN;

  return NULL;
}

CY_Base * FindYObj( char * ObjName, int domain )
{
  LPSTR pSep = strchr(ObjName, ':');
  int NumbFCS = 0;
  if( pSep )
  {
    NumbFCS = atoi(ObjName);
    ObjName = pSep + 1;
  }
  return FindYObjN( ObjName, NumbFCS, domain );
}

CY_Base * FindYObjTyped( char * ObjName, int domain, int y_type /*= 0*/ )
{
  return CY_MainLogic::FindYObjTyped( ObjName, domain, y_type );
}

CY_Base * CY_MainLogic::FindYObjTyped( char * ObjName, int domain, int y_type )
{
  strcpy_s ( CY_Base_Key.Name, ObjName );
  CY_Base_Key.Y_Type = y_type;
  CY_Base_Key.pDR = NULL;
  CY_Base ** ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pY_Obj, CY_MainLogic::kY_Obj, sizeof ( CY_Base *), CompYBase );
  CY_Base_Key.Y_Type = 0;
  if( y_type < 0x200 )
  {
    if ( ppBlk )
      return *ppBlk;
  }
  if( ppBlk && (*ppBlk)->Y_Type==y_type )
    return *ppBlk;

  CY_Base_Key.Y_Type = y_type;
  CY_Base_Key.pDR = NULL;
#ifdef _DEBUG
	CY_Base_Key.pClass_P = (BYTE*)CY_MainLogic::pD_Obj;
#endif
  ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pD_Obj, CY_MainLogic::kD_Obj, sizeof ( CY_DI *), CompDiscret );
  CY_Base_Key.Y_Type = 0;
  if( y_type < 0x200 )
  {
    if ( ppBlk )
      return *ppBlk;
  }
  if( ppBlk && (*ppBlk)->Y_Type==y_type )
    return *ppBlk;
  //
  for( int i=0; i<CY_MainLogic::kY_Obj; i++ )
  {
    if( !strcmp ( CY_MainLogic::pY_Obj[i]->Name, ObjName ) )
    {
      if( y_type>0x1ff )
      {
        if( CY_MainLogic::pY_Obj[i]->Y_Type!=y_type )
          continue;
      }
      return CY_MainLogic::pY_Obj[i];
    }
  }

	for( int i=0; i<CY_MainLogic::kD_Obj; i++ )
  {
    if( !strcmp ( CY_MainLogic::pD_Obj[i]->Name, ObjName ) )
    {
      if( y_type>0x1ff )
      {
        if( CY_MainLogic::pD_Obj[i]->Y_Type!=y_type )
          continue;
      }
      return CY_MainLogic::pD_Obj[i];
    }
  }
  return NULL;
}

CY_DI * FindDObj( char * ObjName, int NumbFcs, int domain )
{
  return CY_MainLogic::FindDObjOld( ObjName, NumbFcs, domain );
}

CY_DI * CY_MainLogic::FindDObjOld( char * ObjName, int NumbFcs, int domain )
{
  CY_DI_Key.NumbFCS = 0;
  CY_DI_Key.domain = domain;

  LPCSTR pSep = strchr(ObjName, ':');
  if( pSep )
  {
    CY_DI_Key.NumbFCS = atoi(ObjName);
    ASS( CY_DI_Key.NumbFCS );
  }

  if( NumbFcs>0 )
    CY_DI_Key.NumbFCS = NumbFcs;

  if( pSep)
    strcpy_s( CY_DI_Key.Name, pSep+1 );
  else
    strcpy_s( CY_DI_Key.Name, ObjName );

  CY_DI ** ppDI = (CY_DI**)bsearch ( &pCY_DI_Key, CY_MainLogic::pD_Obj, CY_MainLogic::kD_Obj, sizeof ( CY_DI *), CompDiscret );
	if ( ppDI )
    return *ppDI;

  for( int i=0; i<CY_MainLogic::kD_Obj; i++ )
  {
    if( CY_DI_Key.domain!=CY_MainLogic::pD_Obj[i]->domain )
      continue;
    if( !strcmp( CY_MainLogic::pD_Obj[i]->Name, ObjName ) )
    {
      if( !CY_DI_Key.NumbFCS || CY_DI_Key.NumbFCS==CY_MainLogic::pD_Obj[i]->NumbFCS )
        return CY_MainLogic::pD_Obj[i];
    }
  }
  CY_DI *pt = CY_MainLogic::GetDObj( ObjName, NumbFcs, domain, true );
  if( pt )
    return pt;
  return NULL;
}

//bool CY_MainLogic::FindAndShowBlk ( HWND hWndMain, char * FCS, char * SelectedObject )
//{
//  for ( int n = 0; n < kDR; n++ )
//  {
//    if ( strcmp ( DR[n]->FCS, FCS ) == 0 )
//    {
//      return ::CreateWnd ( DR[n],	pY_Obj,  kY_Obj, hWndMain, SelectedObject );
//    }
//  }
//  return false;
//}
//
//bool FindAndShowBlk ( HWND hWndMain, char * FCS, char * SelectedObject )
//	{
//  return CY_MainLogic::FindAndShowBlk ( hWndMain, FCS, SelectedObject );
//	}
//
//void CY_MainLogic::ShowCalcuCDebugWindow( HWND hWndMain, LPSTR pszName )
//{
//  CY_Base *pCB = NULL;
//  LPSTR pPnt = strchr(pszName, '.');
//  {
//    TStringTerminator st( pPnt );
//    pCB = FindYObj( pszName );
//    if( !pCB )
//      return;
//  }
//
//  if( id_CALCU!=pCB->Y_Type && id_CALCU_C!=pCB->Y_Type )
//    return;
//
//  CGrafParm * pGP = NULL;
//  pCB->CreatePSMGraf( &pGP );
//}
//
//void ShowCalcuCDebugWindow( HWND hWndMain, LPSTR pszName )
//{
//  CY_MainLogic::ShowCalcuCDebugWindow( hWndMain, pszName );
//}

CY_DI * CY_MainLogic::FindDI( int NumbFCS, int domain, int NumbSW, int Type )
{
  if( CY_MainLogic::m_ppDigNumbTypeSorted )
  {
    CY_DI_Key.NumbFCS = NumbFCS;
    CY_DI_Key.domain = domain;
    CY_DI_Key.Numb = NumbSW;
    CY_DI_Key.Y_Type = Type;
    CY_DI ** ppDI = (CY_DI**)bsearch ( &pCY_DI_Key, CY_MainLogic::m_ppDigNumbTypeSorted, CY_MainLogic::kD_Obj, sizeof( CY_DI *), CompDiscretNumbType );
    if ( ppDI )
      return *ppDI;
  }
  const int Numb = NumbSW;
  const int invNumbFCS = -NumbFCS;

  for ( int n = 0; n < kD_Obj; n++ )
    {
    if ( pD_Obj[n]->Y_Type != Type )
      continue;
    if ( pD_Obj[n]->domain != domain )
      continue;
    if( NumbFCS )
    {
      if( NumbFCS > 0 && pD_Obj[n]->NumbFCS != NumbFCS )
        continue;
      if( NumbFCS < 0 && pD_Obj[n]->NumbFCS == invNumbFCS )
        continue;
    }
    CY_DI * pDI = (CY_DI*)pD_Obj[n]; 
    if ( pDI->Numb == Numb )
      return pDI;
    }
  return NULL;
}

void GetDR( int * kDR, struct CY_DR *** DR )
{
  return CY_MainLogic::GetDR( kDR, DR );
}

void CY_MainLogic::GetDR( int * kDR, struct CY_DR *** DR )
{
  *kDR = CY_MainLogic::kDR;
  *DR = CY_MainLogic::DR;
}
