//#include "StdAfx.h"
#include "Tag.h"
#include "TagA.h"
//#include "FormatScn.h"
#include "Style.h"
#include "HTML.h"
#include <shlwapi.h>
#include   <cstdlib>
#include "../Function.h"
#include <map>
#undef FORMATSCN_API
#ifdef UTILS_EXPORTS
# define FORMATSCN_API __declspec(dllexport)
#else
# define FORMATSCN_API __declspec(dllimport)
#endif

void     FORMATSCN_API SkipSpace( char*& ptr, int* pRow  );

#undef ASSERT
#define ASSERT(V)

#define InitLocal ;\
static int nCount = sizeof(list)/sizeof(list[0]);\
    static bool bInit = true;\
    Qsort( list, nCount, sizeof(list[0]), bInit )


#define FindLocal( Text )  (SLocal*)SearchName( Text, list, nCount, sizeof(list[0]) )

struct SItemCrd
{
    HCHAR m_nName;
    EDataTypes eType;
    int   m_nCount;
    HCHAR m_nFile[4];
    int   m_nNumb[4];
};

int SortCrd( const void * d1, const void * d2 )
{
    SItemCrd* p1 = (SItemCrd*)d1;
    SItemCrd* p2 = (SItemCrd*)d2;
    if ( p1->eType > p2->eType ) return  1;
    if ( p1->eType < p2->eType ) return -1;
    return 0;
}


CStyle::CStyle()
: m_fZoom(1)
, marginX(0)
, marginY(0)
, m_fntStyle(cross::FontStyleRegular)
, m_fHeight(0)
, m_eTextUnit(UnitPoint)
, m_hFont(NULL)
, align(DT_LEFT)
, m_nAddTextcross::RECT(-1)
, m_nAddTextcross::RECTW(0)
, m_eVisible (enumVisible)
, m_eOverflow(enumVisible)
, m_nBkgImage(0)
, m_nFamily(0)
, nTabIndex(-1)
  {
      memset(m_szTitle, 0, sizeof m_szTitle);
  }

CStyle::~CStyle(void)
  {
  }
//
#undef  STD_VAR
#define STD_VAR CHTML& html, CStyle& a, CTag* pTag, float& marginZ, char* value, Scross::RECT& s
typedef void (*LPFuncs)( STD_VAR );
//
void styleLEFT  ( STD_VAR ){ s.Pars( a, html, value, 0 ); a.m_dwFlagObj |= STYLE_LEFT  ; }
void styleRIGHT ( STD_VAR ){ s.Pars( a, html, value, 1 ); a.m_dwFlagObj |= STYLE_RIGHT ; }
void styleWIDTH ( STD_VAR ){ s.Pars( a, html, value, 2 ); a.m_dwFlagObj |= STYLE_WIDTH ; }
void styleTOP   ( STD_VAR ){ s.Pars( a, html, value, 3 ); a.m_dwFlagObj |= STYLE_TOP   ; }
void styleBOTTOM( STD_VAR ){ s.Pars( a, html, value, 4 ); a.m_dwFlagObj |= STYLE_BOTTOM; }
void styleHEIGHT( STD_VAR ){ s.Pars( a, html, value, 5 ); a.m_dwFlagObj |= STYLE_HEIGHT; }
//
void styleBEHAVIOR( STD_VAR )// url(#HDXVectorFactory#text);
  {
  struct SLocal { char* name; DWORD dwType; };
  static SLocal list[] =
    {
    { "#StnEvtReflector"                       , TYPE_REFLECTOR     },
    { "#HDXPageBehavior"                       , TYPE_PAGEBEHAVIOR  },
    { "#DisplayDataRepository"                 , TYPE_REPOSITORY    },
    { "#HSCShapeLinkBehavior"                  , TYPE_SHAPELINKBEH  },
    { "#BindingBehavior"                       , TYPE_BINDING       },
    { "#HDXAlphaBehavior"                      , TYPE_ALPHA         },
    { "#HoverBehavior"                         , TYPE_HOVER         },
    { "#HDXPopupBehavior#Faceplate"            , TYPE_FACEPLATE     },
    { "#hscbreakpointbehavior"                 , TYPE_BREAKPOINT    },
    { "#HDXPopupBehavior#Schematic"            , TYPE_SCHEMATIC     },
    { "#HDXElementsFactory#Table#HDXBinding"   , TYPE_ALARM_TABLE   },
    { "#HSCScriptDataBehavior"                 , TYPE_SCRIPTDATA    },
    { "#HSCIndicatorBehavior"                  , TYPE_INDICATOR     },
    { "#HSCShapeLinkBehavior#shapelinkanimator", TYPE_ANIMATOR      },
    //
    { "#HDXVectorFactory#line"                 , TYPE_LINE          },
    { "#HDXVectorFactory#cross::RECT"                 , TYPE_cross::RECT          },
    { "#HDXVectorFactory#polygon"              , TYPE_POLYGON       },
    { "#HDXVectorFactory#roundcross::RECT"            , TYPE_ROUNDcross::RECT     },
    { "#HDXVectorFactory#oval"                 , TYPE_OVAL          },
    { "#HDXVectorFactory#text"                 , TYPE_TEXT          },
    { "#HDXVectorFactory#arc"                  , TYPE_ARC           },
    { "#HDXVectorFactory#wedge"                , TYPE_WEDGE         },
    { "#HDXVectorFactory#polyline"             , TYPE_POLYLINE      },
    { "#HDXVectorFactory#bezier"               , TYPE_BEZIER        },
    //
    { "#HDXPushButtonBehavior"                 , TYPE_PUSHBUTTON    },
    { "#HDXVectorFactory#shapelink"            , TYPE_SHAPELINK     },
    { "#HDXVectorFactory#group"                , TYPE_GROUP         },
    { "#HDXVectorFactory#image"                , TYPE_IMAGE         },
    //
    { "#default#userData"                      , TYPE_SYS_USERDATA  },
    { "#HDXElementsFactory#Button"             , TYPE_SYS_BUTTON    },
    { "#HSCComboboxBehavior"                   , TYPE_COMBOBOX      },
    { "#AlarmStateIconFactory#AlarmIcon"       , TYPE_ALARMICON     },
    { "#HSCCheckBoxBehavior"                   , TYPE_CHECKBOX      },
      };

  static int nCount = sizeof(list)/sizeof(list[0]);
      static bool bInit = true;
  std::qsort( list, nCount, sizeof(list[0]), SortCrd );

  //
  char* ptr = value;
  while ( 1 )
    {
    char* p = strstr( ptr, "url(" );
    if ( p == NULL ) break;
    ptr = p+4;
    p = strstr( ptr, ")" );
    ASSERT( p );
    if ( p != NULL )
      {
      *p = 0;
      SLocal* find = FindLocal( ptr );
      //if ( lstrcmpi( ptr, "#HDXPopupBehavior#GroupFaceplate" ) == 0 )
      //  AAA();
//Serge?      ASSERT( find );
      if ( find != NULL )
        {
        if ( find->dwType & TYPE_PAINT )
          {
          ASSERT( ( a.m_dwTypeObj & TYPE_PAINT ) == 0 );
          a.AddFlag(find->dwType);
          }
        a.m_dwTypeObj |= find->dwType;
        }
      }
    else break;
    ptr = p+1;
    }
  //
  // РќРµ РѕР±СЂР°Р±Р°С‚С‹РІР°РµРј
  }
