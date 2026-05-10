#pragma once
#ifndef TAG_H
#define TAG_H
//#include "ARM.h"
//#include "..\\DlgDebug.h"
//#include "..\\Wnd\\HTMLWndBase.h"
//#include "HTML.h"
#include "R300\\InterlockContainer.h"


//#include "Dsd.h"
#include <vector>
#include <string.h>
//#include "InterlockContainer.h"
//#include <typeinfo>
//

//#include "Style.h"
//#include "Types/CArray.h"
//#include "Helpers/staticHelpers.h"

 #include "../linuxTypes/CString.h"
 #include <linuxTypes/cpoint.h>

#include "Style.h"
// #include "HoneyMnemoExtend.h"
#include <map>
 #include <ARM.h>
 #include <Function.h>
 #include "BaseHTML.h"
#include <Function.h>

#undef  LOCALS
#define LOCALS HDC hDC, CHTML& html
//
#undef  KERDYK
#define KERDYK HWND hWnd, CHTML& html

#undef  ANALIZ
#define ANALIZ CHTML& html, STegScn* teg

#undef  MAX_QUQU
#define MAX_QUQU 8000

//class EVisibility;

struct SMnemo
{
    std::string name;
    int nCount;
    bool bInvert;
    char list[128][32];
};
enum _TagType
{
    TT_TAG,
    TT_SHAPE,
    TT_SPAN,
    TT_TEXTAREAWITHCALLBACK,
};
typedef enum _TagType TagType;

struct SCRIPT_SOURSE_DATA
{
    int eType;
    std::string  m_ScriptText;
    SCRIPT_SOURSE_DATA() {eType=-1;};
};

struct SUB_STRUCT
{
    bool bFunction;
    std::string body;
    std::vector<std::string> params;  
};

enum Script127 : unsigned int
{
    usAll_OnClick_me_ = 0,
    usCDA_Devctl_Hialm_RedTag_OnUpdate_me_,
    usCDA_Device3States_AlpGPV_OnUpdate_me_,
    usCDA_DigState_AlpPV_OnUpdate_me_,
    usCDA_DigStateButton_AlpPV_OnUpdate_me_,
    usCDA_DigStateCColor_AlpPV_OnUpdate_me_,
    usCDA_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_,
    usCDA_ExecState_Alp_OnUpdate_me_,
    usCDA_Graph_AlpError_OnUpdate_me_,
    usCDA_Numeric_AlpPVFormat_OnUpdate_me_,
    usCDA_OnOffValves_AlpGPV_OnUpdate_me_,
    usCDA_RegCntl_Mode_Hialm_RedTag_EUDesc_OnUpdate_me_,
    usCDA_RegCntlValve_AlpVal_OnUpdate_me_,
    usCDA_RegCtlValves_RedTag_BadCtlFl_OnUpdate_me_,
    usGraphic_KHABAROVSK_OnMouseOver_me_,
    usGraphic_KHABAROVSK_OnMouseUp_me_,
    usNav_SelectBox_OnClick_me_,
    usScd_AbsorberStatus_Khabarovsk_AlpPV_OnUpdate_me_,
    usScd_Device3States_AlpPV_OnUpdate_me_,
    usScd_DigState_AlpPV_OnUpdate_me_,
    usScd_DigStateButton_AlpPV_OnUpdate_me_,
    usScd_DigStateCColor_AlpPV_OnUpdate_me_,
    usScd_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_,
    usScd_Graph_AlpError_OnUpdate_me_,
    usScd_Khabarovsk_BoxLocRem_AlpPV_OnUpdate_me_,
    usScd_Khabarovsk_Bypass_AlpPV_OnUpdate_me_,
    usScd_Khabarovsk_Description_onupdate_me_,
    usScd_Khabarovsk_DigStateCColor_AlpPV_OnUpdate_me_,
    usScd_Khabarovsk_Discrepancy_AlpPV_OnUpdate_me_,
    usScd_Khabarovsk_Enclav_onupdate_me_,
    usScd_Khabarovsk_Interlock_OnUpdate_me_,
    usScd_KHABAROVSK_RotorkValvesLocalRemote_AlpPV_OnUpdate_me_,
    usScd_libKHABAROVSK_Bypass_AlpPV_OnUpdate_me_,
    usScd_libKHABAROVSK_Interlock_AlpPV_OnUpdate_me_,
    usScd_Numeric_AlpPVFormat_OnUpdate_me_,
    usScd_OnOffValves_AlpPV_OnUpdate_me_,
    usScd_RegCntl_Mode_Hialm_EUDesc_OnUpdate_me_1_,
    us_RP1516CB_TextBox_UpdateScript,
    usScd_Khabarovsk_InitAnalog_AlpPV_OnUpdate_me_,
    usScd_Khabarovsk_Actuador_AlpPV_OnUpdate_me_,
    usScd_Khabarovsk_InitDigital_AlpPV_OnUpdate_me_,
    __RecPV1_0140213,
    __RecPV2_0140213,
    __RecPV3_0140213,
    __RecPV1_0140220,
    __TxtMode001,
    usScd_OnOffValves_Khabarovsk_AlpPV_OnUpdate_me_,
    usScd_RegCntlValve_Khabarovsk_AlpVal_OnUpdate_me_,
    __HWLineHeater14_2500109,
    __HWLineHeater13_2500109,
    __HWLineHeater23_2500115,
    __HWLineHeater24_2500115,
    __HWLineHeater33_2500201,
    usScd_RegCntlValve_AlpVal_OnUpdate_me_,
    usScd_Khabarovsk_DamperValves_4State_AlpPV_OnUpdate_me_,
    usCDA_Damper_AlpVal_OnUpdate_me_,

