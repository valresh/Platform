#include "stdafx.h"
#include "../ARM.h"
#include "TexMini.h"
#include "TexChild.h"
//#include "ModelInfo.h"
#include "CommProc.h"
#include "Queue.h"

IMPLEMENT_DYNAMIC(CTexMini, CWnd)
CTexMini::CTexMini(CTexChild* pScheme,HWND& hTouchWnd)
: m_pScheme(pScheme)
, m_hTouchWnd(hTouchWnd)
, m_dScaleX(1.0)
, m_dScaleY(1.0)
, m_bPress(false)
, m_bPresent(false)
, m_hChild(NULL)
  { 
  }

CTexMini::~CTexMini()
  {
  }

BEGIN_MESSAGE_MAP(CTexMini, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
  ON_WM_SETCURSOR()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTexMini message handlers
int CTexMini::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  CModelInfo& model = ::ModelInfo();
  char  szPath[_MAX_PATH];
  char* szName = "Схема";
  sprintf_s( szPath, sizeof(szPath), "%s\\%s.scn", model.dirScript, szName);

  DWORD dwFileAttr = GetFileAttributes(szPath);
//if ( !::IsWindow(m_hTouchWnd) )
  if ( dwFileAttr != DWORD(-1) && dwFileAttr != FILE_ATTRIBUTE_DIRECTORY )
    {
    m_hChild = ::CreateWindow(CLASS_MNEMO, NULL, 
      AFX_WS_DEFAULT_VIEW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        0, 0, 0, 0, m_hWnd, NULL, NULL, NULL );
    ::SendMessage( m_hChild, nBuildQueue, BUILD_MNEMO, (LPARAM)szName );
    m_bPresent = ::IsWindow(m_hChild) == TRUE;
    }
  return 0;
  }

int CTexMini::SizeY()
  {
  RECT rect;
  if ( ::IsWindow(m_hChild) )
    {
    ::GetWindowRect( m_hChild, &rect );
    return (rect.bottom-rect.top);
    }
  // Нормируем на разрешение 1024/768
  rect = WorkRect( m_hWnd );
  return MulDiv( 90, (rect.bottom-rect.top), 768 );
  }

void CTexMini::OnSize(UINT nType, int cx, int cy)
  {
	CWnd::OnSize(nType, cx, cy);
  if ( ::IsWindow(m_hChild) )
    ::SetMessageWnd(m_hChild,m_pScheme->m_hWnd);
  if ( ::IsWindow( m_pScheme->m_hWnd ) )
    BuildBmp();
  }

void CTexMini::BuildImg(CDC& dc, CRect& rect)
  {
  CDC cdc;
  if ( cdc.CreateCompatibleDC( &dc ) )
    {
    CRect rcMask;
    m_pScheme->GetRect(&rcMask);
    rcMask.top = (long)((double)(rcMask.top)*m_dScaleY);
    rcMask.left = (long)((double)(rcMask.left)*m_dScaleX);
    rcMask.right = (long)((double)(rcMask.right)*m_dScaleX+0.5);
    rcMask.bottom = (long)((double)(rcMask.bottom)*m_dScaleY+0.5);
    rcMask.OffsetRect(m_rectPict.TopLeft());
    //
    CRect rcRama = rcMask;
    if ( rcMask.Width() < 30 )
      {
      CPoint pt = rcMask.CenterPoint();
      rcRama = CRect(pt,pt);
      rcRama.InflateRect( 15, 10 );
      }
    //
    CBitmap* pOld = cdc.SelectObject( &m_bmpWnd );
    dc.BitBlt( 0,0, rect.Width(), rect.Height(), &cdc, 0,0,SRCCOPY);
    //
    cdc.SelectObject( &m_bmpMsk );
    static BLENDFUNCTION BF = {AC_SRC_OVER,0,0xAF,0x00};
    dc.AlphaBlend( rcRama.left,rcRama.top,rcRama.Width(),rcRama.Height(),
                   &cdc, 0, 0,
                   min(rcRama.Width (),m_rectPict.Width()),
                   min(rcRama.Height(),m_rectPict.Height()),BF);
    //
    if(rcMask != rcRama)
      {
      CRgn rg;
      rg.CreateRectRgnIndirect(rcRama);
      ::FrameRgn(dc,rg,CBrush(RGB(255,255,255)),3,3);
      }
    //
    CRgn rgn;
    rgn.CreateRectRgnIndirect(rcMask);
    ::FrameRgn(dc,rgn,CBrush(RGB(255,0,0)),3,3);  
    //
    cdc.SelectObject( pOld );
    cdc.DeleteDC();
    }
  }

