#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "FieldsView.h"
#include <macros/base_iterator.h>
#include <float.h>
#include <math.h>
#include <crosslocale.h>
#include <rsu_str.h>

using namespace n_RSUs;
// KFieldsView

IMPLEMENT_DYNCREATE(KFieldsView, CTreeListView)

KFieldsView::KFieldsView()
{

}

KFieldsView::~KFieldsView()
{
}

KFlexDoc* KFieldsView::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(KFlexDoc)));
  return (KFlexDoc*)m_pDocument;
}

BEGIN_MESSAGE_MAP(KFieldsView, CTreeListView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_WM_CREATE()
  ON_WM_TIMER()
END_MESSAGE_MAP()


// KFieldsView diagnostics

#ifdef _DEBUG
void KFieldsView::AssertValid() const
{
	CTreeListView::AssertValid();
}

#ifndef _WIN32_WCE
void KFieldsView::Dump(CDumpContext& dc) const
{
	CTreeListView::Dump(dc);
}
#endif
#endif //_DEBUG


// KFieldsView message handlers
struct SHeaderNames
{
    std::string pName;
    int width;
};
static SHeaderNames s_HN[] =
{
  {
        "Value"
        //"Значение"
        , 350},
  {     
        "Name"
        //"Имя"
        , 270},
};

std::string ConvertUTF8ToCP1251(const std::string& str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);

    len = WideCharToMultiByte(1251, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* cp1251 = new char[len];
    WideCharToMultiByte(1251, 0, wstr, -1, cp1251, len, NULL, NULL);

    std::string result(cp1251);
    delete[] wstr;
    delete[] cp1251;

    return result;
}

std::wstring ConvertUTF8ToWCHAR(const std::string& str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
    std::wstring result(wstr);
    delete[] wstr;

    return result;
}

wchar_t* ConvertUTF8ToWCHARBuf(const std::string& str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* ret = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, ret, len);    
    return ret;
}

int KFieldsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CTreeListView::OnCreateK(lpCreateStruct, TVS_EDITLABELS) == -1)
    return -1;

  //setlocale(LC_ALL, "ru-RU");
  for( int i=0; i<_countof(s_HN); i++ )
  {
      //s_HN[i].pName = ConvertUTF8ToCP1251(s_HN[i].pName);
      //std::string s = ConvertUTF8ToCP1251(s_HN[i].pName);
      //wchar_t* cn = ConvertUTF8ToWCHARBuf(s_HN[i].pName);
      InsertColumn(i, (char*)s_HN[i].pName.c_str(), LVCFMT_LEFT, s_HN[i].width);
      //delete[] cn;
  }

  SetTimer( 500, 700, NULL );

  return 0;
}

LRESULT KFieldsView::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( !(!wParam && 77==lParam) )
    return 0L;

  KFlexDoc *pDoc = GetDocument();
  INotifier* pN = pDoc->GetEventNotifier( eSelectedDR );
  m_connections.push_back( ConnectNotifierA( pN, this, &KFieldsView::OnSelectedDR ) );
  pN = pDoc->GetEventNotifier( eSelectedBlock );
  m_connections.push_back( ConnectNotifierA( pN, this, &KFieldsView::OnSelectedBlock ) );

  return 1L;
}

void KFieldsView::OnSelectedDR( LPSTR pszFileName )
{
  m_ctrlTree.DeleteAllItems();
  m_filledItems.clear();
}

template<size_t _S, typename T>
void AnalogRepresent( char (&buf)[_S], T val )
{
  T integer_portion = 0;
  T m = modf( val, &integer_portion );

  int decimal = 0;
  int sign = 0;
  char tmp[_countof(buf)] = {};
  _fcvt_s( tmp, val, 2, &decimal, &sign );
  if( sign )
    strcpy_s( buf, "-" );
  if( decimal<=0 )
  {
    strcat_s( buf, "0." );
    while( decimal ) 
    {
      strcat_s( buf, "0" );
      ++decimal;
    }
    strcat_s( buf, tmp );
  }
  else if( decimal > 0 )
  {
    strncat_s( buf, tmp, decimal );
    strcat_s( buf, "." );
    strcat_s( buf, &tmp[decimal] );
  }
  for( LPSTR pEnd = buf + strlen(buf)-1; pEnd > buf; --pEnd )
  {
    if( '0'==*pEnd )
      *pEnd = 0;
    else
      break;
  }
}