//
void styleCOLOR( STD_VAR )// #ffd700;
  {
  a.colorTxt = ColorDiez(value);
  }
//
void styleFONT_WEIGHT( STD_VAR )// bold;
  {
  if ( lstrcmpi( value, "normal" ) == 0 )
    a.m_fntStyle = cross::FontStyleRegular;
  else
  if ( lstrcmpi( value, "bold" ) == 0 )
    a.m_fntStyle = cross::FontStyleBold;
  else
    {
    ASSERT( ::IsInt(value) );
    int lfWeight = atoi(value);
    ASSERT( lfWeight == 400 );
    a.m_fntStyle = cross::FontStyleRegular;
    }
  }
//
void styleMARGIN( STD_VAR )// 0px
  {
  marginZ = Number( value );
  if ( marginZ != 0 )
  a.m_dwFlagObj |= STYLE_MARGIN;
  ASSERT( lstrcmpi( value, "px" ) == 0 );
  }
//
void styleMARGINY( STD_VAR )// 0px
  {
  a.marginY = Number( value );
  if ( a.marginY != 0 )
  a.m_dwFlagObj |= STYLE_MARGIN_Y;
  ASSERT( lstrcmpi( value, "px" ) == 0 );
  }
//
void styleMARGINX( STD_VAR )// 0px
  {
  a.marginX = Number( value );
  if ( a.marginX != 0 )
  a.m_dwFlagObj |= STYLE_MARGIN_X;
  ASSERT( lstrcmpi( value, "px" ) == 0 );
  }
//
void styleFONT_SIZE( STD_VAR )// 12pt;
  {
  a.m_fHeight = Number( value );
  if ( lstrcmpi( value, "pt" ) == 0 )
    a.m_eTextUnit = UnitPoint;
  else
  if ( lstrcmpi( value, "px" ) == 0 )
    a.m_eTextUnit = UnitPixel;
  else
    {
    ASSERT(0);
    }
  }
//bold 8pt Helvetica || 8pt Helvetica
void styleFONT( STD_VAR )// 
  {
  SkipSpace( value );
  if ( _strnicmp( value, "bold", 4 ) == 0 )
    a.m_fntStyle = cross::FontStyleBold, value += 4;
  //
  SkipSpace( value );
  a.m_fHeight = Number( value );
  if ( _strnicmp( value, "pt", 2 ) == 0 )
    a.m_eTextUnit = UnitPoint, value += 2;
  else
    ASSERT(0);
  //
  SkipSpace( value );
  ASSERT( lstrlen(value) < LF_FACESIZE );
  a.m_nFamily = a.AddTexts( html, value );
  }
//
void styleFONT_STYLE( STD_VAR )// normal;
  {
  if ( lstrcmpi( value, "normal" ) == 0 ) return;
  if ( lstrcmpi( value, "italic" ) == 0 )
    {
    a.m_dwFlagObj |= (DWORD)STYLE_ITALIC;
    return;
    }
  ASSERT(0);
  }
//
void styleFONT_FAMILY( STD_VAR )// Arial;
  {
  ASSERT( lstrlen(value) < LF_FACESIZE );
  if (!lstrcmp(value,"Felix Titling"))
	  a.m_nFamily = a.AddTexts( html, "Arial" );
  else
	  a.m_nFamily = a.AddTexts( html, value );
  }
//
void stylePOSITION( STD_VAR )// absolute;
  {
  bool Abs = lstrcmpi( value, "absolute" ) == 0;
  bool Rel = lstrcmpi( value, "relative" ) == 0;
  ASSERT( Abs || Rel );
  if ( Rel ) a.m_dwFlags |= HDX_RELATIVE;
  }
//
void styleTEXT_ALIGN( STD_VAR )// center
  {
  a.m_dwFlags |= HDX_ALIGN;
  if ( lstrcmpi( value, "center" ) == 0 )
    a.align = DT_CENTER;
  else
  if ( lstrcmpi( value, "left" ) == 0 )
    a.align = DT_LEFT;
  else
  if ( lstrcmpi( value, "right" ) == 0 )
    a.align = DT_RIGHT;
  else
  if ( lstrcmpi( value, "justify" ) == 0 )
    a.align = 0;//РќР°РґРѕ СЂР°Р·РѕР±СЂР°С‚СЊСЃСЏ DT_RIGHT;
  else
    {
    ASSERT(0);
    }
  }
//
void styleBACKGROUND_REPEAT( STD_VAR )// no-repeat
  {
  ASSERT( lstrcmpi( value, "no-repeat" ) == 0 );
  }
//
void styleZOOM( STD_VAR )// 1
  {
  ASSERT( ::IsFlt(value) );
  a.m_fZoom = (float)atof(value);
  if ( a.m_fZoom < 0.01 ) a.m_fZoom = 1.0;
  a.m_dwFlagObj |= STYLE_ZOOM;
  ASSERT( a.m_fZoom > 0 );
  }
//
void styleBACKGROUND_COLOR( STD_VAR )// #908080
  {
  if ( lstrcmpi( value, "transparent" ) == 0 )return;
  a.colorBkg = ColorDiez(value);
  }
//
void styleTEXT_DECORATION( STD_VAR )// none
  {
  if ( lstrcmpi( value, "none" ) == 0 ) return;
  if ( lstrcmpi( value, "underline" ) == 0
  ||   lstrcmpi( value, "none underline" ) == 0 )
    {
    a.m_dwFlagObj |= STYLE_UNDER ;
    return;
    }
  ASSERT(0);
  }
//
void styleBORDER_TOP_COLOR   ( STD_VAR ) { a.mT.Parsers( value ); }
void styleBORDER_LEFT_COLOR  ( STD_VAR ) { a.mL.Parsers( value ); }
void styleBORDER_RIGHT_COLOR ( STD_VAR ) { a.mR.Parsers( value ); }
void styleBORDER_BOTTOM_COLOR( STD_VAR ) { a.mB.Parsers( value ); }
//
void styleBORDER_TOP    ( STD_VAR ) { a.mT.Parsers( value ); }
void styleBORDER_LEFT   ( STD_VAR ) { a.mL.Parsers( value ); }
void styleBORDER_RIGHT  ( STD_VAR ) { a.mR.Parsers( value ); }
void styleBORDER_BOTTOM ( STD_VAR ) { a.mB.Parsers( value ); }
//
void stylePADDING( STD_VAR )
  {
  CBorder obj;
  obj.Padding( value, STYLE_PAD_T|STYLE_PAD_L|STYLE_PAD_R|STYLE_PAD_B, a.m_dwFlagObj );
  a.mT.padding = obj.padding;
  a.mL.padding = obj.padding;
  a.mR.padding = obj.padding;
  a.mB.padding = obj.padding;
  }

