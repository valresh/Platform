#include "Y_MainLogic.h"

int CompYBase ( const void * pP1, const void * pP2 );
int CompDR ( const void * pP1, const void * pP2 );

static CY_Base CY_Base_Key(false);
static CY_Base * pCY_Base_Key = &CY_Base_Key;

CY_DR Key_DR;
CY_DR * pKey_DR = &Key_DR;
CY_DR * CY_MainLogic::FindDR ( char * FCS )
  {
  lstrcpy ( Key_DR.FCS, FCS );
  CY_DR ** pRes = (CY_DR **)bsearch( &pKey_DR, DR, kDR, sizeof ( CY_DR *), CompDR );
  if ( pRes == NULL )
    return NULL;
  return *pRes;
  }

CY_Base * CY_MainLogic::FindYObj ( char * ObjName, int kObjFind )
  {
  if ( kObjFind <= 0 )
    kObjFind = kY_Obj;
  if ( kObjFind <= 0 )
    return NULL;
  strcpy_s( CY_Base_Key.Name, ObjName );
  CY_Base ** ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pY_Obj, kObjFind, sizeof ( CY_Base *), CompYBase );
	if ( ppBlk )
		return *ppBlk;
  return NULL;
  }

int FindYBaseSame( const void * pP1, const void * pP2 )
{
  CY_Base * pB1 = *(CY_Base**)pP1;
  CY_Base * pB2 = *(CY_Base**)pP2;
  int r = strcmp ( pB1->Name, pB2->Name );
  if( !r )
  {
    if( pB1->pDR && pB2->pDR )
    {
      if( pB1->pDR->domain < pB2->pDR->domain )
        return -1;
      if( pB1->pDR->domain > pB2->pDR->domain )
        return 1;

      if( pB1->pDR->NumbFCS < pB2->pDR->NumbFCS )
        return -1;
      if( pB1->pDR->NumbFCS > pB2->pDR->NumbFCS )
        return 1;
    }
  }
  return r;
}

int FindYBaseOther( const void * pP1, const void * pP2 )
{
  CY_Base * pB1 = *(CY_Base**)pP1;
  CY_Base * pB2 = *(CY_Base**)pP2;
  int r = strcmp ( pB1->Name, pB2->Name );
  if( !r && pB1->pDR && pB2->pDR)
  {
    if( pB1->pDR->domain < pB2->pDR->domain )
      return -1;
    if( pB1->pDR->domain > pB2->pDR->domain )
      return 1;

    if( pB1->pDR->NumbFCS == pB2->pDR->NumbFCS )
      return -1;
  }
  return r;
}

int FindYBaseSameTyped( const void * pP1, const void * pP2 )
{
  CY_Base * pB1 = *(CY_Base**)pP1;
  CY_Base * pB2 = *(CY_Base**)pP2;
  int r = strcmp ( pB1->Name, pB2->Name );
  if( !r && pB1->pDR && pB2->pDR)
  {
    if( pB1->Y_Type > 0x1ff && pB2->Y_Type > 0x1ff )
    {
      if( pB1->Y_Type < pB2->Y_Type )
        return -1;
      if( pB1->Y_Type > pB2->Y_Type )
        return 1;
    }
    if( pB1->pDR->domain < pB2->pDR->domain )
      return -1;
    if( pB1->pDR->domain > pB2->pDR->domain )
      return 1;

    if( pB1->pDR->NumbFCS < pB2->pDR->NumbFCS )
      return -1;
    if( pB1->pDR->NumbFCS > pB2->pDR->NumbFCS )
      return 1;
  }
  return r;
}

int FindYBaseOtherTyped( const void * pP1, const void * pP2 )
{
  CY_Base * pB1 = *(CY_Base**)pP1;
  CY_Base * pB2 = *(CY_Base**)pP2;
  int r = strcmp ( pB1->Name, pB2->Name );
  if( !r && pB1->pDR && pB2->pDR)
  {
    if( pB1->Y_Type > 0x1ff && pB2->Y_Type > 0x1ff )
    {
      if( pB1->Y_Type < pB2->Y_Type )
        return -1;
      if( pB1->Y_Type > pB2->Y_Type )
        return 1;
    }
    if( pB1->pDR->domain < pB2->pDR->domain )
      return -1;
    if( pB1->pDR->domain > pB2->pDR->domain )
      return 1;
    if( pB1->pDR->NumbFCS == pB2->pDR->NumbFCS )
      return -1;
  }
  return r;
}

CY_Base * CY_MainLogic::FindYObj( char * ObjName, bool bSameFCS, int NumbFcs, int domain )
{
  strcpy_s( CY_Base_Key.Name, ObjName );
  CY_DR tDR;
  tDR.NumbFCS = NumbFcs;
  tDR.domain = domain;
  CY_Base_Key.pDR = &tDR;
  
  CY_Base ** ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pY_Obj, CY_MainLogic::kY_Obj, sizeof( CY_Base *), bSameFCS ? FindYBaseSame : FindYBaseOther );

  CY_Base_Key.pDR = NULL;
  if ( ppBlk )
    return *ppBlk;
  return NULL;
}

