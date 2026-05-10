#pragma once
#include <DataTypes_.h>
#include <FlexVarInfo.h>

struct S_DIO
{
  enum { TypeID = 1000 };
#include <FlexBlocks/DIO.h>
  static void BuildList();
};

struct S_AIO
{
  enum { TypeID = 1001 };
#include <FlexBlocks/AIO.h>
  static void BuildList();
};

struct S_A2oo3
{
  enum { TypeID = 1002 };
#include <FlexBlocks/A2oo3.h>
  static void BuildList();
};

struct S_PID
{
  enum { TypeID = 1004 };
#include <FlexBlocks/PID.h>
  static void BuildList();
};

struct S_OPCAI
{
  enum { TypeID = 1005 };
#include <FlexBlocks/OpcAIO.h>
  static void BuildList();
};

struct S_OPCAO
{
  enum { TypeID = 1006 };
#include <FlexBlocks/OpcAIO.h>
  static void BuildList();
};

struct S_OPCDI
{
  enum { TypeID = 1007 };
#include <FlexBlocks/OpcDIO.h>
  static void BuildList();
};

struct S_OPCDO
{
  enum { TypeID = 1008 };
#include <FlexBlocks/OpcDIO.h>
  static void BuildList();
};

struct S_ANATREND
{
  enum { TypeID = 1009 };
#include <FlexBlocks/AnaTrend.h>
  static void BuildList();
};

struct S_TCOREAI
{
  enum { TypeID = 1010 };
#include <FlexBlocks/TCoreAIO.h>
  static void BuildList();
};

struct S_TCOREAO
{
  enum { TypeID = 1011 };
#include <FlexBlocks/TCoreAIO.h>
  static void BuildList();
};

struct S_TCOREDI
{
  enum { TypeID = 1012 };
#include <FlexBlocks/TCoreDIO.h>
  static void BuildList();
};

struct S_TCOREDO
{
  enum { TypeID = 1013 };
#include <FlexBlocks/TCoreDIO.h>
  static void BuildList();
};

struct S_DIGALRM
{
  enum { TypeID = 1014 };
#include <FlexBlocks/DigAlrm.h>
  static void BuildList();
};

struct S_M2U
{
  enum { TypeID = 1015 };
#include <FlexBlocks/M2U.h>
  static void BuildList();
};

struct S_COT
{
  enum { TypeID = 1016 };
#include <FlexBlocks/COT.h>
  static void BuildList();
};

struct S_SYSTIME
{
  enum { TypeID = 1017 };
#include <FlexBlocks/SYSTIME.h>
  static void BuildList();
};

struct S_SYSDATE
{
  enum { TypeID = 1018 };
#include <FlexBlocks/SYSDATE.h>
  static void BuildList();
};

struct S_ANALRM
{
  enum { TypeID = 1019 };
#include <FlexBlocks/AnAlrm.h>
  static void BuildList();
};

struct S_PIDA
{
  enum { TypeID = 1020 };
#include <FlexBlocks/PIDA.h>
  static void BuildList();
};

struct S_MOTOR
{
  enum { TypeID = 1021 };
#include <FlexBlocks/MOTOR.h>
  static void BuildList();
};

struct S_VLV
{
  enum { TypeID = 1022 };
#include <FlexBlocks/VLV.h>
  static void BuildList();
};

struct S_EV
{
  enum { TypeID = 1023 };
#include <FlexBlocks/EV.h>
  static void BuildList();
};

struct S_ADDER
{
  enum { TypeID = 1024 };
#include <FlexBlocks/ADDER.h>
  static void BuildList();
};

struct S_TCOREMAI
{
  enum { TypeID = 1025 };
#include <FlexBlocks/TCoreMAIO.h>
  static void BuildList();
};

struct S_RAMP
{
  enum { TypeID = 1026 };
#include <FlexBlocks/RAMP.h>
  static void BuildList();
};

struct S_TXTSET
{
  enum { TypeID = 1027 };
#include <FlexBlocks/TXTSET.h>
  static void BuildList();
};

struct S_TMR
{
  enum { TypeID = 1028 };
#include <FlexBlocks/TMR.h>
  static void BuildList();
};

struct S_PidSKVO
{
  enum { TypeID = 1029 };
#include <FlexBlocks/PidSKVO.h>
  static void BuildList();
};

struct S_SCLR
{
  enum { TypeID = 1030 };
#include <FlexBlocks/SCLR.h>
  static void BuildList();
};
struct S_DCI16
{
  enum { TypeID = 1031 };
#include <FlexBlocks/DCI16.h>
  static void BuildList();
};
struct S_DCO16
{
  enum { TypeID = 1032 };
#include <FlexBlocks/DCO16.h>
  static void BuildList();
};


struct S_SimDI
{
  enum { TypeID = 1033 };
#include <FlexBlocks/SimDIO.h>
  static void BuildList();
};

struct S_SimDO
{
  enum { TypeID = 1034 };
#include <FlexBlocks/SimDIO.h>
  static void BuildList();
};

struct S_SimAI
{
  enum { TypeID = 1035 };
#include <FlexBlocks/SimAIO.h>
  static void BuildList();
};

struct S_SimAO
{
  enum { TypeID = 1036 };
#include <FlexBlocks/SimAIO.h>
  static void BuildList();
};

struct S_DAlrmGen
{
  enum { TypeID = 1037 };
#include <FlexBlocks/DAlrmGen.h>
  static void BuildList();
};

struct S_SuraAI
{
  enum { TypeID = 1038 };
#include <Sura/AI.h>
  static void BuildList();
};

//////////////////////////////////////////////////////////////////////////
#ifdef FLEXRSULIST_EXPORTS
#define FLEXRSULIST_API _EXP
#else
#define FLEXRSULIST_API _IMP
#endif

FLEXRSULIST_API SVarInfo* GetFieldInfo( DWORD eSharedType, LPCSTR pszFieldName );
FLEXRSULIST_API DWORD EnumFieldsOf( DWORD eSharedType, LPSTR *ppFieldNames, DWORD countIn, DWORD len4Name );
template<DWORD _S, DWORD _L>
DWORD EnumFieldsOf( DWORD eSharedType, char (&szFieldNames)[_S][_L] )
{
  LPSTR ppFieldNames[_S] = { NULL };
  for( DWORD i=0; i<_S; ++i )
  {
    ppFieldNames[i] = szFieldNames[i];
  }
  return EnumFieldsOf( eSharedType, ppFieldNames, _S, _L );
}
