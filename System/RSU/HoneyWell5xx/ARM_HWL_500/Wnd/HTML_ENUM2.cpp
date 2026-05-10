#include "StdAfx.h"
#include "HTMLWndBase.h"
#include "HTML.h"

bool CHTML_ENUM2::WindowPos(WND_POS) {
	bool bAddString = (!::IsWindow(m_hWnd));
	if (!Create(hParent, "combobox", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN)) return false;
	::SetWindowPos(m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height*10, SWP_NOZORDER);
	SetCtrlFont(hFont, fScale);
	if (bAddString) {
		for (int n = 0; n< 2; n++) {
			std::string strText;
			CShFLAG sh;
			if (html.pipe.GetData(mVal.nNumber, &sh)) strText=sh.STATETEXT[n];
			if (n) {
				LRESULT N = ::SendMessage(m_hWnd, CB_ADDSTRING, n, (LPARAM)strText.c_str());
				::SendMessage(m_hWnd, CB_SETITEMDATA, N, (LPARAM)n);
			}
		}
		Timer();
	}
	return true;
}
void CHTML_ENUM2::Timer() {
	if (!::IsWindow(m_hWnd)) return;
	char* str = html.pipe.ValueS_H(mVal);
	CShFLAG sh;
	html.pipe.GetData(mVal.nNumber, &sh);
	int k = (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
	::SetWindowText(m_hWnd, str);
}
//***************************************************************************************
LRESULT CHTML_ENUM2::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if ( (uMsg != CB_SETEDITSEL) && (uMsg != CB_GETCURSEL) && (uMsg != WM_SETTEXT) && (uMsg != WM_CTLCOLOREDIT)  //с этими здесь зацикливается
       )
    {
          //::SendMessage(m_hWnd, CB_SETEDITSEL , 0, -1 );
          CHTMLComboBox::OnMessage(CB_SETEDITSEL, 0, -1);
          ::HideCaret(0);
	}
	if (uMsg == WM_COMMAND) {
		if (HIWORD(wParam) == CBN_SELCHANGE) {
			LRESULT nSel = ::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
			if (nSel != CB_ERR && nSel == 0) {
				BYTE nNew = (BYTE)1;
				BYTE nOld;
				SSendToModel send;
				lstrcpy( send.szValue, "PVFL" );
				send.Set( enumValueChr, &nOld, &nNew );
				html.pipe.SendData( mVal.nNumber, send, true );
				Timer();
			}
		}
	}
	if((WM_LBUTTONDOWN == uMsg) && theApp.IsAllInOne()) {
		HWND hParWnd = m_hWnd;
		bool bSkipMesage = false;
		extern HWND hSensorFaceplateWnd;
		while(hParWnd = GetParent(hParWnd)) {
			if(hParWnd == hSensorFaceplateWnd) {
				bSkipMesage = true;
				::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mVal, 1<<16 | 1);
				break;
			}
		}
		if(bSkipMesage) return 1;
	}
	return CHTMLComboBox::OnMessage(uMsg, wParam, lParam);
}

int CHTML_ENUM2::HWBTN(WORD nCode, UINT uKey) {
	if (mVal.def == NULL) return 0;
	if (lstrcmpi(mVal.def->name, "MODE")) return 0;
	BYTE nNew = 0;
	BYTE nOld = html.pipe.ValueB_H( mVal );
	switch (uKey) {
		case hw_MAN : nNew = 0; break;
		case hw_AUTO: nNew = 1; break;
		case hw_NORM: nNew = 5; break;
		default: return 0;
	}
	SSendToModel send;
	lstrcpy( send.szValue, mVal.def->name );
	send.Set( enumValueChr, &nOld, &nNew );
	html.pipe.SendData( mVal.nNumber, send, true );
	::SetFocus( m_hWnd );
	return 1;
}
