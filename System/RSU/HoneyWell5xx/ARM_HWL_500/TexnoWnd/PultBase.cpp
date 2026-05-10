#include "stdafx.h"
#include "../ARM.h"
#include "Queue.h"
#include "../ValEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
#define MY_CHILD WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CHILD

class CPultBase : public CDialog
  {
  DECLARE_DYNAMIC(CPultBase)
  public:
    CPultBase();
    HWND m_hPostM;
    //
    bool MyCreate( CWnd* pParent, const char* pszDlg, bool bPopup );
    CString m_strFile;
	CValEdit m_wndEdit;
	bool m_bNoExitAfterEnter;

    //
  protected:
	  DECLARE_MESSAGE_MAP()
    //
    virtual void PostNcDestroy();
	LRESULT OnPressQueue(WPARAM wParam,LPARAM lParam);
    //
    DWORD m_dwMainTimer;
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual void OnOK();
    virtual void OnCancel();
	void HideEdit();

  public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
  };
//
#define _MAX_PULT 32
static CPultBase* arrPult[_MAX_PULT];

void AddPult(CPultBase* pPult)
  {
  for ( int n = 0; n < _MAX_PULT; n++ )
  if ( arrPult[n] == NULL )
    {
    arrPult[n] = pPult;
    return;
    }
  ASSERT(0);
  }

void DelPult(CPultBase* pPult)
  {
  for ( int n = 0; n < _MAX_PULT; n++ )
  if ( arrPult[n] == pPult )
    {
    for ( int m = n; m < _MAX_PULT-1; m++ )
    arrPult[m] = arrPult[m+1];
    arrPult[_MAX_PULT-1] = NULL;
    return;
    }
  }

