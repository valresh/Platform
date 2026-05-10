#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "HTMLWndBase.h"
#include "HTML.h"
#include "Tag.h"

ViewCombo::ViewCombo(CHTML& _html)
:CHTMLComboBox( _html ){}

CTag * findAl(CHTML& html, CTag * root){  
  INT_PTR nCount = root->GetSize();
  CTag**     obj = root->GetData();
  for (int i = 0; i < nCount; i++)
  {
	  char * name = obj[i]->Style(html).NameID(html);
	  CHTMLWndBase* wnd = obj[i]->m_pWnd;
	  if (wnd && !strcmp(name, "Table")) 
		  return obj[i];
	  else{
		  CTag * fnd = findAl(html, obj[i]);
		  if (fnd) 
			  return fnd;
		  else 
			  continue;
	  }
  }
  return NULL;
}

bool ViewCombo::WindowPos( WND_POS )
{
  LRESULT N;
  int n = 1;
  if ( !Create( hParent, "combobox", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL ) )
    return false;
  ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height*10, SWP_NOZORDER );
  SetCtrlFont( hFont, fScale );
  
  N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)"(all alarms)" );
  ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n++    );

  N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)"(unacknowledged alarms)" );
  ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n++    );

  N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)"(urgent and high priority alarms)" );
  ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n++    );

  N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)"(urgent priority alarms)" );
  ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n++    );
  
  ::SendMessage( m_hWnd, CB_SETCURSEL, 0, 0 );

  this->almtag = findAl(html, html.mBody);



  return true;
}

LRESULT ViewCombo::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( HIWORD(wParam) == CBN_SELCHANGE )
	{
		LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
		if ( nSel != CB_ERR )
		{
			CViewAlarm* tbl = (CViewAlarm*)(((CHTML_Alarm*)this->almtag->m_pWnd)->wnd);
			if (tbl)
			{
				tbl->SetViewFilter(nSel);
			}
        }
    }
	return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
}

void ViewCombo::Timer(){}

int ViewCombo::HWBTN(WORD nCode,UINT uKey)
{
	return 0;
}
