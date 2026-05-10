#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConAlpPV.h"
//
extern bool g_bGlobalEditPV;// Возможность редактирования PV
//-------------------------------------------------------------------
CTagConAlpPV::CTagConAlpPV(int n)
  {
  }
//
void CTagConAlpPV::Analiz( ANALIZ )
  {
  AnalizTextArea( this, html, teg );
  }
//
void CTagConAlpPV::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsReadOnlyR || style.mClass == clsReadOnlyL );
  if ( AS( mVal.def != NULL ) ) return; 
  ASSERT( mVal.def->eVal == enumValueDbl || mVal.def->eVal == enumValueChr );
  ASSERT( lstrcmpi( mVal.def->name, "PV" ) == 0 );
//  a.m_nFiltrOfChars = a.AddText( html, value, lstrlen(value)+1 );
  //
  if ( g_bGlobalEditPV )
    style.m_eVisible = enumHidden;
  else
    style.m_eVisible = enumVisible;
  }
//
void CTagConAlpPV::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  if ( style.m_eVisible == enumHidden )
    return;
  //
  RECT rect = style.OutsRect();
  //if ( style.m_nFillStyle != -1 )
  //  style.Rect( hDC, html, rect );
  //
  char txt[512] = "";
  bool bPaint = FillText( html, txt );
  if ( !*txt ) return;
  //
  if ( bPaint )
  if ( rect.left != rect.right )
  if ( rect.top != rect.bottom )
  if ( style.m_clrText != 0xff000000 )
  style.TextOut( hDC, style.m_clrText, rect, txt, true );
  }
//
