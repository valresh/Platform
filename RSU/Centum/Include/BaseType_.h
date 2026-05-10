#pragma once
#include <BaseType.h>

enum EValueTypeY
{
  // Для редактора
  enumValueColor =  8,//Цвет
  enumValueStyle =  9,//Стиль линии
  enumValueBrush = 10,//Стиль закраски
  enumValuePoint = 11,//Точка
  enumValueSize  = 12,//Размер
  enumValueRect  = 13,//Область
  enumValueAligH = 14,//Выравнивание по горизонтали
  enumValueAligV = 15,//Выравнивание по вертикали
  enumValueFont  = 16,//Фамилия шрифта
  enumPipeError  = 17,//при выполнении скриптов не получено значение с сервера
};
#pragma warning( disable : 444 ) //Отключаем предупреждение о невиртуальности
//
#ifdef YOKOW_EXPORTS
#define YOKOW_API _EXP
#else
#define YOKOW_API _IMP
#endif

struct YOKOW_API CAlarmBase : public CBase
{
  typedef BYTE tFlagUse_;
  BYTE btEHLN;// Уровень сигнализации (здесь не используется)
  DWORD ALRM;// Тип сигнализации     (enum MESS  )
  __int64 ALRMmask;//маска сигнализаций
  BYTE XALRM  ;//Alarm is in IOP or IOP- status.
  BYTE notACK;// 0- квитированный 1 - не квитированный
  BYTE setACK;// Оператор изъявил желание квитировать объект
  BYTE INTERLOCK;
  tFlagUse_ FlagUse; // 0x01 - Model, 0x02 - Yoko, 0x04 - Y-connect
  char szALRM[32];
  int sharpPV;
  enum
  {
    fuModel = 0x1,
    fuYoko = 0x2,
    fuYConnect = 0x4,
    fuNoModelConnected = 0x8,//0x08 - не произошло подключение к моделе
    fuNoInput = 0x10,//нет данных с IN
    fuHaveSameName = 0x20,//имеется объект такого типа с таким же именем
    fuY_Mask = fuYConnect | fuNoModelConnected | fuNoInput | fuHaveSameName,
  };
};

enum
{
  id_CentumSystem = 0x198,
  id_OneMnemo = 0x199,
  // Специфика модели
  #undef  DATATYPE
  #define DATATYPE( Id, Nomer, Text, Icon ) id_##Id = Nomer,
  // Специфика Yokogawa
  id_AcyMin = 0x0200,
  #undef   YOKO_TYPE
  #define  YOKO_TYPE( a, b, c ) id_##b = 0x##a,
  #include "./YokogawaType.hpp"
  id_AcyMax = 0x53DD,
};

inline bool IsAcy( UINT nType )
{
  return id_AcyMin <= nType && nType < id_AcyMax;
}
//
