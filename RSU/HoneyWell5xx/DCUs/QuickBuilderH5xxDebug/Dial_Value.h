#pragma once
#include "resource.h"
#include <QbBridge2SysParam.h>
#include "GridCtrl.h"
#include <vector>

// CDial_Value dialog

class CDial_Value : public CDialog
{
  KGridCtrl m_List;
  CFont m_Font;
  typedef std::vector<SQbParam> tVarsCont_;
  tVarsCont_ m_Vars;

	DECLARE_DYNAMIC(CDial_Value)

public:
	CDial_Value(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDial_Value();
  void SetVarList( int nCount, SQbParam *pVars );

// Dialog Data
	enum { IDD = IDD_DIAL_VALUE };

protected:
  void FillList();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
  afx_msg void OnDestroy();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
};