void stylePADDING_TOP   ( STD_VAR ) { a.mT.Padding( value, STYLE_PAD_T, a.m_dwFlagObj ); }
void stylePADDING_LEFT  ( STD_VAR ) { a.mL.Padding( value, STYLE_PAD_L, a.m_dwFlagObj ); }
void stylePADDING_RIGHT ( STD_VAR ) { 
    if (Version == LG35_8_KF && html.m_strFile == cross::CString("sysAlarmSummary") &&!lstrcmpi(value, "5px")) return;
	a.mR.Padding( value, STYLE_PAD_R, a.m_dwFlagObj ); }
void stylePADDING_BOTTOM( STD_VAR ) { 
    if (Version == LG35_8_KF && html.m_strFile ==  cross::CString("sysAlarmSummary") &&!lstrcmpi(value, "5px")) return;
	a.mB.Padding( value, STYLE_PAD_B, a.m_dwFlagObj ); }
//
void styleBORDER_TOP_STYLE   ( STD_VAR ) { a.mT.Parsers( value ); }
void styleBORDER_LEFT_STYLE  ( STD_VAR ) { a.mL.Parsers( value ); }
void styleBORDER_RIGHT_STYLE ( STD_VAR ) { a.mR.Parsers( value ); }
void styleBORDER_BOTTOM_STYLE( STD_VAR ) { a.mB.Parsers( value ); }
//
inline EVisibility Visible(char* value)
  {
  if ( lstrcmpi( value, "inherit" ) == 0 ) return enumInherit;
  if ( lstrcmpi( value, "visible" ) == 0 ) return enumVisible;
  if ( lstrcmpi( value, "hidden"  ) == 0 ) return enumHidden;
  if ( lstrcmpi( value, "auto"    ) == 0 ) return enumAuto;
  ASSERT(0);
  return enumVisible;
  }
void styleVISIBILITY( STD_VAR )
  {
  a.m_dwFlagObj |= STYLE_VISIBLE;
  if ( a.m_eVisible != enumHidden )
    a.m_eVisible = Visible(value);
  }
//
inline EVisibility Display(char* value)
  {
  if ( lstrcmpi( value, "inline" ) == 0 ) return enumVisible;
  if ( lstrcmpi( value, "none"   ) == 0 ) return enumHidden;
  ASSERT(0);
  return enumVisible;
  }
void styleDISPLAY( STD_VAR )
  {
  a.m_dwFlags |= HDX_DISPLAY;
  if ( a.m_eVisible != enumHidden )
    a.m_eVisible = Display(value);
  }
//
void styleOVERFLOW( STD_VAR )// hidden;
  {
  a.m_eOverflow = Visible(value);
  }
//
void styleWORD_WRAP( STD_VAR )
  {
  ASSERT( lstrcmpi( value, "normal" ) == 0 || lstrcmpi( value, "break-word" ) == 0 );
  }
//
void styleROWS( STD_VAR )
  {
  ASSERT( lstrcmpi( value, "1" ) == 0 );
  }
//
void styleCURSOR( STD_VAR )
  {
  if ( lstrcmpi( value, "default" ) == 0 )
    {
    }
  else
  if ( lstrcmpi( value, "hand" ) == 0 )
    {
    }
  else
  if ( lstrcmpi( value, "col-resize" ) == 0 )
    {
    }
  else
  if ( lstrcmpi( value, "row-resize" ) == 0 )
    {
    }
  else
    {
    ASSERT( 0 );
    }
  }
//
void styleFILTER( STD_VAR )
  {
  if ( _strnicmp( value, "progid:", 7 ) == 0 )
    a.m_dwFlagObj |= STYLE_PROGID;
  }
//
void styleSCROLLBAR_FACE_COLOR( STD_VAR )
  {
  }
//
void styleSCROLLBAR_HIGHLIGHT_COLOR( STD_VAR )
  {
  }
//
void styleSCROLLBAR_SHADOW_COLOR( STD_VAR )
  {
  }
//
void styleSCROLLBAR_3DLIGHT_COLOR( STD_VAR )
  {
  }
//
void styleSCROLLBAR_ARROW_COLOR( STD_VAR )
  {
  }
//
void styleSCROLLBAR_TRACK_COLOR( STD_VAR )
  {
  }
//
void styleSCROLLBAR_DARKSHADOW_COLOR( STD_VAR )
  {
  }
//
void styleBACKGROUND_IMAGE( STD_VAR )
  {
  if ( lstrcmpi( value, "none" ) == 0 )
    return;
  int dl = lstrlen(value);
  if ( dl == 0 ) return; dl--;
  ASSERT( value[dl] == ')' );
  value[dl] = 0;
  int n = StrCmpNI( value, "url(", 4 );
  ASSERT( n == 0 );
  if ( n == 0 ) value += 4;
  n = StrCmpNI( value, "file:", 5 );
  if ( n == 0 ) value += 5;
  a.m_nBkgImage = a.AddTexts( html, value );
  }
