#pragma once
#include <CBase.h>
#include <crossplatform.h>

#ifdef UTILSHCD_EXPORTS
#define UTILSHCD_API _EXP
#else
#define UTILSHCD_API _IMP
#endif

struct UTILSHCD_API CHcdBase : CBase
{
  BYTE btDummy[8];
  int iDummy;
};

//
enum
{
  id_HCDSystem = 40021,
  id_HCDAcyMin = 40022,
  // Специфика
#undef   HCD_TYPE
#define  HCD_TYPE( a, b, c ) id_##b = a,
#include "./HCDType.hpp"
  id_HCDAcyMax = 40025,
};

inline bool IsHCDAcy( UINT nType )
{
  return id_HCDAcyMin <= nType && nType <= id_HCDAcyMax;
}
