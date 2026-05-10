#pragma once
#include "TexChild.h"
#include "TexMini.h"

class CTexnoFrm : public CFrameWnd
  {
  DECLARE_DYNAMIC(CTexnoFrm)
  protected:
    bool m_bViewMini;
    HWND& m_hTouchWnd;
    CTexChild m_Scheme;
    CTexMini  m_Mini;
    enum{ SplitterHight = 3};
  public:
    CTexnoFrm( HWND& hTouchWnd, int nMonitor );
    virtual ~CTexnoFrm();

    CString String( const char* name );
    bool Correlate( HWND hWnd, bool bEmpty );
    void GoToSchem( LPCTSTR pszSchem );
    const char* GoToSchem( HXML nItem );
    void ShowOrHide( int nCmdShow );
    virtual void MainTimer();
    void SetSizeChild();
    void ShowOrHideMini();
    // Виртуальные методы
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    //
    void EventQueueMsg(WPARAM wParam,LPARAM lParam);
  protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
  };

inline void CTexnoFrm::MainTimer()
  {
  if ( ::IsWindow(m_hWnd) && ::IsWindowVisible(m_hWnd) )
    {
    m_Scheme.MainTimer();
	  m_Mini  .MainTimer();
    }
  }
