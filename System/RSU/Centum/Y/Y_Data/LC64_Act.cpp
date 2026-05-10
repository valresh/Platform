#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "Y_Draw.h"
#include <ConnectY.h>
#include <macros/StrHelps.h>
#include "FlagsSupport.h"

struct CIAct 
{ 
  LPSTR pParam;
  tA_LC64_IParm pAI;
  int Iparam;
  bool bL;
};

static CIAct IAct[] = 
{
  {"ON", &CAct_LC64::SetInt, 1, false},
  {"L", &CAct_LC64::SetInt, 1, false},
  {"H", &CAct_LC64::SetInt, 1, false},
  {"P", &CAct_LC64::SetInt, 1, false},
  {"NON", &CAct_LC64::SetInt, 1, false},
};

static int CompIAct( const void * p1, const void * p2 )
{
  CIAct * pC1 = (CIAct*)p1;
  CIAct * pC2 = (CIAct*)p2;
  return strcmp( pC1->pParam, pC2->pParam );
}

void SortLC64Act()
{
  qsort( IAct, _countof(IAct), sizeof( CIAct ), CompIAct );
}

void CAct_LC64::SetInt( int thr )
{
  Active = true;
  ASS( VarType == 'I' );
  int PV = *(int*)pVar;
  *(int*)pVar = thr;
}

void CAct_LC64::SetByte( BYTE thr )
{
  Active = true;
  if( pObj )
  {
    if( VarType == 'B' )
    {
      pObj->CallSA( Name );
      return;
    }
  }
  ASS( VarType == 'W' );
  BYTE M = *(BYTE*)pVar;
  *(BYTE*)pVar = thr;
}

void CAct_LC64::SetDouble( double thr )
{
  Active = true;
  ASS( VarType == 'D' );
  double M = *(double*)pVar;
  *(double*)pVar = thr;
}

void CAct_LC64::Set_P( bool bChange, bool bState )
{
  switch( VarType  )
  {
  case 'I':
    {
      int PV = *(int*)pVar;
      if( bState )
        *(int*)pVar = 1;
      else if( bChange )
        *(int*)pVar = 0;
    }
    break;
  default:
    ASSD(0);
  }
}

void CAct_LC64::SetIntP_CSV( int thr, bool Y )
{
  Active = Y;
  ASS( VarType == 'I' );
  int &MV = *(int*)pVar;

  if( Y )
    MV = thr;
  else
  {
    switch( thr )
    {
    case 0:
      MV = 2;
      break;
    case 2:
      MV = 0;
      break;
    }
  }
}

void CAct_LC64::SET_0()
{
  Active = true;
  switch( VarType  )
  {
  case 'W':
    {
      BYTE PV = *(BYTE*)pVar;
      *(BYTE*)pVar = 0;
    }
    break;
  case 'I':
    {
      int PV = *(int*)pVar;
      *(int*)pVar = 0;
    }
    break;
  case 'D':
    {
      double PV = *(double*)pVar;
      *(double*)pVar = 0;
    }
    break;
  default:
    ASS( 0 );
  }
}

void CAct_LC64::SetIntALRM( int thr, bool Y )
{
  Active = Y;
  bool b = false;
  if( pObj4ExternAlarm )
   b = pObj4ExternAlarm->SetResetAlrmFromExtern( thr, Y );

  if( !b )
  {
    ASS( VarType == 'I' );
    int &ALRM = *(int*)pVar;
    if( Y )
      ALRM = thr;
    else
    {
      if( ALRM==thr )
        ALRM = 0;
    }
  }
}

void CAct_LC64::SetMaskInt( int thr, bool Y )
{
  Active = Y;
  ASS( VarType == 'I' );
  int &PV = *(int*)pVar;
  if( Y )
    PV |= thr;
  else
    PV &= ~thr;
}

void SelectFunction( CAct_LC64 * pVA )
{
  switch( pVA->VarType )
  {
  case 'I':
    pVA->pAI = &CAct_LC64::SetInt;
    ASS( !pVA->pA );
    ASS( !pVA->pAB );
    ASS( !pVA->pAD );
    break;
  case 'B':
    KKK();
  case 'W':
    pVA->pAB = &CAct_LC64::SetByte;
    ASS( !pVA->pA );
    ASS( !pVA->pAI );
    ASS( !pVA->pAD );
    break;
  case 'D':
    pVA->pAD = &CAct_LC64::SetDouble;
    ASS( !pVA->pA );
    ASS( !pVA->pAI );
    ASS( !pVA->pAB );
    break;
  case ' ':
    break;
  default:
    ASSD( 0 );
  }
}

static LPCSTR PV_CAL = "PV=CAL";

