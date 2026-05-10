#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConShpAlmPr.h"
#include "../Extensions.h"

CTagConShpAlmPr::CTagConShpAlmPr(int n)
  {
  mParam = n;
  }
//
void CTagConShpAlmPr::Analiz( ANALIZ )
  {
  CTagDIV::Analiz( html, teg );
  }
//
void CTagConShpAlmPr::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CharMP str;
  ::GiveDACA( str, html.m_strObj );
  switch ( mParam )
    {
    case 0:// ConShpAlmPr
      mVal = ::FindExperion( html, str, "PVEUHI" );
      mDbl = ::FindExperion( html, str, "PVEULO" );
    break;
    case 1:// ConShpAlmPr_RecH
      mVal = ::FindExperion( html, str, "PVHIALM.TP" );
    break;
    case 2:// ConShpAlmPr_RecL
      mVal = ::FindExperion( html, str, "PVLOALM.TP" );
    break;
    case 3:// ConShpAlmPr_RecHH
      mVal = ::FindExperion( html, str, "PVHHALM.TP" );
    break;
    case 4:// ConShpAlmPr_RecLL
      mVal = ::FindExperion( html, str, "PVLLALM.TP" );
    break;
    default: ASSERT(0);
    };
  //
  if ( mVal.def && mParam != 0 )
    {
    CStyle& style = Style(html);
    style.m_clrLine = 0xff000000;//Add Sergej 26.03.10
    lstrcpy( str, mVal.def->name );
    char* fnd = strchr( str, '.' );
    if ( fnd ) *fnd = 0;
    lstrcat( str, ".PR" );
    mDbl.nNumber = mVal.nNumber;
    mDbl.def = ::NameToValueEx( mVal.def->dwLog, str );
    }
  	ttip[0] = 0;
	this->m_hToolTip = CreateWindowEx( 0,TOOLTIPS_CLASS,0,TTS_ALWAYSTIP|WS_CLIPCHILDREN ,0,0,0,0, html.mWnd->m_hWnd,0,0,0 );
	this->m_ToolInfo.cbSize = sizeof(TOOLINFO);
	this->m_ToolInfo.hwnd = html.mWnd->m_hWnd;
	this->m_ToolInfo.uId = 0;
	this->m_ToolInfo.uFlags = TTF_SUBCLASS;// | TTF_TRANSPARENT | TTF_TRACK;
	this->m_ToolInfo.hinst  = (HINSTANCE)GetModuleHandle(NULL);
	this->m_ToolInfo.lpszText = ttip;
	::GetClientRect(html.mWnd->m_hWnd, &this->m_ToolInfo.rect);
	this->m_ToolInfo.rect.left = this->m_ToolInfo.rect.top = 0;
	this->m_ToolInfo.rect.bottom = this->m_ToolInfo.rect.right = 2000;
	::SendMessage(this->m_hToolTip, TTM_ADDTOOL, 0, (LPARAM)(&(this->m_ToolInfo)));
	::SendMessage(this->m_hToolTip, TTM_ACTIVATE, TRUE, 0);
	::SendMessage(this->m_hToolTip, TTM_SETMAXTIPWIDTH, 0, 300);
  }
