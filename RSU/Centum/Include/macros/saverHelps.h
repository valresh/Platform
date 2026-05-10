#pragma once
#include "../yISerializer.h"

template<typename T>
DWORD ReadWithTest( IYokoStateSer* pStateSer, T& out, T testVal)
{
  DWORD len = pStateSer->SimpleRead( out );
  ASSD( sizeof(out)==len );
  bool r = out==testVal;
  ASSD( r );
  if( !r )
    return r;
  return len;
}

template<typename T>
DWORD ReadTest( IYokoStateSer* pStateSer, T testVal )
{
  T out;
  DWORD len = pStateSer->SimpleRead( out );
  ASSD( sizeof(out)==len );
  bool r = out==testVal;
  ASSD( r );
  if( !r )
    return r;
  return len;
}

template<typename T>
DWORD Read( IYokoStateSer* pStateSer, T &out )
{
  DWORD len = pStateSer->SimpleRead( out );
  ASSD( sizeof(out)==len );
  return len;
}

template<size_t _S>
DWORD Read( IYokoStateSer* pStateSer, char (&szBuf)[_S] )
{
  return pStateSer->ReadLenStr( szBuf, _S );
}

template<typename T>
DWORD TryReadSized( IYokoStateSer* pStateSer, T &obj )
{
  DWORD size = 0;
  DWORD s = pStateSer->SimpleRead( size );
  ASSD( sizeof(size)==s );
  if( sizeof(size)!=s )
    return 0;
  ASSD( size==sizeof(obj) );
  if( size!=sizeof(obj) )
  {
    LONG l = sizeof(s);
    pStateSer->Shift( -l );
    return 0;
  }
  DWORD s2 = pStateSer->SimpleRead( &obj, sizeof(obj) );
  return s + s2;
}

template<typename T, DWORD _S>
DWORD TryReadSized( IYokoStateSer* pStateSer, T (&obj)[_S] )
{
  DWORD sizeSaved = 0;
  DWORD s = pStateSer->SimpleRead( sizeSaved );
  ASSD( sizeof(sizeSaved)==s );
  if( sizeof(sizeSaved)!=s )
    return 0;
  DWORD fullSize = sizeof(obj);
  DWORD s2 = 0;
  if( sizeSaved!=fullSize )
  {
    if( sizeSaved > fullSize )
    {
      s2 = pStateSer->SimpleRead( &obj, fullSize );
      pStateSer->Shift( sizeSaved - fullSize );
      s2 += (sizeSaved - fullSize);
    }
    else
    {
      ASSD(0);
    }
  }
  else
    s2 = pStateSer->SimpleRead( &obj, fullSize );
  return s + s2;
}

template<typename T, typename K>
int WriteFullStruct( IYokoStateSer* pStateSer, K key, LPCSTR pszName, T &obj )
{
  pStateSer->SimpleWrite( key );
  pStateSer->WriteStr( pszName );
  pStateSer->WriteSized( &obj, sizeof(obj) );
  return 0;
}

template<typename T, typename K, DWORD _S>
int WriteFullStruct( IYokoStateSer* pStateSer, K key, LPCSTR pszName, T (&obj)[_S] )
{
  pStateSer->SimpleWrite( key );
  pStateSer->WriteStr( pszName );
  pStateSer->WriteSized( &obj, sizeof(obj) );
  return 0;
}

template<typename T, typename K>
int ReadNamedFullStruct( IYokoStateSer* pStateSer, K testKey, LPCSTR pszName, T &obj )
{
  K key = (K)0;
  ASSD( ReadWithTest( pYokoStateSer, key, testKey ) );

  char buf[64] = { 0 };
  pStateSer->ReadLenStr( buf, _countof(buf) );
  ASSD( !strcmp(buf, pszName) );
  TryReadSized( pStateSer, obj );
  return 0;
}

template<typename T, typename K, DWORD _S>
int ReadNamedFullStruct( IYokoStateSer* pStateSer, K testKey, LPCSTR pszName, T (&obj)[_S] )
{
  K key = (K)0;
  ASSD( ReadWithTest( pYokoStateSer, key, testKey ) );

  char buf[64] = { 0 };
  pStateSer->ReadLenStr( buf, _countof(buf) );
  ASSD( !strcmp(buf, pszName) );
  TryReadSized( pStateSer, obj );
  return 0;
}
