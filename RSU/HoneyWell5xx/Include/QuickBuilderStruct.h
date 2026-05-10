#pragma once
#include <rsuErr.h>
#include <QbBaseType_.h>
#include <QB_Declare.h>
#include <QbBridge2SysParam.h>

struct SUserDef
{
  enum ELinkType
  {
    ltVariable = 0,
    ltConstant,
	ltCDA,
  };
  char paramName[64 * 4];
  ELinkType linkType;
  eVarType dataType;
  union
  {
    short i2;
    int   i4;
    float flt;
	double dbl;
    char chr[48 * 4];
  };
  USHORT GetSize()
  {
    switch( dataType )
    {
    case evtInt2:
      return sizeof(i2);
    case evtInt4:
      return sizeof(i4);
    case evtFloat:
      return sizeof(flt);
    case evtDouble:
      return sizeof(dbl);	  
    case evtString:
      return _countof(chr);
    }
    return 0;
  }
};

struct UTILSQB5XX_API W_QBANALOG_W
{
#include <QuickBuilder/Analog.h>
};
struct UTILSQB5XX_API W_QBANALOG_P
{
#include <QuickBuilder/Analog_P.h>
  SUserDef userDefined[64];
};
struct UTILSQB5XX_API W_QBANALOG : CAlarmBase, W_QBANALOG_P, W_QBANALOG_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_QBANALOG };
  static void BuildList();
  int GetParams( KQbBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSQB5XX_API W_QBSTATUS_W
{
#include <QuickBuilder/Status.h>
};
struct UTILSQB5XX_API W_QBSTATUS_P
{
#include <QuickBuilder/Status_P.h>
  SUserDef userDefined[64];
};
struct UTILSQB5XX_API W_QBSTATUS : CAlarmBase, W_QBSTATUS_P, W_QBSTATUS_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_QBSTATUS };
  static void BuildList();
  int GetParams( KQbBridge2SysParam &params );
};
//////////////////////////////////////////////////////////////////////////
struct UTILSQB5XX_API W_QBPSA_W
{
#include <QuickBuilder/Psa.h>
};
struct UTILSQB5XX_API W_QBPSA_P
{
#include <QuickBuilder/Psa_P.h>
  SUserDef userDefined[128];
};
struct UTILSQB5XX_API W_QBPSA : CAlarmBase, W_QBPSA_P, W_QBPSA_W
{
  static LPCSTR ClassName;
  enum { TypeID = id_QBPSA };
  static void BuildList();
  int GetParams( KQbBridge2SysParam &params );
};