#pragma once
//
// Структура заголовка файла сообщений оператору
struct CMessageHeader
  {
  DWORD dwName;   //Изменения имени MAP-файла
  DWORD dwCount; 
  DWORD dwReserved[14];
  };
// Размер структуры д.б. кратен степени 2
struct CMessageEntry
{
  struct SRecipe
  {
    char szRcpName[64];
    bool bComplete;
    char szBatchID[64];
  };

  CMessageEntry()
    {
    memset( this, 0, sizeof( *this ) );
    clrBkg = 0x0000ff;//красный
    clrTxt = 0x00ffff;//жёлтый
    }
  int nTime;//Время в секундах от старта модели
  int nFlag;//Всяко-разно
  enum // Действия, в зависимости от nFlag
  {
    MSG_BOX  = 0, // Вывести MessageBox
    MSG_WND  = 1, // Вывести %OG сообщение в окно
    CALL_WND = 2, // Вывести %RQ окно с номером btRQ
    DIALOGUE = 3, // Вывести диалог ввода значения переменной CValue
    RCP_LOG,      // запись о загрузке рецепта
  };
  COLORREF clrBkg;
  COLORREF clrTxt;
  BYTE btKvit;// 1-квитировано 0-не квитировано
  BYTE btRQ;
  BYTE btEquipment;
  BYTE btReserved;
  union
  {
    char szText[108+128];// Текст сообщения + Дополнительная строка сообщения
    SRecipe recipe;
  };
};
//
// Размер структуры д.б. кратен степени 2
struct CAlarmEntry
  {
  //
  CAlarmEntry()
    {
    memset( this, 0, sizeof( *this ) );
    }
  //
  #undef  EnumMESS
  #define EnumMESS( Name, Number, Text, CodeYoko ) Name = Number,
  enum MESS
    {
    #include "AlarmMess_.hpp"
    MESS_XPEH
    };
  //
  UINT nTagRf;// Номер в TagRef.rdb (используется только в модели)
  int  nTimeS;// Время от старта модели
  //
  BYTE btEHLN;// Уровень сигнализации (enum STATUS)
  BYTE btType;// Тип сигнализации     (enum Alarms)
  BYTE btOnOf;// 1-начало,     0-конец сигнализации
  BYTE btKwit;// 1-квитировано 0-не квитировано
  BYTE btUdal;// 1-удаление сигнализации из списка
  char szName[17];
  BYTE btAlarmLevel;//
  BYTE btEquipment;//
  };

enum
{
  eAlarmMagicStep = 50,//до этого шага всё квитировано и не пишется в журнал инструктора
};
