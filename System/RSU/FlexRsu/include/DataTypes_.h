#pragma once
#include <BaseType_.h>

struct SFlex : public CBase
{
  enum { TypeID = id_FLEX };
  DWORD TypeFlex;
  DWORD ObjSize;
  BYTE ObjectMem[1];
};


struct CFlexSystemA
{
  enum
  {
    STATUS_UNKNOWN = 0,
    STATUS_INIT    = 1,
    STATUS_START   = 2,
    STATUS_PAUSE   = 3,
  };
  bool bSound;
  BYTE StartStatus;// Состояние модели
  // Чтобы ARM-ы контролировали изменения в этих файлах
  DWORD nFlexAlarmXY;// Количество перебросов файла сигнализации
  DWORD nFlexAlarms0;// Количество записей в Alarms0.dat (nAlarmXY%2) == 0
  DWORD nFlexAlarms1;// Количество записей в Alarms1.dat (nAlarmXY%2) == 1
  DWORD nMsg2Operator;//Количество записей в Msg2UserFlex.dat
  __time64_t startTime;
  double dModelT;// Модельное время (сек)
  int FileFlexA()
  {
    return nFlexAlarmXY % 2;
  }
  __time64_t TimeB()// Время от начала модели (сек)
  {
    return (__time64_t)dModelT;
  }
  __time64_t TimeM()// Полное модельное время (сек)
  {
    return startTime + TimeB();
  }
};

struct CFlexSystem : public CBase, public CFlexSystemA
{
  enum { TypeID = id_FlexSystem };
  //static LPCSTR ClassName;
  int Init();
  int GetParams();
  int SaveState();
  int RestoreState( char * StrName );
};

#include <FlexDeclareShared.h>
struct SFlexFutureTrend : public CBase
{
  enum { TypeID = id_FlexFutureTrend };
#include <FlexBlocks/FutureTrend.h>
  static void BuildList();
};
