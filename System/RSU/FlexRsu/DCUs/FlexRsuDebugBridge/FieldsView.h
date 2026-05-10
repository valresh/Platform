#pragma once
#include "TreeListView.h"
#include "FlexDoc.h"
#include <macros/INotifier.h>
#include <map>

// KFieldsView view

class KFieldsView : public CTreeListView
{
  std::vector<INotifier::CONNECTION> m_connections;
  struct SFieldInfo
  {
    BYTE *pmem;
    eVarType type;
    tVarSizeType_ length;
  };
  typedef std::map<HTREEITEM, SFieldInfo> tContainer_;
  tContainer_ m_filledItems;
	DECLARE_DYNCREATE(KFieldsView)

protected:
	KFieldsView();           // protected constructor used by dynamic creation
	virtual ~KFieldsView();
  KFlexDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
  void OnSelectedDR( LPSTR pszFileName );
  void OnSelectedBlock( KFlexBlockBase *pBlock );
  void OnField( int &add, LPCSTR pszName, BYTE *pMem, eVarType type, tVarSizeType_ varSize );
  virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
protected:
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg LRESULT OnWmUser( WPARAM wParam, LPARAM lParam );
  afx_msg void OnTimer(UINT_PTR nIDEvent);
};


