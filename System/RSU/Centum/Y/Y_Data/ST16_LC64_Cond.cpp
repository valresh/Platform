#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "Y_Draw.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>
#include "FlagsSupport.h"
#include <ST16_LC64_AC.h>
#include <Alarms.h>

void CCond_ST16_LC64::LE()
{
  ASS( VarType == 'I' )
  int PV = *(int*)pVar;
  if( pObj )
    Yes = PV ? true : false;
  else
  {
    if ( PV <= 0 )
      Yes = true;
    else
      Yes = false;
  }
}

void CCond_ST16_LC64::LT()
{
  ASS( VarType == 'I' )
  int PV = *(int*)pVar;
  if( pObj )
    Yes = PV ? true : false;
  else
  {
    if ( PV < 0 )
      Yes = true;
    else
      Yes = false;
  }
}

void CCond_ST16_LC64::EQ()
{
  ASS( VarType == 'I' )
  int PV = *(int*)pVar;
  if( pObj )
    Yes = PV ? true : false;
  else
  {
    if ( PV == 0 )
      Yes = true;
    else
      Yes = false;
  }
}

void CCond_ST16_LC64::GT()
{
  ASS( VarType == 'I' )
  int PV = *(int*)pVar;
  if( pObj )
    Yes = PV ? true : false;
  else
  {
    if ( PV > 0 )
      Yes = true;
    else
      Yes = false;
  }
}

void CCond_ST16_LC64::GE()
{
  ASS( VarType == 'I' )
  int PV = *(int*)pVar;
  if( pObj )
    Yes = PV ? true : false;
  else
  {
    if ( PV >= 0 )
      Yes = true;
    else
      Yes = false;
  }
}

void CCond_ST16_LC64::AND()
{
  ASS( VarType == 'I' )
    int PV = *(int*)pVar;
  if( pObj )
    Yes = PV ? true : false;
  else
  {
    if ( PV >= 0 )
      Yes = true;
    else
      Yes = false;
  }
}

struct CStdCond 
{ 
  char * Cond;
  tC_ST16_LC64 pC;
};

static CStdCond CompareWith0[] = 
{
  "LE", &CCond_ST16_LC64::LE,
  "LT", &CCond_ST16_LC64::LT,
  "GE", &CCond_ST16_LC64::GE,
  "GT", &CCond_ST16_LC64::GT,
  "EQ", &CCond_ST16_LC64::EQ,
  "AND", &CCond_ST16_LC64::AND,
};

struct CBCond
{ 
  LPSTR pParam;
  BYTE Bparam;
};

static CBCond BCond[] = 
{
  {"PAUS", 1},//???CTS_W::_BSTS::PAUS
};

static int CompStdCond ( const void * p1, const void * p2 )
{
  CStdCond * pC1 = (CStdCond*)p1;
  CStdCond * pC2 = (CStdCond*)p2;
  return strcmp ( pC1->Cond, pC2->Cond );
}

static int CompBCond( const void * p1, const void * p2 )
{
  CBCond * pC1 = (CBCond*)p1;
  CBCond * pC2 = (CBCond*)p2;
  return strcmp ( pC1->pParam, pC2->pParam );
}

void SortST16_LC64Cond()
{
  qsort( CompareWith0, _countof(CompareWith0), sizeof( CStdCond ), CompStdCond );
  qsort( BCond, _countof(BCond), sizeof( CBCond ), CompBCond );
}

void CCond_ST16_LC64::CompareInt( int thr )
{
  int PV = 0;
  if( VarType == 'I' )
    PV = *(int*)pVar;
  else if( VarType == 'D' )
    PV = (int)*(double*)pVar;
  else if( VarType == 'W' )
    PV = *(BYTE*)pVar;
  else if( VarType == 'S' )
  {
    LPCSTR psz = (LPCSTR)pVar;
    PV = atoi(psz);
  }
  else
    ASS( 0 );
  if ( PV == thr )
    Yes = true;
  else
    Yes = false;
}

void CCond_ST16_LC64::CompareIntAnd( int thr )
{
  int PV = 0;
  if( VarType == 'I' )
    PV = *(int*)pVar;
  else
    ASS( 0 );
  if( PV & thr )
    Yes = true;
  else
    Yes = false;
}

void CCond_ST16_LC64::CompareIntCalcuActOn( int thr )
{
  int PV = 0;
  if( VarType == 'I' )
    PV = *(int*)pVar;
  else if( VarType == 'D' )
    PV = (int)*(double*)pVar;
  else if( VarType == 'W' )
    PV = *(BYTE*)pVar;
  else if( VarType == 'S' )
  {
    LPCSTR psz = (LPCSTR)pVar;
    PV = atoi(psz);
  }
  else
    ASS( 0 );
  if ( PV >= thr )
    Yes = PV;
  else
    Yes = 0;
}

