#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConCboPVFL.h"

CTagConCboPVFL::CTagConCboPVFL(int n)
  {
  }
//
void CTagConCboPVFL::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }
//
void CTagConCboPVFL::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsComboBox );
  if ( AS( mVal.def != NULL ) ) return; 
  ASSERT( mVal.def->eVal == enumValueChr );
  //
  m_pWnd = new CHTML_PVOP( html, mVal );
  }
//
