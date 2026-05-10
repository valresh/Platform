// #pragma once
// //
// #include "ViewMnemo.h"
// #include "UndoRedo.h"
// //
// #include "HTML.h"
// class CAmmFrm;
// // CMnemoFrm window

// class CMnemoFrm : public CViewMnemo
//   {
//   HWND& m_hTouchWnd;
//   //
//   POINT m_ptMouse;
//   int  m_nShowRevim;
//   void ShowWorkValue( int n );
//   HGLOBAL m_hGlobal;
//   //
//   bool m_bPriorDisp;
//   //
//   public:
//   SUniUndo mUndo;
//     CMnemoFrm(HWND& hTouchWnd,KHoneyPipeClient& rClient);
//     virtual ~CMnemoFrm();
//     //
//   public:
//     virtual bool DrawValue( HDC hDC, RECT& rect );
//     virtual void BuildQueue( LPCTSTR pszName, LPCTSTR pszTagGrp );
// 	  virtual void MainTimer(ULONG dwTimer);
//   public:
//     void EventPanel( LPCTSTR pszFile, LPCTSTR pszText, bool bAddUndoRedo );
//     void CommentRec(const char* pszDir);
//   protected:
//     afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//     DECLARE_MESSAGE_MAP()
//     //
//     afx_msg void OnValueWork1();
//     afx_msg void OnValueWork2();
//     afx_msg void OnValueWork3();
//     afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//   public:
//     afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//     afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//     afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//     afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//     void PageRedo();
//     void PageUndo();
//     void DispUndo();
//     void DispRedo();
//     void PriorDsp();
//   };
