#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "FlexFrame.h"
#include "DebugApp.h"
#include "DrsView.h"
#include "ListOfBlocksView.h"
#include "FieldsView.h"
#include "resource.h"
#include <DataTypes_.h>
#include "FindByName.h"
#include <macros/StrHelps.h>

//extern KDebugApp theApp;
// KFlexFrame

IMPLEMENT_DYNCREATE(KFlexFrame, CFrameWnd)

KFlexFrame::KFlexFrame()
: m_pDocument( NULL )
, m_nInitCounter( -1 )
, m_NotifierSelectedDR( NULL )
, m_NotifierScroll2Block( NULL )
{

}

KFlexFrame::~KFlexFrame()
{
}


BEGIN_MESSAGE_MAP(KFlexFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_CLOSE()
  ON_WM_TIMER()
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_COMMAND(ID_SIM_CONNECTION, &KFlexFrame::OnSimConnection)
  ON_UPDATE_COMMAND_UI(ID_SIM_CONNECTION, &KFlexFrame::OnUpdateSimConnection)
  ON_COMMAND(ID_FIND, &KFlexFrame::OnFind)
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_SEPARATOR,           // status line indicator
};
// KFlexFrame message handlers
int KFlexFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if( !m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, _countof(indicators) ) )
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  m_wndStatusBar.SetPaneInfo( 0, 251 , SBPS_NORMAL |SBPS_STRETCH, 25);
  m_wndStatusBar.SetPaneInfo( 1, 252 , SBPS_NORMAL |SBPS_STRETCH, 200);

  return 0;
}

BOOL KFlexFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
  if( pContext->m_pCurrentDoc )
  {
    ASSERT(pContext->m_pCurrentDoc->IsKindOf(RUNTIME_CLASS(KFlexDoc)));
    m_pDocument = (KFlexDoc*)pContext->m_pCurrentDoc;
  }
  BOOL bCreateSpltr = m_wndSplitter.CreateStatic( this, 1, 2);
  pContext->m_pCurrentFrame = this;

  int cxCur = 150;//ReadSettings();

  //if( m_bSwitch2ContoursTree )
    bCreateSpltr = m_wndSplitter.CreateView( 0, 0, RUNTIME_CLASS(KDrsView), CSize(cxCur, 0), pContext );
  /*else
    bCreateSpltr = m_wndSplitter.CreateView( 0, 0, RUNTIME_CLASS(KDnaAdminsTree), CSize(cxCur, 0), pContext );*/

  bCreateSpltr = m_wndSplitter2.CreateStatic( & m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE,  m_wndSplitter.IdFromRowCol(0,1) );

  bCreateSpltr = m_wndSplitter2.CreateView( 0, 0, RUNTIME_CLASS(KListOfBlocksView), CSize(600, 400), pContext );
  bCreateSpltr = m_wndSplitter2.CreateView( 1, 0, RUNTIME_CLASS(KFieldsView), CSize(600, 0), pContext );

  SetTimer( 500, 500, NULL );

  m_NotifierSelectedDR = m_pDocument->GetEventNotifier( eSelectedDR );
  m_NotifierScroll2Block = m_pDocument->GetEventNotifier( eScrollToBlock );

  return bCreateSpltr;
}

void KFlexFrame::OnClose()
{
  CFrameWnd::OnClose();
  /*if( theApp.m_bDebugMode )
    PostQuitMessage(0);*/
}

void KFlexFrame::OnTimer(UINT_PTR nIDEvent)
{
  const SPrjCfg &Config = m_pDocument->GetConfig();
  if( 500==nIDEvent )
  {
    char szBuf[256];
    if( Config.pInitCounter && -1!=m_nInitCounter )
    {
      if( m_nInitCounter != *Config.pInitCounter )
      {
        KillTimer( nIDEvent );
        //theApp.CloseAllWPF();
        PostMessage( WM_CLOSE, 0, 0 );
        return;
      }
    }
    if( Config.pSteps && Config.pFlexSystem )
    {
      sprintf_s( szBuf, "Шаг-%d, AlrmXY-%d, Alrm0-%d, Alrm1-%d, bSound-%s ", *Config.pSteps,
                              Config.pFlexSystem->nFlexAlarmXY,
                              Config.pFlexSystem->nFlexAlarms0,
                              Config.pFlexSystem->nFlexAlarms1,
                              Config.pFlexSystem->bSound ? "On" : "Off");
      m_wndStatusBar.SetPaneText( 0, szBuf );
    }
    if( Config.pSkipSimIO )
    {
        if( *Config.pSkipSimIO )
          strcpy_s( szBuf, "Отключена связь с СИМУЛЯТОРОМ" );
        else
          szBuf[0] = 0;
        m_wndStatusBar.SetPaneText( 1, szBuf );
    }
  }

  CFrameWnd::OnTimer(nIDEvent);
}

LRESULT KFlexFrame::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( !(!wParam && 77==lParam) )
    return 0L;
  const SPrjCfg &Config = m_pDocument->GetConfig();
  if( Config.pInitCounter )
    m_nInitCounter = *Config.pInitCounter;
  return 1L;
}

void KFlexFrame::OnSimConnection()
{
  const SPrjCfg &Config = m_pDocument->GetConfig();
  if( Config.pSkipSimIO )
    *Config.pSkipSimIO = !*Config.pSkipSimIO;
}

void KFlexFrame::OnUpdateSimConnection(CCmdUI *pCmdUI)
{
  const SPrjCfg &Config = m_pDocument->GetConfig();
  if( Config.pSkipSimIO )
    pCmdUI->SetCheck( *Config.pSkipSimIO ? 0 : 1 );
}

void KFlexFrame::OnFind()
{
  KFindByName dlgFind( m_pDocument );
  if( dlgFind.DoModal() != IDOK )
    return;

  char elements[10][128] = {};
  int C = SplitString( dlgFind.m_ContourFileName, elements, '\\', false );
  if( m_NotifierSelectedDR )
    m_NotifierSelectedDR->DoNotify( eSelectedDR, elements[0] );
  if( m_NotifierScroll2Block )
    m_NotifierScroll2Block->DoNotify( eScrollToBlock, elements[1] );
}
