#include "Server.h"
#include "FlexPool.h"
#include <rsuStr.h>

LPCSTR g_szPntsFilePrefix = "РСУ_FLEXRSU_";

KFlexPool g_Pool;

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  nRSUx::SParamInfo info = g_Pool.GetObj( pszPointName, pszIOtype, pSuffix );
  if( info.pMem )
  {
    char szBuf[512 * 4];
    strcpy( szBuf, pszPointName );
    char* p = strrchr( szBuf, '.' );
    if( !p )
      p = szBuf + strlen(szBuf);
    strncpy( p, ".STATUS", sizeof(szBuf)-(p - szBuf + 1) );
    nRSUx::SParamInfo status = g_Pool.GetObj( szBuf, pszIOtype, pSuffix );
    if( status.pMem )
    {
      KRsuX rsu;
      rsu.ManualInit( status );
      rsu = 1;
    }
  }
  return info;
}

KServer::KServer( LPCSTR pszModelName)
: KFlexRsu( pszModelName ), m_indexInPool (-1)
#ifdef _WIN32
, m_hWndThread(0)
, m_hCmdWnd( 0 )
#endif
{
  m_szShowObject[0] = 0;
  m_szBigName[0] = 0;
  m_bInited = m_bStep0 = m_bStep1 = false;
}

int KServer::Initialize()
{
  if( !m_bInited )
  {
    m_bInited = true;

    strcpy(m_szBigName, m_szObjName);
    std::string ObjName = m_szObjName;

    std::string pszRealName = ObjName;
    std::string pref = g_szPntsFilePrefix;
    pszRealName.replace(pszRealName.find(pref), pref.length(), "");
    
    strcpy( m_szObjName, pszRealName.c_str() );

    if( pRegisterRsuConnection )
      pRegisterRsuConnection( g_szPntsFilePrefix, RsuConnectParamInfo, NULL );

    m_indexInPool = g_Pool.Register( this, pszRealName.c_str(), ObjName.c_str() );

#ifdef _WIN32
    DWORD ThreadId;
    m_hWndThread = ::CreateThread( NULL, 0, s_WndThread, (void*)this, 0, &ThreadId );

    while( !m_hCmdWnd )
      Sleep( 100 );
#endif
  }
  KFlexRsu::Initialize(g_szPntsFilePrefix);
  /*
  if( m_hCmdWnd )
    SendMessage( m_hCmdWnd, WM_USER+1, 2, 0 );
  */  
  return 0;
}

int KServer::AfterInit()
{
  return KFlexRsu::StepBeforRestore();
}

int KServer::AfterRestored()
{
  return KFlexRsu::StepAfterRestore();
}

int KServer::CalcStep( int dtMs )
{
  double dtS = dtMs / 1000.;
  bool addDt = true;
  if( m_indexInPool>0 )
    addDt = false;
  return KFlexRsu::StepCalc( dtS, addDt );
}

#ifdef _WIN32
DWORD KServer::s_WndThread( LPVOID lpParam )
{
  KServer* p = (KServer*)lpParam;
  p->WndThread();
  return 0;
}

namespace ns_details
{
  static LPCSTR s_szWndClassName = "FlexRsuServer_ModelWindowClass";
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
  HWND hCmdWnd = ::CreateWindow( ns_details::s_szWndClassName, 0, WS_DISABLED, 0, 0, 0, 0, 0, 0, g_hInst, this );
  if( !hCmdWnd )
  {
    return;
  }

  ::SetWindowLongPtr( hCmdWnd, GWLP_USERDATA, (LONG_PTR)this );

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
      ShowACSObject( NULL, NULL, (LPSTR)pszName, NULL );
    }
    /*else if (2 == wParam)
      KFlexRsu::Initialize( g_szPntsFilePrefix );*/
    break;
  }
  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}

void KServer::ShowObject( LPCSTR pszName )
{
  if( m_hCmdWnd )
  {
    if( !strcmp( pszName, m_szObjName ) || !strcmp(pszName, m_szBigName))
    {
      strcpy_s( m_szShowObject, m_szObjName);
      PostMessage( m_hCmdWnd, WM_USER+1, 1, (LPARAM)m_szShowObject );
    }
  }
}
#endif
