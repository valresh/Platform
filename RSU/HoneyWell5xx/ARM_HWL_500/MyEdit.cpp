#include "HTML/Style.h"
#include "StdAfx.h"
#include "MyEdit.h"
#include "ARM.h"
//#include "SelectPoint.h"
#include "HTML/HTML.h"

// IMPLEMENT_DYNAMIC(CMyEdit, CEdit)
// BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
// 	ON_WM_KEYUP()
// END_MESSAGE_MAP()

// void CMyEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//   {
//   if (nChar==VK_RETURN)
//     GetParent()->GetParent()->PostMessage(WM_COMMAND,WPARAM(IDC_BUTTONOK),0);
//   CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
//   }
//
// Смысл - потерять фокус при нажатии на Enter
// BEGIN_MESSAGE_MAP(CMaEdit, CEdit)
// 	ON_WM_KEYDOWN()
// 	ON_WM_GETDLGCODE()
//   ON_WM_KEYUP()
//   ON_BN_CLICKED( 104, OnSelectPoint)
// END_MESSAGE_MAP()

//BOOL CMaEdit::MyCreate( CWnd* pParentWnd, UINT nID, DWORD dwStyle )
BOOL CMaEdit::MyCreate( UINT nID, DWORD dwStyle )
{
    //  BOOL bReturn = Create( ES_AUTOHSCROLL|dwStyle, Ccross::RECT(0,0,0,0), pParentWnd, nID );
    //  if ( bReturn )
    // SetFont( pParentWnd->GetFont() );
    //  return bReturn;
    return true;
}

void CMaEdit::OnSelectPoint()
{
    // CSelectPoint dlg;
    // GetWindowText( dlg.mStr );
    // if ( dlg.DoModal() == IDOK )
    //   {
    //   SetWindowText( dlg.mStr );
    //   OnReturn();
    //   }

}

//void CMaEdit::MyActive( CWnd* pWnd, int nItem, int nSubItem, const char* szText, Ccross::RECT rc, CWnd* pBtn )
void CMaEdit::MyActive(int nItem, int nSubItem, const char* szText, Ccross::RECT rc)
{
    // SetParent( pWnd );
    // SetWindowPos( NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER );
    // SetWindowText( szText );
    // ShowWindow( SW_SHOW );
    // SetFocus();
    // SetSel(0,-1);
    // //
    // if ( pBtn )
    //   {
    //   Ccross::RECT cross::RECT = rc;
    //   pWnd->ClientToScreen( cross::RECT );
    //         ScreenToClient( cross::RECT );
    //   pBtn->SetWindowPos( NULL, cross::RECT.right-rc.Height(), cross::RECT.top, rc.Height(), rc.Height(), SWP_NOZORDER );
    //   pBtn->SetParent( this );
    //   pBtn->ShowWindow( SW_SHOW );
    //   }
    // //
    // m_nItem    = nItem   ;
    // m_nSubItem = nSubItem;
    //
}

void CMaEdit::OnReturn()
{
    // CWnd* pOwner = GetOwner();
    // NMHDR	nmhdr;
    // ASSERT_VALID(pOwner);
    // nmhdr.code = NM_RETURN;
    // nmhdr.idFrom =  GetDlgCtrlID();
    // nmhdr.hwndFrom = m_hWnd;
    // pOwner->SendMessage(WM_NOTIFY,(WPARAM)0,(LPARAM)&nmhdr);
}

void CMaEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // if ( VK_RETURN == nChar )
    //    OnReturn();
    //  CEdit::OnKeyDown(nChar,nRepCnt,nFlags);
}

UINT CMaEdit::OnGetDlgCode()
{
    return DLGC_WANTALLKEYS;
}
//------------------------------------------------------
//
// Смысл - потерять фокус при нажатии на Enter
// BEGIN_MESSAGE_MAP(CMaCombo, CComboBox)
// ON_MESSAGE(CB_GETDROPPEDSTATE, OnReturn)
// ON_CONTROL_REFLECT(CBN_SELCHANGE, &CMaCombo::OnCbnSelchange)
// END_MESSAGE_MAP()

