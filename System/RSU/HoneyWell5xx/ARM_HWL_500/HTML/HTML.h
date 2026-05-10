#pragma once
#ifndef HTML_H
#define HTML_H
#include "Tag.h"
//#include "TagSCRIPT.h"
//#include <UniBuffer.h>
//#include "HTML_Path.h"
#include <string>
#include <map>
//#include "BaseHTML.h"
//#include <Windows.h>
//#include "BaseHTML.h"

class CStyle;

enum EShowType95
{
    estNone,
    est95Name,
    est95EngEU,
    est95Hide,
};
//
#define _countofchar(str) (sizeof(str)/sizeof(char)-1)
//
class CHTML : public CTag
{
    void NotFoundM(bool bWrite);
    void SetMnemoM(bool bWrite);
    char ms_chVer;
    static std::vector<std::string> m_ClietPath;
public:
    bool m_bFileLoaded;
    CHTML(cross::CWnd* pWnd,KHoneyPipeClient& _pipe);
    //CHTML();
    CHTML(KHoneyPipeClient& _pipe);
    ~CHTML(void);
    // bool LoadHTM( HWND hWnd, LPCTSTR pszName, LPCTSTR szTagGrp );
    bool LoadHTM( LPCTSTR pszName, LPCTSTR szTagGrp );
    bool LoadHTM( const cross::CString* str );

