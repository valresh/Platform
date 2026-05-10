#pragma once
#include "./RSUBaseType_.h"
#include <HoneywellStruct.h>

struct UTILSH5XX_API COneMnemoHoney : public CAlarmBase
{
  enum
  {
    COUNT_UNIT = 36,
  };
  enum { TypeID = id_OneMnemoHoney };
  static LPCSTR ClassName;
  static void BuildList();
  int Init();
  int GetParams( char * StrName );
  int SaveState();
  int RestoreState( char * StrName );
  bool state1[COUNT_UNIT];int count1[COUNT_UNIT];
  bool state2[COUNT_UNIT];int count2[COUNT_UNIT];
  bool state3[COUNT_UNIT];int count3[COUNT_UNIT];
};

struct UTILSH5XX_API CCBHSystemA
{
  bool bSound;
  // Чтобы ARM-ы контролировали изменения в этих файлах
  int nAlarmXY;// Количество перебросов файла сигнализации
  int nAlarms0;// Количество записей в Alarms0.dat (nAlarmXY%2) == 0
  int nAlarms1;// Количество записей в Alarms1.dat (nAlarmXY%2) == 1
  __time64_t timeB;// Время от начала модели (сек) - берется из CSystemB
  __time64_t timeM;// Полное модельное время (сек) - берется из CSystemB
  int        nStep;// - берется из CSystemB
  int FileA()
  {
    return nAlarmXY % 2;
  }
};

struct UTILSH5XX_API CCBHSystem : public CBase, public CCBHSystemA
{
  enum { TypeID = id_CBHSystem };
  static LPCSTR ClassName;
  int Init();
  int GetParams( char * StrName );
  int SaveState();
  int RestoreState( char * StrName );
  static void BuildList();
};

template<class T>
void tINIT_FROM_CBASE( T *pObj )
{
  memset( ((BYTE*)pObj) + sizeof(CBase), 0, sizeof( T ) - sizeof(CBase) );
  pObj->size = sizeof( T );
}

#undef  _INIT_FROM_CBASE
#define _INIT_FROM_CBASE \
  tINIT_FROM_CBASE( this );

struct UTILSH5XX_API COneAlmGrHoney : public CAlarmBase
{
  enum
  {
    COUNT_UNIT = 36,
  };
  enum { TypeID = id_OneAlmGrHoney };
  static LPCSTR ClassName;
  static void BuildList();
  int Init();
  int GetParams( char * StrName );
  int SaveState();
  int RestoreState( char * StrName );
  bool state1[COUNT_UNIT];int count1[COUNT_UNIT];
  bool state2[COUNT_UNIT];int count2[COUNT_UNIT];
  bool state3[COUNT_UNIT];int count3[COUNT_UNIT];
  char description[128 * 4];
};
