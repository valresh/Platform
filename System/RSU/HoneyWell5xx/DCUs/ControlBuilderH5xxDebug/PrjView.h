#pragma once
#include "ControlBuilderDoc.h"
#include <map>
#include <string>


// KPrjView view

class KPrjView : public CTreeView
{
	DECLARE_DYNCREATE(KPrjView)

protected:
	KPrjView();           // protected constructor used by dynamic creation
	virtual ~KPrjView();
  KControlBuilderDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
  CTreeCtrl* m_pCtrl;
  CImageList m_imgList;
  bool m_bInited;
  HTREEITEM m_itemFirstSelect;

  typedef std::map<std::string, HTREEITEM> tNamedTree_;
  tNamedTree_ m_FindNamedItem;
protected:
  void FillTree( HTREEITEM parent, SH_Module *module, int level, LPCSTR pszSelect );
  int GetImageIndex( HTREEITEM item, KBmBase *type );
  void ShowItem( HTREEITEM hItem );
	DECLARE_MESSAGE_MAP()
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
public:
  afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
};
