#include "StdAfx.h"
#include "../ARM.h"
//#include "../AplicFrm.h"
#include "TagDIV.h"
#include "HTML.h"
#include "_AlarmEntry.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Shape.h"
#include "Scripts127.h"
#include "Style.h"
//#include "ViewTrend.h"
//#include "TagAreaPane.h"
//#include "Trend3.h"

#include "avaloniaWrappers/Gdiplus.h"

//ИЗ РАЗНЫХ ФАЙЛОВ, непонятно зачем в файлах с UI лежат структуры данных
#undef  STD_MESSAGE
#define STD_MESSAGE void* Void, class CMyFont& font, cross::RECT& rc, void* Out, int nWidth, CShBase* sh
typedef void (*LPOutputMess)( STD_MESSAGE );

struct SHeader
{
    CharMP szName;
    int    nWidth;
    BOOL   bVisible;// Видимость
    BOOL   bStatic ;// Изменение ширины колонки
    LPOutputMess func;// Функция вывода сообщения
    int nSortNumber; // номер для сортировки и фильтрации (во ViewAlarm от 0 до 9)
    int nAlign;
};


CTagDIV::CTagDIV(void)
    : m_numberOfShapesAnimated(0)
    , m_useFirstShapeForBadValue(0)
    //, m_pViewAlarm(NULL)
    , m_nDivValue(0)
    , m_linkType(ltUnknown)
{

    cross::CString a;
}
//
CTagDIV::~CTagDIV()
{
    // if(m_pViewAlarm)
    //    delete m_pViewAlarm;
    // m_pViewAlarm = NULL;
}
//
#undef  STD_VAR
#define STD_VAR CHTML& html, CTagDIV& a, CStyle& style, char* value, char* src
typedef void (*LPFuncs)( STD_VAR );
//
//static char szDebugname[64] = "shape072_ValveVerh";
//static char szDebugname[64] = "shape138_PointAlarms";
static char szDebugname[64] = "roundcross::RECT008";
//
void divClass( STD_VAR )
{
    if(strstr(value, "hsc.popup."))
        a.m_ClickType =  ctPopup;

    //класс без точки - это класс из css
    char *pp1 = strchr(value, ' ');
    while(pp1)
    {
        char *pp2 = strchr(++pp1, ' ');
        if(pp2)
        {
            *pp2 = '\0';
            if(*pp1 && strchr(pp1, '.')==0)
            {
                strcpy_s(style.m_sStyleClass, pp1);
                *pp2 = ' ';
                break;
            }
            *pp2 = ' ';           
        }
        else
        {
            if(*pp1 && strchr(pp1, '.')==0)
            {
                strcpy_s(style.m_sStyleClass, pp1);
                break;
            }
        }
        pp1 = pp2;
    }
}
//
void divStyle( STD_VAR )
{
    a.AnalizStyle( html, value );
}
// Специфические ID
bool SpecID( STD_VAR )
{
    if ( lstrcmpi( value, "Background" ) == 0 )
        style.m_dwTypeObj |= TYPE_BACKGROUND;
    else
        if ( lstrcmpi( value, "Page" ) == 0 )
            style.m_dwTypeObj |= TYPE_PAGE;
        else
            // Обрабатываем специфику sysAlarmSummary
            if ( lstrcmpi( value, "lblUnackLabel"      ) == 0
                ||   lstrcmpi( value, "lblAckLabel"        ) == 0
                ||   lstrcmpi( value, "lblShelvedLabel"    ) == 0
                ||   lstrcmpi( value, "lblSuppressedLabel" ) == 0)
                style.AddFlag(TYPE_TEXT);
            else if ( lstrcmpi( value, "lblOfAckTotal"           ) == 0 ) style.AddFlag(TYPE_ACK_TOTAL_OF  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "lblOfUnackTotal"         ) == 0 ) style.AddFlag(TYPE_UNA_TOTAL_OF  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "divAckUnfilteredTotal"   ) == 0 ) style.AddFlag(TYPE_ACK_TOTAL_AL  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "divUnackUnfilteredTotal" ) == 0 ) style.AddFlag(TYPE_UNA_TOTAL_AL  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "divAckFilteredTotal"     ) == 0 ) style.AddFlag(TYPE_ACK_TOTAL_FL  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "divEventCount"/*Analog*/ ) == 0 ) style.AddFlag(TYPE_ACK_TOTAL_FL  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "divUnackFilteredTotal"   ) == 0 ) style.AddFlag(TYPE_UNA_TOTAL_FL  |TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "lblFilterApplied"        ) == 0 ) style.AddFlag(TYPE_FILTER_APPLIED|TYPE_ALARMSPEC);
            else if ( lstrcmpi( value, "divPaused"               ) == 0 ) style.AddFlag(TYPE_PAUSE_ALARM   |TYPE_ALARMSPEC);
            //
            else if ( lstrcmpi( value, "btnClearFilter"   ) == 0 ) style.AddFlag(TYPE_CLEAR_FILTR |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnResetView"     ) == 0 ) style.AddFlag(TYPE_RESET_VIEW  |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnPrintAsReport" ) == 0 ) style.AddFlag(TYPE_ALARM_PRINT |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnPause"         ) == 0 ) style.AddFlag(TYPE_ALARM_PAUSE |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnResume"        ) == 0 ) style.AddFlag(TYPE_ALARM_RESUME|TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnAckPage"       ) == 0 ) style.AddFlag(TYPE_ALARM_ACK   |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnShelveAlarm"   ) == 0 ) style.AddFlag(TYPE_ALARM_SHELVE   |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnUnshelveAlarm" ) == 0 ) style.AddFlag(TYPE_ALARM_UNSHELVE |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnSilenceAudible") == 0 ) style.AddFlag(TYPE_ALARM_SILENCEAU |TYPE_SYS_BUTTON);
            else if ( lstrcmpi( value, "btnDialogShelveAlarm") == 0 ) style.AddFlag(TYPE_ALARM_DIALOGSHELVE |TYPE_SYS_BUTTON);
            // Конец специфики sysAlarmSummary
            else
                return false;
    return true;
}
//
void divID( STD_VAR )
{
    if ( !*value ) return;
  //  style.NameID( html, value );
    //
#ifdef _DEBUG
    if(_stricmp(value, szDebugname)==0)
        AAA();
#endif 
    //
    if ( SpecID( html, a, style, value, src ) )
        return;
    //
    if ( _strnicmp( value, "name", 4 ) == 0 )
    {
        value += 4;
        if ( IsInt( value ) )
        {
            // CDataTrends& mData = CViewTrend::Data();
            // int n = atoi(value);
            // ASSERT( 1 <= n && n <= 8 );n--;
            // STrends* item = mData.ItemStr(html.Group());
            // if ( item != NULL )
            // {
            //     SPoint& pt = item->point[n];
            //     a.m_constValue = mData.Text(pt.hPoint);
            //     if ( a.m_constValue.IsEmpty() ) a.m_constValue = " ";
            // }
            // return;
        }
    }
    else
        if ( _strnicmp( value, "param", 5 ) == 0 )
        {
            value += 5;
            if ( IsInt( value ) )
            {
                // CDataTrends& mData = CViewTrend::Data();
                // int n = atoi(value);
                // ASSERT( 1 <= n && n <= 8 );n--;
                // STrends* item = mData.ItemStr(html.Group());
                // if ( item != NULL )
                // {
                //     SPoint& pt = item->point[n];
                //     a.m_constValue = mData.Text(pt.hParam);
                //     if ( a.m_constValue.IsEmpty() ) a.m_constValue = " ";
                // }
                // return;
            }
        }
}
//
void divTabIndex( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    style.nTabIndex = atoi(value);
}
//
void divHdxProperties( STD_VAR )
{
    // a.AnalizHdx( html, value );
}
//
void divValue( STD_VAR )
{
    ASSERT( IsInt(value) );
    //ASSERT( atoi(value) < 4 );
    a.m_nDivValue = atoi(value);
}
//
void divSrc( STD_VAR )
{
 //   if ( !*value ) return;
 //   style.SrcFile( html, value );
    //CListXML xml;
    //int yy = xml.Load(value);
    lstrcpy( src, value );
}
//
void divNumberOfShapesAnimated( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int b = atoi(value);
	a.m_numberOfShapesAnimated = b;
    if ( b == 1 ) return;
    ASSERT( b == 2 || b == 3 || b == 4 || b == 5 || b == 8 );
}
//
bool CTagDIV::IsBadPropertiesInProject(cross::CString strFileName)
{//у них в проекте ошибки
    if (Version==AT_PRP)
    {
        if ((strFileName==cross::CString("POPUPZ2143"))||(strFileName==cross::CString("POPUPZ2213"))||(strFileName==cross::CString("POPUPXB4_2"))
            ||(strFileName==cross::CString("POPUPXB101"))||(strFileName==cross::CString("POPUPXB-4_3R"))||
            (strFileName==cross::CString("POPUPAZ101"))||(strFileName==cross::CString("POPUPAZ102"))||(strFileName==cross::CString("POPUPAZ103"))||
            (strFileName==cross::CString("POPUPAZ104"))||(strFileName==cross::CString("POPUPAZ105"))||(strFileName==cross::CString("POPUPAZ106")))
            return true;

    }
    return false;
}

cross::CString CTagDIV::GetCorrectPropertiesString(cross::CString strFileName, cross::CString strValue)
{//у них в проекте ошибки
    cross::CString strTemp=strValue;
    if (strFileName==cross::CString("POPUPZ2213"))
        strTemp.Replace(cross::CString("Z_2213F.PVFL"), cross::CString("Z_2213O_1.PVFL"));
    else
        if (strFileName==cross::CString("POPUPZ2143"))
            strTemp.Replace("Z_A3_4","Z_2143");
        else
            if (strFileName==cross::CString("POPUPXB4_2"))
                strTemp.Replace("cmd:HS_XB4_2.PV;","cmd:HS_XB4_2.PVFL;");
            else
                if (strFileName==cross::CString("POPUPXB101"))
                    strTemp.Replace("cmd:HS_XB101.PV;","cmd:HS_XB101.PVFL;");
                else
                    if (strFileName==cross::CString("POPUPXB-4_3R"))
                        strTemp.Replace("cmd:HS_XB_4_3R.PV;","cmd:HS_XB_4_3R.PVFL;");
                    else
                        if (strFileName==cross::CString("POPUPAZ106"))
                        {
                            strTemp.Replace("cmd:HS1_AZ106.PV;","cmd:HS1_AZ106.PVFL;");
                            strTemp.Replace("cmd:HS2_AZ106.PV;","cmd:HS2_AZ106.PVFL;");
                        }
                        else
                            if (strFileName==cross::CString("POPUPAZ105"))
                            {
                                strTemp.Replace("cmd:HS1_AZ105.PV;","cmd:HS1_AZ105.PVFL;");
                                strTemp.Replace("cmd:HS2_AZ105.PV;","cmd:HS2_AZ105.PVFL;");
                            }
                            else
                                if (strFileName==cross::CString("POPUPAZ104"))
                                {
                                    strTemp.Replace("cmd:HS1_AZ104.PV;","cmd:HS1_AZ104.PVFL;");
                                    strTemp.Replace("cmd:HS2_AZ104.PV;","cmd:HS2_AZ104.PVFL;");
                                }
                                else
                                    if (strFileName==cross::CString("POPUPAZ103"))
                                    {
                                        strTemp.Replace("cmd:HS1_AZ103.PV;","cmd:HS1_AZ103.PVFL;");
                                        strTemp.Replace("cmd:HS2_AZ103.PV;","cmd:HS2_AZ103.PVFL;");
                                    }
                                    else
                                        if (strFileName==cross::CString("POPUPAZ102"))
                                        {
                                            strTemp.Replace("cmd:HS1_AZ102.PV;","cmd:HS1_AZ102.PVFL;");
                                            strTemp.Replace("cmd:HS2_AZ102.PV;","cmd:HS2_AZ102.PVFL;");
                                        }
                                        else
                                            if (strFileName==cross::CString("POPUPAZ101"))
                                            {
                                                strTemp.Replace("cmd:HS1_AZ101.PV;","cmd:HS1_AZ101.PVFL;");
                                                strTemp.Replace("cmd:HS2_AZ101.PV;","cmd:HS2_AZ101.PVFL;");
                                            }
    return strTemp;
}

