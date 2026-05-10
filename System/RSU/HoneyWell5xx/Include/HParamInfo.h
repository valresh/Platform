#pragma once
#include <map>
#include <string>
#include <ParmVarInfo.h>
#include <macros/en2str.h>
#include <IControlBuilder.h>


#include <QtCore/QtGlobal>
#ifdef CONTROLBUILDERH5XXDEBUG_EXPORTS
#define CONTROLBUILDERH5XXDEBUG_API Q_DECL_EXPORT
#else
#define CONTROLBUILDERH5XXDEBUG_API Q_DECL_IMPORT
#endif


void CONTROLBUILDERH5XXDEBUG_API FillVal( char *pszBuf, size_t bufSize, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals );
void CONTROLBUILDERH5XXDEBUG_API FillValues( tValEnums_ &vals, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals );
bool CONTROLBUILDERH5XXDEBUG_API SetValue( LPCSTR pszVal, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals );

class CONTROLBUILDERH5XXDEBUG_API KHParamInfo
{
protected:
  BYTE *Addr;
  eVarType Type;
  USHORT stringVarSize;
  LPCSTR pszEnumVals;
  BYTE flags;
public:
  LPCSTR VarName;
public:
  KHParamInfo();
  KHParamInfo(BYTE *pAddr, eVarType nType,  USHORT uStrLen, LPCSTR szEnumVals);
  void Init( BYTE *pAddObj, SVarInfo &var);
  void ManualInit( struct IControlBuilder *pcb, LPCSTR pBlock, LPCSTR pField );
  eVarType GetType()
  {
    return Type;
  }
  BYTE *GetAddr()
  {
	  return Addr;
  }
  USHORT GetStringSize()
  {
	  return stringVarSize;
  }
  LPCSTR GetEnumValues()
  {
	  return pszEnumVals;
  }
  BYTE GetFlags()
  {
	  return flags;
  }
  template<size_t _S>
  void FillVal( char (&buf)[_S] )
  {
    ::FillVal( buf, _S, Type, Addr, stringVarSize, pszEnumVals );
  }
  void FillValues( tValEnums_ &vals )
  {
    ::FillValues( vals, Type, Addr, stringVarSize, pszEnumVals );
  }
  bool SetValue( LPCSTR pszVal )
  {
    return ::SetValue( pszVal, Type, Addr, stringVarSize, pszEnumVals );
  }
};
