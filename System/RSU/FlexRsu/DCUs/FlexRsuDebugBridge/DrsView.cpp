#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "DrsView.h"
#include <macros/AutoRestore.h>

// KDrsView

IMPLEMENT_DYNCREATE(KDrsView, CListView)

KDrsView::KDrsView()
: m_pCtrl( NULL )
, m_bSelectFromMe( false )
{

}

KDrsView::~KDrsView()
{
}

KFlexDoc* KDrsView::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(KFlexDoc)));
  return (KFlexDoc*)m_pDocument;
}

BEGIN_MESSAGE_MAP(KDrsView, CListView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &KDrsView::OnLvnItemchanged)
  ON_NOTIFY_REFLECT(NM_RCLICK, &KDrsView::OnNMRclick)
END_MESSAGE_MAP()


// KDrsView diagnostics

#ifdef _DEBUG
void KDrsView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void KDrsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// KDrsView message handlers
BOOL KDrsView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  dwStyle |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SORTASCENDING | LVS_ALIGNLEFT | LVS_NOSORTHEADER;

  BOOL r = CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  m_pCtrl = &GetListCtrl();

  m_pCtrl->InsertColumn ( 0, "", LVCFMT_LEFT, rect.right-5, 0 );
  m_pCtrl->SetExtendedStyle ( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
  return r;
}

LRESULT KDrsView::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( !(!wParam && 77==lParam) )
    return 0L;

  KFlexDoc *pDoc = GetDocument();
  m_NotifierSelectedDR = pDoc->GetEventNotifier( eSelectedDR );
  m_connections.push_back( ConnectNotifierA( m_NotifierSelectedDR, this, &KDrsView::OnSelectedDR ) );

  int added = 0;
  const SPrjCfg &Config = pDoc->GetConfig();
  for( DWORD n=0; n<Config.nDR; ++n )
  {
    KFlexDR *pDR = Config.ppDR[n];
    m_pCtrl->InsertItem( added++, pDR->m_szFileName );
  }

  return 1L;
}

void KDrsView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
  TAutoRestore<bool> me( m_bSelectFromMe );
  m_bSelectFromMe = true;

  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  if( pNMLV->uNewState==3 )
  {
    int nItem = pNMLV->iItem;
    if( m_NotifierSelectedDR )
    {
      CString name = m_pCtrl->GetItemText( pNMLV->iItem, 0 );
      m_NotifierSelectedDR->DoNotify( eSelectedDR, (LPSTR)(LPCSTR)name );
    }
  }
  else if( pNMLV->uNewState==0 )
    m_NotifierSelectedDR->DoNotify( eSelectedDR, NULL );
  *pResult = 0;
}

void KDrsView::OnSelectedDR( LPSTR pszContourFileName )
{
  if( m_bSelectFromMe )
    return;
  int prev_item = m_pCtrl->GetSelectionMark();
  if( prev_item>=0)
    m_pCtrl->SetItemState(prev_item, ~LVIS_SELECTED, LVIS_SELECTED);
  int C = m_pCtrl->GetItemCount();
  for( int i=0; i<C; ++i )
  {
    CString name = m_pCtrl->GetItemText( i, 0 );
    if( name==pszContourFileName )
    {
      m_pCtrl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
      m_pCtrl->SetSelectionMark(i);
      m_pCtrl->EnsureVisible(i,FALSE);
      break;
    }
  }
}

void KDrsView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
  *pResult = 0;

  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  CString name = m_pCtrl->GetItemText( pNMLV->iItem, 0 );

  if( name.IsEmpty() )
    return;

  KFlexDoc *pDoc = GetDocument();
  CString prjName = pDoc->GetTitle();
  if( prjName.IsEmpty()  )
    return;
  const SPrjCfg &Config = pDoc->GetConfig();
  for( DWORD n=0; n<Config.nDR; ++n )
  {
    KFlexDR *pDR = Config.ppDR[n];
    if( pDR->m_szFileName!=name )
      continue;
    Show( prjName, name, pDR );
    break;
  }
}
