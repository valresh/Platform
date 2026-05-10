// ListOfBlocksView.cpp : implementation file
//

#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "ListOfBlocksView.h"


// KListOfBlocksView

IMPLEMENT_DYNCREATE(KListOfBlocksView, CListView)

KListOfBlocksView::KListOfBlocksView()
: m_pCtrl( NULL )
, m_NotifiereSelectedBlock( NULL )
{
}

KListOfBlocksView::~KListOfBlocksView()
{
}

KFlexDoc* KListOfBlocksView::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(KFlexDoc)));
  return (KFlexDoc*)m_pDocument;
}

BEGIN_MESSAGE_MAP(KListOfBlocksView, CListView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
  ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked)
  ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
END_MESSAGE_MAP()


// KListOfBlocksView diagnostics

#ifdef _DEBUG
void KListOfBlocksView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void KListOfBlocksView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// KListOfBlocksView message handlers
BOOL KListOfBlocksView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  dwStyle |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | LVS_NOSORTHEADER | LVS_SORTASCENDING;

  BOOL r = CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  m_pCtrl = &GetListCtrl();

  m_pCtrl->InsertColumn ( 0, "", LVCFMT_LEFT, rect.right/2, 0 );
  m_pCtrl->InsertColumn ( 1, "", LVCFMT_LEFT, rect.right/2-5, 0 );
  m_pCtrl->SetExtendedStyle ( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

  return r;
}

LRESULT KListOfBlocksView::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( !(!wParam && 77==lParam) )
    return 0L;

  KFlexDoc *pDoc = GetDocument();
  INotifier* pN = pDoc->GetEventNotifier( eSelectedDR );
  m_connections.push_back( ConnectNotifierA( pN, this, &KListOfBlocksView::OnSelectedDR ) );
  pN = pDoc->GetEventNotifier( eScrollToBlock );
  m_connections.push_back( ConnectNotifierA( pN, this, &KListOfBlocksView::OnScrollToBlock ) );
  m_NotifiereSelectedBlock = pDoc->GetEventNotifier( eSelectedBlock );

  return 1L;
}

void KListOfBlocksView::OnScrollToBlock( LPCSTR pszBlock )
{
  LVFINDINFO info;
  info.flags = LVFI_PARTIAL|LVFI_STRING;
  info.psz = pszBlock;
  int nIndex = m_pCtrl->FindItem( &info );
  if( -1==nIndex )
    return;
  m_pCtrl->SetItemState(-1, 0, LVIS_SELECTED);
  m_pCtrl->SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
  m_pCtrl->EnsureVisible(nIndex, FALSE);
}

void KListOfBlocksView::OnSelectedDR( LPSTR pszFileName )
{
  if( !pszFileName || !*pszFileName )
  {
    m_szCurDR.clear();
    m_pCtrl->DeleteAllItems();
    return;
  }
  if( !m_szCurDR.compare(pszFileName) )
    return;
  m_szCurDR = pszFileName;
  m_pCtrl->DeleteAllItems();
  if( !pszFileName )
    return;
  KFlexDoc *pDoc = GetDocument();

  KFlexDR *pDR = NULL;
  const SPrjCfg &Config = pDoc->GetConfig();
  for( DWORD n=0; n<Config.nDR; ++n )
  {
    if( !_strcmpi( Config.ppDR[n]->m_szFileName, pszFileName) )
    {
      pDR = Config.ppDR[n];
      break;
    }
  }
  if( !pDR )
    return;

  int add = 0;
  KFlexBlockBase::eExecOnNullOrder ioMask = KFlexBlockBase::exnShowOnly;
  for( DWORD b=0; b<pDR->m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = pDR->m_ppBlocks[b];
    if( pBlk->m_execOrder )
      continue;
    if( !(pBlk->m_nullOrderExec & ioMask) )
      continue;
    if( !pBlk->m_szVisibleName[0] )
      continue;
    int a = m_pCtrl->InsertItem( add++, pBlk->m_szVisibleName );
    m_pCtrl->SetItemText( a, 1, pBlk->m_pszType );
    m_pCtrl->SetItemData( a, (DWORD_PTR)pBlk );
  }

  ioMask = KFlexBlockBase::exnInputsIO;
  for( DWORD b=0; b<pDR->m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = pDR->m_ppBlocks[b];
    if( pBlk->m_execOrder )
      continue;
    if( !(pBlk->m_nullOrderExec & ioMask) )
      continue;
    if( !pBlk->m_szVisibleName[0] )
      continue;
    int a = m_pCtrl->InsertItem( add++, pBlk->m_szVisibleName );
    m_pCtrl->SetItemText( a, 1, pBlk->m_pszType );
    m_pCtrl->SetItemData( a, (DWORD_PTR)pBlk );
  }

  char szTmpName[64 * 4] = { 0 };
  for( DWORD b=0; b<pDR->m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = pDR->m_ppBlocks[b];
    if( !pBlk->m_execOrder )
      continue;
    int a = 0;
    if( pBlk->m_szVisibleName[0] )
      a = m_pCtrl->InsertItem( add++, pBlk->m_szVisibleName );
    else
    {
      sprintf_s( szTmpName, "# %d", pBlk->m_execOrder );
      a = m_pCtrl->InsertItem( add++, szTmpName );
    }
    m_pCtrl->SetItemText( a, 1, pBlk->m_pszType );
    m_pCtrl->SetItemData( a, (DWORD_PTR)pBlk );
  }

  ioMask = KFlexBlockBase::exnOutputsIO;
  for( DWORD b=0; b<pDR->m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = pDR->m_ppBlocks[b];
    if( pBlk->m_execOrder )
      continue;
    if( !(pBlk->m_nullOrderExec & ioMask) )
      continue;
    if( !pBlk->m_szVisibleName[0] )
      continue;
    int a = m_pCtrl->InsertItem( add++, pBlk->m_szVisibleName );
    m_pCtrl->SetItemText( a, 1, pBlk->m_pszType );
    m_pCtrl->SetItemData( a, (DWORD_PTR)pBlk );
  }
}

void KListOfBlocksView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  if( pNMLV->uNewState==3 )
  {
    int nItem = pNMLV->iItem;
    if( m_NotifiereSelectedBlock )
    {
      KFlexBlockBase *pBlock = (KFlexBlockBase*)m_pCtrl->GetItemData( pNMLV->iItem );
      m_NotifiereSelectedBlock->DoNotify( eSelectedBlock, pBlock );
    }
  }
  else if( pNMLV->uNewState==0 )
    m_NotifiereSelectedBlock->DoNotify( eSelectedBlock, NULL );
  *pResult = 0;
}

void KListOfBlocksView::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
  HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

  if( phdn->iButton == 0 )
  {
    // User clicked on header using left mouse button
    /*if( phdn->iItem == nSortedCol )
      bSortAscending = !bSortAscending;
    else
      bSortAscending = TRUE;

    nSortedCol = phdn->iItem;
    SortTextItems( nSortedCol, bSortAscending );*/

  }
  *pResult = 0;
}