void divParameters( STD_VAR )
{
    if ( !*value ) return;
    if ( style.m_nCountPRM ) return;
    cross::CString strFileName=html.m_strFile;
    strFileName.MakeUpper();
    cross::CString strValue(value);
    if (a.IsBadPropertiesInProject(strFileName))
        strValue=a.GetCorrectPropertiesString(strFileName, strValue);
   // style.SetAlias( html,  strValue.GetBuffer());
    //   strValue.ReleaseBuffer();
}
//
void divLinkType( STD_VAR )
{
    if ( !*value )
        return;
    if (stricmp(value, "embedded") == 0)
        a.m_linkType = ltEmbedded;
    else
        if (stricmp(value, "linked") == 0)
            a.m_linkType = ltLinked;
}
//
void divGlobalScripts( STD_VAR )
{
    // Не обрабатываем
}
//
void divStyleClass( STD_VAR )
{
    // Не обрабатываем
}
//
void divShapeSrc( STD_VAR )
{
    // if ( !a.mImg.Yes() )
    //     LoadImages( value, a.mImg );
}
//
void divXMLFile( STD_VAR )
{
    // Не обрабатываем
}
//
void divImageFile( STD_VAR )
{
    // Не обрабатываем
}
//
void divBorder( STD_VAR )
{
    int nValue = atoi(value);
    if ( nValue == 0 ) return;
    ASSERT(0);
    // Не обрабатываем
}
//
void divPageDescription( STD_VAR )
{
    //  ASSERT( *value == 0 || *value == 'F' );
}
//
void divOnMouseLeave( STD_VAR )
{
    if ( *value == '\'' )
    {
        int yy = 90;
    }
}
//
void divHDX_LOCK( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    int b = atoi(value);
    ASSERT( b == -1 );
}
//
void divTitle( STD_VAR )
{
    strcpy_s(style.m_szTitle, value);
}
//
void divSHORTCUTMENUCUSTOM( STD_VAR )
{
}
//
void divDisabled( STD_VAR )
{
    if ( lstrcmpi( value, "true" ) == 0 )
        style.mDisabled = true;
    else
        if ( lstrcmpi( value, "false" ) == 0 )
            style.mDisabled = false;
        else
            if ( lstrcmpi( value, "" ) == 0 )
                style.mDisabled = true;
            else
            {
                ASSERT(0);
            }
}
//
void divPOPUPDISPLAYFILE( STD_VAR ){/*style.PopupName(html,value);*/}
void divPOPUPLEFT       ( STD_VAR ){/*style.PopupPosX(html,value);*/}
void divPOPUPTOP        ( STD_VAR ){/*style.PopupPosY(html,value);*/}
void divPOSITIONING     ( STD_VAR ){/*style.PopupPosY(html,value);*/}
//void divOnDataChange    ( STD_VAR ){style.DatChange(html,value);}
//
void divHelpFileName     ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divHelpFileTypeList ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divLocateHelpTopicBy( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divHelpTopic        ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divOnPropertyChange ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divButtonMode       ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divBotif            ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void divOnHdxClick       ( STD_VAR ){/*Встречается в sysAlarmSummary*/}

bool findSubrutine(char** ptr, char* search)
{
    if (!_strnicmp(*ptr, search, strlen(search)))
    {
        *ptr += strlen(search);
        return true;
    }
    return false;
}

void divOnUpdate( STD_VAR )
{
    char* ptr = value;
    while(*ptr)
    {
        if (findSubrutine(&ptr, "Cda_CEXXX_AlpPV_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usCda_CEXXX_AlpPV_OnUpdate_me_);
        else if (findSubrutine(&ptr, "LK2B_Cda_CEXXX_AlpPV_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usLK2B_Cda_CEXXX_AlpPV_OnUpdate_me_);
        else if (findSubrutine(&ptr, "LK2B_Cda_CEXXX_ConAlpTP_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usLK2B_Cda_CEXXX_ConAlpTP_OnUpdate_me_);
        else if (findSubrutine(&ptr, "LK2B_Cda_CEXXX_RecPV_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usLK2B_Cda_CEXXX_RecPV_OnUpdate_me_);
        else if (findSubrutine(&ptr, "UZ_Cda_Graph_AlpError_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usUZ_Cda_Graph_AlpError_OnUpdate_me_);
        else if (findSubrutine(&ptr, "All_ILBPGraph_AlpError_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usAll_ILBPGraph_AlpError_OnUpdate_me_);
        else if (findSubrutine(&ptr, "CDA_Interlock_OnUpdate(me)"))
            a.m_vctrUpdateScripts.push_back(usCDA_Interlock_OnUpdate_me_);
        else
            ptr++;
    }
}

void divOnDataChange( STD_VAR )
{
    if(Version > L35_11600_KF)
    {
        if(strlen(value)>2)
        {
            char* pValue = value;
            if(value[0]=='\'')
                pValue++;
            size_t len= strlen(pValue);
            if(pValue[len-1]=='\'')
                pValue[len-1] = 0;
            a.m_ScriptTextId = style.AddTexts( html, pValue );
            //    a.m_ScriptTexteType = eventOnDataChange;
        }
    }
}

void divOnClick ( STD_VAR )
{
    char* ptr = value;
    while(*ptr)
    {
        if (findSubrutine(&ptr, "Nav_SelectBox_OnClick(me)"))
        {
            a.m_vctrOnClickScripts.push_back(usNav_SelectBox_OnClick_me_);
            return;
        }
        else
            ptr++;
    }

    if(Version > L35_11600_KF)
    {
        if(strlen(value)>2)
        {
            char* pValue = value;
            if(value[0]=='\'')
                pValue++;
            size_t len= strlen(pValue);
            if(pValue[len-1]=='\'')
                pValue[len-1] = 0;
            a.m_ScriptTextId = style.AddTexts( html, pValue );
            //         a.m_ScriptTexteType = eventOnClick;
        }
    }
}

void divOnMouseDown( STD_VAR )
{
    char* ptr = value;
    if(_strnicmp(ptr, "call ", sizeof("call ")-1)==0)
    {
        ptr += sizeof("call ")-1;
    }

    if(Version >= L35_11600_KF)
    {
        if(strlen(value)>2)
        {
       //     char *id = style.NameID( html );
            char* pValue = value;
            if(value[0]=='\'')
                pValue++;
            size_t len= strlen(pValue);
            if(pValue[len-1]=='\'')
                pValue[len-1] = 0;
            a.m_ScriptTextId = style.AddTexts( html, pValue );
            //            a.m_ScriptTexteType = eventOnMouseDown;
        }
    }
}

void divFilterApplied    ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
//
void CTagDIV::Analiz( ANALIZ )
{
    // struct SLocal { char* name; LPFuncs func; };
    // static SLocal list[] =
    //     {
    //      { "class"                  , divClass                  },
    //      { "style"                  , divStyle                  },
    //      { "id"                     , divID                     },
    //      { "tabIndex"               , divTabIndex               },
    //      { "hdxproperties"          , divHdxProperties          },
    //      { "value"                  , divValue                  },
    //      { "src"                    , divSrc                    },
    //      { "parameters"             , divParameters             },
    //      { "linkType"               , divLinkType               },
    //      { "globalscripts"          , divGlobalScripts          },
    //      { "styleClass"             , divStyleClass             },
    //      { "shapesrc"               , divShapeSrc               },
    //      { "XMLFile"                , divXMLFile                },
    //      { "ImageFile"              , divImageFile              },
    //      { "border"                 , divBorder                 },
    //      { "numberOfShapesAnimated" , divNumberOfShapesAnimated },
    //      { "PageDescription"        , divPageDescription        },
    //      { "onmouseleave"           , divOnMouseLeave           },
    //      { "HDX_LOCK"               , divHDX_LOCK               },
    //      { "ondatachange"           , divOnDataChange           },
    //      { "title"                  , divTitle                  },
    //      { "SHORTCUTMENUCUSTOM"     , divSHORTCUTMENUCUSTOM     },
    //      { "POSITIONING"            , divPOSITIONING            },
    //      { "POPUPDISPLAYFILE"       , divPOPUPDISPLAYFILE       },
    //      { "POPUPLEFT"              , divPOPUPLEFT              },
    //      { "POPUPTOP"               , divPOPUPTOP               },
    //      { "HelpFileName"           , divHelpFileName           },
    //      { "HelpFileTypeList"       , divHelpFileTypeList       },
    //      { "LocateHelpTopicBy"      , divLocateHelpTopicBy      },
    //      { "HelpTopic"              , divHelpTopic              },
    //      { "onpropertychange"       , divOnPropertyChange       },
    //      { "buttonMode"             , divButtonMode             },
    //      { "motif"                  , divBotif                  },
    //      { "onhdxclick"             , divOnHdxClick             },
    //      { "onclick"                , divOnClick                },
    //      { "filterApplied"          , divFilterApplied          },
    //      { "disabled"               , divDisabled               },
    //      { "onupdate"               , divOnUpdate               },
    //      { "onmousedown"            , divOnMouseDown          },
    //      }InitLocal;
    // //
    // CStyle& style = Style(html);
    // CharMP src = "";
    // int n = 0;
    // while ( teg[n].name )
    // {
    //     SLocal* find = FindLocal(teg[n].name);
    //     //ASSERT( find );
    //     if ( find != NULL )
    //         (*find->func)(html,*this,style,teg[n].value,src);
    //     n++;
    // }
    // //
    // if ( style.m_dwTypeObj & TYPE_PAGE )
    // {
    //     if (html.m_strFile == cross::CString( "sysAlarmSummary"))
    //     {
    //         cross::RECT rc;
    //         // html.mWnd->GetWindowcross::RECT(&rc);
    //         // style.mcross::RECT.Width = rc.right - rc.left ;
    //         // style.mcross::RECT.Height = rc.bottom - rc.top -3;
    //         //html.Size( CSize((LONG)(rc.right - rc.left),(LONG)(rc.bottom - rc.top)) );

    //     }
    //     //      html.Size( CSize((LONG)style.mcross::RECT.Width,(LONG)style.mcross::RECT.Height) );
    // }
    // //
    // if ( style.m_dwTypeObj & TYPE_BACKGROUND )
    //     if ( style.colorBkg != 0xffffffff )
    //         html.Color(style.colorBkg);
    // style.m_dwFlags |= HDX_PRESENT;
    // //
    // // Судя по детальным окнам, надо сделать так. | HDX_LINE_W - не катит
    // //MIHAIL_HWL поменял в нижней строчке, когда делал рисование там было style.m_nLineStyle == 0
    // if ( (style.m_dwFlags & HDX_LINE_S) != 0 && style.m_nLineStyle == eLineStyleNone )
    //     if ( (style.m_dwFlags & (HDX_CLRLINE | HDX_CLRFILL | HDX_FILL_S)) == 0 )
    //     {
    //         style.m_clrFill = 0xffffff;
    //         style.m_clrLine = 0xffffff;
    //     }
    // //
}


//
void CTagDIV::Kerdyk(HWND hWnd, CHTML &html)
{
#ifdef _DEBUG  
    // if( _stricmp(Style(html).NameID( html ), szDebugname)==0)
    //     AAA();
#endif
    if ( m_constValue.IsEmpty() )
        m_constValue = GetValue(html);
    //
    CStyle& style1 = Style(html);

 //   char * id = style1.NameID( html );

    style1.GetCSSStyle();

    // char * src = style1.SrcFile(html);
    // if (strstr(src, "LK2B_Cda_CEEffect_lib_h_pai_01")){
    //     mVal =  FindExperion( html, Alias(html, this, "tagname"), Alias(html, this, "cp_pv") );
    // }

    //const char *tagname = Alias(html, this, "tagname");
    //const char *Parameter = Alias(html, this, "Parameter");

#ifdef _DEBUG  
    // if( strcmp(id, szDebugname)==0)
    //     KKK();
#endif
 //   Shape( html );
  //  CTag::Kerdyk( hWnd, html );
    CStyle& style = Style(html); //style может быть испорчен в Kerdyk так как там добавляются элементы и может быть перераспределена
  //  id = style.NameID( html );   //память на которую ссылается style1
    //
    if(Version >= AM2_NAK)
    {
        if(mVal.nNumber == -1 && m_constValue[0] && TYPE_PNT == TYPE_TEXT)
            lstrcpy(m_szScriptValue, m_constValue);
    }
    //
    if(Version >= DOTF_PM)
    {
        if(TYPE_PNT==TYPE_ROUNDRECT)
        {
            if(style.m_nRoundness>0)
            {
                cross::RECT rc = style.FullRect();
                int w = rc.right-rc.left;
                int h = rc.bottom-rc.top;
                int m = fmax(w,h);
                if(Version > DOTF_PM) //может и в DOTF_PM min()
                    m = min(w,h);
                if(m>0)
                    style.m_nRoundness = m*style.m_nRoundness/100; //т.к исходное значение в %
            }
            else if(Version >= BERTH_ONPZ && style.m_nRoundness==0)
            {
                style.m_nRoundness = 20;
            }
        }
    }
    else
        if(SGK_ONPZ==Version)
        {
            if(mVal.nNumber < 0)
            {
                if(html.m_strFile.CompareNoCase("SGK_OTSECH_POPUP")==0)
                {
                    // if(strcmp(id, "textbox001")==0)
                    //     m_constValue = "";
                }
                else
                    if(html.m_strFile.CompareNoCase("SGK_ZD_POPUP_HW")==0)
                     {
                    //     if(strcmp(id, "textbox003")==0)
                    //         m_constValue = "";
                    }
            }
        }
        else
            if(BERTH_ONPZ==Version)
            {
                if(strstr(m_constValue, "_MP_SL_CurrentMass"))
                    style.align &= ~DT_RIGHT;
            }
    //
    html.m_tagTargShape140 = NULL;
    // if (Version == LG35_8_KF  && strstr(id, "_cross::RECTarg"))
    // {
    //     html.m_tagTargShape140 = this;
    // }
    // if (Version == LG35_8_KF  && strstr(id, "btnDropViewList"))
    // {
    //     //  this->m_pWnd = new ViewCombo(html);
    // }
    // if (Version == LG35_8_KF  && strstr(id, "shape001") && html.m_strFile == cross::CString("M01"))
    // {
    //     style.m_eVisible = enumVisible;
    // }
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    // if ( nCount == 1 && obj[0]->IsYou(html,"P") )
    // {
    //     style.SetLastValue( html, obj[0]->GetValue(html) );
    //     style.CreateFont( html, this, FontStyleBold );
    //     if ( m_constValue.IsEmpty() )
    //         m_constValue = GetValue(html);
    // }
    //
    if ( style.GetFlag(TYPE_ALARM_TABLE) )
        if ( style.m_eVisible != enumHidden )
        {
            //     m_pWnd = new CHTML_Alarm( html, style.colorTxt, style.colorBkg );
        }
    //
    if ( m_numberOfShapesAnimated > 1 )
        SetMVal( html, mVal );
    //
    // Бляха-муха
    if ( !m_constValue.IsEmpty() )
        if ( TYPE_PNT == 0 )
            style.AddFlag(TYPE_TEXT);
    //
    char* v = (char*)style.GetValue( html );
    if ( v[0] == '{' && v[1] == '%' )
    {
        int dl = lstrlen( v );
        while ( v[dl-1] == ' ' ) dl--;
        if ( v[dl-2] == '%' && v[dl-1] == '}' )
        {
            const char* p = strstr( v, "::" );
            if ( p != 0 )
            {
                p += 2;
                v[dl-2] = 0;
                CTag* par = m_pPar;
                while ( par )
                {
                    CStyle& s = par->Style(html);
                    if ( s.m_nCountPRM )
                    {
                   //     m_constValue = s.FindAlias( html, p );
                        break;
                    }
                    par = par->m_pPar;
                }
                v[dl-2] = '%';
            }
        }
    }
    //
    if ( style.m_dwTypeObj & TYPE_BREAKPOINT )
        if ( TYPE_PNT == TYPE_GROUP )
            if ( style.m_nBreakpointType != -1 )
            {//подсовывание параметров анимации в дочерние элементы
                INT_PTR nCount = GetSize();
                CTag**     obj = GetData();
                for ( INT_PTR n = 0; n < nCount; n++ )
                {
                    CStyle& s = obj[n]->Style( html );
#ifdef _DEBUG
  //                  char * s_id = s.NameID( html );
#endif
                    if ( ( s.m_dwTypeObj & TYPE_PAINT ) == TYPE_TEXT )
                        continue;
                    if ( s.m_nBreakpointType == -1 || (Version != LG35_8_KF && s.m_nBreakpointType==1 && s.m_dwFlags & HDX_CLRTEXT))
                    {
                        s.m_nBreakpointType   = style.m_nBreakpointType  ;
                        s.m_nDiscreteStates   = style.m_nDiscreteStates  ;
                        s.m_nDiscreteColors   = style.m_nDiscreteColors  ;
                        s.m_nContinuousColors = style.m_nContinuousColors;
                        s.m_nContinuousValues = style.m_nContinuousValues;
                        //s.m_dwFlags &= ~HDX_CLRTEXT;
                        obj[n]->mVal = mVal;
                        if (Version != LG35_8_KF)
                        {
                            if ( ( s.m_dwTypeObj & TYPE_PAINT ) == TYPE_GROUP)
                            {
                                INT_PTR nCountG = obj[n]->GetSize();
                                CTag**     objG = obj[n]->GetData();
                                for ( INT_PTR ii = 0; ii < nCountG; ii++ )
                                {
                                    CStyle& ss = objG[ii]->Style( html );
                                    if ( ( ss.m_dwTypeObj & TYPE_PAINT ) == TYPE_TEXT )
                                        continue;
#ifdef _DEBUG
 //                                   char * ss_id = ss.NameID( html );
#endif
                                    if ( ss.m_nBreakpointType == -1 )
                                    {
                                        ss.m_nBreakpointType   = style.m_nBreakpointType  ;
                                        ss.m_nDiscreteStates   = style.m_nDiscreteStates  ;
                                        ss.m_nDiscreteColors   = style.m_nDiscreteColors  ;
                                        ss.m_nContinuousColors = style.m_nContinuousColors;
                                        ss.m_nContinuousValues = style.m_nContinuousValues;
                                        objG[ii]->mVal = mVal;
                                    }
                                }
                            }
                        }
                    }
                }
            }
    //
    //int nClass = StyleClass( hDC, html );
    // if (Version == LG35_8_KF && strstr(html.pipe.GetMnemoName( mVal.nNumber ), ".DIGACQA") && !strcmp(mVal.def->name, "PV"))
    // {
    //     const char * nameDIGACQA = html.pipe.GetMnemoName(mVal.nNumber);
    //     if (strstr(nameDIGACQA, ".DIGACQA") && !strcmp(mVal.def->name, "PV"))
    //     {
    //         mVal = FindExperion( html, nameDIGACQA, "PVFL" );
    //     }
    // }
    // if ( style.mClass == clsAlarmEnbS )
    //     if ( mVal.nNumber < 0 )
    //     {
    //         int nActive = html.pipe.GetActive();
    //         const char* name = html.pipe.GetMnemoName( nActive );
    //         CharMP szExt;
    //         ::GiveDACA( szExt, name );
    //         mVal = FindExperion( html, szExt, "NAME" );
    //         //Sergej if ( mVal.nNumber < 0 ) ASSUNO;
    //     }
    // if ( style.m_dwTypeObj & TYPE_ALARMICON )
    // {
    //     if ( mVal.nNumber > -1 && mVal.def)
    //     {
    //         cross::CString strParam(mVal.def->name);
    //         strParam.MakeUpper();
    //         if (strParam="ALMENBSTATE")
    //         {
    //             const char* name = html.pipe.GetMnemoName( mVal.nNumber );
    //             CharMP szExt;
    //             ::GiveDACA( szExt, name );
    //             mVal = FindExperion( html, szExt, "NAME" );
    //         }
    //     }
    // }
}
void CTagDIV::ApplyStyles(LOCALS)
{
    CStyle& style = Style(html);
    // switch(style.mClass)
    // {
    // case clsScTxtSelectBox_NotSelected:
    //     break;
    // case clsTxtError_NoError:
    //     style.m_eVisible = enumHidden;
    //     break;
    // case clsRecPV_LK2B_Normal:
    //     style.m_clrFill = 0x696969;
    //     break;
    // case clsTxtCText_LK2B_Normal:
    //     style.m_clrText = 0x8c8c8c;
    //     break;
    // case clsRecPV_LK2B_Fail:
    //     style.m_clrFill = 0x0000ff;
    //     break;
    // case clsTxtCText_LK2B_Fail:
    //     style.m_clrFill = 0;
    //     break;
    // case clsPlgInterLock_Active:
    //     style.m_clrFill = 0x00a0ff;
    //     style.m_clrLine = 0x0;
    //     style.m_nLineWidth = 2;
    //     style.m_eVisible = enumVisible;
    //     break;
    // case clsPlgInterLock_Inactive:
    //     style.m_clrFill = 0xc0c0c0;
    //     style.m_clrLine = 0x0;
    //     style.m_nLineWidth = 1;
    //     style.m_eVisible = enumVisible;
    //     break;
    // case clsLinBypass_Inactive:
    //     style.m_eVisible = enumHidden;
    //     break;
    // case clsLinOutBypass_Inactive:
    //     style.m_eVisible = enumHidden;
    //     break;
    // case clsLinOutBypass_Active:
    //     style.m_eVisible = enumInherit;
    //     break;
    // case clsLinBypass_Active:
    //     style.m_eVisible = enumInherit;
    //     break;
    // case clsRecPV_Cause_Fail:
    //     style.m_clrFill = 0xc0c0c0;
    //     break;
    // case clsTxtCText_Cause_Fail:
    //     style.m_clrText = 0x000000;
    //     break;
    // case clsTxtCText_Cause_Normal:
    //     style.m_clrText = 0x8c8c8c;
    //     break;
    // case clsRecPV_MO_Normal:
    //     style.m_clrFill  = 0x696969;
    //     break;
    // case clsRecPV_MO_Fail:
    //     style.m_clrFill  = 0x0000ff;
    //     break;
    // case clsUnknown: return;
    // default:
    //     AAA(); //все время сюда попадает __debugbreak();
  //  }
}
void ColorizeRecPV140(CHTML& html, CTagDIV* _this)
{
    CTag * shape = _this->m_pPar->m_pPar;
    CStyle& sh_style = shape->Style(html);
 //   const char * src = sh_style.SrcFile(html);
    const char * tagname = Alias(html, shape, "tagname");
    COLORREF color = RGB(0,0,0);
    if (1)/*strstr(src, "LK2B_Cda_CECause_lib_h_pdl_06.sha")
		|| strstr(src, "LK2B_Cda_CECause_lib_h_pai_01.sha")
        || strstr(src, "LK2B_Cda_CEEffect_lib_h_pai_01.sha")
        || strstr(src, "LK2B_Cda_CE_FO_lib_h_pai_01.sha")
        || strstr(src, "LK2B_Cda_CE_E_lib_h_pai_01.sha")
       )*/
    {
        cross::SParamValueH iAlmType_h = FindExperion(html, tagname, "DACA.PV");
        // CAlarmBase* al = html.pipe.Alarm( iAlmType_h.nNumber );
        // if (al)
        // {
        //     BYTE btPriority=al->btEHLN;
        //     BYTE btOnOf = 1;
        //     switch (btPriority)
        //     {
        //     case 2: color=RGB(0,255,255); break;
        //     case 3:color=RGB(255,255,0); break;
        //     case 4:color=RGB(255,0,0); break;
        //     default: color=RGB(0,0,0);
        //     }
        // }

        if (color == 0x00)
            _this->Style(html).m_clrFill = _this->Style(html).m_clrFillEmpty;
        else
            _this->Style(html).m_clrFill = color;
        
        _this->ApplyStyles(NULL, html);  //т.к. вcеравно вызовется после вызова ColorizeRecPV140()
        COLORREF color2 = _this->Style(html).m_clrFill;

        shape = _this->m_pPar;
        int count = shape->GetSize();
        CTag** objs = shape->GetData();
        for(int i = 0; i< count; i++)
        {
            CTag * tag = objs[i];
 //           char * id = tag->Style(html).NameID(html);
            CStyle& style = tag->Style(html);
            // if (strstr(id, "_RecAlm"))
            // {
            //     if (color == 0x00)
            //         style.m_clrFill = tag->Style(html).m_clrFillEmpty;
            //     else
            //         style.m_clrFill = color;
            // }
            // else
            //     if (strstr(id, "_AlpDescTxt"))
            //     {
            //         //if (color != 0x00)
            //         if( color2 == RGB(0,255,255) || color2 == RGB(255,255,0) || color2 == RGB(255,0,0) )
            //             style.m_clrText = 0x00000000;
            //         else
            //             style.m_clrText = RGB(255,255,255);//0x8c8c8c;
            //     }
            //     else
            //         if (strstr(id, "_TxtPName"))
            //         {
            //             //if (color != 0x00)
            //             if( color2 == RGB(0,255,255) || color2 == RGB(255,255,0) || color2 == RGB(255,0,0) )
            //                 style.m_clrText = 0x00000000;
            //             else
            //                 style.m_clrText = RGB(255,255,255);//0x8c8c8c;
            //         }
            //         else
            //             if (strstr(id, "_TxtCText"))
            //             {
            //                 //if (color != 0x00)
            //                 if( color2 == RGB(0,255,255) || color2 == RGB(255,255,0) || color2 == RGB(255,0,0) )
            //                     style.m_clrText = 0x00000000;
            //                 else
            //                     style.m_clrText = RGB(255,255,255);//0x8c8c8c;
            //             }
            //какая-то ошибка, ХЗ как ее показывать на экране
            //else KKK();
        }
    }
    //какая-то ошибка, ХЗ как ее показывать на экране
    //  else KKK();
}
//
void CTagDIV::Matrix( LOCALS )
{
  //  CTag::Script127Exec(this, hDC, html);
    CStyle& style = Style(html);
 //   char* id = style.NameID( html );
    // if (Version == LG35_8_KF && !strcmp(id, "group008") && !html.m_strFile.CompareNoCase("sysdtlPIDA_fp")) return;
    // if (Version == LG35_8_KF && !strcmp(id, "picRedTag") && !html.m_strFile.CompareNoCase("sysdtldevctla_fp")) return;
    // if (Version == LG35_8_KF && !strcmp(id, "picSecRedTag") && !html.m_strFile.CompareNoCase("sysdtldevctla_fp")) return;
    // if (Version == LG35_8_KF){
    //     if (strstr(id, "ScTxtSelectBox")){
    //         if (html.bluecross::RECT == this){
    //             style.m_eBorderStyle = eBorderSolid;
    //         } else {
    //             style.m_eBorderStyle = eBorderUnk;
    //         }
    //     }
    // }

    // if (strstr(id, "AreaAndViewToolbar") == id){
    //     style.colorBkg = 0xf0f0f0;
    // }
    // if (strstr(id, "alpSTATETEXT1") == id){
    //     strcpy(this->m_szScriptValue, "ПП");
    //     TextBox( hDC, html );
    //     return;
    // }
    // if ((Version == LG35_8_KF)&&(strstr(id,"_FaultZ")))
    // {
    //     style.m_clrFill = 0xff000000;
    // }
    //if (Version == LG35_8_KF && strstr(id, "ShaOI_cross::RECT001")){ //sysdtlDEVCTLA_fp
        // char oi0 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[0]"));
        // char oi1 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[1]"));
        // char oi2 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[2]"));
        // if (oi0 == 1 || oi1 == 1 || oi2 == 1)
        // {
        // cross::RECT cross::RECT = style.Fullcross::RECT();
        // CMyBrush brush( hDC, 0x555555 );
        // brush.cross::RECT( cross::RECT );
        // CMyFont font( hDC, style.m_hFont, 0xffffff );
        // ::DrawText( hDC, "OI", 2, &cross::RECT, DT_CENTER | DT_VCENTER );
        // }
        // return;
        // }
        // else if (/*Version == LG35_8_KF &&*/ strstr(id, "ShaPI_cross::RECT001")){ //sysdtlDEVCTLA_fp
        // char pi0 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[0]"));
        // char pi1 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[1]"));
        // char pi2 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[2]"));
        // if (pi0 == 0 || pi1 == 0 || pi2 == 0)
        // {
        //     cross::RECT cross::RECT = style.Fullcross::RECT();
        //     cross::CMyBrush brush( hDC, 0x555555 );
        //     brush.cross::RECT( cross::RECT );
        //     CMyFont font( hDC, style.m_hFont, 0xffffff );
        //     ::DrawText( hDC, "PI", 2, &cross::RECT, DT_CENTER | DT_VCENTER );
        // }
        // return;
  //  }
//     if (strstr(id, "_RecPV")){
//         CTag * shape = this->m_pPar->m_pPar;
//         CStyle& sh_style = shape->Style(html);
//         const char * src = sh_style.SrcFile(html);
//         if (strstr(src, "LK2B_Cda_CEEffect_lib_h_pai_01")){
//             const char * tn = Alias(html, shape, "Tagname");
//             const char * pv = Alias(html, shape, "cp_pv");
//             SParamValueH val = FindExperion(html, tn, pv);
//             // char v = html.pipe.ValueB_H(val);
//             // if (v == 0){
//             //     style.m_clrFill = 0xff;
//             // } else if (v == 1){
//             //     style.m_clrFill = 0x696969;
//             // }
//         }
//     }
// #ifdef _DEBUG
//     if ( stricmp(id, szDebugname ) == 0)
//         AAA();
// #endif

    style.GetCSSStyle();

    // static SHeader wndTopStdData[] =
    //     {
    //         {""               ,   20, true, false, CMsgAL::Image       , 0},
    //         {"Date && Time "  ,  100, true, false, CMsgAL::DateTime    , 1},
    //         {"Source "        ,  150, true, false, CMsgAL::Source      , 3},
    //         {"Condition "     ,   85, true, false, CMsgAL::Condition   , 4},
    //         {"Priority "      ,   75, true, false, CMsgAL::Priority    , 5},
    //         {"Description "   ,  230, true, false, CMsgAL::Description , 6},
    //         {"Trip Value "    ,   90, true, false, CMsgAL::TripValue   , 7},
    //         {"Live Value "    ,   90, true, false, CMsgAL::LiveValue   , 8},
    //         {"Units "         ,10000, true, false, CMsgAL::Units       , 9},
    //         {"END"            ,    0, true, false},
    //         {NULL}
    // //     };
    // if(L35_11600_KF==Version || AM2_NAK == Version)
    // {
    //     if(m_pViewAlarm == NULL && html.m_nTimerCount == 2  && _stricmp(id, "alarmtable001")==0
    //         && (L35_11600_KF==Version && html.m_strFile.CompareNoCase("nav")==0 || AM2_NAK == Version)
    //         && (!theApp.IsAllInOne() || theApp.DisableKirishiView())
    //         )
    //     {
    //         cross::RECT across::RECT = style.Fullcross::RECT();
    //         int ww = int((across::RECT.right-across::RECT.left)*((CViewMnemo*)html.mWnd)->m_fScale);
    //         int hh = int((across::RECT.bottom-across::RECT.top)*((CViewMnemo*)html.mWnd)->m_fScale);
    //         int xx = int(across::RECT.left*((CViewMnemo*)html.mWnd)->m_fScale);
    //         int yy = int(across::RECT.top*((CViewMnemo*)html.mWnd)->m_fScale);
    //         m_pViewAlarm = new CViewAlarm();
    //         m_pViewAlarm->SetHeader( wndTopStdData, RGB(255,255,255), RGB(0,0,0) );
    //         m_pViewAlarm->MyCreate( html.mWnd, 0 );

    //         ::SetWindowPos( m_pViewAlarm->m_hWnd, NULL, xx, yy, ww, hh, SWP_NOZORDER );
    //         ::ShowWindow(m_pViewAlarm->m_hWnd, SW_SHOW);
    //         ::UpdateWindow(m_pViewAlarm->m_hWnd);

    //         m_pViewAlarm->MainTimer();

    //         return;
    //     }
    // }



   //  if (Version == LG35_8_KF && (!html.m_strFile.Find("BL") || !html.m_strFile.Find("bl") || !html.m_strFile.Find("perm01")) && strstr(id, "_RecPV"))
   //  {
   //      ColorizeRecPV140(html, this);
   //  }
   //  if (Version == LG35_8_KF){
   //      CTag * shape = this->m_pPar->m_pPar;
   //      CStyle& sh_style = shape->Style(html);
   //      const char * src = sh_style.SrcFile(html);
   //      if (strstr(src, "LK2B_Cda_CECause_lib_h_pdl_06")
   //          || strstr(src, "LK2B_Cda_CEEffect_lib_h_pai_01")
   //          || strstr(src, "LK2B_Cda_CECause_lib_h_pai_01")){
   //          if ((html.m_bName140 && strstr(id, "_TxtCText")) || (!html.m_bName140 && strstr(id, "_TxtPName")))
   //              style.m_eVisible = enumHidden;
   //          else
   //              style.m_eVisible = enumVisible;
   //      }


   //  }
   //  if ((Version == LG35_8_KF) && !strcmp(id, "sd") && html.m_strFile == cross::CString("sysdtlDIGACQA_fp"))
   //  {
   //      SParamValueH DABLOCKSINCM_param = FindExperion(html, html.m_strGroup, "DIGACQA.DABLOCKSINCM");
   //      if (DABLOCKSINCM_param.def)
   //      {
   //          // int numofFB = html.pipe.ValueI_H(DABLOCKSINCM_param);

   //          // for(int i = 1; i<= numofFB; i++)
   //          // {
   //          //     char tmp[32];
   //          //     sprintf(tmp, "ConShpLstatus%d", i);
   //          //     CTag* ConShpLstatus = shape_get_object(html, html.mBody, tmp);
   //          //     sprintf(tmp, "txtblock%d", i);
   //          //     CTag* txtblock = shape_get_object(html, html.mBody, tmp);
   //          //     if(txtblock)
   //          //     {
   //          //         sprintf(tmp, "ConAlppvsource%d", i);
   //          //         CTag* ConAlppvsource = shape_get_object(html, html.mBody, tmp);
   //          //         if(ConAlppvsource)
   //          //         {
   //          //             ConShpLstatus->Style(html).m_eVisible = enumVisible;
   //          //             txtblock->Style(html).m_eVisible = enumVisible;
   //          //             ConAlppvsource->Style(html).m_eVisible = enumVisible;
   //          //         }
   //          //     }
   //          // }
   //      }
   //  }

   //  if (strstr(id, "ConShpLstatus") == id);
   //  else if ( style.LocalD( hDC ) )
   //      return;
   //  //if ( StopDebug( html, "shape001_ScConAlarmState" ) )
   //  //  return;//AAA();
   //  if (Version == LG35_8_KF
   //      && (strstr(id, "_ScTxtSelectBox") || strstr(id, "_RecFrame"))
   //      && style.m_nRoundness == 30
   //      && style.m_eBorderStyle == eBorderUnk)
   //      style.m_eVisible = enumHidden;
   //  /*if (Version == LG35_8_KF && strstr(id, "TxtPName"))
      // style.m_clrText = 0xffffff;*/
   //  ApplyStyles(hDC, html);
   //  if (DrawTSoft2PumpsIn1(hDC,html, style))
   //      return;
   //  if ( style.GetFlag(TYPE_ALARM_TABLE) )
   //      return;
   //  //
   //  if ( style.m_dwTypeObj & TYPE_ALARMSPEC )
   //      SpecificAlarm( html );
   //  else
   //      if ( style.m_dwTypeObj & TYPE_SYS_BUTTON )
   //          SpecificButton( html );
   //      else
   //          if ( style.m_dwTypeObj & TYPE_ALARMICON )
   //          {
   //              AlarmEnbState( hDC, html );
   //              return;
   //          }
   //  //
   //  int nClass = StyleClass( hDC, html );
   //  if ( nClass == -1 )
   //      return;
   //  //
   //  if ( nClass == clsAlarmEnbS )
   //  {
   //      AlarmEnbState( hDC, html );
   //      return;
   //  }
   //  //
   //  if ( style.m_eVisible == enumHidden )
   //      return;
   //  //
   //  cross::RECT cross::RECT = style.Fullcross::RECT();
   //  if ((Version == AT_PRP) &&
   //      (!strnicmp(html.m_strFile, "k101", 4) || !strnicmp(html.m_strFile, "k1k2", 4) || !strnicmp(html.m_strFile, "vozduh_kip", 10)) &&
   //      (!strnicmp(id, "cross::RECT001", 7) || !strnicmp(id, "cross::RECT002", 7)) &&
   //      (this->m_pPar->GetType() == TT_SHAPE ))
   //  {
   //      CStyle& s = html.Style(this->m_pPar->m_nStyle);
   //      cross::RECT = s.Fullcross::RECT();
   //  }
   //  if ((Version == UKR_PRP) &&
   //      (!strnicmp(html.m_strFile, "screen2", 4) || !strnicmp(html.m_strFile, "screen5", 4)
   //       || !strnicmp(html.m_strFile, "frame2_17", 10) || !strnicmp(html.m_strFile, "frame2_19", 10)
   //       || !strnicmp(html.m_strFile, "trendsumma", 10) || !strnicmp(html.m_strFile, "trendriform", 10)
   //       || !strnicmp(html.m_strFile, "cehovietrnd", 10)
   //       ) &&
   //      (!strnicmp(id, "cross::RECT001", 7) || !strnicmp(id, "cross::RECT002", 7)) &&
   //      (this->m_pPar->GetType() == TT_SHAPE ))
   //  {
   //      CStyle& s = html.Style(this->m_pPar->m_nStyle);
   //      cross::RECT = s.Fullcross::RECT();
   //  }
   //  //
   //  // SBaseDSD* dsd = Dsd(html);
   //  COLORREF color = 0;
   //  bool bPaint = true;
   //  if ( nClass == 1 )
   //  {
        // if ( dsd && dsd->m_nCountPip > 1 )
        // {
        //     if ( dsd->IsOK( html ) )
        //     {}//color = 0xff00ff;
        //     else
        //         color = 0x80ff;
        // }
        // else
        //     if ( mVal.nNumber >= 0 )
        //     {
        // void* val = html.pipe.ParamValue_H(mVal);
        // const char* name = html.pipe.GetMnemoName( mVal.nNumber );
        // if ( val != NULL )
        // {
        //     bool bChangeConstValue=true;
        //     if (Version==LG35_8_KF || Version==L35_11600_KF)
        //     {
        //         if (dsd==NULL)
        //         {
        //             if (!lstrcmp(mVal.def->name,"DESC"))
        //             {
        //                 if (strstr(id,"_Tagname")!=NULL)
        //                     bChangeConstValue=false;
        //             }
        //         }
        //     }

        //     if(Version == L35_11600_KF)
        //     {
        //         if(html.m_strFile.CompareNoCase("TK602_OIL")==0 || html.m_strFile.CompareNoCase("TK602")==0)
        //         {
        //             if(TYPE_TEXT == TYPE_PNT)
        //                 if(mVal.nNumber>=0)
        //                     if(strcmp(mVal.def->name, "PVFL")==0)
        //                         if(strcmp(&name[strlen(name)-_countof(".AUTO")+1], ".AUTO") == 0)
        //                         {
        //                             bChangeConstValue = false;
        //                             if(strcmp(m_constValue, "AUTO")==0)
        //                             {
        //                                 if(*(char*)val == 0)
        //                                     return;
        //                                 style.m_clrText = RGB(34,139,34);
        //                             }
        //                             else
        //                                 if(strcmp(m_constValue, "MAN")==0)
        //                                 {
        //                                     if(*(char*)val != 0)
        //                                         return;
        //                                     style.m_clrText = RGB(255,0,0);
        //                                 }
        //                         }
        //         }
        //         else
        //             if(html.m_strFile.CompareNoCase("BL_TK601_PERM")==0 || html.m_strFile.CompareNoCase("BL_TK602_PERM")==0)
        //             {
        //                 if(TYPE_TEXT == TYPE_PNT)
        //                     if(mVal.nNumber>=0)
        //                         if(strcmp(&name[strlen(name)-_countof(".DIGACQA")+1], ".DIGACQA") == 0)
        //                         {
        //                             bChangeConstValue = false;
        //                             if(strstr(m_constValue, " РАЗРЕШЁН"))
        //                             {
        //                                 bChangeConstValue = false;
        //                                 if(strcmp((char*)val, "НЕ РАЗРЕШЕНО") == 0)
        //                                     return;
        //                                 style.m_clrText = RGB(30,255,30);
        //                             }
        //                             else
        //                                 if(strstr(m_constValue, " ЗАПРЕШЁН"))
        //                                 {
        //                                     bChangeConstValue = false;
        //                                     if(strcmp((char*)val, "НЕ РАЗРЕШЕНО") != 0)
        //                                         return;
        //                                     style.m_clrText = RGB(255,0,0);
        //                                 }
        //                         }
        //             }
        //     }

        //     if( Version == T1165P_ONPZ && TYPE_PNT == TYPE_POLYGON
        //         && style.m_bClrFillCanChangedByScript
        //         && !strnicmp(html.m_strFile, "TSB_NASOS_1165_ZAGAZ_GR_4", 20))
        //         bChangeConstValue = false;

        //     if( Version == T1163_ONPZ && TYPE_PNT == TYPE_POLYGON
        //         && style.m_bClrFillCanChangedByScript
        //         && !strnicmp(html.m_strFile, "TSB_NASOS_1163_ZAGAZ_GR_1", 20))
        //         bChangeConstValue = false;

        //     if (bChangeConstValue)
        //     {
        //         char txt[512] = "";
        //         bPaint = FillText( html, txt, TYPE_PNT );
        //         m_constValue = txt;
        //     }
        // }
        // else
        // {
        //     if ( ::IsMnemoStyle(MNEMO_EDIT) )
        //         if ( TYPE_PNT != TYPE_GROUP )
        //         {
        //             //color = 0x00f0ff;
        //         }
        // }
        // }
        // else
        //     if ( mVal.nNumber == -2 )
        //     {
        //         if ( !::IsMnemoStyle(MNEMO_EDIT) )
        //         {//чтобы эти элементы не рисовались
        //             if (Version == LG35_8_KF)
        //                 style.m_clrFill = 0xff000000, style.m_clrLine = 0xff000000; //так было
        //             else if(strstr(html.m_strFile, "_fp"))
        //                 return;  //только на фейсплейтах не рисуем
        //         }
        //     }
   // }
    //
    // if (Version == LG35_8_KF)
    //     Change140ProjectAlarmFillColor(hDC,html,style,id);
    // if ( color != 0 )
    // {
    //     // CMyBrush brush( hDC, color );
    //     // brush.cross::RECT( cross::RECT );
    // }
    // //
    // if ( style.colorBkg != 0xffffffff )
    // {
    //     // CMyBrush brush( hDC, style.colorBkg );
    //     // brush.cross::RECT( cross::RECT );
    // }
    // //
    // if ( *m_szScriptValue )
    // {
    //     ASSERT( TYPE_PNT == TYPE_TEXT );
    // }
    // //
    // switch ( TYPE_PNT )
    // {
    // case TYPE_10        : style.Type10  ( hDC, html, cross::RECT ); break;
    // case TYPE_LINE      : style.Line    ( hDC, html, cross::RECT ); break;
    // case TYPE_ARC       : style.Arc     ( hDC, html, cross::RECT ); break;
    // case TYPE_cross::RECT      :
    //     SetAlamColor(html, style);
    //     style.cross::RECT    ( hDC, html, cross::RECT );
    //     if (Version!=LG35_8_KF  && (mVal.nNumber>=0 && mVal.def->eVal==enumValueDbl && style.m_Dicross::RECTion!=-1  /*&& m_valHI.nNumber>=0 && m_valLO.nNumber>=0*/))
    //     { //закрашиваемый по значениям прямоугольник
    //         // double dVal = html.pipe.ValueF_H(mVal);
    //         double dVal =0;
    //         // const char* name = html.pipe.GetMnemoName( mVal.nNumber );
    //         const char* name = "";
    //         double dH = 100.;//html.pipe.ValueF_H( m_valHI );

    //         double dL = 0.;//html.pipe.ValueF_H( m_valLO );
    //         double dd = dH-dL;
    //         if(dd>0.0)
    //         {
    //             cross::RECT rc = cross::RECT;
    //             ::Inflatecross::RECT(&rc, -1, 0);
    //             dVal = fmax(min(dVal, dH), dL);
    //             int nD = int((dVal-dL)/dd*(rc.bottom-rc.top));
    //             if (style.m_Dicross::RECTion == 0)
    //             {//вниз
    //                 rc.top = rc.bottom - nD;
    //             }
    //             else
    //                 if (style.m_Dicross::RECTion == 1)
    //                 {//вверх
    //                     rc.bottom = rc.top + nD;

    //                 }
    //             COLORREF color = RGB(0,255,255);
    //             // CMyBrush brush( hDC, color );
    //             // brush.cross::RECT( rc );
    //         }
    //     }
    //     else
    //         if (Version==LG35_8_KF || Version==L35_11600_KF)
    //         {
    //             if(_stricmp(html.m_strFile, "nav")==0)
    //             {
    //                 if( _stricmp(id+8, "_ScRecAlarm")==0)
    //                 {//окрашивание прямоугольников алармов под названиями мнемосхем
    //                     const char *MnemoName = Alias(html, m_pPar->m_pPar, "DspName");
    //                     if(MnemoName)
    //                     {
    //                         cross::RECT almcross::RECT(cross::RECT);
    //                         almcross::RECT.top= almcross::RECT.bottom - 10;
    //                         almcross::RECT.bottom-=2;
    //                         almcross::RECT.left+=2;
    //                         almcross::RECT.right = almcross::RECT.left - 1 + (almcross::RECT.right - almcross::RECT.left)/3;

    //                         // //получить параметры алармов по имени мнемосхемы:
    //                         // SAlarmsOnMnemo aom = GetAppWnd()->GetAlarmsForMnemo(MnemoName);
    //                         // int nState=0;
    //                         // if (aom.bRedNotAck) nState=1;
    //                         // if (aom.bRedBlink) nState=2;
    //                         // if ((nState==1) ||(nState==2&&PulsTimer()))
    //                         // {
    //                         //     CMyBrush brush( hDC, RGB(255,0,0));
    //                         //     brush.cross::RECT( almcross::RECT );
    //                         // }
    //                         ::Offsetcross::RECT(&almcross::RECT, almcross::RECT.right - almcross::RECT.left, 0);
    //                         // nState=0;
    //                         // if (aom.bYellowNotAck) nState=1;
    //                         // if (aom.bYellowBlink) nState=2;

    //                         // if((nState==1) ||(nState==2&&PulsTimer()))
    //                         // {
    //                         //     CMyBrush brush( hDC, RGB(255,255,0));
    //                         //     brush.cross::RECT( almcross::RECT );
    //                         // }
    //                         // ::Offsetcross::RECT(&almcross::RECT, almcross::RECT.right - almcross::RECT.left, 0);

    //                         // nState=0;
    //                         // if (aom.bBlueNotAck) nState=1;
    //                         // if (aom.bBlueBlink) nState=2;
    //                         // if((nState==1) ||(nState==2&&PulsTimer()))
    //                         // {
    //                         //     CMyBrush brush( hDC, RGB(0,255,255));
    //                         //     brush.cross::RECT( almcross::RECT );
    //                         // }
    //                     }
    //                 }
    //             }
    //         }
    //     break;
    // case TYPE_ROUNDcross::RECT : style.Round   ( hDC, html, cross::RECT ); break;
    // case TYPE_OVAL      : style.Oval    ( hDC, html, cross::RECT ); break;
    // case TYPE_POLYGON   : style.Polygon ( hDC, html, cross::RECT ); break;
    // case TYPE_WEDGE     : style.Wedge   ( hDC, html, cross::RECT ); break;
    // case TYPE_POLYLINE  : style.Polyline( hDC, html, cross::RECT ); break;
    // case TYPE_BEZIER    : style.Bezier  ( hDC, html, cross::RECT ); break;
    // case TYPE_TEXT      :
    // case TYPE_ACK_TOTAL_AL  : // Квитированные сигнализации все
    // case TYPE_UNA_TOTAL_AL  : // Неквитированные сигнализации все
    // case TYPE_ACK_TOTAL_OF  : // Квитированные сигнализации of
    // case TYPE_UNA_TOTAL_OF  : // Неквитированные сигнализации of
    // case TYPE_ACK_TOTAL_FL  : // Квитированные сигнализации отфильтрованные
    // case TYPE_UNA_TOTAL_FL  : // Неквитированные сигнализации отфильтрованные
    // case TYPE_FILTER_APPLIED: // Наличие фильтра
    // case TYPE_PAUSE_ALARM   : // Пауза обновлений
    //     TextBox( hDC, html );
    //     break;
    // case TYPE_IMAGE:
    //     // if ( style.m_nBkgImage )
    //     //     Background( hDC, mImg, cross::RECT );
    //     // else
    //     //     OutImage  ( hDC, mImg, cross::RECT, true );
    //      break;
    // };
}
//
void CTagDIV::Change140ProjectAlarmFillColor( HDC hDC, CHTML& html, CStyle& style,char* id )
{
    if ((style.m_dwFlags &HDX_ALMSTATE)&&( strstr(id,"_Alarm" )!=NULL || strstr(id,"_oval044" )!=NULL))
    {
//         CAlarmBase* al = html.pipe.Alarm( mVal.nNumber );
//         if (al!=NULL)
//         {
//             BYTE btKvit = al->notACK ? 0 : 1;
//             BYTE btPriority=al->btEHLN;
//             BYTE btOnOf = 1;
//             if ( PulsTimer() || btKvit )
//             {
//                 switch (btPriority)
//                 {
//                 case 2:style.m_clrFill=RGB(0,255,255); break;
//                 case 3:style.m_clrFill=RGB(255,255,0); break;
//                 case 4:style.m_clrFill=RGB(255,0,0); break;
//                 default:
//                     style.m_clrFill=style.m_clrFillEmpty;
//                     //если эти цвета только по алармам - то что ниже - убрать
//                     /*if(style.m_nBreakpointType == 1)
//                                {
//                                void* val = html.pipe.ParamValue_H(mVal);
// #ifdef _DEBUG
//                                const char* name = html.pipe.GetMnemoName( mVal.nNumber );
// #endif
//                                if ( val != NULL && mVal.def->eVal == enumValueChr)
//                                    style.m_clrFill = style.DiscColor(html, *(int*)val);
//                                 }
//                                */
//                     break;
//                 }
//             }
//             else
//                 style.m_clrFill=style.m_clrFillEmpty;
//         }
    }
}

bool CTagDIV::DrawTSoft2PumpsIn1(HDC hDC,CHTML& html, CStyle& style)
{
    // if (style.mClass==clsT_SOFT2PumpsIn1)
    // {
        if ( mVal.nNumber >= 0 )
        {
            // void* val = html.pipe.ParamValue_H(mVal);
            // if (( val != NULL )&&(mVal.def->eVal==enumValueChr))
            // {
            //     BYTE bSecondPump=0;
            //     BYTE bFirstPump=0;
            //     BYTE* pValue=(BYTE*) val;
            //     bFirstPump=*pValue;
            //     //получение второй точки, она в следующем тэге
            //     CStyle& styleText=html.Style(m_nStyle+1);
            //     if ( styleText.m_nPlaceDsd > 0 )
            //     {
            //         SBaseDSD* dsd=html.DSD(styleText.m_nPlaceDsd);
            //         if (dsd!=NULL)
            //         {
            //             SParamValueH* var2 = dsd->Value(html);
            //             if (var2!=NULL)
            //             {
            //                 // void* val2 = html.pipe.ParamValue_H(*var2);
            //                 // if (( val2 != NULL )&&(var2->def->eVal==enumValueChr))
            //                 // {
            //                 //     BYTE* pValue2=(BYTE*) val2;
            //                 //     bSecondPump=*pValue2;
            //                 //     COLORREF clr=RGB(128,128,128);
            //                 //     std::string strText="";
            //                 //     if ((bFirstPump==5)&&(bSecondPump==5))
            //                 //     {
            //                 //         strText="&";
            //                 //         clr=RGB(0,255,0);
            //                 //     }
            //                 //     else
            //                 //     {
            //                 //         if (bFirstPump==5)
            //                 //         {
            //                 //             strText="1";
            //                 //             clr=RGB(0,255,0);
            //                 //         }
            //                 //         else
            //                 //             if (bSecondPump==5)
            //                 //             {
            //                 //                 strText="2";
            //                 //                 clr=RGB(0,255,0);
            //                 //             }
            //                 //             else
            //                 //             {
            //                 //                 strText="!";
            //                 //                 bool bBlinkChangeColor=PulsTimer();
            //                 //                 if (bBlinkChangeColor)
            //                 //                     clr=RGB(255,0,0);
            //                 //             }
            //                 //     }
            //                 //     SetColor(html,clr);
            //                 //     cross::RECT cross::RECTPumps = style.Fullcross::RECT();
            //                 //     style.Oval    ( hDC, html, cross::RECTPumps );
            //                 //     styleText.TextOut( hDC, styleText.m_clrText, cross::RECTPumps, strText.c_str(), true );
            //                 //     return true;
            //                 // }
            //             }
            //         }
            //     }
          //  }
        }
  //  }
    return false;
}

void CTagDIV::TextBox( HDC hDC, CHTML& html )
{
    CStyle& style = Style(html);
    cross::RECT rect = style.OutsRect();
     if (Version== LG35_8_KF && html.m_strFile == cross::CString("sysAlarmSummary") 
         //&& style.mcross::RECT.X == 35 && style.mcross::RECT.Y == 2 && style.mcross::RECT.Width == 200 && style.mcross::RECT.Height == 28
          )
     {
            cross::Gdiplus::Graphics g(hDC);

            cross::Gdiplus::SolidBrush blueBrush(cross::Gdiplus::Color(255, 138, 149, 170));
            cross::Gdiplus::SolidBrush whiteBrush(cross::Gdiplus::Color(255, 255, 255, 255));

            g.FillRectangle(&blueBrush, cross::Gdiplus::RectF(0,0,html.Size().cx+10,37));
            cross::Gdiplus::Font fnt("Arial", 12, cross::FontStyleBold);
            g.DrawString("Alarms", 6, &fnt, cross::Gdiplus::RectF(rect.left, rect.top, rect.right-rect.left,  rect.bottom-rect.top), NULL, &whiteBrush);
         return;
     }
    if ( style.m_dwFlags & (HDX_CLRLINE|HDX_CLRFILL) )
        style.Rect( hDC, html, rect );
    else
        if (Version == UKR_PRP && !html.m_strFile.CompareNoCase("LIGHT_TIME"))
            style.Rect( hDC, html, rect );
        else
            if((Version >= AM2_NAK) && (style.m_dwFlags & HDX_LINE_S) && (style.m_nLineStyle >0) && style.m_clrLine != 0xff000000)
                style.Rect( hDC, html, rect );

    const char* text = m_constValue;
    if ( *m_szScriptValue || (Version >= AM2_NAK && TYPE_PNT == TYPE_TEXT && mVal.nNumber == -1) )
        text = m_szScriptValue;
    /*
  if ( strchr( text, '5' ) != NULL )
    {
    static HFILE h = _lcreat( "c:/aaa.sss", 0 );
    char* id = style.NameID( html );
    _hwrite( h, id, lstrlen(id) );
    _hwrite( h, " ", 1 );
    _hwrite( h, text, lstrlen(text) );
    _hwrite( h, "\r\n", 2 );
    return;KKK();
    }*/
    //else
    if (Version==Реформинг_КНПЗ)
    { //MIHAIL_ADD_HWL рисуется серый прямоугольник и меняется цвет текста на белый, буква MODE
        // if (style.mClass==clsTxtMode)
        // {
        //     style.m_clrText=0xffffff;
        //     style.m_clrFill=0x808080;
        //     // CMyBrush brush( hDC, style.m_clrFill );
        //     // brush.cross::RECT( cross::RECT );
        // }
    }

    if(DOTF_PM==Version) //Blink пока только тут
    {
       if(style.mTextColorBlink)
            if(PulsTimer())
                text = NULL;
    }

    if ( text && style.m_clrText != 0xff000000 )
    {
        char *ref = (char*)strstr(text, "&#");
        if( Version >= L35_11600_KF &&
            ref && isdigit(ref[2]) && strchr(ref, ';')
            )
        {//тут символы unicode
            wchar_t wtext[512];
            wcsncpy(wtext, string_to_wstring(text).c_str(), ref-text);
            wtext[ref-text] = 0;
            int ii = 0;
            int jj = int(ref-text);
            wtext[jj] = 0;
            const char *refd = strchr(ref, ';');
            const char *lastrefd = refd;
            do
            {
                unsigned int wc = atoi(&ref[ii+2]);
                if(wc < 0x10000)
                {
                    wtext[jj++] = (wchar_t)wc;
                    wtext[jj] = 0;
                    refd = strchr(refd+1, ';');
                    if(refd == NULL)
                        break;
                    lastrefd = refd;
                    ii= refd - text + 1;
                }
                else break;
            }
            while (text[ii]=='&' && text[ii+1]=='#' && isdigit(text[ii+2]));

            wcscpy(&wtext[jj], string_to_wstring(lastrefd+1).c_str());
            style.TextOutW( hDC, style.m_clrText, rect, wtext, true );
        }
        else
            style.TextOut( hDC, style.m_clrText, rect, text, true );
    }
    //
    // if ( ::IsMnemoStyle( MNEMO_EDIT ) )
    //     if ( mVal.def == NULL )
    //         if ( mVal.nNumber != -1 )
    //             style.BadValue( hDC, "Надо вставить в модель!!! ");
}
//
void CTagDIV::AlarmEnbState( LOCALS )
{
    CStyle& style = Style(html);
    char* id = style.NameID( html );
    if ( StopDebug( html, "shape082_ConAlarmState" ) )
        AAA();
    // //
    CAlarmBase* al = html.pipe.Alarm( mVal.nNumber );
    //LPCTSTR name = html.pipe.GetMnemoName(mVal.nNumber);
    if ( al == NULL ) return;
    // //
     cross::RECT rect = style.FullRect();
    // //
     SIZE sz = { 16, 16 };// Заведомо знаем размер картинки
     int w = ((rect.right-rect.left)-sz.cx)/2;
     int h = ((rect.bottom-rect.top)-sz.cy)/2;
     POINT pt = { rect.left+w, rect.top+h };
    // //
    // BYTE btKvit = al->notACK ? 0 : 1;
    // // Для квитирования нужны права
    // bool  bUnit = html.pipe.IsTrueUnit( mVal.nNumber );
    // if ( !bUnit ) btKvit = 1;
    // //
    // BYTE btOnOf = 1;// В отличии от сигнализаций, здесь предистория неизвестна
    // //
    // theApp.DrawAlarmIcon( hDC, pt, NULL, al->btEHLN, btKvit, btOnOf,al->Shelved  );
}
//
// void CTagDIV::Action( CHTML& html, HWND hWnd, bool bDown, CPoint * pt )
// {
//     CStyle& style = Style(html);
//     char* id = style.NameID(html);
//     if (Version == LG35_8_KF && html.m_strFile == "sysAlarmSummary" && strstr(id, "btnDropAreaPane"))
//     {
//         if (html.m_pAreaPane)
//         {
//             CTagAreaPane* pane=(CTagAreaPane*)html.m_pAreaPane;
//             pane->ToggleView();
//         }
//         return;
//     }
//     if (Version == LG35_8_KF && ( !html.m_strFile.CompareNoCase("sysdtlMOS_fp") || !html.m_strFile.CompareNoCase("sysdtlPOS_fp"))  && strstr(id, "shpPVFL"))
//     {
//         BYTE nOld = html.pipe.ValueB_H(mVal);
//         BYTE pnNew;
//         if (strstr(id, "shpPVFL1"))
//         {
//             pnNew = 1;
//         }
//         else if (strstr(id, "shpPVFL2"))
//         {
//             pnNew = 0;
//         }
//         if (nOld != pnNew)
//         {
//             BYTE nNew=pnNew;
//             SSendToModel send;
//             lstrcpy( send.szValue, mVal.def->name );
//             send.Set( enumValueChr, &nOld, &nNew );
//             html.pipe.SendData( mVal.nNumber, send, true );
//         }
//         return;
//     }
//     if (Version == LG35_8_KF && strstr(id, "cross::RECTarg") && pt)
//     {
//         CTag * gH = shape_get_object(html, this->m_pPar, "groupH");
//         CTag * gV = shape_get_object(html, this->m_pPar, "groupV");

//         INT_PTR nHc = gH->GetSize();
//         CTag** pHo = gH->GetData();

//         INT_PTR nVc = gV->GetSize();
//         CTag** pVo = gV->GetData();

//         for(int i = 0; i<nHc; i++)
//         {
//             for(int j = 0; j<nVc; j++)
//             {
//                 char* v_id = pVo[j]->Style(html).NameID(html);
//                 char* h_id = pHo[i]->Style(html).NameID(html);

//                 cross::RECT rcH;
//                 rcH.top = pHo[i]->Style(html).mcross::RECT.Y;
//                 rcH.left = pHo[i]->Style(html).mcross::RECT.X;
//                 rcH.bottom = pHo[i]->Style(html).mcross::RECT.Y + pHo[i]->Style(html).mcross::RECT.Height;
//                 rcH.right = pHo[i]->Style(html).mcross::RECT.X + pHo[i]->Style(html).mcross::RECT.Width;

//                 cross::RECT rcV;
//                 rcV.top = pVo[j]->Style(html).mcross::RECT.Y;
//                 rcV.left = pVo[j]->Style(html).mcross::RECT.X;
//                 rcV.bottom = pVo[j]->Style(html).mcross::RECT.Y + pVo[j]->Style(html).mcross::RECT.Height;
//                 rcV.right = pVo[j]->Style(html).mcross::RECT.X + pVo[j]->Style(html).mcross::RECT.Width;

//                 POINT pnt;
//                 pnt.x = pt->x / ((CViewMnemo*)html.mWnd)->m_fScale;
//                 pnt.y = pt->y / ((CViewMnemo*)html.mWnd)->m_fScale;

//                 if(::PtIncross::RECT(&rcH, pnt) && ::PtIncross::RECT(&rcV, pnt))
//                 {
//                     CTag * vsel = shape_get_object(html, this->m_pPar, "cross::RECTVSel");
//                     CTag * hsel = shape_get_object(html, this->m_pPar, "cross::RECTHSel");

//                     vsel->Style(html).mcross::RECT.Y      =  rcV.top;
//                     vsel->Style(html).mcross::RECT.Height = (rcH.bottom - rcV.top);
//                     vsel->Style(html).mcross::RECT.X      =   rcV.left;
//                     vsel->Style(html).mcross::RECT.Width  = (rcV.right - rcV.left);
//                     vsel->Style(html).m_eVisible   =  enumVisible;

//                     hsel->Style(html).mcross::RECT.Y      =   rcH.top;
//                     hsel->Style(html).mcross::RECT.Height = (rcH.bottom - rcH.top);
//                     hsel->Style(html).mcross::RECT.X      =  rcH.left;
//                     hsel->Style(html).mcross::RECT.Width  = (rcV.right - rcH.left);
//                     hsel->Style(html).m_eVisible   =  enumVisible;
//                     return;
//                 }
//             }
//         }
//     }
//     if (CAplicFrm::IsOpenFaceplateByScTxtSelectBox() && strstr(id, "ScTxtSelectBox") )
//     {
//         if ( (html.m_strFile.Find("bl") == 0 || html.m_strFile.Find("BL") == 0))
//         {
//             html.bluecross::RECT = this;
//             if (true && pt){
//                 CTag* cross::RECTarg = shape_get_object(html, html.mBody, "cross::RECTarg");
//                 CTag * gH = shape_get_object(html, cross::RECTarg->m_pPar, "groupH");
//                 CTag * gV = shape_get_object(html, cross::RECTarg->m_pPar, "groupV");

//                 INT_PTR nHc = gH->GetSize();
//                 CTag** pHo = gH->GetData();

//                 INT_PTR nVc = gV->GetSize();
//                 CTag** pVo = gV->GetData();

//                 for(int i = 0; i<nHc; i++)
//                 {
//                     for(int j = 0; j<nVc; j++)
//                     {
//                         char* v_id = pVo[j]->Style(html).NameID(html);
//                         char* h_id = pHo[i]->Style(html).NameID(html);

//                         cross::RECT rcH;
//                         rcH.top = pHo[i]->Style(html).mcross::RECT.Y;
//                         rcH.left = pHo[i]->Style(html).mcross::RECT.X;
//                         rcH.bottom = pHo[i]->Style(html).mcross::RECT.Y + pHo[i]->Style(html).mcross::RECT.Height;
//                         rcH.right = pHo[i]->Style(html).mcross::RECT.X + pHo[i]->Style(html).mcross::RECT.Width;

//                         cross::RECT rcV;
//                         rcV.top = pVo[j]->Style(html).mcross::RECT.Y;
//                         rcV.left = pVo[j]->Style(html).mcross::RECT.X;
//                         rcV.bottom = pVo[j]->Style(html).mcross::RECT.Y + pVo[j]->Style(html).mcross::RECT.Height;
//                         rcV.right = pVo[j]->Style(html).mcross::RECT.X + pVo[j]->Style(html).mcross::RECT.Width;

//                         POINT pnt;
//                         pnt.x = pt->x / ((CViewMnemo*)html.mWnd)->m_fScale;
//                         pnt.y = pt->y / ((CViewMnemo*)html.mWnd)->m_fScale;

//                         if(::PtIncross::RECT(&rcH, pnt) && ::PtIncross::RECT(&rcV, pnt))
//                         {
//                             CTag * vsel = shape_get_object(html, cross::RECTarg->m_pPar, "cross::RECTVSel");
//                             CTag * hsel = shape_get_object(html, cross::RECTarg->m_pPar, "cross::RECTHSel");

//                             vsel->Style(html).mcross::RECT.Y      =  rcV.top;
//                             vsel->Style(html).mcross::RECT.Height = (rcH.bottom - rcV.top);
//                             vsel->Style(html).mcross::RECT.X      =   rcV.left;
//                             vsel->Style(html).mcross::RECT.Width  = (rcV.right - rcV.left);
//                             vsel->Style(html).m_eVisible   =  enumVisible;

//                             hsel->Style(html).mcross::RECT.Y      =   rcH.top;
//                             hsel->Style(html).mcross::RECT.Height = (rcH.bottom - rcH.top);
//                             hsel->Style(html).mcross::RECT.X      =  rcH.left;
//                             hsel->Style(html).mcross::RECT.Width  = (rcV.right - rcH.left);
//                             hsel->Style(html).m_eVisible   =  enumVisible;
//                             i = nHc + 1;
//                             break;
//                         }
//                     }
//                 }


//             }
//         }
//         CTag * otarget;
//         CTag * oshape = this->m_pPar->m_pPar;

//         if (shape_contains_object(html, oshape, "ScConAlpState"))
//             otarget = shape_get_object(html, oshape, "ScConAlpState");
//         else
//             otarget = shape_get_object(html, oshape, "ScConAlpPV");

//         const char * sfaceplatefile = Alias(html, oshape, "faceplatefile");

//         if (*sfaceplatefile == 0)
//         {
//             if(otarget)
//                 otarget->Action(html, hWnd, bDown);
//             else
//             {
//                 if(Version == AM2_NAK || Version == DMW_NAK)
//                 {
//                     otarget = shape_get_object(html, oshape, "PbFrame");
//                     if(otarget)
//                     {
//                         SBaseDSD* dsd = otarget->Dsd(html);
//                         if(dsd)
//                             dsd->ActionDSD( hWnd, html );
//                     }
//                 }
//             }
//         }
//         else if (*sfaceplatefile == '-' && Version == LG35_8_KF );
//         else
//         {
//             //GetAppWnd()->ChoiseFaceplate( Alias(html, oshape, "tagname"), GetAppWnd()->GiveAmm(hWnd), sfaceplatefile );
//             if (Version != LG35_8_KF)
//             {
//                 if ( !IsAction( html, hWnd, bDown ) )
//                     GetAppWnd()->ChoiseFaceplate( Alias(html, oshape, "tagname"), GetAppWnd()->GiveAmm(hWnd) );
//             }
//             else{
//                 if ( GetAppWnd()->m_wndSens && hWnd == GetAppWnd()->m_wndSens.m_wndMnemo->m_hWnd )
//                 {//для сенсорной панели
//                     GetAppWnd()->m_wndSens.ResetFaceplate();
//                     if ((strcmpi(sfaceplatefile,"sysdtlmos_fp.htm")!=0)&&(strcmpi(sfaceplatefile,"sysdtlflaga_rr_fp.htm")!=0)&&(strcmpi(sfaceplatefile,"sysdtlpos_fp.htm")!=0))
//                         //GetAppWnd()->m_wndSens.m_wndfp->Faceplate(Alias(html, oshape, "tagname"), sfaceplatefile);
//                         GetAppWnd()->m_wndSens.GoFP(Alias(html, oshape, "tagname"), sfaceplatefile);//.m_wndfp->Faceplate();
//                     else
//                     {
//                         int rr = !strcmp(sfaceplatefile, "sysdtlflaga_rr_fp.htm");
//                         const char * pv   = rr ? Alias(html, oshape, "cp_pv")   : 0;
//                         const char * rst  = rr ? Alias(html, oshape, "cp_rst")  : 0;
//                         const char * desc = rr ? Alias(html, oshape, "cp_desc") : 0;
//                         GetAppWnd()->ChoiseFaceplate( Alias(html, oshape, "tagname"), GetAppWnd()->GiveAmm(hWnd, true), sfaceplatefile, pv, rst, desc );

//                     }
//                 }
//                 else {//открывается и в своём окне и на сенсорной панели
//                     CAplicFrm* appwnd = GetAppWnd();
//                     if (!strcmp(sfaceplatefile, "sysDtlInterlocka_fp.htm"))
//                     {
//                         appwnd->GiveAmm()->GoToPage(sfaceplatefile, Alias(html, oshape, "tagname"));
//                         //appwnd->ChoiseFaceplate( Alias(html, oshape, "tagname"), GetAppWnd()->GiveAmm(hWnd, true), sfaceplatefile, 0, 0, 0 );
//                     }
//                     else
//                     {
//                         int rr = !strcmp(sfaceplatefile, "sysdtlflaga_rr_fp.htm");
//                         const char * pv   = rr ? Alias(html, oshape, "cp_pv")   : 0;
//                         const char * rst  = rr ? Alias(html, oshape, "cp_rst")  : 0;
//                         const char * desc = rr ? Alias(html, oshape, "cp_desc") : 0;
//                         appwnd->ChoiseFaceplate( Alias(html, oshape, "tagname"), GetAppWnd()->GiveAmm(hWnd, true), sfaceplatefile, pv, rst, desc );
//                         if (::IsWindow(appwnd->m_wndSens.m_hWnd))
//                         {
//                             appwnd->m_wndSens.ResetFaceplate();
//                             if ((strcmpi(sfaceplatefile,"sysdtlmos_fp.htm")!=0)&&(strcmpi(sfaceplatefile,"sysdtlflaga_rr_fp.htm")!=0)&&(strcmpi(sfaceplatefile,"sysdtlpos_fp.htm")!=0))
//                                 //appwnd->m_wndSens.m_wndfp->Faceplate(Alias(html, oshape, "tagname" ), sfaceplatefile, pv, rst, desc);
//                                 appwnd->m_wndSens.GoFP(Alias(html, oshape, "tagname" ), sfaceplatefile, pv, rst, desc);
//                         }
//                     }
//                 }
//             }
//         }
//         if (html.m_tagTargShape140 && pt)
//         {
//             cross::RECT rcRT;
//             CStyle& style140 = html.m_tagTargShape140->Style(html);
//             rcRT.top = style140.mcross::RECT.Y;
//             rcRT.left = style140.mcross::RECT.X;
//             rcRT.bottom = style140.mcross::RECT.Y + style140.mcross::RECT.Height;
//             rcRT.right = style140.mcross::RECT.X + style140.mcross::RECT.Width;

//             POINT pnt;
//             pnt.x = pt->x / ((CViewMnemo*)html.mWnd)->m_fScale;
//             pnt.y = pt->y / ((CViewMnemo*)html.mWnd)->m_fScale;

//             if(::PtIncross::RECT(&rcRT, pnt))
//             {
//                 ((CTagDIV*)html.m_tagTargShape140)->Action(html, hWnd, bDown, pt);
//             }
//         }
//         return;
//     }

//     if(!bDown)
//     {
//         if ((this->m_vctrOnClickScripts.size() > 0 && this->m_vctrOnClickScripts[0] == usNav_SelectBox_OnClick_me_ ))
//         {
//             m_vctrOnClickScripts.clear();
//             const char * dspName = Alias(html, this->m_pPar->m_pPar, "DspName");
//             if (theApp.IsAllInOne())
//                 GetAppWnd()->GiveRegularAmm(hWnd)->GoToPage(dspName,html.Group());
//             else
//                 GetAppWnd()->GiveAmm(hWnd)->GoToPage(dspName,html.Group());
//             return;
//         }
//     }
//     CTag::Action( html, hWnd, bDown );
// }


void CTagDIV::SetAlamColor(CHTML& html, CStyle& style)
{
    if (Version == LG35_8_KF)
        return; //там как то по-другому

    if (style.m_dwFlags & HDX_ALMSTATE)
    {
        // CAlarmBase* al = html.pipe.Alarm( mVal.nNumber );
        //if (al!=NULL)
        {
            //BYTE btKvit = al->notACK ? 0 : 1;
            BYTE btKvit =0;

            //BYTE btPriority=al->btEHLN;
            BYTE btPriority =0;
            BYTE btOnOf = 1;
            // if ( PulsTimer() || btKvit )
            // {
            //     //может зависить от проекта
            //     switch (btPriority)
            //     {
            //     case 2:style.m_clrFill=RGB(255,255,0); break;//style.m_clrFill=RGB(0,255,255); break;
            //     case 3:
            //     case 4:style.m_clrFill=RGB(255,0,0); break;
            //     default:
            //         style.m_clrFill=style.m_clrFillEmpty;
            //         break;
            //     }
            // }
            // else
            //     style.m_clrFill=style.m_clrFillEmpty;
        }
    }
}

