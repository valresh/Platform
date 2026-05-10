#pragma once
#include "Anchor.h"
#include "ArrayObj.h"
#include "SoftGDI.h"

struct Addr
{
//	CSocketClient *pSockClient;
	SUniTags *pSTagsObj;
	SUniChar *pSUniChar;
	HANDLE hEvent;
};
//
class CTexChild : public CWnd, public CArrayObj
  {
    friend class CAnchor;
    friend class CTexMini;
    friend class CTexnoFrm;
    bool m_bPaintStatic;
    void Select( HDC hDC );
  protected:
	  class CTexMini* m_pMini;
//	  CToolTipCtrl  m_ToolTip;
    CAnchor m_Anchor;
    HWND& m_hParentUSB;
  public:
	  CTexChild(CTexMini* pMini,HWND& hTouchWnd,int nMonitor);
	  virtual ~CTexChild();

    const char* LoadData( HXML nItem );
    void LoadData( LPCTSTR pszSchem, LPCTSTR pszGoto = NULL );
    void ShowOrHide(int nCmdShow);
    void AnchorScroll(LPSIZE sz);
    void MainTimer();
	void GoToPanoram3D(CString strFind);
    static void HideDialog();
  protected:
	  DECLARE_MESSAGE_MAP()

	  int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	  LRESULT HandleMButtonDown2(WPARAM wParam, LPARAM lParam);
    void MenuContext(UINT nID);
  public:
	  virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
	//CSocketClient sockClient;
    CSocketClient *m_psockClient;
    //
    int GetScale(){ return m_nScale; };
    SIZE GetSizeEmf()
      {
      SIZE sz = { EmfW(m_hEmf), EmfH(m_hEmf) };
      return sz;
      };
    HMyEMF GetEmf() { return m_hEmf; }
    const char*  GetName() { return m_strName; }
    void GetRect(LPRECT rc);
    void SetPointToCenter(LPPOINT pt);
    void SetObjToCenter(int nIndex);
    //
    const char* SetActive(const char* pszModel);
  protected:
    void SaveScale();
    void RestScale(LPCTSTR pszGoto);
    int  LimitShift( int* dx, int* dy, bool b1 = true );
    int  MinScale(int n = 0);
    int  SetScale(int nScale);
    void Paint(CDC& dc);
    int  FindShape(LPPOINT lpPt = NULL);
    void DrawMenuItem(HDC hDC,char* name,RECT& rect,DWORD dwFlags);
    void PrintSxema();
    void CallDialog(const char* name);
	void ControlTexMini(const char* pszName );
    //
    class 
    CBaseWnd*  BaseWnd( EDataTypes eType, UINT eSxema, const char* pszName, const char* pszComm );
    void  CreateObject( EDataTypes eType, UINT eSxema, const char* pszName, const char* pszComm,bool bAviFors=false );
    class CBaseWnd* FindItem(EDataTypes eType, const char* pszName,CWnd* pWnd);
    class CBaseWnd* FindItem(EDataTypes eType, const char* pszName,bool bAviFors=false);
    class CBaseWnd* GiveZadv(CBaseWnd* pObj);
  private:
	  CString GetKlapanName(CString strName);
	  void RefreshModelData();
    //
  private: // Для отображения
    int    m_nScale;
    CPoint m_ptOffset;
    int    m_nPressed;
    CPoint m_ptMouse;
    HMyEMF m_hEmf;
    //
    HBITMAP m_hBmpMeta;
    HBITMAP m_hBmpStat;
    CString m_strName;
    int m_nSelect;
    CRect m_rcSelect;
	Addr addr;
    HANDLE  m_hTread;

    HMENU ScaleMenu( HMENU hMenu );
  public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg LRESULT OnEventQueueMsg(WPARAM wParam,LPARAM lParam);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  };
