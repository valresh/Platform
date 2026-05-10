// // AplicFrm.h : interface of the CAmmFrm class
// #pragma once

// #include "SxemaFrm.h"
// #include "MnemoFrm.h"
// #include "MyStatusBar.h"
// #include "ButtonFrm.h"
// #include "ViewAlarm.h"

// class CBlankView;
// class CAmmFrm : public CButtonFrm
//   {
//   DECLARE_DYNAMIC(CAmmFrm)
//   bool* m_pGotoPole;
//   int m_nPosition;
//   CAplicFrm* m_pParent;
//   protected:
//     KHoneyPipeClient client;
//     bool m_bNoPCYWork;// Отказ РСУ
//     CBlankView* m_pBlankView;// Чёрное окно при отказе РСУ
//     CSxemaFrm   m_wndSxema;  // Окно технологической схемы
//     CAmmFrm*    m_pAmmFrame;
//     CViewAlarm* m_pViewAlarm;
//   public:
//     CMnemoFrm   m_wndMnemo;  // Окно мнемосхемы
//     CAmmFrm( int nMonitor, HWND& hMnemo, HWND& hSxema, int nPosition = -1);
//     virtual ~CAmmFrm();

//     CViewMnemo* GetMnemo() { return &m_wndMnemo; };
// 	virtual bool IsMnemo(HWND hWnd){ return m_wndMnemo.m_hWnd == hWnd; };
// 	int GetMonitor(){ return m_wndSxema.GetMonitor(); }
//   // Attributes
//   public:
//     bool IsShowSxeme();
//     virtual void MainTimer( DWORD dwTimer );
//     virtual BOOL MyCreate( CAplicFrm* pParentWnd, LPRECT rcApp = NULL );
//     void Forsunka(WPARAM wParam,LPARAM lParam);
//   // Overrides
//   public:
//     virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//     virtual void PostNcDestroy();
//     virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
//     virtual void RecalcLayout(BOOL bNotify = TRUE);
//   public:
//   // Generated message map functions
//   DECLARE_MESSAGE_MAP()
//   protected:
//     afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
//     afx_msg void OnSetFocus(CWnd *pOldWnd);
//     afx_msg UINT OnGetDlgCode();
//   public:
//     void GotoPole(int nCorrelate);
//     void GotoOper(int nCorrelate);
//     void ShowOrHideMini();
//   protected:
//     void ChangePCY(bool bSet);
//   private:
//     CWnd* GetActive();
//     inline CView* GetTop()
//       {
//       return m_bNoPCYWork ? (CView*)m_pBlankView : (CView*)&m_wndMnemo;
//       }
//     afx_msg void OnClose();
//   public:
// 	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
// 	void SetNeighbourFrame(CAmmFrm* pNeighFrame){m_pAmmFrame=pNeighFrame;}
// 	virtual void ZoomMnemo(int nChoise);
//   void PageRedo();
//   void PageUndo();
//   void DispUndo();
//   void DispRedo();
//   void PriorDsp();
//   virtual void GoToPage(LPCTSTR szName,LPCTSTR szTagGrp);
//   void GoToMiniTrend(LPCTSTR szName, LPCTSTR szTag);
//   private:
//     afx_msg BOOL OnNcActivate(BOOL bActive);
//   };
