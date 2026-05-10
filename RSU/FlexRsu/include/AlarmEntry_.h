#pragma once
#include <crossplatform.h>

// Размер структуры д.б. кратен степени 2
#pragma pack(push, 1)
struct CAlarmEntry
{
  CAlarmEntry()
  {
    memset( this, 0, sizeof( *this ) );
  }
  __int64 nMomentAbs;//бсолютное время срабатывание сигнализации
  __int64 nAckMomentAbs;//абсолютное время квитирования сигнализации
  bool btOnOff;// 1-начало, 0-конец сигнализации
  bool bAcked;// 1 - квитирован
  DWORD ID_CLASS;
  DWORD ID_PNT_NAME;
  //18 байтов. Размер страницы 4K
  char szName[256];//[64];
  char szDescr[512];//[128];
  char szDescr1[512];//[128];
  char szAlrmType[256];//[64];
  char szValue[128];//[32];
  char szFileName[366-12];//4096-512*6-256*2-128-18=366-nAckMomentAbs(8)+sizeof(id)
  char szDescr2[512];//[128];
  char szReserve1[512];//[128];
  char szReserve2[512];//[128];
  char szReserve3[512];//[128];
  DWORD id;  
};
#pragma pack(pop)

// Структура заголовка файла сообщений оператору
struct CMessageHeader
{
  DWORD dwName;   //Изменения имени MAP-файла
  DWORD dwCount; 
  DWORD dwReserved[14];
};

struct CMessageEntry
{
  enum // Действия, в зависимости от nFlag
  {
    MSG_WND  = 1, // Вывести M2U сообщение в окно
  };
  CMessageEntry()
  {
    ZeroMemory( this, sizeof( *this ) );
    clrBkg = 0x0000ff;//красный
    clrTxt = 0x00ffff;//жёлтый
  }
  DWORD nFlag;
  COLORREF clrBkg;
  COLORREF clrTxt;
  bool bShowHide;//1- show, 0-hide
  __time64_t nMoment;// Полное модельное время (сек)
  char szTagName[64*4]; //utf-8
  DWORD nRecord;
  char szText[(36+128)*4];// Текст сообщения + Дополнительная строка сообщения //utf-8
  char __padding[80]; // Выравнивания для степени 2 (1024)
};
