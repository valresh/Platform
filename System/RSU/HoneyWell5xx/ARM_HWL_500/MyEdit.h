#pragma once
//#include "afxwin.h"
#include "HoneyMnemoExtend.h"
#include "../linuxTypes/atltypes.h"

class CMyEdit
    //: public CEdit
  {
//  DECLARE_DYNAMIC(CMyEdit)
  public:
  // DECLARE_MESSAGE_MAP()
  // afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  };
//
// Смысл - потерять фокус при нажатии на Enter
class CMaEdit
      //: public CEdit
  {
  public:
  //BOOL MyCreate( CWnd* pParentWnd, UINT nID, DWORD dwStyle );
  BOOL MyCreate( UINT nID, DWORD dwStyle );
  //void MyActive( CWnd* pWnd, int nItem, int nSubItem, const char* szText, CRect rc, CWnd* pBtn );
  void MyActive(int nItem, int nSubItem, const char* szText, CRect rc);
  int m_nItem;
  int m_nSubItem;
  //
  //DECLARE_MESSAGE_MAP()
  void OnSelectPoint();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  UINT OnGetDlgCode();
  void OnReturn();
  };
//
// Смысл - потерять фокус при нажатии на Enter
class CMaCombo
      //: public CComboBox
  {
  static char* mList[];
  static int  m_nCount;
  public:
  //BOOL MyCreate( CWnd* pParentWnd, UINT nID );
  BOOL MyCreate( UINT nID );

  //void MyActive( CWnd* pWnd, int nItem, int nSubItem, const char* szText, CRect rc );
  void MyActive( int nItem, int nSubItem, const char* szText, CRect rc );

  int m_nItem;
  int m_nSubItem;
  void Fill();
  //
 // DECLARE_MESSAGE_MAP()
	LRESULT OnReturn(WPARAM wParam, LPARAM lParam);
  public:
//    afx_msg void OnCbnSelchange();
  };

class CMyBmpButton
      //: public CBitmapButton
  {
  public:
    //bool MyCreate(LPCTSTR lpszCaption, DWORD dwStyle,
                //const RECT& rect, CWnd* pParentWnd, UINT nID);
      bool MyCreate(LPCTSTR lpszCaption, DWORD dwStyle,
                    const RECT& rect, UINT nID);
  };
//
// Смысл - самоуничтожиться при потере фокуса или при нажатии на Enter
class CTagEdit
      //: public CEdit
  {
  class CHTML& m_rHtml;
  SParamValueH  mVal;
  public:
    CTagEdit( CHTML& html )
    : m_rHtml(html)
      {
      };
  HWND MyCreate( HWND hWnd, class CTag* pTag, double fScale, POINT ptOffset );
  void Return();
  //
//  DECLARE_MESSAGE_MAP()
  protected:
    virtual void PostNcDestroy();
  public:
    // afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    // afx_msg UINT OnGetDlgCode();
    // afx_msg void OnEnKillfocus();
  };
