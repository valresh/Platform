#include "stdafx.h"
#include "Y_Data.h"
#include <sstream>
#include "../Y_TransCALCU/Trans.h"
#include "CalcVar.h"
#include <SR_DCU.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ConnectY.h>
#include <rsuPaths.h>

using namespace std;
using namespace ns_translator;

static KCalcRes Stack[100];
static int kStack = 0;

CY_CALCU_BASE::CY_CALCU_BASE()
: cVars( 0 )
, cOPs( 0 )
, pOPs( NULL )
, pVars( NULL )
, pLocalVars( NULL )
, cLocalVars( 0 )
, Type( Simple )
, nStepDuration( 1 )
, m_LastWorkStep( 0 )
{
}

LPCSTR GetCalcuTxtFormatPath()
{
  static char Path_CalcuTxtFormat[_MAX_PATH] = { 0 };
  if( !*Path_CalcuTxtFormat )
  {
    strcpy_s( Path_CalcuTxtFormat, rsuGetProjectPath() );
    strcat_s( Path_CalcuTxtFormat, "Data\\CALCU\\TXT\\%s.txt");
  }
  return Path_CalcuTxtFormat;
}

bool CY_CALCU_BASE::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "CLSR", 4) )
  {
    KYTrans trans(this);
    
    stringstream ss;
    LPCSTR pAppend = "C";

    string strUp( ParmValue );
    if( Simple==Type)
    {
      transform( strUp.begin(), strUp.end(), strUp.begin(), toupper );
      pAppend = NULL;
    }
    else
    {
      BOOL bComOpen = FALSE;
      for( string::iterator it=strUp.begin(), end(strUp.end()); it!=end; ++it )
      {
        string::value_type &chr = *it;
        switch( chr )
        {
        case '"':
          bComOpen = !bComOpen;
          break;
        case '\r':
        case '\n':
          ASS( !bComOpen );
        default:
          if( !bComOpen )
            chr = toupper( chr );
        }
      }
    }

    sprintf_s( szAlgorithmFileName, "%s.txt", Name );
    char Path[512] = { 0 };
    sprintf_s ( Path, sizeof ( Path ), GetCalcuTxtFormatPath(), Name );
    if( pAppend )
    {
      strcat_s( Path, pAppend );
      strcat_s( szAlgorithmFileName, pAppend);
    }
    FILE * F = NULL;
    fopen_s( &F, Path, "wt" );
    if( F )
    {
      fprintf ( F, "%s", ParmValue );
      fclose ( F );
    }
    ss << strUp;
    trans.Translate( ss );

    vector<KVar> vVars;

    trans.GetResult( cVars, cOPs);
    if( cVars )
    {
      vVars.resize( cVars );
      pVars = new KCalcVar[cVars];
    }
    if( cOPs )
    {
      pOPs = new SOPElement[cOPs];
    }
    trans.GetResult( cVars ? &vVars[0] : NULL, cOPs ? (SOPElement*)pOPs : NULL ); 

    for( int i=0; i<cVars; i++ )
    {
      KVar& var = vVars[i];
      KCalcVar &cv = ((KCalcVar*)pVars)[i];
      strcpy_s( cv.NameValue, var.szName );
      switch( var.Type )
      {
      case KVar::Const:
        cv.m_Type = ns_CALCU::Const;
        break;
      case KVar::ConstString:
        cv.m_Type = ns_CALCU::Const;
        cv.m_TypeVar = ns_CALCU::STRING;
        break;
      case KVar::Reference:
        cv.m_Type = ns_CALCU::ReferenceToField;
        break;
      case KVar::Double:
        cv.m_Type = ns_CALCU::Var;
        cv.m_TypeVar = ns_CALCU::REAL;
        break;
      case KVar::Integer:
        cv.m_Type = ns_CALCU::Var;
        cv.m_TypeVar = ns_CALCU::INTEGER;
        break;
      case KVar::Condition:
        cv.m_Type = ns_CALCU::Condition;
        break;
      case KVar::UnknownTypeReference:
        cv.m_Type = ns_CALCU::Var;
        cv.m_TypeVar = ns_CALCU::HZ;
        {
          stringstream tr;
          tr << "В " << szAkaObjName << " встретилась неопределенного типа переменная " << cv.NameValue << ", будет определяться по месту\n";
          OutputDebugString( tr.str().c_str() );
        }
        break;
      default:
        ASS( 0 );
      }
    }

    cLocalVars = 0;
    for( int i=0; i<cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)pVars)[i];
      if( ns_CALCU::Var==cv.m_Type)
        cLocalVars++;
    }

    if( cLocalVars )
    {
      pLocalVars = new SLocalVar[cLocalVars];

      cLocalVars = 0;
      for( int i=0; i<cVars; i++ )
      {
        KCalcVar &cv = ((KCalcVar*)pVars)[i];
        if( ns_CALCU::Var==cv.m_Type)
        {
          SLocalVar &lv = ((SLocalVar*)pLocalVars)[cLocalVars++];
          strcpy_s( lv.NameVar, cv.NameValue );
          lv.m_TypeVar = cv.m_TypeVar;
          lv.dValue = cv.dValue;
          lv.nInVars = i;
        }
      }
    }

    return true;
  }
  return CY_Base::ReadParm( ParmName, ParmValue, pData );
}

