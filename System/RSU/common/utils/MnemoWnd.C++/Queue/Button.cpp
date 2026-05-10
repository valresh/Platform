#include "Queue.h"
#include "../Lang.h"
//
// Обработка кнопок
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_Button : public SPROLOZ, public SBtnDataEx
  {
  char szGoto[64];
  bool bCompr;
  } Queue_Button;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//int PressButton(DefinePress)
//  {
//  Queue_Button* p = (Queue_Button*)s;
//  if ( nCode == 0 )
//    p->m_bPressed = true;
//  else
//  if ( nCode == 1 )
//    { }
//  else
//  if ( nCode == 2 )
//    p->m_bPressed = false;
//  //
//  bool bPtInRect = ::PaintSelBtn( q, point, p->mRect, p );
//  if ( nCode == 2 && bPtInRect )
//    {
//    if ( *p->szGoto != '\0' )
//      {
//      if ( p->bCompr )
//        if ( lstrcmpi( p->szGoto, q.szName ) == 0 )
//          return 0;
//      static char szGoto[256];
//      lstrcpy( szGoto, p->szGoto );
//      if ( p->bCompr )
//      ::SendMessage( q.hWnd, nBuildQueue, BUILD_MNEMO, (LPARAM)(LPCTSTR)szGoto );
//      ::PostMsg( q.hWnd, QUEUE_GOTO, (LPARAM)szGoto );
//      }
//    }
//  //
//  return 0;
//  }
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Функция отображения
//int PaintButton(DefinePaint)
//  {
//  QueueP(Queue_Button);
//  //
//  if ( p->bCompr )
//    {
//    if ( lstrcmpi( p->szGoto, q.szName ) == 0 )
//      p->bkgNorma   = 0x84c7ff;
//    else
//      p->bkgNorma   = 0xc0c0c0;
//    }
//  //
//  bool bPressed = PressedButton( p, p->mRect, q );
//  ::PaintBtn( hDC, p->mRect, p, bPressed );
//  //
//  if ( ::IsMnemoStyle(MNEMO_EDIT) )
//    {
//    if ( lstrcmpi( "Null", p->szGoto ) == 0 )
//      {
//      CMyPen pen(hDC, 0xFF, 2 );
//      int cx = p->Cx();
//      int cy = p->Cy();
//      POINT pt[5] = {{cx,p->mRect.top},{p->mRect.right,cy},{cx,p->mRect.bottom},{p->mRect.left,cy},{cx,p->mRect.top}};
//      ::Polyline( hDC, pt, 5 );
//      }
//    }
//  //
//  return 1;
//  }
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void AddButton(DefineTegs)
//  {
//  //
//  AddQueueEx(Queue_Button);
//  // Код, посылаемый в АРМ
//  p->btSendType = QUEUE_BUTTON;
//  // Определение функции вывода
//  p->paint = &PaintButton;
//  p->press = &PressButton;
//  //
//  DefaultButton( p, teg[0].value );
//  //
//  STegObj obj[] =
//    {
//    { 'S', "goto",  p->szGoto, 63 },
//    { 'L', "comp", &p->bCompr },
//    TEG_BUTTON // Кнопочная информация
//    EXT_QUEUE  // Дополнительная информация
//    //
//    };
//  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
//  p->nNumber  = def.GetActiveObj();
//  PostfixButton( p );
//  //
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
