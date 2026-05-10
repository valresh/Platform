#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
//
#include "HTMLWndBase.h"
#include "../ViewTrend.h"
#include "../Trend3.h"
//
bool CHTML_CboName::WindowPos( WND_POS )
  {
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if ( !CHTMLComboBox::WindowPos( hParent, nNumber, hFont, fScale, rc ) )
    return false;
  //
  if ( bAddString )
    {
    int nGroup = atoi(html.Group());
    CDataTrends& mData = CViewTrend::Data();
    UINT nCount   = mData.Count();
    STrends* item = mData.Item(UINT(0));
    if ( item != NULL )
      {
      for ( UINT n = 0; n < nCount; n++ )
      if ( item[n].TrueGrup() )
        {
        const char* name = mData.Text(item[n].hTitle);
        LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)name );
                    ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)(n+1));
        if ( (n+1) == nGroup ) m_nOldSelect = int(N);
        }
      }
    ::SendMessage( m_hWnd, CB_SETCURSEL, m_nOldSelect, 0 );
    }
  return true;
  }
//
void CHTML_CboName::Timer()
  {
  }
//***************************************************************************************
LRESULT CHTML_CboName::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_COMMAND )
  if ( HIWORD(wParam) == CBN_SELCHANGE )
    {
    LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
    if ( nSel != CB_ERR && nSel != m_nOldSelect )
      {
      UINT nNew = (UINT)::SendMessage( m_hWnd, CB_GETITEMDATA, nSel, 0 );
      char szText[16];
      sprintf_s( szText, sizeof(szText), "%d", nNew );
      GetAppWnd()->ButtonOk( barGROUP, szText, m_hWnd );
      return 0;
      }
    }
  return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
  }