bool CY_CALCU_BASE::SaveParms( HANDLE hFile )
{
  DWORD Lw;
  WRT_VAR(L_Class_P);
  if( L_Class_P > 0 && pClass_P )
  {
    WRT_DATA( L_Class_P, pClass_P)
  }

  WRT_KEY(eYKeyCALCUbegin)
  
  WRT_VAR(cVars)
  for( int i=0; i<cVars; i++ )
  {
    KCalcVar &cv = ((KCalcVar*)pVars)[i];
    WRT_DATA( sizeof(SVarRetainedPart), (SVarRetainedPart*)&cv );
  }
  
  WRT_VAR(cOPs)
  if( cOPs )
    WRT_DATA( cOPs*sizeof(SOPElement), pOPs );

  WRT_VAR(cLocalVars)
  if( cLocalVars )
    WRT_DATA( cLocalVars*sizeof(SLocalVar), pLocalVars );

  WRT_VAR(szAlgorithmFileName);
  WRT_KEY(eYKeyCALCUend)
  return true;
}

bool CY_CALCU_BASE::RestParms( HANDLE hFile )
{
  int L = 0;
  DWORD Lr;
  READ_VAR(L);
  if ( L > 0 && pClass_P )
  {
    READ_DATA( min(L_Class_P, L),pClass_P);
  }
  READ_KEY(eYKeyCALCUbegin, false)
  READ_VAR(cVars)

  if( cVars )
  {
    pVars = new KCalcVar[cVars];
    for( int i=0; i<cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)pVars)[i];
      READ_DATA( sizeof(SVarRetainedPart), (SVarRetainedPart*)&cv );
    }
  }
  READ_VAR(cOPs)
  if( cOPs )
  {
    pOPs = new SOPElement[cOPs];
    READ_DATA( cOPs*sizeof(SOPElement), pOPs );
  }

  READ_VAR(cLocalVars)
  if( cLocalVars )
  {
    pLocalVars = new SLocalVar[cLocalVars];
    READ_DATA( cLocalVars*sizeof(SLocalVar), pLocalVars );
  }

  READ_VAR(szAlgorithmFileName);
  READ_KEY(eYKeyCALCUend, false);
  return true;
}

int CY_CALCU_BASE::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  __super::GetParams( params );
  if( szAlgorithmFileName[0] )
  {
    SYParam& __P = params.Add( szAlgorithmFileName, "#Алгоритм" );
    __P.pCallExternWindowHandler = CallDrawCalcuCHandler;
  }
  PARM( nStepDuration, "stepDuration" )
  PARM( m_LastWorkStep, "#Шаг Срабатывания")
  return 0;
}

bool CY_CALCU_BASE::CreatePSMGraf( CGrafParm ** GP )
{
  if( pCreateCalcuBaseWnd )
    (*pCreateCalcuBaseWnd)( szAlgorithmFileName, NULL, this, Stack, &kStack );
  return true;
}

