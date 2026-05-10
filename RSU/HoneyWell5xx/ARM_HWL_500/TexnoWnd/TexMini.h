#pragma once
#define IN
#define OUT
#include <atlimage.h>

// CTexMini
class CTexMini : public CWnd
  {
  DECLARE_DYNAMIC(CTexMini)
  bool m_bPresent;
  HWND m_hChild;
  double m_dScaleX,m_dScaleY;
  HWND& m_hTouchWnd;
  protected:
    CBitmap m_bmpMsk;
    CBitmap m_bmpWnd;
    CBitmap m_bmpMem;//Итоговая картинка на экране
    //
    CRect m_rectPict;
    bool  m_bPress;
    class CTexChild* m_pScheme;

    BOOL MouseMoveLoop(const POINT& pt);
    void BuildImg(CDC& dc, CRect& rect);
  public:
    CTexMini(CTexChild* pScheme,HWND& hTouchWnd);
    virtual ~CTexMini();
	HWND GetMnemoHWND(){return m_hChild;};

    void BuildBmp();
    void SendSchemePointF(const POINT& pt);
    void SetPresent( bool bSet );
    void MainTimer();
    int  SizeY();
  protected:
    DECLARE_MESSAGE_MAP()

  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnDestroy();
  };
