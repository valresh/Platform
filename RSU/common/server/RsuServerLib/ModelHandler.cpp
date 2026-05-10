#include "stdafx.h"
#include "ModelHandler.h"
#include <rsuPaths.h>
#include <rsuErr.h>
#include <rsuIPC.h>


namespace ns_details
{
  static LPCSTR s_szWndClassName = "RsuServer_ModelWindowClass";
  static int s_flagRegisteredWindow = 0;
}

KModelHandler::KModelHandler()
: m_hWndThread( 0 )
, m_hInst( 0 )
, m_hWait( NULL )
, m_hModelLib( 0 )
, m_pServer( NULL )
, m_hCalcComplete( NULL )
{
}

KModelHandler::~KModelHandler()
{
#ifdef _OLD
  ::DestroyWindow( hCmdWnd );
#endif
}

int KModelHandler::Init( HINSTANCE hInst, LPCSTR pszDll, LPCSTR pszModelName )
{
  m_hInst = hInst;
  m_szDll = pszDll;
  m_szModelName = pszModelName;

  m_hWait = CreateEvent( NULL, TRUE, FALSE, NULL );

  if (m_hWait == NULL)
    return -1;

  DWORD ThreadId;
  m_hWndThread = ::CreateThread( NULL, 0, s_WndThread, (void*)this, 0, &ThreadId );
  if( !m_hWndThread )
    return -2;

#ifdef _OLD
  WaitForSingleObject( m_hWait, INFINITE);
  ResetEvent(m_hWait);

  if( !hCmdWnd )
    return -3;
#endif

#ifdef _OLD
  PostMessage( hCmdWnd, WM_USER, 0, ewcLoad );
#else
  WndProcImpl( hCmdWnd, WM_USER, 0, ewcLoad );
#endif
  WaitForSingleObject( m_hWait, INFINITE);
  ResetEvent(m_hWait);
  if( !m_hModelLib )
    return -4;

#ifdef _OLD
  PostMessage( hCmdWnd, WM_USER, 0, ewcInitialize );
#else
  WndProcImpl(hCmdWnd, WM_USER, 0, ewcInitialize);
#endif
  WaitForSingleObject( m_hWait, INFINITE);
  ResetEvent(m_hWait);
  if( !m_pServer )
    return -5;

#ifdef _OLD
  char szBuf[_MAX_PATH];
  sprintf_s( szBuf, "%s_CalcComplete", pszModelName );
  m_hCalcComplete = CreateEvent( NULL, FALSE, TRUE, szBuf );

  if( NULL==m_hCalcComplete )
    return -6;
#endif

  return 0;
}

DWORD KModelHandler::s_WndThread( LPVOID lpParam )
{
  KModelHandler* p = (KModelHandler*)lpParam;
  p->WndThread();
  return 0;
}

void KModelHandler::WndThread()
{
#ifdef _OLD
  if( !ns_details::s_flagRegisteredWindow )
  {
    WNDCLASSEX wc;
    ::memset ( &wc, 0, sizeof( wc ) );
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = s_WndProc;
    wc.hInstance     = m_hInst;
    wc.lpszClassName = ns_details::s_szWndClassName;
    ::RegisterClassEx( &wc );
  }
  ++ns_details::s_flagRegisteredWindow;
  hCmdWnd = ::CreateWindow( ns_details::s_szWndClassName, 0, WS_DISABLED, 0, 0, 0, 0, 0, 0, m_hInst, this );
  if( !hCmdWnd )
  {
    SetEvent( m_hWait );
    return;
  }

  ::SetWindowLongPtr( hCmdWnd, GWLP_USERDATA, (LONG_PTR)this );

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#else
#endif
}

LRESULT KModelHandler::s_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CREATE:
    {
      CREATESTRUCT *pCreate = (CREATESTRUCT*)lParam;
      if( pCreate->lpCreateParams )
      {
        KModelHandler* p = reinterpret_cast<KModelHandler*>(pCreate->lpCreateParams);
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
    KModelHandler* p = reinterpret_cast<KModelHandler*>(lg);
    if( p )
      return p->WndProcImpl( hWnd, msg, wParam, lParam );
  }

  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT KModelHandler::WndProcImpl( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_CREATE:
    hCmdWnd = hWnd;
    SetEvent( m_hWait );
    return 0;
  case WM_USER:
    switch( lParam )
    {
    case ewcLoad:
      if( !m_hModelLib )
        m_hModelLib = LoadServer( m_szDll.c_str() );
      SetEvent( m_hWait );
      break;
    case ewcInitialize:
      if( !m_pServer )
        m_pServer = Initialize();
      SetEvent( m_hWait );
      break;
    }
    break;
  case WM_USER+1:
    switch( lParam )
    {
    case epkAfterInit:
      if( 0==m_pServer->AfterInit() )
        return epkAfterInit;
      break;
    case epkAfterRestored:
      if( 0==m_pServer->AfterRestored() )
        return epkAfterRestored;
      break;
    case epkCalcStep:
      m_pServer->CalcStep( (int)wParam );
#ifdef _OLD
      SetEvent( m_hCalcComplete );
#endif
      return epkCalcStep;
      break;
    }
    break;
  case WM_COPYDATA:
    return OnCopyData( wParam, lParam );
  }
  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}

HMODULE KModelHandler::LoadServer( LPCSTR DllName )
{
  char szPath[_MAX_PATH] = { 0 };
  _makepath_s( szPath, NULL, rsuGetExecutionPath(), DllName, "dll" );
  HMODULE hLib = LoadLibrary(szPath);
  return hLib;
}

IRsuModel *KModelHandler::Initialize()
{
  tCreateServer pCreateServer = (tCreateServer)::GetProcAddress( m_hModelLib, "CreateServer" );
  if( !pCreateServer )
    return NULL;
  IRsuModel *pServer = pCreateServer( m_szModelName.c_str() );
  pServer->Initialize();
  return pServer;
}

LRESULT KModelHandler::OnCopyData( WPARAM wParam, LPARAM lParam )
{
  if( !m_pServer )
    return 0;
  COPYDATASTRUCT &cds = *(COPYDATASTRUCT*)lParam;
  SCopyDataHeader &header = *(SCopyDataHeader*)cds.lpData;
  switch( header.type )
  {
  case epkSaveRSUParams:
  case epkRestoreRSUParams:
  case epkSaveRSUState:
  case epkRestoreRSUState:
    {
      SSaveRestoreSP &cmd = *(SSaveRestoreSP*)cds.lpData;
      LPCSTR pszPath = cmd.szPath;
      int r = -1;
      switch( header.type )
      {
      case epkSaveRSUParams:
        r = m_pServer->ParamsSave( pszPath );
        break;
      case epkRestoreRSUParams:
        r = m_pServer->ParamsRestore( pszPath );
        break;
      case epkSaveRSUState:
        r = m_pServer->StateSave( pszPath );
        break;
      case epkRestoreRSUState:
        r = m_pServer->StateRestore( pszPath );
        break;
      }
      if( 0==r )
        return header.type;
    }
    break;
  case epkShowObject:
    {
      SShowObject &cmd = *(SShowObject*)cds.lpData;
      LPCSTR pszName = cmd.szName;
      m_pServer->ShowObject( pszName );
      return cmd.type;
    }
    break;
  }
  return 0;
}
