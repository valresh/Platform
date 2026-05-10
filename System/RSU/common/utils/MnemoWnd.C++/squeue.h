#pragma once
#include "../../../system/include/SoftGDI.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct SBuffer
  {
  unsigned char* szQueue;    // Указатель на выходной буфер очереди.
  unsigned long  nPtr;       // Текущее положение в буфере очереди.
  unsigned long  dwQueueMax; // Размер буфера очереди.
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct SParams
  {
  char szName[32];// Имя параметра
  char* buff;     // Очередь элементов
  int   dl;
  char szParam[10][128];
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct SQueue
  {
  ////
  class CPipeClient* m_pPipe;
  class CSocketClient* m_pSocket;
  //HWND hWnd;
  //
  SBuffer Stat;// Очередь статических  элементов
  SBuffer Dyna;// Очередь динамических элементов
  //
  SParams* arrFunc;
  int      cntFunc;
  int      maxFunc;
  //
  int      maxPribor;
  int      cntPribor;
  HPRIBOR* arrPribor;
  //
  //HBITMAP bmpStat;// Статические элементы
  //HBITMAP bmpFull;// Полная картинка
  //
  char szName [_MAX_FNAME];
  char szSxema[_MAX_FNAME];
  char szMnemoBar[_MAX_FNAME];
  //
  // Устанавливаемые параметры
  POINT  setShift ;
  FLOAT  setScale ;
  // Текущие параметры окна при выводе
  POINT  curShift ;
  FLOAT  curScale ;
  int    curWidth ;
  int    curHeight;
  //
  //SIZE szWnd;// Заданные параметры окна
  //
  DWORD m_dwMainTimer;
  int   m_nCodeWindow;
  bool  m_bMouseWheel;
  //
  int nLedTest;
  LPPOINT ptAlarm;
  
  };