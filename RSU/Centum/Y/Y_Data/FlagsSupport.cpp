#include "stdafx.h"
#include "Y_Data.h"
#include "ReadParms.h"
#include <rsuErr.h>

typedef struct tagIdNames
{
  int id;
  LPCSTR lpszName;
  __int64 mask;
  int yokoCode;
}SIdNames;

static const SIdNames slistAlarms[] =
{
#undef  EnumMESS
#define EnumMESS( Name, Number, Text, CodeYoko ) { Number, Text, __int64(1)<<Number, CodeYoko },
#include <AlarmMess_.hpp>
  { 0, "AON", 0, 0 },
  { 1, "AOF", 0, 0 },
};

static LPCSTR pSkipAlarms[]=
{
  "PERROR",
  "LOCK",
};


int GetAlrmNumber( LPCSTR pName )
{
  size_t l = strlen(pName);
  for( int n = 0; n < _countof(slistAlarms); n++ )
  {
    if( !strncmp( pName, slistAlarms[n].lpszName, l) )
    {
      if( 0==slistAlarms[n].lpszName[l] )
        return slistAlarms[n].id;
      if( '+'==slistAlarms[n].lpszName[l] )
        return slistAlarms[n].id;
    }
  }
  for( int i=0; i<_countof(pSkipAlarms); ++i )
  {
    if( !strcmp( pName, pSkipAlarms[i]) )
      return -1;
  }
  ASS( 0 );
  return -1;
}

__int64 GetAlrmMask( LPCSTR pName )
{
  size_t l = strlen(pName);
  for( int n = 0; n < _countof(slistAlarms); n++ )
  {
    if( !strncmp( pName, slistAlarms[n].lpszName, l) )
    {
      if( 0==slistAlarms[n].lpszName[l] )
        return slistAlarms[n].mask;
      if( '+'==slistAlarms[n].lpszName[l] )
        return slistAlarms[n].mask;
    }
  }
  ASS( 0 );
  return 0;
}

int GetAlrmYokoCode( LPCSTR pName )
{
  size_t l = strlen(pName);
  for( int n = 0; n < _countof(slistAlarms); n++ )
  {
    if( !strncmp( pName, slistAlarms[n].lpszName, l) )
    {
      if( 0==slistAlarms[n].lpszName[l] )
        return slistAlarms[n].yokoCode;
      if( '+'==slistAlarms[n].lpszName[l] )
        return slistAlarms[n].yokoCode;
    }
  }
  ASS( 0 );
  return 0;
}

#undef  EnumMESS
#define EnumMESS( Name, Number, Text, CodeYoko ) { Number, Text },

static const SIdNames slistModes[] =
{
  #include <FlagConstants.hpp>
  { SMode::OS,   "O/S" },
  { SMode::SEMI, "SEMI" },
  { SMode::AUT,  "AUT" },
  { SMode::MAN,  "MAN" },
  { SMode::CAS,  "CAS" },
  { SMode::PRD,  "PRD" },
  { SMode::RCAS, "RCAS" },
  { SMode::ROUT, "ROUT" },
};


int GetModeNumber( LPCSTR pName, CY_Base *pObj )
{
  for( int n = 0; n < _countof(slistModes); n++ )
  {
    if( !strcmp( pName, slistModes[n].lpszName) )
      return slistModes[n].id;
  }
  if( !strcmp(pName, "IMAN") )
    return 128;
  if( !strcmp(pName, "SEMI") )
    return 64;
  if( !strcmp(pName, "TRK") )
    return 256;
#pragma message("Переделать SMode.M IMAN\n")
  //ASS( 0 );
  return -1;
}

LPCSTR GetModeName( int val )
{
  for( int n = 0; n < _countof(slistModes); n++ )
  {
    if( val==slistModes[n].id )
      return slistModes[n].lpszName;
  }
  return NULL;
}

int GetEnumValue( LPCSTR pEName, LPCSTR pEVal, struct CY_Base *pObj )
{
  if( !pObj )
    return -1;
  int I = -1;
  for( int i=0; i< pObj->kClassVarInfo; i++ )
  {
    if( strcmp( pObj->ClassVarInfo[i].VarName, pEName ) )
      continue;
    if( pObj->ClassVarInfo[i].bEnum )
    {
      LPCSTR pV = pObj->ClassVarInfo[i].pszEnumVals;
      int n = 0;
      LPCSTR pArgs[20] = { NULL };
      pArgs[n++] = pV;
      LPCSTR pE = strchr(pV, ',');
      for( ; pE; pE = strchr(pE, ',') )
      {
        pE++;
        pArgs[n++] = pE;
        ASS( n < _countof(pArgs) );
      }
      size_t L = strlen(pEVal);
      for( int i=0; i<n; i++)
      {
        if( !strncmp( pArgs[i], pEVal, L) )
        {
          I = i;
          break;
        }
        LPCSTR pE = strchr(pArgs[i], ',');
        size_t lV = L;
        if( pE )
          lV = pE - pArgs[i];
        else
          lV = strlen( pArgs[i] );
        if( !strncmp( pArgs[i], pEVal, __min(L, lV) ) )
        {
          I = i;
          break;
        }
      }
      ASS( I!=-1 );
      break;
    }
  }
  return I;
}
