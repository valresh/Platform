#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_PenC : public SPROLOZ
{
    int nStyle;
    int nWidth;
    COLORREF color;
    //HPEN hPen;
    //HPEN hOld;
    DWORD style[2];
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct Queue_PenD : public SPROLOZ
{
    //DWORD_PTR dwPlace;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Это на всякий случай
void ClearPenC(SQueue& q,void* s)
  {
  //Queue_PenC* p = (Queue_PenC*)s;
  //if ( p->hPen )
  //  ::DeleteObject( p->hPen );
  //p->hPen = NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintPenC(DefinePaint)
  {
//  QueueP(Queue_PenC);
//  LOGBRUSH brush = {BS_SOLID,p->color,0};
//  if ( p->nStyle == PS_DOT )
//    {
//    static DWORD style[2] = {3,1};
//    p->hPen = ::ExtCreatePen(PS_USERSTYLE | PS_ENDCAP_FLAT | PS_GEOMETRIC, p->nWidth, &brush,2,style);
//    }
//  else
//  if ( p->nStyle == PS_USERSTYLE )
//    {
//    p->hPen = ::ExtCreatePen(PS_USERSTYLE | PS_ENDCAP_FLAT | PS_GEOMETRIC, p->nWidth, &brush,2,p->style);
//    }
//  else
//    {
////  p->hPen = ::CreatePen( p->nStyle, p->nWidth, p->color );
//    p->hPen = ::ExtCreatePen(p->nStyle | PS_ENDCAP_FLAT | PS_GEOMETRIC, p->nWidth, &brush,0,NULL);
////    p->hPen = ::ExtCreatePen(p->nStyle | PS_ENDCAP_SQUARE | PS_GEOMETRIC, p->nWidth, &brush,0,NULL);
//    }
//  p->hOld = (HPEN)::SelectObject(hDC,p->hPen);
  return 1;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int PaintPenD(DefinePaint)
  {
  //QueueP(Queue_PenD);
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
DWORD_PTR dwPen[32];
void AddPenC(DefineTegs)
  {
  //static int nCount = sizeof(dwPen)/sizeof(dwPen[0])-1;
  //if ( def.penCount > nCount )
  //  return;
  //
  AddQueueEx(Queue_PenC);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintPenC;
  //p->clear = &ClearPenC;
  //
  POINT pt = {2,2};
  //
  STegObj obj[] =
    {
    { 'P', "style", &pt        },
    { 'I', "w"    , &p->nWidth },
    { 'I', "width", &p->nWidth },
    { 'A', "color", &p->color  },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  p->nStyle = PS_SOLID;
  if ( lstrcmpi(teg[0].value,"DOT") == 0 ) p->nStyle = PS_DOT;
  if ( lstrcmpi(teg[0].value,"DASH") == 0 ) p->nStyle = PS_DASH;
  if ( lstrcmpi(teg[0].value,"DASHDOT") == 0 ) p->nStyle = PS_DASHDOT;
  if ( lstrcmpi(teg[0].value,"DASHDOTDOT") == 0 ) p->nStyle = PS_DASHDOTDOT;
  if ( lstrcmpi(teg[0].value,"USER") == 0 )
    {
    p->nStyle   = PS_USERSTYLE;
    p->style[0] = pt.x;
    p->style[1] = pt.y;
    }
  //
  //dwPen[def.penCount] = (DWORD_PTR)(&p->hPen)-(DWORD_PTR)(Buffer.szQueue);
  //def.penCount++;
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddPenD(DefineTegs)
  {
  //if ( def.penCount <= 0 )
  //  return;
  AddQueueEx(Queue_PenD);
  //// Код, посылаемый в АРМ
  //p->btSendType = QUEUE_NO;
  //// Определение функции вывода
  //p->paint = &PaintPenD;
  ////
  //def.penCount--;
  //p->dwPlace = dwPen[def.penCount];
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
