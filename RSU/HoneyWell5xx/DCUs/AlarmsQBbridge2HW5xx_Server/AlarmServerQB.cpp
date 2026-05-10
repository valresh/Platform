#include "AlarmServerQB.h"
#include <ConnectQB.h>
#include <ConnectH.h>

void fnAlarmQBbridgeImpl( LPCSTR PointName, int Alarm, BYTE Prty, bool ON, double Value, void * bMod, BYTE qbaN )
{
  if( !pAlarm )
    return;
  pAlarm(PointName, Alarm, Prty, ON, Value, (int*)bMod, qbaN, NULL );
}

KAlarmServerQB::KAlarmServerQB( LPCSTR pszModelName )
#ifdef _WIN32
    : 
    m_hWndThread( 0 ),
    m_hCmdWnd(0)
#endif
{
  m_bInited = m_bStep0 = m_bStep1 = false;
  pAlarmQB = fnAlarmQBbridgeImpl;
}

int KAlarmServerQB::Initialize()
{
  if( !m_bInited )
  {
    m_bInited = true;
#ifdef _WIN32
    DWORD ThreadId;
    m_hWndThread = ::CreateThread( NULL, 0, s_WndThread, (void*)this, 0, &ThreadId );

    while( !m_hCmdWnd )
      Sleep( 100 );
    if( m_hCmdWnd )
      SendMessage( m_hCmdWnd, WM_USER+1, 2, 0 );
#endif
  }
  return 0;
}

int KAlarmServerQB::AfterInit()
{
  m_bStep0 = true;
  return 0;
}

int KAlarmServerQB::AfterRestored()
{
  m_bStep1 = true;
  return 0;
}

int KAlarmServerQB::CalcStep( int dtMs )
{
  return 0;
}

#ifdef _WIN32
DWORD KAlarmServerQB::s_WndThread( LPVOID lpParam )
{
  KAlarmServerQB* p = (KAlarmServerQB*)lpParam;
  p->WndThread();
  return 0;
}

namespace ns_details
{
  static LPCSTR s_szWndClassName = "QB4xxAlarmServer_ModelWindowClass";
  static int s_flagRegisteredWindow = 0;
}

extern HINSTANCE g_hInst;

void KAlarmServerQB::WndThread()
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

LRESULT KAlarmServerQB::s_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CREATE:
    {
      CREATESTRUCT *pCreate = (CREATESTRUCT*)lParam;
      if( pCreate->lpCreateParams )
      {
        KAlarmServerQB* p = reinterpret_cast<KAlarmServerQB*>(pCreate->lpCreateParams);
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
    KAlarmServerQB* p = reinterpret_cast<KAlarmServerQB*>(lg);
    if( p )
      return p->WndProcImpl( hWnd, msg, wParam, lParam );
  }

  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}


LRESULT KAlarmServerQB::WndProcImpl( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CREATE:
    m_hCmdWnd = hWnd;
    return 0;
  case WM_USER+1:
    break;
  }
  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}

void KAlarmServerQB::ShowObject( LPCSTR pszName )
{
  if( m_hCmdWnd )
    SendMessage( m_hCmdWnd, WM_USER+1, 1, (LPARAM)pszName );
}
#endif
