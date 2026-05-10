#pragma once
#include "HTML.h"
#include "TagTextArea.h"

void vbsDrawAllObjects(CHTML * html);

class CTagCmFbCxxInvert: public CTagTextArea
{
public:
    CTagCmFbCxxInvert(int n);
    void Matrix( LOCALS );
};
class CTagCmFbGxNumInputs: public CTagTextArea
{
public:
    CTagCmFbGxNumInputs(int n);
    void Matrix( LOCALS );
};
class CTagCmFbGxFistCond: public CTagTextArea
{
public:
    CTagCmFbGxFistCond(int n);
    void Matrix( LOCALS );
};
class CTagCmFbGxSO: public CTagTextArea
{
public:
    CTagCmFbGxSO(int n);
    void Matrix( LOCALS );
};
class CTagCmFbUpDetected: public CTagTextArea
{
public:
    CTagCmFbUpDetected(int n);
    void Matrix( LOCALS );
};
class CTagCmFbFirstUp: public CTagTextArea
{
public:
    CTagCmFbFirstUp(int n);
    void Matrix( LOCALS );
};
class CTagCmFbCXXSo: public CTagTextArea
{
public:
    CTagCmFbCXXSo(int n);
    void Matrix( LOCALS );
};
class CTagCmFbCXXFl: public CTagTextArea
{
public:
    CTagCmFbCXXFl(int n);
    void Matrix( LOCALS );
};
class CTagCmFbCXXByPreq: public CTagTextArea
{
public:
    CTagCmFbCXXByPreq(int n);
    void Matrix( LOCALS );
};
class CTagCmFbGXAlgId: public CTagTextArea
{
public:
    CTagCmFbGXAlgId(int n);
    void Matrix( LOCALS );
};
class CNotBindedCombo: public CTag
{
public:
    CNotBindedCombo(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
};
class CTagCmFbCXXByPerm: public CTagTextArea
{
public:
    CTagCmFbCXXByPerm(int n);
    void Matrix( LOCALS );
};
class CHTML_NBCB: public CHTMLComboBox
{
	CNotBindedCombo* m_ptrHolder;
public:
	CHTML_NBCB(CHTML& html, CNotBindedCombo * holdr, HWND hParent = NULL)
		: CHTMLComboBox( html )
		, m_ptrHolder(holdr)
	{
		Create(hParent, "combobox", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL);
	}
	~CHTML_NBCB(void){}
	bool WindowPos( WND_POS )
	{ 
		if (!::IsWindow(m_hWnd) && !Create( hParent, "combobox", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL))
			return false;
		::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height*10, SWP_NOZORDER );
		SetCtrlFont( hFont, fScale );
		return true;
	}
	LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{ 
		return CHTMLComboBox::OnMessage(uMsg, wParam, lParam ); 
	}
	void Timer()
	{
		if (m_ptrHolder)
		{
#ifdef _DEBUG
            char * id = m_ptrHolder->Style(html).NameID(html);
#endif
			if (m_ptrHolder->Style(this->html).m_eVisible == enumHidden)
			{
				::ShowWindow(this->m_hWnd, SW_HIDE);
			}
			else
			{
				::ShowWindow(this->m_hWnd, SW_SHOW);
			}
		}
	}
	int HWBTN(WORD nCode,UINT uKey){ return 0; }
	const char * CurrentValue()
	{
		LRESULT c = ::SendMessage(this->m_hWnd, CB_GETCOUNT  , 0, 0);
		LRESULT l = ::SendMessage(this->m_hWnd, CB_GETCURSEL , 0, 0);
		if (c == 0 || l == CB_ERR) return NULL;
		LRESULT d = ::SendMessage(this->m_hWnd, CB_GETITEMDATA, l, 0);

		return NULL;
	}
	void CurrentValue(const char * value){}
	void ResetContent()
	{
		::SendMessage(this->m_hWnd, CB_RESETCONTENT, 0, 0);
	}
	void AddString(char * str)
	{
		::SendMessage(this->m_hWnd, CB_ADDSTRING, 0, (LPARAM)str);
	}
};

CTag* fnCM_FB_CXXINVERT(int n);
CTag* fnCM_FB_GXNUMINPUTS(int n);
CTag* fnCM_FB_GXFIRSTCOND(int n);
CTag* fnCM_FB_GXSO(int n);
CTag* fnCM_FB_UPDETECTED(int n);
CTag* fnCM_FB_FIRSTUP(int n);
CTag* fnCM_FB_CXXSO(int n);
CTag* fnCM_FB_CXXFL(int n);
CTag* fnCM_FB_CXXByPreq(int n);
CTag* fnCM_FB_GXAlgId(int n);
CTag* fnNotBindedCombo(int n);
CTag* fnCM_FB_CXXByPerm(int n);

