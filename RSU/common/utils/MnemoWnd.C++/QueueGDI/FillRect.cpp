#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_FillRect : public SPROLOZ
  {
  COLORREF color;
  int ed;
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
//int PaintFillRect(DefinePaint)
//  {
//  QueueP(Queue_FillRect);
//  HBRUSH hBrush = ::CreateSolidBrush( p->color );
//  if ( hBrush != NULL )
//    {
//    ::FillRect( hDC, &p->mRect, hBrush );
//    if ( p->ed )
//    ::DrawEdge( hDC, &p->mRect, p->ed, BF_RECT );
//    ::DeleteObject( hBrush );
//    }
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddFillRect(DefineTegs)
  {
  AddQueueEx(Queue_FillRect);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintFillRect;
  //
  STegObj obj[] =
    {
    { 'R', "rect", &p->mRect },
    { 'E', "rc"  , &p->mRect },
    { 'I', "edge", &p->ed    },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  if ( *teg[0].value == '-' )
    p->color = def.bkColor;
  else
    p->color = ::ParserColors(teg[0].value);
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
