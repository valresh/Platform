//#include "stdafx.h"
#include "Queue.h"
#include "../Lang.h"
//
//extern Gdiplus::Image*  GdiplusImageFile( const char* pszFile );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Заголовок мнемосхемы
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_Header : public SPROLOZ
  {
  EDirect eDir;
  //HGLOBAL hGlobal;
  //Gdiplus::Image* gdiplusImage;
  char Bitmap[100];
  COLORREF bkColor;

  // vladexl: Moved from SQUEUE
  char szSxema[_MAX_FNAME];
  SIZE szWnd;// Заданные параметры окна
  int   m_nCodeWindow;
  bool  m_bMouseWheel;
  } ;
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void ClearHeader(SQueue& q,void* s)
//  {
//  Queue_Header* p = (Queue_Header*)s;
//  if ( p->hGlobal )
//    ::GlobalFree( p->hGlobal );
//
//  if(p->gdiplusImage)
//    delete p->gdiplusImage;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//int PaintHeader(DefinePaint)
//  {
//  QueueP(Queue_Header);
//  //
//  RECT rect;
//  ::GetClientRect( q.hWnd, &rect );
//  //
//  LPPOINT pt = (LPPOINT)&rect;
//  PtWT( pt[0], q );
//  PtWT( pt[1], q );
//  //
//  HBRUSH hBrush = ::CreateSolidBrush( p->bkColor );
//  if ( hBrush != NULL )
//    {
//    ::FillRect(hDC,&rect,hBrush);
//    ::DeleteObject( hBrush );
//    }
//  //
//  if ( p->hGlobal != NULL )
//    {
//    LPBITMAPINFO info = (LPBITMAPINFO)::GlobalLock( p->hGlobal );
//    if ( info != NULL )
//      {
//      LPBYTE bits = (LPBYTE)info;
//      bits += sizeof( BITMAPINFOHEADER );
//      //
//      if ( p->eDir == dirFree )
//        {
//        for ( int x = rect.left; x < rect.right; x += info->bmiHeader.biWidth )
//        for ( int y = rect.top; y < rect.bottom; y += info->bmiHeader.biHeight )
//          {
//          ::StretchDIBits( hDC, x, y,
//                    info->bmiHeader.biWidth,info->bmiHeader.biHeight,
//              0, 0, info->bmiHeader.biWidth,info->bmiHeader.biHeight,
//                bits, info, DIB_RGB_COLORS,SRCCOPY);
//          }
//        }
//      else
//      if ( p->eDir == dirLeft )
//        {
//        int w = info->bmiHeader.biWidth;
//        int h = info->bmiHeader.biHeight;
//          ::StretchDIBits( hDC, 0, 0, w, h, 0, 0, w, h,
//                bits, info, DIB_RGB_COLORS,SRCCOPY);
//        }
//      ::GlobalUnlock( p->hGlobal );
//      }
//    }
//  else
//  if(p->gdiplusImage)
//  {
//    int hh = p->gdiplusImage->GetHeight();
//    int ww = p->gdiplusImage->GetWidth();
//    Gdiplus::Graphics Grf( hDC );
//    if ( p->eDir == dirFree )
//      Grf.DrawImage ( p->gdiplusImage, 0, 0, rect.right-rect.left, rect.bottom-rect.top );
//    else
//    if ( p->eDir == dirLeft )
//        Grf.DrawImage ( p->gdiplusImage, 0, 0, ww, hh);
//  }
//
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddHeader(DefineTegs)
  {
  AddQueueEx(Queue_Header);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintHeader;
  //p->clear = &ClearHeader;
  //
  // Ссылка на технологическую схему
  StrCPY( p->szSxema, teg[0].value, sizeof(p->szSxema) );
  //
  p->bkColor = 0xC0C0C0;
  p->eDir    = dirLeft;
  p->szWnd.cx = 100;
  p->szWnd.cy = 100;
  p->m_nCodeWindow = 0;
  //
  memset(p->Bitmap, 0, sizeof(p->Bitmap));
  //
  p->m_bMouseWheel = 0;
  //
  STegObj obj[] =
    {
    { 'A', "color" , &p->bkColor },
    { 'P', "size"  , &p->szWnd    },
    { 'I', "code"  , &p->m_nCodeWindow },
    { 'L', "wheel" , &p->m_bMouseWheel },
    //
    { 'S', "bitmap", &p->Bitmap, 99 },
    { 'N', "direct", &p->eDir  },
    { 'N', "d"     , &p->eDir  },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  //def.bkColor = p->bkColor;
  //
  // if ( *szBitmap )
  //SIZE sz = { info->bmiHeader.biWidth, info->bmiHeader.biHeight };
  //def.m_queue.szWnd = sz;
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
