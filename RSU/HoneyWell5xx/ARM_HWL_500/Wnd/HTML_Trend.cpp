#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "../ViewTrend.h"
#include "../ViewTrend4.h"
#include "../ViewTrendTag.h"
#include "../ViewTrendLoc.h"
#include "../ViewTrendLoc2.h"
#include "../ViewTrendLoc3.h"
#include "../ViewTrendLocG.h"
#include "../ViewMiniTrend2.h"
#include "HTML.h"
//
CHTML_Trend::~CHTML_Trend()
  {
  if ( wnd ) delete wnd;
  }
//
bool CHTML_Trend::WindowPos( WND_POS )
  {
  bool bInit = ( !::IsWindow( m_hWnd ) );
  if ( wnd == NULL )
    {
    if ( html.m_bR300 )
      {
      if ( lstrcmpi( html.m_strFile, "sysTnd03" ) == 0 )
        wnd = new CViewTrend4(html.Group(),true),mTr3 = 1;
      else
      if ( lstrcmpi( html.m_strFile, "sysGrpTrend" ) == 0 )
        wnd = new CViewTrend(html.Group());
      else
      if ( lstrcmpi( html.m_strFile, "sysGrpNumeric" ) == 0 )
        wnd = new CViewTrend(html.Group());
      else
        wnd = new CViewTrendTag(html,mDsd);
      }
	else if (Version == AT_PRP)
	{
		if (!strnicmp(this->html.m_strFile, "TrendyUPPN-220(2)", 17) || !strnicmp(this->html.m_strFile, "TrendyUPPN-220", 14))
		{
			wnd = new CViewTrendLoc2(html,mDsd, false);
		}
		else if (!strnicmp(this->html.m_strFile, "E-5", 3))
		{
			wnd = new CViewTrendLoc2(html, mDsd, true);
		}
		else
		{
			wnd = new CViewTrendLoc3(html,mDsd);
		}
	}
	else if (Version == UKR_PRP)
	{
		if (!strcmpi(this->html.m_strFile, "trendsumma") 
			|| !strcmpi(this->html.m_strFile, "trendriform") 
			|| !strcmpi(this->html.m_strFile, "cehovietrnd") 
			|| !strcmpi(this->html.m_strFile, "e-5_npu"))
		{
			wnd = new CViewTrendLoc2(html, mDsd, false);
		}
		else
		{
			wnd = new CViewTrendLoc3(html,mDsd);
		}
	}
    else
      if ( lstrcmpi( html.m_strFile, "Mini_Trend_Popup" ) == 0 )
          wnd = new CViewMiniTrend2(html.Group());
    else
		if(Version == T1165P_ONPZ || Version == T1163_ONPZ) 
			wnd = new CViewTrendLocG(html,mDsd);
				 //new CViewTrend4(html.Group(),true),mTr3 = 1;
		else 
			wnd = new CViewTrendLoc(html,mDsd);
    //
    //wnd->CalcSizes( rc.Width, rc.Height );
    DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    if ( wnd->CreateEx( 0, NULL, "Тренды", dwStyle, 0,0,0,0, hParent, 0 ) )
    m_hWnd = wnd->m_hWnd;
    }
  //
  if ( wnd != NULL )
    {
    ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height, SWP_NOZORDER | SWP_NOSIZE );
    //
    wnd->CalcSizes( rc.Width, rc.Height );
    LOGFONT lf = theApp.GiveLogF(hFont);
    lf.lfHeight = (int)(lf.lfHeight*fScale);
    //wnd->SetMyFont( lf );
    //wnd->SetScaleFromSmall(fScale);
    }
  //
  if ( bInit )
    {
    }
  return true;
  }
//
void CHTML_Trend::Timer()
  {
  //if ( !::IsWindow( m_hWnd ) ) return;
  //if ( !::IsWindowVisible( m_hWnd ) ) return;
  //wnd->MainTimer(dwMainTimer);
  }
//
//***************************************************************************************
int CHTML_Trend::HWBTN(WORD nCode,UINT uKey)
  {
  if ( nCode == HW_KEYDOWN )
  if ( ::IsWindowVisible(m_hWnd) && mTr3 )
    {
    CViewTrend4*  wnd = (CViewTrend4*)CWnd::FromHandle(m_hWnd);
    switch ( uKey )
      {
      case hw_STEP_UP  : return wnd->Prev();
      case hw_STEP_DOWN: return wnd->Next();
      }
    }
  return 0;
  }
