#include "stdafx.h"
#include "SafetyManager.h"
#include "ViewList.h"
#include "SafetyManagerApp.h"

// CViewList

IMPLEMENT_DYNCREATE(CViewList, CTreeView)

CViewList::CViewList()
: m_pCtrl( NULL )
, m_CurSelected( -1 )
{
}

CViewList::~CViewList()
{
}

KSafetyManagerDoc* CViewList::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(KSafetyManagerDoc)));
  return (KSafetyManagerDoc*)m_pDocument;
}

BEGIN_MESSAGE_MAP(CViewList, CTreeView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewList::OnNMDblclk)
END_MESSAGE_MAP()


// CViewList diagnostics

#ifdef _DEBUG
void CViewList::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewList::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewList message handlers

BOOL CViewList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_TRACKSELECT | WS_TABSTOP;// | 0x800,0,0,142,214,WS_EX_CLIENTEDGE

  BOOL r = CTreeView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  m_pCtrl = &GetTreeCtrl();
  m_imgList.Create(IDB_FOLDERS,16, 0, RGB(255,255,255));
  m_pCtrl->SetImageList(&m_imgList,TVSIL_NORMAL);

  return r;
}

LRESULT CViewList::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( wParam )
  {
    if( m_CurSelected==lParam )
      return 0;
    HTREEITEM hCurSel = m_pCtrl->GetNextItem(TVI_ROOT, TVGN_ROOT );
    while( hCurSel )
    {
      int nItem = (int)m_pCtrl->GetItemData(hCurSel);
      if( lParam==nItem )
      {
        m_pCtrl->SelectItem( hCurSel );
        GetDocument()->SetTitle( m_pCtrl->GetItemText(hCurSel) );
        break;
      }
      hCurSel = m_pCtrl->GetNextItem(hCurSel, TVGN_NEXT );
    }
    m_CurSelected = lParam;

    return 0L;
  }

  HTREEITEM hRoot = NULL;
  char name[16*4];
  UINT nCount = 0;
  LFscFile* mFile = GetDocument()->m_pOwner->FscFileList( nCount );

  for( UINT n = 0; n < nCount; ++n )
  {
    LFscFile& item = mFile[n];
    if( item.nParent == -1 )
    {
      sprintf_s( name, "%04d", item.nNumber );
      HTREEITEM hItem = m_pCtrl->InsertItem(name,4,5,hRoot);
      m_pCtrl->SetItemData(hItem,n);
      Recursive( hItem, n, mFile, nCount );
    }
  }

  return 1L;
}

void CViewList::Recursive( HTREEITEM hRoot, UINT N, LFscFile* mFile, UINT nCount )
{
  char name[256];
  bool bChild = false;
  for( UINT n = N+1; n < nCount; ++n )
  {
    LFscFile& item = mFile[n];
    if( item.nParent == N )
    {
      sprintf_s( name, "%04d", item.nNumber );
      HTREEITEM hItem = m_pCtrl->InsertItem(name,2,3,hRoot);
      m_pCtrl->SetItemData(hItem,n);
      Recursive( hItem, n, mFile, nCount );
      bChild = true;
    }
  }
}

void CViewList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
  *pResult = 0;
  HTREEITEM hItem = m_pCtrl->GetSelectedItem();
  if ( hItem == NULL )  
    return;
  CString fscName = m_pCtrl->GetItemText( hItem );
  int nItem = (int)m_pCtrl->GetItemData(hItem);
  if( nItem < 0 )
    return;

  m_CurSelected = nItem;

  GetDocument()->SetTitle( fscName );
  GetDocument()->OnSelectFsc( fscName, nItem );
}
