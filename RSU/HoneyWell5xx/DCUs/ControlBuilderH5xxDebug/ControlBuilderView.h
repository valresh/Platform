#pragma once
#include "ControlBuilderDoc.h"
#include "BlkDraw.h"

// KControlBuilderView view

class KControlBuilderView : public CScrollView
{
	DECLARE_DYNCREATE(KControlBuilderView)
protected:
  typedef std::vector<KBlkDraw> tBlkDrawCont_;
protected:
	KControlBuilderView();           // protected constructor used by dynamic creation
	virtual ~KControlBuilderView();
  KControlBuilderDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  void SetScrollSizes( SIZE size);
  void ChangeScale( BOOL bInc );
  void Draw( CDC& dc );
  void DrawBlock( CDC& dc, KBlkDraw &blk );

	DECLARE_MESSAGE_MAP()
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
protected:
  CFont m_fontName, m_fontType, m_fontParams, m_fontRefM;

  CPoint m_ptMouse;
  double m_fScale;
  HBITMAP m_hBitmap;
  tBlockPtr_ m_module;
  CSize m_curSize;

  bool m_bFirstDraw;
  tBlkDrawCont_ m_blks;

  struct SCon
  {
    SCon() : bInput( false )
    {
      coord.SetRectEmpty();
    }
    std::string myName;
    std::vector<std::string> otherNames;
    CRect coord;
    bool bInput;
    bool operator ()( const SCon &thr )
    {
      if( myName==thr.myName && bInput==thr.bInput )
        return true;
      return false;
    }
  };

  typedef std::vector<SCon> tAllConnects_;//возможные соединения
  tAllConnects_ m_probConnects;
};
