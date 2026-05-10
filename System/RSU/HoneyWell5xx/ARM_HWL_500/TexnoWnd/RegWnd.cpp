// RegDlg.cpp : implementation file
#include "stdafx.h"
#include "../ARM.h"
#include "RegWnd.h"
#if 0
//-----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CValEdit,CEdit)
BEGIN_MESSAGE_MAP(CValEdit,CEdit)
  ON_WM_KEYDOWN()
END_MESSAGE_MAP()
void CValEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
  {
  if ( VK_RETURN == nChar )
    {
    CWnd* pOwner = GetOwner();
    NMHDR nmhdr;
    ASSERT_VALID(pOwner);
    nmhdr.code = NM_RETURN;
    nmhdr.idFrom =  GetDlgCtrlID();
    nmhdr.hwndFrom = m_hWnd;
    pOwner->SendMessage(WM_NOTIFY,(WPARAM)0,(LPARAM)&nmhdr);
    }
  CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CRegWnd,CBaseWnd)

CRegWnd::CRegWnd(KSmartValueTypeDV eSxema)
: CBaseWnd(id_Reg,eSxema)
, m_eInputType(no_input)
, m_h16(NULL)
, m_h18(NULL)
  {
  }
//
CRegWnd::~CRegWnd()
  {
  if ( m_h16 ) ::DeleteObject(m_h16);
  if ( m_h18 ) ::DeleteObject(m_h18);
  }

BEGIN_MESSAGE_MAP( CRegWnd, CBaseWnd )
  ON_WM_PAINT()
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_NOTIFY(NM_RETURN, IDC_EDIT_VAL, OnReturn)
	ON_BN_CLICKED(IDC_BUTTON_SP, OnSP)
	ON_BN_CLICKED(IDC_BUTTON_OP, OnOP)
	ON_BN_CLICKED(IDC_RADIO_MAN, OnMAN)
	ON_BN_CLICKED(IDC_RADIO_AUTO,OnAUTO)
	ON_BN_CLICKED(IDC_RADIO_CAS, OnCAS)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_OP, OnSpinOP)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SP, OnSpinSP)
END_MESSAGE_MAP()

void CRegWnd::DoDataExchange(CDataExchange* pDX)
  {
  CBaseWnd::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_VAL, m_wndEdit);
  DDX_Control(pDX, IDC_STATIC_PV, m_wndPV);
  DDX_Control(pDX, IDC_STATIC_SP, m_wndSP);
  DDX_Control(pDX, IDC_STATIC_OP, m_wndOP);
  DDX_Control(pDX, IDC_BUTTON_SP, m_btnSP);
  DDX_Control(pDX, IDC_BUTTON_OP, m_btnOP);
  }

BOOL CRegWnd::OnInitDialog()
  {
  CBaseWnd::OnInitDialog();
  //
  m_wndEdit.SetOwner(this);
  //
  CMyFonts font(0,0);
  font.GiveCourier( 18 ); m_h18 = font.Clear();
  font.GiveCourier( 16 ); m_h16 = font.Clear();
  //
  HWND hWnd = ::GetWindow( m_hWnd, GW_CHILD );
  while ( ::IsWindow( hWnd ) )
    {
    int id = ::GetDlgCtrlID( hWnd );
    switch ( id )
      {
      case IDC_EDIT_VAL :
      case IDC_STATIC_PV:
      case IDC_STATIC_SP:
      case IDC_STATIC_OP:
        GetDlgItem(id)->SetFont( CFont::FromHandle( m_h18 ) );
      break;
      default:
        GetDlgItem(id)->SetFont( CFont::FromHandle( m_h16 ) );
      break;
      }
    hWnd = ::GetNextWindow(hWnd,GW_HWNDNEXT);
    }
  //
  //m_spinSP.SetRange(0, 100);
  //m_spinOP.SetRange(MIN_OP, MAX_OP);
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
  }

bool CRegWnd::MyCreate( CWnd* pParent, bool bAviFors )
  {
  //
  BOOL bRes = Create( IDD_REGULATOR, pParent );
  if ( !bRes ) return false;
  //
  SetOwner( pParent );
  //
  m_nNumber = CPipeClient::ms_pMainPipe->FindObj(m_eType,m_strName,m_strName);
  CPipeClient::ms_pMainPipe->End();
  //
  SetWindowText( m_strName );
  CRect rect;
  GetWindowRect( rect );
  POINT point = GetPointWork( *pParent, rect.Size() );
  SetWindowPos( NULL, point.x, point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE );
  //
  mPV.Init( m_nNumber, id_Reg, "PV"   );
  mSP.Init( m_nNumber, id_Reg, "SP"   );
  mOP.Init( m_nNumber, id_Reg, "OP"   );
  mMD.Init( m_nNumber, id_Reg, "Мода" );
  CPipeClient::ms_pMainPipe->Refresh();
  LoadProps();
  //
  return true;
  }

// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CRegWnd::Specific( int& nDialog )
  {
  // CPipeClient::ms_pMainPipe->GetData( m_eType, m_strName, this );
  }

