//#include "StdAfx.h"

//#include "Function.h"
//#include "HTML.h"
//#include "..\\FindAbstract.h"
//#include "AliasCSV.h"
//#include "ViewTrend.h"
//#include "ViewTrend4.h"
//#include "Extensions.h"
//#include <SmBaseType_.h>
//#include <QbBaseType_.h>
//#include <Windows.h>
// #include <CommProc.h>
// #include "HTML/Tag.h"
// #include "HTML/Style.h"

// #define InitLocal ;\
// _static int nCount = sizeof(list)/sizeof(list[0]);\
//     static bool bInit = true;\
//     Qsort( list, nCount, sizeof(list[0]), bInit )
//     //

//
#include <HTML/Style.h>

#include "HTML/HTML.h"


#undef ASSERT
#define ASSERT(V)

typedef char CharMP[_MAX_PATH];

bool IsStrBOL(const char* ptr)
{
    if ( lstrcmpi( ptr, "TRUE" ) == 0 )
        return true;
    if ( lstrcmpi( ptr, "FALSE" ) == 0 )
        return false;
    if ( lstrcmpi( ptr, "1" ) == 0 )
        return true;
    if ( lstrcmpi( ptr, "0" ) == 0 )
        return false;
    ASSERT(0);
    return false;
}
//
float Number( char*& a )
{
    char* b = a;
    if ( *a == '-' )
        a++;
    while ( '0' <= *a && *a <= '9' || *a == '.' ) a++;
    //
    char ch = *a; *a = 0;
    float n = (float)atof( b );
    *a = ch;
    return n;
}
// Проверка подлинности номера группы
int TrueGroupNumber( const char* ptr )
{
    //  return CViewTrend::Data().TrueGrup( ptr  );
    return 0;
}
// По имени тэга определяем группу
int FindGroupNumber( const char* name )
{
    //  return CViewTrend::Data().FindGrup( name  );
    return 0;
}
// По имени тэга определяем группу трендов
int FindTrendNumber( const char* name )
{
    //  return CViewTrend4::Data().FindGrup( name  );
    return 0;
}
//
bool TrueMnemoName( const char* name )
{
    char szPath[_MAX_PATH];
    //когда пустой путь передается
    if (lstrcmp(name,"")==0)
        return false;
    // sprintf_s( szPath, _MAX_PATH, "%sAbstract\\%s.htm", ::GetBasePath(), name );
    // //
    // _WIN32_FILE_ATTRIBUTE_DATA attr;
    // if (::GetAttr( szPath, attr  ) == TRUE)
    //  return true;
    // else //когда уже есть расширение htm
    // {
    //  sprintf_s( szPath, _MAX_PATH, "%sAbstract\\%s", ::GetBasePath(), name );
    //  return (::GetAttr( szPath, attr  ) == TRUE);
    // }

}
//
COLORREF ColorDiez( const char* ptr )
{
    if ( *ptr != '#' )
    {
        if ( IsInt( ptr ) )
            return (COLORREF)atoi(ptr);
        struct SLocal { const char* name; COLORREF color; };
        static SLocal list[] =
            {
             { "white"          , 0x00ffffff },
             { "black"          , 0x00000000 },
             { "red"            , 0x000000ff },
             { "blue"           , 0x00ff0000 },
             { "darkblue"       , 0x00800000 },
             { "lightgrey"      , 0x00c0c0c0 },
             { "gray"           , 0x00808080 },
             { "green"          , 0x00008000 },
             { "darkgreen"      , 0x00008000 },
             { "yellow"         , 0x0000ffff },
             { "none"           , 0xff000000 },
             { "transparent"    , 0xff000000 },
             { "dimgray"        , 0x00696969 },
             { "buttontext"     , GetSysColor(COLOR_BTNTEXT     ) },
             { "threedshadow"   , GetSysColor(COLOR_BTNSHADOW   ) },
             { "threedface"     , GetSysColor(COLOR_BTNFACE     ) },
             { "threedhighlight", GetSysColor(COLOR_BTNHIGHLIGHT) },
             { "window"         , GetSysColor(COLOR_WINDOW      ) },
             { "buttonface"     , GetSysColor(COLOR_BTNFACE     ) },
             { "indianred"      , 0x005c5ccd },
             }InitLocal;
        //
        SLocal* find = FindLocal( ptr );
        if ( find != NULL )
            return find->color;
        //
        // if ( *ptr ) ASSUNO
        // return (COLORREF)atoi(ptr);
    }
    ptr++;
    int dl = lstrlen( ptr );
    ASSERT( dl > 0 && ( dl % 2 ) == 0 );
    COLORREF n = 0;
    for ( int k = 0; k < dl; k++ )
    {
        char chr = ptr[k];
        if( '0' <= chr && chr <= '9') chr -= '0';
        else
            if( 'a' <= chr && chr <= 'f') chr -= ('a'-10);
            else
                if( 'A' <= chr && chr <= 'F') chr -= ('A'-10);
                else
                {
                    ASSERT(0);
                }
        n = n*16 + chr;
    }
    char* ch = (char*)&n;
    char  hc = ch[0];
    ch[0] = ch[2];
    ch[2] = hc;
    return (n & 0xFFFFFF);
    //  return ((_LongMac(n)>>8) & 0xFFFFFF) | 0x2000000;
}
//
inline BYTE MM( BYTE b, int n )
{
    return MulDiv( b, n, 4 );
}
//
COLORREF M_14( COLORREF color )
{
    RGBTRIPLE* t = (RGBTRIPLE*)&color;
    t->rgbtBlue  = MM( t->rgbtBlue , 3 );
    t->rgbtGreen = MM( t->rgbtGreen, 3 );
    t->rgbtRed   = MM( t->rgbtRed  , 3 );
    return color;
}
//
COLORREF M_34( COLORREF color )
{
    RGBTRIPLE* t = (RGBTRIPLE*)&color;
    t->rgbtBlue  = MM( t->rgbtBlue , 1 );
    t->rgbtGreen = MM( t->rgbtGreen, 1 );
    t->rgbtRed   = MM( t->rgbtRed  , 1 );
    return color;
}
//
COLORREF M_12( COLORREF color )
{
    RGBTRIPLE* t = (RGBTRIPLE*)&color;
    t->rgbtBlue  = MM( t->rgbtBlue , 2 );
    t->rgbtGreen = MM( t->rgbtGreen, 2 );
    t->rgbtRed   = MM( t->rgbtRed  , 2 );
    return color;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool LoadImageFile( const char* str, SImage& img, bool bMakeBmp = false )
{
    ASSERT( str && *str );
    if ( !(str && *str) ) return false;
    //
    CharMP szPath,szExt;
    _splitpath( str, 0, 0, 0, szExt );
    //  _fullpath(  szPath, str, SizeMP );
    //
#if 0
  if ( lstrcmpi( szExt, ".wmf" ) == 0 )
    {
    HDC hDC = ::CreateMetaFile( szPath );
    if ( hDC != NULL )
      img.hWmf = ::CloseMetaFile(hDC);
    return img.hWmf != NULL;
    }
  else
#else
    if ( lstrcmpi( szExt, ".wmf" ) == 0 )
    {
        lstrcpy( szExt, ".bmp" );
        int dl = lstrlen( szPath );
        lstrcpy( szPath+dl-4, szExt );
        // if (!theApp.FileExists(szPath))
        // {
        //     lstrcpy( szExt, ".wmf" );
        //     lstrcpy( szPath+dl-4, szExt );
        //     //    GsrvR gsrv = GSRV( szExt );
        //     // if ( gsrv )
        //     //   img.hBmp = (*gsrv)(szPath);
        //     return img.hBmp != NULL;
        // }
    }
#endif
    //  if ( lstrcmpi( szExt, ".emf" ) == 0 )
    //    {
    //    img.hEmf = ::EmfCreate();
    //    if ( !::EmfOpen( img.hEmf, szPath ) )
    //      ::EmfDelete( img.hEmf );
    //    return img.hEmf != NULL;
    //    }
    //  else
    //  if ( lstrcmpi( szExt, ".bmp" ) == 0
    //  ||   lstrcmpi( szExt, ".gif" ) == 0
    //  ||   lstrcmpi( szExt, ".jpg" ) == 0
    //  ||   bMakeBmp && (lstrcmpi( szExt, ".png" ) == 0 )
    //     )
    //    {
    //    GsrvR gsrv = GSRV( szExt );
    //    if ( gsrv )
    //    img.hBmp = (*gsrv)(szPath);
    //    return img.hBmp != NULL;
    //    }
    //  else
    //  if ( lstrcmpi( szExt, ".png" ) == 0 )
    //    {
    //    if(!theApp.FileExists(szPath))
    //      return false;
    // USES_CONVERSION;
    // img.hPng = new Gdiplus::Image(T2CW(szPath));
    //    return img.hPng != NULL;
    //    }
    //  else
    //  if ( !*szExt ) return false;
    //  //
    ASSERT(0);
    return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool LoadImages( const char* str, SImage& img, char* dst, bool bMakeBmp )
{
    // ASSERT( str && *str );
    // if ( !(str && *str) ) return false;

    // bool bLoad = LoadImageFile( str, img, bMakeBmp );
    // if ( !bLoad )
    //   {
    //   const char* find = FindAbstract( str );
    //   if ( find != NULL )
    //     bLoad = LoadImageFile( find, img, bMakeBmp );
    //   if ( bLoad && dst )
    //     lstrcpy( dst, find );
    //   }
    // else
    // if ( dst != NULL )
    //   {
    //   CharMP szPath,szExt;
    //   _splitpath( str, 0, 0, 0, szExt );
    //   _fullpath(  szPath, str, SizeMP );
    //     lstrcpy( dst, szPath );
    //   }
    // //
    // return bLoad;
    return true;
}
//
// Bitmap* BMP(const char* pszName)
//   {
//   Bitmap* bmp = NULL;
//   WIN32_FILE_ATTRIBUTE_DATA info;
//   if ( ::GetFileAttributesEx( pszName, GetFileExInfoStandard, &info ) )
//     {
//     if ( info.dwFileAttributes != -1 )
//       {
//       wchar_t wPath[_MAX_PATH];
//       if ( gFuncPipe )
//         (*gFuncPipe)( pszName );
//       MultiByteToWideChar(CP_ACP, 0, pszName, -1, wPath, _MAX_PATH);
//       //
// 	    bmp = Bitmap::FromFile( wPath );
//       }
//     }
// 	return bmp;
//   }
//
void OutImage ( HDC hDC, SImage& rImg, cross::RECT& rect, bool transparent )
{
    // if ( rImg.hEmf )
    //   {
    //   ::EmfHoney( rImg.hEmf, hDC, cross::RECT );
    //   }
    // else
    // if ( rImg.hBmp )
    //   {
    //   LPBITMAPINFO info = (LPBITMAPINFO)::GlobalLock( rImg.hBmp );
    //   if ( info != NULL )
    //     {
    //     LPBYTE bits = (LPBYTE)info;
    //     bits += sizeof( BITMAPINFOHEADER );
    //     if ( info->bmiHeader.biClrImportant > 0 )
    //       bits += info->bmiHeader.biClrImportant*sizeof(COLORREF);
    //    Graphics g(hDC);
    // g.SetInterpolationMode( InterpolationModeNearestNeighbor);
    // Bitmap bmp(info, bits);
    // ImageAttributes attr;
    // Color edge; bmp.GetPixel(0,0, &edge);
    // if ( (Version == AM2_NAK || Version == UKR_PRP || RCOL_SI == Version || Version >= BERTH_ONPZ ) && transparent )
    //           attr.SetColorKey(edge, edge);
    // g.DrawImage(&bmp, cross::RECTF((Gdiplus::REAL)cross::RECT.left, (Gdiplus::REAL)cross::RECT.top, (Gdiplus::REAL)(cross::RECT.right - cross::RECT.left), (Gdiplus::REAL)(cross::RECT.bottom - cross::RECT.top)),
    // 	0, 0, (Gdiplus::REAL)bmp.GetWidth(), (Gdiplus::REAL)bmp.GetHeight(), UnitPixel, &attr);
    //     ::GlobalUnlock( rImg.hBmp );
    //     }
    //   }
    // else
    // if ( rImg.hPng )
    //   {
    //    Graphics g(hDC);
    // Image * bmp = rImg.hPng;
    // g.SetInterpolationMode( InterpolationModeNearestNeighbor);
    // if((Version < DOTF_PM)
    // 	|| !transparent) //рисовать в выделеный прямоугольник?
    //       g.DrawImage(bmp, cross::RECTF((Gdiplus::REAL)cross::RECT.left, (Gdiplus::REAL)cross::RECT.top, (Gdiplus::REAL)bmp->GetWidth(), (Gdiplus::REAL)bmp->GetHeight()),
    // 	0, 0, (Gdiplus::REAL)bmp->GetWidth(), (Gdiplus::REAL)bmp->GetHeight(), UnitPixel);
    //       else
    //       g.DrawImage(bmp, cross::RECTF((Gdiplus::REAL)cross::RECT.left, (Gdiplus::REAL)cross::RECT.top, (Gdiplus::REAL)cross::RECT.right-cross::RECT.left, (Gdiplus::REAL)cross::RECT.bottom-cross::RECT.top),
    //    0, 0, (Gdiplus::REAL)bmp->GetWidth(), (Gdiplus::REAL)bmp->GetHeight(), UnitPixel);
    //   }
    // else
    // if ( rImg.hWmf )
    //   {
    //   ::PlayMetaFile( hDC, rImg.hWmf );
    //   CMyBrush brush( hDC, 0xff8000 );
    //   brush.cross::RECT( cross::RECT );
    //   }
    // else
    //   {
    //   CMyBrush brush( hDC, 0xff80 );
    //   brush.cross::RECT( cross::RECT );
    //   }
}

void Background( HDC hDC, SImage& rImg, cross::RECT& rect )
{
    // if ( rImg.hEmf )
    //   {
    //   ::EmfHoney( rImg.hEmf, hDC, cross::RECT );
    //   }
    // else
    // if ( rImg.hBmp )
    //   {
    //   LPBITMAPINFO info = (LPBITMAPINFO)::GlobalLock( rImg.hBmp );
    //   if ( info != NULL )
    //     {
    //     LPBYTE bits = (LPBYTE)info;
    //     bits += sizeof( BITMAPINFOHEADER );
    //     if ( info->bmiHeader.biClrImportant > 0 )
    //       bits += info->bmiHeader.biClrImportant*sizeof(COLORREF);
    //     int nMode = ::SetStretchBltMode( hDC, HALFTONE);
    //     POINT pt = {cross::RECT.left,cross::RECT.top};
    //     while ( pt.x < cross::RECT.right )
    //       {
    //       pt.y = cross::RECT.top;
    //       while ( pt.y < cross::RECT.bottom )
    //         {
    //         ::StretchDIBits( hDC, pt.x, pt.y,
    //           info->bmiHeader.biWidth, info->bmiHeader.biHeight,
    //             0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
    //               bits, info, DIB_RGB_COLORS,SRCCOPY);
    //         pt.y += info->bmiHeader.biHeight;
    //         }
    //       pt.x += info->bmiHeader.biWidth;
    //       }
    //     ::SetStretchBltMode( hDC, nMode );
    //     ::GlobalUnlock( rImg.hBmp );
    //     }
    //   }
    // else
    //   {
    //   CMyBrush brush( hDC, 0xff );
    //   brush.cross::RECT( cross::RECT );
    //   }
}
// Ищет картинку в системной библиотеке
SImage ImagesR300( const char* name, bool bMakeBmp )
{
    CharMP szPath,szCurr;
    //  GetCurrentDicross::RECTory(SizeMP,szCurr);
    CharMP szExt, tmpname;
    _splitpath( name, 0, 0, 0, szExt );
    //
    SImage img;
    if ( (bMakeBmp && (img.hBmp == NULL)) || (!bMakeBmp && !img.Yes()) )
    {
        //   sprintf_s( szPath, SizeMP, "%s%s\\Images\\", ::GetBasePath(), R300 );
        SetCurrentDirectory(szPath);
        LoadImages( name, img, NULL, bMakeBmp );
    }
    //
    if ( (bMakeBmp && (img.hBmp == NULL)) || (!bMakeBmp && !img.Yes()) )
    {
        //   sprintf_s( szPath, SizeMP, "%s%s\\Toolbar\\", ::GetBasePath(), R300 );
        SetCurrentDirectory(szPath);
        LoadImages( name, img, NULL, bMakeBmp );
    }
    //
    if ( (bMakeBmp && (img.hBmp == NULL)) || (!bMakeBmp && !img.Yes()) )
    {
        //   sprintf_s( szPath, SizeMP, "%s%s\\AlarmStateIcon\\", ::GetBasePath(), R300 );
        SetCurrentDirectory(szPath);
        LoadImages( name, img, NULL, bMakeBmp);
    }
    //
    /*if ( img.hBmp == NULL )
    {
    sprintf_s( szPath, SizeMP, "%s%s\\Img\\", ::GetBasePath(), R300 );
    SetCurrentDicross::RECTory(szPath);
    LoadImages( name, img, NULL, bMakeBmp);
    }*/
    SetCurrentDirectory(szCurr);

    if ( (img.hBmp == NULL) && (_stricmp(szExt, ".bmp")==0))
    {
        strcpy_s(tmpname, name);
        strcpy_s(tmpname+strlen(tmpname)-4, 6, ".png");
        img = ImagesR300( tmpname, bMakeBmp);
        // if ( (bMakeBmp && (img.hBmp == NULL)) || ((!bMakeBmp) && (img.hPng == NULL)) )
        // {
        //     strcpy(tmpname+strlen(tmpname)-4, ".gif");
        //     img = ImagesR300( tmpname, true );
        // }
    }    

    return img;
}
//
HGLOBAL GlobalR300( const char* name, bool bMakeBmp )
{
    SImage img = ImagesR300( name, bMakeBmp );
    return img.hBmp;
}

HBITMAP BitmapR300( SImage& rImage, SIZE& size  )
{
    // cross::RECT cross::RECT = { 0, 0, 0, 0 };
    // if ( rImage.hBmp )
    // {
    //     LPBITMAPINFO info = (LPBITMAPINFO)::GlobalLock( rImage.hBmp );
    //     if ( info != NULL )
    //     {
    //         ::Setcross::RECT( &cross::RECT, 0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight );
    //         ::GlobalUnlock( rImage.hBmp );
    //     }
    // }
    // // else
    // // if ( rImage.hPng )
    // //    ::Setcross::RECT( &cross::RECT, 0, 0, rImage.hPng->GetWidth(), rImage.hPng->GetHeight() );
    // // //
    // // size.cx = max(size.cx,cross::RECT.right);
    // // size.cy = max(size.cy,cross::RECT.bottom);
    // //
    // HDC hDC = ::CreateDC("DISPLAY",NULL,NULL,NULL);
     HBITMAP hBitmap;
    //= ::CreateCompatibleBitmap( hDC, cross::RECT.right, cross::RECT.bottom );
    // if ( hDC != NULL )
    // {
    //     HDC hdc = ::CreateCompatibleDC( hDC );
    //     if ( hdc != NULL )
    //     {
    //         HBITMAP hOld = (HBITMAP)::SelectObject( hdc, hBitmap );

    //         if(Version!=LG35_8_KF)
    //         {
    //             HBRUSH brush = ::CreateSolidBrush(RGB(240,240,240));
    //             ::Fillcross::RECT(hdc, &cross::RECT, brush);
    //             ::DeleteObject(brush);
    //             OutImage( hdc, rImage, cross::RECT, true );
    //         }
    //         else OutImage( hdc, rImage, cross::RECT );
    //         ::SelectObject( hdc, hOld );
    //         ::DeleteDC( hdc );
    //     }
    //     ::DeleteDC( hDC );
    // }
    return hBitmap;
}

HBITMAP BitmapR300( HGLOBAL hGlobal, SIZE& size )
{
    SImage img;
    img.hBmp = hGlobal;
    return BitmapR300( img, size );
}
//
inline void Shift( char* text, int N )
{
    while ( *text )
    {
        *text = text[N];
        if ( !*text )
            break;
        text++;
    }
}
//
void AnalizText( cross::CString& out, const char* pszText )
{
    // cross::CString str;
    // str = pszText;
    // char* text = const_cast<char*>(str.GetBuffer());
    // ::AnalizText( text );// Убираем &xxx;
    // //
    // while ( *text == ' ' ) text++;
    // int dl = lstrlen(text)-1;
    // if ( dl > 0 )
    // while ( text[dl] == ' ' || text[dl] == 0xa || text[dl] == 0xd )
    //   {
    //   text[dl--] = 0;
    //   if ( dl < 0 ) break;
    //   }
    // //
    // int n = 0;
    // while ( text[n] )
    //   {
    //   if ( text[n] == 0x0d && text[n+1] == 0x0a )
    //     {
    //     Shift( text+n, 2 );
    //     if ( text[n] == 0x0d && text[n+1] == 0x0a )
    //       n += 1;
    //     }
    //   //
    //   if ( text[n] == '<' )
    //     {
    //     if ( StrCmpN( text+n, "<P>", 3 ) == 0 )
    //       Shift( text+n, 3 ),n--;
    //     else
    //     if ( StrCmpN( text+n, "</P>", 4 ) == 0 )
    //       Shift( text+n, 4 ),n--;
    //     else
    //     if ( StrCmpN( text+n, "<BR>", 4 ) == 0 )
    //       text[n] = '\n', Shift( text+n+1, 3 );
    //     }
    //   n++;
    //   };
    // //
    // dl = lstrlen(text)-1;
    // if ( dl > 0 )
    // while ( text[dl] == ' ' || text[dl] == 0xa || text[dl] == 0xd )
    //   {
    //   text[dl--] = 0;
    //   if ( dl == 0 ) break;
    //   }
    // //
    // //WCHAR* w = (WCHAR*)text;
    // //if ( *text == 0xd0 )
    // //  {
    // //  int yy = 90;
    // //  }
    // out = text;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool IsNaN(const char* ptr,double& val)
{
    if ( ::IsFlt(ptr) )
    {
        val = atof(ptr);
        return true;
    }
    val = NaN;
    while ( *ptr == ' ' ) ptr++;
    int n = 0;
    while ( *ptr && *ptr == '-' ) ptr++,n++;
    if ( n > 2 )
    {
        while ( *ptr == ' ' ) ptr++;
        if ( !*ptr ) return true;
    }
    return false;
}
//-----------------------------------------------
inline int IsProc( char*& value )
{
    if ( lstrcmpi( value, "%" )  == 0 )
        return 0;
    if ( lstrcmpi( value, "px" ) == 0 )
        return 1;
    static char* ex = "expression(";
    static int   dl = lstrlen(ex);
    //expression(-roundcross::RECT001.style.pixelHeight + 41)
    if ( _strnicmp( value, ex, dl ) == 0 )
    {
        value += dl;
        int ld = lstrlen(value)-1;
        value[ld] = 0;
        return 2;
    }
    ASSERT(0);
    return 3;
}
//
void SRect::Pars( CStyle& a, CHTML& html, char* value, int n )
{
    v[n] = Number( value );
    int is = IsProc(value);
    if ( is == 0 )  p[n] = true;
    else
        if ( is == 2 )
        {
            /*
    CVBScript mScript;
    mScript.SetVars( &html, NULL, NULL );
    if ( !mScript.Compile( value, NULL, false ) )
      {
      //m_pLink = NULL;
      return;
      }
    mScript.Post();
    */
            v[n] = 2000;
        }
    t[n] = true;
}

// PointF Scross::RECT::SetXY( REAL x, REAL w, int n )
//   {
//   if ( t[n+1] && p[n+1] )
//     {
//     ASSERT(0);//Нужен пример
//     }
//   //
//   if ( p[n+0] ) v[n+0] = (REAL)(x+w*v[n+0]*0.01);
//   if ( p[n+1] ) v[n+1] = (REAL)(x+w*v[n+1]*0.01);
//   if ( p[n+2] ) v[n+2] = (REAL)(  w*v[n+2]*0.01);
//   //
//   PointF pt(x,w);
//   //
//   if ( t[n+0] && t[n+1] && !t[n+2] ) // Заданы left-right
//     pt.X = v[n+0], pt.Y = v[n+1]-v[n+0];
//   else
//   if ( t[n+0] && t[n+2] && !t[n+1]  ) // Заданы left-width
//     pt.X = v[n+0], pt.Y = v[n+2];
//   else
//   if ( !t[n+0] && !t[n+1] && t[n+2]  ) // Задана только ширина (например - Page)
//     pt.Y = v[n+2];
//   else
//   if ( !t[n+0] && !t[n+1] && !t[n+2]  )// Ничего не задано (например - BODY)
//     {}
//   else
//   // Это нашёл в sysAlarmSummary
//   if ( t[n+0] && !t[n+1] && !t[n+2]  ) // Задана только left
//     {
//     pt.X =   v[n+0];
//     pt.Y = w-v[n+0];
//     }
//   else
//   if ( t[n+1] && t[n+2] && !t[n+0]  ) // Заданы right-width
//     {
//     pt.X = w-v[n+1]-v[n+2];
//     pt.Y = v[n+2];
//     }
//   else
//     {
//     ASSERT(0);//Это надо посмотреть
//     }
//   //
//   return pt;
//   }
//
bool Disp( CHTML& html, CTag* tag )
{
    do
    {
        CStyle& a = tag->Style(html);
        if ( a.m_dwFlags & HDX_DISPLAY )
            return true;
        tag = tag->m_pPar;
    }
    while ( tag );
    return false;
}
// left,right,width  - 0,1,2
// top,bottom,height - 3,4,5
void SRect::SetRect( CHTML& html, CTag* tag, bool bResize )
{
    //   // На мнемосхемах bRelative == true не нашёл
    //   // На мнемосхемах bDisplay  == true не нашёл
    //   //
    //   CStyle& a = tag->Style(html);
    //   bool bRelative = ( a.m_dwFlags & HDX_RELATIVE ) != 0;
    //   bool bDisplay  = Disp( html, tag );
    //   //
    //   char*  id = a.NameID( html );
    // #ifdef _DEBUG
    //   if ( lstrcmp( id, "car_shp_car" ) == 0 )
    //     AAA();
    //   if ( lstrcmp( id, "car_shp_cist_fill" ) == 0 )
    //     AAA();
    // #endif
    //   cross::RECTF cross::RECT;
    //   CTag* par = tag->m_pPar;
    //   CTag* obj = par;
    //   if ( par )
    //     {
    //     if ( bRelative )
    //       {
    //     	INT_PTR n = par->GetSize();
    //       if ( n > 0 )
    //         obj = par->GetAt(n-1);
    //       else
    //         AAA();
    //       }
    //     CStyle& b = obj->Style(html);
    //     char*  id2 = b.NameID( html );
    //     cross::RECT         = b.mcross::RECT;
    //     cross::RECT.X      += b.mL.padding;
    //     cross::RECT.Y      += b.mT.padding;
    //     cross::RECT.Width  -= (b.mL.padding+b.mR.padding);
    //     cross::RECT.Height -= (b.mT.padding+b.mB.padding);
    //     }
    //   //roundcross::RECT001
    //   //
    //   PointF x = SetXY( cross::RECT.X, cross::RECT.Width , 0 );
    //   PointF y = SetXY( cross::RECT.Y, cross::RECT.Height, 3 );
    //   //
    //   if ( bDisplay )
    //     {
    //     if ( bRelative )
    //       {
    //       a.mcross::RECT.X      = x.X+cross::RECT.X;//+cross::RECT.Width ;
    //       a.mcross::RECT.Y      = y.X+cross::RECT.Y;//+cross::RECT.Height;
    //       a.mcross::RECT.Width  = x.Y;
    //       a.mcross::RECT.Height = y.Y;
    //       // Бляха-муха
    //       if ( obj )
    //         {
    //         CStyle& b = obj->Style(html);
    //         const char* name = b.GetName (html);
    //         if ( lstrcmpi( name, "IMG" ) == 0 )
    //           {
    //           CTag* par = tag->m_pPar;
    //           CStyle& c = par->Style(html);
    //           a.mcross::RECT.X += b.mcross::RECT.Width;
    //           a.mcross::RECT.Width = c.mcross::RECT.X+c.mcross::RECT.Width-a.mcross::RECT.X;
    //           }
    //         }
    //       }
    //     else
    //       {
    //       a.mcross::RECT.X     += x.X;
    //       a.mcross::RECT.Y     += y.X;
    //       a.mcross::RECT.Width  = x.Y;
    //       a.mcross::RECT.Height = y.Y;
    //       }
    //     }
    //   else
    //     {
    //     if ( bRelative )
    //       {
    //       ASSERT(0);// Я этого не видел
    //       a.mcross::RECT.X      = x.X+cross::RECT.X;//+cross::RECT.Width ;
    //       a.mcross::RECT.Y      = y.X+cross::RECT.Y;//+cross::RECT.Height;
    //       a.mcross::RECT.Width  = x.Y;
    //       a.mcross::RECT.Height = y.Y;
    //       }
    //     else
    //       {
    //       // Бляха-муха
    //       if ( par )
    //         {
    //         CStyle& b = par->Style(html);
    //         const char* name = b.GetName (html);
    //         bool bS = (strcmp(id, "car_shp_cist_fill")==0 || strcmp(id, "car_shp_cist_bg")==0) && Version==BERTH_ONPZ;
    //         if ( lstrcmpi( name, "Shape" ) == 0 || bS)
    //           {
    //           x.X = a.mcross::RECT.X;
    //           y.X = a.mcross::RECT.Y;
    //           REAL x0 = a.mcross::RECT.X+a.mcross::RECT.Width;
    //           REAL y0 = a.mcross::RECT.Y+a.mcross::RECT.Height;
    //           REAL x1 = x.X+x.Y;
    //           REAL y1 = y.X+y.Y;
    //           if ( x1 > x0 ) x.Y -= (x1-x0);
    //           if ( y1 > y0 ) y.Y -= (y1-y0);
    //           }
    //         }
    //       a.mcross::RECT.X      = x.X;
    //       a.mcross::RECT.Y      = y.X;
    //       a.mcross::RECT.Width  = x.Y;
    //       a.mcross::RECT.Height = y.Y;
    //       }
    //     }
    //   //
    //   if (bResize) //на некотрорых мнемосхемах элементы вылезают за границы
    //      html.m_pageSize.cy =  max(LONG(a.mcross::RECT.Y+a.mcross::RECT.Height), html.m_pageSize.cy);
    //
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
int NextGroup( int nGroup )
{
    //Sergej13
    /*
  nGroup++;
  SGroupID* grp = NULL;
  while ( grp == NULL )
    {
    grp = GroupFind(nGroup);
    if ( grp != NULL )
      return grp->ID;
    nGroup++;
    if ( nGroup >= 400 )
      nGroup = 1;
    }
    */
    return -1;
}
//
int PrevGroup( int nGroup )
{
    //Sergej13
    /*
  nGroup--;
  SGroupID* grp = NULL;
  while ( grp == NULL )
    {
    grp = GroupFind(nGroup);
    if ( grp != NULL )
      return grp->ID;
    nGroup--;
    if ( nGroup < 1 )
      nGroup = 400;
    }
    */
    return -1;
}
// Вынужденная мера
void GiveDACA( char* dst, const char* src )
{
    lstrcpy( dst, src );
    char* fnd = strchr( dst, '.' );
    if (  fnd ) *fnd = 0;
    lstrcat( dst, ".DACA" );
}
//
COLORREF GetColorIndicateAlarmMinus1( BYTE nPriority )
{
    if (Version==LG35_8_KF)
    {
        if ((nPriority>1)&&(nPriority<5))
            return 0x0000ff;//красный
        return 0;
    }
    else
        if (Version==DOTF_PM)
        {
            switch ( nPriority )
            {
            case 2: return RGB(255,255,0);//желтый
            case 3:
            case 4:
                return RGB(255,0,0);  //красный
            };
            return 0;
        }
    return GetColorPriorityAlarm( nPriority );
}

COLORREF GetColorPriorityAlarm( BYTE nPriority )
{
    if (Version==DOTF_PM)
    {
        switch ( nPriority )
        {
        case 2: return RGB(255,255,0);//желтый
        case 3:
        case 4:
            return RGB(255,0,0);  //красный
        };
        return 0xff000000;
    }
    else
        if (Version==AM2_NAK || Version==DMW_NAK || Version==ITH_NAK)
        {
            switch ( nPriority )
            {
            case 2:
            case 3:
            case 4:
                return RGB(255,0,0);  //красный
            };
        }

    switch ( nPriority )
    {
    case 2: return 0xffff00;//голубой
    case 3: return 0x00ffff;//желтый
    case 4: return 0x0000ff;//красный
    };
    return 0xff000000;
}

COLORREF GetColorPriorityAlarm( KHoneyPipeClient& pipe, cross::SParamValueH& val )
{
    //   //EnumSTATUS(None     ,0,"N 00")
    //   //EnumSTATUS(Journal  ,1,"J 00")
    //   //EnumSTATUS(Low      ,2,"L 00")
    //   //EnumSTATUS(High     ,3,"H 00")
    //   //EnumSTATUS(Emergncy ,4,"U 00")
    //   BYTE var = pipe.ValueB_H( val );
    //   return GetColorPriorityAlarm(var);
    //   }
    // //
    // bool AckTag( KHoneyPipeClient& pipe, int nNumber )
    //   {
    //   // Используем факт одинакового смещения переменной
    //   _static SValueDef* def = ::NameToValueEx( id_AICHANNEL, "UnackAlarmExists" );
    //   _static SParamValueH var;
    //   var.nNumber = nNumber;
    //   var.def     = def;
    //   BYTE noAck  = pipe.ValueB_H( var, 0 );
    //   // if ( noAck == 0 )
    //   //   return false;
    //   //

    //   //MIHAIL_HWL 7 строчек нижних закомментировал
    //   //CAlarmBase* al = pipe.Alarm( nNumber );
    //   //if ( al != NULL )
    //   //  {
    //   //  if ( al->setACK )
    //   //    return false;
    //   //  }
    //   //else return false;
    //   //
    //   _static SSendToModel send;
    //   _static BYTE bOld = 0;
    //   _static BYTE bNew = 1;
    //   send.Set( enumValueChr, &bOld, &bNew );
    //   lstrcpy( send.szValue, "Квитирование" );
    //   pipe.SendData( nNumber, send, noAck != 0 );
    //   // Изврат
    //   const char* name = pipe.GetMnemoName( nNumber );
    //   if ( name )
    //     {
    //     const char* find = strstr( name, ".PIDA" );
    //     if ( find )
    //       {
    //       GiveDACA( send.szName, name );
    //       CShDATAACQ sh;
    //       pipe.GetData( id_DATAACQ, send.szName, &sh );
    //       if ( IsTypeOK )
    //         {
    //         if ( sh.setACK )
    //           return false;
    //         send.eData = (EDataTypes)id_DATAACQ;
    //         send.Set( enumValueChr, &bOld, &bNew );
    //         pipe.SendData( send, sh.notACK != 0 );
    //         }
    //       }
    //     }
       return true;
}
//
UINT QueryAcyObj( LPCTSTR name, LPTSTR szHoney, LPTSTR szParam )
{
    // return Client().FindHoney( name, szHoney, szParam );

    return 0;
}

LPCTSTR QueryFaceplate( LPCTSTR name, LPTSTR szObj, UINT* pType, const char * fp )
{
    //  name = Alias(name);
    //  if (fp && strstr(fp, "sysdtlmos"))
    //  {
    // if (!strchr(name, '.'))
    // 	sprintf(szObj, "%s.%s", name, "MOS");
    // else
    // 	sprintf(szObj, "%s", name);
    // if (pType) *pType = (EDataTypes)id_FLAG;
    // return "MOS";
    //  }
    //  else if (fp && strstr(fp, "sysdtlflaga_rr_fp")){
    //   return NULL;
    //  }
    //  else if (fp && strstr(fp, "sysdtlpos"))
    //  {
    // if (!strchr(name, '.'))
    // 	sprintf(szObj, "%s.%s", name, "POS");
    // else
    // 	sprintf(szObj, "%s", name);
    // return "POS";
    //  }
    //  else if (fp && strstr(fp, "sysDtlInterlocka"))
    //  {
    // if (!strchr(name, '.'))
    // 	sprintf(szObj, "%s.%s", name, "INTERLOCKA");
    // else
    // 	sprintf(szObj, "%s", name);
    // return "INTERLOCKA";
    //  }
    //  lstrcpy( szObj, name );
    //  ::CharUpper( szObj );
    //  //
    //  UINT nType = QueryAcyObj( szObj );
    //  if ( !::IsAcyEx( nType ) )
    //    return NULL;
    //  //
    //  struct SLocal { const char* name; EDataTypes type; };
    //  _static SLocal list[] =
    //    {
    //    { "PIDA"    , (EDataTypes)id_PID     },
    //    { "DEVCTLA" , (EDataTypes)id_DEVCTL  },
    //    { "DATAACQA", (EDataTypes)id_DATAACQ },
    //    { "DACA"    , (EDataTypes)id_DATAACQ },
    //    { "FLAGA"   , (EDataTypes)id_FLAG    },
    // { "DB"   , (EDataTypes)id_FLAG    },
    // { "DB_LSL"   , (EDataTypes)id_FLAG    },
    // { "LR" , (EDataTypes)id_DEVCTL  },


        // //MIHAIL_HWL добавил новый, пока пустой вызывается 353.htm PD1246
        // { "AUXCALCA"   , (EDataTypes)id_AUXCALC   },
        // { "DIGACQA"   , (EDataTypes)id_DIGACQ   },
        // { "SWITCHA"   , (EDataTypes)id_SWITCH   },
        // //MIHAIL_HWL таких точек здесь нет
        //    //{ "ANA"     , (EDataTypes)id_AN      },
        //    //{ "STA"     , (EDataTypes)id_ST      },
        // //MIHAIL_HWL AUTOMAN было закомментировано
        //    { "AUTOMANA", (EDataTypes)id_AUTOMAN },
        //    {NULL}
        //    };
        //  //
        //  if ((Version == LG35_8_KF) || (Version >= DOTF_PM))
        //  {
        //     if( (nType == id_OVRDSEL) || (nType ==id_DEVCTL) || (nType == id_PIDFF)
        //         || (Version != LG35_8_KF) && ((nType==id_SWITCH) || (nType==id_FLAG) || (nType==id_AUXCALC) || (nType==id_DATAACQ)
        //         || (nType==id_DOCHANNEL)  || (nType==id_DICHANNEL || nType==id_AUTOMAN || nType==id_TOTALIZER || nType==id_REGCALC)
        //         || (nType==id_CONTROLMODULE)
        //         )
        //       )
        //       {
        //         if ( pType )
        //            *pType = nType; //id_CONTROLMODULE;
        //      return "SCANGRPDTL";
        //       }
        //     else
        //     if((nType == id_DIGACQ) && strstr(name, ".STATE"))
        //     {
        //         strcpy(szObj, name);
        //         *strstr(szObj, ".STATE")=0;
        //         strcat( szObj, ".DEVCTLA");
        //         nType = QueryAcyObj( szObj );
        //         if( ::IsAcyEx( nType ) )
        //           if ( pType )
        //             *pType = nType;
        //         return "SCANGRPDTL";
        //     }
        //     else
        //     if( IsSMAcy( nType ) )
        //     {
        //        if ( pType )
        //            *pType = nType;
        //        return "GROUPDETAILDISPLAY";
        //     }
        //     else
        //  if( IsQBAcy( nType ) )
        //     {
        //        if ( pType )
        //            *pType = nType;
        //        return "INSTRUCTIONDISPLAY";
        //     }
        //  }
        //  //
        //  if ( pType ) *pType = nType;
        //  int n = 0;
        //  if ( nType != id_CONTROLMODULE )
        //    {
        //    const char* p = strrchr( name, '.' );
        //    if ( p != NULL )
        //    while ( list[n].name )
        //      {
        //      if ( nType == list[n].type )
        //   {
        // 	  if ( nType == id_DIGACQ )
        // 		  return "DIGACQA";
        // 	  if (Version == LG35_8_KF && nType == id_SWITCH)
        // 		  return "SWA";
        // 	  if (Version == LG35_8_KF && nType == id_FLAG && strcmp(p+1,"START")==0)
        // 		  return "FLAGA";
        // 	  if ( lstrcmpi( p+1, list[n].name ) == 0 )
        // 		return list[n].name;
        //   }
        //      n++;
        //      }
        //    //
        //    if ( p == NULL )
        //    while ( list[n].name )
        //      {
        //      if ( nType == list[n].type )
        //        return list[n].name;
        //      n++;
        //      }
        //    if ( nType == id_NUMERIC )
        //      {
        //      _static char* n = "ANA";
        //      return n;
        //      }
        //    return NULL;
        //    }
        //  //
        //  while ( list[n].name )
        //    {
        //    sprintf_s( szObj, SizeMP, "%s.%s", name, list[n].name );
        //    UINT nType = QueryAcyObj( szObj );
        //    if ( pType ) *pType = nType;
        //    if ( ::IsAcy( nType ) )
        //      return list[n].name;
        //    n++;
        //    }

        //   if ( nType == id_CONTROLMODULE )
        //   {
        //      char name2[MAX_PATH];
        //      strcpy_s(name2, name);
        //      if(char *ref = strchr(name2, '.'))
        //        *ref = 0;
        //      strcpy_s( szObj, SizeMP, name2 );
        //      if ( pType )
        //          *pType = nType;
        //      return "SCANGRPDTL";
        //   }

        //  return NULL;
        return "";
}
//
cross::SParamValueH GrupExperion( CHTML& html, int nNumber )
{
    // CDataTrends& mData = CViewTrend::Data();
    cross::SParamValueH val;
    // STrends* item = mData.ItemStr(html.Group());
    // if ( item == NULL )
    //   return val;
    // ASSERT( 0 <= nNumber && nNumber < MAX_TREND );
    // SPoint& pt = item->point[nNumber];
    // const char* name  = mData.Text(pt.hPoint);
    // const char* value = mData.Text(pt.hParam);
    // return FindExperion( html, name, value );
    return val;
}
//
cross::SParamValueH FindExperion( CHTML& html, const char* nameIn, const char* value, UINT* pnType )
{
    // const char *ref = NULL;
    // char name[256]="";
    // strcpy_s(name, nameIn);
    // if ( html.m_DisplayDataRepository.size() )
    // {
    //  if((strncmp(nameIn, "{%", sizeof("{%")-1)==0) && (ref = strstr(nameIn+sizeof("{%"), "%}")))
    //  {
    //     char namekey[64]="";
    //     strncpy_s(namekey, nameIn+sizeof( "{%")-1, ref-nameIn-sizeof( "{%")+1);
    //     std::string namekeyval =  html.FindInDisplayDataRepository(namekey);
    //     strcpy_s(name, namekeyval.c_str());
    //  }
    //  else
    //  {
    //      strcpy_s(name, html.FindInDisplayDataRepository(name).c_str());
    //  }
    //  if((strncmp(value, "{%", sizeof("{%")-1)==0) && (ref = strstr(value+sizeof("{%"), "%}")))
    //  {
    //   char key[64]="";
    //   strncpy_s(key, value+sizeof( "{%")-1, ref-value-sizeof( "{%")+1);
    //   if (Version != LG35_8_KF || (strcmp(key, "InterlockName")!=0)) //{%InterlockName%} обрабатываются отдельно
    //   { //обработка {%какое_то_имя%}
    //   char val[128];
    //   std::string curkeyval =  html.FindInDisplayDataRepository(key);
    //   sprintf_s(val, "%s%s", curkeyval.c_str(), ref+2);
    //   SParamValueH pv = FindExperion( html.pipe, name, val, pnType );
    //   SParamValueHMulti *pvm = new SParamValueHMulti(html.m_DisplayDataRepository);
    //   if(pvm)
    //     {
    //     pv.pMulti = pvm;
    //     strcpy_s(pvm->curkeyval, curkeyval.c_str());
    //     strcpy_s(pvm->key, key);
    //     pvm->mDefs[curkeyval] = std::pair<int, SValueDef*>(pv.nNumber, pv.def);
    //     for(std::map<std::string, std::string>::iterator itt = html.m_DisplayDataRepository.begin(); itt != html.m_DisplayDataRepository.end(); ++itt)
    //       {
    //          std::string curparamvalReal = html.FindInDisplayDataRepository(itt->second);
    //          if(pvm->mDefs.find(curparamvalReal) == pvm->mDefs.end())
    //            {
    //             sprintf_s(val, "%s%s", itt->second.c_str(), ref+2);
    //             SParamValueH pv2 = FindExperion( html.pipe, name, val);
    //             if((pv2.nNumber >=0) && pv2.def)
    //               pvm->mDefs[itt->second] = std::pair<int, SValueDef*>(pv2.nNumber, pv2.def);
    //             else KKK();
    //            }
    //       }
    //     }
    //   return pv;
    //   }
    //  }
    // }
    // //else
    // if (Version == LG35_8_KF || Version == L35_11600_KF)
    //  {
    //   if( strcmp(value, "CPCA.SSTATE") == 0)  //этого нет на сервере - подменил на похожее
    //     return FindExperion( html.pipe, name, "CEESTATE" );
    //   else
    //   if( strcmp(value, "SPCA.SSTATE") == 0)  //этого нет на сервере - подменил на похожее
    //     return FindExperion( html.pipe, name, "CEESTATE" );
    //  }
    // return FindExperion( html.pipe, name, value, pnType);

    cross::SParamValueH val;
    return val;

}
//

#include <QbBaseType_.h>
bool ReplaceControlModuleWithQBType(cross::SParamValueH& val, UINT nType, CharMP szHoney, CharMP szParam,KHoneyPipeClient& rPipe )
{
    // if ( nType == id_CONTROLMODULE )
    // 	{
    // 		nType = id_QBANALOG;
    // 		val.def = NameToValueEx( nType, szParam );
    // 		if ( val.def != NULL )
    // 		{
    // 			val.nNumber = rPipe.FindObj(nType,szHoney,szHoney);
    // 			return true;
    // 		}
    // 		else
    // 		{
    // 			nType = id_QBSTATUS;
    // 			val.def = NameToValueEx( nType, szParam );
    // 			if ( val.def != NULL )
    // 			{
    // 				val.nNumber = rPipe.FindObj(nType,szHoney,szHoney);
    // 				return true;
    // 			}
    // 		}
    // 	}
    // return false;
    return true;
}

cross::SParamValueH FindExperion( KHoneyPipeClient& rPipe, const char* name, const char* value, UINT* pnType )
{
    cross::SParamValueH val;
    //   if ( !*name && !*value )
    //     return val;
    //   name = Alias(name);
    //   //
    // #ifdef _DEBUG
    //   if ( lstrcmpi( name, "BZTA6701" ) == 0 )
    //     KKK();
    // #endif
    //   if ( lstrcmpi( value, "Description" ) == 0 )
    // 	  value="DESC";
    //   else
    //   if ( lstrcmpi( value, "AssociatedDisplay" ) == 0 )
    // 	  value="SCANASSOCDSP";
    //   else
    //   if ( lstrcmpi( value, "FullItemName" ) == 0 )
    // 	  value="NAME";
    //   //
    //   CharMP szFull,szHoney,szParam;
    //   const char * t;
    //   if (Version == LG35_8_KF && (t = strstr(value, "{%InterlockName%}")))
    //   {
    //   sprintf_s( szFull, "%s.INTERLOCKA%s", name, t+17 );
    //   }
    //   else
    //   if ((Version == LG35_8_KF) && ((strcmp(name, "PK2E1.START")==0)||(strcmp(name, "PK1E1.START")==0)))
    //   {
    //      if(strnicmp(value, "flaga.", sizeof("flaga.")-1)==0)
    //         sprintf_s( szFull, "%s.%s", name, value + sizeof("falga.")-1 );
    //      else sprintf_s( szFull, "%s.%s", name, value );
    //   }
    //   else
    //   {
    //   if(value[0])
    //     sprintf_s( szFull, "%s.%s", name, value );
    //   else strcpy_s( szFull, name );
    //   }
    //   char* param = strrchr( szFull, '.' );
    //   //
    //   UINT nType = rPipe.FindHoney( szFull, szHoney, szParam );
    //   if(pnType)
    //     *pnType = nType;
    //   if ( ::IsAcyEx(nType) )
    //     {
    // 	val.def = NameToValueEx( nType, szParam );
    //     if ( val.def != NULL )
    //       {
    //       val.nNumber = rPipe.FindObj(nType,szHoney,szHoney);
    //       return val;
    //       }
    //     else
    // 	{
    // 	  if (lstrcmp(szParam,"DESCRIPTION")==0)
    // 	  {
    // 		  lstrcpy(szParam,"DESC");
    // 		  val.def = NameToValueEx( nType, szParam );
    // 		  if ( val.def != NULL )
    // 		  {
    // 			  val.nNumber = rPipe.FindObj(nType,szHoney,szHoney);
    // 			  return val;
    // 		  }
    // 	  }
    //       else
    //       if(Version == DOTF_PM)
    //       {
    //         if (nType == id_SMDISCRET && stricmp(szParam,"PV")==0)
    //           {//заплатка - нет PV
    //           lstrcpy(szParam, "PVFL");
    //           val.def = NameToValueEx( nType, szParam );
    // 		  if ( val.def != NULL )
    // 		    {
    // 			  val.nNumber = rPipe.FindObj(nType,szHoney,szHoney);
    // 			  return val;
    // 		    }
    //           }
    //       }
    // 	  //два объекта CONTROLMODULE и QBANALOG с одним именем,
    // 	  //запрашивается QBANALOG, но FindHoney возвращает тип CONTROLMODULE
    // 	  if (Version==UKR_PRP || Version==T1163_ONPZ || Version==SGK_ONPZ)
    //          if(ReplaceControlModuleWithQBType(val, nType, szHoney, szParam, rPipe ))
    // 			return val;

    //       val.nNumber = -2;

    //       if(IsQBAcy(nType))
    //       {
    //         if(pnType)
    //         {
    //            *pnType = nType;
    //            val.nNumber = -4;
    //         }
    //         return val;
    //       }

    //       if ( lstrcmpi( szParam, "CP_PVFORMAT" ) == 0 ) return val;
    //       if ( lstrcmpi( szParam, "CP_EUDESC"   ) == 0 ) return val;
    // #if 1
    //       lstrcat( szParam, " - " );
    //       #undef   HONEY_TYPE
    //       #define  HONEY_TYPE( a, b, c ) case id_##b: lstrcat( szParam, c ); break;
    //       switch ( nType )
    //         {
    //         #include "HoneywellType.hpp"
    //         }
    //       //
    //       #define _MAX_BAD_PAR 1024
    //       _static CharMP szBad[_MAX_BAD_PAR];
    //       if ( ::IsMnemoStyle(MNEMO_COMMENT) )
    //       for ( int n = 0; n < _MAX_BAD_PAR; n++ )
    //       if ( *szBad[n] )
    //         {
    //         if ( lstrcmpi( szBad[n], szParam ) == 0 )
    //           return val;
    //         }
    //       else
    //         {
    //         if ( lstrcmpi( szParam, "OP - FLAG" ) == 0 )
    //           KKK();
    //         if ( lstrcmpi( szParam, "PIDA.PV - CONTROLMODULE" ) == 0 )
    //           KKK();
    //         lstrcpy( szBad[n], szParam );
    //         HFILE hFile = _lcreat( "C:/ForDebug/BadParam.txt", 0 );
    //         if ( hFile != HFILE_ERROR )
    //           {
    //           for ( int m = 0; m <= n; m++ )
    //           if ( *szBad[m] )
    //             {
    //             _hwrite( hFile, szBad[m], lstrlen(szBad[m]) );
    //             _hwrite( hFile, "\r\n", 2 );
    //             }
    //           else break;
    //           _lclose( hFile );
    //           }
    //         break;
    //         }
    // #endif
    //       return val;
    //       }
    //     }
    //   val.nNumber = -2;
    //   // код для сбора отсутствующих точек
    //   //char line[512];
    //   //sprintf(line, "%s.%s\r\n", name, value);
    //   //OutputDebugStringA(line);
    //   if ( lstrcmpi( value, "CP_PVFORMAT" ) == 0 ) return val;
    //   if ( lstrcmpi( value, "CP_EUDESC"   ) == 0 ) return val;
    //   if ( lstrcmpi( value, "ALARMVALUE"  ) == 0 ) return val;
    //   //
    // #if 1
    //   #define _MAX_BAD 1024
    //   _static CharMP szBad[_MAX_BAD];
    //   if ( ::IsMnemoStyle(MNEMO_COMMENT) )
    //   for ( int n = 0; n < _MAX_BAD; n++ )
    //   if ( *szBad[n] )
    //     {
    //     if ( lstrcmpi( szBad[n], szFull ) == 0 )
    //       return val;
    //     }
    //   else
    //     {
    //     lstrcpy( szBad[n], szFull );
    //     HFILE hFile = _lcreat( "c:/ForDebug/BadHoney.txt", 0 );
    //     if ( hFile != HFILE_ERROR )
    //       {
    //       for ( int m = 0; m < _MAX_BAD; m++ )
    //       if ( *szBad[m] )
    //         {
    //         _hwrite( hFile, szBad[m], lstrlen(szBad[m]) );
    //         _hwrite( hFile, "\r\n", 2 );
    //         }
    //       else break;
    //       _lclose( hFile );
    //       }
    //     break;
    //     }
    // #endif
    return val;
}
//

const char* Alias( CHTML& html, CTag* pTag, char* value )
{
    do
    {
        CStyle& style = pTag->Style( html );
        // if ( style.m_dwFlags & HDX_ALIAS_TABLE )
        //     return style.FindAlias( html, value );
        // else
        //     pTag = pTag->m_pPar;
    }
    while ( pTag );
    return value;
}
//
UINT GetInt( LPCTSTR key, LPCTSTR entry, int nDefault)
{
    // CWinApp* pApp = AfxGetApp();
    // if ( pApp == NULL ) return nDefault;
    // return pApp->GetProfileInt( key, entry, nDefault );
    return 0;
}

BOOL WriteInt( LPCTSTR key, LPCTSTR entry, int nValue)
{
    // CWinApp* pApp = AfxGetApp();
    // if ( pApp == NULL ) return FALSE;
    // return pApp->WriteProfileInt( key, entry, nValue );
    return true;
}

int GetInt( LPCTSTR key, LPCTSTR entry, int nDef, int nMin, int nMax )
{
    // int nVal = GetInt( key, entry, nDef );
    // nVal = min( nVal, nMax );
    // nVal = max( nVal, nMin );
    // return nVal;
    return 0;
}

LPCTSTR GetString( LPCTSTR key, LPCTSTR entry, LPCTSTR pDefault )
{
    // CWinApp* pApp = AfxGetApp();
    // if ( pApp == NULL ) return pDefault;
    // cross::CString str = pApp->GetProfileString( key, entry, pDefault );
    // static char text[512];
    // lstrcpy( text, str );
    // return text;
    return (LPCTSTR)"";
}
BOOL WriteString( LPCTSTR key, LPCTSTR entry, LPCTSTR pValue)
{
    // CWinApp* pApp = AfxGetApp();
    // if ( pApp == NULL ) return FALSE;
    // return pApp->WriteProfileString( key, entry, pValue );
    return true;
}
BOOL GetBinary( LPCTSTR key, LPCTSTR entry, LPBYTE* ppData, UINT* pBytes)
{
    // CWinApp* pApp = AfxGetApp();
    // if ( pApp == NULL ) return FALSE;
    // return pApp->GetProfileBinary( key, entry, ppData, pBytes );
    return true;
}
BOOL WriteBinary( LPCTSTR key, LPCTSTR entry, LPBYTE   pData, UINT nBytes)
{
    // CWinApp* pApp = AfxGetApp();
    // if ( pApp == NULL ) return FALSE;
    // return pApp->WriteProfileBinary( key, entry, pData, nBytes );
    return true;
}

#define CURRENT_STATE  _T("Current State")
UINT GetInt( LPCTSTR entry, int nDefault )
{
    //  return GetInt( CURRENT_STATE, entry, nDefault );
    return 0;
}

BOOL WriteInt( LPCTSTR entry, int nValue )
{
    // return WriteInt( CURRENT_STATE, entry, nValue );
    return 0;
}

int GetInt( LPCTSTR entry, int nDef, int nMin, int nMax )
{
    //return GetInt( CURRENT_STATE, entry, nDef, nMin, nMax );
    return 0;
}

#include <fstream>
#include <algorithm>
#include <map>
static bool _loaded = false;
static bool _loaded_states = false;
static std::vector<std::string> _assets;
static std::vector<std::string> _groups;
static std::map<std::string, std::string> _locmap;
static std::map<std::string, std::string> _grpmap;
static std::map<std::string, std::string> _digmap;
static std::map<std::string, std::string> _flgmap;
void EnsureLoaded()
{
    if (!_loaded)
    {
        //	std::string path = GetBasePath();
        // path += "CBH\\AlarmProps.csv";
        // std::ifstream file(path.c_str());
        // if (file.is_open())
        // {
        // 	char line[128];
        // 	while(!file.getline(line, 128).eof())
        // 	{
        // 		char * _grp = strchr(line, ';'); *_grp = 0; _grp++;
        // 		char * _loc = strchr(_grp, ';'); *_loc = 0; _loc++;
        // 		std::string name(line);
        // 		std::string grp(_grp);
        // 		std::string loc(_loc);
        // 		if (grp.size() && std::find(_groups.begin(), _groups.end(), grp)==_groups.end()) _groups.push_back(grp);
        // 		if (loc.size() && std::find(_assets.begin(), _assets.end(), loc)==_assets.end()) _assets.push_back(loc);
        // 		if (grp.size()) _grpmap[name] = grp;
        // 		if (loc.size()) _locmap[name] = loc;
        // 	}
        // 	file.close();
        // 	std::sort(_assets.begin(), _assets.end(), std::less<std::string>());
        // 	std::sort(_groups.begin(), _groups.end(), std::less<std::string>());
        // 	_loaded = true;
        // }
    }
}
void EnsureLoadedStates()
{
    if (!_loaded_states)
    {
        // std::string path = GetBasePath();
        // path += "CBH\\AlarmStates.csv";
        // std::ifstream file(path.c_str());
        // if (file.is_open())
        // {
        // 	char line[128];
        // 	while(!file.getline(line, 128).eof())
        // 	{
        // 		char * _dig = strchr(line, ';'); *_dig = 0; _dig++;
        // 		char * _flg = strchr(_dig, ';'); *_flg = 0; _flg++;
        // 		std::string name(line);
        // 		std::string dig(_dig);
        // 		std::string flg(_flg);
        // 		if (dig.size()) _digmap[name] = dig;
        // 		if (flg.size()) _flgmap[name] = flg;
        // 	}
        // 	file.close();
        // 	_loaded_states = true;
        // }
    }
}
const char * GetAlmGroup(const char * block)
{
    EnsureLoaded();
    if(_grpmap.find(block) != _grpmap.end())
        return _grpmap[block].c_str();
    return NULL;
}
const char * GetLocation(const char * block)
{
    EnsureLoaded();
    if(_locmap.find(block) != _locmap.end())
        return _locmap[block].c_str();
    return NULL;
}
const char * GetDigaNormal(const char * block)
{
    EnsureLoadedStates();
    if(_digmap.find(block) != _digmap.end())
        return _digmap[block].c_str();
    return NULL;
}
const char * GetFlagNormal(const char * block)
{
    EnsureLoadedStates();
    if(_flgmap.find(block) != _flgmap.end())
        return _flgmap[block].c_str();
    return NULL;
}
std::vector<std::string>* GetAssets()
{
    EnsureLoaded();
    return &_assets;
}
std::vector<std::string>* GetAlmGroups()
{
    EnsureLoaded();
    return &_groups;
}
//MIHAIL_HWL 
int GUStantion()
{
    return 4;
}
void CorrectName(char* point, char* param)
{
    if (Version == AT_PRP)
    {
        ::CharUpper(point);
        ::CharUpper(param);
        char _str[_MAXHONEYNAME];
        char *str; str = _str;
        memset(str, 0, _MAXHONEYNAME);
        strcpy(str, point);
        int l = (int)strlen(str);
        str[l++] = '.';
        str[l] = 0;
        strcpy(str + l, param);
        char *t;
        t = str;
        while(*t)
        {
            switch(*t)
            {
                // case 'У': *t = 'Y'; break;
                // case 'К': *t = 'K'; break;
                // case 'Е': *t = 'E'; break;
                // case 'Н': *t = 'H'; break;
                // case 'З': *t = '3'; break;
                // case 'В': *t = 'B'; break;
                // case 'А': *t = 'A'; break;
                // case 'Р': *t = 'P'; break;
                // case 'О': *t = 'O'; break;
                // case 'С': *t = 'C'; break;
                // case 'М': *t = 'M'; break;
                // case 'Т': *t = 'T'; break;
            }
            t++;
        }
        //@591
        if (!strnicmp("TSA2115.PRM_NM", str, 14))
            strcpy(str+7, ".DACA.PV\0");
        //@574
        if (!_strnicmp("TOTAL_P2221.PLOTN_2221", str, 22))
            strcpy(str+22, ".PV\0");
        if (!_strnicmp("F221_WAY.TOTALIZER_RGS.PV", str, 25))
            strcpy(str, "F2221_WAY.TOTALIZER_RGS.PV\0");
        //@557
        if (!_strnicmp("TOTAL_P2234.PLOTN_2234", str, 22))
            strcpy(str+22, ".PV\0");
        //@557
        if (!_strnicmp("TOTAL_M2210_2211.MASSA_2210_2211", str, 32))
            strcpy(str+32, ".PV\0");
        //@550
        if (!_strnicmp("VI_E5_2.AUXCALC7.[7]", str, 20))
            strcpy(str+16, ".C[7]\0");
        //@524
        if (!_strnicmp("QR_2227_", str, 8) && !_strnicmp(".PLOT_PRIV_POTOK", str+9, 16))
            strcpy(str+25, ".PV\0");
        while(*str)
        {
            if (*str == '.')
            {
                str++;
                *point = 0;
                break;
            }
            else
            {
                *point = *str;
                point++;
                str++;
            }
        }
        while(*str)
        {
            *param = *str;
            param+=1;
            str+=1;
        }
    }
}

static const char* B64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int B64index[256] =
    {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
        0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
        0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

const std::string b64encode(const void* data, const size_t &len)
{
    std::string result((len + 2) / 3 * 4, '=');
    unsigned char *p = (unsigned  char*) data;
    char *str = &result[0];
    size_t j = 0, pad = len % 3;
    const size_t last = len - pad;

    for (size_t i = 0; i < last; i += 3)
    {
        int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
        str[j++] = B64chars[n >> 18];
        str[j++] = B64chars[n >> 12 & 0x3F];
        str[j++] = B64chars[n >> 6 & 0x3F];
        str[j++] = B64chars[n & 0x3F];
    }
    if (pad)  /// Set padding
    {
        int n = --pad ? int(p[last]) << 8 | p[last + 1] : p[last];
        str[j++] = B64chars[pad ? n >> 10 & 0x3F : n >> 2];
        str[j++] = B64chars[pad ? n >> 4 & 0x03F : n << 4 & 0x3F];
        str[j++] = pad ? B64chars[n << 2 & 0x3F] : '=';
    }
    return result;
}

const std::string b64decode(const void* data, const size_t &len)
{
    if (len == 0) return "";

    unsigned char *p = (unsigned char*) data;
    size_t j = 0,
        pad1 = len % 4 || p[len - 1] == '=',
        pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
    const size_t last = (len - pad1) / 4 << 2;
    std::string result(last / 4 * 3 + pad1 + pad2, '\0');
    unsigned char *str = (unsigned char*) &result[0];

    for (size_t i = 0; i < last; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad1)
    {
        int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
        str[j++] = n >> 16;
        if (pad2)
        {
            n |= B64index[p[last + 2]] << 6;
            str[j++] = n >> 8 & 0xFF;
        }
    }
    return result;
}

std::string b64encode(const std::string& str)
{
    return b64encode(str.c_str(), str.size());
}

std::string b64decode(const std::string& str64)
{
    return b64decode(str64.c_str(), str64.size());
}

std::string string_to_upper( const std::string& in )
{
    std::string tempstr = in;
    std::transform( in.begin(), in.end(), tempstr.begin(),[](unsigned char c) {
        return std::tolower(c);
    } );
    return tempstr;
}

std::wstring string_to_wstring( const std::string& in, std::locale loc)
{
    std::wstring out( in.length(), 0 );
    std::string::const_iterator ii = in.begin(), ie = in.end();
    std::wstring::iterator jj = out.begin();
    for( ; ii!=ie; ++ii, ++jj )
    {
        wchar_t wch=std::use_facet< std::ctype< wchar_t > > ( loc ).widen( *ii );
        if(!wch)
            *jj = L'?';
        else  *jj = wch;
    }

    return out;
}

std::string wstring_to_string( const std::wstring& in, std::locale loc)
{
    std::string out( in.length(), 0 );
    std::wstring::const_iterator ii = in.begin(), ie = in.end();
    std::string::iterator jj = out.begin();
    for( ; ii!=ie; ++ii, ++jj )
    {
        char ch= std::use_facet< std::ctype< wchar_t > > ( loc ).narrow( *ii );
        if(!ch)
            *jj = '?'; //символа нет в кодировке
        else  *jj = ch;          
    }

    return out;
}

std::string FindFile(const char* ppath, const char* pname)
{

    char szPath[512];
    sprintf_s( szPath, "%s%s", ppath, pname );
    std::string sres;
    //
    if(GetFileAttributes(szPath)!=0xFFFFFFFF)
        return szPath;
    //
    sprintf_s( szPath, "%s*", ppath );
    WIN32_FIND_DATA Find;
    HANDLE hFind = ::FindFirstFile( szPath, &Find );
    if ( hFind != INVALID_HANDLE_VALUE )
    {
        BOOL Res = TRUE;
        while ( Res )
        {
            // if ( ::IsDir( Find, true ) )
            //   {
            //   sprintf_s( szPath, "%s%s\\", ppath, Find.cFileName );
            //   sres = FindFile(szPath, pname);
            //   if(sres.size())
            //      return sres;
            //   }
            Res = ::FindNextFile( hFind, &Find );
        }
        ::FindClose( hFind );
    }
    return "";
}
