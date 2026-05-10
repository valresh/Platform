#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "../ViewChart.h"
#include "HTML.h"
//
CHTML_Chart::~CHTML_Chart()
  {
  ::DestroyWindow( m_hWnd );
  }
//
bool CHTML_Chart::WindowPos( WND_POS )
  {
  bool bInit = ( !::IsWindow( m_hWnd ) );
  if ( wnd == NULL )
    {
    wnd = new CFrameChart( html.Group() );
    DWORD dwStyle = WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    if ( wnd->CreateEx( 0, NULL, "Структура", dwStyle, 0,0,0,0, hParent, 0 ) )
      m_hWnd = wnd->m_hWnd;
    }
  //
  if ( wnd != NULL )
    {
    ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height, SWP_NOZORDER );
    }
  //
  if ( bInit )
    {
    }
  //
  return true;
  }
//
void CHTML_Chart::Timer()
  {
  //if ( !::IsWindow( m_hWnd ) ) return;
  //if ( !::IsWindowVisible( m_hWnd ) ) return;
  //wnd->MainTimer(dwMainTimer);
  }
//
//***************************************************************************************
