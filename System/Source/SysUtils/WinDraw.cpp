#include "WinDraw.h"
#include "Err.h"

struct Collector * pCollector;
int LastPressedKey = 0;

QPen * CreatePen( int Style, int Size, DWORD color)
{
  QPen * pPen;
  if ( pCollector )
  {
    ASS(pCollector)
    ASS(pCollector->kPen < 4)
    pPen = &pCollector->Pen[pCollector->kPen++];
  }
  else
  {
    pPen = new QPen();
  }
  pPen->setStyle((Qt::PenStyle)Style);
  pPen->setColor((QColor)color);
  pPen->setWidth(Size );
  return pPen;
}

QBrush * GetStockObject( int Style )
{
  if ( Style == NULL_BRUSH )
  {
  QColor color(255,255,255,255); // Start with a standard color
//  ASS(pCollector->kBrush < 4)
  QBrush * pBrush = new QBrush( color );//&pCollector->Brush[pCollector->kBrush++];;
//  pBrush->setColor( color );
  return pBrush;
  }
  ASS(0)
}

void * SelectObject ( Collector & C, void * pObj )
{
  return NULL;
}

QPen * SelectObject ( Collector & C, QPen * pPen )
{
  if ( pPen )
    {
    C.pP->setPen( *pPen );
    return NULL;
    }
  return NULL;
}
QBrush * SelectObject ( Collector & C, QBrush * pBrush )
{
  if ( pBrush )
  {
    C.pP->setBrush( *pBrush );
  }
  return NULL;
}

QFont * SelectObject ( Collector & C, QFont * pFont )
{
  if ( pFont )
  {
    C.pFont = pFont;
    C.pP->setFont( *pFont );

  }
  return NULL;
}

void Rectangle (  Collector & C, int L, int T, int R, int B )
{
  C.pP->drawRect( L, T, R - L, B - T );
}

int SetBkColor ( Collector & C, DWORD color )
{
  QBrush Brush;
  Brush.setColor( color );
  C.pP->setBackground( Brush);
  return 0;
}

int SetTextAlign ( Collector & C, DWORD align )
{
  C.Align |= align;
//painter.drawText(rect, Qt::AlignCenter, "Hello World");
}
//enum Qt::BGMode
// QPainter
int SetTextColor ( Collector & C, DWORD color )
{
  ASS(pCollector->kPen < 4)
  QPen * pPen = &pCollector->Pen[pCollector->kPen++];
  pPen->setColor((QColor)color);
  C.pTextColor = pPen;
  //painter.setPen(QColor("red"));
}

int SetBkMode ( Collector & C, DWORD mode )
{
  C.pP->setBackgroundMode((Qt::BGMode)mode);
}

QFont * CreateFont(
  int pointSize,
  int, int, int, int weight, int, int, int,
  int, int, int, int,
  int, char * Family )
{

  QFont * pFont = new QFont(Family, abs(pointSize)/1.5, QFont::ExtraBold );//weight );
  return pFont;
}

void TextOut (Collector & C, int x, int y, char * Txt, int lTxt )
{
  //QString text = Txt;
  //C.pP->setFlqthelp://org.qt-project.qtgui.6102/qtcore/qt.html#AlignmentFlag-enum

  QFontMetrics FM ( C.pP->font());
  QRect rc = FM.boundingRect( Txt );
  DWORD Align = C.Align;
//  if ( Align & Qt::AlignLeft )
  if ( Align & Qt::AlignHCenter )
    x -= rc.width() >> 1;
  if ( Align & Qt::AlignRight )
     x -= rc.width();

  if ( Align & Qt::AlignTop )
    y += rc.height() - 3;
  if ( Align & Qt::AlignBottom )
    y += 3;
  if ( Align & Qt::AlignVCenter )
    y += rc.height() >> 1;
//  if ( Align & Qt::AlignBaseline )
//    y -= rc.height() + 3;
  if ( C.pTextColor )
    {
    QPen Old = C.pP->pen();
    C.pP->setPen( *C.pTextColor );
    C.pP->drawText( x, y, Txt ); //left,baseline
    C.pP->setPen( Old );
    return;
    }
  C.pP->drawText( x, y, Txt );
}

