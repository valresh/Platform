#include "stdafx.h"
#include "ARM.h"
#include "AplicFrm.h"
#include "SxemaFrm.h"

CSxemaFrm::CSxemaFrm(HWND& hTouchWnd,int nMonitor)
: CTexnoFrm(hTouchWnd,nMonitor)
, m_bLoad(true)
, m_nNumber( nMonitor )
  {
  }

CSxemaFrm::~CSxemaFrm(void)
  {
  }

BEGIN_MESSAGE_MAP(CSxemaFrm, CTexnoFrm)
  ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CSxemaFrm::PreCreateWindow(CREATESTRUCT& cs)
  {
  cs.style |= WS_MAXIMIZE;
  return CTexnoFrm::PreCreateWindow(cs);
  }

void CSxemaFrm::PostNcDestroy()
  {
  //CFrameWnd::PostNcDestroy();
  }

BOOL CSxemaFrm::MainCreate( CWnd* pParentWnd )
  {
  RECT rect = ::RectWnd(m_nNumber);
  //
  DWORD style = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  if ( !CreateEx(0,NULL,"Технологическая схема",style,rect,pParentWnd,0) )
    return FALSE;
  //
  return TRUE;
  }
//
BOOL CSxemaFrm::IsShowSxeme()
  {
  return ::IsWindow(m_hWnd) && IsWindowVisible();
  }
//
int CSxemaFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
  if (CTexnoFrm::OnCreate(lpCreateStruct) == -1)
    return -1;
  //
  DWORD style = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  //
  CString strChildClass = AfxRegisterWndClass(
    CS_VREDRAW | CS_HREDRAW,::LoadCursor(NULL,IDC_HAND),
    (HBRUSH)::GetStockObject(BLACK_BRUSH),0);
  //
  UINT id = AFX_IDW_PANE_FIRST;
  //
  BOOL bRes = m_Mini.Create(strChildClass,"Уменьшенное окно",style,rectDefault,this,++id);
  if(!bRes)
    return FALSE;
  //
  bRes = m_Scheme.Create(NULL,"Увеличенное окно",style|WS_VISIBLE,rectDefault,this,++id);
  if(!bRes)
    return FALSE;
  //
  return 0;
  }
//
void CSxemaFrm::GotoOper()
  {
  if ( !::IsWindow(m_hWnd) )
    return;
  ShowOrHide(SW_HIDE);
  }
//
void CSxemaFrm::GotoPole( const char* name, int nCorrelate )
  {
  if ( !::IsWindow(m_hWnd) )
    return;
  //
  switch ( nCorrelate )
    {
    //case 0:
    //  GoToSchem( NULL );
    //break;
    case 1:
      if ( Correlates(name) )
        break;
    case 0:
    case 2:
      if ( m_bLoad )
        {
        m_bLoad = false;
        CString str = String("LastView");
        const char* pszName = NULL;
        if ( lstrlen(str) ) pszName = str;
        GoToSchem( pszName );
        }
      else
        Invalidate( false );
    break;
    };
  //
  ShowOrHide(SW_SHOW);
  }
//
bool CSxemaFrm::Correlates( const char* name )
  {
  //
  // Загружаем файл
  CharMP szPath;
  CModelInfo& model = ::ModelInfo();
  sprintf_s( szPath, SizeMP, "%sCtrlTab.xml", model.dirScript );
  CFileXML xml;
  UINT nReturn = xml.Load( szPath );
  if ( nReturn != 0 ) return false;
  //
  // Определяем наличие объектов
  HXML hDoc = xml.FindItem( INVALID_HXML, "Document" );
  if ( hDoc == INVALID_HXML ) return false;
  HXML hMnemo = xml.FindItem( hDoc, "MnemoToSxema" );
  if ( hMnemo == INVALID_HXML ) return false;
  //
  // Ищем по списку данное имя
  HXML hItem = xml.FindItem( hMnemo, "item" );
  while ( hItem != INVALID_HXML )
    {
    const char* mnemo = xml.Param( hItem, "mnemo" );
    const char* sxema = xml.Param( hItem, "sxema" );
    if ( mnemo && sxema && *mnemo && sxema )
    if ( lstrcmpi( name, mnemo ) == 0 )
      {
      GoToSchem( sxema );
      return true;
      }
    hItem = xml.NextItem( hItem );
    }
  return false;
  }
//