//
void CStyle::AnalizSTYLE( CHTML& html, CTag* pTag, char* ptr )
  {
  //if ( lstrcmp( ptr, "position: relative; left: 6px; top: 1px; " ) == 0 )
  //  {
  //  int yy = 90;
  //  }
  //
  CTag* par = pTag->m_pPar;
  if  ( par )
    {
    CStyle& style = par->Style(html);
    // mcross::RECT = style.mcross::RECT;
    // mcross::RECT.X += style.mL.padding;
    // mcross::RECT.Y += style.mT.padding;
    // mcross::RECT.Width  -= (style.mR.padding+style.mL.padding);
    // mcross::RECT.Height -= (style.mB.padding+style.mT.padding);
    }
  //
  int N;

  int  FORMATSCN_API ParserEXT ( char*& ptr, STegScn*& scn, int& nCount );

  // STegScn* teg = theApp.ParsEXT( ptr, N );
  ASSERT( N > 0 );
  //
  struct SLocal { char* name; LPFuncs func; };
  static SLocal list[] =
    {
    { "LEFT"               , styleLEFT                },// 1300px
    { "RIGHT"              , styleRIGHT               },// 1300px
    { "WIDTH"              , styleWIDTH               },// 110px
    { "TOP"                , styleTOP                 },// 214px
    { "BOTTOM"             , styleBOTTOM              },// 214px
    { "HEIGHT"             , styleHEIGHT              },// 43px
    { "BEHAVIOR"           , styleBEHAVIOR            },//
    { "OVERFLOW"           , styleOVERFLOW            },// hidden
    { "COLOR"              , styleCOLOR               },// #ffd700
    { "MARGIN"             , styleMARGIN              },// 0px
    { "MARGINX"            , styleMARGINX             },// 0px
    { "MARGINY"            , styleMARGINY             },// 0px
    { "FONT-WEIGHT"        , styleFONT_WEIGHT         },// bold
    { "FONT-SIZE"          , styleFONT_SIZE           },// 12pt
    { "FONT-STYLE"         , styleFONT_STYLE          },// normal
    { "FONT-FAMILY"        , styleFONT_FAMILY         },// Arial
    { "POSITION"           , stylePOSITION            },// absolute
    { "TEXT-ALIGN"         , styleTEXT_ALIGN          },// center
    { "BACKGROUND-REPEAT"  , styleBACKGROUND_REPEAT   },// no-repeat
    { "ZOOM"               , styleZOOM                },// 1
    { "BACKGROUND-COLOR"   , styleBACKGROUND_COLOR    },// #908080
    { "TEXT-DECORATION"    , styleTEXT_DECORATION     },// none
    { "BORDER-TOP-COLOR"   , styleBORDER_TOP_COLOR    },
    { "BORDER-LEFT-COLOR"  , styleBORDER_LEFT_COLOR   },
    { "BORDER-RIGHT-COLOR" , styleBORDER_RIGHT_COLOR  },
    { "BORDER-BOTTOM-COLOR", styleBORDER_BOTTOM_COLOR },
    { "BORDER-TOP-STYLE"   , styleBORDER_TOP_STYLE    },
    { "BORDER-LEFT-STYLE"  , styleBORDER_LEFT_STYLE   },
    { "BORDER-RIGHT-STYLE" , styleBORDER_RIGHT_STYLE  },
    { "BORDER-BOTTOM-STYLE", styleBORDER_BOTTOM_STYLE },
    { "BORDER-TOP"         , styleBORDER_TOP          },
    { "BORDER-LEFT"        , styleBORDER_LEFT         },
    { "BORDER-RIGHT"       , styleBORDER_RIGHT        },
    { "BORDER-BOTTOM"      , styleBORDER_BOTTOM       },
    { "PADDING"            , stylePADDING             },
    { "PADDING-TOP"        , stylePADDING_TOP         },
    { "PADDING-LEFT"       , stylePADDING_LEFT        },
    { "PADDING-RIGHT"      , stylePADDING_RIGHT       },
    { "PADDING-BOTTOM"     , stylePADDING_BOTTOM      },
    { "VISIBILITY"         , styleVISIBILITY          },
    { "DISPLAY"            , styleDISPLAY             },
    { "WORD-WRAP"          , styleWORD_WRAP           },
    { "ROWS"               , styleROWS                },
    { "CURSOR"             , styleCURSOR              },
    { "FILTER"             , styleFILTER              },
    { "BACKGROUND-IMAGE"   , styleBACKGROUND_IMAGE    },
    { "BACKGROUND"         , styleBACKGROUND_IMAGE    },
    { "FONT"               , styleFONT                },// 8pt Helvetica
    { "SCROLLBAR-FACE-COLOR"       , styleSCROLLBAR_FACE_COLOR       },
    { "SCROLLBAR-HIGHLIGHT-COLOR"  , styleSCROLLBAR_HIGHLIGHT_COLOR  },
    { "SCROLLBAR-SHADOW-COLOR"     , styleSCROLLBAR_SHADOW_COLOR     },
    { "SCROLLBAR-3DLIGHT-COLOR"    , styleSCROLLBAR_3DLIGHT_COLOR    },
    { "SCROLLBAR-ARROW-COLOR"      , styleSCROLLBAR_ARROW_COLOR      },
    { "SCROLLBAR-TRACK-COLOR"      , styleSCROLLBAR_TRACK_COLOR      },
    { "SCROLLBAR-DARKSHADOW-COLOR" , styleSCROLLBAR_DARKSHADOW_COLOR },
    }InitLocal;
  //
  Scross::RECT scross::RECT;
  memset( &scross::RECT, 0, sizeof(Scross::RECT) );
  float marginZ;
  for ( int n = 0; n < N; n++ )
    {
    // SLocal* find = FindLocal( teg[n].name );
    // //ASSERT( find );
    // if ( find != NULL )
    // (*find->func)(html,*this,pTag,marginZ,teg[n].value,scross::RECT);
    }
  scross::RECT.Setcross::RECT( html, pTag, html.m_bResize );
  //
  if ( m_dwFlagObj & STYLE_MARGIN )
    {
    marginX = marginZ;
    marginY = marginZ;
    }
  else
  if ( m_dwFlagObj & (STYLE_MARGIN_X|STYLE_MARGIN_Y) )
    m_dwFlagObj |= STYLE_MARGIN;
  //
  }

void CStyle::Queue( CHTML& html, CTag* pTag )
  {
  CreateFont( html, pTag, m_fntStyle );
  //
  if ( m_nBkgImage )
    {
    char* value = html.Buffer(m_nBkgImage);
    // if ( LoadImages( value, pTag->mImg ) )
    //   {
    //   if ( html.m_bBinData ) return;
    //   AddFlag(TYPE_IMAGE);
    //   }
    }
  }

int FindFamily( CHTML& html, CTag* pTag )
  {
  while ( pTag )
    {
    CStyle& style = pTag->Style(html);
    if ( style.m_nFamily != 0 )
      return style.m_nFamily;
    pTag = pTag->m_pPar;
    }
  return 0;
  }

float FindHeight( CHTML& html, CTag* pTag )
  {
  while ( pTag )
    {
    CStyle& style = pTag->Style(html);
    if ( style.m_fHeight != 0 )
      return style.m_fHeight;
    pTag = pTag->m_pPar;
    }
  return 0;
  }

