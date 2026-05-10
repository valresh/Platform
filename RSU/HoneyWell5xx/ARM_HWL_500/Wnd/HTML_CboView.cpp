#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
//
#include "HTMLWndBase.h"
//
bool CHTML_CboView::WindowPos( WND_POS )
  {
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if ( !CHTMLComboBox::WindowPos( hParent, nNumber, hFont, fScale, rc ) )
    return false;
  //
  if ( bAddString )
    {
    ::SendMessage( m_hWnd, CB_ADDSTRING, 0, (LPARAM)"Group Detail"    );
    ::SendMessage( m_hWnd, CB_ADDSTRING, 0, (LPARAM)"Group Trend"     );
    ::SendMessage( m_hWnd, CB_ADDSTRING, 0, (LPARAM)"Numeric History" );
    m_nOldSelect = ReadInt( "GroupView", 0 ) % 3;
    ::SendMessage( m_hWnd, CB_SETCURSEL, m_nOldSelect, 0 );
    }
  return true;
  }
//
void CHTML_CboView::Timer()
  {
  }
//***************************************************************************************
LRESULT CHTML_CboView::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_COMMAND )
  if ( HIWORD(wParam) == CBN_SELCHANGE )
    {
    LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
    if ( nSel != CB_ERR && nSel != m_nOldSelect )
      {
      int n = int(nSel % 3);
      SaveInt( "GroupView", n );
      GetAppWnd()->ButtonOk( barGROUP, html.Group(), m_hWnd );
      return 0;
      }
    }
  return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
  }
