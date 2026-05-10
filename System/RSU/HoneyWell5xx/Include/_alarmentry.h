#pragma once
#include "Common.h"
//
// Структура заголовка файла системных сообщений
struct CEventHeader
  {
  DWORD dwName;   //Изменения имени MAP-файла
  DWORD dwCount; 
  DWORD dwReserved[14];
  };
//
// Структура заголовка файла сигнализаций
struct CAlarmHeader
  {
  DWORD dwName;   //Изменения имени MAP-файла
  DWORD dwCount; 
  DWORD dwReserved[14];
  };
//
// Структура заголовка файла сообщений оператору
struct CMessageHeader
  {
  DWORD dwName;   //Изменения имени MAP-файла
  DWORD dwCount; 
  DWORD dwReserved[14];
  };
//
  // Размер структуры д.б. кратен степени 2
  struct CMessageEntry
  {
  CMessageEntry()
    {
    memset( this, 0, sizeof( *this ) );
    clrBkg = 0x0000ff;//красный
    clrTxt = 0x00ffff;//жёлтый
    }
  int nTime ;//Время в секундах от старта модели
      int nFlags;//на будущее ( м.б. приоритет и т.п.)
  COLORREF clrBkg;
  COLORREF clrTxt;
	BYTE btKvit;// 1-квитировано 0-не квитировано
  BYTE btReserved[3];
  char szText[108 * 4];// Текст сообщения + Дополнительная строка сообщения
      char __padding[60]; // Выравнивание для степени 2 (512)
  };
  //
#undef  EnumMESS
#define EnumMESS( Name, Number, Text ) A_##Name = Number,
  enum EAlarms // в порядке убывания уровней
  {
#include "_AlarmMess.hpp"
  };
  //
  // Размер структуры д.б. кратен степени 2
  struct CAlarmEntry
  {
  CAlarmEntry(){ memset( this, 0, sizeof( *this ) ); }
  //
#undef  EnumSTATUS
#define EnumSTATUS( Name, Number, Text ) Name = Number,
  enum STATUS     //ПРИОРИТЕТ
    {
#include "_AlarmStatus.hpp"
    STATUS_XPEH
    };
  //
  UINT dwName;// Номер в Names.dat
  UINT dwType;// Тип объекта
  UINT dwLocl;// Используется только в Alarms.dll
  int  nTimeS;// Время от старта модели
  int  nTimeA;// Абсолютное время
  //
  BYTE btEHLN;// Уровень сигнализации (enum STATUS)
  BYTE btType;// Тип сигнализации     (enum Alarms)
  BYTE btOnOf;// 1-начало,     0-конец сигнализации
  BYTE btKvit;// 1-квитировано 0-не квитировано
  BYTE btUdal;// 1-удаление сигнализации из списка
  BYTE qbaN;//
  BYTE btRes2;//
  BYTE btRes3;//
  float fltValue;
      char pntFullName[64*4]; // utf-8
      char szValue[32*4]; // utf-8
      char __padding[96]; // Выравнивание для степени 2 (512)
  };
///////////////////////////////////////////////////////////////////////////////
