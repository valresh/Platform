#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include <algorithm>
#include "..\\AplicFrm.h"
//

CHTML_ENUM_LIST::CHTML_ENUM_LIST(CHTML& _html, COMBO_LIST *pDataList) : CHTMLComboBox( _html )
{ 
    m_dListCount = 0;
    m_pDataList = pDataList;

    if(m_pDataList)
      {
       for(int ii=0; m_pDataList[ii].pshowItem; ii++)
          {
           m_dListCount = ii+1;
           SParamValueH val;
           if(m_pDataList[ii].pparam)
             val = FindExperion(html, html.m_strGroup.GetBuffer(), m_pDataList[ii].pparam);
           mVals.push_back(val);
          }
      }
}

bool CHTML_ENUM_LIST::WindowPos( WND_POS )
  {
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if ( !CHTMLComboBox::WindowPos( hParent, nNumber, hFont, fScale, rc ) )
    return false;

  if ( bAddString )
    {
       if(m_pDataList)
        {
         for(int ii=0;  m_pDataList[ii].pshowItem; ii++)
          {
           LRESULT N = OnMessage( CB_ADDSTRING, ii, (LPARAM)m_pDataList[ii].pshowItem );
           OnMessage( CB_SETITEMDATA, N, (LPARAM)ii);
          }
        }
    }
  return true;
}

LRESULT CHTML_ENUM_LIST::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_COMMAND )
  if ( HIWORD(wParam) == CBN_SELCHANGE )
    {
     LRESULT nSel = CHTMLComboBox::OnMessage( CB_GETCURSEL, 0, 0  );
     if( m_pDataList && (m_dListCount > nSel) && mVals[nSel].def && m_pDataList[nSel].pparam )
       {
	   char cc = html.pipe.ValueB_H(mVals[nSel]);
       SSendToModel send;
       lstrcpy( send.szValue, mVals[nSel].def->name );
       send.Set( enumValueChr, &cc, &m_pDataList[nSel].val2set );
       html.pipe.SendData( mVals[nSel].nNumber, send, true );
       }
    }
  return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
  }
