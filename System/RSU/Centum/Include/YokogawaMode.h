#pragma once

struct SMode
{
  typedef unsigned int tMode;
  enum
  {
    OS            = 0x80000000,
    SEMI          = 0x01000000,
    MAN           = 0x00800000,
    AUT           = 0x00400000,
    CAS           = 0x00200000,
    PRD           = 0x00100000,
    RCAS          = 0x00080000,
    ROUT          = 0x00040000,
    _MODE_MASK    = 0x81fc0000,
    MD_IMAN       = 0x08000000,//в будующем надо поля заменить на биты
    MD_TRK        = 0x04000000,
  };
  tMode M;
  bool TRK;
  bool IMAN;
};
