#include "Y_MainLogic.h"
#include <macros/StrHelps.h>
#include "../Y_Data/CalcVar.h"

void CY_MainLogic::TestForExistWB( LPSTR pWBName, int NumbFCS, int domain, bool bRefOtherFcs, LPCSTR pszExistComment /*= NULL*/ )
{
  if( strncmp( pWBName, "%WB", 3) )
    return;
  char realName[24];
  strcpy_s( realName, pWBName );
  const int numbWB = atoi( realName + 3 );
  if( 'S'==realName[9] )
  {
    NumbFCS = atoi( realName+12 );
    realName[9] = 0;
  }
  CY_DI *pDIO = FindDI( NumbFCS, domain, numbWB, id__WB );
  if( pDIO )
    return;
  char szWWname[64] = { 0 };
  strncpy_s( szWWname, realName, 7 );
  szWWname[2] = 'W';
  const int numbWW = atoi( szWWname + 3 );
  pDIO = FindDI( NumbFCS, domain, numbWW, id__WW );
  if( !pDIO )
  {
    OutputDebugString( "Не существует \"" );
    OutputDebugString( szWWname );
    OutputDebugString( "\", что бы создать \"" );
    OutputDebugString( pWBName );
    OutputDebugString( "\"\n" );
    return;
  }
  CY_DI * pDI = CreateDobj( realName, id__WB, NumbFCS, domain, numbWB );
  ASSD( pDI );
  if( !pDI )
    return;
  pDI->Numb = numbWB;
  strncpy_s( pDI->Name, realName, 16 );
  pDI->Name[16] = 0;
  strcpy_s( pDI->szNumber, pDI->Name );
  pDI->Y_Type = id__WB;
  strcpy_s( pDI->Model, "WB" );
  pDI->NumbFCS = NumbFCS;
  pDI->domain = domain;
  pDI->pointTypeZ = CY_DI::eptDigital;
  pDI->IOZ = pDIO->IOZ;
  pDI->modelTypeZ = CY_DI::eptDigital;
  ASS(CY_MainLogic::kD_Obj<_countof(pD_Obj));
  CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
  pDI->SetObjName( domain * CY_DR::s_dwFcsNumModulus + NumbFCS );
  if( pszExistComment && *pszExistComment )
  {
    _snprintf_s( pDI->Comment, _countof(pDI->Comment), _countof(pDI->Comment)-1, "%s, порожден искусственно, бит в %s", pszExistComment, pDIO->Name );
  }
  else
    sprintf_s( pDI->Comment, "Порожден искусственно, бит в %s", pDIO->Name );
  
  char szBuf[256];
  sprintf_s( szBuf, "%s,   %s, FCS %d\n", pDI->Name, pDI->Comment, NumbFCS );
  OutputDebugString( szBuf );
}

void CY_MainLogic::CreateLeftWBs()
{
  char NameObj[128];
  for( int n = 0; n < kConnection; n++ )
  {
    CObjConnection * pC = &Connection[n];
    const int pnt_out = pC->PntOut;
    if( pnt_out > 0 )
      {
      memcpy( NameObj, pC->Out, pnt_out );
      NameObj[pnt_out] = 0;
      }
    else
      strcpy_s( NameObj, pC->Out );
    bool bRefOtherFcs = pC->bRefOtherFcsOut;
    TestForExistWB( NameObj, pC->NumbFCS, pC->domain, bRefOtherFcs );

    const int pnt_in = pC->PntIn;
    if( pnt_in > 0 )
    {
      memcpy( NameObj, pC->In, pnt_in );
      NameObj[pnt_in] = 0;
    }
    else
      strcpy_s( NameObj, pC->In );
    bRefOtherFcs = pC->bRefOtherFcsIn;
    TestForExistWB( NameObj, pC->NumbFCS, pC->domain, bRefOtherFcs );
  }

  for( int i=0; i<CY_MainLogic::kY_Obj; i++ )
  {
    CY_Base *pObj = CY_MainLogic::pY_Obj[i];
    const char sep = '.';
    if( id_CALCU==pObj->Y_Type || id_CALCU_C==pObj->Y_Type )
    {
      CY_CALCU_BASE *pCalc = (CY_CALCU_BASE*)pObj;
      for( int i=0; i<pCalc->cVars; i++ )
      {
        KCalcVar &cv = ((KCalcVar*)pCalc->pVars)[i];
        char * P = strchr ( cv.NameValue, sep );
        TStringTerminator stP( P );
        TestForExistWB( cv.NameValue, pCalc->pDR->NumbFCS, pCalc->pDR->domain, false );
        KKK();
      }
    }
    else if( id_LC64==pObj->Y_Type || id_LC64_E==pObj->Y_Type )
    {
      CY_LC64_E_Base *plc64 = (CY_LC64_E_Base*)pObj;
      for ( DWORD n = 0; n < plc64->kMTRX; ++n )
      {
        for( DWORD r=0; r<_countof(plc64->RefName[n]); ++r )
        {
          char *pName = plc64->RefName[n][r];
          if( !pName[0] )
            continue;
          char * P = strchr( pName, sep );
          TStringTerminator stP( P );
          TestForExistWB( pName, plc64->pDR->NumbFCS, plc64->pDR->domain, false );
        }
      }
      KKK();
    }
    else if( id_ST16==pObj->Y_Type || id_ST16E==pObj->Y_Type )
    {
      CY_ST16 *pST16 = (CY_ST16*)pObj;
      for ( DWORD n = 0; n < _countof(pST16->Cond); ++n )
      {
        char *pName = pST16->Cond[n];
        if( !pName[0] )
          continue;
        char * P = strchr( pName, sep );
        TStringTerminator stP( P );
        TestForExistWB( pName, pST16->pDR->NumbFCS, pST16->pDR->domain, false, pST16->CondComm[n] );
      }
      for ( DWORD n = 0; n < _countof(pST16->Act); ++n )
      {
        char *pName = pST16->Act[n];
        if( !pName[0] )
          continue;
        char * P = strchr( pName, sep );
        TStringTerminator stP( P );
        TestForExistWB( pName, pST16->pDR->NumbFCS, pST16->pDR->domain, false, pST16->ActComm[n] );
      }
      KKK();
    }
  }
}

void CY_MainLogic::CreateRQs( int domain )
{
  char _Name[16];
  for( int i=0; i<200; ++i )
  {
    sprintf_s( _Name, "%%RQ%04d", i + 1 );
    CY_DI * pDI = CreateDobj( _Name, id__RQ, 0, domain, i+1 );
    pDI->Numb = i+1;
    strcpy_s( pDI->Name, _Name );
    pDI->Y_Type = id__RQ;
    strcpy_s( pDI->Model, "RQ" );
    pDI->NumbFCS = m_NumbFCS;
    pDI->domain = domain;
    pDI->pointTypeZ = CY_DI::eptDigital;
    pDI->IOZ = 'O';
    ASS(CY_MainLogic::kD_Obj<_countof(pD_Obj));
    CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
    strcpy_s( pDI->szAkaObjName, _Name );
  }
}