void CStyle::CreateFont( CHTML& html, CTag* pTag, cross::FontStyle fntStyle )
  {
  m_fntStyle = fntStyle;
  //
  if ( GetFlag(TYPE_TEXT) || GetFlag(TYPE_COMBOBOX) || GetFlag(TYPE_CHECKBOX)
  ||   GetFlag(TYPE_PUSHBUTTON) ||   GetFlag(TYPE_ALARM_TABLE)
  ||   (m_dwTypeObj & TYPE_ALPHA )
  //||   typeid(*pTag) == typeid(CTagA)
  )
    {
    if ( m_nFamily == 0 )
      m_nFamily = FindFamily( html, pTag );
    //
    if ( m_fHeight == 0 )
      m_fHeight = FindHeight( html, pTag );
    //
    char* value = html.Buffer(m_nFamily);
    LOGFONT lf;
    memset( &lf, 0, sizeof(lf) );
    lstrcpy( lf.lfFaceName, value );
    lf.lfQuality = CLEARTYPE_QUALITY;
    // if ( m_eTextUnit == UnitPoint )
    //   lf.lfHeight = -MulDiv( int(m_fHeight+0.5), GetDeviceCaps( html.m_hHDC, LOGPIXELSY ), 72);
    // else
    //   lf.lfHeight = -int(m_fHeight+0.5);
      
    // char* id = NameID( html );
    // if(!strcmp(id,"shaSeqCmd_cmbCom"))
    // 	lf.lfHeight = -12;
    //
    if ( fntStyle == cross::FontStyleRegular )
      lf.lfWeight = FW_NORMAL;
    else
    if ( fntStyle == cross::FontStyleBold )
      lf.lfWeight = FW_BOLD;
    else { ASSERT(0); }
    //
    if ( m_dwFlagObj & STYLE_ITALIC ) lf.lfItalic    = 1;
    if ( m_dwFlagObj & STYLE_UNDER  ) lf.lfUnderline = 1;
    //
  //  m_hFont = theApp.GiveFont( lf );
    }
  }

  cross::RECT CStyle::FullRect()
  {
      cross::RECT rc = { int(mRect.X+0.5),
                 int(mRect.Y+0.5),
                 int(mRect.X+mRect.Width+0.5),
                 int(mRect.Y+mRect.Height+0.5) };
      return rc;
  }

  cross::RECT CStyle::OutsRect()
  {
      cross::RECT rc = { int(mRect.X+mL.padding+0.5),
                 int(mRect.Y+mT.padding+0.5),
                 int(mRect.X-mR.padding+mRect.Width +0.5),
                 int(mRect.Y-mB.padding+mRect.Height+0.5) };
      return rc;
  }

void CStyle::Local( LOCALS )
  {
  if ( m_dwFlagObj & (STYLE_ZOOM|STYLE_MARGIN) )
    {
    XFORM xf;
    memset( &xf, 0, sizeof(xf) );
    xf.eM11 = m_fZoom;
    xf.eM22 = m_fZoom;
    xf.eDx  = marginX*xf.eM11;
    xf.eDy  = marginY*xf.eM22;
 //   ::ModifyWorldTransform( hDC, &xf, MWT_LEFTMULTIPLY );
    }
  //
  }

void CStyle::Posts( HDC hDC, CHTML& html )
  {
  if ( m_dwFlagObj & (STYLE_ZOOM|STYLE_MARGIN) )
    {
    XFORM xf;
    memset( &xf, 0, sizeof(xf) );
    xf.eM11 = float(1./m_fZoom);
    xf.eM22 = float(1./m_fZoom);
    xf.eDx  = -marginX;
    xf.eDy  = -marginY;
 //   ::ModifyWorldTransform( hDC, &xf, MWT_LEFTMULTIPLY );
    }
  }

void CStyle::TextOut( HDC hDC, COLORREF color, cross::RECT rc, const char* text, bool bTop, COLORREF back )
  {
//   CMyFont font( hDC, m_hFont, color );
//   if ( m_nAddTextcross::RECT < 0 )
//     {
//     m_nAddTextcross::RECT = 0;
//     cross::RECT cross::RECT;
//     font.Calc( cross::RECT, text );
//     int w = cross::RECT.right - cross::RECT.left;
//     int W = rc.right - rc.left;
//     //
//     const char* a = text;
//     while ( *a )
//       {
//       if ( *a == '\n' )
//         {
//         align |= DT_WORDBREAK;
//         break;
//         }
//       a++;
//       }
//     //
//     if ( W > w && (align & DT_WORDBREAK)==0 )
//       align |= DT_SINGLELINE | (bTop ? DT_TOP : DT_VCENTER);
//     else
//     {
//       if(strchr(text, ' ') == NULL && (align & DT_WORDBREAK)==0)
//         m_nAddTextcross::RECTW = min(9, w - W); //РѕРґРЅРѕ СЃР»РѕРІРѕ
//       else align |= DT_WORDBREAK;
//     }
// 	if (Version == LG35_8_KF && !strcmp(text, "Р РµР¶. Р РµРіРµРЅРµСЂР°С†РёРё"))
// 	{
// 		align |= 0;
// 		align |= DT_SINGLELINE;
// 		align &= ~DT_WORDBREAK;
// 	}
//     int h = cross::RECT.bottom - cross::RECT.top;
//     int H = rc  .bottom - rc  .top;
//     if ( h > H ) m_nAddTextcross::RECT = (h-H);
// 	if ((Version==LG35_8_KF)&&(lstrcmp(text,"Р РµР·РµСЂРІСѓР°СЂ СЃРјР°Р·РѕС‡РЅРѕРіРѕ РјР°СЃР»Р° Р•Рњ-1")==0))
// 		m_nAddTextcross::RECT=(H-h);
//     }
//   rc.bottom += m_nAddTextcross::RECT;
//   //
// #if 0
//   CMyPen pen( hDC, 0xff );
//   pen.cross::RECT( rc );
// #endif
//   //
//   if ( align & DT_CENTER )
//     rc.left -= 2,rc.right += 2;
//   else
//   if ( align & DT_RIGHT )
//     rc.left -= m_nAddTextcross::RECTW;
//   else
//   if(Version == LG35_8_KF)
//     rc.right += 5;
//   else
//     if(!(align & DT_WORDBREAK))
//       rc.right += 5;
//   //
//   if (back != 0xff000000)
//   {
// 	  cross::RECT txtrc;
// 	  font.Calc(txtrc, text);

// 	  if ( align & DT_RIGHT )
// 	  {
// 		  txtrc.bottom = txtrc.bottom + rc.top;
// 		  txtrc.top = txtrc.top + rc.top;
// 		  txtrc.left =  rc.right - txtrc.right;
// 		  txtrc.right = txtrc.left + txtrc.right;
// 	  }
// 	  else
// 	  {
// 		  txtrc.bottom = txtrc.bottom + rc.top;
// 		  txtrc.top = txtrc.top + rc.top;
// 		  txtrc.left = txtrc.left + rc.left;
// 		  txtrc.right = txtrc.right + rc.left;
// 	  }
// 	  HBRUSH brush = ::CreateSolidBrush(back);
// 	  ::Fillcross::RECT(hDC, &txtrc, brush);
// 	  ::DeleteObject(brush);
//   }

//   if (Version ==DOTF_PM)
//   {//С‚РµРєСЃС‚С‹ Р·Р°РїРѕР»РЅСЏРµРјС‹Рµ СЃСЂРёРїС‚Р°РјРё РјРѕРіСѓС‚ РЅРµ РІР»РµР·Р°С‚СЊ - РЅР° СЃРєСЂРёРЅС€РѕС‚Р°С… РѕР±СЂРµР·Р°СЋС‚
//     if(!bTop && strchr(text, '\n')==0)
//     {
//      cross::RECT cross::RECT;
//      char tmptext[256];
//      strcpy_s(tmptext, text);
//      for(int ii=0; ii<15; ii++)
//       {
//       font.Calc( cross::RECT, tmptext );
//       if((cross::RECT.right-cross::RECT.left) > (rc.right-rc.left))
//         {
//         size_t len = strlen(tmptext);
//         if(len<1)
//           break;
//         tmptext[len-1] = 0;
//         }
//       }
//      ::DrawText( hDC, tmptext, lstrlen(tmptext), &rc, align|DT_NOPREFIX );
//      return;
//     }
//   }
// #ifdef _DEBUG
//   // if(strcmp(text, "PIA0009") == 0)
//      //  KKK();
// #endif
//   ::DrawText( hDC, text, lstrlen(text), &rc, align|DT_NOPREFIX );
  }