void CTexMini::MainTimer()
  {
  if ( ::IsWindow(m_hChild) )
    ::MainTimer( m_hChild, 0 );
  }

void CTexMini::OnPaint()
  {
	CPaintDC dc(this);
  //
  CRect rect;
  GetClientRect(rect);
  if ( m_bPresent )
    {
//    ::MainTimer( m_hChild, 0 );
    return;
    }
  //
  if ( m_bmpWnd.m_hObject == NULL ) return;
  //
  CDC cdc;
  if ( cdc.CreateCompatibleDC( &dc ) )
    {
    CBitmap* pOld = cdc.SelectObject( &m_bmpMem );
    BuildImg( cdc, rect);
    dc.BitBlt( 0,0, rect.Width(), rect.Height(), &cdc, 0,0,SRCCOPY);
    cdc.SelectObject( pOld );
    cdc.DeleteDC();
    }
  }
//
void CTexMini::BuildBmp()
  {
    if (m_bPresent)
		return;
	CRect rect;
	GetClientRect(rect);
	m_rectPict = rect;
  //
  SIZE sz = m_pScheme->GetSizeEmf();
  //
	double kx = double(rect.Width ())/sz.cx;
	double ky = double(rect.Height())/sz.cy;
	if ( kx > ky )
	  {
    m_rectPict.DeflateRect(0,4);
    ky = double(m_rectPict.Height())/sz.cy;
  	double Width     = rect.Width()*ky/kx;	// ширина рисунка
  	m_rectPict.left  = m_rectPict.left + int((m_rectPict.Width() - Width)*0.5);
	  m_rectPict.right = m_rectPict.left + int(Width);
	  }
	else
	  {
    m_rectPict.DeflateRect(4,0);
    kx = double(m_rectPict.Width ())/sz.cx;
  	double Height     = m_rectPict.Height()*kx/ky;	// высота рисунка
	  m_rectPict.top    = m_rectPict.top + int((m_rectPict.Height() - Height)*0.5);
    m_rectPict.bottom = m_rectPict.top + int(Height);
	  }
  //
  m_dScaleX = double(m_rectPict.Width ())/sz.cx;
  m_dScaleY = double(m_rectPict.Height())/sz.cy;
  //
  CRect rcim(0,0,rect.Width(),m_rectPict.Height());
  //
  if ( m_bmpWnd.m_hObject ) m_bmpWnd.DeleteObject();
  if ( m_bmpMsk.m_hObject ) m_bmpMsk.DeleteObject();
  if ( m_bmpMem.m_hObject ) m_bmpMem.DeleteObject();
  //
  //
	CDC* pDC = GetDC();
  if ( pDC != NULL )
    {
    m_bmpMsk.CreateCompatibleBitmap( pDC, rcim.Width(),rcim.Height());
    m_bmpWnd.CreateCompatibleBitmap( pDC, rect.Width(),rect.Height());
    m_bmpMem.CreateCompatibleBitmap( pDC, rect.Width(),rect.Height());
    //
    CDC cdc;
    if ( cdc.CreateCompatibleDC( pDC ) )
      {
      CBitmap* pOld = cdc.SelectObject( &m_bmpWnd );
  	  ::FillRect(cdc,rect,(HBRUSH)::GetStockObject(BLACK_BRUSH));
      //
      CBrush brush(0x600000);
      rect = m_rectPict;
      rect.InflateRect(2,2);
      ::FillRect(cdc,rect,brush);
      //
      ::EmfPaint(m_pScheme->GetEmf(),cdc,m_rectPict);
      //
      cdc.SelectObject( &m_bmpMsk );
      cdc.PatBlt( 0, 0, m_rectPict.Width(), m_rectPict.Height(), WHITENESS );
      cdc.FillSolidRect ( 0, 0, m_rectPict.Width(), m_rectPict.Height(), RGB ( 5, 255, 5 ));
      //
      cdc.SelectObject( pOld );
      cdc.DeleteDC();
      }
  	ReleaseDC(pDC);
    }
  Invalidate(false);
  }

