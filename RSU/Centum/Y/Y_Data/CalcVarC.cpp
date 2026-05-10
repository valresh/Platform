#include "stdafx.h"
#include "CalcVar.h"
#include <rsuErr.h>
#include <locale.h>
#include "FlagsSupport.h"

struct SCondIvals
{
  LPCSTR pszName;
  int val;
};
struct SCondWvals
{
  LPCSTR pszName;
  BYTE val;
};

static SCondIvals CondIvals[] =
{
  {"OFF", 0},
  {"ON", 1},
  {"L",0},
  {"H",1},
  {"NON",1},
  {"AON",0},
  {"AOF",1},
};
 
template<typename T, typename S>
void AssignCondConst( T &cValue, LPCSTR pCondName, S *pS, const int C )
{
  for ( int i=0; i<C; i++ )
  {
    if( strcmp(pS[i].pszName, pCondName) )
      continue;
    cValue = pS[i].val;
    return;
  }
  //ASS( 0 );
}

void KCalcVar::AssignCondConstD( double &cValue, LPCSTR pCondName, LPCSTR pFieldName )
{
  cValue = DBL_MAX;
  //ASS( !strcmp(pFieldName,"PV") );
  AssignCondConst( cValue, pCondName, CondIvals, _countof(CondIvals) );
  if( DBL_MAX==cValue && IsFlt(pCondName) )
    cValue = atof( pCondName );
  if( DBL_MAX==cValue )
    cValue = GetModeNumber( pCondName, NULL );
}

void KCalcVar::AssignCondConstI( int &cValue, LPCSTR pCondName, LPCSTR pFieldName )
{
  cValue = -1;
  //ASS( !strcmp(pFieldName,"PV") );
  AssignCondConst( cValue, pCondName, CondIvals, _countof(CondIvals) );
  if( !strcmp(pCondName,"L") )
    assignConditionAsIs = 111;
  if( -1==cValue && IsInt(pCondName) )
    cValue = atoi( pCondName );
  if( -1==cValue )
  {
    cValue = GetModeNumber( pCondName, NULL );
    if( !_stricmp( pFieldName,"MODE") )
      assignConditionAsIs = 222;
  }
  if( -1==cValue && !strcmp(pFieldName,"ALRM") )
    cValue = GetAlrmNumber( pCondName );
}

void KCalcVar::AssignCondConstW( BYTE &cValue, LPCSTR pCondName, LPCSTR pFieldName )
{
  cValue = 255;
  if( !strcmp(pFieldName,"ALRM") )
    cValue = GetAlrmNumber( pCondName );
  if( !strcmp(pFieldName,"AOFS") )
    cValue = GetAlrmNumber( pCondName );
  else if( !strcmp(pFieldName,"MODE") )
    cValue =  GetModeNumber( pCondName, NULL );
  else if( !strcmp(pFieldName,"AFLS") )
    cValue =  GetModeNumber( pCondName, NULL );
  else if( m_Obj )
  {
    int e = GetEnumValue( pFieldName, pCondName, m_Obj );
    if( -1 != e )
      cValue = e;
  }
  ASSD( 255!=cValue );
}

bool KCalcVar::IsInt(const char* ptr)
{
  while ( *ptr == ' ' ) ptr++;
  if ( *ptr == '-' ) ptr++;
  else
    if ( *ptr == '+' ) ptr++;
  //
  bool bSymbol = false;
  while ( *ptr )
  {
    if ( *ptr == ' ' && bSymbol )
    {
      while ( *ptr == ' ' ) ptr++;
      return *ptr == 0;
    }
    if ( !('0' <= *ptr && *ptr <= '9') )
      return false;
    ptr++;
    bSymbol = true;
  }
  return bSymbol;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool KCalcVar::IsFlt(const char* ptr)
{
  //static lconv* conv = localeconv();
  struct lconv* conv = localeconv();
  char dp = conv->decimal_point[0];
  if ( dp != '.' )
  {
    const char* p = strchr( ptr, '.' );
    if ( p != NULL ) dp = '.';
  }
  //
  while ( *ptr == ' ' ) ptr++;
  if ( *ptr == '-' ) ptr++;
  else
    if ( *ptr == '+' ) ptr++;
  //
  bool bFloat = false;
  bool bSymbol = false;
  while ( *ptr )
  {
    if ( *ptr == ' ' && bSymbol )
    {
      while ( *ptr == ' ' ) ptr++;
      return *ptr == 0;
    }
    //
    if ( ('e' == *ptr || *ptr == 'E') && bSymbol )
    {
      ptr++;
      if ( *ptr == 0  ) return false;
      if ( *ptr == 32 ) return false;
      return IsInt(ptr);
    }
    //
    if ( *ptr == dp )
    {
      if ( bFloat )
        return false;
      bFloat = true;
      ptr++;
      continue;
    }
    //
    if ( !('0' <= *ptr && *ptr <= '9') )
      return false;
    bSymbol = true;
    ptr++;
  }
  return bSymbol;
}
