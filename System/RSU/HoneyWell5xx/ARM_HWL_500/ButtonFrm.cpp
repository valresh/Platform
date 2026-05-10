#include "StdAfx.h"
#include "ARM.h"
#include "AplicFrm.h"
#include "ButtonFrm.h"

#include "uxtheme.h"
#pragma comment(lib, "UxTheme.lib")

IMPLEMENT_DYNAMIC(CButtonFrm, CFrameWnd)
CButtonFrm::CButtonFrm( int nMonitor, bool bGrey)
: m_bNotActive(bGrey)
, m_bClose(true)
, m_bZamokOn(false)
, m_bGalkaOn(false)
, m_nMonitor(nMonitor)
  {
  _static bool bInit = false;
  if ( bInit ) return;
  bInit = true;
  //  
  bmpGalka[0].LoadBitmap(IDB_GALKA2GREY);
  bmpGalka[1].LoadBitmap(IDB_GALKA2ON   );
  bmpGalka[2].LoadBitmap(IDB_GALKA2   );
  //
  bmpZamok[0].LoadBitmap(IDB_ZAMOK2GREY);
  bmpZamok[1].LoadBitmap(IDB_ZAMOK2ON    );
  bmpZamok[2].LoadBitmap(IDB_ZAMOK2      );
  }

CBitmap CButtonFrm::bmpGalka[];
CBitmap CButtonFrm::bmpZamok[];

BEGIN_MESSAGE_MAP(CButtonFrm, CFrameWnd)
  ON_WM_NCPAINT()
  ON_WM_KILLFOCUS()
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CButtonFrm::~CButtonFrm(void)
  {
  }

BOOL CButtonFrm::Create(
    LPCTSTR lpszClassName,
    LPCTSTR lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    LPCTSTR lpszMenuName,
    DWORD dwExStyle,
    CCreateContext* pContext)
{
  BOOL r=CFrameWnd::Create(
    lpszClassName,
    lpszWindowName,
    dwStyle,
    rect,
    pParentWnd,
    lpszMenuName,
    dwExStyle,
    pContext);
  
  if(r)
  {
       SetWindowTheme(m_hWnd, L"", L"");
       return r;
  }

  return false;
}


void CButtonFrm::OnNcPaint()
  {
  CFrameWnd::OnNcPaint();
  if (m_nMonitor!=-1)
    PaintExtButton();
  }
//
void CButtonFrm::PaintExtButton()
  {
  if ( !::IsWindow(m_hWnd) ) 
	  return;
  //if (GetAppWnd()->GetFirstFrame()==this) 
	 // return;
  //
  CWindowDC dc( CWnd::FromHandle(m_hWnd) );
	dc.SetStretchBltMode(HALFTONE);
  //
  int n;
  CRect rect;
  CDC dcDisplayMemory;
  if ( dcDisplayMemory.CreateCompatibleDC(&dc) )
    {
    //
    n = m_bNotActive ? 0 : (m_bGalkaOn ? 1 : 2);
    rect = GalkaButton();
    dcDisplayMemory.SelectObject(&bmpGalka[n]);
    BITMAP bm;
    bmpGalka[n].GetBitmap(&bm);
    dc.StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dcDisplayMemory,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
    //
    if ( this == GetAppWnd() )
      {
      n = m_bNotActive ? 0 : (m_bZamokOn ? 1 : 2);
      rect = ZamokButton();
      dcDisplayMemory.SelectObject(&bmpZamok[n]);
      bmpZamok[n].GetBitmap(&bm);
      dc.StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dcDisplayMemory,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
      }
    //
    dcDisplayMemory.DeleteDC();
    }
  }
//
void CButtonFrm::OnSetFocus(CWnd* pOldWnd)
  {
  if (m_nMonitor!=-1)
		PaintExtButton();
  GetAppWnd()->SetActiveButtonFrm(this);
  CViewMnemo* mnemo = GetMnemo();
  if ( mnemo )
  GetAppWnd()->SetActiveMnemo( mnemo->m_hWnd );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CButtonFrm::OnKillFocus(CWnd* pNewWnd)
{
	/*CFrameWnd::OnKillFocus(pNewWnd);*/
  if (m_nMonitor!=-1)
		PaintExtButton();
}

CRect CButtonFrm::ZamokButton(bool bFromNCLBDown)
{
	int cx=GetSystemMetrics(SM_CXSIZE);
    cx=2*cx/3;
	cx-=(cx/10)*2;
	int cy=GetSystemMetrics(SM_CYSIZE)-4;
	CRect rc;
	GetWindowRect(&rc);
	int nSdvig;
	if (m_bClose)
		nSdvig=5+2*2+cx*3;
	else
		nSdvig=5+2+cx*2;
	CRect tempRect=CRect(rc.Width()-nSdvig,5,
		rc.Width()-nSdvig+cx,5+cy);
	if (bFromNCLBDown)
	{
		tempRect=CRect(rc.right-nSdvig, rc.top+5,
			rc.right-nSdvig+cx, rc.top+5+cy);
	}
	return tempRect;
}
//
CRect CButtonFrm::GalkaButton(bool bFromNCLBDown)
{
    CRect rc;
    if(!::IsWindow(m_hWnd))
      return rc;
	int cx=GetSystemMetrics(SM_CXSIZE);
    cx=2*cx/3;
	cx-=(cx/10)*2;
	int cy=GetSystemMetrics(SM_CYSIZE)-4;
	GetWindowRect(&rc);
	int nSdvig;
	if (m_bClose)
		nSdvig=5+2+cx*2;
	else
		nSdvig=5+cx;
	CRect tempRect=CRect(rc.Width()-nSdvig,5,
		rc.Width()-nSdvig+cx,5+cy);
	if (bFromNCLBDown)
	{
		tempRect=CRect(rc.right-nSdvig, rc.top+5,
			rc.right-nSdvig+cx, rc.top+5+cy);
	}
	return tempRect;
}
//
void CButtonFrm::PostNcDestroy()
  {
  GetAppWnd()->SetActiveButtonFrm(NULL);
  delete this;
  }
//