BOOL CTexMini::MouseMoveLoop(const POINT& pt)
  {
	CClientDC dc(this);
	BOOL bQuit = FALSE;

	SendSchemePointF(pt);
	SetCapture();

	BOOL bScroll = FALSE;
	MSG msg;	
	while( !bQuit && GetMessage( &msg, NULL, 0, 0 ) )
  	{ 
		CPoint pt = CPoint(msg.pt);
		ScreenToClient(&pt);
		dc.DPtoLP(&pt);

		if(msg.hwnd == m_hWnd)
	  	{
			switch(msg.message)
			{
			case WM_LBUTTONUP:
        m_bPress = false;
				bQuit = TRUE;
				break;

			case WM_MOUSEMOVE:
				if(msg.wParam & MK_LBUTTON)
					SendSchemePointF(pt);
				continue;

			case WM_PAINT:
				break;

			default:
				continue;
			}
		}
		// обрабатываем сообщение
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	ReleaseCapture();
	return TRUE;
  }

BOOL CTexMini::OnEraseBkgnd(CDC* pDC)
  {
	return FALSE;
  }

void CTexMini::SendSchemePointF(const POINT& point)
  {
  int nScale = m_pScheme->GetScale();
  POINT ptc = {point.x-m_rectPict.left,point.y-m_rectPict.top};
  ptc.x = (long)((double)(ptc.x*nScale)/m_dScaleX/100.);
  ptc.y = (long)((double)(ptc.y*nScale)/m_dScaleY/100.);
  m_pScheme->SetPointToCenter(&ptc);
  }

void CTexMini::OnLButtonDown(UINT nFlags, CPoint point)
  {
  if ( !m_bPresent )
    {
    m_bPress = false;
    if ( m_rectPict.PtInRect( point ) )
      {
		  m_bPress = true;
      SendSchemePointF(point);
      }
    }
	CWnd::OnLButtonDown(nFlags, point);
  }

void CTexMini::OnMouseMove(UINT nFlags, CPoint point)
  {
  if ( !m_bPresent )
    {
  	if ( nFlags & MK_LBUTTON && m_bPress )
	  	MouseMoveLoop(point);
    }
	CWnd::OnMouseMove(nFlags, point);
  }

BOOL CTexMini::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
  {
  if ( !m_bPresent )
    {
    POINT point;
    ::GetCursorPos( &point );
    ScreenToClient( &point );
    if ( !m_rectPict.PtInRect( point ) )
      {
      static HCURSOR hCursor = ::LoadCursor(NULL,IDC_NO);
      ::SetCursor( hCursor );
      return FALSE;
      }
    }
  return CWnd::OnSetCursor(pWnd, nHitTest, message);
  }

void CTexMini::OnDestroy()
  {
  CWnd::OnDestroy();

  if ( ::IsWindow(m_hChild) )
    ::DestroyWindow(m_hChild);
  m_hChild = NULL;
  m_bPresent = false;
  }

void CTexMini::SetPresent( bool bSet )
  {
  if ( ::IsWindow(m_hChild) )
    {
    m_bPresent = bSet;
    ::ShowWindow(m_hChild,m_bPresent?SW_SHOW:SW_HIDE);
    }
  }