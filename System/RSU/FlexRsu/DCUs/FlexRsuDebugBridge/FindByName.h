#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "FlexDoc.h"


// KFindByName dialog

class KFindByName : public CDialog
{
  KFlexDoc *m_pDocument;
	DECLARE_DYNAMIC(KFindByName)

  bool FiltrName( LPCSTR Filtr, LPCSTR Name );
  void InsertItem( LPCSTR pszName, LPCSTR pszType, LPSTR pszPath );
  void ReadStr();
  void SaveStr();
  CString m_findString;
  void Count( void *entry, DWORD &C );
public:
	KFindByName( KFlexDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~KFindByName();

// Dialog Data
	enum { IDD = IDD_KFINDBYNAME };
  CString m_ContourFileName;
protected:
  CComboBox m_Mask;
  CListCtrl m_List;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  afx_msg void OnBnClickedFind();
  afx_msg void OnNMDblclkReport(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg LRESULT OnEnum( WPARAM wParam, LPARAM lParam );
  virtual BOOL OnInitDialog();
  virtual void OnOK();
};
