#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
//
#include "HTMLWndBase.h"
#include "TagSCRIPT.h"
//
bool CHTML_LoopTuneEdit::WindowPos( WND_POS )
{
	bool bInit = ( !::IsWindow( m_hWnd ) );
	if ( !Create( hParent, "edit", WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, WS_EX_CLIENTEDGE ) )
		return false;
	::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height, SWP_NOZORDER );
	SetCtrlFont( hFont, fScale );	
	if ( bInit )
	{
		double v = this->html.pipe.ValueF_H(*(this->m_Val));
		char val[16]; sprintf_s(val, "%f", v);
		::SetWindowText( m_hWnd,  val);
	}
	return true;
}
LRESULT CHTML_LoopTuneEdit::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_KEYDOWN && wParam == VK_RETURN )
	{
		char strval[128];
		::GetWindowText( m_hWnd, strval, 128 );
		if (m_Val && Version == LG35_8_KF) 
		{
			double fOld = this->html.pipe.ValueF_H(*(this->m_Val));
			double fNew = atof(strval);
			if (fOld != fNew)
			{
				SSendToModel send;
				lstrcpy( send.szValue, m_Val->def->name );
				send.Set( enumValueDbl, &fOld, &fNew );
				html.pipe.SendData( m_Val->nNumber, send, true );
			}
		}
	}
	return CHTMLEditBox::OnMessage( uMsg, wParam, lParam );
}
void CHTML_LoopTuneEdit::Pressed()
{
	if ( !::IsWindow( m_hWnd ) ) return;
	//
	::ShowWindow( m_hWnd, SW_SHOW );
	::SendMessage( m_hWnd, EM_SETSEL, 0, -1 );
	::SetFocus( m_hWnd );
}
void CHTML_LoopTuneEdit::Release()
{
	if ( !::IsWindow( m_hWnd ) ) return;
	::SendMessage( m_hWnd, WM_KILLFOCUS, 0, 0 );
}
void CHTML_LoopTuneEdit::Timer(){}
int CHTML_LoopTuneEdit::HWBTN(WORD nCode,UINT uKey)
{
	return 0;
}

bool CHTML_Edit::WindowPos( WND_POS )
  {
  bool bInit = ( !::IsWindow( m_hWnd ) );
  if ( !Create( hParent, "edit", WS_CHILD | WS_BORDER | ES_CENTER | ES_AUTOHSCROLL, WS_EX_CLIENTEDGE ) )
    return false;
  ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height, SWP_NOZORDER );
  SetCtrlFont( hFont, fScale );
  //
  if ( bInit )
    {
    ::SetWindowText( m_hWnd, m_pszValue );
    }
  return true;
  }
//
void CHTML_Edit::Timer()
  {
  }
void CHTML_Edit::Release()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  ::SendMessage( m_hWnd, WM_KILLFOCUS, 0, 0 );
  }
//
void CHTML_Edit::Pressed()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  //
  ::ShowWindow( m_hWnd, SW_SHOW );
  ::SendMessage( m_hWnd, EM_SETSEL, 0, -1 );
  ::SetFocus( m_hWnd );
  }
//***************************************************************************************
LRESULT CHTML_Edit::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_KEYDOWN && wParam == VK_RETURN )
    {
    ::GetWindowText( m_hWnd, m_pszValue, 128 );
    if(m_pScript)
      m_pScript->RunScript();
    else
    if(IsInt(m_pszValue))
      {
        if(html.m_strFile.CompareNoCase("sysGrpDetail")==0 || html.m_strFile.CompareNoCase("sysGrpTrend")==0 ||html.m_strFile.CompareNoCase("sysGrpNumeric")==0)
        {
          GetAppWnd()->ButtonOk( barGROUP, m_pszValue, m_hWnd );
          return 0;
        }
      }  
    }
  return CHTMLEditBox::OnMessage( uMsg, wParam, lParam );
  }