    usCda_CEXXX_AlpPV_OnUpdate_me_,
    usLK2B_Cda_CEXXX_AlpPV_OnUpdate_me_,
    usLK2B_Cda_CEXXX_ConAlpTP_OnUpdate_me_,
    usLK2B_Cda_CEXXX_RecPV_OnUpdate_me_,
    usUZ_Cda_Graph_AlpError_OnUpdate_me_,
    usAll_ILBPGraph_AlpError_OnUpdate_me_,
    usCDA_Interlock_OnUpdate_me_,

    UPDATE_SCRIPTS_COUNT,
};

enum ClickType
{
    ctFaceplate,
    ctPopup,
    ctMnemo,
};



//CAPMApp theApp;

class CHTML;
class CTag;
//
typedef void (*PFnChange)(CHTML& html, CTag *pTag);
//
class CTag : public std::vector<CTag*>//, public SBaseHTML
{
protected:
    void BadRect( HDC hDC, CHTML& html, COLORREF color = 0xff0000 );
    CTag* AnalizSpecific( ANALIZ, char ID, void* );
public:

    auto begin() {
        return std::vector<CTag*>::begin();
    }

    auto end() {
        return std::vector<CTag*>::end();
    }

    size_t GetSize() {
        return std::vector<CTag*>::size();
    }

    CTag**  GetData() {
        return std::vector<CTag*>::data();
    }


    CTag();
    CTag(const CTag &) = default;
    CTag(CTag &&) = delete;
    CTag &operator=(const CTag &) = default;
    CTag &operator=(CTag &&) = delete;
    virtual ~CTag(void);
    void Clear();
    bool Parser( CHTML& html, char* szBuffer, char*& ptr, class CBin* bin, SCRIPT_SOURSE_DATA *pSriptData = NULL, CTag **ppTag = NULL);
    //
    CTag* PtInTag( CHTML& html, cross::POINT& pt, bool bAll, bool loadInnerFirst = false );
    CTag* PtInBoundTag( CHTML& html, cross::POINT& pt ); //поиск первого видимого тега привязанного к модельной точке
    int FindTags(CHTML& html, cross::POINT& pt, std::vector<CTag*>& tags); //поиск всех видимых тегов (кроме "Page") под курсором
    bool FindTitle( CHTML& html, cross::POINT& pt, LPSTR title );
    CTag* TabIndex( CHTML& html, cross::POINT& pt );
    //
    SInterlockContainer m_strInterlockContainer;
    virtual bool Close(){ return false;};
    virtual void PaintThis( LOCALS );
    //
    virtual TagType GetType(){ return TT_TAG; }
    virtual void Postik( LOCALS );
    virtual void Matrix( LOCALS );
    virtual void Analiz( ANALIZ ){}
    virtual void Kerdyk( KERDYK );
    virtual void Script( KERDYK );
    virtual void Release();
    //virtual void Action( CHTML& html, HWND hWnd, bool bDown,CPoint * pt = NULL );
    virtual void Action( CHTML& html, bool bDown,cross::CPoint * pt = NULL );
    //  virtual void DlgDebug( CDlgDebug& dlg, POINT& pt, int nPaint );
    //void WindowPos( HWND hWnd, CHTML& html, FLOAT fScale, POINT& ptOffset );
    int   FindNumber(CHTML& html);
    CTag* LinkScript(CHTML& html,UINT id);
    CTag* LinkScript(CHTML& html,const char* c);
    CTag* FindNearestTag(CHTML& html, CTag *pThisTag, const char* c);
    //   CTag* CTag::FindType(CHTML& html, const std::type_info &type);
    CTag* Objects( CHTML& html, const char* t );
    CTag* GetFlag( CHTML& html, DWORD dwFlag );
    bool  SetLink( CHTML& html, DWORD dwFlag );
    void  SetMVal( CHTML& html, cross::SParamValueH val );
    void  ShiftDY( CHTML& html, float dy );
    void  ScaleDY( CHTML& html, float dy, float scale );
    void  Disable( CHTML& html );
    //
    // void  NaviGo( CHTML& html, HWND hWnd, const char* mnemoname);
    void  NaviGo( CHTML& html, const char* mnemoname);
    //bool IsAction( CHTML& html, HWND hWnd, bool bDown );
    bool IsAction( CHTML& html, bool bDown );
    //bool ActionSysBtn( CHTML& html, HWND hWnd );
    bool ActionSysBtn( CHTML& html );
    //	bool IsS1Open_DEVCTLA(CHTML& html, SParamValueH& mVal);
    //
    INT_PTR FullCount();
    bool IsPresent(CHTML& html);
    //
    static CStyle& Style(CHTML& html);
    void AnalizStyle( CHTML& html, char* value );
    void AnalizHdx  ( CHTML& html, char* value );
    static int  m_nStyle;
    void InitStyle( CHTML& html );
    //
    CTag* m_pPar;
    SBaseDSD* Dsd(CHTML& html);
    bool IsSpecUpMenu089_Project(CStyle& style, CHTML& html);
    void GoToSpecScheme089_Project(CHTML& html);
    std::string GetSpecSchemeName089_Project(CHTML& html);
    bool IsRadioButtonFromFaceplate(CStyle& style, CHTML& html);
    CTag* FindTagById(CHTML& html, const char *nameid );
    void SetVisibility(CHTML& html, int vis);
    void ChangeTags(CHTML &html, PFnChange pFnChange);
    //
    bool m_bPressed;
    //
    // Если обрабатывается только один объект
    cross::SParamValueH mVal,mDbl;
    struct _userdefined
    {
        UINT nType;
        char param[256];
        _userdefined(){memset(this,0,sizeof(_userdefined));}
    }
    m_UserDefined;
    //SInterlockContainer mInterlockContainer;
    //
    void NotFound( CHTML& html, cross::CString* list, int& nCounts );
    void SetMnemo( CHTML& html, const char* mnemo, SMnemo* list, int& nCounts );
    // CHTMLWndBase* m_pWnd;
    //     if(bSuppressRotate)
    //       {  // CTagINPUT не поворачиваются
    //        ::ModifyWorldTransform( hDC, &style.m_XF, MWT_LEFTMULTIPLY );
    //       }
    //     }
    //   }
    //   //
    //   Postik( hDC, html );
    //   style.Posts( hDC, html );
    //   //
    //   if ( m_pWnd ) m_pWnd->Timer();
    //   }
    //
    // /*virtual*/ void CTag::Script( KERDYK )
    //   {
    //     INT_PTR nCount = size();
    //   CTag**     obj = data();
    // 	for ( INT_PTR n = 0; n < nCount; n++ )
    //     obj[n]->Script( hWnd, html );
    //   }
    //
    // void CTag::WindowPos( HWND hWnd, CHTML& html, FLOAT fScale, POINT& ptOffset )
    //   {
    //   if ( m_bNoPaint )
    //     return;
    //   //
    //   if ( m_pWnd )
    //     {
    //     CStyle& style = Style(html);
    // 	if (Version == LG35_8_KF && !html.m_strFile.CompareNoCase("systnd03")){
    // 		cross::RECT _rc;
    // 		::GetWindowcross::RECT(hWnd, &_rc);
    // 		style.m_rcTruecross::RECT.right = (_rc.right - _rc.left)/fScale;
    // 	}
    // #ifdef _DEBUG
    //     char * id = style.NameID(html);
    // #endif
    //     cross::RECT cross::RECT = style.m_rcTruecross::RECT;
    //     cross::RECT rc;
    //     rc.X = int(cross::RECT.left*fScale)-ptOffset.x;
    //     rc.Y = int(cross::RECT.top *fScale)-ptOffset.y;
    //     rc.Width  = int((cross::RECT.right-cross::RECT.left)*fScale);
    //     rc.Height = int((cross::RECT.bottom-cross::RECT.top)*fScale);
    //     if(mVal.pMulti==NULL)
    //     m_pWnd->WindowPos( hWnd, mVal.nNumber, style.m_hFont, fScale, rc );
    //     else{
    //          CHTMLEditBox *pEditBoxWnd  = dynamic_cast<CHTMLEditBox*>(m_pWnd);
    //          if(pEditBoxWnd==NULL)
    //             m_pWnd->WindowPos( hWnd, mVal.nNumber, style.m_hFont, fScale, rc );
    //          else pEditBoxWnd->WindowPos( hWnd, mVal, style.m_hFont, fScale, rc );
    //         }
    //     }
    //   //
    // 	INT_PTR nCount = GetSize();
    //   CTag**     obj = GetData();
    // 	for ( INT_PTR n = 0; n < nCount; n++ )
    //     obj[n]->WindowPos( hWnd, html, fScale, ptOffset );
    //   }
    //
    int HWBTN(CHTML& html,WORD nCode,UINT uKey);
    SImage  mImg;
    cross::CString m_constValue;
    bool m_bNoPaint;
    int  mParam;
    //
    //  int   GetCount(CHTML& html);
    //   const char* GetName (CHTML& html,int n);
    //  const char* GetValue(CHTML& html,int n);
    const char* GetName (CHTML& html);
    const char* GetValue(CHTML& html);
    bool  StopDebug(CHTML& html,LPCTSTR str = NULL);
    //
    bool ValueValue( CHTML& html, DWORD dw, struct SValueValue& val );
    bool IsYou( CHTML& html, const char* szID );
    //
    char  m_szScriptValue[512];
    char  m_szNumericDisplayValue[32];
    void  Values(CHTML& html);
    class CTagSCRIPT* m_pChange;
    class CTagSCRIPT* m_pClickM;
    class CTagSCRIPT* m_pMouseD;
    class CTagSCRIPT* m_pMouseU;
    class CTagSCRIPT* m_pPageComplete;
    class CHTMLWndBase* m_pLink;// Для связи с окнами
    void ShowOrHide( CHTML& html, LPCTSTR tag, bool bShow );
    bool FillClass( CHTML& html );
    int  StyleClass( HDC hDC, CHTML& html );
    void SetColor( CHTML& html, COLORREF color );
    bool FillText( CHTML& html, char* str, int TypePnt = 0 );
    bool OutDbl( CHTML& html, double val, char* str );
    bool OutChr( CHTML& html, BYTE   val, char* str );
    bool OutStr( CHTML& html, char*  val, char* str );
    bool OutInt( CHTML& html, int    val, char* str );
    bool SetColorForSpecClass(CHTML& html, int nStyleClass, double dValue);
    bool SetColorForSpecClass(CHTML& html, int nStyleClass, BYTE bValue);
  //  void Script127Exec( CTag* oSource, HDC hDC, CHTML& html );


    std::vector<Script127> m_vctrOnClickScripts;
    std::vector<Script127> m_vctrMouseOverScripts;
    std::vector<Script127> m_vctrMouseUpScripts;
    std::vector<Script127> m_vctrUpdateScripts;

    //	std::map<std::string, SParamValueH> vals;
    char  m_szFormat[32];
    bool m_bIsShape;
    bool m_bCancelChange;
    //скрипт прямо в теге:
    bool m_shelved;
    bool m_RTN;
    bool m_acknowledged;
    int  m_priority;

    UINT  m_ScriptTextId;  
    DWORD m_ScriptTexteType;
    int m_nControllerDigits; //число десятичных знаков в контроллере, если -1 на это число не ориетируемcя
    ClickType m_ClickType;
    std::map<std::string, std::string> m_Attributes; //устанавливаются и проверяются скриптами
    //
    static std::map<std::string, SUB_STRUCT> m_CommonSubroutines;
    static void LoadCommomScripts();
    static void LoadCommomScripts(char * subpath, char *sbegin);
    //
};
//
void AnalizSPAN    ( CTag* pTag, ANALIZ );
void AnalizTextArea( CTag* pTag, ANALIZ );
void AnalizDIV     ( CTag* pTag, ANALIZ );

#endif