bool CY_CALCU_BASE::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool bCommon = CY_Base::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( bCommon )
    return bCommon;

  for( int i=0; i<cLocalVars; i++ )
  {
    SLocalVar &lv = ((SLocalVar*)pLocalVars)[i];
    if( strcmp(VarName, lv.NameVar) )
      continue;
    switch( lv.m_TypeVar )
    {
    case ns_CALCU::INTEGER:
      *pVar = &lv.iValue;
      *Type = 'I';
      break;
    case ns_CALCU::REAL:
      *pVar = &lv.dValue;
      *Type = 'D';
      break;
    case ns_CALCU::FLAG:
      *pVar = &lv.wValue;
      *Type = 'W';
      break;
    case ns_CALCU::HZ:
      *pVar = &lv.dValue;
      *Type = 'X';
      break;
    default:
      ASS(0);
    }
    return true;
  }

  return false;
}

void CY_CALCU_BASE::InitTrans()
{
  for( int i=0; i<cVars; i++ )
  {
    KCalcVar &cv = ((KCalcVar*)pVars)[i];
    cv.Init( this );
    KKK();
  }
}

int CY_CALCU_BASE::SaveState()
{
  pYokoStateSer->SimpleWrite( emkcStruct );
  pYokoStateSer->SimpleWrite( cLocalVars );
  for( int i=0; i<cLocalVars; i++ )
  {
    SLocalVar &lv = ((SLocalVar*)pLocalVars)[i];
    void *pVar = NULL;
    DWORD size = 0;
    switch( lv.m_TypeVar )
    {
    case ns_CALCU::INTEGER:
      pVar = &lv.iValue;
      size = sizeof(lv.iValue);
      break;
    case ns_CALCU::REAL:
      pVar = &lv.dValue;
      size = sizeof(lv.dValue);
      break;
    case ns_CALCU::FLAG:
      pVar = &lv.wValue;
      size = sizeof(lv.wValue);
      break;
    default:
      continue;
    }
    DWORD key = emkcStruct;
    pYokoStateSer->SimpleWrite( &key, sizeof( key ));
    pYokoStateSer->WriteStr( lv.NameVar );
    pYokoStateSer->WriteSized( pVar, size );
  }
  return 0;
}

int CY_CALCU_BASE::RestoreState( char * StrName )
{
  for( int i=0; i<cLocalVars; i++ )
  {
    SLocalVar &lv = ((SLocalVar*)pLocalVars)[i];
    if( strcmp( StrName, lv.NameVar ) )
      continue;
    void *pVar = NULL;
    DWORD size = 0;
    switch( lv.m_TypeVar )
    {
    case ns_CALCU::INTEGER:
      pVar = &lv.iValue;
      size = sizeof(lv.iValue);
      break;
    case ns_CALCU::REAL:
      pVar = &lv.dValue;
      size = sizeof(lv.dValue);
      break;
    case ns_CALCU::FLAG:
      pVar = &lv.wValue;
      size = sizeof(lv.wValue);
      break;
    default:
      continue;
    }
    ASS(pYokoStateSer);
    if( !pYokoStateSer )
      return 2;
    pYokoStateSer->SimpleRead( pVar, size );
    return 0;
  }

  return 1;
}

int CY_CALCU_BASE::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  DWORD key = emkcCalcuLocalVars;
  pYokoStateSer->SimpleWrite( key );

  int C = 0;
  for( int i=0; i<cLocalVars; i++ )
  {
    SLocalVar &lv = ((SLocalVar*)pLocalVars)[i];
    void *pVar = NULL;
    DWORD size = 0;
    switch( lv.m_TypeVar )
    {
    case ns_CALCU::INTEGER:
      pVar = &lv.iValue;
      size = sizeof(lv.iValue);
      break;
    case ns_CALCU::REAL:
      pVar = &lv.dValue;
      size = sizeof(lv.dValue);
      break;
    case ns_CALCU::FLAG:
      pVar = &lv.wValue;
      size = sizeof(lv.wValue);
      break;
    default:
      continue;
    }
    ++C;
  }

  pYokoStateSer->SimpleWrite( C );

  for( int i=0; i<cLocalVars; i++ )
  {
    SLocalVar &lv = ((SLocalVar*)pLocalVars)[i];
    void *pVar = NULL;
    DWORD size = 0;
    switch( lv.m_TypeVar )
    {
    case ns_CALCU::INTEGER:
      pVar = &lv.iValue;
      size = sizeof(lv.iValue);
      break;
    case ns_CALCU::REAL:
      pVar = &lv.dValue;
      size = sizeof(lv.dValue);
      break;
    case ns_CALCU::FLAG:
      pVar = &lv.wValue;
      size = sizeof(lv.wValue);
      break;
    default:
      continue;
    }
    key = emkcStruct;
    pYokoStateSer->SimpleWrite( key );
    pYokoStateSer->WriteStr( lv.NameVar );
    pYokoStateSer->SimpleWrite( lv.m_TypeVar );
    pYokoStateSer->WriteSized( pVar, size );
  }
  return 0;
}