CPultBase* FndPult(const char* name)
  {
  for ( int n = 0; n < _MAX_PULT; n++ )
  if ( arrPult[n] )
  if ( lstrcmpi( arrPult[n]->m_strFile, name ) == 0 )
    {
    arrPult[n]->ShowWindow(SW_SHOW);
    arrPult[n]->SetForegroundWindow();
    return arrPult[n];
    }
  return NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HWND Pults( HWND hParent, HWND hPostM, const char* pszPult, bool bPopup )
  {
  CString strName(pszPult);
  strName.MakeUpper();
  if (strName=="PANORAM3D.EXE")
  {
	  theApp.StartPanoram3D();
	  return NULL;
  }
  CPultBase* pult = NULL;
  if ( bPopup )
    {
    pult = FndPult(pszPult);
    if ( pult ) 
      return pult->m_hWnd;
    }
  pult = new CPultBase();
  if ( pult != NULL )
    {
    pult->m_hPostM = hPostM;
    if ( !pult->MyCreate( CWnd::FromHandle(hParent), pszPult, bPopup ) )
      {
      delete pult;
      return NULL;
      }
    //
    pult->ShowWindow(SW_SHOW);
    pult->SetForegroundWindow();
    return pult->m_hWnd;
    }
  return NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CPultBase
IMPLEMENT_DYNAMIC(CPultBase,CDialog)
CPultBase::CPultBase()
: m_dwMainTimer(0)
, m_bNoExitAfterEnter(false)
  {
  }

BEGIN_MESSAGE_MAP( CPultBase, CDialog )
  ON_WM_TIMER()
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_REGISTERED_MESSAGE(nPressQueue,OnPressQueue)
END_MESSAGE_MAP()
//
LRESULT CPultBase::OnPressQueue(WPARAM wParam,LPARAM lParam)
  {
  UINT nEvent = LOWORD(wParam);
  UINT nParam = HIWORD(wParam);
  switch ( nEvent )
    {
  case QUEUE_EDIT_W:
    // Получает RECT окна Edit
		  {
			  CRect rc;
			  rc=*(RECT*)lParam;
			  if (::IsWindow(m_wndEdit))
			  {
					m_wndEdit.SetWindowRect(rc);
					return (LRESULT)m_wndEdit.m_hWnd;
			  }
			  else
				  return NULL;
		  }
		  break;
  case QUEUE_HIDEEDIT:
		HideEdit();
		break;
    }
  return 0;
  }

void CPultBase::HideEdit()
{
		  if (::IsWindow(m_wndEdit)&&::IsWindowVisible(m_wndEdit))
			  m_wndEdit.SendPointValue();
}

void CPultBase::PostNcDestroy()
  {
  if ( ::IsWindow( m_hPostM ) )
  ::PostMessage( m_hPostM, WM_COMMAND, 999, 0 );//Возврат в диалог откуда пришли
  DelPult(this);
  delete this;
  }

bool CPultBase::MyCreate( CWnd* pParent, const char* pszDlg, bool bPopup )
  {
  //
  if ( bPopup )
    {
    DWORD dwStyle = WS_SYSMENU | WS_CAPTION | WS_POPUP
                | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_THICKFRAME;
    DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE;
    //
    CRect rect(0,0,0,0);
    BOOL bRes = CreateEx( dwExStyle, CLASS_MNEMO, NULL, dwStyle, rect, pParent, 0 );
    if ( !bRes ) return false;
    }
  else
    {
    DWORD dwStyle = MY_CHILD | WS_MAXIMIZE;
    BOOL bRes = CreateEx( 0, CLASS_MNEMO, NULL, dwStyle, 0,0,0,0, pParent->m_hWnd, HMENU(AFX_IDW_PANE_FIRST) );
    if ( !bRes ) return false;
    }
  //
  m_strFile = pszDlg;
  //
  SetOwner( pParent );
  //
  ::SetMessageWnd( m_hWnd, pParent->m_hWnd, Version==AM2_NAK ); //последний параметр true когда пульт вызывается из пульта
  ::SendMessage  ( m_hWnd, nBuildQueue, BUILD_MNEMO, (LPARAM)(LPCTSTR)pszDlg );

  //
  if ( bPopup )
    {
    SetTimer( 1000, 1000, NULL );
    AddPult(this);
	if (lstrcmp(pszDlg,"oknoVibora")==0)
	{
		  ::SetMessageWnd( m_hWnd, this->m_hWnd );
		  m_wndEdit.MyCreate(this, IDC_MNEMOEDIT );
		  m_wndEdit.ShowWindow(SW_HIDE);
		  m_bNoExitAfterEnter=true;
	}
	if ((Version==AM2_NAK)&&
		( (lstrcmp(pszDlg,"101-J_2_MAN")==0)||(lstrcmp(pszDlg,"102-J_MAN")==0)))
	{
		  ::SetMessageWnd( m_hWnd, this->m_hWnd );
		  m_wndEdit.MyCreate(this, IDC_MNEMOEDIT );
		  m_wndEdit.ShowWindow(SW_HIDE);
		  m_bNoExitAfterEnter=true;
	}
    }
  //
  return true;
  }

void CPultBase::OnTimer(UINT_PTR nIDEvent)
  {
  ::MainTimer( m_hWnd, m_dwMainTimer++ );
  CDialog::OnTimer(nIDEvent);
  }

void CPultBase::OnPaint()
  {
  CPaintDC dc(this); // device context for painting
  PaintDlg( m_hWnd, dc, 0 );
  }

BOOL CPultBase::OnEraseBkgnd(CDC* pDC)
  {
  return FALSE;//CDialog::OnEraseBkgnd(pDC);
  }

void CPultBase::OnOK()
  {
	  if(m_bNoExitAfterEnter)
	  {
		  if (::IsWindow(m_wndEdit)&&::IsWindowVisible(m_wndEdit))
			  m_wndEdit.SendPointValue();
		  return;
	  }
  CDialog::OnOK();
  DestroyWindow();
  }

void CPultBase::OnCancel()
  {
	  if(m_bNoExitAfterEnter)
	  {
		  HideEdit();
		  return;
	  }
  CDialog::OnCancel();
  DestroyWindow();
  }
