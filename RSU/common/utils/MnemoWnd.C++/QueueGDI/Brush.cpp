#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_BrushC : public SPROLOZ
  {
  COLORREF color;
  //HBRUSH hBrush;
  //HBRUSH hOld;
  int nHatch;
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct Queue_BrushD : public SPROLOZ
{
  DWORD_PTR dwPlace;
} ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Это на всякий случай
void ClearBrushC(SQueue& q,void* s)
  {
  //Queue_BrushC* p = (Queue_BrushC*)s;
  //if ( p->hBrush )
  //  ::DeleteObject( p->hBrush );
  //p->hBrush = NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintBrushC(DefinePaint)
  {
  //QueueP(Queue_BrushC);
  //    if ( p->nHatch == -1 )
  //  p->hBrush = ::CreateSolidBrush( p->color );
  //else
  //  p->hBrush = ::CreateHatchBrush( p->nHatch, p->color );
  //p->hOld = (HBRUSH)::SelectObject(hDC,p->hBrush);
  return 1;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int PaintBrushD(DefinePaint)
  {
  //QueueP(Queue_BrushD);
  //if ( p->dwPlace != 0 )
  //  {
  //  HGDIOBJ* hGdi = (HGDIOBJ*)(beginQ+p->dwPlace);
  //  ::SelectObject(hDC,hGdi[1]);
  //  ::DeleteObject( hGdi[0] );
  //  hGdi[0] = NULL;
  //  }
  return 1;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD_PTR dwBrush[32];
void AddBrushC(DefineTegs)
  {
  //static int nCount = sizeof(dwBrush)/sizeof(dwBrush[0])-1;
  //if ( def.brsCount > nCount )
  //  return;
  //
  AddQueueEx(Queue_BrushC);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  p->paint = &PaintBrushC;
  p->clear = &ClearBrushC;
  //
  p->nHatch = -1;
  if ( *teg[0].value == '-' )
    p->color = def.bkColor;
  else
    p->color = ::ParserColors(teg[0].value);
  //
  STegObj obj[] =
    {
    { 'I', "h"    , &p->nHatch },
    { 'I', "hatch", &p->nHatch },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  //dwBrush[def.brsCount] = (DWORD_PTR)(&p->hBrush)-(DWORD_PTR)(Buffer.szQueue);
  //def.brsCount++;
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddBrushD(DefineTegs)
  {
  //if ( def.penCount <= 0 )
  //  return;
  AddQueueEx(Queue_BrushD);
  //// Код, посылаемый в АРМ
  //p->btSendType = QUEUE_NO;
  //// Определение функции вывода
  //p->paint = &PaintBrushD;
  ////
  //def.brsCount--;
  //p->dwPlace = dwBrush[def.brsCount];
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