int CY_CALCU_BASE::StateRestore( IYokoStateSer* pStateSer )
{
  int r = __super::StateRestore( pStateSer );
  if( r )
    return r;
  DWORD key = 0;
  pStateSer->SimpleRead( key );
  if( emkcCalcuLocalVars!=key )
  {
    pStateSer->Shift( -LONG(sizeof(key)) );
    return 0;
  }
  int saved = 0;
  pStateSer->SimpleRead( saved );
  char testName[64] = {};
  DWORD sizeSaved = 0;
  ns_CALCU::eTypeValue savedTypeVar;
  for( int s=0; s<saved; ++s )
  {
    pStateSer->SimpleRead( key );
    ASSD( emkcStruct==key );
    pStateSer->ReadLenStr( testName, _countof(testName) );
    pStateSer->SimpleRead( savedTypeVar );
    pStateSer->SimpleRead( sizeSaved );
    bool bRestored = false;
    for( int i=0; i<cLocalVars; i++ )
    {
      SLocalVar &lv = ((SLocalVar*)pLocalVars)[i];
      if( strcmp( testName, lv.NameVar ) )
        continue;
      void *pVar = NULL;
      DWORD size = 0;
      switch( savedTypeVar )
      {
      case ns_CALCU::INTEGER:
        pVar = &lv.iValue;
        size = sizeof(lv.iValue);
        break;
      case ns_CALCU::REAL:
        pVar = &lv.dValue;
        size = sizeof(lv.dValue);
        break;
      case ns_CALCU::FLAG:
        pVar = &lv.wValue;
        size = sizeof(lv.wValue);
        break;
      default:
        ASSD(0);
        pYokoStateSer->Shift( sizeSaved );
        continue;
      }
      lv.m_TypeVar = savedTypeVar;
      pYokoStateSer->SimpleRead( pVar, size );
      bRestored = true;
      break;
    }
    if( !bRestored )
      pYokoStateSer->Shift( sizeSaved );
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////
void push_back( KCalcVar &var )
{
  ASS( kStack < _countof(Stack) );
  Stack[kStack++] = var;
}

void push_back( KCalcVar *var )
{
  ASS( kStack < _countof(Stack) );
  Stack[kStack] = *var;
  Stack[kStack].m_bAddrVar = TRUE;
  Stack[kStack].m_pTypeVar = &var->m_TypeVar;
  kStack++;
}

KCalcRes pop_back()
{
  ASS( kStack >= 1 );
  KMethodOnDestroy<KCalcRes> C(Stack[--kStack], &KCalcRes::Reset );
  return C;
}

void Operation( eYToken_value cmd, bool *pMathErr )
{
  switch( cmd )
  {
  default:
    ASS( 0 );
    break;
  case eytINVERTBITS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes res;
      res = ~(int)V1;
      push_back( res );
    }
    break;
  case eytValMINUS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes res;
      if( V1.IsReal() )
        res = -(double)V1;
      else
        res = -(int)V1;
      push_back( res );
    }
    break;
  case eytNOT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes res;
      res = !(int)V1;
      push_back( res );
    }
    break;
  case eytBITOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = (int)V2 | (int)V1;
      push_back( res );
    }
    break;
  case eytBITAND:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = (int)V2 & (int)V1;
      push_back( res );
    }
    break;
  case eytSHIFT_RIGHT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2) >> ((int)V1);
      push_back( res );
    }
    break;
  case eytSHIFT_LEFT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2) << ((int)V1);
      push_back( res );
    }
    break;
  case eytOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2 || (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytAND:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2 && (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytNE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        if( abs((double)V2 - (double)V1) < 0.0001 )
          res = 0;
        else
          res = ((double)V2 != (double)V1) ? 1 : 0;
      }
      else if( V1.IsString() || V2.IsString() )
        res = strcmp( V1, V2) ? 1 : 0;
      else
        res = ((int)V2 != (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytLIT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        if( abs((double)V2 - (double)V1) < 0.0001 )
          res = 0;
        else
          res = ((double)V2 < (double)V1) ? 1 : 0;
      }
      else
        res = ((int)V2 < (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytBIG:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        if( abs((double)V2 - (double)V1) < 0.0001 )
          res = 0;
        else
          res = ((double)V2 > (double)V1) ? 1 : 0;
      }
      else
        res = ((int)V2 > (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytEQUAL:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = (abs((double)V2 - (double)V1) < 0.0001) ? 1 : 0;
      else if( V1.IsString() || V2.IsString() )
      {
        LPCSTR s1 = "", s2 = "";
        char tmp1[8], tmp2[8];
        if( V2.IsString() )
          s2 = V2;
        else if( V2.IsInteger() )
        {
          sprintf_s( tmp2, "%d", (int)V2 );
          s2 = tmp2;
        }
        else
        {
          ASSD(0);
        }
        if( V1.IsString() )
          s1 = V1;
        else if( V1.IsInteger() )
        {
          sprintf_s( tmp1, "%d", (int)V1 );
          s1 = tmp1;
        }
        else
        {
          ASSD(0);
        }
        res = 0==_strcmpi( s1, s2) ? 1 : 0;
      }
      else
        res = ((int)V2 == (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytLE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        if( abs((double)V2 - (double)V1) < 0.0001 )
          res = 1;
        else
          res = ((double)V2 <= (double)V1) ? 1 : 0;
      }
      else
        res = ((int)V2 <= (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytGE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        if( abs((double)V2 - (double)V1) < 0.0001 )
          res = 1;
        else
          res = ((double)V2 >= (double)V1) ? 1 : 0;
      }
      else
        res = ((int)V2 >= (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytPLUS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        res = (double)V2 + d1;
      }
      else
      {
        int d1 = V1;
        res = (int)V2 + d1;
      }
      push_back( res );
    }
    break;
  case eytMUL:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        res = (double)V2 * d1;
      }
      else
      {
        int d1 = V1;
        res = (int)V2 * d1;
      }
      push_back( res );
    }
    break;
  case eytMINUS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        res = (double)V2 - d1;
      }
      else
      {
        int d1 = V1;
        res = (int)V2 - d1;
      }
      push_back( res );
    }
    break;
  case eytXOR:
    {
      KCalcRes y = pop_back();
      KCalcRes x = pop_back();
      KCalcRes r;
        r = (int)x ^ (int)y;
      push_back( r );
    }
    break;
  case eytDIV:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsInteger() && V2.IsInteger() )
      {
        int d1 = V1;
        int d2 = V2;
        int r = INT_MAX;
        if( 0!=d1 )
          r = d2 / d1;
        else
        {
          if( pMathErr )
            *pMathErr = true;
        }
        res = r;
      }
      else if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        double d2 = V2;
        double r = DBL_MAX;
        if( 0!=d1 )
            r = d2 / d1;
        else
        {
          if( pMathErr )
            *pMathErr = true;
        }
        res = r;
      }
      else
      {
        int d1 = V1;
        int r = INT_MAX;
        if( d1!=0 )
          r = (int)V2 / d1;
        else
        {
          if( pMathErr )
            *pMathErr = true;
        }
        res = r;
      }
      push_back( res );
    }
    break;
  case eytMOD:
  case eytPERCENT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      int d1 = V1;
      int r = 0;
      if( d1!=0 )
        r = (int)V2 % d1;
      res = r;
      push_back( res );
    }
    break;
  case eytASSIGN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      if( V2.IsReal() )
        V2 = (double)V1;
      else if( V2.IsInteger() )
      {
        if( V1.IsReal() )
        {
          double t = V1;
          t += 0.5;
          V2 = (int)t;
        }
        else
          V2 = (int)V1;
      }
      else if( V2.IsFlag() )
        V2 = (int)V1;
      else if( V2.IsString() )
        V2 = (LPCSTR)V1;
      else if( V2.IsUnknown() )
      {
        if( V1.IsReal() )
          V2 = (double)V1;
        else if( V1.IsInteger() )
          V2 = (int)V1;
        else if( V1.IsFlag() )
          V2 = (BYTE)V1;
        else if( V1.IsUnknown() )
        {}
        else
        {
          ASSD(0);
        }
      }
      else
      {
        ASSD( 0 );
      }
    }
    break;
  }
}

