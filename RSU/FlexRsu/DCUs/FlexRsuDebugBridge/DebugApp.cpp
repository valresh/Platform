#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "DebugApp.h"
#include "FlexFrame.h"
#include "FlexDoc.h"
#include "resource.h"
#include <rsuErr.h>
#include <rsus/rsuPaths.h>
#include <crosspath.h>
#include "../../FlexDcuEditor/FlexRsuDebugBridgeToGUI/memoryDescriptor.h"

// KDebugApp

IMPLEMENT_DYNCREATE(KDebugApp, CWinApp)

KDebugApp::KDebugApp()
: m_pDocManager( NULL )
, m_bDebugMode( FALSE )
, m_hGui( NULL )
{
}

KDebugApp::~KDebugApp()
{
}

BOOL KDebugApp::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int KDebugApp::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinApp::ExitInstance();
}

BEGIN_MESSAGE_MAP(KDebugApp, CWinApp)
END_MESSAGE_MAP()


class KThreadSupport
{
  struct S
  {
    HANDLE hThread;
    KDebugApp *pApp;
  }_array[20];
public:
  KThreadSupport()
  {
    ZeroMemory( _array, sizeof(_array) );
  }
  KDebugApp *Get()
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
    _array[i].pApp = new KDebugApp;
    return _array[i].pApp;
  }
}static s_controller;

// KDebugApp message handlers
void KDebugApp::Open( LPCSTR pszPrjName, HWND hMainWnd, SPrjCfg *pConfig, bool bExitOnClose )
{
  m_bDebugMode = bExitOnClose;

  KDebugApp *pApp = s_controller.Get();
  if( !pApp )
    return;

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
  CWinThread* pThread = pState->m_pCurrentWinThread;
  if( !pState->m_pCurrentWinThread )
    pState->m_pCurrentWinThread = pApp;

  if( !m_pDocManager )
  {
    /*void *pmem = new( sizeof(*m_pDocManager) );
    if( !pmem )
      return;*/
    m_pDocManager = new /*(pmem )*/ CDocManager;
    if( !m_pDocManager )
      return;

    /*pmem = pfnMemAllocImpl( sizeof(CMultiDocTemplate) );
    if( !pmem )
      return;*/
    CMultiDocTemplate* pDocTemplate = new /*(pmem)*/ CMultiDocTemplate( IDR_FLEX,
                                                                        RUNTIME_CLASS(KFlexDoc),
                                                                        RUNTIME_CLASS(KFlexFrame),
                                                                        NULL
                                                                        );
    m_pDocManager->AddDocTemplate(pDocTemplate);
  }

  m_pDocManager->OnFileNew();
  POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
  CDocument* pOpenDocument = NULL;
  while (pos != NULL )
  {
    CDocTemplate* pTemplate = (CDocTemplate*)m_pDocManager->GetNextDocTemplate(pos);
    ASSERT_KINDOF(CDocTemplate, pTemplate);
    POSITION pos1 = pTemplate->GetFirstDocPosition();
    while( pos1 )
    {
      pOpenDocument = pTemplate->GetNextDoc( pos1 );
      if( pOpenDocument && pOpenDocument->IsKindOf( RUNTIME_CLASS(KFlexDoc)) )
      {
        KFlexDoc* pDoc = (KFlexDoc*)pOpenDocument;
        if( !pDoc->IsEmpty() )
          continue;

        *pDoc = *pConfig;
        pDoc->SetTitle( pszPrjName );

        POSITION pos2 = pOpenDocument->GetFirstViewPosition();
        CView *pView = pOpenDocument->GetNextView( pos2 );
        if( pView )
        {
          CFrameWnd *pFr = pView->GetParentFrame();
          if( pFr )
          {
            pFr->SendMessageToDescendants( WM_USER, 0, 77, TRUE, TRUE);
            pFr->PostMessage( WM_USER, 0, 77 );
          }
        }

        pos = NULL;
        break;
      }
      else
        pOpenDocument = NULL;
    }
  }
}

void KDebugApp::CloseAll()
{
  if( !m_pDocManager )
    return;
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();

  POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
  CDocument* pOpenDocument = NULL;
  while (pos != NULL )
  {
    CDocTemplate* pTemplate = (CDocTemplate*)m_pDocManager->GetNextDocTemplate(pos);
    ASSERT_KINDOF(CDocTemplate, pTemplate);
    POSITION pos1 = pTemplate->GetFirstDocPosition();
    while( pos1 )
    {
      pOpenDocument = pTemplate->GetNextDoc( pos1 );
      if( pOpenDocument && pOpenDocument->IsKindOf( RUNTIME_CLASS(KFlexDoc)) )
      {
        POSITION pos2 = pOpenDocument->GetFirstViewPosition();
        CView *pView = pOpenDocument->GetNextView( pos2 );
        if( pView )
        {
          CFrameWnd *pFr = pView->GetParentFrame();
          if( pFr )
          {
            pFr->PostMessage( WM_CLOSE, 0, 0 );
          }
        }
      }
    }
  }

}

using namespace n_GUI;

void KDebugApp::CloseAllWPF()
{
  if( !m_hGui )
  {
    fs::path p = fs::path(rsuGetExecutionPath()) / fs::path("FlexRsuDebugBridgeToGUI.dll");
    m_hGui = LoadLibrary(p.generic_string().c_str() );
    if( !m_hGui )
      return;
  }

  lpfnCloseAllWindows call = (lpfnCloseAllWindows)GetProcAddress( m_hGui, "CloseAllWindows" );
  if( call )
    call();
}
