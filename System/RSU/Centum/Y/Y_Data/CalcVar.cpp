#include "stdafx.h"
#include "CalcVar.h"
#include <Y_Data.h>
#include <ConnectY.h>
#include <macros/StrHelps.h>
#include <rsuErr.h>
#include <stddef.h>

KCalcVar::KCalcVar()
{
  ZeroMemory( this, sizeof(*this) );
  assignConditionAsIs = -1;
}

bool KCalcVar::Numb()
{
  if( IsInt(NameValue) )
  {
    iValue = atoi(NameValue);
    m_TypeVar = ns_CALCU::INTEGER;
  }
  else if( IsFlt( NameValue ) )
  {
    dValue = atof(NameValue);
    m_TypeVar = ns_CALCU::REAL;
  }
  else if( !_strcmpi( NameValue, "TRUE") )
  {
    iValue = 1;
    m_TypeVar = ns_CALCU::INTEGER;
  }
  else if( !_strcmpi( NameValue, "FALSE") )
  {
    iValue = 0;
    m_TypeVar = ns_CALCU::INTEGER;
  }
  else if( strlen(NameValue)>1 && NameValue[0]=='0' && NameValue[1]=='x' )
  {
    char *p = NULL;
    iValue = strtoul(NameValue, &p, 16);
    m_TypeVar = ns_CALCU::INTEGER;
  }
  else
  {
    ASS( 0 );
    return false;
  }
  return true;
}

void KCalcVar::OutValue( char *txt, size_t _S )
{
  switch( m_TypeVar )
  {
  case ns_CALCU::STRING:
    if( pszValue && (ns_CALCU::ReferenceToField==m_Type || ns_CALCU::Const==m_Type)  )
      strcpy_s( txt, _S, pszValue );
    else
    {
      strcpy_s( txt, _S, "???");
    }
    break;
  case ns_CALCU::INTEGER:
    if( piValue && ns_CALCU::ReferenceToField==m_Type )
      sprintf_s( txt, _S, "%d", *piValue );
    else if( piValue && ns_CALCU::Condition==m_Type )
      sprintf_s( txt, _S, "{%d} val=%d ? iCond=%d", *piValue==cond_iValue, *piValue, cond_iValue );
    else if( ns_CALCU::Const==m_Type )
      sprintf_s( txt, _S, "%d", iValue );
    else
    {
      strcpy_s( txt, _S, "????");
    }
    break;
  case ns_CALCU::REAL:
    if( pdValue && ns_CALCU::ReferenceToField==m_Type )
      _snprintf_s( txt, _S, _S-1, "%0.12f", *pdValue );
    else if( ns_CALCU::Const==m_Type )
      sprintf_s( txt, _S, "%0.12f", dValue );
    else
    {
      strcpy_s( txt, _S, "?????");
    }
    break;
  case ns_CALCU::IntFromReal:
    {
      if( pdValue && ns_CALCU::ReferenceToField==m_Type )
        _snprintf_s( txt, _S, _S-1, "%d IntFromReal", (int)*pdValue );
    else if( ns_CALCU::Const==m_Type )
        sprintf_s( txt, _S, "%d IntFromReal", (int)dValue );
      else
        strcpy_s( txt, _S, "????? IntFromReal");
    }
    break;
  case ns_CALCU::FLAG:
    if( pwValue && ns_CALCU::ReferenceToField==m_Type )
      sprintf_s( txt, _S, "%d", *pwValue );
    else if( pwValue && ns_CALCU::Condition==m_Type )
      sprintf_s( txt, _S, "{%d} val=%d ? wCond=%d", *pwValue==cond_wValue, *pwValue, cond_wValue );
    else if( ns_CALCU::Const==m_Type )
      sprintf_s( txt, _S, "%d", wValue );
    else
    {
      strcpy_s( txt, _S, "??????");
    }
    break;
  default:
    KKK();
    strcpy_s( txt, _S, "???????");
  }
}

