#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConAlpOP.h"

CTagConAlpOP::CTagConAlpOP(int n): m_nSelected(0)
  {
  }
//
void CTagConAlpOP::Analiz( ANALIZ )
  {
  AnalizTextArea( this, html, teg );
  }
//
void CTagConAlpOP::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsAlphaNum );
  if ( AS( mVal.def != NULL ) )
    return;
  //
  ASSERT( mVal.def->eVal == enumValueDbl );
  ASSERT( lstrcmpi( mVal.def->name, "OP" ) == 0 );
  //
  m_pWnd = new CHTML_OP( html, style.Filtr(html) );
  }
//
void CTagConAlpOP::Matrix( LOCALS )
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
	  style.TextOut( hDC, m_nSelected ? 0x00ffffff : style.m_clrText, rect, txt, true, m_nSelected ? 0x00D77800 : 0xff000000 );
  }
//
void CTagConAlpOP::Select()
{
  this->m_nSelected = 1;
}
void CTagConAlpOP::DeSelect()
{
  this->m_nSelected = 0;
}
