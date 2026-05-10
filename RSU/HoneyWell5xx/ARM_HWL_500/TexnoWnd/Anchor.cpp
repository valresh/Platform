#include "stdafx.h"
#include "../ARM.h"
#include "BaseType.h"
#include "TexChild.h"
#include "Anchor.h"

CAnchor::CAnchor(CTexChild* pParent)
: m_bQuitTracking(FALSE)
, m_pTex(pParent)
  {
  }

CAnchor::~CAnchor()
  {
  }

BEGIN_MESSAGE_MAP(CAnchor, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CAnchor::PreTranslateMessage(MSG* pMsg)
{
	BOOL bRetVal = FALSE;

	switch (pMsg->message)
	{
	// any of these messages cause us to quit scrolling
	case WM_MOUSEWHEEL:
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
		m_bQuitTracking = TRUE;
		bRetVal = TRUE;
		break;
	// Button up message depend on the position of cursor
	// This enables the user to click and drag for a quick pan.
	case WM_MBUTTONUP:
		{
		CPoint pt(pMsg->lParam);
		ClientToScreen(&pt);
		if (!PtInRect(&m_rectDrag, pt))
      m_bQuitTracking = TRUE;
		bRetVal = TRUE;
		}
		break;
	}
	return bRetVal;
  }

void CAnchor::OnTimer(UINT_PTR nIDEvent)
{
	ASSERT(nIDEvent == ID_TIMER_TRACKING);
	UNUSED(nIDEvent);

	int nCursor = -1;

	CPoint ptNow;
	GetCursorPos(&ptNow);

	CRect rectClient;
	GetWindowRect(&rectClient);

	// decide where the relative position of the cursor is
	// pick a cursor that points where we're going
	nCursor = 0;

  if (ptNow.y < rectClient.top)
	  nCursor = AFX_IDC_MOUSE_PAN_N;
	else if (ptNow.y > rectClient.bottom)
		nCursor = AFX_IDC_MOUSE_PAN_S;

	// if horizontal scrolling allowed, cosider horizontal
	// directions for the cursor, too. Only consider diagonal
	// if we can scroll both ways.
	if (ptNow.x < rectClient.left)
		{
		if (nCursor == 0)
			nCursor = AFX_IDC_MOUSE_PAN_W;
		else
			nCursor--;
		}
	else
  if (ptNow.x > rectClient.right)
		{
		if (nCursor == 0)
			nCursor = AFX_IDC_MOUSE_PAN_E;
		else
			nCursor++;
		}

	if (m_bQuitTracking)
  	{
		KillTimer(ID_TIMER_TRACKING);
		ReleaseCapture();
		ShowWindow(SW_HIDE);
		SetCursor(NULL);
	  }
	else
  if (nCursor == 0)
	  {
		SetCursor(m_hAnchorCursor);
	  }
	else
  	{
		HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nCursor),RT_GROUP_CURSOR);
		HICON hCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(nCursor));
		ASSERT(hCursor != NULL);
		SetCursor(hCursor);

		// ask the view how much to scroll this time
		CSize sizeDistance; // = ptNow - rectClient.CenterPoint();

		if (ptNow.x > rectClient.right)
			sizeDistance.cx = ptNow.x - rectClient.right;
		else if (ptNow.x < rectClient.left)
			sizeDistance.cx = ptNow.x - rectClient.left;
		else
			sizeDistance.cx = 0;

		if (ptNow.y > rectClient.bottom)
			sizeDistance.cy = ptNow.y - rectClient.bottom;
		else if (ptNow.y < rectClient.top)
			sizeDistance.cy = ptNow.y - rectClient.top;
		else
			sizeDistance.cy = 0;

    m_pTex->AnchorScroll(&sizeDistance);
		// restore ourselves and repaint
		// SetFocus();
		UpdateWindow();

		// move ourselves back (since we're a child, we scroll too!)
		SetWindowPos(&CWnd::wndTop,
			m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP/2,
			m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP/2, 0, 0,
			SWP_NOACTIVATE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
}

void CAnchor::SetBitmap(UINT nID)
{
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_GROUP_CURSOR);
	ASSERT(hInst != NULL);
	m_hAnchorCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(nID));
}

BOOL CAnchor::Create()
  {
	if ( !::GetSystemMetrics(SM_MOUSEWHEELPRESENT) )
    return FALSE;
  //
  SetBitmap(AFX_IDC_MOUSE_ORG_HV);
  //
	BOOL bRetVal = 
		CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
			AfxRegisterWndClass(CS_SAVEBITS),
			NULL,
			WS_POPUP, -100, -100,
			AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP,
			NULL, NULL);
  if ( bRetVal )
    {
  	SetOwner(m_pTex);
		CRgn rgn;
		rgn.CreateEllipticRgn(0, 0, AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP);
		SetWindowRgn(rgn, TRUE);
    }
	return bRetVal;
  }

BOOL CAnchor::Anchord(CPoint* ptAnchor)
  {
  if ( ptAnchor )
    {
    m_bQuitTracking = FALSE;
    m_ptAnchor = *ptAnchor;
    m_pTex->ClientToScreen(&m_ptAnchor);

    m_rectDrag = CRect(m_ptAnchor,m_ptAnchor);
    m_rectDrag.InflateRect( GetSystemMetrics(SM_CXDOUBLECLK),
                            GetSystemMetrics(SM_CYDOUBLECLK) );
		SetCapture();
    SetTimer(ID_TIMER_TRACKING, 50, NULL);
		SetWindowPos(&CWnd::wndTop,
			m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP/2,
			m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP/2, 0, 0,
			SWP_NOACTIVATE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
  else
    {
    ReleaseCapture();
    KillTimer(ID_TIMER_TRACKING);
		SetCursor(NULL);
    ShowWindow(SW_HIDE);
    }
	return TRUE;
	}

void CAnchor::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	// shrink a pixel in every dimension for border
	rect.DeflateRect(1, 1, 1, 1);
	dc.Ellipse(rect);

	// draw anchor shape
	dc.DrawIcon(0, 0, m_hAnchorCursor);
}
