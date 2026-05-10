#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConCboOP.h"

CTagConCboOP::CTagConCboOP(int n)
  {
  }
//
void CTagConCboOP::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }
//
void CTagConCboOP::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  ASSERT( style.mClass == clsComboBox );
  if ( mVal.def == NULL )
  {
      if(html.m_strFile.CompareNoCase("sysdtlSMdevctla_3SVLV_fp")==0
         || html.m_strFile.CompareNoCase("sysdtlSMdevctla_PUMP_fp") == 0
        )
      {
          std::vector<std::string> strings;
          std::string CurrentPoint = html.Group().GetBuffer();
          strings.push_back(CurrentPoint+".{%BlkCMD5%}.PVFL");
          strings.push_back(CurrentPoint+".{%BlkCMD4%}.PVFL");
          strings.push_back(CurrentPoint+".{%BlkCMD6%}.PVFL");
          m_pWnd = new CHTML_PVOP_M( html, (const char*)m_szScriptValue, strings );
      }
      return; 
  }
  ASSERT( mVal.def->eVal == enumValueChr );
  ASSERT( lstrcmpi( mVal.def->name, "OP" ) == 0 );
  //
  m_pWnd = new CHTML_PVOP( html, mVal );
  }
//