void KCalcVar::GetBeautyName( char *txt, size_t _S )
{
  if( ns_CALCU::ReferenceToField==m_Type )
  {
    strcpy_s( txt, _S, NameValue );
    return;
  }
  switch( m_TypeVar )
  {
  case ns_CALCU::STRING:
    if( ns_CALCU::Const==m_Type )
      sprintf_s( txt, _S, "\"%s\"", NameValue );
    else
      sprintf_s( txt, _S, "?\"%s\"?", NameValue );
    break;
  case ns_CALCU::REAL:
  case ns_CALCU::INTEGER:
  case ns_CALCU::FLAG:
    if( ns_CALCU::Const==m_Type )
      sprintf_s( txt, _S, "'%s'", NameValue[0] ? NameValue : "Const" );
    else if( ns_CALCU::Condition==m_Type )
      sprintf_s( txt, _S, "{%s}", NameValue );
    else
      sprintf_s( txt, _S, "?\"%s\"?", NameValue );
    break;
  }
}

struct CStdCond 
{ 
  LPCSTR Cond;
  KCalcVar::tCompare pC;
};

static int CompStdCond ( const void * p1, const void * p2 )
{
  CStdCond * pC1 = (CStdCond*)p1;
  CStdCond * pC2 = (CStdCond*)p2;
  return strcmp ( pC1->Cond, pC2->Cond );
}

static CStdCond CompareWith0[] = 
{
  "LE", &KCalcVar::LE,
  "LT", &KCalcVar::LT,
  "GE", &KCalcVar::GE,
  "GT", &KCalcVar::GT,
  "EQ", &KCalcVar::EQ,
};

void SortRLcompare()
{
  qsort( CompareWith0, _countof(CompareWith0), sizeof( CStdCond ), CompStdCond );
}

