#pragma once
////
//#include "Lang.h"
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define _MAX_POLY 8
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define TYPE_NULL 0 //Пустой объект (признак конца очереди)
struct Queue_Null
{
  BYTE btType;
} ;
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define TYPE_LIBRARY   1        // Моделеспецифичные объекты
//typedef struct : public SPROLOZ // обрабатываются в своей DLL-ке
//{
//} Queue_Library;
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define TYPE_MACROS   2
struct Queue_Macros : public SPROLOG
{
  int nParams;
  SBuffer Buffer;
};
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define TYPE_UNKNOWN  3
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ADDTYPE(A,B,C) \
  if ( !CLang::Memory(Buffer,sizeof(A)) ) return C\
  A* p = (A*)(Buffer.szQueue + Buffer.nPtr);\
  p->btType = B;\
  p->wdSizeOf = (WORD)sizeof(A);\
  Buffer.nPtr += sizeof(A);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define EXTTYPE(A,B,C) ADDTYPE(A,B,C) \
    p->dwShift = (DWORD)(def.szBeg-def.szPtr);\
    p->dwSizeT = (DWORD)(def.szEnd-def.szBeg);\
    if ( CLang::ms_nStack ) p->dwSizeT = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Добавление элемента в очередь
#define AddQueue(A,B) \
if ( !def.ms_bAddQueue && (::IsMnemoStyle(MNEMO_HIDE_SCN) == 0 ) ) return;\
EXTTYPE(A,B,;)
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define INIT static bool bInit = true; if ( bInit ) { bInit=false; Init(q);}
//unsigned char SkipQueue(unsigned char*& szQueue);
////
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//SQueue BuildQueue ( class CQueueWnd*, const char* szPath, const char* szName);
//void   PaintQueue (HDC hDC,SQueue& q,CQueueWnd* pWnd);
//void   FreeQueue  (SQueue& q);
//int    MouseQueueL(CQueueWnd* wnd,SQueue& q,POINT point );
//int    MouseQueueM(CQueueWnd* wnd,SQueue& q,POINT point );
//int    MouseQueueS(CQueueWnd* wnd,SQueue& q,POINT point,int* nNumber,bool bLButton);
//int    FindQueue(SQueue& q,int nNumber,RECT& rc);
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
