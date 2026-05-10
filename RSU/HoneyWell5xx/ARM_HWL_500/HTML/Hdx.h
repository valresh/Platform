#pragma once
//
#include "BaseHTML.h"

//INCLUDE

#include "avaloniaWrappers/Gdiplus.h"


#include "linuxTypes/CString.h"
#include "Function.h"



//
#define HDX_WIDTH       0x00000001
#define HDX_HEIGHT      0x00000002
#define HDX_POINTS      0x00000004
#define HDX_ROTATE      0x00000008
#define HDX_CLRFILL     0x00000010
#define HDX_CLRLINE     0x00000020
#define HDX_CLRTEXT     0x00000040
#define HDX_LINE_W      0x00000080
#define HDX_FILL_S      0x00000100
#define HDX_LINE_S      0x00000200
#define HDX_QUADRANT    0x00000400
#define HDX_CLRFILLG    0x00000800
#define HDX_CLRFILLL    0x00001000
#define HDX_ALMSTATE    0x00002000
#define HDX_SCNSTATE    0x00004000
#define HDX_PRESENT     0x00008000
#define HDX_INITRECT    0x00010000
#define HDX_DISK_COLORS 0x00020000
#define HDX_CONT_COLORS 0x00040000
#define HDX_RANGELOHI   0x00080000
#define HDX_RANGEORIGIN 0x00100000
#define HDX_ALIAS_TABLE 0x00200000
#define HDX_VBASIC      0x00400000
#define HDX_ROUND       0x00800000
//
#define HDX_RELATIVE    0x01000000
#define HDX_DISPLAY     0x02000000
#define HDX_BINDING     0x04000000
#define HDX_ALIGN       0x08000000
#define HDX_ALMSTATEMINUS1    0x10000000
//
#define HDX_PRESSED     0x80000000
//
enum EStyleClass : char
{
    clsUnknown = 0,
    clsTagName = 1,
// #undef  S_CLASS
// #define S_CLASS( Name, Text ) cls##Name,
    clsAlphaNum,
 //   S_CLASS( AlphaNum , "DataEntry_Alphanumeric_Style"        )
    clsComboBox,
   // S_CLASS( ComboBox , "Dataentry_Combobox_Style"            )
    clsHyperLink,
 //   S_CLASS( HyperLink, "HyperLink"                           )
    clsTagEuDesc,
 //   S_CLASS( TagEuDesc, "TagnameEUdesc"                       )
    clsPV,
 //   S_CLASS( PV       , "PV"                                  )
    clsAlarmEnbS,
 //   S_CLASS( AlarmEnbS, "AlarmEnbState"                       )
    clsAlarmType,
    //S_CLASS( AlarmType, "AlarmTypes"                          )
    clsFrame,
    //S_CLASS( Frame    , "Frame"                               )
    clsExecState,
    //S_CLASS( ExecState, "ExecState"                           )
    clsSimState,
   // S_CLASS( SimState , "SimState_Style"                      )
    clsReadOnlyL,
    //S_CLASS( ReadOnlyL, "ReadOnly_Alphatext_LeftAlign_Style"  )
    clsReadOnlyR,
   // S_CLASS( ReadOnlyR, "ReadOnly_Alphatext_RightAlign_Style" )
    clsTitleBarR,
  //  S_CLASS( TitleBarR, "Titlebar_AlphaNumeric_Right_Style"   )
    clsTxtMode,
  //  S_CLASS( TxtMode, "TxtMode")
    clsConAlpOP,
 //   S_CLASS( ConAlpOP, "ConAlpOP")
    clsPlgIn_RegValve_Close,
    //S_CLASS(PlgIn_RegValve_Close, "PlgIn_RegValve_Close")
    clsPlgOut_RegValve_Close,
    //S_CLASS(PlgOut_RegValve_Close, "PlgOut_RegValve_Close")
    clsOvlBody_Pump_Stop,
 //   S_CLASS(OvlBody_Pump_Stop, "OvlBody_Pump_Stop")
    clsOvlBody_Blower_Stop,
 //   S_CLASS(OvlBody_Blower_Stop, "OvlBody_Blower_Stop")
    clsPlgIn_OnOffValve_Close,
 //   S_CLASS(PlgIn_OnOffValve_Close, "PlgIn_OnOffValve_Close")
    clsPlgOut_OnOffValve_Close,
    //S_CLASS(PlgOut_OnOffValve_Close, "PlgOut_OnOffValve_Close")
    clsWdgC_MOV_Close,
 //   S_CLASS(WdgC_MOV_Close, "WdgC_MOV_Close")
    clsPlgOut_MOV_Close,
 //   S_CLASS(PlgOut_MOV_Close, "PlgOut_MOV_Close")
    clsPlgIn_MOV_Close,
//    S_CLASS(PlgIn_MOV_Close, "PlgIn_MOV_Close")
    clsScTxtSelectBox_NotSelected,
 //   S_CLASS(ScTxtSelectBox_NotSelected, "ScTxtSelectBox_NotSelected")
    clsT_SOFT2PumpsIn1,
//    S_CLASS(T_SOFT2PumpsIn1,"T_SOFT2PumpsIn1")
    clsRecAlm_MOS_off,
 //   S_CLASS(RecAlm_MOS_off,"RecAlm_MOS_off")

