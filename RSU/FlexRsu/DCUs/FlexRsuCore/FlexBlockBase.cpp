#include <crossplatform.h>
#include "FlexBlockBase.h"
#include <FlexRsuConnect.h>
#include <rsuErr.h>
#include <macros/AutoRestore.h>
#include <crosslocale.h>

double _atof( LPCSTR _String )
{
  LPSTR p = strchr( (LPSTR)_String, ',' );
  TAutoRestore<char> r;
  if( p )
  {
    r.Set( *p );
    *p = '.';
  }

  return cross::locale::safe_atof( _String );
}

LPCSTR firstDig( LPCSTR pStr, size_t &len )
{
  len = 0;
  LPCSTR pStart = pStr;
  if( !pStr )
    return NULL;
  for( pStr; *pStr; ++pStr )
  {
    if( isdigit(*pStr) )
    {
      len = pStr - pStart;
      return pStr;
    }
  }
  return NULL;
}

LPCSTR firstNoDig( LPCSTR pStr, size_t &len )
{
  len = 0;
  LPCSTR pStart = pStr;
  if( !pStr )
    return NULL;
  for( pStr; *pStr; ++pStr )
  {
    if( !isdigit(*pStr) )
    {
      len = pStr - pStart;
      return pStr;
    }
  }
  return NULL;
}

int CompVarInfo( LPCSTR pszP1, LPCSTR pszP2 )
{
  size_t l1, l2;
  LPCSTR pDig1 = firstDig( pszP1, l1 );
  LPCSTR pDig2 = firstDig( pszP2, l2 );
  if( !pDig1 || !pDig2 || (l1!=l2))
    return _strcmpi( pszP1, pszP2 );
  int res = 0;
  if( l1==l2 )
  {
    res = strncmp ( pszP1, pszP2, l1 );
    if( res )
      return _strcmpi( pszP1, pszP2 );
  }
  int v1 = atoi(pDig1);
  int v2 = atoi(pDig2);
  if( v1 < v2 )
    res = -1;
  else if( v1 > v2 )
    res = 1;
  else
  {
    pDig1 = firstNoDig( pszP1+l1, l1 );
    pDig2 = firstNoDig( pszP2+l2, l2 );
    if( pDig1 && pDig2 )
      return CompVarInfo( pDig1, pDig2 );
    if( !pDig1 && !pDig2 )
      return _strcmpi( pszP1, pszP2 );
    if( !pDig1 )
      return _strcmpi( "", pDig2 );
    return _strcmpi( pDig1, "" );
  }
  return res;
}

int CompVarInfo( const void * pP1, const void * pP2 )
{
  SVarInfo * pI1 = (SVarInfo*)pP1;
  SVarInfo * pI2 = (SVarInfo*)pP2;
  return CompVarInfo( pI1->pszName, pI2->pszName );
}

KFlexConnection** CloneConnections( DWORD &nCon, KFlexConnection **ppSrc, DWORD nSrc )
{
  nCon = nSrc;
  if( !nSrc )
    return NULL;
  KFlexConnection **ppN = (KFlexConnection**)pFlexMemoryAllocator( sizeof(KFlexConnection*) * nSrc );
  for( DWORD i=0; i<nSrc; ++i )
  {
    ppN[i] = ppSrc[i];
  }
  return ppN;
}

KFlexBlockBase::KFlexBlockBase()
{
}

void *KFlexBlockBase::operator new( size_t size )
{
  return pFlexMemoryAllocator( (DWORD)size ); 
}

void KFlexBlockBase::OnCreated()
{
  ASSD( !"не может быть, что нет полей");
}

void KFlexBlockBase::OnAttributesSetted()
{
}

void KFlexBlockBase::OnProjectReady( IProjectContainer *pCont )
{
}

