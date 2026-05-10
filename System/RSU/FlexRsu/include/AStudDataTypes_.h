#pragma once
#include <CBase.h>
#include <AStudBaseType_.h>
#include <AsTypes.h>
#include <limits.h>

#ifdef ASTUDCOMMON_EXPORTS
#define ASTUDCOMMON_API _EXP
#else
#define ASTUDCOMMON_API _IMP
#endif

#pragma warning( push )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4200 )

//#define DESC_AP
//#define EU_AP

struct ASTUDCOMMON_API SAsPoint : public CBase
{
  typedef asUI2 tSizeType_;
  typedef AS_VARENUM tVarType_;//для совместимости с VARTYPE
  enum
  {
    eMaxBlockSize = USHRT_MAX,
  };
  enum { TypeID = id_AStudPoint };
  enum eDir
  {
    edNone = 0,
    edOut = 1,
    edIn  = 2,
    edInOut = 3,
  };
  enum eModbusExchange
  {
    embNo = 0,
    embHolding = 4,
    embInput = 8,
  };
  static LPCSTR ClassName;
  //SFixString<128> debugName;
  //SFixString<64> debugAppName;
  bool b4Arm;
  int flagAssignedFromArm;
  int nAsSource;
  bool bRestoredFromState;
  LPCSTR pszRsuName;
  BYTE *pRsuMem;
  tSizeType_ rsuSize;
  tVarType_ varType;
  tSizeType_ varSize;
  eDir dirExchange;
  eModbusExchange mbExchange;
  bool bLinkedWithRSU;
  bool bHistory;
  DWORD nAssignChangeCounter;
  asUI1 Quality;
#ifdef DESC_AP
  tSizeType_ descrLen;//длина строки без закрывающего 0, но память веделена с учетом него
#endif
  DWORD ackCookie[2];
#ifdef EU_AP
  char Unit[16];
#endif
  BYTE object[];
#ifdef DESC_AP
  LPSTR GetDescr();
#endif
};

struct ASTUDCOMMON_API SAsAllSystems : public CBase
{
  enum { TypeID = id_AStudAllSystems };
  static LPCSTR ClassName;
  bool bSound;
  int nAlarmXY;// Количество перебросов файла сигнализации
  int nAlarms0;// Количество записей в Alarms0.dat (nAlarmXY%2) == 0
  int nAlarms1;// Количество записей в Alarms1.dat (nAlarmXY%2) == 1
  DWORD nMsg2Operator;//Количество записей в Msg2UserNafta.dat
  __time64_t modelTime;//модельное время timeM();// Полное модельное время (сек)
  __time64_t startTime;
  FILETIME ftModelTime;
  FILETIME ftStartTime;
  DWORD asPointSize;
  UINT64 nStep64;
  int FileA()
  {
    return nAlarmXY % 2;
  }
};

#pragma warning( pop )