void Function( eYFunctions nF, SOPElement* OP, int &nOP )
{
  switch( nF )
  {
  default:
    ASS( 0 );
    break;
  case eyfTC:
    {
      KCalcRes Tb = pop_back();
      KCalcRes T = pop_back();
      KCalcRes Fi = pop_back();
      double dblTb = Tb;
      double dblT = T;
      double dblFi = Fi;
      double r = sqrt( (dblTb+273.15) / (dblT+273.15) ) * dblFi;
      KCalcRes res;
      res = r;
      push_back( res );
    }
    break;
  case eyfLMAX:
  case eyfLMIN:
    {
      KCalcRes res = pop_back();
      int nParms = OP[nOP++];
      for( int i=0; i<nParms; ++i )
      {
        KCalcRes l = pop_back();
        if( nF==eyfLMAX )
        {
          if( (int)res < (int)l )
            res = l;
        }
        else
        {
          if( (int)res > (int)l )
            res = l;
        }
      }
      push_back( res );
    }
    break;
  case eyfDMAX:
  case eyfDMIN:
    {
      KCalcRes res = pop_back();
      int nParms = OP[nOP++];
      for( int i=0; i<nParms; ++i )
      {
        KCalcRes l = pop_back();
        if( nF==eyfDMAX )
        {
          if( (double)res < (double)l )
            res = l;
        }
        else
        {
          if( (double)res > (double)l )
            res = l;
        }
      }
      push_back( res );
    }
    break;
  case eyfDLIMIT:
    {
      KCalcRes h = pop_back();
      KCalcRes l = pop_back();
      KCalcRes t = pop_back();
      KCalcRes r;
      if( t.IsReal() )
      {
        r = (double)t;
        if( (double)r < (double)l )
          r = (double)l;
        if( (double)t > (double)h )
          r = (double)h;
      }
      else
      {
        r = (int)t;
        if( (int)r < (int)l )
          r = (int)l;
        if( (int)t > (int)h )
          r = (int)h;
      }
      push_back( r );
    }
    break;
  case eyfSIN:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      r = (double)sin( (double)V );
      push_back( r );
    }
    break;
  case eyfCOS:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      r = (double)cos( (double)V );
      push_back( r );
    }
    break;
  case eyfATAN:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      r = (double)atan( (double)V );
      push_back( r );
    }
    break;
  case eyfEXP:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      r = (double)exp( (double)V );
      push_back( r );
    }
    break;
  case eyfLOG:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      r = (double)log( (double)V );
      push_back( r );
    }
    break;
  case eyfPOWER:
    {
      KCalcRes y = pop_back();
      KCalcRes x = pop_back();
      KCalcRes r;
      if( y.IsInteger() )
        r = (double)::pow( (double)x, (int)y );
      else
        r = (double)::pow( (double)x, (double)y );
      push_back( r );
    }
    break;
  case eyfABS:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      if( V.IsReal() )
        r = fabs( V );
      else if( V.IsInteger() )
        r = abs( (int)V );
      push_back( r );
    }
    break;
  case eyfINT:
    {
      KCalcRes V = pop_back();
      KCalcRes r;
      if( V.IsReal() )
        r = (int)V;
      else if( V.IsInteger() )
        r = V;
      push_back( r );
    }
    break;
  case eyfSQRT:
{
      KCalcRes V = pop_back();
      KCalcRes r;
      
      double t = V;
      if( t <= 0 )
        t = 0;
      else
        t = sqrt( t );

      if( V.IsReal() )
        r = t;
      else if( V.IsInteger() )
        r = (int)t;
      else
        r = t;
      push_back( r );
    }
    break;
  }
}

