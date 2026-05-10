#include "StdAfx.h"
#include "TagBODY.h"
#include "HTML.h"
#include "Style.h"

CTagBODY::CTagBODY(void)
  {
  }
//
#undef  STD_VAR
#define STD_VAR CHTML& html, CTagBODY& a, char* value
typedef void (*LPFuncs)( STD_VAR );
//
void bodyStyle( STD_VAR )
  {
  a.AnalizStyle( html, value );
  }
//
void bodyScrol( STD_VAR )
  {
  ASSERT( lstrcmpi( value, "no" ) == 0 );
  }
//
void bodyOnLoad       ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnUnload     ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnDragStart  ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnSelectStart( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnFocusOut   ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnDblClick   ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnClick      ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void bodyOnResize     ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
//
void CTagBODY::Analiz( ANALIZ )
  {
  struct SLocal { char* name; LPFuncs func; };
  static SLocal list[] =
    {
    { "style"         , bodyStyle         },
    { "scroll"        , bodyScrol         },
    { "onload"        , bodyOnLoad        },
    { "onunload"      , bodyOnUnload      },
    { "ondragstart"   , bodyOnDragStart   },
    { "onselectstart" , bodyOnSelectStart },
    { "onfocusout"    , bodyOnFocusOut    },
    { "ondblclick"    , bodyOnDblClick    },
    { "onclick"       , bodyOnClick       },
    { "onresize"      , bodyOnResize      },
    }InitLocal;
  //
  int n = 0;
  while ( teg[n].name )
    {
    SLocal* find = FindLocal( teg[n].name );
    ASSERT( find );
    if ( find != NULL )
    (*find->func)(html,*this,teg[n].value);
    n++;
    }
  //
  CStyle& style = Style(html);
  if ( style.colorBkg != 0xffffffff )
    {
    html.Color(style.colorBkg);
    style.colorBkg = 0xffffffff;
    }
  else
    html.Color(::GetSysColor( COLOR_WINDOW ));
  style.m_dwFlags |= HDX_PRESENT;
  //
  ASSERT( html.mBody == NULL );
  html.mBody = this;
  //
  }

  void CTagBODY::Kerdyk(HWND hWnd, CHTML &html)
  {
      CTag::Kerdyk( hWnd, html );
      if ((Version == LG35_8_KF || Version == AM2_NAK || Version == DMW_NAK) && html.m_strFile == cross::CString("sysDtlInterlocka_fp"))
      {
          m_strInterlockContainer.Reset();
          html.Title("Interlock");
          //	page.style.backgroundColor = g_ColorSet_Array(1,0) 'ColorBackGround
          //	loadingText.fillColor = g_ColorSet_Array(1,0) 'ColorBackGround
          //	loadingText.textColor = g_ColorSet_Array(1,1) 'ColorMsgText
          //	loadingText.style.zIndex = 40 'Bring Member loadingText to Front
          //	MsgBoxField.style.padding = 5 'Set some Space between Text and Border
          //	MsgBoxField.fillColor = g_ColorSet_Array(1,2) 'ColorMsgBackGround
          //	MsgBoxField.textColor = g_ColorSet_Array(1,1) 'ColorMsgText
          //	CM_Description.textColor = g_ColorSet_Array(1,1) 'ColorMsgText
          //	CM_Display.textColor = g_ColorSet_Array(1,1) 'ColorMsgText
          //	al_text.textColor = g_ColorSet_Array(1,1) 'ColorMsgText
          //	for z = 1 To 12
          //		page.all("Upstream" & z).fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          //		page.all("Upstream" & z).textColor = g_ColorSet_Array(1,11) 'ColorGatterTextDefault
          //		page.all("UpstreamBypass" & z).fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          //		page.all("UpstreamBypass" & z).textColor = g_ColorSet_Array(1,11) 'ColorGatterTextDefault
          //		page.all("Negator" & z).fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          //		page.all("Negator" & z).textColor = g_ColorSet_Array(1,11) 'ColorGatterTextDefault
          //		page.all("Bypass_BG" & z).fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          //		page.all("C" & z & "FNeg").fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          //	next
          //	page.all("FinalBypass_BG").fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          // get_object(&html, "textGate1")->Style(html).m_clrText = 0;
          // get_object(&html, "textGate2")->Style(html).m_clrText = 0;
          // get_object(&html, "textGate3")->Style(html).m_clrText = 0;
          // get_object(&html, "textGate4")->Style(html).m_clrText = 0;

          // get_object(&html, "Gate1")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "Gate2")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "Gate3")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "Gate4")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "Gate1")->Style(html).m_dwFlags |= HDX_CLRFILL;
          // get_object(&html, "Gate2")->Style(html).m_dwFlags |= HDX_CLRFILL;
          // get_object(&html, "Gate3")->Style(html).m_dwFlags |= HDX_CLRFILL;
          // get_object(&html, "Gate4")->Style(html).m_dwFlags |= HDX_CLRFILL;

          // get_object(&html, "NegGate1")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "NegGate2")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "NegGate3")->Style(html).m_clrFill = 0xcccccc;
          // get_object(&html, "NegGate4")->Style(html).m_clrFill = 0xcccccc;

          //	backButton.textColor = g_ColorSet_Array(1,11) 'ColorGatterTextDefault
          //	backButton.fillColor = g_ColorSet_Array(1,12) 'ColorGatterBackGroundDefault
          //	call showAcclock
          //	call SetCheckboxStatus
          //	call DrawAllObjects
          //	g_UpdateAllowed = true
          //	g_CM_Array(0,0) = DisplayDataRepository.GetValue("CurrentPoint")
          //	g_CM_Array(1,0) = DisplayDataRepository.GetValue("InterlockName")
          //	al_combobox.AddString "OPERATOR"
          //	al_combobox.AddString "SUPERVISOR"
          //	al_combobox.AddString "ENGINEER"
          //	al_combobox.AddString "PROGRAM"
      }
  }
static CTag* _get_object(CHTML* html, CTag* tag, char * id)
{
    char * _id = tag->Style(*html).NameID(*html);
    if (!strcmp(_id, id)) return tag;
    else
    {
        INT_PTR nCount = tag->GetSize();
        CTag** children = tag->GetData();
        for ( INT_PTR n = 0; n < nCount; n++ )
        {
            CTag* rslt = _get_object(html, children[n], id);
            if (rslt) return rslt;
        }
    }
    return NULL;
}
static CTag* get_object(CHTML* html, char * id)
{
    return _get_object(html, html->mBody, id);
}