    clsTxtError_NoError,
//    S_CLASS(TxtError_NoError,"TxtError_NoError")
    clsRecPV_LK2B_Normal,
//    S_CLASS(RecPV_LK2B_Normal,"RecPV_LK2B_Normal")
    clsTxtCText_LK2B_Normal,
 //   S_CLASS(TxtCText_LK2B_Normal,"TxtCText_LK2B_Normal")
    clsRecPV_LK2B_Fail,
 //   S_CLASS(RecPV_LK2B_Fail,"RecPV_LK2B_Fail")
    clsTxtCText_LK2B_Fail,
 //   S_CLASS(TxtCText_LK2B_Fail,"TxtCText_LK2B_Fail")
    clsPlgInterLock_Active,
//    S_CLASS(PlgInterLock_Active,"PlgInterLock_Active")
    clsPlgInterLock_Inactive,
//    S_CLASS(PlgInterLock_Inactive,"PlgInterLock_Inactive")
    clsLinBypass_Active,
 //   S_CLASS(LinBypass_Active,"LinBypass_Active")
    clsLinBypass_Inactive,
 //   S_CLASS(LinBypass_Inactive,"LinBypass_Inactive")
    clsLinOutBypass_Inactive,
//    S_CLASS(LinOutBypass_Inactive,"LinOutBypass_Inactive")
    clsLinOutBypass_Active,
//    S_CLASS(LinOutBypass_Active,"LinOutBypass_Active")
    clsRecPV_Cause_Fail,
//   S_CLASS(RecPV_Cause_Fail,"RecPV_Cause_Fail")
    clsTxtCText_Cause_Fail,
//    S_CLASS(TxtCText_Cause_Fail,"TxtCText_Cause_Fail")
    clsTxtCText_Cause_Normal,
//    S_CLASS(TxtCText_Cause_Normal,"TxtCText_Cause_Normal")
    clsRecPV_MO_Normal,
//    S_CLASS(RecPV_MO_Normal,"RecPV_MO_Normal")
    clsRecPV_MO_Fail,
 //   S_CLASS(RecPV_MO_Fail,"RecPV_MO_Fail")

//#include "StyleClass.hpp"
};
EStyleClass GiveClass( const char* value );
//
struct CBorder
{
    CBorder(): color(0xff000000), padding(0), mStyle(PS_SOLID|PS_ENDCAP_FLAT), width(0), trans(false), eStyle(eBorderNone){}
    void Parsers( char* value );
    void Padding( char* value, DWORD set, DWORD& flag );
    COLORREF color; FLOAT padding; int mStyle; int width; bool trans; EBorderStyle eStyle;
    void Left  ( HDC hDC, cross::RECT& rect, bool bPress );
    void Right ( HDC hDC, cross::RECT& rect, bool bPress );
    void Top   ( HDC hDC, cross::RECT& rect, bool bPress );
    void Bottom( HDC hDC, cross::RECT& rect, bool bPress );
};
//
enum
{
    eLineStyleNone=0,
    eLineStyleSolid=1,
    eLineStyle2_2=2,
    eLineStyle4_2=3,
    eLineStyle8_2=4,
    eLineStyle1_2=5,
    eLineStyle4_2_1_2=6
};

enum
{
    eArrowTypeLine=1,
    eArrowTypeFill=2,
    eArrowTypeCircle=3,
};

struct strPointArray3
{
    POINT ptArray[3];
    bool bIsEmpty;
};

enum PoinDirect
{
    pdRight = 0,
    pdLeft  = 1
};

