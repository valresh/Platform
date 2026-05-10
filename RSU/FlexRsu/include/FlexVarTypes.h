#pragma once
#include <cstdint>
#include <macros/DynString.h>
#ifndef FLEXRSUDEBUGBRIDGETOGUI_EXPORTS
#include <crossplatform.h>
#endif

typedef WORD tVarSizeType_;

enum eVarType
{
  evtHZ = 0,
  evtDigital32 = 1,
  evtAnalog32 = 2,
  evtChars = 3,
  evtDigital64 = 4,
  evtAnalog64 = 5,
  evtPrivConstString = 6,
};

#ifndef FLEXRSUDEBUGBRIDGETOGUI_EXPORTS
typedef int32_t tDigital32;
typedef int64_t tDigital64;
#else
typedef __int32 tDigital32;
typedef __int64 tDigital64;
#endif

typedef float   tAnalog32;
typedef char    tChars;
typedef double  tAnalog64;
typedef KDynString tPrivConstDynString;