void CCond_ST16_LC64::CompareInt64And( __int64 thr )
{
  __int64 PV = 0;
  if( VarType == 'L' )
    PV = *(__int64*)pVar;
  else
    ASS( 0 );
  if( PV & thr )
    Yes = true;
  else if( !PV && !thr )
    Yes = true;
  else
    Yes = false;
}

void CCond_ST16_LC64::CompareByte( BYTE thr )
{
  ASS( VarType == 'W' );
  BYTE w = *(BYTE*)pVar;
  if ( w == thr )
    Yes = true;
  else
    Yes = false;
}

void CCond_ST16_LC64::CompareArrayByte4ST16( BYTE thr )
{
  ASS( VarType == 'W' );
  BYTE *w = (BYTE*)pVar;

  BYTE eq = 0;
  for( USHORT i=0, c(__max(1, saCount )); i<c; ++i )
  {
    if ( w[i] == thr )
      eq = true;
  }
  Yes = eq;
}

void CCond_ST16_LC64::CompareBool( bool thr )
{
  ASS( VarType == 'B' );
  bool w = *(bool*)pVar;
  if ( w == thr )
    Yes = true;
  else
    Yes = false;
}

void SelectFunction( CCond_ST16_LC64 * pVC )
{
  switch( pVC->VarType )
  {
  case 'I':
    pVC->pCIP = &CCond_ST16_LC64::CompareInt;
    break;
  case 'W':
    pVC->pCBP = &CCond_ST16_LC64::CompareByte;
    break;
  case 'B':
    pVC->pCbool = &CCond_ST16_LC64::CompareBool;
    break;
  default:
    ASS( 0 );
  }
}

static bool s_Const_No = false;