template<size_t _S>
bool ValueRepresent( char (&buf)[_S], BYTE *pMem, eVarType type )
{
  buf[0] = 0;
  std::string v_name = "";
  switch (type)
  {
  case evtDigital32:
      sprintf_s(buf, "%I32d", *(tDigital32*)pMem);
      break;
  case evtDigital64:
      sprintf_s(buf, "%I64d", *(tDigital64*)pMem);
      break;
  case evtAnalog32:
  {
      tAnalog32 val = *(tAnalog32*)pMem;
      if (FLT_MAX == val || (-FLT_MAX) == val)
          return false;
      //AnalogRepresent( buf, val );
      sprintf_s(buf, "%f", val);
  }
  break;
  case evtChars:
      if (pMem)
      {
        v_name = ConvertUTF8ToCP1251((tChars*)pMem);
        strcpy_s(buf, v_name.c_str());
      }
    break;
  case evtAnalog64:
    {
      tAnalog64 val = *(tAnalog64*)pMem;
      //AnalogRepresent( buf, val );
      sprintf_s( buf, "%f", val );
    }
    break;
  case evtPrivConstString:
    strcpy_s( buf, *(tPrivConstDynString*)pMem );
    break;
  }
  return true;
}

static LPCSTR s_szFirstField[] = { "OP", "PV", "SP", "MODE", "I", "P", "INVERSE" };

void KFieldsView::OnSelectedBlock( KFlexBlockBase *pBlock )
{
  m_ctrlTree.DeleteAllItems();
  m_filledItems.clear();
  if( !pBlock )
    return;

  DWORD pos = 0;
  SVarInfo var;
  char value[256];
  while( BYTE *pMem = pBlock->EnumVars( pos, var) )
  {
    bool bSkip = true;
    for( int s=0; s<_countof(s_szFirstField); ++s )
    {
      if( !strcmp( var.pszName, s_szFirstField[s]) )
      {
        bSkip = false;
        break;
      }
    }
    if( bSkip )
      continue;
    if( !ValueRepresent( value, pMem, var.evtType ) )
      continue;
    HTREEITEM hRootItem = m_ctrlTree.InsertItem( "", TVI_ROOT );
    SetSubItemText( hRootItem, 1, var.pszName );
    SFieldInfo f = { pMem, var.evtType, var.size };
    m_filledItems[hRootItem] = f;
  }
  pos = 0;
  while( BYTE *pMem = pBlock->EnumVars( pos, var) )
  {
    bool bSkip = false;
    for( int s=0; s<_countof(s_szFirstField); ++s )
    {
      if( !strcmp( var.pszName, s_szFirstField[s]) )
      {
        bSkip = true;
        break;
      }
    }
    if( bSkip )
      continue;
    if( !ValueRepresent( value, pMem, var.evtType ) )
      continue;
    HTREEITEM hRootItem = m_ctrlTree.InsertItem( "", TVI_ROOT );
    SetSubItemText( hRootItem, 1, var.pszName );
    SFieldInfo f = { pMem, var.evtType, var.size };
    m_filledItems[hRootItem] = f;
  }
}

void SetValue( LPCSTR pszText, BYTE *pMem, eVarType type, tVarSizeType_ length )
{
  if( !pszText || !pMem )
    return;
  switch( type )
  {
  case evtDigital32:
    *(tDigital32*)pMem = atoi(pszText);
    break;
  case evtDigital64:
    *(tDigital64*)pMem = _atoi64(pszText);
    break;
  case evtAnalog32:
    *(tAnalog32*)pMem = (tAnalog32)cross::locale::safe_atof(pszText);
    break;
  case evtChars:
    strcpy_s( (tChars*)pMem, length, pszText );
    break;
  case evtAnalog64:
    *(tAnalog64*)pMem = (tAnalog64)cross::locale::safe_atof(pszText);
    break;
  }
}

void KFieldsView::OnTimer(UINT_PTR nIDEvent)
{
  if( 500==nIDEvent )
  {
    KillTimer( nIDEvent );
    char value[256 * 4];
    int n = 0;
    for( base_iterator<tContainer_> it(m_filledItems); it; ++it, ++n )
    {
      HTREEITEM hItem = (*it).first;
      tContainer_::mapped_type &han = (*it).second;
      ValueRepresent( value, han.pmem, han.type );
      SetSubItemText( hItem, 0, value );
    }
    m_ctrlTree.Invalidate(FALSE);
    SetTimer( 500, 700, NULL );
  }

  CTreeListView::OnTimer(nIDEvent);
}

DWORD m_dwEditCtrlStyle = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_NOHIDESEL;
CString m_strValidEditCtrlChars = "0123456789.-";

BOOL KFieldsView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  LPNMHDR pNmhdr = (LPNMHDR)lParam;
  if(m_ctrlTree.m_hWnd == pNmhdr->hwndFrom)
  {
    if( 0xfe65 ==(0xffff&pNmhdr->code) )
    {
      TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)lParam;
      HTREEITEM hItem = pTVDispInfo->item.hItem;
      tContainer_::iterator it = m_filledItems.find( hItem );
      if( m_filledItems.end()!=it )
      {
        tContainer_::mapped_type &han = it->second;
        SetValue( pTVDispInfo->item.pszText, han.pmem, han.type, han.length );
        return TRUE;
      }
    }
  }
  return __super::OnNotify( wParam, lParam, pResult );
}
