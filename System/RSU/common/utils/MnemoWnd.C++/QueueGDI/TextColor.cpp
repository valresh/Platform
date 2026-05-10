#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
typedef struct Queue_TextColor : public SPROLOZ
  {
  COLORREF color;
  } ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Функция отображения
//int PaintTextColor(DefinePaint)
//  {
//  QueueP(Queue_TextColor);
//  ::SetTextColor(hDC, p->color);
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddTextColor(DefineTegs)
  {
  AddQueueEx(Queue_TextColor);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintTextColor;
  p->color = ParserColors( teg[0].value );
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
