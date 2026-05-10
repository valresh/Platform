// DialName.cpp : implementation file
//
#include "stdafx.h"
#include "../ARM.h"
#include "TexnoFrm.h"
#include "DialName.h"
#include "Queue.h"
#include "TestTime.h"
#include <string>
#include <algorithm>
#include <list>

//
IMPLEMENT_DYNAMIC(CDialName, CDialog)
CDialName::CDialName()
: CDialog(CDialName::IDD,NULL)
, m_TreeYbottom( 0 )
, m_MaskYtop( 0 )
  {
  }

CDialName::~CDialName()
  {
  }

void CDialName::DoDataExchange(CDataExchange* pDX)
  {
    DDX_Control(pDX, IDC_TREE_CTRL, m_wndTree);
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MASK, m_MaskCtrl);
}

BEGIN_MESSAGE_MAP(CDialName, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CTRL, OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_CTRL, OnTvnItemexpandedTree1)
  ON_EN_CHANGE(IDC_MASK, &CDialName::OnEnChangeMask)
END_MESSAGE_MAP()

// CDialName message handlers
BOOL CDialName::OnInitDialog()
  {
  CDialog::OnInitDialog();

  m_imgList.Create(IDB_FOLDERS,16, 0, RGB(255,255,255));
  CBitmap bm;
  bm.LoadBitmap(IDB_ITEMS);
  m_imgList.Add(&bm,RGB(0,255,0));
  m_wndTree.SetImageList(&m_imgList,TVSIL_NORMAL);

  CRect tree, mask, wnd;
  m_wndTree.GetWindowRect( &tree );
  ScreenToClient( &tree );
  m_MaskCtrl.GetWindowRect( &mask );
  ScreenToClient( &mask );
  GetClientRect( &wnd );

  m_TreeYbottom = wnd.Height() - tree.bottom;
  m_MaskYtop = wnd.bottom - mask.top;

  return TRUE;  // return TRUE unless you set the focus to a control
  }
//
BOOL CDialName::Create(CTexChild* pParent)
  {
  m_pSend = pParent;
  //
  if ( !::IsWindow( m_hWnd ) )
    {
    //CTestTime tm("C:/Build.menu","MENU" );
    CString strError = LoadList(*m_pSend);
    if ( !strError.IsEmpty() )
      {
      AfxMessageBox( strError, MB_ICONEXCLAMATION );
      return FALSE;
      }
    CDialog::Create(IDD, pParent);
    }
  //
  if ( !::IsWindow( m_hWnd ) )
    return FALSE;
  //
  FillTree();
  m_MaskCtrl.SetWindowText( _T("") );
  CRect rect;
  GetWindowRect( rect );
  POINT pt = GetPointWork(NULL,rect.Size());
  SetWindowPos( NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
  //
  return TRUE;
  }
//
void CDialName::OnOK()
  {
  }

void CDialName::OnSize(UINT nType, int cx, int cy)
{
  CDialog::OnSize(nType, cx, cy);

  if( m_wndTree.GetSafeHwnd() && m_MaskCtrl.GetSafeHwnd() )
  {
    CRect rect;
    GetClientRect(rect);
    rect.bottom -= m_TreeYbottom;
    m_wndTree.MoveWindow(rect);

    GetClientRect(rect);
    rect.top = rect.bottom - m_MaskYtop;
    m_MaskCtrl.MoveWindow(rect);

  }
}

void CDialName::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
  {
  *pResult = 0;
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  ASSERT(pNMTreeView != NULL);	
  int m = (int)pNMTreeView->itemNew.lParam;

  // если выбрана группа
  if(m_wndTree.ItemHasChildren(pNMTreeView->itemNew.hItem))
    return;

  CWnd* pParent = GetParent();
  // -1 всегда папка
  if ( m >= 0 )	
    {
    SItemCrd& item = mItem.Item(m);
    int N = 0;
    char* file = File(m,N);
    int   numb = item.m_nNumb[N];
    const char* pszName = m_pSend->GetName();
    if ( lstrcmpi(pszName,file) != 0 )
    m_pSend->LoadData( file );
    m_pSend->SetObjToCenter(numb);
    }
  }

void CDialName::OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult)
  {
  *pResult = 0;
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  int Image = 1;
  if(pNMTreeView->itemNew.state & TVIS_EXPANDED)
    Image = 0;
  // изменяем иконку
  m_wndTree.SetItemImage(pNMTreeView->itemNew.hItem,Image,Image);
  }
