#pragma once
#include <stdlib.h>

struct SHParam
{
  char ParamName[64*4];
  void *pAddr;
  int Size;
  char Type;
};

class KHBridge2SysParam
{
  SHParam *m_Params;
  int m_Used, m_countParams;
public:
  KHBridge2SysParam( SHParam *pParams, int countParams )
    : m_Used(0)
    , m_countParams(countParams)
    , m_Params(pParams)
  {
  }
  int Count()
  {
    return m_Used;
  }
  SHParam& Add( double &val, LPCSTR pBlockName, LPCSTR pFieldName )
  {
    SHParam &Y = push( pBlockName, pFieldName, val );
    Y.Type = 'D';
    return Y;
  }
  SHParam& Add( int &val, LPCSTR pBlockName, LPCSTR pFieldName )
  {
    SHParam &Y = push( pBlockName, pFieldName, val );
    Y.Type = 'I';
    return Y;
  }
  SHParam& Add( BYTE &val, LPCSTR pBlockName, LPCSTR pFieldName )
  {
    SHParam &Y = push( pBlockName, pFieldName, val );
    Y.Type = 'W';
    return Y;
  }
protected:
  template<typename T>
  SHParam& push( LPCSTR pBlockName, LPCSTR pFieldName, T &val )
  {
    if( m_Used>=m_countParams )
      throw "дальнейшая работа невозможна";
    SHParam& H = m_Params[m_Used];
    ++m_Used;
    sprintf_s( H.ParamName, sizeof(H.ParamName), "%s.%s", pBlockName, pFieldName );
    H.pAddr = &val;
    H.Size = sizeof(val);
    return H;
  }
};