class CHdx : public SBaseHTML
{
    void Rect0( HDC hDC, RECT& rc, COLORREF fill, bool bLine );
public:
    CHdx();
    virtual ~CHdx() {}
    void AnalizHDX( CHTML& html, char* ptr, SImage& rImg );
    void Angles();
    void AddFlag( DWORD dwFlag );
    //
    bool LocalD( HDC hDC );
    void PostsD( HDC hDC );
    //
    void Line    ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Arc     ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Rect    ( HDC hDC, CHTML& html, cross::RECT& rc, bool bOther = false );
    void Round   ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Oval    ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Polygon ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Polygon2( HDC hDC, CHTML& html, cross::RECT& rc );
    void Polyline( HDC hDC, CHTML& html, cross::RECT& rc );
    void Wedge   ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Type10  ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Bezier  ( HDC hDC, CHTML& html, cross::RECT& rc );
    void Level   ( HDC hDC, CHTML& html, cross::RECT& rc, double dLevel, COLORREF color );
    void Button  ( HDC hDC, cross::RECT& rc, bool bPress );
    void Arrow   ( HDC hDC, POINT pt, POINT vc, int A );
    void Arrows  ( HDC hDC, LPPOINT pt );
    //
    void Gradient( cross::RECT& rc, TRIVERTEX* vert );
    void Gradient( HDC   hDC , TRIVERTEX* vert );
    //
    char* Filtr (CHTML& html);
    char* NameID(CHTML& html);
    void  NameID(CHTML& html,char* value);
    char* SrcFile(CHTML& html);
    void  SrcFile(CHTML& html,char* value);
    char* PopupName(CHTML& html);
    char* CurrentPointName(CHTML& html);
    void  PopupName(CHTML& html,char* value);
    void  PopupPosX(CHTML& html,char* value);
    void  PopupPosY(CHTML& html,char* value);
    void  Positioni(CHTML& html,char* value);
    void  DatChange(CHTML& html,char* value);

