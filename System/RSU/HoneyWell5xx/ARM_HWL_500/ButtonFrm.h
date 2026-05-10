#pragma once

class CButtonFrm : public CFrameWnd
  {
  static CBitmap bmpGalka[3];
  static CBitmap bmpZamok[3];
	DECLARE_DYNAMIC(CButtonFrm)
public:
	CButtonFrm( int nMonitor, bool m_bGrey );
	~CButtonFrm(void);

  virtual class CViewMnemo* GetMnemo() { return NULL; }
  virtual void PostNcDestroy();
  virtual BOOL Create(
    LPCTSTR lpszClassName,
    LPCTSTR lpszWindowName,
    DWORD dwStyle = WS_OVERLAPPEDWINDOW,
    const RECT& rect = rectDefault,
    CWnd* pParentWnd = NULL,
    LPCTSTR lpszMenuName = NULL,
    DWORD dwExStyle = 0,
    CCreateContext* pContext = NULL);

	DECLARE_MESSAGE_MAP()
public:
	void PaintExtButton();
	afx_msg void OnNcPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CRect ZamokButton(bool bFromNCLBDown=false);
	CRect GalkaButton(bool bFromNCLBDown=false);
	bool m_bZamokOn;
	bool m_bGalkaOn;
	bool m_bNotActive;
	bool m_bClose;
  int  m_nMonitor;
  };
