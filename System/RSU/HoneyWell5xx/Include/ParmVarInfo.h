#pragma once
#include <RSUBaseType_.h>

struct SVarInfo // Все переменные
{
  enum eFlags : BYTE
  {
    efEnum  = 0x1,
    efVar   = 0x2,
    efParam = 0x4,
    efMaskType = 0x6,
  };
  DWORD sVar;//offset
  LPCSTR VarName;
  eVarType Type;
  BYTE flags;//иначе это поле состояния
  USHORT varSize;
  LPCSTR pszEnumVals;
  SVarInfo()
  {
    memset ( this, 0, sizeof( *this ));
  }
};

int CompVarInfo( const void * pP1, const void * pP2 );
