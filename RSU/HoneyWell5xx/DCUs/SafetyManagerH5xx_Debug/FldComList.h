#pragma once
#include "SafetyManagerDoc.h"

// KFldComList view

class KFldComList : public CListView
{
  CListCtrl* m_pCtrl;
  bool m_bInited;
	DECLARE_DYNCREATE(KFldComList)

protected:
  LFscBase::typeB m_SelectItemType;
protected:
	KFldComList();           // protected constructor used by dynamic creation
	virtual ~KFldComList();
  KSafetyManagerDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
  afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


