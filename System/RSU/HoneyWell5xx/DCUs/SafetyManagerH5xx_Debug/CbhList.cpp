#include "stdafx.h"
#include <crossplatform.h>
#include "SafetyManagerApp.h"
#include "CbhList.h"
#include <rsuNoNames.h>
#include <BaseType.h>
#include <SafetyManagerStruct.h>
#include <UtilsSM.h>
#include "EditFix.h"

#pragma comment( lib, "UtilsSM5xx.lib" )

// KCbhList

IMPLEMENT_DYNCREATE(KCbhList, CListView)

KCbhList::KCbhList()
: m_pCtrl( NULL )
, m_bInited( false )

{

}

KCbhList::~KCbhList()
{
}

BEGIN_MESSAGE_MAP(KCbhList, CListView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_NOTIFY_REFLECT(NM_DBLCLK, &KCbhList::OnNMDblclk)
END_MESSAGE_MAP()


// KCbhList diagnostics

#ifdef _DEBUG
void KCbhList::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void KCbhList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// KCbhList message handlers
BOOL KCbhList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  // TODO: Add your specialized code here and/or call the base class

  dwStyle |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SORTASCENDING | LVS_ALIGNLEFT | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER;

  BOOL r = CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  m_pCtrl = &GetListCtrl();

  m_pCtrl->InsertColumn ( 0, "", LVCFMT_LEFT, 200, 0 );
  m_pCtrl->InsertColumn ( 1, "", LVCFMT_LEFT, 200, 0 );
  m_pCtrl->SetExtendedStyle ( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

  return r;
}

static KNoName *s_IOs;
LRESULT KCbhList::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( m_bInited )
    return 0L;
  m_bInited = true;
  s_IOs = KNoName::Create( "SM5xx_IOs", false );
  DWORD ID = 0;
  CBase* pBase = NULL;
  int nFcs = 0;
  const char* pszName;
  int added = 0;
  while( s_IOs->WhileBase( ID, -1, &pBase, &pszName, &nFcs ) )
  {
    LPCSTR pszClass = NULL;
    LPCSTR pszNN = NULL;
    s_IOs->GetNameEx( pBase->ID_PNT_NAME, pBase->ID_CLASS, &pszClass, &pszNN, NULL );
    int n = m_pCtrl->InsertItem( added++, pszName );
    m_pCtrl->SetItemText( n, 1, pszClass );
    m_pCtrl->SetItemData( n, pBase->ID_CLASS );
  }

  return 1L;
}

static LPCSTR s_ppSharedFields[] = { "PV", "PVFL", "OPFL", "OP" };

void KCbhList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
  *pResult = 0;

  int nItem = m_pCtrl->GetNextItem( -1, LVNI_SELECTED );
  if ( nItem < 0 )
    return;

  SDotValue vals[10] = {};
  IFscStorage::SVarInfo vars[ _countof(vals) ] = { 0 };
  int c = 0;

  DWORD ID_CLASS = (DWORD)m_pCtrl->GetItemData( nItem );
  CString name = m_pCtrl->GetItemText( nItem, 0 );
  CBase *pBase = s_IOs->FindStruct( name, ID_CLASS, 0 );

  for( int i=0; i<_countof(s_ppSharedFields); ++i )
  {
    SValueDef* def = NameToValueSM( pBase->ID_CLASS, s_ppSharedFields[i] );
    if( !def )
      continue;
    BYTE *pMem = (BYTE*)pBase;
    pMem += def->dwShift;
    USHORT size = 0;
    switch( def->eVal )
    {
    case enumValueDbl:
      vals[c].dVal = *(double*)pMem;
      size = sizeof(double);
      break;
    case enumValueBol:
      vals[c].bVal = *(bool*)pMem;
      size = sizeof(bool);
      break;
    case enumValueInt:
      vals[c].nVal = *(int*)pMem;
      size = sizeof(int);
      break;
    case enumValueChr:
      vals[c].cVal = *(char*)pMem;
      size = sizeof(char);
      break;
    default:
      ASSD(0);
      continue;
    }
    strcpy_s( vars[c].szName, s_ppSharedFields[i] );
    vars[c].val = &vals[c];
    vals[c].eType = def->eVal;
    ++c;
  }

  KEditFix dlg( vars, c );
  dlg.DoModal();

}
