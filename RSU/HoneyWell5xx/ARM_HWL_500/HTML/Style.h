#pragma once
#ifndef STYLE_H
#define STYLE_H

//#include "Windows.h"


//custom includes

#include <avaloniaWrappers/AfxControls.h>
#include <avaloniaWrappers/Gdiplus.h>
//
#undef  LOCALS
#define LOCALS HDC hDC, CHTML& html
//
#undef  KERDYK
#define KERDYK CHTML& html
//
#undef  ANALIZ
#define ANALIZ CHTML& html, STegScn* teg
//
#define TYPE_BACKGROUND   0x80000000//id=Background
#define TYPE_PAGE         0x40000000//id=Page
#define TYPE_REFLECTOR    0x20000000//?#StnEvtReflector
#define TYPE_PAGEBEHAVIOR 0x10000000//?#HDXPageBehavior
#define TYPE_REPOSITORY   0x08000000//?#DisplayDataRepository
#define TYPE_SHAPELINKBEH 0x04000000//?#HSCShapeLinkBehavior
#define TYPE_BINDING      0x02000000//?#BindingBehavior
#define TYPE_ALPHA        0x01000000//?#HDXAlphaBehavior
#define TYPE_HOVER        0x00800000//?#HoverBehavior
#define TYPE_FACEPLATE    0x00400000//?#HDXPopupBehavior#Faceplate
#define TYPE_BREAKPOINT   0x00200000//?#hscbreakpointbehavior
#define TYPE_SCHEMATIC    0x00100000//?#HDXPopupBehavior#Schematic
#define TYPE_SCRIPTDATA   0x00080000//?#HSCScriptDataBehavior
#define TYPE_INDICATOR    0x00040000//?#HSCIndicatorBehavior (СѓСЂРѕРІРЅРё)
#define TYPE_ANIMATOR     0x00020000//?#HSCShapeLinkBehavior#shapelinkanimator
//sysAlarmSummary.htm
#define TYPE_ALARMSPEC    0x00000800// РЎРїРµС†РёС„РёРєР° РѕРєРЅР° sysAlarmSummary
#define TYPE_ALARMICON    0x00001000//?#AlarmStateIconFactory#AlarmIcon"
#define TYPE_SYS_BUTTON   0x00002000//?#HDXElementsFactory#Button
#define TYPE_SYS_USERDATA 0x00004000//?#default#userData
//
#define TYPE_PAINT      0xff
#define TYPE_LINE       0x01 //#HDXVectorFactory#line
#ifndef TYPE_RECT
#define TYPE_RECT       0x02 //#HDXVectorFactory#cross::RECT
#endif
#define TYPE_POLYGON    0x03 //#HDXVectorFactory#polygon
#define TYPE_ROUNDRECT  0x04 //#HDXVectorFactory#roundcross::RECT
#define TYPE_OVAL       0x05 //#HDXVectorFactory#oval
#define TYPE_TEXT       0x06 //#HDXVectorFactory#text
#define TYPE_ARC        0x07 //#HDXVectorFactory#arc
#define TYPE_WEDGE      0x08 //#HDXVectorFactory#wedge
#define TYPE_POLYLINE   0x09 //#HDXVectorFactory#polyline
#define TYPE_10         0x0A //РЎР»РѕР¶РЅС‹Р№ С‚РёРї РїРѕР»РёРіРѕРЅС‹ Рё РїРѕР»РёР»Р°Р№РЅС‹
#define TYPE_BEZIER     0x0B //#HDXVectorFactory#bezier
//
#define TYPE_SHAPELINK  0x0C //#HDXVectorFactory#shapelink
#define TYPE_GROUP      0x0D //#HDXVectorFactory#group
#define TYPE_IMAGE      0x0E //#HDXVectorFactory#image
//
#define TYPE_TREND_GRP   0x10 //
#define TYPE_TREND_SYS   0x11 //
#define TYPE_CHART_TAG   0x12 //
#define TYPE_COMBOBOX    0x13 //#HSCComboboxBehavior
#define TYPE_PUSHBUTTON  0x14 //#HDXPushButtonBehavior
#define TYPE_ALARM_TABLE 0x15 //#HDXElementsFactory#Table#HDXBinding
#define TYPE_CHECKBOX    0x16 //#HSCCheckBoxBehavior
//
#define TYPE_ACK_TOTAL_AL   0x80 // РљРІРёС‚РёСЂРѕРІР°РЅРЅС‹Рµ СЃРёРіРЅР°Р»РёР·Р°С†РёРё РІСЃРµ
#define TYPE_UNA_TOTAL_AL   0x81 // РќРµРєРІРёС‚РёСЂРѕРІР°РЅРЅС‹Рµ СЃРёРіРЅР°Р»РёР·Р°С†РёРё РІСЃРµ
#define TYPE_ACK_TOTAL_OF   0x82 // РљРІРёС‚РёСЂРѕРІР°РЅРЅС‹Рµ СЃРёРіРЅР°Р»РёР·Р°С†РёРё of
#define TYPE_UNA_TOTAL_OF   0x83 // РќРµРєРІРёС‚РёСЂРѕРІР°РЅРЅС‹Рµ СЃРёРіРЅР°Р»РёР·Р°С†РёРё of
#define TYPE_ACK_TOTAL_FL   0x84 // РљРІРёС‚РёСЂРѕРІР°РЅРЅС‹Рµ СЃРёРіРЅР°Р»РёР·Р°С†РёРё РѕС‚С„РёР»СЊС‚СЂРѕРІР°РЅРЅС‹Рµ
#define TYPE_UNA_TOTAL_FL   0x85 // РќРµРєРІРёС‚РёСЂРѕРІР°РЅРЅС‹Рµ СЃРёРіРЅР°Р»РёР·Р°С†РёРё РѕС‚С„РёР»СЊС‚СЂРѕРІР°РЅРЅС‹Рµ
#define TYPE_FILTER_APPLIED 0x86 // РќР°Р»РёС‡РёРµ С„РёР»СЊС‚СЂР°
#define TYPE_PAUSE_ALARM    0x87 // РџР°СѓР·Р° РѕР±РЅРѕРІР»РµРЅРёР№
//
#define TYPE_CLEAR_FILTR  0x80 // Clear All Filters
#define TYPE_RESET_VIEW   0x81 // Reset View
#define TYPE_ALARM_PRINT  0x82 // PrintReset View
#define TYPE_ALARM_PAUSE  0x83 // Pause
#define TYPE_ALARM_RESUME 0x84 // Resume
#define TYPE_ALARM_ACK    0x85 // Acknowledge page
#define TYPE_ALARM_SHELVE 0x86
#define TYPE_ALARM_UNSHELVE 0x87
#define TYPE_ALARM_SILENCEAU 0x88
#define TYPE_ALARM_DIALOGSHELVE 0x89
//
#define TYPE_PNT  ( style.m_dwTypeObj & TYPE_PAINT )
//
#define STYLE_ITALIC    0x00000010
#define STYLE_UNDER     0x00000020
#define STYLE_ZOOM      0x00000040
#define STYLE_MARGIN    0x00000080
#define STYLE_MARGIN_X  0x00000100
#define STYLE_MARGIN_Y  0x00000200
#define STYLE_VISIBLE   0x00000400
#define STYLE_PAD_L     0x00001000
#define STYLE_PAD_R     0x00002000
#define STYLE_PAD_T     0x00004000
#define STYLE_PAD_B     0x00008000
#define STYLE_PROGID    0x00010000
#define STYLE_LEFT      0x00020000
#define STYLE_RIGHT     0x00040000
#define STYLE_WIDTH     0x00080000
#define STYLE_TOP       0x00100000
#define STYLE_BOTTOM    0x00200000
#define STYLE_HEIGHT    0x00400000
//




