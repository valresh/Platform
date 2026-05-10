#pragma once

#define ID_TIMER_TRACKING	0xE000
#define AFX_CX_ANCHOR_BITMAP	32
#define AFX_CY_ANCHOR_BITMAP	32

class CAnchor
    //: public CWnd
  {
  private:
//    using CWnd::Create;
  public:
	  CAnchor(class CTexChild* pParent);
	  virtual ~CAnchor();

      bool Create();
      bool Anchord(CPoint* ptAnchor);
	  void SetBitmap(UINT nID);

	  CRect m_rectDrag;
	  CPoint m_ptAnchor;
	  BOOL m_bQuitTracking;
	  HCURSOR m_hAnchorCursor;

	  virtual BOOL PreTranslateMessage(MSG* pMsg);

	  afx_msg void OnPaint();
	  afx_msg void OnTimer(UINT_PTR nIDEvent);
	  DECLARE_MESSAGE_MAP()
    CTexChild* m_pTex;
  };
