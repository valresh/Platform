#include "StdAfx.h"
#include "HTML.h"
#include "TagA.h"
#include "Style.h"

#undef  STD_VAR
#define STD_VAR CHTML& html, CTagA& a, CStyle& style, char* value
typedef void (*LPFuncs)( STD_VAR );

void aClass( STD_VAR )
  {
    if(_strnicmp(value, "hsc.hyperlink", _countof("hsc.hyperlink")-1)==0)
       a.m_ClickType = ctMnemo;
  }
//
void aStyle( STD_VAR )
  {
  a.AnalizStyle( html, value );
  }
//
void aID( STD_VAR )
  {
  style.NameID( html, value );
  }
//
void aTabIndex( STD_VAR )
  {
  style.nTabIndex = atoi(value);
  }
//
void aHref( STD_VAR )
  {
  a.href = value;
  }
//
CTagA::CTagA()
{
}

void CTagA::Analiz( ANALIZ )
  {
   struct SLocal {  char* name; LPFuncs func; };
  static SLocal list[] =
    {
    { "class"             , aClass             },
    { "style"             , aStyle             },
    { "id"                , aID                },
    { "href"              , aHref              },
    }InitLocal;
  //
  CStyle& style = Style(html);
  int n = 0;
  while ( teg[n].name )
    {
    SLocal* find = FindLocal( teg[n].name );
    ASSERT( find );
    if ( find != NULL )
    (*find->func)(html,*this,style,teg[n].value);
    n++;
    }
  }
//
void CTagA::Kerdyk( KERDYK )
  {
  m_constValue = GetValue(html);
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  char* id = style.NameID(html);
#ifdef _DEBUG
  if ( _stricmp( id, "hyperlink001" ) == 0)
    AAA();
#endif

  style.GetCSSStyle();

  style.CreateFont( html, this, style.m_fntStyle );
}

void CTagA::Matrix( LOCALS )
{
  CStyle& style = Style(html);
  if ( style.LocalD( hDC ) )
      return;
  char* id = style.NameID( html );

  style.GetCSSStyle();
  //
#ifdef _DEBUG
#endif
  //
  style.GetCSSStyle();
  //
  if ( style.m_eVisible == enumHidden )
  {
     return;
  }
  //
  cross::RECT cross::RECT = style.Fullcross::RECT();
  //
  //
  int nClass = StyleClass( hDC, html );
  //
#define NOFILLTEXT  "??????????"
  char str[512] = NOFILLTEXT;
  if ((mVal.nNumber == -1) && *m_szScriptValue )
      {
      lstrcpy( str, m_szScriptValue );
      }
  else
      lstrcpy( str, m_constValue );
  //
  if ( !str[0] )
      return;
  //
  COLORREF clrText=style.m_clrText;
  cross::RECT = style.Outscross::RECT();
  if ( cross::RECT.left != cross::RECT.right )
  if ( cross::RECT.top != cross::RECT.bottom )
  if ( style.m_clrText != 0xff000000 )
  {
      bool bDrawText=true;
      if (bDrawText)
        style.TextOut( hDC, clrText, cross::RECT, str, true );
  }
  //
}
//
void CTagA::Action( CHTML& html, HWND hWnd, bool bDown, CPoint* pt )
{
    if(!bDown)
    {
      if(_strnicmp(href.c_str(), "PAGE://", _countof("PAGE://")-1)==0)
      {
        std::string mnemoname = href.substr(_countof("PAGE://")-1);
        if(mnemoname[mnemoname.size()-1] == '/')
           mnemoname = mnemoname.substr(0,mnemoname.size()-1);
        if(mnemoname[mnemoname.size()-1] == ' ')
           mnemoname = mnemoname.substr(0,mnemoname.size()-1);

  //      NaviGo(html, hWnd, mnemoname.c_str());
      }
    }
}