/*virtual*/ void CRegWnd::LoadProps()
  {
  Refresh( this );
  char text[512];
  text[0] = 0;
  CPipeClient::ms_pMainPipe->FillValue( text, mPV, -3,  NULL, true );
  m_wndPV.SetWindowText(text);
  text[0] = 0;
  CPipeClient::ms_pMainPipe->FillValue( text, mSP, -3,  NULL, true );
  m_wndSP.SetWindowText(text);
  text[0] = 0;
  CPipeClient::ms_pMainPipe->FillValue( text, mOP,  1,  NULL, true );
  m_wndOP.SetWindowText(text);
  //
	int nMode = -1;
  BYTE val = CPipeClient::ms_pMainPipe->ValueB( mMD );
  switch ( val )
    {
    case CReg::Man    :nMode = 0;break;
    case CReg::Auto   :nMode = 1;break;
    case CReg::Cascade:nMode = 2;break;
    }
  if ( nMode != -1 )
    {
    CheckRadioButton( IDC_RADIO_MAN, IDC_RADIO_CAS, IDC_RADIO_MAN+nMode );
    m_btnSP.EnableWindow( nMode == 1 );
    m_btnOP.EnableWindow( nMode == 0 );
    }
  }

void CRegWnd::OnPaint()
  {
  CPaintDC dc(this); // device context for painting
  RECT rect;
  GetClientRect( &rect );
  CMyBrush brush( dc, ::GetSysColor(COLOR_BTNFACE) );
  brush.Rect( rect );
  }

void CRegWnd::OnSP()
  {
  if (m_eInputType == sp_input)
    EndEdit();
  else
    BeginEdit(sp_input);
  }

void CRegWnd::OnOP()
  {
  if (m_eInputType == op_input)
    EndEdit();
  else
    BeginEdit(op_input);
  }

void CRegWnd::OnSpinSP(NMHDR *pNMHDR, LRESULT *pResult)
  {
  //LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  //double dDelta = pNMUpDown->iDelta*STEP_SIZE;
  //IncSP(dDelta);
  *pResult = 0;
  }

void CRegWnd::OnSpinOP(NMHDR *pNMHDR, LRESULT *pResult)
  {
  //LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  //double dDelta = pNMUpDown->iDelta*STEP_SIZE;
  //IncOP(dDelta);
  *pResult = 0;
  }

void CRegWnd::OnMAN (){SetMode(CReg::Man    );}
void CRegWnd::OnAUTO(){SetMode(CReg::Auto   );}
void CRegWnd::OnCAS (){SetMode(CReg::Cascade);}
void CRegWnd::SetMode(CReg::eMode mode)
  {
  SSendToModel send( mMD.def );
  BYTE Old = CPipeClient::ms_pMainPipe->ValueB( mMD );
  BYTE New = mode;
  send.Set( &Old, &New );
  CPipeClient::ms_pMainPipe->SendData( m_nNumber, send, true );
  }

bool CRegWnd::SetSP(double dVal)
  {
  SSendToModel send( mSP.def );
  double dOld = CPipeClient::ms_pMainPipe->ValueD( mSP );
  send.Set( &dOld, &dVal );
  CPipeClient::ms_pMainPipe->SendData( m_nNumber, send, true );
  return true;
  }

bool CRegWnd::SetOP(double dVal)
  {
  SSendToModel send( mOP.def );
  double dOld = CPipeClient::ms_pMainPipe->ValueD( mOP );
  send.Set( &dOld, &dVal );
  CPipeClient::ms_pMainPipe->SendData( m_nNumber, send, true );
  return true;
  }

void CRegWnd::OnReturn(NMHDR*, LRESULT *)
  {
  CString str;
  m_wndEdit.GetWindowText(str);
  if ( IsFlt( str ) )
    {
    double dVal = atof(str);
    switch(m_eInputType)
      {
      case sp_input:
      if( SetSP(dVal) )
        EndEdit();
      break;
      case op_input:
      if ( SetOP(dVal) )
        EndEdit();
      }
    }
  else
    {
    AfxMessageBox( _T("Ошибка ввода."), MB_ICONEXCLAMATION );
    m_wndEdit.SetFocus();
    }
  }

void CRegWnd::BeginEdit(EInputType eInputType)
  {
  ASSERT(m_eInputType == no_input);
  CRect	rect(0,0,0,0);
  char text[512] = "";
	switch( eInputType )
    {
    case sp_input:
      m_wndSP.GetWindowRect(rect);
      m_wndSP.ShowWindow(SW_HIDE);
    //m_spinSP.ShowWindow(SW_HIDE);
      CPipeClient::ms_pMainPipe->FillValue( text, mSP, -3,  NULL, true );
    break;
    case op_input:
      m_wndOP.GetWindowRect(rect);
      m_wndOP.ShowWindow(SW_HIDE);
    //m_spinOP.ShowWindow(SW_HIDE);
      CPipeClient::ms_pMainPipe->FillValue( text, mOP,  1,  NULL, true );
    break;
    default:
    ASSERT(false);
    }
  //
  ScreenToClient(rect);
  m_wndEdit.SetWindowPos( &wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW );
  m_wndEdit.SetWindowText(text);
  m_wndEdit.SetFocus();
  m_eInputType = eInputType;
  }

void CRegWnd::EndEdit()
  {
  ASSERT(m_eInputType != no_input);
	m_wndEdit.ShowWindow(SW_HIDE);
  switch(m_eInputType)
    {
    case sp_input:
      m_wndSP.ShowWindow(SW_SHOW);
    //m_spinSP.ShowWindow(SW_SHOW);
    break;
    case op_input:
      m_wndOP.ShowWindow(SW_SHOW);
    //m_spinOP.ShowWindow(SW_SHOW);
    break;
    }
  m_eInputType = no_input;
  }

void CRegWnd::OnOK()
  {
  if ( m_eInputType != no_input )
    OnReturn(NULL,NULL);
//CDialog::OnOK();
//DestroyWindow();
  }

void CRegWnd::OnKickIdle()
  {
  }
//-----------------------------------------------------------------------------
#endif
