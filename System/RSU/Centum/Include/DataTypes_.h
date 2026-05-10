#pragma once

#include "./BaseType_.h"
#include <YokogawaStruct.h>

struct YOKOW_API COneMnemo : public CAlarmBase
  STD_DEFINE enum { TypeID = id_OneMnemo };
  int nCountAlarm;
  int cTagAlarms;//кол-во тэгов на текущей мнемосхеме в сигнализациях
  int cTagAlarmsNoAck;//  -//-                   не квитированные
  int cChildMnemoAlarms;//кол-во нижележащих мнемосхем  в сигнализациях
  int cChildMnemoAlarmsNoAck;//  -//-                   не квитированные
};

struct YOKOW_API CCentumSystemA
{
  bool bSound;
  // Чтобы ARM-ы контролировали изменения в этих файлах
  int nAlarmXY;// Количество перебросов файла сигнализации
  int nAlarms0;// Количество записей в Alarms0.dat (nAlarmXY%2) == 0
  int nAlarms1;// Количество записей в Alarms1.dat (nAlarmXY%2) == 1
  int nMsgToOperator;// Количество записей в MsgToOperatorY.dat
  bool bYinCalc;
  //берётся из модели для передачи в самостоятельный сервер
  double dtAsIs;
  __time64_t modelTime;//модельное время timeM();// Полное модельное время (сек) - берется из CSystemB
  __time64_t timeB;// Время от начала модели (сек) - берется из CSystemB
  int        nStep;// - берется из CSystemB
  int FileA()
  {
    return nAlarmXY % 2;
  }
};

struct YOKOW_API CCentumSystem : public CBase, public CCentumSystemA
  STD_DEFINE enum { TypeID = id_CentumSystem };
  int domains[10];
  DWORD mainLogicVer;
};
