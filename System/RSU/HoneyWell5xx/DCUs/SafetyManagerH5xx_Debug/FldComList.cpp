#include "stdafx.h"

#include "FldComList.h"
#include "SafetyManagerApp.h"

// KFldComList

IMPLEMENT_DYNCREATE(KFldComList, CListView)

KFldComList::KFldComList()
: m_pCtrl( NULL )
, m_SelectItemType( LFscBase::HZ )
, m_bInited( false )
{

}

KFldComList::~KFldComList()
{
}

KSafetyManagerDoc* KFldComList::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(KSafetyManagerDoc)));
  return (KSafetyManagerDoc*)m_pDocument;
}

BEGIN_MESSAGE_MAP(KFldComList, CListView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_NOTIFY_REFLECT(NM_DBLCLK, &KFldComList::OnNMDblclk)
END_MESSAGE_MAP()


// KFldComList diagnostics

#ifdef _DEBUG
void KFldComList::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void KFldComList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// KFldComList message handlers
BOOL KFldComList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  // TODO: Add your specialized code here and/or call the base class

  dwStyle |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SORTASCENDING | LVS_ALIGNLEFT | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER;

  BOOL r = CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  m_pCtrl = &GetListCtrl();

  m_pCtrl->InsertColumn ( 0, "", LVCFMT_LEFT, 200, 0 );
  m_pCtrl->SetExtendedStyle ( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

  return r;
}

LRESULT KFldComList::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( m_bInited )
    return 0L;
  m_bInited = true;
  UINT nItem = 0;
  int added = 0;
  IFscStorage *pCur = GetDocument()->m_pOwner;
  for( LFscBase* base = pCur->GetItem( nItem ); base; ++nItem, base = pCur->GetItem( nItem ) )
  {
    int nName = -1;
    if( m_SelectItemType != base->mB )
      continue;
    switch( base->mT )
    {
    case 0x0B:
    case 0x0C:
      {
        SPointIn* data = (SPointIn*)pCur->FscGiveData( base->nBuff );
        nName = data->nDbf;
      }
      break;
    case 0x19:
    case 0x1A:
      {
        SPointOut* data = (SPointOut*)pCur->FscGiveData( base->nBuff );
        nName = data->nDbf;
      }
      break;
    default:
      continue;
    }
    if( -1 ==nName )
      continue;
    LPCSTR pName = static_cast<IFscStorage*>(pCur)->DbfName( nName, 1);
    if( !pName || !*pName )
      continue;
    int n = m_pCtrl->InsertItem( added++, pName );
    m_pCtrl->SetItemData( n, nName );
  }
  return 1L;
}

void KFldComList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  *pResult = 0;

  int nItem = m_pCtrl->GetNextItem( -1, LVNI_SELECTED );
  if ( nItem < 0 )
    return;

  DWORD nName = (DWORD)m_pCtrl->GetItemData( nItem );
  IFscStorage *pCur = GetDocument()->m_pOwner;
  LPCSTR pName = static_cast<IFscStorage*>(pCur)->DbfName( nName, 1);
  LPCSTR pFldNumb = static_cast<IFscStorage*>(pCur)->DbfName( nName, 11);
  if( !pFldNumb || !*pFldNumb )
    return;

  int nFld = atoi(pFldNumb);

  UINT nCount = 0;
  LFscFile* mFile = pCur->FscFileList( nCount );
  for( UINT n = 0; n < nCount; ++n )
  {
    LFscFile& item = mFile[n];
    if( item.nNumber==nFld )
    {
      char name[16*4];
      sprintf_s( name, "%04d", item.nNumber );
      GetDocument()->OnSelectFsc( name, n );
      break;
    }
  }
}
