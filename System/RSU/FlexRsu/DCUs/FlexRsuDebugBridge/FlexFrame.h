#pragma once
#include "FlexDoc.h"


// KFlexFrame frame

class KFlexFrame : public CFrameWnd
{
  INotifier* m_NotifierSelectedDR;
  INotifier* m_NotifierScroll2Block;

  DWORD m_nInitCounter;
  KFlexDoc *m_pDocument;
	DECLARE_DYNCREATE(KFlexFrame)
protected:
	KFlexFrame();           // protected constructor used by dynamic creation
	virtual ~KFlexFrame();

protected:
  CStatusBar  m_wndStatusBar;
  CSplitterWnd m_wndSplitter, m_wndSplitter2;
protected:
	DECLARE_MESSAGE_MAP()
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnClose();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
public:
  afx_msg void OnSimConnection();
  afx_msg void OnUpdateSimConnection(CCmdUI *pCmdUI);
  afx_msg void OnFind();
};
