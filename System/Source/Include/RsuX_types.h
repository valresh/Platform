#pragma once

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define RSUX_API Q_DECL_EXPORT
#else
#define RSUX_API Q_DECL_IMPORT
#endif

namespace nRSUx
{
  enum EParamType
  {
    e_byte = -19,
    e_dword = -18,
    e_RS24_SINT_STRING = -17,
    e_LINT = -16,
    e_word = -15,
    e_chars = -14,
    e_bool = -13,
    e_STRING = -12,
    e_SINT = -11,
    e_INT = -10,
    e_DINT = -9,
    e_BOOL = -8,
    e_REAL = -7,
    e_short = -6,
    e_char = -5,
    e_int = -4,
    e_double = -3,
    e_float = -2,
    e_str = -1,
    e_unknown,
  };

  struct RSUX_API SParamInfo
  {
    SParamInfo() 
      : offset(0)
      , size(0)
      , type( e_unknown )
      , bitOffset(-1)
      , pMem(NULL)
      , pszType(NULL)
      , pszParamName(NULL)
      , bPriorityOnCBase( 0 )
      , bSwapDoubleData( 0 )
    {}
    size_t offset;
    size_t size;
    int type;
    int bitOffset;
    BYTE *pMem;
    LPCSTR pszType;
    LPCSTR pszParamName;
    WORD bPriorityOnCBase;
    WORD bSwapDoubleData;
  };

RSUX_API void operator << ( SParamInfo &parm, double* pD );
RSUX_API void operator << ( SParamInfo &parm, float* pD );
RSUX_API void operator << ( SParamInfo &parm, int* pI );
RSUX_API void operator << ( SParamInfo &parm, bool* pI );
RSUX_API void operator << ( SParamInfo &parm, short* pI );
RSUX_API void operator << ( SParamInfo &parm, BYTE* pI );
};