enum EVisibility
{
    enumHidden  = 0,
    enumInherit = 1,
    enumVisible = 2,
    enumAuto    = 3,
};
//

// enum FontStyle
// {
//     FontStyleRegular    = 0,
//     FontStyleBold       = 1,
//     FontStyleItalic     = 2,
//     FontStyleBoldItalic = 3,
//     FontStyleUnderline  = 4,
//     FontStyleStrikeout  = 8
// };


enum Unit
{
    UnitWorld,      // 0 -- World coordinate (non-physical unit)
    UnitDisplay,    // 1 -- Variable -- for PageTransform only
    UnitPixel,      // 2 -- Each unit is one device pixel.
    UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
    UnitInch,       // 4 -- Each unit is 1 inch.
    UnitDocument,   // 5 -- Each unit is 1/300 inch.
    UnitMillimeter  // 6 -- Each unit is 1 millimeter.
};

#include "Hdx.h"

class CStyle : public CHdx
{
    int m_nAddTextRect;
    int m_nAddTextRectW;
public:
    CStyle(/*CTag* pPar*/);
    virtual ~CStyle(void);
    void AnalizSTYLE( CHTML& html, CTag* pTag, char* ptr );
    void CreateFont ( CHTML& html, CTag* pTag, cross::FontStyle fntStyle );
    void TextOut( HDC hDC, COLORREF color, cross::RECT rc, const char* text, bool bTop, COLORREF back = 0xff000000 );
    void TextOutW( HDC hDC, COLORREF color, cross::RECT rc, const wchar_t* text, bool bTop, COLORREF back = 0xff000000);
    void Queue( CHTML& html, CTag* pTag);
    bool GetFlag( DWORD dwFlag );
    bool HdxFlag( DWORD dwFlag );
    bool IsShape();
    //
    void Local( LOCALS );
    void Posts( LOCALS );
    //
    bool GetCSSStyle();  // РЅР°С…РѕРґРёС‚СЃСЏ СЃС‚РёР»СЊ РїРѕ РёРјРµРЅРё m_sStyleClass РѕРїСЂРµРґРµР»РµРЅРЅС‹Р№ РІ css-С„Р°Р»Р°С… Рё Р·Р°РїРѕР»РЅСЏСЋС‚СЃСЏ РїР°СЂР°РјРµС‚СЂС‹
    std::string GetCSSStyleImage(); // РЅР°С…РѕРґРёС‚СЃСЏ СЃС‚РёР»СЊ РїРѕ РёРјРµРЅРё m_sStyleClass РѕРїСЂРµРґРµР»РµРЅРЅС‹Р№ РІ css-С„Р°Р»Р°С… Рё РІРѕР·РІСЂР°С‰Р°РµС‚ РёРјСЏ С„Р°Р№Р»Р° РёР·РѕР±СЂР°Р¶РµРЅРёСЏ
    //
    cross::RECT OutsRect();
    cross::RECT FullRect();
    //
    float marginX;
    float marginY;
    float m_fZoom;
    //
    int   align;
    EVisibility m_eVisible;
    EVisibility m_eOverflow;
    int m_nBkgImage;
    int m_nFamily;
    //
    HFONT m_hFont;
    //
    cross::FontStyle m_fntStyle;
    float m_fHeight;
    Unit m_eTextUnit;
    int nTabIndex;
    char m_szTitle[1024];
    //
    void BadValue( HDC hDC, LPCTSTR text );
    //
    static void LoadcssSlyles(wchar_t *name);
    static std::string FindCSSSyileParam( std::string styleName, std::string paramName);
};

inline bool CStyle::GetFlag( DWORD dwFlag )
{
    return (m_dwTypeObj & TYPE_PAINT) == dwFlag;
}

inline bool CStyle::HdxFlag( DWORD dwFlag )
{
    return (m_dwFlags & dwFlag) == dwFlag;
}

inline bool CStyle::IsShape()
{
#define SHAPE_DIV (TYPE_SHAPELINKBEH | TYPE_SHAPELINK)
    return (m_dwTypeObj & SHAPE_DIV) == SHAPE_DIV;
}

#endif
