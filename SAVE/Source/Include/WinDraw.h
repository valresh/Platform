#ifndef WINDRAW_H
#define WINDRAW_H

#ifndef NO_CPP

#include "crossplatform.h"
#include "BaseModel.h"
#include "WinFiles.h"
#endif

#include <QPen>


#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct CRect
{
    int left;
    int top;
    int right;
    int bottom;
};

struct CSize
{
    int cx;
    int cy;
};

#define HPEN QPen *
#define HBRUSH QBrush *
#define HDC void *
#undef RECT
#define RECT CRect
#define HFONT QFont *
#define TA_CENTER Qt::AlignHCenter
#define TA_BASELINE Qt::AlignBaseline
#define TA_TOP Qt::AlignTop
#define TRANSPARENT Qt::TransparentMode,
#define OPAQUE  Qt::OpaqueMode
#define FW_BOLD QFont::Bold
#define OUT_TT_ONLY_PRECIS 0
#define ANTIALIASED_QUALITY 0
#define TA_LEFT Qt::AlignLeft
#define TA_BOTTOM Qt::AlignBottom
#define SIZE CSize
// Qt::AlignTop
// Qt::AlignVCenter
// Qt::AlignBaseline
// Qt::AlignRight
// Qt::AlignHCenter
// Qt::AlignJustify
#define ANSI_CHARSET 0
#define OUT_OUTLINE_PRECIS 0
#define CLIP_DEFAULT_PRECIS 0
#define VARIABLE_PITCH 0
#define VK_CONTROL 0
#define VK_SHIFT 0
// #define left left()
// #define top top()
// #define right right()
// #define bottom bottom()
#define PS_SOLID 1
#define FW_NORMAL QFont::Normal
//
#define INVALID_HANDLE_VALUE NULL
//#define _HANDLE QFile *

#define NULL_BRUSH 0
IN_DLL extern int LastPressedKey;

#define HWND void *
#define HDC void *

struct Collector
{
    QPen Pen[4];
    int kPen;
    QBrush Brush[4];
    int kBrush;
    QFont * pFont;
    DWORD Align;
    QPen * pTextColor;
    QPainter * pP;
    class QPixmap * pixmap;
    double Sx, Sy;
    double M;
    int x, y;
    Collector()
    {
      Sx = Sy = 0.;
      M = 1.;
      kPen = 0;
      kBrush = 0;
      pFont = NULL;
      Align = 0;
      pTextColor = NULL;
      x = 0;
      y = 0;
      pP = NULL;
      pixmap = NULL;
    }
    void Init ( QPainter * _pP )
    {
      kPen = 0;
      kBrush = 0;
      pFont = NULL;
      Align = 0;
      pTextColor = NULL;
      pP =_pP;
    }
};


extern IN_DLL struct Collector * pCollector;



 IN_DLL QPen * CreatePen( int Style, int Size, DWORD color);
 IN_DLL QBrush * GetStockObject( int Style );
 IN_DLL QPen * SelectObject ( Collector & C, QPen * pPen );
 IN_DLL void * SelectObject ( Collector & C, void * pObj );
 IN_DLL QBrush * SelectObject ( Collector & C, QBrush * pBrush );
 IN_DLL QFont * SelectObject ( Collector & C, QFont * pFont );
 IN_DLL void Rectangle (  Collector & C, int L, int T, int R, int B );
 IN_DLL int SetBkColor ( Collector & C, DWORD color );
 IN_DLL int SetTextAlign ( Collector & C, DWORD align );
 IN_DLL int SetTextColor ( Collector & C, DWORD color );
 IN_DLL int SetBkMode ( Collector & C, DWORD mode );
 IN_DLL void TextOut ( Collector & C, int x, int y, char * Txt, int lTxt );
 IN_DLL int GetTextExtentPoint32( Collector & C, const char * Txt, int L, CSize * Size );

 IN_DLL BOOL DeleteObject ( void * pObj );
#define MM_TEXT 0
 IN_DLL void SetMapMode (Collector & C, int mode );
struct  TEXTMETRIC
  {
    double O;
  };
IN_DLL void GetTextMetrics (Collector & C, TEXTMETRIC * pTM );

 IN_DLL QFont * CreateFont(
   int Size,
   int, int, int, int Type, int, int, int,
   int, int, int, int,
   int, char * Name );
 IN_DLL QBrush * CreateSolidBrush( DWORD color );
 IN_DLL void TransparentStd( Collector & C, CRect & rc, DWORD color);
 IN_DLL void TransparentRect( Collector & C, CRect & rc, DWORD color, DWORD fon);
 IN_DLL void MoveToEx ( Collector & C, int x, int y, void * );
 IN_DLL void LineTo ( Collector & C, int x, int y );
 IN_DLL void Ellipse ( Collector & C, int l, int t, int r, int b );
 IN_DLL DWORD GetAsyncKeyState( int Key );
 IN_DLL void FillRect ( Collector & C, CRect * rc, class QBrush * pBrush );


 struct CDrawObjData
 {
     char * ObjName;
     char * ShemaName;
     QColor *pCol;
     CRect rc;
     void * hWnd;
     Collector hDC;
     int ID_Dial;
     int nModelID;
 };

//
//#ifdef LINUX
 // #define left left()
 // #define top top()
 // #define right right()
 // #define bottom bottom()
//#endif

// #ifdef LINUX
// #include <QtCore/qglobal.h>
// #ifdef UTILS_EXPORTS
// #define IN_DLL Q_DECL_EXPORT
// #else
// #define IN_DLL Q_DECL_IMPORT
// #endif
// #else

// #ifdef LINUX
// #include "WinDraw.h"
// #endif

#endif // WINDRAW_H
