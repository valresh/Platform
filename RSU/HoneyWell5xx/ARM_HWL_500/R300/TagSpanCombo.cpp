#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagSpanCombo.h"

CTagSpanCombo::CTagSpanCombo(int n)
  {
  mParam = n;
  }
//
void CTagSpanCombo::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }
//
void CTagSpanCombo::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  switch ( mParam )
    {
    case 0:// Список имён групп
      m_pWnd = new CHTML_CboName(html);
    return;
    case 1:// Вид окна трендов
      m_pWnd = new CHTML_CboView(html);
    return;
    }
  }
//
