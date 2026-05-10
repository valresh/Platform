#include "stdafx.h"
#include "../ARM.h"
#include "TexnoFrm.h"
#include "BaseType.h"
#include "CommProc.h"
#include "Queue.h"

IMPLEMENT_DYNAMIC(CTexnoFrm, CFrameWnd)
//
char* pszMiniKey = "MiniFrame";
char* pszMainKey = "Технологическая схема";
//
CTexnoFrm::CTexnoFrm(HWND& hTouchWnd,int nMonitor)
: m_Scheme(&m_Mini,hTouchWnd,nMonitor)
, m_Mini(&m_Scheme,hTouchWnd)
, m_hTouchWnd(hTouchWnd)
, m_bViewMini(false)
  {
  m_bViewMini = (bool)( GetInt(pszMainKey,pszMiniKey,m_bViewMini) != 0 );
  }

CTexnoFrm::~CTexnoFrm()
  {
  WriteInt(pszMainKey,pszMiniKey,m_bViewMini);
  }

BEGIN_MESSAGE_MAP(CTexnoFrm, CFrameWnd)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
  ON_WM_PAINT()
END_MESSAGE_MAP()

CString CTexnoFrm::String( const char* name )
  {
  return GetString( _T("Sheme"), m_Scheme.KeyName(name), "" );
  }

bool CTexnoFrm::Correlate(HWND hWnd,bool bEmpty)
  {
  char* szSxema = (char*)::SendMessage(hWnd,nBuildQueue,BUILD_SXEMA,0);
  if ( szSxema && *szSxema )
    {
    m_Scheme.LoadData(szSxema);
    return true;
    }
  return false;
  }

const char* CTexnoFrm::GoToSchem( HXML nItem )
  {
  return m_Scheme.LoadData( nItem );
  }

void CTexnoFrm::GoToSchem(LPCTSTR pszSchem)
  {
  m_Scheme.LoadData( pszSchem );
  }
//
void CTexnoFrm::ShowOrHide( int nCmdShow )
  {
  m_Scheme.ShowOrHide( nCmdShow );
  ShowWindow(nCmdShow);
  if ( nCmdShow != SW_HIDE )
    {
      SetSizeChild();
      m_Scheme.SetFocus();
    }
  else
    {
  	m_Mini.ShowWindow(nCmdShow);
    CTexChild::HideDialog();
    }
  }

void CTexnoFrm::OnPaint()
  {
  CPaintDC dc(this);
  if ( m_bViewMini )
    {
    RECT rect;
    GetClientRect(&rect);
    rect.top += m_Mini.SizeY();
    rect.bottom += rect.top + SplitterHight;
    ::FillRect( dc, &rect, GetSysColorBrush(COLOR_BTNFACE) );
    }
  }

void CTexnoFrm::SetSizeChild()
  {
  CRect rect;
  GetClientRect(rect);
  //
  if ( m_bViewMini )
    {
    int bottom = rect.bottom;
    rect.bottom = rect.top + m_Mini.SizeY();
    m_Mini.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER );

    rect.top = rect.bottom + SplitterHight;
    rect.bottom = bottom;
    }
  m_Mini.ShowWindow( m_bViewMini?SW_SHOW:SW_HIDE);
  m_Scheme.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER );
  }

void CTexnoFrm::OnSize(UINT nType, int cx, int cy)
  {
	CFrameWnd::OnSize(nType, cx, cy);
  if ( cx > 0 && cy > 0 )
  if ( ::IsWindow(m_Scheme) && ::IsWindow(m_Mini) )
    SetSizeChild();
  }

void CTexnoFrm::OnSetFocus(CWnd* pOldWnd)
  {
	if ( ::IsWindow( m_Scheme ) )
		m_Scheme.SetFocus();
  else
	__super::OnSetFocus(pOldWnd);
  }

void CTexnoFrm::ShowOrHideMini()
  {
  if ( !::IsWindow(m_hWnd) )
    return;
  m_bViewMini = !m_bViewMini;
  SetSizeChild();
  }

BOOL CTexnoFrm::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
  {
  // let the view have first crack at the command
  if (m_Scheme.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
    return TRUE;
  // otherwise, do default handling
  return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
  }

void CTexnoFrm::EventQueueMsg(WPARAM wParam,LPARAM lParam)
  {
  m_Scheme.OnEventQueueMsg(wParam,lParam);
  }
