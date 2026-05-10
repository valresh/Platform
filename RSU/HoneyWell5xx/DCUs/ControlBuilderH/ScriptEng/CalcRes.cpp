#include <rsuErr.h>
#include "CalcRes.h"
#include "BmBase.h"

KCalcRes::KCalcRes()
: m_bAddrVar( FALSE )
, m_pTypeVar( NULL )
{
  m_bitOffset = -1;
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
  return m_TypeVar==evtDouble;
}

bool KCalcRes::IsInteger()
{
  return m_TypeVar==evtInt;
}

bool KCalcRes::IsFlag()
{
  return m_TypeVar==evtBool;
}

bool KCalcRes::IsUnknown()
{
  return m_TypeVar==evtHZ;
}

KCalcRes::operator double()
{
  ASS( m_Type!=ns_CALCU::None );
  switch( m_TypeVar )
  {
  case evtDouble:
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      dValue = *pdValue;
    return dValue;
  case evtInt:
    if( m_Type==ns_CALCU::ReferenceToField && piValue )
    {
      if( m_bitOffset>=0 )
        iValue = *piValue & (1<<m_bitOffset) ? 1 : 0;
      else
        iValue = *piValue;
    }
    return iValue;
  case evtBool:
    if( m_Type==ns_CALCU::ReferenceToField && pbValue )
      bValue = *pbValue;
    return bValue ? 1. : 0;
  case evtHZ:
    if( pdValue )
    {
      dValue = *pdValue;
      return dValue;
    }
    return 0;
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
    m_TypeVar = evtDouble;
  }
  if( evtHZ==m_TypeVar )
  {
    m_Type = ns_CALCU::Const;
    m_TypeVar = evtDouble;
    if( m_pTypeVar )
      *m_pTypeVar = m_TypeVar;
  }
  switch( m_TypeVar )
  {
  case evtDouble:
    dValue = thr;
    if( (m_Type==ns_CALCU::ReferenceToField || m_bAddrVar) && pdValue )
      *pdValue = thr;
    break;
  case evtInt:
    iValue = (int)thr;
    if( (m_Type==ns_CALCU::ReferenceToField || m_bAddrVar) && piValue )
      *piValue = (int)thr;
    break;
  default:
    ASSD( 0 );
    return *this;
  }
  if( m_pObj && m_szField[0] )
    m_pObj->OnAssignField( m_szField );
  return *this;
}

KCalcRes::operator int()
{
  ASS( m_Type!=ns_CALCU::None );
  switch( m_TypeVar )
  {
  case evtDouble:
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      dValue = *pdValue;
    return (int)dValue;
  case evtInt:
    if( m_Type==ns_CALCU::ReferenceToField && piValue )
    {
      if( m_bitOffset>=0 )
        iValue = *piValue & (1<<m_bitOffset) ? 1 : 0;
      else
        iValue = *piValue;
    }
    return iValue;
  case evtBool:
    if( m_Type==ns_CALCU::ReferenceToField && pbValue )
      bValue = *pbValue;
    return bValue;
  case evtByte:
    if( m_Type==ns_CALCU::ReferenceToField && pbtValue )
      btValue = *pbtValue;
    return btValue;
  default:
    ASSD( 0 );
  }
  return 0;
}

KCalcRes::operator bool()
{
  ASS( m_Type!=ns_CALCU::None );
  switch( m_TypeVar )
  {
  case evtBool:
    if( m_Type==ns_CALCU::ReferenceToField && pbValue )
      bValue = *pbValue;
    return bValue;
  default:
    ASSD( 0 );
  }
  return 0;
}

KCalcRes& KCalcRes::operator = ( const int &thr )
{
  if( m_Type==ns_CALCU::None )
    m_Type = ns_CALCU::Const;
  if( evtHZ==m_TypeVar )
  {
    m_TypeVar = evtInt;
    if( m_pTypeVar )
      *m_pTypeVar = m_TypeVar;
  }
  switch( m_TypeVar )
  {
  case evtDouble:
    dValue = thr;
    if( m_Type==ns_CALCU::ReferenceToField && pdValue )
      *pdValue = thr;
    break;
  case evtInt:
    iValue = thr;
    if( m_Type==ns_CALCU::ReferenceToField && piValue )
    {
      if( m_bitOffset>=0 )
      {
        if( thr )
          *piValue |= (1<<m_bitOffset);
        else
          *piValue &= ~(1<<m_bitOffset);
      }
      else
        *piValue = thr;
    }
    break;
  case evtBool:
    bValue = thr ? true : false;
    if( m_Type==ns_CALCU::ReferenceToField && pbValue )
      *pbValue = thr ? true : false;
    break;
  case evtByte:
    btValue = thr;
    if( m_Type==ns_CALCU::ReferenceToField && pbtValue )
      *pbtValue = thr;
    break;
  default:
    ASSD( 0 );
    return *this;
  }
  if( m_pObj && m_szField[0] )
    m_pObj->OnAssignField( m_szField );
  return *this;
}

void KCalcRes::shift2index( int index )
{
  ASSD( m_szField[0] && m_pObj );
  if( !m_pObj || !m_szField[0] )
    return;
  char t[_countof(m_szField)];
  sprintf_s( t, "%s[%d]", m_szField, index);
  this->InitManual( m_pObj, t );
}

void KCalcRes::AssignMeString( const KCalcRes &thr )
{
  if( !pszValue || !thr.pszValue )
    return;
  strncpy_s( pszValue, varSize, thr.pszValue, varSize-1 );
}