void KFlexBlockBase::AssignValue( LPCSTR pszName, LPCSTR pszValue )
{
  eVarType type = evtHZ;
  tVarSizeType_ varSize = 0;
  BYTE *pMem = GetField( pszName, &type, &varSize );
  ASSD( pMem );
  if( !pMem )
    return;
  switch( type )
  {
  case evtDigital32:
    ASS( sizeof(tDigital32)==varSize );
    *(tDigital32*)pMem = atoi(pszValue);
    break;
  case evtAnalog32:
    ASS( sizeof(tAnalog32)==varSize );
    *(tAnalog32*)pMem = (tAnalog32)_atof(pszValue);
    break;
  case evtChars:
    strcpy_s( (tChars*)pMem, varSize, pszValue );
    break;
  case evtAnalog64:
    ASS( sizeof(tAnalog64)==varSize );
    *(tAnalog64*)pMem = (tAnalog64)_atof(pszValue);
    break;
  case evtPrivConstString:
    ASS( sizeof(tPrivConstDynString)==varSize );
    *(tPrivConstDynString*)pMem = pszValue;
    break;
  default:
    ASS(0);
    break;
  }
}

BYTE* KFlexBlockBase::GetField( LPCSTR pszName, eVarType *pType /*= NULL*/, tVarSizeType_ *pVarSize /*= NULL*/ )
{
  BYTE *pMem = NULL;
  if( m_pPrivateObject && m_kPrivateVarsInfo )
  {
    SVarInfo Key;
    Key.pszName = pszName;
    SVarInfo* pI = (SVarInfo*)bsearch( &Key, m_pPrivateVarsInfo, m_kPrivateVarsInfo, sizeof( SVarInfo ), CompVarInfo );
    pMem = FillVarInfoBy( pI, m_pPrivateObject, pType, pVarSize );
    if( pMem )
      return pMem;
  }
  if( m_pSharedObject && m_kSharedVarsInfo )
  {
    SVarInfo Key;
    Key.pszName = pszName;
    SVarInfo* pI = (SVarInfo*)bsearch( &Key, m_pSharedVarsInfo, m_kSharedVarsInfo, sizeof( SVarInfo ), CompVarInfo );
    pMem = FillVarInfoBy( pI, m_pSharedObject, pType, pVarSize );
  }
  return pMem;
}

BYTE* KFlexBlockBase::FillVarInfoBy( SVarInfo* pI, BYTE *pObj, eVarType *pType /*= NULL*/, tVarSizeType_ *pVarSize /*= NULL*/ )
{
  BYTE *pMem = NULL;
  if( !pI )
    return pMem;
  pMem = pObj + pI->shift;
  if( pType )
    *pType = pI->evtType;
  if( pVarSize )
    *pVarSize = pI->size;
  return pMem;
}

void KFlexBlockBase::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  ASSD( !"кто-то не получит связь" );
}

KFlexBlockBase::SReturn KFlexBlockBase::StepCalc( SCallParams &params )
{
  SReturn sr;
  return sr;
}

BYTE* KFlexBlockBase::EnumVars( DWORD &pos, SVarInfo &var )
{
  ZeroMemory( &var, sizeof(var) );
  WORD privShared = HIWORD(pos);
  WORD index = LOWORD(pos);

  if( privShared > 1 )
  {
    pos = MAKELONG( index, privShared );
    return NULL;
  }

  SVarInfo * pVarsInfo = NULL;
  DWORD count = 0;

  if( 0==privShared )
  {
    pVarsInfo = m_pPrivateVarsInfo;
    count = m_kPrivateVarsInfo;
  }
  else if( 1==privShared )
  {
    pVarsInfo = m_pSharedVarsInfo;
    count = m_kSharedVarsInfo;
  }

  if( !pVarsInfo || index >= count )
  {
    ++privShared;
    if( privShared > 1 )
    {
      pos = MAKELONG( index, privShared );
      return NULL;
    }
    index = 0;
    pVarsInfo = privShared ? m_pSharedVarsInfo : m_pPrivateVarsInfo;
    count = privShared ? m_kSharedVarsInfo : m_kPrivateVarsInfo;
  }

  BYTE *pMem = NULL;
  if( !pVarsInfo || index >= count )
    ++privShared;
  else
  {
    memcpy( &var, &pVarsInfo[index], sizeof(var) );
    pMem = GetField( var.pszName );
    ++index;
  }

  pos = MAKELONG( index, privShared );

  return pMem;
}

void KFlexBlockBase::OnAfterRestoreState()
{
}
