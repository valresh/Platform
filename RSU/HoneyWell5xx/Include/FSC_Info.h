#pragma once
#include <BaseType.h>

struct SFormatHdr
{
  char name[48*4];
};

struct CValue
{
  EValueType eType;
  union { double dVal; int nVal; char cVal; bool bVal; float fVal; char sVal[8]; __int64 lVal; };
};

struct CFSC_Info
{
  enum
  {
    MAX_FSC_INFO = 15000,
  };

  char NAME[32*4];
  char DESC[128 * 4];
  char DESC2[32 * 4];
  char szPLCADDR[6][8 * 4];
  char szFLDNumber[8 * 4];
  int  nFLDNumber;
  char szFldName[32 * 4];
  char szPowerUpValue[20 * 4];
  char szPointType[8 * 4];
  char szDataType[8 * 4];
  char szBlockName[32 * 4];
  char szPointDetailDisplay[32 * 4];
  char szGroupDetailDisplay[32 * 4];
  char szAssociatedDisplay[32 * 4];
  char szStatesText[2][32 * 4];
  char szNormalState[32 * 4];
  char szBottomScale[32 * 4], szTopScale[32 * 4];
  char szEngineeringUnits[24 * 4];
  char szAlarmLimitHighHigh[32 * 4], szAlarmLimitHigh[32 * 4], szAlarmLimitLow[32 * 4], szAlarmLimitLowLow[32 * 4];

//
  enum eComAllocationType
  {
    catHZ,
    catInput,
    catOutput,
    catInOut,
  }ComAllocationType[6];
  int PLCADDR[6];
  char szMaster[6][16 * 4];
  char szSlave[6][16 * 4];

  int NUMB_FSC;
  int NUMB;
  enum eType { NoneType, COM, FLD, POLE, SYS, COUNT, CAB, FSC, Other, PNL, MOV, BNS, MCC/*, FFB*/ };
  eType Type;
  char szTypeLocation[4*2];
  enum eIO { NoneIO, In, Out };
  eIO IO;
  enum eDA { NoneDA, Discrete, Analog };
  eDA DA;
  enum ePointType
  {
    hz,
    M,
    DI,
    DO,
    T,
    AI,
    BI,
    BO,
    R,
    C,
    AO,
    MS,
  }PointType;

  CValue *pPlcComm;
};
