#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConCboPVMan.h"
//
extern bool g_bGlobalEditPV;// Возможность редактирования PV
//-------------------------------------------------------------------
CTagConCboPVMan::CTagConCboPVMan(int n)
  {
  }
//
void CTagConCboPVMan::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }
//
void CTagConCboPVMan::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsComboBox );
  if ( AS( mVal.def != NULL ) ) return; 
  ASSERT( mVal.def->eVal == enumValueChr );
  ASSERT( lstrcmpi( mVal.def->name, "PV" ) == 0 );
  //
  if ( g_bGlobalEditPV )
    style.m_eVisible = enumVisible, m_pWnd = new CHTML_PVOP( html, mVal );
  else
    style.m_eVisible = enumHidden;
  }
//
