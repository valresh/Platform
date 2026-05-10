#include "stdafx.h"
#define EXT_PARAM
#include "Extensions.h"


//////////////////////////////////////////////////////////////////
#if 0
CParamExtension::CParamExtension( char * _Group )
  {
  NumbExt = -1;
  Group = _Group;
  CLEAR(Names)
  WasRead = false;
  kNames = GetIntList( Group, 32, Names );
  }
//
bool CParamExtension::SetValue ( const char * Value )
  {
  int L = lstrlen ( Value );
  if ( L == 0 )
    return false;
  for ( int n = 0; n < kNames; n++ )
    {
    if ( strncmp ( Names[n], Value, L ) == 0 )
      {
      NumbExt = n;
      return true;
      }
    }
  return false;
  }

void CParamExtension::ChangeVal( double Delta )
  {
  if ( Delta < 1. && NumbExt > 0 )
    {
    NumbExt--;
    return;
    }
  if ( Delta > 1. && NumbExt < kNames - 1 )
    {
    NumbExt++;
    return;
    }
  }

void CParamExtension::Out ( char Txt[256] )
  {
  if ( NumbExt < 0 )
    {
    lstrcpy ( Txt, "НЕ ЗАДАНА" );
    return;
    }
  if ( NumbExt >= kNames )
    {
    lstrcpy ( Txt, "ОШИБКА" );
    return;
    }
  lstrcpy ( Txt, Names[NumbExt] );
  }

bool CParamExtension::IsWrite()
  {
  return true;
  }

int CParamExtension::Len()
  {
  if ( NumbExt < 0 )
    return 0;
  if ( NumbExt >= kNames )
    return 0;
  return lstrlen ( Names[NumbExt] ) + 1;
  }

void * CParamExtension::Data()
  {
  if ( NumbExt < 0 )
    return NULL;
  if ( NumbExt >= kNames )
    return NULL;
  return Names[NumbExt];
  }

bool CParamExtension::SetData( int Len, const void * pData )
  {
  char * Name = (char*)pData;
  NumbExt = -1;
  for ( int n = 0; n < kNames; n++ )
    {
    if ( lstrcmp ( Names[n], Name ) == 0 )
      {
      WasRead = true;
      NumbExt = n;
      return true;
      }
    }
  return false;
  }

bool CParamExtension::SetOld ( int OldNumbExt )
  {
  if ( WasRead )
    return false;
  NumbExt = OldNumbExt;
  return true;
  }

void * GetExt( char * Group, char * Interface );

IExtension * CParamExtension::GetExt( )
  {
  if ( NumbExt < 0 || NumbExt >= kNames )
    return NULL;
  return (IExtension*)::GetExt( Group, Names[NumbExt] );
  }
#endif