#pragma once
#include <RsuX.h>

class KRsuF : public KRsuX
{
public:
  operator LPCSTR()
  {
    return (LPCSTR)m_PointInfo.pMem;
  }
  int GetType()
  {
    return m_PointInfo.type;
  }
  size_t GetLength()
  {
    return m_PointInfo.size;
  }
  void operator = ( LPSTR pszText )
  {
    if( !m_PointInfo.pMem || nRSUx::e_chars!=m_PointInfo.type )
      return;
    strncpy_s( (char*)m_PointInfo.pMem, m_PointInfo.size, pszText, ((size_t)-1));
  }
};
