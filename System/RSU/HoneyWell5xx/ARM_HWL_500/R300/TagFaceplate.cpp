#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagFaceplate.h"

CTagFaceplate::CTagFaceplate(int n)
  {
  mParam = n;
  }
//
void CTagFaceplate::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }
//
void CTagFaceplate::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  // Faceplate для групп
  if ( 0 <= mParam && mParam < 8 )
    mVal = ::GrupExperion( html, mParam );
  else // Faceplate на детальных окнах
    mVal = ::FindExperion( html, html.Group(), "NAME" );
  //
  if ( mVal.nNumber >= 0 )
    m_pWnd = new CHTML_Mnemo( html );
  //
  }
//