void CorrectLC64ActFunc( char * Act, CAct_LC64 * pVA, CY_Base *pObj )
{
  if( !pObj )
    return;
  if( !strcmp(Act,PV_CAL) )
    return;
  LPSTR pParam = strchr( Act, '.' );
  LPSTR pPointTest = strrchr( Act, '.');
  ASS( pParam );
  ASS( pParam==pPointTest );
  TStringTerminator pointRestore( *pParam++ );

  if( !_strcmpi( Act, "CSV") && 'P'==*pParam )
  {
    switch( pObj->Y_Type )
    {
    case id_SI_1:
    case id_SO_1:
    case id_SO_2:
    case id_SI_2:
    case id_SIO_11:
    case id_SIO_12:
    case id_SIO_21:
    case id_SIO_22:
    case id_SIO_22P:
    case id_SI_2E:
    case id_SIO_21E:
    case id_SIO_22E:
    case id_SIO_22PE:
    case id_MC_2:
    case id_MC_2E:
    case id_MC_3:
    case id_MC_3E:
      {
        ASS( &CAct_LC64::SetInt==pVA->pAI );
        pVA->pAI = NULL;
        pVA->pAIstate = &CAct_LC64::SetIntP_CSV;
      }
      break;
    }
  }
}

void __declspec(dllexport) GetLC64ActFunc( LPSTR Act, CAct_LC64 * pVA, CY_Base *pObj )
{
  if( !strcmp(Act,PV_CAL) )
  {
    SelectFunction( pVA );
    pVA->Iparam = 1;
    return;
  }
  LPSTR pParam = strchr( Act, '.' );
  LPSTR pPointTest = strrchr( Act, '.');
  ASS( pParam );
  ASS( pParam==pPointTest );

  TStringTerminator pointRestore( *pParam++ );

  if( !_strcmpi( Act, "MODE") )
  {
    SelectFunction( pVA );
    pVA->Iparam = GetModeNumber(pParam, pObj );
  }
  else if( !_strcmpi( Act, "ALRM") )
  {
    pVA->Iparam = GetAlrmNumber(pParam);
    if( pVA->Iparam>=0 )
    {
      ASSD( 'I'==pVA->VarType );
      pVA->pAIstate = &CAct_LC64::SetIntALRM;
      ASS( !pVA->pAI );
      ASS( !pVA->pA );
      ASS( !pVA->pAB );
      ASS( !pVA->pAD );
      pVA->pObj4ExternAlarm = pObj;
    }
  }
  else if( !_strcmpi( Act, "AF") )
  {
    ASSD( 'I'==pVA->VarType );
    ASS( !pVA->pAI );
    ASS( !pVA->pA );
    ASS( !pVA->pAB );
    ASS( !pVA->pAD );
    pVA->pAIstate = &CAct_LC64::SetMaskInt;
    pVA->Iparam = GetAlrmYokoCode(pParam);
  }
  else if( !_strcmpi( Act, "SA") )
  {
    ASS( id_ST16==pObj->Y_Type || id_ST16E==pObj->Y_Type );
    SelectFunction( pVA );
    pVA->Iparam = 1;
    pVA->VarType = 0;
    pVA->pVar = NULL;
    char sz[16] = {};
    sprintf_s( sz, "%s.%s", Act, pParam );
    bool b = pObj->GetVar( sz, &pVA->pVar, NULL, &pVA->VarType, NULL );
    ASS( b );
    if( !b )
      return;
    pVA->pObj = pObj;
    ASS( pVA->VarType=='W' );
  }
  else
  {
    BOOL bDigitsOnly = TRUE;
    BOOL bShifted = FALSE;
    switch( *pParam )
    {
    case 'P':
    case 'p':
      bShifted = TRUE;
      ++pParam;
      break;
    }
    const size_t L = strlen( pParam );
    if( !L )
      bDigitsOnly = FALSE;
    for( size_t i=0; i<L; i++)
    {
      if( !isdigit(pParam[i]) )
      {
        bDigitsOnly = FALSE;
        break;
      }
    }
    if( bShifted )
    {
      if( !bDigitsOnly )
        --pParam;
      else
      {
        switch( *pParam )
        {
        case '0':
        case '1':
        case '2':
          break;
        default:
          ASSD(0);
          break;
        }
      }
    }
    if( bDigitsOnly )
    {
      SelectFunction( pVA );
      pVA->Iparam = atoi( pParam );
    }
    else
    {
      CIAct Key;
      Key.pParam = pParam;
      CIAct* pIRes = (CIAct*)bsearch( &Key, IAct, _countof(IAct), sizeof( CIAct ), CompIAct );
      if( pIRes )
      {
        SelectFunction( pVA );
        pVA->Iparam = pIRes->Iparam;
        pVA->L_Type = pIRes->bL;
      }
      else
      {
        int I = GetEnumValue( Act, pParam, pObj );
        if( I>=0 )
        {
          SelectFunction( pVA );
          pVA->Iparam = I;
        }
      }
    }

  }

}