void CY_CALCU_BASE::WorkTranslation( double dt, SMode &mode )
{
  if( 0==dt )
    return;
  m_LastWorkStep = CY_Base::s_nCalcSteps;
  KCalcVar *Var = (KCalcVar*)pVars;
  SOPElement* OP = (SOPElement*)pOPs;

  ZeroMemory( Stack, sizeof(Stack) );
  int nOP = 0;
  kStack = 0;

  int n = 0;
  bool bMathErr = false;
  while( SingleOP_exec( nOP, &mode, &bMathErr ) )
  {
    ++n;
  }
  if( bMathErr )
    Alarm.SEND_ON( A_CERR, 0 );
  else
    Alarm.SEND_OFF( A_CERR );
  
  kStack = 0;
}

bool CY_CALCU_BASE::SingleOP_exec( int &nOP, SMode *pMode, bool *pMathErr )
{
  if( 0==nOP )
    SetSkipAllOutTerminals();
  KCalcVar *Var = (KCalcVar*)pVars;
  SOPElement* OP = (SOPElement*)pOPs;
  if( nOP < cOPs )
  {
    SOPElement CurrOP = (SOPElement)OP[nOP++];
    switch( CurrOP.OP )
    {
    default:
      ASS( 0 );
      return false;
    case OP_EXIT:
      return false;
    case OP_NOP:
    case OP_LABEL:
      return true;
    case OP_SWITCH:
      return true;
    case OP_GOTO:
      nOP = OP[nOP];
      return true;
    case OP_OTHERWISE:
      {
        KCalcRes S = pop_back();
        nOP += 2;
      }
      return true;
    case OP_CASE:
      {
        KCalcRes c = pop_back();
        KCalcRes S = pop_back();
        int Next = nOP+2;
        if( (int)c!=(int)S )
        {
          Next = OP[nOP++];
          push_back( S );
        }
        nOP = Next;
      }
      return true;
    case OP_AddrVar:
      {
        int N = OP[nOP++];
        ASS( N < cVars );
        push_back( &Var[N] );
      }
      return true;
    case OP_LoadVar:
      {
        int N = OP[nOP++];
        ASS( N < cVars );
        push_back( Var[N] );
      }
      return true;
    case OP_Operation:
      {
        eYToken_value CMD = OP[nOP++];
        Operation( CMD, pMathErr );
      }
      return true;
    case OP_Function:
      {
        eYFunctions nF = (eYFunctions)(int)OP[nOP++];
        Function( nF, OP, nOP );
      }
      return true;
    case OP_BREAKTO:
      {
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= cOPs);
        nOP = OnEndif;
      }
      return true;
    case OP_GOTO_OTHER_ELSE:
      {
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= cOPs);
        nOP = OP[OnEndif];
      }
      return true;
    case OP_IF:
      {
        int OnFalse = OP[nOP++];// Переход по false
        if( !(OnFalse>0 && OnFalse <= cOPs) )
        {
          if( pMode )
            pMode->M = SMode::OS;
          ASSD(0);
          return false;
        }
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= cOPs);
        KCalcRes V1 = pop_back();
        int Next = -1;
        int R = V1;

        if( !R )
          Next = OnFalse;

        if ( Next > 0 )
          nOP = Next;
      }
      return true;
    }
  }
  
  return false;
}

void CY_CALCU_BASE::SetSkipAllOutTerminals()
{
  for( int i=0; i<cVars; i++ )
  {
    KCalcVar &cv = ((KCalcVar*)pVars)[i];
    if( !cv.m_pSignal )
      continue;
    cv.m_pSignal->On( CSignal::_SkipTrasfer );
  }
}

bool CY_CALCU_BASE::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
{
  for( pos; pos<cVars; ++pos )
  {
    KCalcVar &cv = ((KCalcVar*)pVars)[pos];
    if( !cv.m_Obj || cv.m_Obj==this )
      continue;
    if( ppObj )
      *ppObj = cv.m_Obj;
    *ppName = cv.NameValue;
    ++pos;
    return true;
  }
  return false;
}
