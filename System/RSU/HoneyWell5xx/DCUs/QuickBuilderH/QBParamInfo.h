#pragma once
#include "ParmVarInfo.h"
#include <macros/en2str.h>

void FillVal( char *pszBuf, size_t bufSize, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals );
void FillValues( tValEnums_ &vals, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals );
void SetValue( LPCSTR pszVal, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals );
