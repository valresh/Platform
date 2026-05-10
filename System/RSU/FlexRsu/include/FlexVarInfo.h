#pragma once
#include <BaseType_.h>

typedef DWORD tShiftType_;
struct SVarInfo
{
  typedef BYTE tFlagType_;
  enum eFlags : tFlagType_
  {
    efEmpty               = 0,
    efEnum                = 0x01,
    efSaveInState         = 0x02,//сохранять в состоянии, всегда
  };

  LPCSTR pszName;
  eVarType evtType;
  tShiftType_ shift;
  tVarSizeType_ size;
  tFlagType_ flags;//eFlags
};
