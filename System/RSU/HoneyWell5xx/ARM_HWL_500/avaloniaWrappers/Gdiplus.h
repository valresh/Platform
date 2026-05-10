#ifndef GDIPLUS_H
#define GDIPLUS_H

//ИЗ НЕЙРОСЕТИ, ЧТО БЫ НЕ ПРАВИТЬ GDI

#include <string>
#include <algorithm>
// #include <cctype>
#include <./Api.h>


//#include <crosswindows.h>
/*#ifndef INT
#define INT int
#endif

#ifndef VOID
#define VOID void*
#endif

typedef unsigned long DWORD;

#ifndef COLORREF
#define COLORREF DWORD
#endif


#ifndef BOOL
#define BOOL bool
#endif

#ifndef BYTE
#define BYTE uint8_t//std::byte
#endif
*/
#ifndef REAL
#define REAL double
#endif

#ifndef PS_SOLID
#define PS_SOLID            0
#endif

#define DWORD_PTR uintptr_t

#define WORD unsigned short
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))


namespace cross {

#ifndef FontStyle
enum FontStyle
{
    FontStyleRegular    = 0,
    FontStyleBold       = 1,
    FontStyleItalic     = 2,
    FontStyleBoldItalic = 3,
    FontStyleUnderline  = 4,
    FontStyleStrikeout  = 8
};
#endif

enum QualityMode
{
    QualityModeInvalid   = -1,
    QualityModeDefault   = 0,
    QualityModeLow       = 1, // Best performance
    QualityModeHigh      = 2  // Best rendering quality
};

enum SmoothingMode
{
    SmoothingModeInvalid     = QualityModeInvalid,
    SmoothingModeDefault     = QualityModeDefault,
    SmoothingModeHighSpeed   = QualityModeLow,
    SmoothingModeHighQuality = QualityModeHigh,
    SmoothingModeNone,
    SmoothingModeAntiAlias
};

enum InterpolationMode
{
    InterpolationModeInvalid          = QualityModeInvalid,
    InterpolationModeDefault          = QualityModeDefault,
    InterpolationModeLowQuality       = QualityModeLow,
    InterpolationModeHighQuality      = QualityModeHigh,
    InterpolationModeBilinear,
    InterpolationModeBicubic,
    InterpolationModeNearestNeighbor,
    InterpolationModeHighQualityBilinear,
    InterpolationModeHighQualityBicubic
};

enum PixelOffsetMode
{
    PixelOffsetModeInvalid     = QualityModeInvalid,
    PixelOffsetModeDefault     = QualityModeDefault,
    PixelOffsetModeHighSpeed   = QualityModeLow,
    PixelOffsetModeHighQuality = QualityModeHigh,
    PixelOffsetModeNone,    // No pixel offset
    PixelOffsetModeHalf     // Offset by -0.5, -0.5 for fast anti-alias perf
};

enum DashStyle
{
    DashStyleSolid,          // 0
    DashStyleDash,           // 1
    DashStyleDot,            // 2
    DashStyleDashDot,        // 3
    DashStyleDashDotDot,     // 4
    DashStyleCustom          // 5
};

namespace Gdiplus {

// Функция преобразует строку в нижний регистр
inline std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}



class Font  {
public:

    WrapperFont font;

    Font(char* family, double emSize, INT  style = FontStyleRegular ){

        font.fontFamily = family;
         font.fontSize = (std::byte)emSize;
        // //TODO СООТВЕТВИЕ аволонии и констат, можно переопределить ENUM
        // font.fontWeight = (std::byte)style;
    }

    public: WrapperFont*  getWrapper () {

        return &font;
    }


};

class Size
{
public:
    Size()
    {
        Width = Height = 0;
    }

    Size(const Size& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    Size(INT width,
         INT height)
    {
        Width = width;
        Height = height;
    }

    Size operator+(const Size& sz) const
    {
        return Size(Width + sz.Width,
                    Height + sz.Height);
    }

    Size operator-(const Size& sz) const
    {
        return Size(Width - sz.Width,
                    Height - sz.Height);
    }

    BOOL Equals(const Size& sz) const
    {
        return (Width == sz.Width) && (Height == sz.Height);
    }

    BOOL Empty() const
    {
        return (Width == 0 && Height == 0);
    }

public:

    INT Width;
    INT Height;
};


class Point
{
public:
    Point()
    {
        X = Y = 0;
    }

