#include "stdafx.h"
#include "QuickBuilderH5xxDebug.h"
#include "QuickBuilderApp.h"
#include <rsuErr.h>
#include "Dial_Value.h"


// KQuickBuilderApp

IMPLEMENT_DYNCREATE(KQuickBuilderApp, CWinThread)

KQuickBuilderApp::KQuickBuilderApp()
{
}

KQuickBuilderApp::~KQuickBuilderApp()
{
}

BOOL KQuickBuilderApp::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int KQuickBuilderApp::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(KQuickBuilderApp, CWinThread)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

class KThreadSupport
{
  struct S
  {
    HANDLE hThread;
    KQuickBuilderApp *pApp;
  }_array[20];
public:
  KThreadSupport()
  {
    ZeroMemory( _array, sizeof(_array) );
  }
  KQuickBuilderApp *Get()
  {
    HANDLE cur = GetCurrentThread();
    int i = 0;
    for( i; i<_countof(_array); ++i )
    {
      if( !_array[i].hThread )
        break;
      if( cur==_array[i].hThread )
        return _array[i].pApp;
    }
    ASS( i<_countof(_array) );
    _array[i].hThread = cur;
    _array[i].pApp = new KQuickBuilderApp;
    return _array[i].pApp;
  }
}static s_controller;

// KQuickBuilderApp message handlers
bool ShowDebugWnd( KQbBase *pPnt )
{
  KQuickBuilderApp *pApp = s_controller.Get();
  if( !pApp )
    return false;

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
  CWinThread* pThread = pState->m_pCurrentWinThread;
  if( !pState->m_pCurrentWinThread )
    pState->m_pCurrentWinThread = pApp;

  SQbParam buf[256];
  KQbBridge2SysParam paramCont( buf, _countof(buf) );
  pPnt->GetAsParams( paramCont );

  CDial_Value * pDlg = new CDial_Value;
  if( pDlg == NULL )
    return false;

  if( !pDlg->Create( CDial_Value::IDD ) )
  {
    delete pDlg;
    return false;
  }

  char Title[256];

  if(pDlg)
  {
    sprintf_s(	Title, "%s[%s]", (LPCSTR)pPnt->PointName, pPnt->GetTypeName() );
    pDlg->SetWindowText( Title );

    pDlg->ShowWindow( SW_SHOW );
    pDlg->SetVarList( paramCont.Count(), buf );
  }

  return false;
}
