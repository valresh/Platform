#pragma once
//
#include "Api.h"
#include "HTML/HTML.h"
class CAmmFrm;
// CViewMnemo window

class CViewMnemo
//: public CScrollView
{
    // HWND m_hChild;
    bool m_bChild;

    bool Build( LPCTSTR pszName, LPCTSTR szTagGrp,bool bResize);


protected:
    //
    int m_nTime;
    HBITMAP m_hStatic;
    HBITMAP m_hBitmap;
    // CToolTipCtrl  m_ToolTip;


    //
    CTag* m_pPressed;
    CTag* m_pBoundTag; //найденный для контекстного меню привязанный тег (если m_pPressed не найден)
    CTag* m_lastPressed;
    CTag* m_pSelectedTag; //найденный под курсорм тег (для event в скриптах)
    CTag* LButtonDown(POINT point,bool bDraw = false);
    CTag* FindBoundTag(POINT point); //поиск первого видимого тега привязанного к модельной точке
    CTag* TabIndexCur(POINT point,bool bDraw = false);
    int  FindTags(POINT point, std::vector<CTag*>& tags); //поиск всех видимых тегов (кроме "Page") под курсором
    bool RunPageScriptClick(POINT& point);
    //
    virtual void Draw( HDC hDC = NULL );

    void Graf( HDC hDC );
    bool m_bPaintStat;
    //
public:
    POINT Scroll( POINT pt, bool bScale );
    bool Build(const  std::string & htmlContent);
    CViewMnemo(KHoneyPipeClient& rClient,bool bChild = false);
  //  void Draw (IRenderContext RC);
    CViewMnemo();
    virtual ~CViewMnemo();
    //
    CHTML mDoc;
    FLOAT m_fScale;
    int   m_nScale;
    FLOAT GetScale( int nChoise );
    virtual void MainTimer(ULONG dwTimer);
    virtual void BuildQueue( LPCTSTR pszName, LPCTSTR pszTagGrp );
    virtual bool DrawValue( HDC hDC, RECT& rect );
    virtual bool Keyboard( WORD nCode, UINT nChar );
    // virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    // virtual void OnDraw(CDC* pDC);
protected:
    //	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //  virtual void PostNcDestroy();
public:
    const char* Faceplate( const char* pszName,const char * fp = 0, const char* pv = 0, const char * rst = 0, const char * desc = 0 );
    const char* FacePlate( CHTML& html, int nNumber );
protected:
    // afx_msg void OnPaint();
    // DECLARE_MESSAGE_MAP()
    //
public:
    //
    // afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    // afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    // afx_msg void OnKillFocus(CWnd* pNewWnd);
    // afx_msg void OnSetFocus(CWnd* pOldWnd);
    // afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    // afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    // afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    // afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    // afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    // afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    // afx_msg LRESULT OnXButtonDown(WPARAM wParam,LPARAM lParam);
    // afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    // afx_msg void OnAckOne();
    // afx_msg void OnDetail();
    // afx_msg void OnAssocs();
    // afx_msg void OnTrend();
    // afx_msg void OnMiniTrend();
    // afx_msg void OnGroup();
    // afx_msg void OnMan  ();
    // afx_msg void OnAuto ();
    // afx_msg void OnNorm ();
    // afx_msg void OnRaise();
    // afx_msg void OnLower();
    // afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    // afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT item);
    // afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT item);

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void Zoom(int nChoise);
    void SetSizes();
    void ClearPage();
    bool MnemoAck();
private:
    void CorrectFirstMonitorWindowSize();
};