void KCalcVar::Init( CY_Base *owner )
{
  LPSTR pRestoryPoint = NULL;
  LPCSTR pCondValue = NULL;
  char restChar = '.';
  switch( m_Type )
  {
  case ns_CALCU::Condition:
    {
      LPSTR pPoint = strchr( NameValue, '.' );
      pRestoryPoint = strrchr( NameValue, restChar );
      ASS( pPoint );
      ASS( pRestoryPoint );
      if( pRestoryPoint!=pPoint )
        *pRestoryPoint = 0, pCondValue = pRestoryPoint+1;
      else
      {
        restChar = '=';
        pRestoryPoint = strrchr( NameValue, restChar );
        ASS( pRestoryPoint );
        *pRestoryPoint = 0, pCondValue = pRestoryPoint+1;
        if( !strcmp(pCondValue,"BAD") )
        {
          ecvCondConstRes = condBAD;
          break;
        }
        //ASS(0);
      }
    }
  case ns_CALCU::ReferenceToField:
    {
      m_Obj = owner;
      LPSTR pPoint = strchr( NameValue, '.' );
      LPSTR pField = NameValue;
      if( pPoint )
      {
        TStringTerminator st( pPoint );
        CY_Base *p = pFindYObjN( NameValue, owner->pDR->NumbFCS, owner->pDR->domain );
        //ASSD( p );
        if( !p )
          break;
        pField = pPoint + 1;
        m_Obj = p;
      }
      void * pTuneVar = NULL;
      void *pVar = NULL;
      CY_Base::eVarType Work = CY_Base::evtP;
      char varType = ' ';
      bool Res = m_Obj->GetVar( pField, &pVar, &pTuneVar, &varType, &Work );
      ASSD( Res );
      if( !Res )
        break;
      if( id__AN==m_Obj->Y_Type )
      {
        CAlarmBase::tFlagUse_ *pf = m_Obj->GetFlagUse();
        if( pf )
          *pf |= CAlarmBase::fuYConnect;
      }
      switch( varType )
      {
      case 'I':
        m_TypeVar = ns_CALCU::INTEGER;
        piValue = (int*)pVar;
        if( pCondValue )
        {
          AssignCondConstI( cond_iValue, pCondValue, pField );
          switch( cond_iValue )
          {
          case 128:
            ASS( !strcmp(pCondValue,"IMAN") );
            m_TypeVar = ns_CALCU::FLAG;
            varType = 'B';
            pwValue = (BYTE*)pVar + offsetof( SMode, SMode::IMAN );
            cond_wValue = 1;
            break;
          case -1:
            ASS( id_RL==m_Obj->Y_Type );
            if( id_RL==m_Obj->Y_Type )
            {
              SortRLcompare();
              CStdCond Key;
              Key.Cond = pCondValue;
              ASS( 'X'==*pField );
              CStdCond * pRes = (CStdCond*)bsearch( &Key, CompareWith0, _countof(CompareWith0), sizeof(CStdCond), CompStdCond );
              ASS( pRes );
              if( pRes )
                m_pCompare = pRes->pC;
              KKK();
            }
            break;
          }
        }
        break;
      case 'D':
        m_TypeVar = ns_CALCU::REAL;
        pdValue = (double*)pVar;
        if( pCondValue )
          AssignCondConstD( cond_dValue, pCondValue, pField );
        break;
      case 'B':
      case 'W':
        m_TypeVar = ns_CALCU::FLAG;
        if( !strcmp(pField, "SA") )
        {
          ASSD( id_ST16==m_Obj->Y_Type || id_ST16E==m_Obj->Y_Type );
          char sz[16] = {};
          sprintf_s( sz, "%s.%s", pField, pCondValue );
          bool Res = m_Obj->GetVar( sz, &pVar, &pTuneVar, &varType, NULL, NULL, &saCount );
          ASSD( Res );
          if( !Res )
            break;
          cond_wValue = 1;
          pwValue = (BYTE*)pVar;
          m_pCompare = &KCalcVar::CompareFlagArrayByte4ST16;
        }
        else
        {
          pwValue = (BYTE*)pVar;
          if( pCondValue )
            AssignCondConstW( cond_wValue, pCondValue, pField );
        }
        break;
      case 'S':
        m_TypeVar = ns_CALCU::STRING;
        pszValue = (LPSTR)pVar;
        break;
      default:
        m_TypeVar = ns_CALCU::HZ;
        pwValue = (BYTE*)pVar;
        //ASSD( 0 );
        //KKK();
        break;
      }
    }
    break;
  case ns_CALCU::Const:
    if( ns_CALCU::STRING==m_TypeVar )
      pszValue = NameValue;
    else
      Numb();
    break;
  case ns_CALCU::Var:
    {
      void * pTuneVar = NULL;
      void *pVar = NULL;
      CY_Base::eVarType Work = CY_Base::evtP;
      char varType = ' ';

      LPSTR pPoint = strchr( NameValue, '.' );
      LPSTR pField = "PV";
      TStringTerminator st( pPoint );
      if( pPoint )
      {
        *pPoint = 0;
        pField = pPoint + 1;
      }

      bool Res = false;
      CY_Base *p = pFindYObjN( NameValue, owner->pDR->NumbFCS, owner->pDR->domain );
      if( p )
      {
        m_Obj = p;
        Res = m_Obj->GetVar( pField, &pVar, &pTuneVar, &varType, &Work );
      }

      if( !Res )
        Res = owner->GetVar( NameValue, &pVar, &pTuneVar, &varType, &Work );
      ASS( Res );
      if( Res )
      {
        m_Type = ns_CALCU::ReferenceToField;
        switch( varType )
        {
        case 'I':
          m_TypeVar = ns_CALCU::INTEGER;
          piValue = (int*)pVar;
          break;
        case 'D':
          m_TypeVar = ns_CALCU::REAL;
          pdValue = (double*)pVar;
          break;
        case 'W':
          m_TypeVar = ns_CALCU::FLAG;
          pwValue = (BYTE*)pVar;
          break;
        case 'X':
          pdValue = (double*)pVar;
          break;
        default:
          ASS(0);
        }
        break;
      }

      switch( m_TypeVar )
      {
      case ns_CALCU::HZ:
        KKK();
      case ns_CALCU::REAL:
        pdValue = &dValue;
      break;
      case ns_CALCU::INTEGER:
        piValue = &iValue;
        break;
      case ns_CALCU::FLAG:
        pwValue = &wValue;
        break;
      }
    }
    break;
  default:
    ASS( 0 );
  }
  if( pRestoryPoint )
    *pRestoryPoint = restChar;
}
//////////////////////////////////////////////////////////////////////////
KCalcRes::KCalcRes()
: m_bAddrVar( FALSE )
, m_pTypeVar( NULL )
{
}

KCalcRes& KCalcRes::operator = ( const KCalcVar &thr )
{
  *(static_cast<KCalcVar*>(this)) = thr;
  return *this;
}

void KCalcRes::Reset()
{
  KCalcVar empty;
  *this = empty;
}

bool KCalcRes::IsReal()
{
  return m_TypeVar==ns_CALCU::REAL;
}

bool KCalcRes::IsInteger()
{
  return m_TypeVar==ns_CALCU::INTEGER || m_TypeVar==ns_CALCU::IntFromReal;
}