//
bool CDialName::FillTree( const char* pSubString )
{
  std::string upperSubString;
  if( pSubString )
    upperSubString = pSubString;
  std::transform( upperSubString.begin(), upperSubString.end(), upperSubString.begin(), &toupper );

  //
  struct STreeCrd
    {
    char name[32];
    EDataTypes eType;
    int nImage;
    };
  //
  static STreeCrd list[] =
    {
    #undef  DATATYPE
    #define DATATYPE( Id, Nomer, Text, Icon ) { _T(Text), id_##Id, Icon },
    #include "BaseType.hpp"
    {_T("")},
    };
  // заполняем дерево
  m_wndTree.SetRedraw(FALSE);
  m_wndTree.DeleteAllItems();
  //
  CString str;
  HTREEITEM hOther = NULL;
  //
  int nSize = mItem.Count();
  if ( nSize == 0 )  
    return false;
  m_vFlagForShow.resize( nSize );

  SItemCrd* obj = (SItemCrd*)mItem.m_szBuffer;
  int n = 0;
  const bool bFilterOn = pSubString && pSubString[0]!=0;
	do
    {
    int N = n;
    int forFilter = 0;
    while ( N < nSize && obj[n].eType == obj[N].eType ) 
    {
		if ( !::IsMnemoStyle(MNEMO_EDIT) )
		{
		   if (obj[n].eType==id_Goto)
		   {
			N++;
			continue;
		   }
		}
      m_vFlagForShow[N] = 1;
      if( !bFilterOn )
        forFilter++;
      else
      {
        std::string testName = Name( N );
        std::transform( testName.begin(), testName.end(), testName.begin(), &toupper );
        const char* pSub = strstr( testName.c_str(), upperSubString.c_str() );
        if( pSub )
          forFilter++;
        else
          m_vFlagForShow[N] = 0;
      }
      N++;
    }
    int M = 0;
    while ( *list[M].name )
    {
      if ( list[M].eType == obj[n].eType )
        break;
      M++;
    }
    if ( *list[M].name )
    {
      if( !bFilterOn )
        str.Format( "%s (%d)", list[M].name, N-n );
      else
        str.Format( "%s (%d/%d)", list[M].name, forFilter, N-n );
      //
      if( forFilter)
      {
        HTREEITEM hRoot = m_wndTree.InsertItem(str,1,1);
        m_wndTree.SetItemData(hRoot,-1);
        for ( int m = n; m < N; m++ )
        {
          if( !bFilterOn || m_vFlagForShow[m]==1 )
          {
            HTREEITEM hItem = m_wndTree.InsertItem(Name(m),list[M].nImage,list[M].nImage,hRoot);
            m_wndTree.SetItemData(hItem,m);
          }
        }
        m_wndTree.SortChildren(hRoot);
      }
    }
    else
    if ( ::IsMnemoStyle(MNEMO_HIDE_SCN) )
      {
      if ( hOther == NULL )
        {
        hOther = m_wndTree.InsertItem("Я",1,1);
        m_wndTree.SetItemData(hOther,-1);
        }
      for ( int m = n; m < N; m++ )
        {
        str.Format( "%d %s", obj[n].eType, Name(m) );
        HTREEITEM hItem = m_wndTree.InsertItem(str,2,2,hOther);
        m_wndTree.SetItemData(hItem,m);
        }
      m_wndTree.SortChildren(hOther);
      }
    n = N;
    }
  while ( n < nSize );
  //
  m_wndTree.SortChildren(NULL);
  m_wndTree.SetRedraw(TRUE);

  return bFilterOn;
}
//

typedef std::list<std::string> tSL_;
void CDialName::OnEnChangeMask()
{
  CString maskText;
  m_MaskCtrl.GetWindowText( maskText );

  tSL_ expanded;
  HTREEITEM hItem = m_wndTree.GetRootItem();
  while( hItem )
  {
    if( m_wndTree.GetItemState( hItem,TVIS_EXPANDED) & TVIS_EXPANDED ) 
    {
      std::string t = m_wndTree.GetItemText( hItem ).GetBuffer();
      std::string::size_type pos = t.find( '(' );
      if( std::string::npos != pos )
        expanded.push_back( t.substr( 0, pos) );
    }
    hItem = m_wndTree.GetNextItem( hItem, TVGN_NEXT );
  }

  FillTree( maskText );

  hItem = m_wndTree.GetRootItem();
  while( hItem )
  {
    std::string t = m_wndTree.GetItemText( hItem ).GetBuffer();
    tSL_::iterator iter = expanded.begin();
    while ( iter!=expanded.end() )
    {
      if( std::string::npos != t.find( *iter ) )
      {
        m_wndTree.Expand( hItem, TVE_EXPAND );
        break;
      }
      ++iter;
    }
    hItem = m_wndTree.GetNextItem( hItem, TVGN_NEXT );
  }
}
