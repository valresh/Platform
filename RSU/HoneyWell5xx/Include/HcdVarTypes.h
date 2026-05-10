#pragma once

typedef DWORD tHcdVarSizeType_;

enum eHcdVType
{
  ehtDisable = -1,
  ehtHZ = 0,
  ehtDigital32 = 1,
  ehtAnalog32 = 2,
  ehtBool = 3,
  ehtDynStr = 4,
  ehtEnum = 6,
};

typedef int     tDigital32;
typedef float   tAnalog32;
typedef BYTE    tBool;
typedef BYTE    tEnum;