    POINT* PT(CHTML& html);
    COLORREF ContColor(CHTML& html,int n);
    COLORREF DiscColor(CHTML& html,int n);
    COLORREF Con_Color(CHTML& html,double val);
    void  SetAlias ( CHTML& html, char* value );
    LPCTSTR FindAlias( CHTML& html, LPCTSTR value );
    void DrawLinesWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc );
    void DrawWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc );
    void DrawOvalWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc,bool gradient = false );
    void DrawBezierWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc );
    void ChangePenFromLineStyle(cross::Gdiplus::Pen& pen );
    POINT GetArrowShift(int& nArrowWidth, int& nArrowType);
    void FillPoint(POINT& pt, int nShiftX, int nShiftY);
    void DrawEllipseArrow(HDC hDC,POINT& pt,POINT& ptShiftXY);
    void DrawFillTriangleArrow(HDC hDC,POINT& pt,POINT& vc,POINT& ptShiftXY);
    void DrawLineArrow(HDC hDC,POINT& pt,POINT& vc,POINT& ptShiftXY);
    strPointArray3 GetPointArrayFillTriangleArrow(POINT& pt,POINT& vc,POINT& ptShiftLengthWidth);
    strPointArray3 GetPointArrayLineArrow(POINT& pt,POINT& vc,POINT& ptShiftLengthWidth);
    strPointArray3 GetPointArray(POINT& pt,POINT& vc,POINT& ptShiftLengthWidth, bool bIsStartPoint);
    //
    DWORD m_dwFlags;
    //
    int m_inverseLogic;
    //
    int m_nLineWidth;
    int m_nFillStyle;
    int m_nLineStyle;
    int m_nQuadrant;
    int m_nObjectType;
    int m_nRoundness;
    int m_nGradientFillStyle;
    //
    int m_nPointPT;// Количество точек
    int m_nPlacePT;// Место в массиве
    int m_nPoint10;// Количество кривых
    int m_nPlace10;// Место в массиве
    //
    COLORREF m_clrFill;
    COLORREF m_clrLine;
    COLORREF m_clrText;
    COLORREF m_clrFillGrd;
    COLORREF m_clrFillLvl;
    COLORREF m_clrFillEmpty;
    //
    int m_nBreakpointType;
    int m_nDiscreteStates;
    int m_nDiscreteColors;
    int m_nContinuousColors;
    int m_nContinuousValues;
    //
    EBorderStyle m_eBorderStyle;
    REAL         m_nBorderWidth;
    //
    int m_nNumberOfChars;
    int m_nFiltrOfChars;
    //
    int m_nHDXBINDINGID;
    int m_nPlaceDsd;
    //
    int fillDRECTion;
    PoinDirect m_PointerDirection;
    int m_nEndArrow;
    int m_nStartArrow;
    // Перенесено из CStyle
    cross::Gdiplus::RectF mRect;
    bool EqBorder();
    bool IsEq (EBorderStyle e);
    bool IsNeq(EBorderStyle e);
    CBorder mL,mR,mT,mB; void Border( HDC hDC );
    //
    COLORREF colorTxt;
    COLORREF colorBkg;
    //
    DWORD m_dwTypeObj;
    DWORD m_dwFlagObj;
    DWORD m_dwShiftTxt;
    // Перенесено из CTag
    cross::RECT m_rcTrueRect;
    XFORM m_XF;
    UINT  mId;
    UINT  mSrc;
    int   mBorderImage;
    //
    bool mFillColorBlink;
    bool mLineColorBlink;
    bool mTextColorBlink;
    bool mLevelFillColorBlink;
    bool mDisabled;//Неактивность кнопки и серый цвет текста
    bool mLevelFilled;
    //
    double m_dRangeHi;
    double m_dRangeLo;
    double m_dRangeOrigin;
    double m_dValue;
    double m_dAngleNew,m_dAngleOld;
    UINT m_nCountPRM;
    UINT m_nPlacePRM;
    //
    UINT  m_nPopup; // Имя файла, которое должно всплыть при нажатии
    UINT  m_nPopupCurrentPoint; // Имя точки CurrentPoint //"C:\ProgramData\Honeywell\Experion PKS\Client\Abstract\popups\popupPid.htm?currentpoint=TCA2065
    UINT  m_nPopupGroupParametrs; // Имена точек  POPUPDISPLAYFILE=".\TSB_NASOS_NO_PID_POPUP_10.htm?PARAM1=N28&amp;PARAM2=N28_AVT&amp;PARAM3=N28_DO&amp;PARAM4=N28_ASUTP&amp;PARAM5=N28_PR_FREQ&amp;PARAM6=N28_PR_I&amp;PARAM7=N28_ASTOP&amp;PARAM8=PT1H281&amp;PARAM9=PT1H282&amp;PARAM10=PT2H281&amp;PARAM11=TE2H282&amp;PARAM12=TT1H281&amp;PARAM13=TT1H282&amp;PARAM14=TE2H281&amp;PARAM15=TT2H281&amp;PARAM16=LA1H281&amp;PARAM17=LA1H281_F&amp;PARAM18=LA1H281_ERR&amp;PARAM19=LA1H282&amp;PARAM20=LA1H282_F&amp;PARAM21=LA1H282_ERR&amp;PARAM22=LA2H281&amp;PARAM23=LA2H281_F&amp;PARAM24=LA2H281_ERR&amp;PARAM25=LSA1L281&amp;PARAM26=LSA1L282&amp;PARAM27=N28_AO&amp;PARAM30=H-28&amp;PARAM31=PT1H282_F&amp;PARAM32=PT1H281_F&amp;PARAM33=TT1H282_F&amp;PARAM34=TT1H281_F&amp;PARAM35=N28_PR_FREQ_F&amp;PARAM36=TE2H281_F&amp;PARAM37=TE2H282_F&amp;PARAM38=PT2H281_F&amp;PARAM39=N28_PR_I_F&amp;PARAM40=TT2H281_F&amp;PARAM41=N28_T_E&amp;PARAM42=N28_STBT&amp;PARAM43=LSA1L281_F&amp;PARAM44=LSA1L281_ERR&amp;PARAM45=LSA1L282_F&amp;PARAM46=LSA1L282_ERR"
    //
    POINT m_ptPopup;// Координаты этого окна
    int   m_nPos;   // Какое-то позиционирование
    EStyleClass mClass;
    char  m_sStyleClass[256]; //название стиля из cs.css
    int   m_alarm;
    //
    bool m_bNoLineColor; //true нет параметра lineColor
    int  m_TimeFormat; //-1 -нет его
    bool m_bReadOnly;  //если false - оператор может редактировать значение
    bool m_bParameterFormat;  //если true - данные нужно выводить как сконфигурировано в контроллере (для PV - PVFORMAT)
    int m_Direction;
    //
    bool m_bClrFillCanChangedByScript;
    bool m_bChangedByData;
    //
    char m_fontWeight[64];
    //
    cross::RECT as;
};

inline void SetRGB(TRIVERTEX& val,COLORREF color)
{
    val.Red = GetRValue(color)<<8;
    val.Blue = GetBValue(color)<<8;
    val.Green = GetGValue(color)<<8;
    val.Alpha = 0;
}

inline void CHdx::AddFlag( DWORD dwFlag )
{
    m_dwTypeObj &= 0xffffff00;
    m_dwTypeObj |= dwFlag;
}