//BOOL CMaCombo::MyCreate( CWnd* pParentWnd, UINT nID )
BOOL CMaCombo::MyCreate(  UINT nID )
{
    // BOOL bReturn = Create( WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWN/*| CBS_SORT*/, Ccross::RECT(0,0,0,300), pParentWnd, nID );
    // if ( bReturn ) SetFont( pParentWnd->GetFont() );
    // return bReturn;
    return true;
}

//void CMaCombo::MyActive( CWnd* pWnd, int nItem, int nSubItem, const char* szText, Ccross::RECT rc )
void CMaCombo::MyActive( int nItem, int nSubItem, const char* szText, Ccross::RECT rc )
{
    //  CWnd* p = GetWindow( GW_CHILD );
    //HWND hWnd = ::GetWindow( m_hWnd, GW_CHILD );
    // int id  = ::GetDlgCtrlID( hWnd );
    // //
    // SetParent( pWnd );
    // SetWindowPos( NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER );
    // SelectString(-1, szText );
    // ShowWindow( SW_SHOW );
    // SetFocus();
    // ShowDropDown();
    //
    m_nItem    = nItem   ;
    m_nSubItem = nSubItem;
    //
}

char* CMaCombo::mList[] =
    {
        "PV",
        "DACA.PV",
        "PIDA.SP",
        "PIDA.PV",
        "PIDA.OP",
        "FLAGA.PV",
        "DEVCTLA.PV",
        "AUTOMANA.OP",
        "AI.PV",
};

int CMaCombo::m_nCount = sizeof(mList)/sizeof(mList[0]);

void CMaCombo::Fill()
{
    //need add some structure for
    // for ( int n = 0; n < m_nCount; n++ )
    //     AddString( mList[n] );
}

// void CMaCombo::OnCbnSelchange()
// {
//     int nSel = GetCurSel();
//     if ( nSel < 0 ) return;
//     if ( nSel > (m_nCount-1) ) return;
//     SetWindowText(mList[nSel]);
//     OnReturn( 0, 0 );
// }

LRESULT CMaCombo::OnReturn(WPARAM wParam, LPARAM lParam)
{
    // CWnd* pOwner = GetOwner();
    // NMHDR	nmhdr;
    // ASSERT_VALID(pOwner);
    // nmhdr.code = NM_RETURN;
    // nmhdr.idFrom =  GetDlgCtrlID();
    // nmhdr.hwndFrom = m_hWnd;
    // pOwner->SendMessage(WM_NOTIFY,(WPARAM)0,(LPARAM)&nmhdr);
    return 0;
}

//bool CMyBmpButton::MyCreate( LPCTSTR szText, DWORD dwStyle, const cross::RECT& cross::RECT, CWnd* pWnd, UINT nID )
bool CMyBmpButton::MyCreate( LPCTSTR szText, DWORD dwStyle, const cross::RECT& cross::RECT,  UINT nID )
{
    // if ( !Create( szText, dwStyle, cross::RECT, pWnd, nID ) )
    //     return false;
    //  AutoLoad( nID, pWnd );

    cross::CString buttonName = szText;
    ASSERT(!buttonName.IsEmpty());      // must provide a title

    // LoadBitmaps(buttonName + _T("U"), buttonName + _T("D"),
    //             buttonName + _T("F"), buttonName + _T("X"));

    // we need at least the primary
    // if (m_bitmap.m_hObject == NULL)
    //     return FALSE;

    // // size to content
    // SizeToContent();
    return TRUE;
};
//---------------------------------------------------------------------------------------
// 
// BEGIN_MESSAGE_MAP(CTagEdit, CEdit)
// ON_WM_KEYDOWN()
// ON_WM_GETDLGCODE()
// ON_WM_KEYUP()
// ON_CONTROL_REFLECT(EN_KILLFOCUS, &CTagEdit::OnEnKillfocus)
// END_MESSAGE_MAP()

