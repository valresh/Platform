#include "AlarmServer.h"

KAlarmServer::KAlarmServer( LPCSTR pszModelName )
{
  m_bInited = m_bStep0 = m_bStep1 = false;
}

int KAlarmServer::Initialize()
{
  int result = 0;
  if( !m_bInited )
  {
    m_bInited = true;

    result = 0; // KAlarmKernalHW5xx::InitL();
    //DWORD ThreadId;
    //m_hWndThread = ::CreateThread( NULL, 0, s_WndThread, (void*)this, 0, &ThreadId );

    //while( !m_hCmdWnd )
    //  Sleep( 100 );
    //if( m_hCmdWnd )
    //  SendMessage( m_hCmdWnd, WM_USER+1, 2, 0 );
  }
  return result;
}

int KAlarmServer::AfterInit()
{
  if( !m_bStep0 )
      KAlarmKernalHW5xx::InitL();
  else
      KAlarmKernalHW5xx::Reset();
  m_bStep0 = true;
  return 0;
}

int KAlarmServer::AfterRestored()
{
  m_bStep1 = true;
  return 0;
}

int KAlarmServer::CalcStep( int dtMs )
{
  double dtS = dtMs / 1000.;
  KAlarmKernalHW5xx::StepTL( dtS );
  return 0;
}

namespace ns_details
{
  static LPCSTR s_szWndClassName = "HW5xxAlarmServer_ModelWindowClass";
  static int s_flagRegisteredWindow = 0;
}

#ifdef _WIN32
void KAlarmServer::ShowObject( LPCSTR pszName )
{
//  if( m_hCmdWnd )
//    SendMessage( m_hCmdWnd, WM_USER+1, 1, (LPARAM)pszName );
}
#endif
