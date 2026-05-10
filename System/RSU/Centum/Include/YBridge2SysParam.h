#pragma once
#include <stdlib.h>
#include <crossplatform.h>

#ifdef YBRIDGE2SYSPARAM_EXPORTS
#define YBRIDGE2SYSPARAM_API _EXP
#else
#define YBRIDGE2SYSPARAM_API _IMP
#endif


struct YBRIDGE2SYSPARAM_API SYParam
{
#ifdef _WIN32
  typedef void (*tExternWindowShow_)( HWND hParent, char type, int len, void *addr, int domain );
#endif

  struct SYParamStrVal
  {
    char szName[16*4];
    union
    {
      int iValue;
      BYTE eValue;
    };
  };

  char ParamName[64*4];

  LPCSTR pszName;
  void *pAddr;
  int Size;
  char Type;
  void *pDefValue;
  int domain;

  SYParam::SYParamStrVal* pextStaticVals;//должен указывать на static
  int extCount;
  bool bFullEqValue;//при false сравнение I идет по &

  //tExternWindowShow_ pCallExternWindowHandler;

  SYParam();
  void Clear();
  bool Out( LPSTR pszTxt, size_t size );
  template<size_t _S>
  bool Out( char (&Txt)[_S] )
  {
    return Out( Txt, _S );
  }
  void SetValue( LPCSTR pszValue );
  bool ChangeVal( double Delta );
};

class KYBridge2SysParam
{
  SYParam *m_yParams;
  int m_Used, m_countParams;
public:
  KYBridge2SysParam( SYParam *pyParams, int countParams ) 
    : m_Used(0)
    , m_countParams(countParams)
    , m_yParams(pyParams)
  {}
  int Count()
  {
    return m_Used;
  }
  SYParam& Add( double &val, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'D';
    return Y;
  }
  SYParam& Add( int &val, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'I';
    return Y;
  }
  SYParam& Add( unsigned int &val, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'I';
    return Y;
  }
  SYParam& Add( bool &val, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'B';
    return Y;
  }
  SYParam& Add( BYTE &val, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'W';
    return Y;
  }
  SYParam& Add( char &val, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'C';
    return Y;
  }
  SYParam& Add( LPSTR pszVal, LPCSTR pszName )
  {
    SYParam &Y = push( pszName, *pszVal );
    Y.Type = 'S';
    Y.Size = (int)strlen(pszVal);
    return Y;
  }
  SYParam& AddL( LPSTR pszVal, LPCSTR pszName, USHORT elSize )
  {
    SYParam &Y = push( pszName, *pszVal );
    Y.Type = 'S';
    Y.Size = (int)elSize;
    return Y;
  }
  /*template<size_t _S>//почему-то не вызывается
  SYParam& Add( char (&val)[_S], LPCSTR pszName )
  {
    SYParam &Y = push( pszName, val );
    Y.Type = 'S';
    Y.Size = _S;
    return Y;
  }*/
  //template<int _S>
  //SYParam& AddCallback( char (&val)[_S], LPCSTR pszName, SYParam::tExternWindowShow_ pFn )
  //{
  //  SYParam &Y = push( pszName, val );
  //  Y.Type = 'S';
  //  Y.Size = _S;
  //  Y.pCallExternWindowHandler = pFn;
  //  return Y;
  //}
  SYParam& Add( unsigned int &val, LPCSTR pszName, int C, SYParam::SYParamStrVal *pStaticVals )
  {
    SYParam &Y = push( pszName, val );
    Y.Type= 'I';
    Y.extCount = C;
    Y.pextStaticVals = pStaticVals;
    return Y;
  }
  SYParam& Add( char &val, LPCSTR pszName, int C, SYParam::SYParamStrVal *pStaticVals )
  {
    SYParam &Y = push( pszName, val );
    Y.Type= 'C';
    Y.extCount = C;
    Y.pextStaticVals = pStaticVals;
    return Y;
  }
  SYParam& Add( BYTE &val, LPCSTR pszName, int C, SYParam::SYParamStrVal *pStaticVals )
  {
    SYParam &Y = push( pszName, val );
    Y.Type= 'F';
    Y.extCount = C;
    Y.pextStaticVals = pStaticVals;
    return Y;
  }
protected:
  template<typename T>
  SYParam& push( LPCSTR pszName, T &val )
  {
    if( m_Used>=m_countParams )
      throw "дальнейшая работа невозможна";
    SYParam& Y = m_yParams[m_Used];
    ++m_Used;
    strncpy_s( Y.ParamName, pszName, _countof(Y.ParamName)-1 );
    Y.pszName = Y.ParamName;
    Y.pAddr = &val;
    Y.Size = sizeof(val);
    return Y;
  }
};

template<size_t _S>
void AddS( KYBridge2SysParam &params, char (&val)[_S], LPCSTR pszName )
{
  SYParam &Y = params.Add( val, pszName );
  Y.Size = _S;
}
