#pragma once
#include <crossplatform.h>

#ifdef FLEXRSUCORE_EXPORTS
#define FLEXRSUCORE_API _EXP
#else
#define FLEXRSUCORE_API _IMP
#endif

class FLEXRSUCORE_API KDynString
{
  LPSTR m_pString;
private:
  void AssignString( LPCSTR pszSrc );
  void Test4Null();
public:
  KDynString() : m_pString( NULL )
  {
  }
  KDynString( LPCSTR pszThr )
  {
    AssignString( pszThr );
  }
  void operator = ( LPCSTR pszThr )
  {
    AssignString( pszThr );
  }
  operator LPCSTR()
  {
    Test4Null();
    return m_pString;
  }
  bool empty()
  {
    return !m_pString || !(*m_pString);
  }
  size_t length() const
  {
    if( !m_pString )
      return 0;
    return strlen(m_pString);
  }
};