//
void CTagConShpAlmPr::Matrix( LOCALS )
  {
  CStyle& style = Style(html);

	POINT pt;
	if(GetCursorPos(&pt) && html.mWnd){
		RECT rc;
		html.mWnd->GetWindowRect(&rc);
		pt.x = pt.x - rc.left;
		pt.y = pt.y - rc.top;
		POINT point = ((CViewMnemo*)html.mWnd)->Scroll(pt, true);
		RECT trc = style.m_rcTrueRect;
		trc.left-=3;
		trc.right+=3;
		trc.top-=3;
		trc.bottom+=3;

		if (::PtInRect(&trc, point)){

			char tip[256] = {0};
			char str[128];
			::GiveDACA( str, html.m_strObj );
			
			SParamValueH valHI = ::FindExperion( html, str, "PVHIALM.TP" );
			SParamValueH valLO = ::FindExperion( html, str, "PVLOALM.TP" );
			SParamValueH valHH = ::FindExperion( html, str, "PVHHALM.TP" );
			SParamValueH valLL = ::FindExperion( html, str, "PVLLALM.TP" );
			SParamValueH valDESC = ::FindExperion( html, str, "EUDESC" );
			char * desc = html.pipe.ValueS_H(valDESC);
			double hh = html.pipe.ValueF_H(valHH);
			double hi = html.pipe.ValueF_H(valHI);
			double lo = html.pipe.ValueF_H(valLO);
			double ll = html.pipe.ValueF_H(valLL);

			if (!IsNaN(hh)) sprintf(tip, "PV High High: %.2f %s\n", hh, desc);
			if (!IsNaN(hi)) sprintf(tip, "%sPV High: %.2f %s\n", tip, hi, desc);
			if (!IsNaN(lo)) sprintf(tip, "%sPV Low: %.2f %s\n", tip, lo, desc);
			if (!IsNaN(ll)) sprintf(tip, "%sPV Low Low: %.2f %s\n", tip, ll, desc);

			if (strcmp(tip, m_ToolInfo.lpszText))
			{
				strcpy(m_ToolInfo.lpszText, tip);
				::SendMessage(this->m_hToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&m_ToolInfo);
			}
		}
		else{
			m_ToolInfo.lpszText[0] = 0;
			::SendMessage(this->m_hToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&m_ToolInfo);
		}
	}


	
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  if ( style.m_eVisible == enumHidden )
    return;
  //
  if ( mParam != 0 )
    {
    //ENUM_S(PVHHALM,PR,"PV HiHi Alarm Priority",NONE,JOURNAL,LOW,HIGH,URGENT)
    COLORREF color=GetColorPriorityAlarm( html.pipe, mDbl );
	if (color==0xff000000)
		return;
    RECT rect = style.FullRect();
    CMyBrush brush( hDC, color );
    brush.Rect( rect );
    return;
    }
  //
  INT_PTR nCount = GetSize();
  CTag**     obj = GetData();
  if ( nCount != 1 ) ASSUNO
  //
  nCount = obj[0]->GetSize();
  obj    = obj[0]->GetData();
  //
  double dMax = html.pipe.ValueF_H( mVal );
  double dMin = html.pipe.ValueF_H( mDbl );
  if ( IsNaN(dMax) || IsNaN(dMin) || dMax <= dMin )
    {
    for ( INT_PTR n = 0; n < nCount; n++ )
      {
      CStyle& child = obj[n]->Style(html);
      child.m_eVisible = enumHidden;
      child.mRect.Height = 0;
      }
    return;
    }
  //
  RectF rect = style.mRect;
  double h = rect.Height;
  //
  for ( INT_PTR n = 0; n < nCount; n++ )
    {
    CStyle& child = obj[n]->Style(html);
    char* isd = child.NameID( html );
    if ( obj[n]->mParam > 0 )
      {
      double dVal = html.pipe.ValueF_H( obj[n]->mVal );
      if ( IsNaN(dVal) )
        {
        child.m_eVisible = enumHidden;
        child.mRect.Height = 0;
        continue;
        }
      child.m_eVisible = enumVisible;
      double v = (dVal-dMin)/(dMax-dMin);
      child.m_clrFill = GetColorPriorityAlarm( html.pipe, obj[n]->mDbl );
      switch ( obj[n]->mParam )
        {
        case 1:// ConShpAlmPr_RecH
        case 3:// ConShpAlmPr_RecHH
          child.mRect.Height = REAL(h*(1.-v));
        break;
        case 2:// ConShpAlmPr_RecL
        case 4:// ConShpAlmPr_RecLL
          child.mRect.Height = REAL(h*v);
          child.mRect.Y = REAL(rect.Y+h*(1.-v));
        break;
        };
      }
    }
  }
//
