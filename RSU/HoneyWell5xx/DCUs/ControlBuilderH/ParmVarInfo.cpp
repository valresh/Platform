#include <rsuErr.h>
#include "ParmVarInfo.h"
#include <stdlib.h>

LPCSTR firstDig( LPCSTR pStr, size_t &len )
{
  len = 0;
  LPCSTR pStart = pStr;
  if( !pStr )
    return NULL;
  for( pStr; *pStr; ++pStr )
  {
    if( !isascii(*pStr) )
      continue;
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

    if( !isascii(*pStr) || !isdigit(*pStr) )
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
    res = _strnicmp( pszP1, pszP2, l1 );
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
  return CompVarInfo( pI1->VarName, pI2->VarName );
}