bool KCalcRes::IsFlag()
{
  return m_TypeVar==ns_CALCU::FLAG;
}

bool KCalcRes::IsString()
{
  return m_TypeVar==ns_CALCU::STRING;
}

bool KCalcRes::IsUnknown()
{
  return m_TypeVar==ns_CALCU::HZ;
}

KCalcRes::operator double()
{
  ASS( m_Type!=ns_CALCU::None );
  switch( m_TypeVar )
  {
  case ns_CALCU::IntFromReal:
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      dValue = *pdValue;
    return (int)dValue;
  case ns_CALCU::REAL:
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      dValue = *pdValue;
    return dValue;
  case ns_CALCU::INTEGER:
    if( m_Type==ns_CALCU::ReferenceToField && piValue )
      iValue = *piValue;
    return iValue;
  case ns_CALCU::HZ:
    if( pdValue )
    {
      dValue = *pdValue;
      return dValue;
    }
    return 0;
  case ns_CALCU::FLAG:
    if( m_Type==ns_CALCU::ReferenceToField && pwValue )
      wValue = *pwValue;
    if( m_Type==ns_CALCU::Condition && pwValue )
    {
      if( !m_pCompare )
        wValue = (*pwValue==cond_wValue) ? 1 : 0;
      else
      {
        m_Obj->CallSA( NameValue );
        (this->*m_pCompare)();
      }
    }
    return wValue;
  case ns_CALCU::STRING:
    {
      if( pszValue )
        dValue = atof( pszValue );
      return dValue;
    }
  default:
    ASS( 0 );
  }
  return 0;
}

KCalcRes& KCalcRes::operator = ( const double &thr )
{
  if( m_Type==ns_CALCU::None )
  {
    m_Type = ns_CALCU::Const;
    m_TypeVar = ns_CALCU::REAL;
  }
  if( ns_CALCU::HZ==m_TypeVar )
  {
    m_Type = ns_CALCU::Const;
    m_TypeVar = ns_CALCU::REAL;
    if( m_pTypeVar )
      *m_pTypeVar = m_TypeVar;
  }
  switch( m_TypeVar )
  {
  case ns_CALCU::REAL:
    dValue = thr;
    if( (m_Type==ns_CALCU::ReferenceToField || m_bAddrVar) && pdValue )
      *pdValue = thr;
    break;
  case ns_CALCU::INTEGER:
    iValue = (int)thr;
    if( (m_Type==ns_CALCU::ReferenceToField || m_bAddrVar) && piValue )
      *piValue = (int)thr;
    break;
  default:
    ASS( 0 );
  }
  if( m_pSignal )
    m_pSignal->Off( CSignal::_SkipTrasfer );
  return *this;
}

KCalcRes::operator int()
{
  if( m_Type==ns_CALCU::Condition && ecvCondConstRes )
  {
    if( condBAD==ecvCondConstRes )
      return 0;
    ASS(0);
  }
  ASS( m_Type!=ns_CALCU::None );
  switch( m_TypeVar )
  {
  case ns_CALCU::IntFromReal:
  case ns_CALCU::REAL:
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      dValue = *pdValue;
    return (int)dValue;
  case ns_CALCU::INTEGER:
    if( m_Type==ns_CALCU::ReferenceToField && piValue )
      iValue = *piValue;
    if( m_Type==ns_CALCU::Condition && piValue )
    {
      if( !m_pCompare )
        iValue = (*piValue==cond_iValue) ? 1 : 0;
      else
      {
        m_Obj->CallSA( NameValue );
        (this->*m_pCompare)();
      }
    }
    return iValue;
  case ns_CALCU::FLAG:
    if( m_Type==ns_CALCU::ReferenceToField && pwValue )
      wValue = *pwValue;
    if( m_Type==ns_CALCU::Condition && pwValue )
      wValue = (*pwValue==cond_wValue) ? 1 : 0;
    return wValue;
  case ns_CALCU::STRING:
    iValue = atoi( pszValue );
    return iValue;
  default:
    KKK();
    //ASS( 0 );
  }
  return 0;
}

