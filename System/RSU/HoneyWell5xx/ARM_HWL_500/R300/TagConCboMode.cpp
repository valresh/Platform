#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConCboMode.h"

CTagConCboMode::CTagConCboMode(int n)
  {
  }
//
void CTagConCboMode::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }
//
void CTagConCboMode::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
//ASSERT( style.mClass == clsComboBox );
  if ( AS( mVal.def != NULL ) )
    return;
  //
  ASSERT( mVal.def->eVal == enumValueChr );
  ASSERT( lstrcmpi( mVal.def->name, "MODE" ) == 0 );
  //
  if ( style.m_eVisible == enumVisible )
    m_pWnd = new CHTML_ENUM( html, mVal );
  }
//
