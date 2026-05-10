#include "CalcVar.h"
#include <rsuIsType.h>
#include <BmBase.h>
#include <macros/StrHelps.h>
#include "CBPool.h"
#include <limits>
#include <rsuErr.h>
#include <crosslocale.h>

extern KCBPool g_Pool;

KCalcVar::KCalcVar()
: pdValue( NULL )
, m_bitOffset( -1 )
, m_pObj( NULL )
{
  m_szField[0] = 0;
}

bool KCalcVar::Numb()
{
  if( ::IsInt(m_NameValue) )
  {
    iValue = atoi(m_NameValue);
    m_TypeVar = evtInt;
  }
  else if( ::IsFlt( m_NameValue ) )
  {
    dValue = cross::locale::safe_atof(m_NameValue);
    m_TypeVar = evtDouble;
  }
  else if( !_strcmpi( m_NameValue, "TRUE") )
  {
    iValue = 1;
    m_TypeVar = evtInt;
  }
  else if( !_strcmpi( m_NameValue, "FALSE") )
  {
    iValue = 0;
    m_TypeVar = evtInt;
  }
  else if( strlen(m_NameValue)>1 && m_NameValue[0]=='0' && m_NameValue[1]=='x' )
  {
    char *p = NULL;
    iValue = strtoul(m_NameValue, &p, 16);
    m_TypeVar = evtInt;
  }
  else if( !_strcmpi( m_NameValue, "NAN") )
  {
    dValue = NaN;
    m_TypeVar = evtDouble;
  }
  else if( !_strcmpi( m_NameValue, "+INF") )
  {
    dValue = std::numeric_limits<double>::infinity();
    m_TypeVar = evtDouble;
  }
  else if( !_strcmpi( m_NameValue, "-INF") )
  {
    dValue = -std::numeric_limits<double>::infinity();
    m_TypeVar = evtDouble;
  }
  else
  {
    ASS( 0 );
    return false;
  }
  return true;
}

bool Link2SMGetVar( LPCSTR pBlock, LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize );
bool KCalcVar::Init( KBmBase *pRoot )
{
  m_bitOffset = -1;
  switch( m_Type )
  {
  case ns_CALCU::Var:
    {
      ns_CALCU::eType curType = m_Type;
      m_Type = ns_CALCU::ReferenceToField;
      Init( pRoot );

      switch( m_TypeVar )
      {
      case evtInt:
        if( !piValue )
          piValue = &iValue;
        break;
      case evtDouble:
        if( !pdValue )
          pdValue = &dValue;
        break;
      case evtBool:
        if( !pbValue )
          pbValue = &bValue;
        break;
      default:
        ASS(0);
        break;
      }
      KKK();
    }
    break;
  case ns_CALCU::ReferenceToField:
    {
      BYTE *pVar = NULL;
      USHORT varSize = 0;
      LPCSTR pRef = m_NameValue;
      bool r = false;
      char buf[64] = {};
      if( '\''==*pRef )
      {
        ++pRef;
        int j = 0;
        for( pRef; '\''!=pRef[0]; ++pRef, ++j )
          buf[j] = pRef[0];
        ++pRef;
        strcat_s( buf, pRef );
      }
      else
        strcpy_s( buf, pRef );

      r = pRoot->GetVar( buf, &pVar, &m_TypeVar, &varSize );
      if( !r )
      {
        LPSTR p = NULL;
        int n = 0;
        for( LPSTR pT = buf; *pT; ++pT )
        {
          if( '.'==*pT )
          {
            p = pT;
            n++;
            if( 2==n )
              break;
          }
        }
        ASS( p );
        TStringTerminator st(p);
        ++p;
        if( 1==n )
          m_pObj = pRoot->QuickFindObj( buf, 1 );
        else
          m_pObj = pRoot->QuickFindObj( buf );
        if( m_pObj )
        {
          r = m_pObj->GetVar( p, &pVar, &m_TypeVar, &varSize );
          if( !r )
            m_pObj = NULL;
          else
            strcpy_s( m_szField, p );
        }
        else
          r = g_Pool.GetVar( NULL, buf, p, &pVar, &m_TypeVar, &varSize );
        if( !r )
          r = Link2SMGetVar( buf, p, &pVar, &m_TypeVar, &varSize );
      }
      if( !r )
      {
        OutputDebugString("Calc Не связался с точкой ");
        OutputDebugString( pRef );
        return false;
      }
      /*ASS( r );
      ASS( pVar );*/
      switch( m_TypeVar )
      {
      case evtInt:
        piValue = (int*)pVar;
        break;
      case evtDouble:
        pdValue = (double*)pVar;
        break;
      case evtBool:
        pbValue = (bool*)pVar;
        break;
      case evtByte:
        pbtValue = (BYTE*)pVar;
        break;
      case evtString:
        pszValue = (char*)pVar;
        this->varSize = varSize;
        /*OutputDebugString("Calc Не связался с точкой ");
        OutputDebugString( pRef );
        OutputDebugString(", т.к. это строка ");
        return false;*/
        break;
      default:
        ASSD( !r );
        break;
      }
      KKK();
    }
    break;
  case ns_CALCU::Const:
    KKK();
    break;
  default:
    ASS(0);
  }
  return true;
}

bool KCalcVar::InitManual( KBmBase *pOwner, LPCSTR pField )
{
  m_bitOffset = -1;
  m_Type = ns_CALCU::ReferenceToField;
  BYTE *pVar = NULL;
  LPCSTR pRef = pField;
  USHORT varSize = 0;
  bool r = false;
  r = pOwner->GetVar( pRef, &pVar, &m_TypeVar, &varSize );

  ASSD( r );
  ASSD( pVar );
  switch( m_TypeVar )
  {
  case evtInt:
    piValue = (int*)pVar;
    break;
  case evtDouble:
    pdValue = (double*)pVar;
    break;
  case evtBool:
    pbValue = (bool*)pVar;
    break;
  case evtString:
    pszValue = (char*)pVar;
    this->varSize = varSize;
    break;
  default:
    ASSD(0);
    return r;
  }
  strcpy_s( m_szField, pField );

  return r;
}