void __declspec(dllexport) GetST16_LC64CondFunc( char * Cond, CCond_ST16_LC64 * pVC, CY_Base *pObj )
{
  char sep = '.';
  LPSTR pParam = strchr( Cond, sep );
  LPSTR pPointTest = strrchr( Cond, sep );
  if( !pParam )
  {
    sep = '=';
    pParam = strchr( Cond, sep );
    pPointTest = strrchr( Cond, sep );

    if( pParam )
    {
      LPCSTR pState = pParam+1;
      if( !strcmp( pState,"BAD") )
      {
        pVC->Yes = s_Const_No;
        pVC->pVar = &s_Const_No;
        pVC->VarType = 'B';
      }
      else if( !strncmp( pState, "CLP", 3) )
      {
        ASS( !strncmp( Cond, "MV", 2) );
        CSignal *out = pObj->GetOUT("OUT");
        ASS( out );
        if( !out )
          return;
        pVC->pVar = &out->Status;
        pVC->VarType = 'I';
        pVC->pCIP = &CCond_ST16_LC64::CompareIntAnd;
        switch( pState[3] )
        {
        case '-':
          pVC->Iparam = CSignal::CLPm;
          break;
        case '+':
          pVC->Iparam = CSignal::CLPp;
          break;
        case '\0':
          pVC->Iparam = CSignal::CLP;
          break;
        default:
          ASS(0);
        }
      }
      else if( !strcmp( pState,"CAL") )
      {
        void * pVar = NULL;
        void * pTuneVar = NULL; 
        char Type = ' ';
        CY_Base::eVarType Work = CY_Base::evtP;
        if( pObj->GetVar( Cond, &pVar, &pTuneVar, &Type, &Work ) )
        {
          pVC->pVar = pVar;
          pVC->VarType = Type;
          SelectFunction( pVC );
          if( !strcmp( Cond, "PV=CAL") )
            pVC->Iparam = 1;
        }
        else
        {
          ASSD(0);
        }
      }
      else
      {
        ASS(0);
      }
      return;
    }
  }
  ASS( pParam );
  ASS( pParam==pPointTest );

  if( !pVC->pVar )
  {
    char * Q = strchr ( Cond, sep );
    ASS( Q );
    if( !Q )
      return;
    void * pVar = NULL;
    void * pTuneVar = NULL; 
    char Type = ' ';
    CY_Base::eVarType Work = CY_Base::evtP;
    *Q = 0;
    if ( pObj )
    {
      bool b = pObj->GetVar( Cond, &pVar, &pTuneVar, &Type, &Work );
      ASSD(b);
    }
    *Q = sep;
    pVC->pVar = pVar;
    pVC->VarType = Type;
  }

  TStringTerminator pointRestore( *pParam++ );

  if( !_strcmpi( Cond, "MODE") )
  {
    pVC->Iparam = GetModeNumber(pParam, pObj);
    switch( pVC->Iparam )
    {
    case 128:
      pVC->VarType = 'B';
      pVC->pVar = (BYTE*)pVC->pVar + offsetof( SMode, SMode::IMAN );
      break;
    case 256:
      pVC->VarType = 'B';
      pVC->pVar = (BYTE*)pVC->pVar + offsetof( SMode, SMode::TRK );
      break;
    }
    SelectFunction( pVC );
  }
  else if( !_strcmpi( Cond, "AFLS") )
  {
    SelectFunction( pVC );
    pVC->Iparam = GetModeNumber(pParam, pObj);
  }
  else if( !_strcmpi( Cond, "ALRM") )
  {
    if( !pParam || !pObj )
    {
      SelectFunction( pVC );
      pVC->Iparam = GetAlrmNumber(pParam);
    }
    else
    {
      void * pVar = NULL;
      void * pTuneVar = NULL; 
      char Type = ' ';
      CY_Base::eVarType Work = CY_Base::evtP;
      if( pObj->GetVar( "ALRMmask", &pVar, &pTuneVar, &Type, &Work ) )
      {
        AlarmFlags af = (AlarmFlags)GetAlrmMask( pParam );
        if( A_NR==af )
          af = (AlarmFlags)0LL;
        pVC->I64param = af;
        pVC->pVar = pVar;
        pVC->pCI64P = &CCond_ST16_LC64::CompareInt64And;
        pVC->VarType = Type;
      }
      else
      {
        ASSD(0);
      }
    }
  }
  else if( !_strcmpi( Cond, "XALRM") )
  {
    SelectFunction( pVC );
    pVC->Iparam = GetAlrmNumber(pParam);
  }
  else if( !_strcmpi( Cond, "AOFS") )
  {
    SelectFunction( pVC );
    pVC->Iparam = GetAlrmNumber(pParam);
  }
  else if( !_strcmpi( Cond, "AF") )
  {
    SelectFunction( pVC );
    pVC->Iparam = GetAlrmNumber(pParam);
  }
  else if( !_strcmpi( Cond, "SA") )
  {
    ASS( id_ST16==pObj->Y_Type || id_ST16E==pObj->Y_Type );
    pVC->pCBP = &CCond_ST16_LC64::CompareArrayByte4ST16;
    pVC->Iparam = 1;
    pVC->VarType = 0;
    pVC->pVar = NULL;
    char sz[16] = {};
    sprintf_s( sz, "%s.%s", Cond, pParam );
    bool b = pObj->GetVar( sz, &pVC->pVar, NULL, &pVC->VarType, NULL, NULL, &pVC->saCount );
    if( !b )
    {
      OutputDebugString("!!!!!  Условие не готово \""); OutputDebugString( sz ); OutputDebugString("\"\n");
      return;
    }
    pVC->pObj = pObj;
    ASS( pVC->VarType=='W' );
  }
  else
  {
    int I = GetEnumValue( Cond, pParam, pObj );
    if( I>=0 )
    {
      SelectFunction( pVC );
      pVC->Iparam = I;
      return;
    }
    BOOL bDigitsOnly = TRUE;
    const size_t L = strlen( pParam );
    for( size_t i=0; i<L; i++)
    {
      if( !isdigit(pParam[i]) )
      {
        bDigitsOnly = FALSE;
        break;
      }
    }
    if( bDigitsOnly )
    {
      pVC->pCIP = &CCond_ST16_LC64::CompareInt;
      pVC->Iparam = atoi( pParam );
    }
    else if( !_strcmpi(pParam,"ON") )
    {
      pVC->pCIP = &CCond_ST16_LC64::CompareInt;
      pVC->Iparam = 1;
    }
    else if( !_strcmpi(pParam,"OFF") )
    {
      pVC->pCIP = &CCond_ST16_LC64::CompareInt;
      pVC->Iparam = 0;
    }
    else
    {
      CStdCond Key;
      Key.Cond = pParam;
      if( id_RL!=pObj->Y_Type )
      {
        OutputDebugString("Сравнение с 0 для не RL\n");
      }
      CStdCond * pRes = (CStdCond*)bsearch( &Key, CompareWith0, _countof(CompareWith0), sizeof( CStdCond ), CompStdCond );
      if ( pRes )
      {
        pVC->pC = pRes->pC;
        if( id_RL==pObj->Y_Type )
        {
          ASS( 'X'==*Cond );
          pVC->pObj = pObj;
        }
      }
      else
      {
        CBCond Key;
        Key.pParam = pParam;
        CBCond * pRes = (CBCond*)bsearch( &Key, BCond, _countof(BCond), sizeof( CBCond ), CompBCond );
        if ( pRes )
        {
          pVC->Iparam = pRes->Bparam;
          pVC->pCBP = &CCond_ST16_LC64::CompareByte;
        }
        else
          ASS( 0 );
      }
    }
  }

}
