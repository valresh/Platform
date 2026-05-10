#include "stdafx.h"
#include "Resource.h"
#include "FlexRsuDebugBridge.h"
#include "FindByName.h"
#include <rsus/rsuPaths.h>
#include <vector>
#include <macros/AutoRestore.h>


// KFindByName dialog

#define MASK_FILE_NAME "%sFind_Flex.txt"

IMPLEMENT_DYNAMIC(KFindByName, CDialog)

KFindByName::KFindByName( KFlexDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(KFindByName::IDD, pParent)
  , m_pDocument( pDoc )
{

}

KFindByName::~KFindByName()
{
}

void KFindByName::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO1, m_Mask);
  DDX_Control(pDX, IDC_REPORT, m_List);
}


BEGIN_MESSAGE_MAP(KFindByName, CDialog)
  ON_BN_CLICKED(IDC_FIND, &KFindByName::OnBnClickedFind)
  ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KFindByName::OnNMDblclkReport)
  ON_MESSAGE( WM_USER+1, OnEnum)
END_MESSAGE_MAP()


// KFindByName message handlers

BOOL KFindByName::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect rc;
  m_List.GetClientRect( &rc );
  int w = rc.right - GetSystemMetrics( SM_CXVSCROLL ) - 2;
  m_List.InsertColumn ( 0, "Имя", LVCFMT_LEFT, 200, 0 );
  w -= 200;
  m_List.InsertColumn ( 1, "Путь", LVCFMT_LEFT, w-100, 1 );
  m_List.InsertColumn ( 2, "Тип", LVCFMT_LEFT, 100, 1 );
  m_List.SetExtendedStyle ( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

  ReadStr();

  if( m_pDocument && !m_pDocument->m_initialFind.IsEmpty() )
  {
    m_Mask.SetWindowText( m_pDocument->m_initialFind );
    m_pDocument->m_initialFind = "";
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void KFindByName::OnNMDblclkReport(NMHDR *pNMHDR, LRESULT *pResult)
{
  *pResult = 0;
  PostMessage ( WM_COMMAND, IDOK );
}

bool CompName (int lN, const char * Name, int lS, const char * S)
{
  if ( lN == 0 && lS == 0 )
    return true;
  if ( lS == 1 && S[0] == '*' )
    return true;
  if ( lN > 0 && lS == 0 )
    return false;
  if ( S[0] == '*' )
  {
    for ( int n = 0; n < lN; n++ )
    {
      if ( CompName ( lN - n, Name + n, lS - 1, S + 1 ))
        return true;
    }
    return false;
  }
  if ( S[0] == '?' )
  {
    return CompName ( lN - 1, Name + 1, lS - 1, S + 1 );
  }
  if ( toupper(S[0]) != toupper(Name[0]) )
    return false;
  return CompName( lN - 1, Name + 1, lS - 1, S + 1 );
}

bool KFindByName::FiltrName( LPCSTR Filtr, LPCSTR Name )
{
  int lN = lstrlen ( Name );
  return CompName ( lN, Name, lstrlen( Filtr ), Filtr );
}

void KFindByName::Count( void *entry, DWORD &C )
{
  ++C;
  /*size_t c = entry->GetChild( NULL, 0 );
  if( !c )
    return;
  std::vector<IProgram*> buf;
  buf.resize( c );
  c = entry->GetChild( &buf.at(0), buf.size() );
  for( size_t i=0; i<c; ++i )
  {
    Count( buf.at(i), C );
  }*/
}

void KFindByName::OnBnClickedFind()
{
  if( !m_pDocument )
    return;
  if( !m_findString.IsEmpty() )
  {
    m_findString.Empty();
    return;
  }
  UpdateData();
  m_List.DeleteAllItems( );

  char ObjTxtFiltr[256] = { 0 };
  m_Mask.GetWindowText( ObjTxtFiltr, _countof(ObjTxtFiltr) );
  char *Filtr = ObjTxtFiltr;

  LRESULT Ind = m_Mask.SendMessage( CB_FINDSTRINGEXACT, -1, (LPARAM)Filtr );
  if ( Ind != CB_ERR )
    m_Mask.SendMessage( CB_DELETESTRING, Ind, 0 );  
  m_Mask.InsertString( 0, Filtr );

  SaveStr();

  while( *Filtr )
  {
    *Filtr = toupper( *Filtr );
    Filtr++;
  }
  m_findString = ObjTxtFiltr;

  DWORD pos = 0;

  PostMessage( WM_USER+1, 0, 0 );
}

LRESULT KFindByName::OnEnum( WPARAM wParam, LPARAM lParam )
{
  const SPrjCfg &Config = m_pDocument->GetConfig();

  char szFullPath[1024] = {};
  LPCSTR Filtr = m_findString;
  for( DWORD n=0; n<Config.nDR; ++n )
  {
    KFlexDR *pDR = Config.ppDR[n];
    for( DWORD b=0; b<pDR->m_nBlocks; ++b )
    {
      KFlexBlockBase *pBlk = pDR->m_ppBlocks[b];
      if( pBlk->m_szVisibleName.empty() )
        continue;
      if( FiltrName(Filtr, pBlk->m_szVisibleName) )
      {
        sprintf_s( szFullPath, "%s\\%s", pDR->m_szFileName, (LPCSTR)pBlk->m_szVisibleName);
        InsertItem( pBlk->m_szVisibleName, pBlk->m_pszType ? pBlk->m_pszType : "?", szFullPath  );
      }
    }
  }
  return 1L;
}

void KFindByName::InsertItem( LPCSTR pszName, LPCSTR pszType, LPSTR pszPath )
{
  size_t L = strlen(pszPath);
  LPSTR pszEnd = pszPath + L;

  int c = m_List.GetItemCount();
  int N = m_List.InsertItem( c, pszName );
  m_List.SetItemText( N, 1, pszPath );
  m_List.SetItemText( N, 2, pszType );
}

void KFindByName::OnOK()
{
  int nItem = m_List.GetNextItem( -1, LVNI_SELECTED );
  if ( nItem < 0 )
    return;

  m_ContourFileName = m_List.GetItemText( nItem, 1 );
  /*m_ContourFileName.Append( "\\" );
  m_ContourFileName.Append( m_List.GetItemText( nItem, 0 ) );*/
  CDialog::OnOK();
}

void KFindByName::ReadStr()
{
  char Path[_MAX_PATH];
  sprintf_s ( Path, MASK_FILE_NAME, rsuGetMemoryPath().c_str());
  FILE * F; 
  fopen_s ( &F, Path, "rt" );
  if ( F == NULL )
    return ;
  while( fgets ( Path, _countof(Path), F ))
  {
    int L = (int)strlen ( Path );
    if ( L < 2 )
      continue;
    Path[L-1] = 0;
    m_Mask.AddString( Path );
  }
  fclose( F );
}

void KFindByName::SaveStr()
{
  LRESULT K = (LRESULT)m_Mask.SendMessage( CB_GETCOUNT, 0, 0 ); 
  char Path[_MAX_PATH];
  sprintf_s( Path, MASK_FILE_NAME, rsuGetMemoryPath().c_str());
  rsuTakeOffReadOnly( Path );
  FILE * F; fopen_s ( &F, Path, "wt" );
  if( F == NULL )
    return;
  for( LRESULT n = 0; n < K; ++n )
  {
    m_Mask.SendMessage( CB_GETLBTEXT, n, (LPARAM)Path );
    fprintf( F, "%s\n", Path );
  }
  fclose ( F );
}