CY_Base * CY_MainLogic::FindYObjTyped( char * ObjName, bool bSameFCS, int NumbFcs, int domain, int y_type )
{
  strcpy_s( CY_Base_Key.Name, ObjName );
  CY_DR tDR;
  tDR.NumbFCS = NumbFcs;
  tDR.domain = domain;
  CY_Base_Key.pDR = &tDR;
  CY_Base_Key.Y_Type = y_type;

  CY_Base ** ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pY_Obj, CY_MainLogic::kY_Obj, sizeof( CY_Base *), bSameFCS ? FindYBaseSameTyped : FindYBaseOtherTyped );

  CY_Base_Key.pDR = NULL;
  CY_Base_Key.Y_Type = 0;
  if ( ppBlk )
    return *ppBlk;
  return NULL;
}

CY_Base * CY_MainLogic::FindDObj( char * ObjName, bool bSameFCS, int NumbFcs, int domain )
{
  strcpy_s( CY_Base_Key.Name, ObjName );
  CY_DR tDR;
  tDR.NumbFCS = NumbFcs;
  tDR.domain = domain;
  CY_Base_Key.pDR = &tDR;

  CY_Base ** ppBlk = (CY_Base**)bsearch ( &pCY_Base_Key, CY_MainLogic::pD_Obj, CY_MainLogic::kD_Obj, sizeof( CY_Base *), bSameFCS ? FindYBaseSame : FindYBaseOther );

  CY_Base_Key.pDR = NULL;
  if ( ppBlk )
    return *ppBlk;
  return NULL;
}

CY_Base * CY_MainLogic::FindObjByNumb( LPCSTR szObjNumb, int NumbFcs, int domain )
{
  for( int i=0; i<kY_Obj; i++ )
  {
    CY_Base *pBlk = CY_MainLogic::pY_Obj[i];
    if( !pBlk->pDR || pBlk->pDR->domain!=domain || pBlk->pDR->NumbFCS!=NumbFcs )
      continue;
    if( !strcmp( pBlk->szNumber, szObjNumb) )
      return pBlk;
  }
  for( int i=0; i<kD_Obj; i++ )
  {
    CY_DI *pBlk = CY_MainLogic::pD_Obj[i];
    if( pBlk->NumbFCS!=NumbFcs || pBlk->domain!=domain )
      continue;
    if( !strcmp( pBlk->szNumber, szObjNumb) )
      return pBlk;
  }
  return NULL;
}

CY_DI * CY_MainLogic::FindDObj ( char * ObjName, int kObjFind )
  {
  strcpy_s( CY_Base_Key.Name, ObjName );
  if ( kObjFind <= 0 )
    kObjFind = CY_MainLogic::kD_Obj;
  if ( kObjFind <= 0 )
    return NULL;
  CY_DI ** ppDI = (CY_DI**)bsearch ( &pCY_Base_Key, CY_MainLogic::pD_Obj, kObjFind, sizeof ( CY_Base *), CompYBase );
	if ( ppDI )
    return *ppDI;
  return NULL;
  }

int CY_MainLogic::AddYObj( CY_Base * pObj )
{
  ASS( pObj->pDR );//должна быть привязка к контроллеру
  int nB = 0;
  int nE = 0;
  int nI = 0;// Точка вставки
  CY_Base * pB;
  if ( kY_Obj > 0 )
    {
    nE = kY_Obj - 1;
    while ( nE - nB > 1 )
      {
      int n = ( nB + nE ) >> 1;
      CY_Base * pB = pY_Obj[n];
      int Res = CompYBase( &pB, &pObj);//strcmp ( pB->Name, pObj->Name );
      if ( Res == 0 )
      {
        ASS( !"не должно встречаться совпадений" );
        return -1;
      }
      if ( Res > 0 )
        nE = n;
      else
        nB = n;
      }
    pB = pY_Obj[nB];
    int ResB = CompYBase( &pB, &pObj);//strcmp ( pB->Name, pObj->Name );
    if ( ResB == 0 )
    {
      ASS( !"не должно встречаться совпадений" );
      return -2;
    }
    if ( ResB > 0 )
      nI = nB;
    else
      {
      pB = pY_Obj[nE];
      int ResE = CompYBase( &pB, &pObj);//strcmp ( pB->Name, pObj->Name );
      ASS( ResB <= ResE );
      if ( ResE == 0 )
      {
        ASS( !"не должно встречаться совпадений" );
        return -3;
      }
      if ( ResE > 0 )
        nI = nE;
      else
        nI = nE + 1 ;
      }
    }
  ASS( kY_Obj < _countof(pY_Obj) )
  if ( nI < kY_Obj )
    {
    memmove ( &pY_Obj[nI+1], &pY_Obj[nI], ( kY_Obj - nI ) * sizeof ( CY_Base * ) );
    }
  pY_Obj[nI] = pObj;
  kY_Obj++;
  return 0;
}

bool AddDObj( CY_Base * pObj )
  {
  return true;
  }
