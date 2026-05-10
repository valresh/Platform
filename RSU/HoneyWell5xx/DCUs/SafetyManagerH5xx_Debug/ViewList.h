#pragma once
#include "SafetyManagerDoc.h"

// CViewList view

class CViewList : public CTreeView
{
  CTreeCtrl* m_pCtrl;
  CImageList m_imgList;
  UINT m_CurSelected;

  void Recursive( HTREEITEM hRoot, UINT N, LFscFile* mFile, UINT nCount );
	DECLARE_DYNCREATE(CViewList)

protected:
	CViewList();           // protected constructor used by dynamic creation
	virtual ~CViewList();
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
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
  afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
public:
};
