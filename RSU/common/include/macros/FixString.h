#pragma once
#include <rsuErr.h>
#include <stdlib.h>
#include <rsuStr.h>

template<size_t _S>
struct SFixString
{
  char text[_S];
  SFixString()
  {
    text[0] = 0;
  }
  SFixString( LPCSTR pszThr )
  {
    if( pszThr )
      strcpy( text, pszThr );
    else
      text[0] = 0;
  }
  void operator = ( LPCSTR pszThr )
  {
    if( pszThr )
      strcpy( text, pszThr );
    else
      text[0] = 0;
  }
  template<size_t _R>
  void operator = ( const SFixString<_R> &r )
  {
    strcpy( text, r.c_str() );
  }
  operator LPCSTR()
  {
    return text;
  }
  LPCSTR c_str() const
  {
    return text;
  }
  bool empty()
  {
    return (!text[0]) ? true : false;
  }
  void assign( LPCSTR pszSrc, size_t len )
  {
    strncpy_s( text, pszSrc, len );
    ASSD( len < (_S-1) );
    text[len] = 0;
  }
  size_t length() const
  {
    return strlen(text);
  }
  char& operator[]( int index )
  {
    ASSD( index>=0 && index<countof(text) );
    return text[index];
  }
  template<size_t _R>
  bool operator<( SFixString<_R> b) const
  {
    return _Kstricmp( text, b.text ) < 0;
  }
  template<size_t _R>
  bool operator==( SFixString<_R> b) const
  {
    return !_Kstricmp( text, b.text );
  }
  template<size_t _R>
  bool operator!=( SFixString<_R> b) const
  {
    return !operator==(b);
  }
  template<size_t _L, size_t _R>
  bool operator()( SFixString<_L> a, SFixString<_R> b) const
  {
    return _Kstricmp(a.text, b.text ) < 0;
  }
  bool operator==( LPCSTR thr) const
  {
    return !_Kstricmp( text, thr ) ? true : false;
  }
  bool operator!=( LPCSTR thr) const
  {
    return _Kstricmp( text, thr ) ? true : false;
  }
};

