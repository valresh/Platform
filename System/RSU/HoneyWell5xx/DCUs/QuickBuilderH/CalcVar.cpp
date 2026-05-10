#include "CalcVar.h"
#include "QbBase.h"

KCalcVar::KCalcVar()
{
  Clear();
}

KCalcVar::KCalcVar( KCalcVar &thr )
{
  CopyFrom( thr );
}

void KCalcVar::CopyFrom( KCalcVar &thr )
{
  *(KVar*)this =  *(KVar*)&thr;
  Clear();
  type = thr.type;
  size = thr.size;
  m_pObj = thr.m_pObj;
  szName = szName.c_str();
  memcpy( szValue, thr.szValue, sizeof(szValue) );
  switch( locType )
  {
  default:
    ASSD(0);
    break;
  case VarFieldText:
    pMem = (BYTE*)szValue;
    break;
  case Ref2FieldVar:
    pMem = thr.pMem;
    break;
  case ConstInteger:
  case ConstFloat:
  case ConstBoolTF:
    pMem = (BYTE*)&int4;
    break;
  }
}

void KCalcVar::Clear()
{
  m_bAddrVar = false;
  type = evtHZ;
  pMem = NULL;
  size = 0;
  m_bAddrVar = false;
  int4 = 0;
  m_pObj = NULL;
}

void KCalcVar::operator = ( KCalcVar &thr )
{
  CopyFrom( thr );
}

bool KCalcVar::NotLinked()
{
  return NULL==pMem;
}

bool KCalcVar::Init()
{
  bool res = false;
  switch( locType )
  {
  default:
    ASSD(0);
    break;
  case Ref2FieldVar:
    res = true;
    break;
  case ConstInteger:
    res = true;
    int4 = atoi(szName);
    type = evtInt4;
    size = sizeof(int4);
    break;
  case VarFieldText:
    res = true;
    strcpy_s( szValue, szName );
    type = evtString;
    size = (BYTE)_countof(szValue);
    break;
  }
  return res;
}

KCalcVar::operator LPCSTR()
{
  ASSD( evtString==type && pMem );
  return (LPCSTR)pMem;
}

bool KCalcVar::GetBool()
{
  ASSD( evtBool==type && pMem );
  return *(bool*)pMem;
}

void KCalcVar::operator = ( bool v )
{
  bl = v;
  locType = ConstBoolTF;
  type = evtBool;
  m_bAddrVar = false;
  pMem = (BYTE*)&bl;
  size = sizeof(bl);
}

void KCalcVar::Set( short r )
{
  int2 = r;
  locType = ConstInteger;
  type = evtInt2;
  m_bAddrVar = false;
  pMem = (BYTE*)&int2;
  size = sizeof(int2);
}

void KCalcVar::Set( double r )
{
  dbl = r;
  locType = ConstFloat;
  type = evtDouble;
  m_bAddrVar = false;
  pMem = (BYTE*)&dbl;
  size = sizeof(dbl);
}

void KCalcVar::InitTo( BYTE *pAddr, eVarType Type, USHORT varSize )
{
  Clear();
  pMem = pAddr;
  type = Type;
  size = varSize;
  locType = Ref2FieldVar;
}

template<typename L, typename R>
bool KCalcVar::Compare( L l, R r, eToken_value cmd )
{
  switch( cmd )
  {
  default:
    ASSD( 0 );
    break;
  case etGT:
    return l > r;
  case etLT:
    return l < r;
  case etEQ:
    return l == r;
  case etNE:
    return l != r;
  case etGE:
    return l >= r;
  case etLE:
    return l <= r;
  }
  return false;
}

void KCalcVar::Compare( KCalcVar &res, KCalcVar &thr, eToken_value cmd )
{
  if( type==thr.type )
  {
    ASSD( pMem && thr.pMem );
    switch( type )
    {
    default:
      ASSD(0);
      break;
    case evtInt4:
      res = Compare( *(int*)pMem, *(int*)thr.pMem, cmd );
      break;
    }
  }
  else
  {
    ASSD( pMem && thr.pMem );
    switch( type )
    {
    default:
      ASSD(0);
      break;
    case evtDouble:
      switch( thr.type )
      {
      default:
        ASSD(0);
        break;
      case evtInt4:
        res = Compare( *(double*)pMem, *(int*)thr.pMem, cmd );
        break;
      }
      break;
    }
  }
}

void KCalcVar::AssignFrom( KCalcVar &thr )
{
  if( type==thr.type )
  {
    ASSD( pMem && thr.pMem );
    ASSD( size==thr.size );
    memcpy(pMem, thr.pMem, size);
  }
  else
  {
    ASSD( pMem && thr.pMem );
    switch( type )
    {
    default:
      ASSD(0);
      return;
    case evtInt2:
      switch( thr.type )
      {
      default:
        ASSD(0);
        break;
      case evtDouble:
        *(short*)pMem = *(double*)thr.pMem;
        break;
      case evtInt4:
        *(short*)pMem = *(int*)thr.pMem;
        break;
      }
      break;
    }
  }
  if( !m_pObj )
    return;
  if( szName.empty() )
    return;
  m_pObj->OnScriptAssgned( szName );
}

template<typename T, typename R>
T KCalcVar::Action( T l, R r, eToken_value cmd )
{
  T o = 0;
  switch( cmd )
  {
  default:
    ASSD(0);
    break;
  case etMINUS:
    o = l - r;
    break;
  case etPLUS:
    o = l + r;
    break;
  case etDIV:
    if( 0==r )
      break;
    o = l / r;
    break;
  case etMUL:
    o = l * r;
    break;
  }
  return o;
}

void KCalcVar::Action( KCalcVar &res, KCalcVar &thr, eToken_value cmd )
{
  ASSD( pMem && thr.pMem );
  switch( type )
  {
  default:
    ASSD(0);
    break;
  case evtDouble:
    switch( thr.type )
    {
    default:
      ASSD(0);
      break;
    case evtDouble:
      res.Set( Action( *(double*)pMem, *(double*)thr.pMem, cmd ) );
      break;
    case evtInt4:
      res.Set( Action( *(double*)pMem, *(int*)thr.pMem, cmd ) );
      break;
    }
    break;
  case evtInt2:
    switch( thr.type )
    {
    default:
      ASSD(0);
      break;
    case evtInt2:
      res.Set( Action( *(short*)pMem, *(short*)thr.pMem, cmd ) );
      break;
    }
    break;
  case evtInt4:
    switch( thr.type )
    {
    default:
      ASSD(0);
      break;
    case evtDouble:
      res.Set( (double)Action( *(int*)pMem, *(double*)thr.pMem, cmd ) );
      break;
    }
    break;
  }
}
