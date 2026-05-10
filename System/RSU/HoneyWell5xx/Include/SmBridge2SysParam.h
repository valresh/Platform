#pragma once
#include <stdlib.h>

struct SSmParam
{
  char ParamName[64 * 4];
  void *pAddr;
  int Size;
  char Type;
};

class KSmBridge2SysParam
{
  SSmParam *m_Params;
  int m_Used, m_countParams;
public:
  KSmBridge2SysParam( SSmParam *pParams, int countParams )
    : m_Used(0)
    , m_countParams(countParams)
    , m_Params(pParams)
  {
  }
  int Count()
  {
    return m_Used;
  }
  SSmParam& Add( double &val, LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'D';
    return Y;
  }
  SSmParam& Add( int &val, LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'I';
    return Y;
  }
  SSmParam& Add( BYTE &val, LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'W';
    return Y;
  }
  SSmParam& Add( short &val, LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'H';
    return Y;
  }
  SSmParam& Add( float &val, LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'F';
    return Y;
  }
  SSmParam& Add( bool &val, LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'B';
    return Y;
  }
  template<size_t _S>
  SSmParam& Add( char (&val)[_S], LPCSTR pName )
  {
    SSmParam &Y = push( pName, val );
    Y.Type = 'S';
    Y.Size = _S;
    return Y;
  }
protected:
  template<typename T>
  SSmParam& push( LPCSTR pName, T &val )
  {
    if( m_Used>=m_countParams )
      throw "дальнейшая работа невозможна";
    SSmParam& H = m_Params[m_Used];
    ++m_Used;
    strcpy_s( H.ParamName, pName );
    H.pAddr = &val;
    H.Size = sizeof(val);
    return H;
  }
};
