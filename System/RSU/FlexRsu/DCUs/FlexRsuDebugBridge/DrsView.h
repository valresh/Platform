#pragma once
#include "FlexDoc.h"
#include <macros/INotifier.h>
#include "OpenInGraph.h"

// KDrsView view

class KDrsView : public CListView, KOpenInGraph
{
  CListCtrl* m_pCtrl;
  INotifier* m_NotifierSelectedDR;
  std::vector<INotifier::CONNECTION> m_connections;
  bool m_bSelectFromMe;

	DECLARE_DYNCREATE(KDrsView)

protected:
	KDrsView();           // protected constructor used by dynamic creation
	virtual ~KDrsView();
  KFlexDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
  void OnSelectedDR( LPSTR pszContourFileName );
	DECLARE_MESSAGE_MAP()
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
  afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};
