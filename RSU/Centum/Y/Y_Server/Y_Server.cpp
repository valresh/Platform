#include "stdafx.h"
#include "Y_Server.h"
#include <rsuNoNames2D.h>
#include <ConnectY.h>
#include <ShowProjConnect.h>
#include <RsuX.h>
#include <time.h>

LPCSTR g_szPntsFilePrefix = "РСУ_Y";

extern KNoName2D *g_yokoObjs, *g_yokoIOs;
static KNoName2D *pYokos[] = { g_yokoObjs, g_yokoIOs };
extern CCentumSystem* g_pCentumSystem;

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

KServer::KServer( LPCSTR pszModelName )
: CY_MainLogic( pszModelName )
, m_hWndThread( 0 )
, m_hCmdWnd( 0 )
{
  m_szShowObject[0] = 0;
  m_bInited = m_bStep0 = false;
}

int KServer::Initialize()
{
  if( m_bInited )
    return 0;
  m_bInited = true;
  if( !pRegisterYWstructs )
    LoadLibrary("YokoRegisterInRSU.dll");
  if( pRegisterYWstructs )
    pRegisterYWstructs();

  if( pRegisterRsuConnection )
    pRegisterRsuConnection( g_szPntsFilePrefix, RsuConnectParamInfo, NULL );

  DWORD ThreadId;
  m_hWndThread = ::CreateThread( NULL, 0, s_WndThread, (void*)this, 0, &ThreadId );

  int r = __super::InitL();
  g_pCentumSystem = m_pCentumSystem;
  return r;
}

int KServer::AfterInit()
{
  CY_Base::s_nCalcSteps = 0;
  g_CalcStep = CY_Base::s_nCalcSteps;
  if( m_bStep0 )
  {
    DWORD id = 0;
    CBase* pBase;
    LPCSTR pszName = NULL;
    for( int g=0; g<_countof(pYokos); ++g )
    {
      DWORD id = 0;
      while( pYokos[g]->WhileBase( id, 0, &pBase, &pszName, NULL, NULL ) )
      {
        if( !IsAcy(pBase->ID_CLASS) )
          continue;
        CAlarmBase* pAlrmed = (CAlarmBase*)pBase;
        pAlrmed->btEHLN = 0;
        pAlrmed->ALRM = 0;
        pAlrmed->ALRMmask = 0;
        pAlrmed->XALRM = 0;
        pAlrmed->notACK = 0;
        pAlrmed->setACK = 0;
      }
    }
    return 0;
  }
  m_bStep0 = true;
  return __super::Step0L();
}

int KServer::AfterRestored()
{
  return __super::Step1L();
}

int KServer::CalcStep( int dtMs )
{
  double dtH = 0;
  if( m_pCentumSystem )
  {
    dtH = m_pCentumSystem->dtAsIs;
    CY_Base::s_ModelFullTimeT = m_pCentumSystem->modelTime;
  }
  if( 0==dtH )
    dtH = dtMs / 3600. / 1000.;
  if( 0==CY_Base::s_ModelFullTimeT )
    CY_Base::s_ModelFullTimeT = time(NULL);
  ++CY_Base::s_nCalcSteps;
  return __super::StepTL( dtH );
}


DWORD KServer::s_WndThread( LPVOID lpParam )
{
  KServer* p = (KServer*)lpParam;
  p->WndThread();
  return 0;
}

namespace ns_details
{
  static LPCSTR s_szWndClassName = "YServer_ModelWindowClass";
  static int s_flagRegisteredWindow = 0;
}

extern HINSTANCE g_hInst;

void KServer::WndThread()
{
  if( !ns_details::s_flagRegisteredWindow )
  {
    WNDCLASSEX wc;
    ::memset ( &wc, 0, sizeof( wc ) );
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = s_WndProc;
    wc.hInstance     = g_hInst;
    wc.lpszClassName = ns_details::s_szWndClassName;
    ::RegisterClassEx( &wc );
  }
  ++ns_details::s_flagRegisteredWindow;
  m_hCmdWnd = ::CreateWindow( ns_details::s_szWndClassName, 0, WS_DISABLED, 0, 0, 0, 0, 0, 0, g_hInst, this );
  if( !m_hCmdWnd )
  {
    return;
  }

  ::SetWindowLongPtr( m_hCmdWnd, GWLP_USERDATA, (LONG_PTR)this );

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}


LRESULT KServer::s_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CREATE:
    {
      CREATESTRUCT *pCreate = (CREATESTRUCT*)lParam;
      if( pCreate->lpCreateParams )
      {
        KServer* p = reinterpret_cast<KServer*>(pCreate->lpCreateParams);
        if( p )
          return p->WndProcImpl( hWnd, msg, wParam, lParam );
      }
    }
    break;
  case WM_DESTROY:
    break;
  }

  LONG_PTR lg = GetWindowLongPtr( hWnd, GWLP_USERDATA);
  if( lg )
  {
    KServer* p = reinterpret_cast<KServer*>(lg);
    if( p )
      return p->WndProcImpl( hWnd, msg, wParam, lParam );
  }

  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}


LRESULT KServer::WndProcImpl( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CREATE:
    m_hCmdWnd = hWnd;
    return 0;
  case WM_USER+1:
    if( 1==wParam )
    {
      LPCSTR pszName = (LPCSTR)lParam;
      if( !strcmp(pszName, m_szObjName) )
        ShowCentumProject( NULL, false );
      if( pOpenDialSYSObjParams )
      {
        CY_Base *pModel = NULL, *pModel2 = NULL;
        if( pFindYObj )
          pModel = pFindYObj( (LPSTR)pszName, 0 );
        if( pModel )
          pOpenDialSYSObjParams( pModel, NULL );
        pModel2 = pFindDObj( (LPSTR)pszName, 0, 0 );
        if( pModel2 && pModel!=pModel2 )
          pOpenDialSYSObjParams( pModel2, NULL );
      }
    }
    break;
  }
  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}

void KServer::ShowObject( LPCSTR pszName )
{
  if( m_hCmdWnd )
  {
    strcpy_s( m_szShowObject, pszName );
    SendMessage( m_hCmdWnd, WM_USER+1, 1, (LPARAM)m_szShowObject );
  }
}