int CHTML_Edit::HWBTN( WORD nCode, UINT uKey )
  {
  return 0;
  }
//
bool CHTML_OTHER::WindowPos( WND_POS )
{
	bool bInit = ( !::IsWindow( m_hWnd ) );
	if ( !Create( hParent, "edit", WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, WS_EX_CLIENTEDGE ) )
		return false;
	::SetWindowPos( m_hWnd, NULL, rc.X-2, rc.Y, rc.Width+2, rc.Height, SWP_NOZORDER );
	//SetCtrlFont( hFont, fScale );	
    {
    LOGFONT lf = theApp.GiveLogF(hFont);
    lf.lfHeight = (int)(lf.lfHeight*fScale*0.85);
    if ( m_nFontHeight != lf.lfHeight )
      {
      m_nFontHeight  = lf.lfHeight;
      HFONT hNewFont = ::CreateFontIndirect( &lf );
      HFONT hOldFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0, 0 );
      ::SendMessage( m_hWnd, WM_SETFONT, (WPARAM)hNewFont, 1 );
      if ( hOldFont != NULL )
      ::DeleteObject( hOldFont );
      }
    }
	if ( bInit )
      SetText();

	return true;
}

LRESULT CHTML_OTHER::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_KEYDOWN && wParam == VK_RETURN )
	{
		char strval[128];
		::GetWindowText( m_hWnd, strval, 128 );
		if (m_Val && m_Val->def) 
		{
          switch(m_Val->def->eVal)
          {
           case enumValueDbl:
            {
			double fOld = this->html.pipe.ValueF_H(*(this->m_Val));
			double fNew = atof(strval);
			if (fOld != fNew)
			  {
				SSendToModel send;
				lstrcpy( send.szValue, m_Val->def->name );
				send.Set( enumValueDbl, &fOld, &fNew );
				html.pipe.SendData( m_Val->nNumber, send, true );
			  }
            }
            break;
           case enumValueInt:
            {
			int nOld = this->html.pipe.ValueI_H(*(this->m_Val));
			int nNew = atoi(strval);
			if (nOld != nNew)
			  {
				SSendToModel send;
				lstrcpy( send.szValue, m_Val->def->name );
				send.Set( enumValueInt, &nOld, &nNew );
				html.pipe.SendData( m_Val->nNumber, send, true );
			  }
            }
            break;
          }
		}
	}
	return CHTMLEditBox::OnMessage( uMsg, wParam, lParam );
}

void CHTML_OTHER::Pressed()
{
	if ( !::IsWindow( m_hWnd ) ) return;
	//
	::ShowWindow( m_hWnd, SW_SHOW );
	::SendMessage( m_hWnd, EM_SETSEL, 0, -1 );
	::SetFocus( m_hWnd );
}

void CHTML_OTHER::Release()
{
	if ( !::IsWindow( m_hWnd ) ) return;
	::SendMessage( m_hWnd, WM_KILLFOCUS, 0, 0 );
}

void CHTML_OTHER::Timer()
{
    if( GetFocus() == m_hWnd)
      return;
    SetText();
}

int CHTML_OTHER::HWBTN(WORD nCode,UINT uKey)
{
	return 0;
}

void CHTML_OTHER::SetText()
{
    if (m_Val && m_Val->def)
     {
        switch(m_Val->def->eVal)
         {
          case enumValueDbl:
           {
	       double v = html.pipe.ValueF_H(*m_Val);
           if(m_pszFormat==NULL)
              m_pszFormat = "%.0f";
	       char str[32]; sprintf_s(str, m_pszFormat, v);
	       ::SetWindowText( m_hWnd, str);
           }
           break;
          case  enumValueInt:
           {
	       int v = html.pipe.ValueI_H(*m_Val);
	       char str[32]; sprintf_s(str, "%i", v);
	       ::SetWindowText( m_hWnd, str);
           }
           break;
        }
     }
}
