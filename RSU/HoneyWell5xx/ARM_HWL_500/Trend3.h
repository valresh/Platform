#pragma once
//
#include "UniBuffer.h"
#include "PipeTrends.h"
#include "TrendItem.h"
#include <map>
//
// Данные для групп трендов
struct SPoint
  {
  SPoint()
  : dMaxEU(NfN), dMinEU(NfN)
  , dMaxPR(100), dMinPR(0.0)
  , hPoint(INVALID_HNAME)
  , hParam(INVALID_HNAME)
  , color(0)
  , mView(true)
    {
    };
  HNAME hPoint,hParam;
  float dMaxEU,dMinEU;
  float dMaxPR,dMinPR;
  COLORREF color;
  bool     mView;
  bool IsYou( const char* name, class CDataTrends* data );
  };
//
struct STitle
  {
  STitle()
  : hTitle   (INVALID_HNAME)
  , nRazmah  (5)
  , nInterval(1)
  , nScaleEng(0)
  , nNumber  (0)
  , nSelect  (0)
    {
    }
  HNAME hTitle;
  int  nNumber;
  BYTE nSelect;
  BYTE nRazmah;
  BYTE nInterval;
  BYTE nScaleEng;
  };
//
struct STrends : public STitle
  {
  STrends(COLORREF* arrColor = NULL)
    {
    if ( arrColor )
    for ( int n = 0; n < MAX_TREND; n++ )
      point[n].color = arrColor[n%32];
    }
  SPoint point[MAX_TREND];
  //
  bool TrueGrup()
    {
    for ( int n = 0; n < MAX_TREND; n++ )
    if ( point[n].hPoint != INVALID_HNAME )
      return true;
    return false;
  }
  //
  bool FindGrup( const char* name, class CDataTrends* data )
    {
    for ( int n = 0; n < MAX_TREND; n++ )
    if  ( point[n].IsYou(name,data) )
      return true;
    return false;
    }
  };
//
class CDataTrends : public SUniBuffer
  {
  SUniName mName;
  SUniChar mChar;
  // Для записи в бинарный файл
  _WIN32_FILE_ATTRIBUTE_DATA mFileAttr;
  char ms_chVer;
  CharMP m_szBin;
  public:
    CDataTrends(const char* pszFile,const char* find);
    virtual ~CDataTrends();
    void LoadFile(const char* pszFile,const char* find);
    void SaveFile();
	void SaveCvs();
    void FillCombo( CComboBox& wnd );
	std::map<std::string,int> FillMap();
    void SetCurSel( CComboBox& wnd, int N );
    int  ItemNumber(int N);
    int  PrevTrend( int N );
    int  NextTrend( int N );
    STrends* Item(UINT n)
      {
      return (STrends*)Obj(n);
      }
    STrends* ItemStr(LPCTSTR N)
      {
      int n = atoi(N)-1;
      return (STrends*)Obj(n);
      }
    //
    LPCTSTR  Text( HNAME   n ) { return mName.Name(n);}
    HNAME AddText( LPCTSTR n ) { return mName.Add (n);}
    //
    int TrueGrup( const char* ptr  );
    int FindGrup( const char* name );
  protected:
    void ParserTxt(char* ptr,const char* find);
    void ParserXml(char* ptr,const char* find);
  };
//
inline bool StrScn( const char* name, const char* text, int& m )
  {
  int dl = lstrlen( text );
  if ( _strnicmp( name, text, dl ) == 0 )
    {
    m = atoi( text+dl );
    m = max( m, 1 );
    m = min( m, MAX_TREND );
    return true;
    }
  return false;
  }
//
