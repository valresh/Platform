#pragma once
#include <shemedata.h>
#include <DataTypes_.h>
#include <AStudDataTypes_.h>
#include <AStudBaseType_.h>
#include <AsTypes.h>

struct CFlex_W
{
  eVarType mType;
  tVarSizeType_ dataSize;
  union
  {
    tAnalog32 Analog;
    tDigital32 Discret;
    tChars String[1];
  };
};

struct CShFlex : public CShBase, public CFlex_W
{
  SH_BASE(CShFlex)
};

struct CShFlexSystem : public CShBase, public CFlexSystem
{
  SH_BASE(CShFlexSystem)
};

struct CShFlexFutureTrend : public CShBase, public SFlexFutureTrend
{
  SH_BASE(CShFlexFutureTrend)
};

struct SAsPoint_W
{
  AS_VARENUM mType;
  asUI2 dataSize;
  union
  {
    asBOOL  bVal;//bool
    asI2    sVal;//short
    asUI2   usVal;//word
    asI1    iVal;//char
    asUI1   uiVal;//byte
    asI4    lVal;//int
    asUI4   ulVal;//dword
    asR4    fltVal;//float
    asR8    dblVal;//double
    char    String[1];
  };
};

struct CShAsPoint : public CShBase, public SAsPoint_W
{
  enum { TypeID = id_AStudPoint };
  SH_BASE(CShAsPoint)
};

struct CShAStudSystem : public CShBase, public SAsAllSystems
{
  enum { TypeID = id_AStudAllSystems };
  SH_BASE(CShAStudSystem)
};