    bool Buffer();
    void ClearPage();
    bool FindFile( const char* pszFolder, char* szPath );
    bool FindName( char* szPath );
    cross::CString m_strFile,m_strFull,m_strObj,m_strGroup;
    const char* m_strPV;
    const char* m_strReset;
    const char* m_strDesc;
    EShowType95 m_eShow95;
    void SaveBin();
    void ApplyLayout( float fScale, SIZE sz );
    void SetPopup(bool b){m_bPopup=b;}
    bool GetPopup(){return m_bPopup;}
    CTag* m_tagTargShape140;
    CTag* m_pAreaPane;
    bool m_bResize; //нужно прверить не вылезают ли элементы за размеры страниц и увеличть её в этом случае
    char  m_szTitle[512]; //тоже что m_strTitle, но заполняется скриптами
    char WINDOWS_EXTERNAL_CURRENTPAGE[512]; //имя мнемосхемы для открытия (из скрипта)
    ULONG m_nTimerCount;
    bool m_bNeedPipeEnd;
    UINT m_LastTagCount;   //последнее число элементов в массиве pipe
    CTagSCRIPT* m_pScriptClick;
    CTag* m_psrcelement;
    POINT m_eventpoint;
    //
    SIZE     Size () {return m_pageSize ;} void Size (SIZE     v) { m_pageSize  = v;}
    COLORREF Color() {return m_pageColor;}
    void Color(COLORREF v)
    {
        m_pageColor = v;
    }
    cross::CString& Title() {return m_strTitle ;} void Title(LPCTSTR  v) { m_strTitle  = v;}
    cross::CString& Group() {return m_strGroup ;} void Group(LPCTSTR  v) { m_strGroup  = v;}
    void setPPVals(const char* pv, const char * rst, const char * desc );
    //
    void Print( bool bWrite );
    bool m_bPrint;
    bool m_bName140;
    //
    CStyle &Style(int n);
    int AddStyle(int n = 1) {
        // if ( !m_bBinData )
        // if ( !mStyle.Realloc(n) ) return 0;
        // return mStyle.AddCurr(n);
        return 0;
    }
    //
    static std::string GetSafeviewCategory(const char* name);
    //
    SBaseDSD* DSD(int n){SBaseDSD* list = (SBaseDSD*)mDsdPR.m_szBuffer;return list+n ;}
    bool AddDSD(int n){ return mDsdPR.Realloc(n);}
    int CurrDsd(int n){ return mDsdPR.AddCurr(n);}
    //
    bool AddChar(int n){return mChars.Realloc(n);}
    char* Buffer(int n){return mChars.m_szBuffer+n;}
    int CurrChar(int n){return mChars.AddCurr(n);}
    //
    bool AddParam(int n){return mPlace.Realloc(n);}
    int CurrParam(int n){return mPlace.AddCurr(n);}
    // Используется свойство равенства структур
    SParamsData* Param(int n) { SParamsData* list = (SParamsData*)mPlace.m_szBuffer; return list+n;}
    POINT      * Point(int n) { POINT      * list = (POINT      *)mPlace.m_szBuffer; return list+n;}
    COLORREF   * Color(int n) { COLORREF   * list = (COLORREF   *)mPlace.m_szBuffer; return list+n*2;}
    int        * Int  (int n) { int        * list = (int        *)mPlace.m_szBuffer; return list+n*2;}
    double     * Doubl(int n) { double     * list = (double     *)mPlace.m_szBuffer; return list+n;}
    //
    int Current( int n, int set )
    {
        switch ( n )
        {
        case 0: if ( set >= 0 ) mPlace.m_nCurrent = set; return mPlace.m_nCurrent;
        case 1: if ( set >= 0 ) mChars.m_nCurrent = set; return mChars.m_nCurrent;
        case 2: if ( set >= 0 ) mStyle.m_nCurrent = set; return mStyle.m_nCurrent;
        case 3: if ( set >= 0 ) mDsdPR.m_nCurrent = set; return mDsdPR.m_nCurrent;
        };
        return -1;
    }
    //
    int Count( int n )
    {
        switch ( n )
        {
        case 0: return mPlace.Count();
        case 1: return mChars.Count();
        case 2: return mStyle.Count();
        case 3: return mDsdPR.Count();
        };
        return -1;
    }
    //
    HDC  m_hHDC;
    bool m_bR300;
    bool m_bPopup;
    bool m_bBinData;
    void ClearBuffers();
    CTag* mBody;
    CTag* blueRect;
    KHoneyPipeClient& pipe;
    cross::CWnd* mWnd;
    SIZE  m_pageSize;
    bool  m_LoadAcceleration;  //ускорение загрузки html
    //
    void Paint( HDC hDC );
    void Dlg(class CDlgDebug& dlg, cross::POINT& pt, int nPaint );
    CTag* PtInTag( cross::POINT& pt, bool bAll );
    CTag* PtInBoundTag( cross::POINT& pt ); //поиск первого видимого тега привязанного к модельной точке
    int  FindTags(cross::POINT point, std::vector<CTag*>& tags); //поиск всех видимых тегов (кроме "Page") под курсором
    bool  FindTitle( cross::POINT& pt, LPSTR title );
    CTag* TabIndex(cross::POINT& pt );
    //
    cross::SParamValueH* Value() { return (cross::SParamValueH*)mPipes.m_szBuffer; }
    SUniBuffer& PipeBuff() { return mPipes;}
    int m_nViewNameEu;
    void ShowOrHide( HWND hWnd, int n );
    std::map<std::string, std::string> m_DisplayDataRepository;
    std::string FindInDisplayDataRepository(std::string key, bool bCheck = false);
    void DisplayDataRepositorySetValue(std::string key, std::string val);

    std::map<std::string, O_VAL> m_CommomValue;
    void SetCommomValue(std::string key, O_VAL val) { m_CommomValue[key] = val; }
    const O_VAL* GetCommomValue(std::string key);

    CTag* FindTagInBodyById(CHTML& html, const char *nameid );
    void CtrlBlockSet140(char *id);
    std::string strCtrlBlockName;
    void AckAll();
    //
    std::map<std::string, SUB_STRUCT> m_Subroutines;
    void GetSubroutines(LPCSTR vv);
    static void GetSubroutines0(std::map<std::string, SUB_STRUCT> &_Subroutines, LPCSTR vv, char *sbegin, char *send, bool bFuction);
    void pagecomplete();
    static bool IsTrueMemoname( const char* szPath );
    //
private:
    SUniBuffer mPlace;
    SUniBuffer mChars;
    SUniBuffer mStyle;
    SUniBuffer mDsdPR;
    SUniBuffer mPipes;
    //
private:
    COLORREF m_pageColor;
    //SIZE     m_pageSize;
    cross::CString  m_strTitle;
    //
    _WIN32_FILE_ATTRIBUTE_DATA wfad[4];
};
#endif
