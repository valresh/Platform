#pragma once
//#include <CJFrameWnd.h>
//#include <CJTabCtrlBar.h>
#include "SafetyManagerDoc.h"

// KSafetyManagerFrame frame

class KSafetyManagerFrame : public CFrameWnd
{
  KSafetyManagerDoc *m_pDocument;
	DECLARE_DYNCREATE(KSafetyManagerFrame)
protected:
	KSafetyManagerFrame();           // protected constructor used by dynamic creation
	virtual ~KSafetyManagerFrame();

  CSplitterWnd	m_wndSplitter;
  CTabCtrl m_wndWorkspace;
  //CCJTabCtrlBar		m_wndWorkspace;
  CStatusBar  m_wndStatusBar;
protected:
  CString FormText();
	DECLARE_MESSAGE_MAP()
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnUpdateUI(CCmdUI *pCmdUI);
  afx_msg void OnPowerUpValue();
public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
};