void CStyle::TextOutW( HDC hDC, COLORREF color, cross::RECT rc, const wchar_t* text, bool bTop, COLORREF back )
  {
  // CMyFont font( hDC, m_hFont, color );
  // if ( m_nAddTextcross::RECT < 0 )
  //   {
  //   m_nAddTextcross::RECT = 0;
  //   cross::RECT cross::RECT;
  //   //
  //   ::Setcross::RECTEmpty( &cross::RECT );
  //   if ( *text )
  //     ::DrawTextW( hDC, text, (int)wcslen(text), &cross::RECT, DT_SINGLELINE|DT_CALCcross::RECT );
  //   //
  //   int w = cross::RECT.right - cross::RECT.left;
  //   int W = rc  .right - rc  .left;
  //   //
  //   const wchar_t* a = text;
  //   while ( *a )
  //     {
  //     if ( *a == L'\n' )
  //       {
  //       align |= DT_WORDBREAK;
  //       break;
  //       }
  //     a++;
  //     }
  //   //
  //   if ( W > w && (align & DT_WORDBREAK)==0 )
  //     align |= DT_SINGLELINE | (bTop ? DT_TOP : DT_VCENTER);
  //   else
  //     align |= DT_WORDBREAK;
  //   int h = cross::RECT.bottom - cross::RECT.top;
  //   int H = rc  .bottom - rc  .top;
  //   if ( h > H ) m_nAddTextcross::RECT = (h-H);
  //   }
  // rc.bottom += m_nAddTextcross::RECT;
  // //
  // if ( align & DT_CENTER )
  //   rc.left -= 2,rc.right += 2;
  // else
  // if ( align & DT_RIGHT )
  //   rc.left -= 5;
  // else
  //   if(!(align & DT_WORDBREAK))
  //     rc.right += 5;
  // //
  // if (back != 0xff000000)
  // {
     //  cross::RECT txtrc;
     //  //font.Calc(txtrc, text);
  //     ::Setcross::RECTEmpty( &txtrc );
  //     if ( *text )
  //       ::DrawTextW( hDC, text, (int)wcslen(text), &txtrc, DT_SINGLELINE|DT_CALCcross::RECT );
  //     //
     //  if ( align & DT_RIGHT )
     //  {
        //   txtrc.bottom = txtrc.bottom + rc.top;
        //   txtrc.top = txtrc.top + rc.top;
        //   txtrc.left =  rc.right - txtrc.right;
        //   txtrc.right = txtrc.left + txtrc.right;
     //  }
     //  else
     //  {
        //   txtrc.bottom = txtrc.bottom + rc.top;
        //   txtrc.top = txtrc.top + rc.top;
        //   txtrc.left = txtrc.left + rc.left;
        //   txtrc.right = txtrc.right + rc.left;
     //  }
     //  HBRUSH brush = ::CreateSolidBrush(back);
     //  ::Fillcross::RECT(hDC, &txtrc, brush);
     //  ::DeleteObject(brush);
  // }

  // ::DrawTextW( hDC, text, (int)wcslen(text), &rc, align|DT_NOPREFIX );
}

//////////////////////////////////////////// СЃС‚РёР»Рё РёР· css-С„Р°Р№Р»РѕРІ ///////////////////////////////////////////////////////////
struct CSS_STYLE
{
	int line_style;
	int line_width;
    int roundness; 
    DWORD line_color;
	DWORD fill_color;
	DWORD text_color;
    DWORD fill_level_color;
    EVisibility visible;

    char ImageExt[8];
    std::map<std::string, std::string> otherparams;

    CSS_STYLE() { memset(this, -1, ((char*)&visible - (char*)this) + sizeof visible); ImageExt[0]=0;}
};
//
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
//#include "../Function.h"

wchar_t* SkipSpaceW( wchar_t* ptr )
{
    while(*ptr == L' ' || *ptr == L'\t') 
         ptr++;
    return ptr;
}

wchar_t* SkipSpaceFW( wchar_t* ptr )
{
    while(*ptr == L' ' || *ptr == L'\t' || *ptr == L'\xD' || *ptr == L'\xA') 
         ptr++;
    return ptr;
}

static std::map<std::string, CSS_STYLE> cssSlyles;
std::string pathforimage;

