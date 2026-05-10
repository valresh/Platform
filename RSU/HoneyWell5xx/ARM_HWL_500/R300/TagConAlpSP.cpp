#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConAlpSP.h"
#include "../Extensions.h"

CTagConAlpSP::CTagConAlpSP(int n): m_nSelected(0)
  {
  }
//
void CTagConAlpSP::Analiz( ANALIZ )
  {
  AnalizTextArea( this, html, teg );
  }
//
void CTagConAlpSP::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsAlphaNum );
  if ( AS( mVal.def != NULL ) ) return; 
  ASSERT( mVal.def->eVal == enumValueDbl );
  ASSERT( lstrcmpi( mVal.def->name, "SP" ) == 0 );
  //
  const char * name = html.pipe.GetMnemoName(mVal.nNumber);
  CShBase* sh = html.pipe.GetBase(mVal.nNumber);
  if ( sh != NULL )
   {
    if (Version==LG35_8_KF || style.m_bParameterFormat)
    {
    SValueDef* def = ::NameToValueEx( EDataTypes(mVal.def->dwLog), "PVFORMAT" );
    if ( def != NULL )
      {   //MIHAIL они сказали, для всех SP у них 3 знака
		  if (Version!=AT_PRP)
		  {
			  ASSERT( def->eVal == enumValueChr );
			  double d = def->D(sh);
			  char format[32];
			  sprintf_s( format, sizeof(format), "%%.%df", int(d) );
			  style.m_nFiltrOfChars = style.AddTexts( html, format );
		  }
      }
     }
    }
  m_pWnd = new CHTML_SP( html, style.Filtr(html) );
  }
//
void CTagConAlpSP::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  if ( style.m_eVisible == enumHidden )
    return;
  //
  RECT rect = style.OutsRect();
  if ( style.m_nFillStyle != -1 )
    style.Rect( hDC, html, rect );
  //
  char txt[512] = "";
  bool bPaint = FillText( html, txt );
  if ( !*txt ) return;
  //
  if ( bPaint )
  if ( rect.left != rect.right )
  if ( rect.top != rect.bottom )
  if ( style.m_clrText != 0xff000000 )
     {
	  style.TextOut( hDC, m_nSelected ? 0x00ffffff : style.m_clrText, rect, txt, true, m_nSelected ? 0x00D77800 : 0xff000000 );
      CHTMLEditBox *pEditBoxWnd  = dynamic_cast<CHTMLEditBox*>(m_pWnd);
      if(pEditBoxWnd && !pEditBoxWnd->m_bEdited)
        ::SetWindowText(pEditBoxWnd->m_hWnd, txt );
     }
  }
//
void CTagConAlpSP::Select()
{
  this->m_nSelected = 1;
}
void CTagConAlpSP::DeSelect()
{
  this->m_nSelected = 0;
}
//
