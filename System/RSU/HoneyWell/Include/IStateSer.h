#pragma once
#include <stdlib.h>
#include <crossplatform.h>
#include <algorithm>

struct IStateSer
{  
  template<class T> void Write( const T &obj )
  {
    WriteImpl( &obj, sizeof(obj) );
  }

  template<class T> bool Read( T &obj )
  {
    return ReadImpl( &obj, sizeof(obj) );
  }
  template<DWORD _S> bool Read( char (&str)[_S] )
  {
    return Read( str, _S );
  }

  virtual void Write( LPCSTR pStr )
  {
    DWORD l = (DWORD)strlen( pStr )+1;
    Write( l );
    WriteImpl( pStr, l );
  }
  virtual bool Read( char *pszBuf, DWORD size )
  {
    return ReadImpl( pszBuf, size );
  }
  virtual bool Read( DWORD objSize, void *pObj )
  {
    return ReadImpl( pObj, objSize );
  }

  virtual bool NeedConvertUTF() = 0;
  virtual void WriteImpl( const void *pObj, DWORD objSize ) = 0;
  virtual bool ReadImpl( void *pObj, DWORD objSize ) = 0;
  virtual bool ReadImpl( char *pBuf, DWORD s )
  {
    DWORD l = 0;
    if( !Read( l ) )
      return false;
    //ASS( l<=s );
    l = std::min<DWORD>( l, s );
    if( !ReadImpl( (void*)pBuf, l ) )
      return false;
    if( l > 0 )
      pBuf[l-1] = 0;
    return true;
  }
};