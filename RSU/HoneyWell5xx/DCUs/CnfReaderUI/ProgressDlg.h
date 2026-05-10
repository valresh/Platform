#pragma once
#include "resource.h"

// KProgressDlg dialog

class KProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(KProgressDlg)

public:
	KProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~KProgressDlg();
  void OnDestroy();

// Dialog Data
	enum { IDD = IDD_PROGRESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CProgressCtrl m_Progress;
  CStatic m_Txt;
};
