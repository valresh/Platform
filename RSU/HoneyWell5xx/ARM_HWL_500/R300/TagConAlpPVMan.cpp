#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConAlpPVMan.h"
//
extern bool g_bGlobalEditPV;// Возможность редактирования PV
//-------------------------------------------------------------------
CTagConAlpPVMan::CTagConAlpPVMan(int n)
  {
  }
//
void CTagConAlpPVMan::Analiz( ANALIZ )
  {
  AnalizTextArea( this, html, teg );
  }
//
void CTagConAlpPVMan::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsAlphaNum );
  if ( AS( mVal.def != NULL ) ) return; 
  ASSERT( mVal.def->eVal == enumValueDbl );
  ASSERT( lstrcmpi( mVal.def->name, "PV" ) == 0 );
  //
  if ( g_bGlobalEditPV )
    style.m_eVisible = enumVisible, m_pWnd = new CHTML_PV( html, style.Filtr(html) );
  else
    style.m_eVisible = enumHidden;
  }
//
void CTagConAlpPVMan::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  if ( style.m_eVisible == enumHidden )
    return;
  //
  char txt[512] = "";
  bool bPaint = FillText( html, txt );
  if ( !*txt ) return;
  //
  RECT rect = style.OutsRect();
  if ( bPaint )
  if ( rect.left != rect.right )
  if ( rect.top != rect.bottom )
  if ( style.m_clrText != 0xff000000 )
  style.TextOut( hDC, style.m_clrText, rect, txt, true );
  }
//