HWND CTagEdit::MyCreate( HWND hWnd, CTag* pTag, double fScale, POINT ptOffset )
{
    mVal = pTag->mVal;
    //
//    CWnd* pWnd = CWnd::FromHandle( hWnd );
    CStyle& style = pTag->Style(m_rHtml);
    //cross::RECT cross::RECT = style.Fullcross::RECT();
    cross::RECT cross::RECT = style.m_rcTruecross::RECT;
    cross::RECT rc;
    rc.left   = int(cross::RECT.left  *fScale)-ptOffset.x;
    rc.top    = int(cross::RECT.top   *fScale)-ptOffset.y+1;
    rc.right  = int(cross::RECT.right *fScale)-ptOffset.x;;
    rc.bottom = int(cross::RECT.bottom*fScale)-ptOffset.y;
    //
    DWORD dwStyle = WS_VISIBLE;
    if ( style.align & DT_CENTER ) dwStyle |= ES_CENTER;
    else if ( style.align & DT_RIGHT  ) dwStyle |= ES_RIGHT;
    //
    // BOOL bReturn = Create( ES_AUTOHSCROLL|dwStyle, rc, pWnd, 1945 );
    // if ( !bReturn )
    // {
    //     delete this;
    //     return NULL;
    // }
    //
    // LOGFONT lf = theApp.GiveLogF(style.m_hFont);
    // lf.lfHeight = (int)(lf.lfHeight*fScale);
    // HFONT hNewFont = ::CreateFontIndicross::RECT( &lf );//theApp.GiveFont(lf);
    // HFONT hOldFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0, 0 );
    // ::SendMessage( m_hWnd, WM_SETFONT, (WPARAM)hNewFont, 1 );
 //   if ( hOldFont != NULL ) ::DeleteObject( hOldFont );
    //
    char str[512] = "??????????";
    pTag->FillText( m_rHtml, str );
    // SetWindowText( str );
    // SetFocus();
    // ::SendMessage( m_hWnd, EM_SETSEL, 0, -1 );
  //  return m_hWnd;   
    return hWnd;
}

void CTagEdit::Return()
{
    SSendToModel send( mVal.def );
 //   void* Old = m_rHtml.pipe.ParamValue_H( mVal );
    void* New = NULL;
    char str[512] = "??????????";
 //   GetWindowText( str, 32 );
    switch ( mVal.def->eVal )
    {
    case enumValueDbl:
        if ( ::IsFlt(str) )
        {
            static double v;
            v = atof(str);
            New = &v;
        }
    //    else
  //          AfxMessageBox( "Ошибка формата числа с плавающей точкой", MB_ICONEXCLAMATION );
        break;
    case enumValueInt:
        if ( ::IsInt(str) )
        {
            static int v;
            v = atoi(str);
            New = &v;
        }
        // else
        //     AfxMessageBox( "Ошибка формата целого числа", MB_ICONEXCLAMATION );
        break;
    case enumValueChr:
        if ( ::IsInt(str) )
        {
            static BYTE v;
            v = atoi(str);
            New = &v;
        }
        // else
        //     AfxMessageBox( "Ошибка формата целого числа", MB_ICONEXCLAMATION );
        break;
    case enumValueStr:
        New = str;
        break;
    default:
        ASSERT(0);
        return;
    }
    //
    if ( New != NULL )
    {
    //    send.Set( Old, New );
    //    m_rHtml.pipe.SendData( mVal.nNumber, send, true );
 //       DestroyWindow();
    }
}

// void CTagEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
// {
//     CEdit::OnKeyDown(nChar,nRepCnt,nFlags);
//     if ( VK_RETURN == nChar )
//         Return();
// }

// UINT CTagEdit::OnGetDlgCode()
// {
//     return DLGC_WANTALLKEYS;
// }

void CTagEdit::PostNcDestroy()
{
    delete this;
}

// void CTagEdit::OnEnKillfocus()
// {
//     DestroyWindow();
// }