void CStyle::LoadcssSlyles(wchar_t *name)
{
    //std::wstring szbasePath = string_to_wstring( ::GetBasePath() );
    //std::wstring path = szbasePath + L"Abstract/CSS/";
    //path += name;
    //std::string szbasePath(::GetBasePath());
    std::string szbasePath;
    std::string path = szbasePath + "Abstract/CSS/";
    if(Version == VSB_KF)
      path = szbasePath + "Abstract/Displays/CSS/";
    pathforimage = szbasePath + "Memory/Bin/CSSImages";
    CreateDicross::RECTory(pathforimage.c_str(), NULL);
    path += wstring_to_string(name);
    //wchar_t* szBuffer = (wchar_t*)::FileToBuff( path.c_str() );

#ifdef _DEBUG
    if(wcscmp(name, L"RegValves.css")==0)
      AAA();            //.PlgIn_RegValve_Open
#endif

    std::ifstream fcss(path.c_str(), std::ios::binary);
    if(fcss.is_open())
    {
     std::streampos begin, end;
     begin = fcss.tellg();
     fcss.seekg(0, std::ios::end);
     end = fcss.tellg();
     fcss.seekg(0, std::ios::beg);    
     std::streamsize lengh = end;
     //std::auto_ptr<char> Buffer = new char[lengh];
     char* Buffer = new char[lengh+4];
     fcss.read(Buffer, lengh);
     wchar_t *pp1 = (wchar_t*)(Buffer+2);
     pp1[lengh/2-1] = 0;
     wchar_t *pp2 = wcsstr(pp1, L"\xD\xA");
     std::vector<std::wstring> names;
     while (pp2)
     {          
          if(pp1[0] == L'.')
          {
              wchar_t str[1024];
              if(pp2 < pp1)
                pp2 = wcsstr(pp1, L"\xD\xA");
              
              wchar_t *bracket = wcschr(pp1, L'{');
              wchar_t *endstr = wcsstr(pp1, L"\xD\xA");
              //wchar_t *comma = wcschr(pp1, L",");
              if(bracket && endstr)
                if(bracket < endstr)
                {//РґР°РЅРЅС‹Рµ РІ С‚РѕР№Р¶Рµ СЃС‚СЂРѕРєРµ
                   *endstr = 0;
                   *bracket = 0;
                   for(wchar_t *ppp = bracket-1; *ppp == L' '; ppp--)
                       *ppp = 0; 
                   wchar_t *pImage = wcsstr(bracket+1, L"background-image:url(data:image/");
                   if(pImage)
                   {
                   wchar_t *pImageType = pImage + _countof(L"background-image:url(data:image/")-1;
                   wchar_t *pp3 = wcsstr(pImageType, L";base64,");
                   if(pp3)
                     {
                     *pp3 = 0;
                     pImage = pp3 + _countof(L";base64,")-1;
                     wchar_t *pEndImage = wcschr(pImage, L')');
                     if(pEndImage && pEndImage < pp2)
                       {
                       std::string data =  b64decode(wstring_to_string(pImage));
                       wchar_t fname[MAX_PATH];
                       //Р·Р°РїРёСЃС‹РІР°СЋ С„Р°Р№Р»С‹ РєР°СЂС‚РёРЅРѕРє
                       wchar_t *styleName = pp1 + 1;
                       swprintf_s(fname, L"%s/%s.%s", string_to_wstring(pathforimage).c_str(), styleName, pImageType);
                       std::ofstream fimg(fname, std::ios::binary);
                       if(fimg.is_open())
                        {
                         fimg.write(data.c_str(), data.size());     
                         std::string sname = string_to_upper(wstring_to_string(styleName));
                         std::map<std::string, CSS_STYLE>::iterator tt2 = cssSlyles.find(sname);
                         if(tt2 == cssSlyles.end())
                         {
                           CSS_STYLE cssStyle;
                           strcpy_s(cssStyle.ImageExt, wstring_to_string(pImageType).c_str());
                           cssSlyles[sname] =  cssStyle;
                         }
                         else strcpy_s(tt2->second.ImageExt, wstring_to_string(pImageType).c_str());
                        }
                       }
                     }
                   pp1 = endstr +2;
                   pp2 = wcsstr(pp1, L"\xD\xA");
                   continue;
                   }
                }
              memcpy(str, pp1, min(sizeof str - 2, 2*(pp2 - pp1)));
              str[min(_countof(str) - 1, (pp2 - pp1))] = 0;
              if(wcschr(str, ',') == NULL)
              {    
                  wchar_t *psp = wcschr(str, L' ');
                  if(psp != NULL)
                     *psp = 0;
                  psp = wcschr(str, L'\t');
                  if(psp != NULL)
                     *psp = 0;
#ifdef _DEBUG
                  if(wcscmp(&str[1], L"TxtTagname_TxtEUdesc")==0)
                    AAA();
#endif
                  names.push_back(&str[1]);
                  pp1 = pp2 + 2;
                  pp2 = wcsstr(pp1, L"\xD\xA");
              } 
              else
              { //СЃРїРёСЃРѕРє РёРјРµРЅ
                  wchar_t *pp3 = wcschr(pp1, L'{');
                  if(pp3 == NULL)
                    break;
                  *(pp3-1) = 0;
                  do
                  {
                      wchar_t name[256];
                      wchar_t *pp4 = wcschr(pp1, L',');
                      if(pp4)
                      {
                       *pp4 = 0;
                       wcscpy_s(name, pp1+1);
                       names.push_back(name);
                       pp1 = SkipSpaceFW(pp4+1);
                      }
                      else
                      {
                         pp1 = SkipSpaceFW(pp1);
                         pp4 = wcschr(pp1, L'\xD');
                         if(pp4)
                           *pp4 = 0;
                         names.push_back(pp1+1);
                         break;
                      }

                  }
                  while (true);
   
                  pp1 = pp3;
                  pp2 = wcsstr(pp1, L"\xD\xA");
              }
          }
          else
          if(pp1[0] == L'{')
          {
            pp2 = wcsstr(pp1, L"\xD\xA");
            if(pp2 == NULL)
             break;
                                
            CSS_STYLE cssStyle; 
            bool bfilled =  false;
            while(pp2[0] != L'}')
            {
                pp1 = pp2 + 2;   
                pp2 = SkipSpaceFW( pp2 );
                if(wcsncmp(pp2, L"hw-fill-color:", _countof(L"hw-fill-color:")-1)==0)
                {
                    pp2 += _countof(L"hw-fill-color:")-1;
                    wchar_t *pp3 = wcschr(pp2, L';');
                    if(pp3)
                       *pp3 = 0;
                    cssStyle.fill_color =  ColorDiez(wstring_to_string(pp2).c_str());
                    bfilled = true;
                    pp2 = wcsstr(pp3+1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
                else
                if(wcsncmp(pp2, L"hw-roundness:", _countof(L"hw-roundness:")-1)==0)
                {
                    pp2 += _countof(L"hw-roundness:")-1;
                    wchar_t *pp3 = wcschr(pp2, L';');
                    if(pp3)
                       *pp3 = 0;
                    cssStyle.roundness =  _wtoi(pp2);
                    bfilled = true;
                    pp2 = wcsstr(pp3+1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
                else
                if(wcsncmp(pp2, L"hw-line-color:", _countof(L"hw-line-color:")-1)==0)
                {
                    pp2 += _countof(L"hw-line-color:")-1;
                    wchar_t *pp3 = wcschr(pp2, L';');
                    if(pp3)
                       *pp3 = 0;
                    cssStyle.line_color =  ColorDiez(wstring_to_string(pp2).c_str());
                    bfilled = true;
                    pp2 = wcsstr(pp3+1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
                else
                if(wcsncmp(pp2, L"hw-text-color:", _countof(L"hw-text-color:")-1)==0)
                {
                    pp2 += _countof(L"hw-text-color:")-1;
                    wchar_t *pp3 = wcschr(pp2, L';');
                    if(pp3)
                       *pp3 = 0;
                    cssStyle.text_color =  ColorDiez(wstring_to_string(pp2).c_str());
                    bfilled = true;
                    pp2 = wcsstr(pp3+1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
                else
                if(wcsncmp(pp2, L"hw-fill-level-color:", _countof(L"hw-fill-level-color:")-1)==0)
                {
                    pp2 += _countof(L"hw-fill-level-color:")-1;
                    wchar_t *pp3 = wcschr(pp2, L';');
                    if(pp3)
                       *pp3 = 0;
                    cssStyle.fill_level_color =  ColorDiez(wstring_to_string(pp2).c_str());
                    bfilled = true;
                    pp2 = wcsstr(pp3+1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
                else
                if(wcsncmp(pp2, L"hw-visibility:", _countof(L"hw-visibility:")-1)==0)
                {
                    pp2 += _countof(L"hw-visibility:")-1;
                    wchar_t *pp3 = wcschr(pp2, L';');
                    if(pp3)
                       *pp3 = 0;
                    cssStyle.visible =  Visible((char*)wstring_to_string(pp2).c_str());
                    bfilled = true;
                    pp2 = wcsstr(pp3+1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
                else
                if(pp1[0] != L'}')
                {//С‡С‚Рѕ С‚Рѕ РЅРµРѕР±СЂР°Р±Р°С‚С‹РІР°РµРјРѕРµ РІ С„РёРіСѓСЂРЅС‹С… СЃРєРѕР±РєР°С…
                    wchar_t *pp3 = wcschr(pp2, L':');
                    wchar_t *pp4 = wcschr(pp2, L';');
                    if(pp3 && pp4 && pp3<pp4)
                     {
                        *pp3 = 0;
                        *pp4 = 0;
                        bfilled = true;
                        cssStyle.otherparams[wstring_to_string(pp2)] = wstring_to_string(pp3+1);
                        *pp3 = ':';
                        *pp4 = ';';
                     }
                    pp2 = wcsstr(pp1, L"\xD\xA");
                    if(pp2 == NULL)
                      break;
                    pp1 = pp2 + 2;   
                }
            }
            if(bfilled)
            for(std::vector<std::wstring>::iterator tt = names.begin(); tt != names.end(); tt++) 
            {
               std::string sname = string_to_upper(wstring_to_string(*tt));
               std::map<std::string, CSS_STYLE>::iterator tt2 = cssSlyles.find(sname);
               if(tt2 == cssSlyles.end())
                 cssSlyles[sname] =  cssStyle;
               else{//СѓР¶Рµ РµСЃС‚СЊ - РґРѕР±Р°РІР»СЏРµРј
                   if(cssStyle.line_style != -1)
                      tt2->second.line_style = cssStyle.line_style;
                   if(cssStyle.line_width != -1)
                      tt2->second.line_width = cssStyle.line_width;
                   if(cssStyle.roundness != -1)
                      tt2->second.roundness = cssStyle.roundness;
                   if(cssStyle.line_color != -1)
                      tt2->second.line_color = cssStyle.line_color;
                   if(cssStyle.fill_color != -1)
                      tt2->second.fill_color = cssStyle.fill_color;
                   if(cssStyle.text_color != -1)
                      tt2->second.text_color = cssStyle.text_color;
                   if(cssStyle.fill_level_color != -1)
                      tt2->second.fill_level_color = cssStyle.fill_level_color;
                   if(cssStyle.visible != -1)
                      tt2->second.visible = cssStyle.visible;
                   for(std::map<std::string, std::string>::iterator tt3 = cssStyle.otherparams.begin(); tt3!=cssStyle.otherparams.end(); tt3++)
                      tt2->second.otherparams[tt3->first] = tt3->second;
                   AAA();
                   }
            }
            names.clear();
            pp2 = wcsstr(pp1, L"\xD\xA");
            if(pp2 == NULL)
               break;
            pp1 = pp2 + 2;   
          }
          else
          if(wcsncmp(pp1, L"@import", _countof(L"@import")-1) == 0)
          {
            wchar_t *pp3 = SkipSpaceW(pp1 + _countof(L"@import")-1);
            if(wcsncmp(pp3, L"url(\".\\", _countof( L"url(\".\\")-1) == 0) //С‚РѕР»СЊРєРѕ СЌС‚Рё
               {
                   pp3 += _countof( L"url(\".\\")-1;
                   wchar_t *pp4 = wcschr(pp3, L'"');
                   if(pp4)
                   {
                     *pp4 = 0;
                     LoadcssSlyles(pp3);
                     *pp4 = L'"';
                   }
               }
            else
            if(wcsncmp(pp3, L"url(\"./", _countof( L"url(\"./")-1) == 0) //С‚РѕР»СЊРєРѕ СЌС‚Рё
               {
                   pp3 += _countof( L"url(\"./")-1;
                   wchar_t *pp4 = wcschr(pp3, L'"');
                   if(pp4)
                   {
                     *pp4 = 0;
                     LoadcssSlyles(pp3);
                     *pp4 = L'"';
                   }
               }

            pp2 = wcsstr(pp1, L"\xD\xA");
            if(pp2 == NULL)
             break;
            pp1 = pp2 + 2;
          }
          else 
          {
            pp2 = wcsstr(pp1, L"\xD\xA");
            if(pp2 == NULL)
             break;
            pp1 = pp2 + 2;
          }
     }
     AAA();
     delete []Buffer;      
    }
  //::BuffFree( szBuffer );

}

bool CStyle::GetCSSStyle()
{
     std::map<std::string, CSS_STYLE>::iterator tt = cssSlyles.find(string_to_upper(m_sStyleClass));
     if(tt != cssSlyles.end())
     {
         CSS_STYLE &cssStyle = tt->second;
         //if(cssStyle.line_style != -1)
         //  m_nLineStyle = cssStyle.line_style;
         if(cssStyle.line_width != -1)
           m_nLineWidth = cssStyle.line_width;
         if(cssStyle.roundness != -1)
         {
            cross::RECT rc = FullRect();
            int w = rc.right-rc.left;
            int h = rc.bottom-rc.top;
            int m = min(w,h);
            if(m>0)
             m_nRoundness = m*cssStyle.roundness/100; //С‚.Рє РёСЃС…РѕРґРЅРѕРµ Р·РЅР°С‡РµРЅРёРµ РІ %
         }
         if(cssStyle.line_color != -1)
         {
            m_clrLine = cssStyle.line_color;
            m_dwFlags |= HDX_CLRLINE;
         }
         if(cssStyle.fill_color != -1)
         {
            m_clrFill = cssStyle.fill_color;
            if( m_clrFill != 0xff000000 )
            {
            m_dwFlags |= HDX_CLRFILL;
            m_bClrFillCanChangedByScript = true;
            }
         }
         if(cssStyle.text_color != -1)
         {
            m_clrText  = cssStyle.text_color;
            m_dwFlags |= HDX_CLRTEXT;
         }
         if(cssStyle.fill_level_color != -1)
         {
            m_clrFillLvl = cssStyle.fill_level_color;
            m_dwFlags |= HDX_CLRFILLL;
         }
         if(cssStyle.visible != -1)
            m_eVisible = cssStyle.visible;
         return true;
     }
     return false;
}

std::string CStyle::GetCSSStyleImage()
{
     std::map<std::string, CSS_STYLE>::iterator tt = cssSlyles.find(string_to_upper(m_sStyleClass));
     if(tt != cssSlyles.end() && tt->second.ImageExt[0])
     {
         return pathforimage + "/" + m_sStyleClass + "." + tt->second.ImageExt;
     }
     return "";
}

std::string CStyle::FindCSSSyileParam( std::string styleName, std::string paramName)
{
  std::map<std::string, CSS_STYLE>::iterator tt = cssSlyles.find(string_to_upper(styleName));
  if(tt != cssSlyles.end())
  {
      std::map<std::string, std::string>::iterator tt2 = tt->second.otherparams.find(paramName);
      if(tt2 != tt->second.otherparams.end())
          return tt2->second;
  }
  return "";
}