int GetTextExtentPoint32( Collector & C, const char * Txt, int L, CSize * Size )
{
  if ( C.pFont == NULL )
    return 0;
  QFontMetrics fm( *C.pFont );
  // 1. Получить точный размер (ширина и высота)
  QSize size = fm.size(Qt::TextSingleLine, Txt );
  int width = size.width();
  int height = size.height();
  Size->cx = width;
  Size->cy = height;
  return width;
}

QBrush * CreateSolidBrush( DWORD color )
{
  QBrush * pBrush = new QBrush ( color );
  return pBrush;
}
// #define left left()
// #define top top()
// #define right right()
// #define bottom bottom()

#include <QRect>
void TransparentRect( Collector & C, CRect & rc, DWORD color, DWORD fon)
{
//@@  if ( C.pixmap == NULL )
    return;
//  QRect VP = C.pP->viewport();
//  QRect WP = C.pP->window();
  double L = ( rc.left + C.Sx ) * C.M;// / C.M + C.Sx;
  double R = ( rc.right + C.Sx) * C.M;// / C.M + C.Sx;
  double T = ( rc.top + C.Sy ) * C.M;// / C.M - C.Sy;
  double B = ( rc.bottom + C.Sy ) * C.M;// / C.M - C.Sy;
  double W = R - L;
  double H = B - T;
  //QPainter
  try
  {
  int ww = C.pixmap->width();
  int hh = C.pixmap->height();
  QPixmap PM = C.pixmap->copy( L, T, W, H );
  QImage I = PM.toImage();
  QRgb From = fon;
  QRgb To = color;
  int w = I.width();
  int h = I.height();
  for (int y = 0; y < h; ++y)
  {
    QRgb *line = reinterpret_cast<QRgb*>(I.scanLine(y));
    for (int x = 0; x < w; ++x)
    {
      QRgb &rgb = line[x];
      if ( rgb == From )
        line[x] = To;
    }
  }
   QRectF target( L / C.M - C.Sx, T / C.M - C.Sy, rc.right - rc.left, rc.bottom - rc.top);
   QRectF source(0, 0,  W, H);
   C.pP->drawImage(target, I, source);
  }
  catch(...)
  {
    KKK();
  }

//   C.pP->drawPixmap((int)L, (int)T, PM );
  // QPen Pen;
  // Pen.setStyle(Qt::SolidLine);
  // Pen.setColor(Qt::yellow);
  // Pen.setWidth( 2 );
  // C.pP->setPen( Pen );
  // C.pP->drawLine( L, T, L+W, T);
  // C.pP->drawLine( L+W, T, L+W, T+H);
  // C.pP->drawLine( L+W, T+H, L, T+H );
  // C.pP->drawLine( L, T+H, L, T );
}
IN_DLL void TransparentStd( Collector & C, CRect & rc, DWORD color)
{
//  0xAARRGGBB
//  DWORD New = ( color << 16 ) & 0xFF0000;
//  New |= ( color ) & 0x00FF00;
//  New |= ( color >> 16 ) & 0x0000FF;
  TransparentRect( C,rc, color, qRgb(1, 2, 3));
}

void FillRect ( Collector & C, CRect * rc, class QBrush * pBrush )
{
  QRect R ( rc->left,rc->top,rc->right - rc->left, rc->bottom - rc->top );
  C.pP->fillRect( R, *pBrush );
}

void MoveToEx ( Collector & C, int x, int y, void * )
{
  C.x = x;
  C.y = y;
}
void LineTo ( Collector & C, int x, int y )
{
  C.pP->drawLine( C.x, C.y, x, y );
  C.x = x;
  C.y = y;
}
void Ellipse ( Collector & C, int l, int t, int r, int b )
{
  C.pP->drawEllipse( l, t, r - l, b - t );
}
DWORD GetAsyncKeyState( int Key )
{
  if ( LastPressedKey == Key )
    return 0xFFFFFFFF;
  return 0;
}

BOOL DeleteObject ( void * pObj )
{
  return TRUE;
}
void SetMapMode (Collector & C, int mode )
{

}
void GetTextMetrics (Collector & C, TEXTMETRIC * pTM )
{

}
