#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagGrpState.h"

CTagGrpState::CTagGrpState(int n)
  {
  mParam = n;
  }
//
void CTagGrpState::Analiz( ANALIZ )
  {
  CTagDIV::Analiz( html, teg );
  }
//
void CTagGrpState::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  char param[64];
  sprintf_s( param, sizeof(param), "STATETEXT(%d)", mParam );
  mVal = FindExperion( html, html.m_strObj, param );
  //
  }
//
void CTagGrpState::PaintThis( LOCALS )
  {
  CStyle& style = Style(html);
  if ( style.m_eVisible == enumVisible )
    CTag::PaintThis( hDC, html );
  }
//
void CTagGrpState::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  //
  char* val = NULL;
  if ( mVal.nNumber >= 0 )
    val = html.pipe.ValueS_H(mVal);
  //
  if ( val )
    {
    while ( *val == ' ' ) val++;
    if ( !*val ) val = NULL;
    }
  //
  if (val && !stricmp(val, "Not Used") /*&& Version == LG35_8_KF*/)
  {
    style.m_eVisible = enumHidden;
	return;
  }
  if ( !val )
  if(BERTH_ONPZ!=Version || html.m_strFile.CollateNoCase("sysdtlSMdevctla_3SVLV_fp") != 0)	
    style.m_eVisible = (val == NULL) ? enumHidden : enumVisible;
  }
//