KCalcRes::operator BYTE()
{
  ASS( m_Type!=ns_CALCU::None );
  switch( m_TypeVar )
  {
  case ns_CALCU::FLAG:
    if( m_Type==ns_CALCU::ReferenceToField && pwValue )
      wValue = *pwValue;
    if( m_Type==ns_CALCU::Condition && pwValue )
      wValue = (*pwValue==cond_wValue) ? 1 : 0;
    return wValue;
  default:
    ASS( 0 );
  }
  return 0;
}

KCalcRes::operator LPCSTR()
{
  switch( m_TypeVar )
  {
  case ns_CALCU::STRING:
    ASSD( ns_CALCU::Const==m_Type || ns_CALCU::ReferenceToField==m_Type );
    ASSD(pszValue);
    return pszValue;
  case ns_CALCU::INTEGER:
    {
      ASSD( ns_CALCU::Const==m_Type );
      static char s_tmpINT[32];
      sprintf_s( s_tmpINT, "%d", cond_iValue );
      return s_tmpINT;
    }
  default:
    ASS( 0 );
  }
  return NULL;
}

KCalcRes& KCalcRes::operator = ( const int &thr )
{
  if( m_Type==ns_CALCU::None )
    m_Type = ns_CALCU::Const;
  if( ns_CALCU::HZ==m_TypeVar )
  {
    m_TypeVar = ns_CALCU::INTEGER;
    if( m_pTypeVar )
      *m_pTypeVar = m_TypeVar;
  }
  switch( m_TypeVar )
  {
  case ns_CALCU::REAL:
    dValue = thr;
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      *pdValue = thr;
    break;
  case ns_CALCU::INTEGER:
    iValue = thr;
    if( m_Type==ns_CALCU::ReferenceToField && piValue )
      *piValue = thr;
    else if( m_Type==ns_CALCU::Condition && piValue )
    {
      if( 111==assignConditionAsIs )
        *piValue = thr;
      else
      {
        if( thr )
          *piValue = cond_iValue;
        else if( 222!=assignConditionAsIs )
          *piValue = !cond_iValue;
      }
      if( m_Obj )
        m_Obj->OnAssigned( piValue );
    }
    break;
  case ns_CALCU::FLAG:
    wValue = thr;
    if( m_Type==ns_CALCU::ReferenceToField && pwValue )
      *pwValue = thr;
    else if( m_Type==ns_CALCU::Condition && pwValue )
    {
      if( 111==assignConditionAsIs )
        *pwValue = thr;
      else
      {
        if( thr )
          *pwValue = cond_wValue;
        else if( 222!=assignConditionAsIs )
          *pwValue = !cond_wValue;
      }
    }
    break;
  default:
    ASS( 0 );
  }
  if( m_pSignal )
    m_pSignal->Off( CSignal::_SkipTrasfer );
  return *this;
}

KCalcRes& KCalcRes::operator = ( LPCSTR thr )
{
  if( !thr )
    return *this;
  switch( m_TypeVar )
  {
  case ns_CALCU::STRING:
    if( m_Type==ns_CALCU::ReferenceToField && pszValue )
      strcpy( pszValue, thr);
    break;
  default:
    ASS(0);
  }
  if( m_pSignal )
    m_pSignal->Off( CSignal::_SkipTrasfer );
  return *this;
}

void KCalcVar::LE()
{
  ASS( m_TypeVar == ns_CALCU::INTEGER );
  int PV = *piValue;
  iValue = PV ? true : false;
}

void KCalcVar::LT()
{
  ASS( m_TypeVar == ns_CALCU::INTEGER );
  int PV = *piValue;
  iValue = PV ? true : false;
}

void KCalcVar::EQ()
{
  ASS( m_TypeVar == ns_CALCU::INTEGER );
  int PV = *piValue;
  iValue = PV ? true : false;
}

void KCalcVar::GT()
{
  ASS( m_TypeVar == ns_CALCU::INTEGER );
  int PV = *piValue;
  iValue = PV ? true : false;
}

void KCalcVar::GE()
{
  ASS( m_TypeVar == ns_CALCU::INTEGER );
  int PV = *piValue;
  iValue = PV ? true : false;
}

void KCalcVar::CompareFlagArrayByte4ST16()
{
  ASS( m_TypeVar == ns_CALCU::FLAG );
  BYTE *w = (BYTE*)pwValue;

  BYTE eq = 0;
  for( USHORT i=0, c(__max(1, saCount )); i<c; ++i )
  {
    if ( w[i]==cond_wValue )
      eq = 1;
  }
  wValue = eq;
}
