#pragma once

#include "TexnoWnd/TexnoFrm.h"
#include "TouchKbd.h"

class CSxemaFrm : public CTexnoFrm
  {
  bool m_bLoad;
  int m_nNumber;
  public:
    CSxemaFrm(HWND& hTouchWnd,int nMonitor);
   ~CSxemaFrm(void);
    //
    BOOL MainCreate( CWnd* pParentWnd );
    //
    void GotoOper();
    void GotoPole( const char* name, int nCorrelate );
    bool Correlates( const char* name );
    BOOL IsShowSxeme();
	int GetMonitor(){ return m_nNumber; }
  protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void PostNcDestroy();
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  };
