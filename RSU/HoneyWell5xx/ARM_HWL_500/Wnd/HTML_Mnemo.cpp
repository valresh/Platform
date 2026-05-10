#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "../ViewMnemo.h"
//
CHTML_Mnemo::~CHTML_Mnemo()
  {
  if ( wnd ) delete wnd;
  }
//
bool CHTML_Mnemo::WindowPos( WND_POS )
  {
  bool bInit = ( !::IsWindow( m_hWnd ) );
  if ( wnd == NULL )
    {
    wnd = new CViewMnemo(Client(),true);
    if ( wnd->CreateEx( 0, NULL, "Faceplate", WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,0,0,0, hParent, 0 ) )
      {
      m_hWnd = wnd->m_hWnd;
      wnd->Zoom(-1);
      }
    }
  //
  if ( wnd != NULL )
    ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height, SWP_NOZORDER );
  //
  if ( bInit )
    wnd->FacePlate( html, nNumber );
  else
    wnd->SetSizes();
  return true;
  }
//
void CHTML_Mnemo::Timer()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  if ( !::IsWindowVisible( m_hWnd ) ) return;
  wnd->MainTimer(dwMainTimer);
  }
//
//***************************************************************************************