    Point(const Point &point)
    {
        X = point.X;
        Y = point.Y;
    }

    Point(const Size &size)
    {
        X = size.Width;
        Y = size.Height;
    }

    Point(INT x,
          INT y)
    {
        X = x;
        Y = y;
    }

    Point operator+(const Point& point) const
    {
        return Point(X + point.X,
                     Y + point.Y);
    }

    Point operator-(const Point& point) const
    {
        return Point(X - point.X,
                     Y - point.Y);
    }

    BOOL Equals(const Point& point)
    {
        return (X == point.X) && (Y == point.Y);
    }

public:

    INT X;
    INT Y;
};

class Rect
{
public:

    Rect()
    {
        X = Y = Width = Height = 0;
    }

    Rect(INT x,
         INT y,
         INT width,
         INT height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    Rect(const Point& location,
         const Size& size)
    {
        X = location.X;
        Y = location.Y;
        Width = size.Width;
        Height = size.Height;
    }

    Rect* Clone() const
    {
        return new Rect(X, Y, Width, Height);
    }

    void* GetLocation(Point* point) const
    {
        point->X = X;
        point->Y = Y;
    }

    void* GetSize(Size* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }

    VOID GetBounds( Rect* rect) const
    {
        rect->X = X;
        rect->Y = Y;
        rect->Width = Width;
        rect->Height = Height;
    }

    INT GetLeft() const
    {
        return X;
    }

    INT GetTop() const
    {
        return Y;
    }

    INT GetRight() const
    {
        return X+Width;
    }

    INT GetBottom() const
    {
        return Y+Height;
    }

    BOOL IsEmptyArea() const
    {
        return (Width <= 0) || (Height <= 0);
    }

    BOOL Equals( const Rect & rect) const
    {
        return X == rect.X &&
               Y == rect.Y &&
               Width == rect.Width &&
               Height == rect.Height;
    }

    BOOL Contains( INT x,
                   INT y) const
    {
        return x >= X && x < X+Width &&
               y >= Y && y < Y+Height;
    }

    BOOL Contains( const Point& pt) const
    {
        return Contains(pt.X, pt.Y);
    }

    BOOL Contains( Rect& rect) const
    {
        return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
               (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
    }

    VOID Inflate(INT dx,
                 INT dy)
    {
        X -= dx;
        Y -= dy;
        Width += 2*dx;
        Height += 2*dy;
    }

    VOID Inflate(const Point& point)
    {
        Inflate(point.X, point.Y);
    }

    BOOL Intersect(const Rect& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static BOOL Intersect( Rect& c,
                          const Rect& a,
                          const Rect& b)
    {
        // INT right = min(a.GetRight(), b.GetRight());
        // INT bottom = min(a.GetBottom(), b.GetBottom());
        // INT left = max(a.GetLeft(), b.GetLeft());
        // INT top = max(a.GetTop(), b.GetTop());

        // c.X = left;
        // c.Y = top;
        // c.Width = right - left;
        // c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    BOOL IntersectsWith(const Rect& rect) const
    {
        return (GetLeft() < rect.GetRight() &&
                GetTop() < rect.GetBottom() &&
                GetRight() > rect.GetLeft() &&
                GetBottom() > rect.GetTop());
    }

    static BOOL Union( Rect& c,
                      const Rect& a,
                       const Rect& b)
    {
        // INT right = max(a.GetRight(), b.GetRight());
        // INT bottom = max(a.GetBottom(), b.GetBottom());
        // INT left = min(a.GetLeft(), b.GetLeft());
        // INT top = min(a.GetTop(), b.GetTop());

        // c.X = left;
        // c.Y = top;
        // c.Width = right - left;
        // c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    VOID Offset( const Point& point)
    {
       // Offset(point.X, point.Y);
    }

    VOID Offset( INT dx,
                 INT dy)
    {
        X += dx;
        Y += dy;
    }

public:

    INT X;
    INT Y;
    INT Width;
    INT Height;
};

class RectF {
public:

    WrapperRectF m_rect;

    RectF(int _x, int _y, int _width, int _hight) {

        m_rect.X = _x;
        m_rect.Y = _y;
        m_rect.width = _width;
        m_rect.height = _hight;

    }

    WrapperRectF* getWrapper() {

        return &m_rect;
    }

};

class CMyBrush {
public:

    WrapperSolidBrush cMyBrush;

    CMyBrush(int hDC, int color, bool bSelect = false) {
        // cMyBrush. =  (HDC)hDC;
        // cMyBrush.colorreff =  color;
    }

    public: WrapperSolidBrush* getWrapper() {


        return &cMyBrush;
    }



};


class CMyPen {
public:

    // double m_x, m_y, m_width, m_height;

    WrapperPen cMyPen;


    CMyPen(HDC hDC, COLORREF color, int nWidth = 0,int nStyle = PS_SOLID) {
        cMyPen.hDC =  hDC;
        cMyPen.color =  color;
        cMyPen.nWidth =  nWidth;
        cMyPen.nStyle =  nStyle;
    }

    public: WrapperPen* getWrapper() {


        return &cMyPen;
    }



};

class Color {
private:

    WrapperColor m_color;
public:


    Color(int A, int R, int G, int B)
    {
        m_color.A = (std::byte)A;
        m_color.R = (std::byte)R;
        m_color.G = (std::byte)G;
        m_color.B = (std::byte)B;
    }


    WrapperColor* getWrapper() {
        return &m_color;
    }

    void SetFromCOLORREF(COLORREF color) {





        m_color.A =  (std::byte)255;
        m_color.R = (std::byte)GetRValue(color);
        m_color.G = (std::byte)GetGValue(color);
        m_color.B = (std::byte)GetBValue(color);


    }

    COLORREF GetColorReff(){
        return ((COLORREF)(((int)(BYTE)(m_color.R)|((WORD)((BYTE)(m_color.G))<<8))|(((DWORD)(BYTE)(m_color.B))<<16)));
    }

};

class Pen {
public:

    // double m_x, m_y, m_width, m_height;

    WrapperPen cMyPen;


    Pen(HDC hDC, COLORREF color, int nWidth = 0,int nStyle = PS_SOLID) {
        cMyPen.hDC =  hDC;
        cMyPen.color =  color;
        cMyPen.nWidth =  nWidth;
        cMyPen.nStyle =  nStyle;
    }

    Pen(Color color,int nWidth = 0) {
        //todo get colorreff from
        //cMyPen.color =  color;
     //todo get color from COLOR
        cMyPen.nWidth =  nWidth;
    }

    void SetDashStyle(DashStyle style) {

        //todo
    }

    void SetDashPattern( const REAL* dashArray,  INT count) {

        //todo
    }




    public: WrapperPen* getWrapper() {

        return &cMyPen;
    };



};



class SolidBrush {

public:
    WrapperBrush m_WSB;

public:

    SolidBrush(int8_t _R, int _G, int _B, int _A) {
        m_WSB.color.R = (std::byte)_R;
        m_WSB.color.G = (std::byte)_G;
        m_WSB.color.B = (std::byte)_B;
        m_WSB.color.A = (std::byte)_A;

        //call callback for dicross::RECT call


    }

    SolidBrush(Color color) {
        m_WSB.color.R = color.getWrapper()->R;
        m_WSB.color.G = color.getWrapper()->G;
        m_WSB.color.B = color.getWrapper()->B;
        m_WSB.color.A = color.getWrapper()->A;

        //call callback for dicross::RECT call


    }

    public: WrapperBrush* getWrapper() {
        return &m_WSB;
    }
};
class Graphics {

private:
    HDC m_hdc;

public:

    Graphics(HDC _hdc) {
      //  m_hdc = _hdc;

    }

    //todo
    //GdiplusStartup

    void FillRectangle(SolidBrush* _soludBrush,const cross::Gdiplus::RectF& _rectf) {

        m_RenderCallbacks._FillRectangeBCallback(_soludBrush->getWrapper(), &_rectf.m_rect);
    }

    void DrawString(const char *string, INT length, Font *font, RectF &layoutRect,  void *strtingFormat, SolidBrush *brush ) {
        //TODO Get
        m_RenderCallbacks._DrawStringCallback(string, length, const_cast<WrapperFont*>(font->getWrapper()), layoutRect.getWrapper(), brush->getWrapper());
    }

    void SetSmoothingMode(SmoothingMode mode) {

        //TODO SMOOTMODE AVALONIA

    }

    void SetInterpolationMode (InterpolationMode mode) {
         //TODO  AVALONIA
    }

    void SetPixelOffsetMode(PixelOffsetMode mode) {


    }

    void DrawRectangle(Pen * pen, Rect& rect) {

        //нет для авалонии готовой функции, нужно писать
    }


};


}
}
#endif
