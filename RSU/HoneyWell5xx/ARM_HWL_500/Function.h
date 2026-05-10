
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#pragma once
#include "linuxTypes/cstring.h"
#include "crosswindows.h"


//#include <HoneyMnemoExtend.h>

// #include "ARM.h"
// //#include "Windows.h"
// #include "linuxTypes/cstring.h"
// #include "linuxTypes/cpoint.h"
// //#include "HTML/Hdx.h"
// #include <vector>
// #include <CommProc.h>

// class SParamValueH;

// class CStyle;


//
bool IsStrBOL(const char* ptr);
//
float Number( char*& a );
COLORREF ColorDiez( const char* ptr );
//
int NextGroup( int nGroup );
int PrevGroup( int nGroup );
//
COLORREF M_14( COLORREF color );
COLORREF M_34( COLORREF color );
COLORREF M_12( COLORREF color );
//
//Bitmap* BMP(const char* pszName);
//
struct SImage
  {
  SImage()
    {
    memset( this, 0, sizeof(SImage) );
    }
  //
  bool Yes()
    {
  //  return hEmf != NULL || hBmp != NULL || hWmf != NULL || hPng != NULL;
        return true;
    }
  //
  void Clear()
    {
 //   if ( hEmf ) ::EmfDelete     ( hEmf ); hEmf = NULL;
    if ( hBmp ) ::GlobalFree    ( hBmp ); hBmp = NULL;
    if ( hWmf ) ::DeleteMetaFile( hWmf ); hWmf = NULL;
   // if ( hPng ) delete hPng;              hPng = NULL;
    }
  //
//  HMyEMF    hEmf;
  HGLOBAL   hBmp;
  HMETAFILE hWmf;
//  Image *   hPng;
  };
//
bool LoadImages( const char* str, SImage& img, char* dst = NULL, bool bMakeBmp = false );
void OutImage  ( HDC hDC, SImage& rImg, RECT& rect, bool transparent = false );
void Background( HDC hDC, SImage& rImg, RECT& rect );
int  TrueGroupNumber( const char* ptr );
bool TrueMnemoName ( const char* name );
int  FindGroupNumber( const char* name );
int  FindTrendNumber( const char* name );
//
HGLOBAL GlobalR300( const char* name, bool bMakeBmp = false );
SImage  ImagesR300( const char* name, bool bMakeBmp = false );
HBITMAP BitmapR300( HGLOBAL hGlobal, SIZE& size  );
HBITMAP BitmapR300( SImage& rImage , SIZE& size  );
//
void GiveDACA( char* dst, const char* src );
//COLORREF GetColorPriorityAlarm( KHoneyPipeClient& pipe, SParamValueH& val );
COLORREF GetColorPriorityAlarm( BYTE nPriority );
COLORREF GetColorIndicateAlarmMinus1( BYTE nPriority );
//bool AckTag( KHoneyPipeClient& pipe, int nNumber );
LPCTSTR QueryFaceplate( LPCTSTR name, LPTSTR szObj, UINT* nType = NULL, const char * fp = 0 );
UINT QueryAcyObj( LPCTSTR name, LPTSTR szHoney = NULL, LPTSTR szParam = NULL );
//
enum EBorderStyle : char
  {
  eBorderUnk = 0,
  eBorderNone,
  eBorderInset,
  eBorderOutset,
  eBorderSolid,
  eBorderDashed,
  eBorderRidge,
  eBorderGroove,
  };
EBorderStyle BorderStyle( const char* ptr );
void AnalizText( cross::CString& out, const char* pszText );
//---------------------------------------------------------
class CTag;
class CHTML;
cross::SParamValueH GrupExperion( CHTML& html, int nNumber );
cross::SParamValueH FindExperion( CHTML& html, const char* name, const char* value, UINT* pnType = NULL );
//SParamValueH FindExperion( KHoneyPipeClient& rPipe, const char* name, const char* value, UINT* pnType = NULL );
const char* Alias( CHTML& html, CTag* pTag, char* value );
//---------------------------------------------------------
struct SRect
  {
  // left,right,width  - 0,1,2
  // top,bottom,height - 3,4,5
  float v[6];// Значение
  bool p[6];// В процентах
  bool t[6];// Установлено v
  void Pars( class CStyle& a, CHTML& html, char* value, int n );
  void SetRect( CHTML& html, class CTag* tag, bool bResize = false );
  //
 // PointF SetXY( REAL x, REAL w, int n );
  };
//---------------------------------------------------------
int GetInt( LPCTSTR key, LPCTSTR entry, int nDef, int nMin, int nMax );
int GetInt( LPCTSTR entry, int nDef, int nMin, int nMax );
UINT    GetInt     ( LPCTSTR entry, int nDefault = 0);
UINT    GetInt     ( LPCTSTR key, LPCTSTR entry, int nDefault = 0);
BOOL    WriteInt   ( LPCTSTR entry, int nValue);
BOOL    WriteInt   ( LPCTSTR key, LPCTSTR entry, int nValue);
LPCTSTR GetString  ( LPCTSTR key, LPCTSTR entry, LPCTSTR pDefault );
BOOL    WriteString( LPCTSTR key, LPCTSTR entry, LPCTSTR pValue);
BOOL    GetBinary  ( LPCTSTR key, LPCTSTR entry, LPBYTE* ppData, UINT* pBytes);
BOOL    WriteBinary( LPCTSTR key, LPCTSTR entry, LPBYTE   pData, UINT nBytes);
int GUStantion();

const int _MAXHONEYNAME = 64;
const char * GetAlmGroup(const char * block);
const char * GetLocation(const char * block);
const char * GetDigaNormal(const char * block);
const char * GetFlagNormal(const char * block);
std::vector<std::string>* GetAssets();
std::vector<std::string>* GetAlmGroups();
void CorrectName(char*, char*);
std::string b64encode(const std::string& str);
std::string b64decode(const std::string& str64);
std::string string_to_upper( const std::string& in );

#include <algorithm>
#include <locale>
std::wstring string_to_wstring( const std::string& in, std::locale loc  = std::locale("") );
std::string wstring_to_string( const std::wstring& in, std::locale loc  = std::locale("") );

std::string FindFile(const char* ppath, const char* pname);


#endif
