#include "stdafx.h"
#include "SafetyManager.h"
#include "SafetyManagerView.h"
#include "SafetyManagerApp.h"
#include <InlineGDI.h>
#include "EditFix.h"

//#define NEW_DRAW

// KSafetyManagerView

IMPLEMENT_DYNCREATE(KSafetyManagerView, CScrollView)

KSafetyManagerView::KSafetyManagerView()
: m_hBitmap(NULL)
, m_fScale(2.0)
, m_curFsc( -1 )
, m_ptMouse(0,0)
, m_nSelect( -1 )
{
}

KSafetyManagerView::~KSafetyManagerView()
{
  if( m_hBitmap ) 
    ::DeleteObject( m_hBitmap );
}

BEGIN_MESSAGE_MAP(KSafetyManagerView, CScrollView)
  ON_MESSAGE( WM_USER, OnWmUser)
  ON_WM_ERASEBKGND()
  ON_WM_MOUSEWHEEL()
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_SETCURSOR()
  ON_WM_TIMER()
END_MESSAGE_MAP()


// KSafetyManagerView drawing
KSafetyManagerDoc* KSafetyManagerView::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(KSafetyManagerDoc)));
  return (KSafetyManagerDoc*)m_pDocument;
}

bool GiveBitmap(HDC hDC,int nWidth,int nHeight,HBITMAP& hBitmap)
{
  bool bReturn = false;
  if ( hBitmap != NULL )
  {
    BITMAP info;
    ::GetObject( hBitmap, sizeof(BITMAP), &info);
    if ( info.bmWidth < nWidth || info.bmHeight < nHeight )
    {
      ::DeleteObject(hBitmap);
      hBitmap = NULL;
    }
  }
  //
  if ( hBitmap == NULL )
  {
    bReturn = true;
    hBitmap = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
  }
  return bReturn;
}

void KSafetyManagerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
  CRect rect;
  GetClientRect( rect );

  CDC cdc;
  if ( cdc.CreateCompatibleDC( pDC ) )
  {
    ::GiveBitmap( *pDC, rect.Width(), rect.Height(), m_hBitmap );
    if ( m_hBitmap != NULL )
    {
      HBITMAP hOldBmp = (HBITMAP)::SelectObject( cdc, m_hBitmap );
      {
        CMyBrush brush( cdc, BACKGROUND);
        brush.Rect( rect );
      }
      Draw( cdc );
      CPoint pt = GetDeviceScrollPosition();
      pDC->BitBlt( pt.x, pt.y, rect.Width(), rect.Height(), &cdc, 0, 0, SRCCOPY );
      //
      ::SelectObject( cdc, hOldBmp );
    }
    cdc.DeleteDC();
  }
}


// KSafetyManagerView diagnostics

#ifdef _DEBUG
void KSafetyManagerView::AssertValid() const
{
	CScrollView::AssertValid();
}

void KSafetyManagerView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// KSafetyManagerView message handlers
LRESULT KSafetyManagerView::OnWmUser( WPARAM wParam, LPARAM lParam )
{
  if( !wParam )
    return 0L;

  m_curFsc = (UINT)lParam;
  LFscFile& mItem = GetDocument()->m_pOwner->FscFileItem( m_curFsc );

  CSIZE size = { 100, 100 };
  SetScrollSizes( size );

  return 1L;
}

void KSafetyManagerView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CSIZE size = { 100, 100 };
  SetScrollSizes( size );

  SetTimer( 500, 500, NULL );
}

void KSafetyManagerView::SetScrollSizes( CSIZE size)
{
  if( -1 != m_curFsc )
    size = GetDocument()->m_pOwner->FscFileItem( m_curFsc ).szDoc;

  CSize sz = CSize(int(m_fScale*size.cx),int(m_fScale*size.cy));

  __super::SetScrollSizes(MM_TEXT, sz);
}

void KSafetyManagerView::Draw( CDC& dc )
{
#ifdef NEW_DRAW
	CSafetyMgrDraw cDraw;
	cDraw.m_pCur = GetDocument()->m_pOwner;
	cDraw.Draw (&dc, m_curFsc, m_fScale, this);
#else
  XFORM xT;
  int nGraph = dc.SetGraphicsMode( GM_ADVANCED );
  dc.GetWorldTransform( &xT );

  CPoint pt = GetDeviceScrollPosition();
  FLOAT dx = -FLOAT(pt.x);
  FLOAT dy = -FLOAT(pt.y);
  XFORM xf = {FLOAT(m_fScale),0,0,FLOAT(m_fScale),dx,dy};

  dc.SetWorldTransform( &xf );
  //
  CFont font;
  LOGFONT lf;
  memset( &lf, 0, sizeof(lf) );
  lf.lfCharSet = DEFAULT_CHARSET;
  lf.lfHeight  = -8;
  strcpy_s( lf.lfFaceName, "Courier New" );
  font.CreateFontIndirect(&lf);
  CFont* old = dc.SelectObject( &font );
  int nMode = dc.SetBkMode( TRANSPARENT );

  COLORREF color = dc.SetTextColor( 0xffffff );
  //////////////////////////////////////////////////////////////////////////
  CMyFont mFont( dc, font, 0xfffffd );
  if( -1!=m_curFsc )
    GetDocument()->m_pOwner->FscFileDraw( mFont, m_curFsc, -1, false );
  //////////////////////////////////////////////////////////////////////////
  dc.SetTextColor( color );

  dc.SetBkMode( nMode );
  dc.SelectObject( &old );
  font.DeleteObject();
  //
  dc.SetWorldTransform( &xT );
  dc.SetGraphicsMode( nGraph );
#endif // NEW_DRAW
}

BOOL KSafetyManagerView::OnEraseBkgnd(CDC* pDC)
{
  // TODO: Add your message handler code here and/or call default

  return FALSE;//CScrollView::OnEraseBkgnd(pDC);
}

BOOL KSafetyManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
  if( !CScrollView::PreCreateWindow(cs) )
    return FALSE;

  cs.dwExStyle |= WS_EX_CLIENTEDGE;
  cs.style &= ~WS_BORDER;
  cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
                                    ::LoadCursor(NULL, IDC_ARROW),
                                    reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)),
                                    NULL);

  return TRUE;
}

void KSafetyManagerView::ChangeScale( BOOL bInc )
{
  CPoint pos = GetDeviceScrollPosition();
  CRect rect;
  GetWindowRect( &rect );
  ScreenToClient( &rect );
  CPoint centre = rect.CenterPoint();

  CPoint pt;
  pt.x = (int)(double(centre.x+pos.x)/m_fScale);
  pt.y = (int)(double(centre.y+pos.y)/m_fScale);

  CSIZE szDoc = { rect.Width(), rect.Height() };
  if( -1 != m_curFsc )
    szDoc = GetDocument()->m_pOwner->FscFileItem( m_curFsc ).szDoc;

  if( bInc )
  {
    if ( m_fScale < 10 ) 
    {
      m_fScale += 1.0; 
      SetScrollSizes( szDoc ); 
      Invalidate(false);
    }
  }
  else
  {
    if ( m_fScale > 1. )
    {
      m_fScale -= 1.0;
      SetScrollSizes( szDoc );
      Invalidate(false);
    }
  }

  pt.x = (LONG)(pt.x * m_fScale);
  pt.y = (LONG)(pt.y * m_fScale);
  centre.x *= -1;
  centre.y *= -1;
  pt.Offset( centre );
  ScrollToPosition( pt );
}

BOOL KSafetyManagerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  if( zDelta>0 )
    ChangeScale( TRUE );
  else
    ChangeScale( FALSE );

  return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void KSafetyManagerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  switch ( nChar )
  {
  case VK_ADD:
  case VK_OEM_PLUS: 
    ChangeScale(TRUE);
    break;
  case VK_SUBTRACT:
  case VK_OEM_MINUS:
    ChangeScale( FALSE );
    break;
  }

  __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

int SetArrow()
{
  static HCURSOR hArrow = ::LoadCursor( NULL, IDC_ARROW );
  ::SetCursor( hArrow );
  return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int SetHand()
{
  static HCURSOR hHand = ::LoadCursor( NULL, IDC_HAND );
  ::SetCursor( hHand );
  return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int SetLapa()
{
  static HCURSOR hLapa = theApp.LoadCursor( IDC_YES_TRACK );
  ::SetCursor( hLapa );
  return 0;
}

void KSafetyManagerView::OnLButtonDown(UINT nFlags, CPoint point)
{
  CPoint ptOffset = GetDeviceScrollPosition();
  POINT pt;
  pt.x = int( (point.x+ptOffset.x)*m_fScale);
  pt.y = int( (point.y+ptOffset.y)*m_fScale);
  m_ptMouse = point;
  SetLapa();

  __super::OnLButtonDown(nFlags, point);
}

void KSafetyManagerView::OnLButtonUp(UINT nFlags, CPoint point)
{
  POINT pt;
  ::GetCursorPos( &pt );
  ScreenToClient( &pt );
  CPoint ptOffset = GetDeviceScrollPosition();
  pt.x = (int)(double(pt.x+ptOffset.x)/m_fScale);
  pt.y = (int)(double(pt.y+ptOffset.y)/m_fScale);
  m_ptMouse.x = -1;
  m_ptMouse.y = -1;
  SetArrow();

  LFscBase* obj = GetDocument()->m_pOwner->FscInRect( m_curFsc, pt );
  if( obj )
  {
    if(   0x0D <= obj->mT && obj->mT <= 0x10
      ||  0x15 <= obj->mT && obj->mT <= 0x18 )
    {
      Action( obj );
      return;
    }
    else
    {
      QuickWatch( obj );
    }
  }

  __super::OnLButtonUp(nFlags, point);
}

void KSafetyManagerView::OnMouseMove(UINT nFlags, CPoint point)
{
  if ( nFlags & MK_LBUTTON && m_ptMouse.x > 0 )
  {
    int dx = point.x-m_ptMouse.x;
    int dy = point.y-m_ptMouse.y;
    if ( !(dx == 0 && dy == 0) )
    {
      m_ptMouse = point;
      CPoint ptOffset = GetDeviceScrollPosition();
      ptOffset.x -= dx;
      ptOffset.y -= dy;
      //
      CSize sz = GetTotalSize();
      ptOffset.x = std::max<LONG>( ptOffset.x, 0 );
      ptOffset.y = std::max<LONG>( ptOffset.y, 0 );
      ptOffset.x = std::min<LONG>( ptOffset.x, sz.cx );
      ptOffset.y = std::min<LONG>( ptOffset.y, sz.cy );
      //
      ScrollToPosition(ptOffset);
    }
  }

  __super::OnMouseMove(nFlags, point);
}

POINT KSafetyManagerView::Scroll( POINT point, bool bScale )
{
  POINT pt = GetDeviceScrollPosition();
  point.x += pt.x;
  point.y += pt.y;
  if( bScale )
  {
    point.x = int( point.x/m_fScale );
    point.y = int( point.y/m_fScale );
  }
  return point;
}

BOOL KSafetyManagerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  if ( m_ptMouse.x > 0 && m_ptMouse.y > 0 )
  {
    SetLapa();
    return 0;
  }
  //
  POINT pt;
  ::GetCursorPos( &pt );
  ScreenToClient( &pt );
  CPoint ptOffset = GetDeviceScrollPosition();
  pt.x = (int)(double(pt.x+ptOffset.x)/m_fScale);
  pt.y = (int)(double(pt.y+ptOffset.y)/m_fScale);


  KSafetyManagerDoc *pCur = GetDocument();
  LFscBase* obj = pCur->m_pOwner->FscInRect( m_curFsc, pt );
  if( obj )
  {
    SetHand();
    return 0;
  }

  return __super::OnSetCursor(pWnd, nHitTest, message);
}

void KSafetyManagerView::OnTimer(UINT_PTR nIDEvent)
{
  Invalidate( FALSE );
  __super::OnTimer(nIDEvent);
}

void KSafetyManagerView::QuickWatch( LFscBase* obj )
{
  IFscStorage::SVarInfo vars[10] = { 0 };
  LPCSTR pPntName = NULL;
  int c = GetDocument()->m_pOwner->GetVars( obj, vars, _countof(vars), &pPntName );
  if( !c )
    return;
  KEditFix dlg( vars, c );
  if( pPntName && *pPntName )
    dlg.m_strDlgCaption = pPntName;
  dlg.DoModal();
}

void KSafetyManagerView::Action( LFscBase* obj )
{
  // Возврат на исходный файл
  IFscStorage *pCur = GetDocument()->m_pOwner;
  if( obj->mT == 0x0D  //"Дискретный переход доставка";
    ||   obj->mT == 0x0E  //"Аналоговый переход доставка";
    ||   obj->mT == 0x0F  //"Дискретный переход доставка от multi";
    ||   obj->mT == 0x10 )//"Аналоговый переход доставка от multi";
  {
    SGoto* dat = (SGoto*)pCur->FscGiveData( obj->nBuff );
    SetNumb( dat->nSrc );
  }
  // Переход по ссылке
  else if( obj->mT == 0x15  //"Дискретный переход";
        || obj->mT == 0x16 )//"Аналоговый переход";
  {
    SGoto* dat = (SGoto*)pCur->FscGiveData( obj->nBuff );
    SetNumb( dat->nDst );
  }
  // Выбор схемы
  else if ( obj->mT == 0x17  //"Аналоговый переход (Мульти)";
          ||obj->mT == 0x18 )//"Дискретный переход (Мульти)";
  {
    SGotoN* dat = (SGotoN*)pCur->FscGiveData( obj->nBuff );
    /*CDialogGoto dlg( dat->nDst );
    if ( dlg.DoModal() == IDOK )
      SetNumb( dat->nDst[dlg.N] );*/
  }
  // Вызов окна процедуры
  else if ( obj->mT == 0x41 )//"Процедура";
  {
    SGotoN* dat = (SGotoN*)pCur->FscGiveData( obj->nBuff );
    /*CDialogGoto dlg( dat->nDst );
    if ( dlg.DoModal() )
      SetNumb( dat->nDst[dlg.N] );*/
  }
  //>>>
}

void KSafetyManagerView::SetNumb( UINT nFile )
{
  UINT nCount = 0;
  LFscFile* mFile = GetDocument()->m_pOwner->FscFileList( nCount );
  for( UINT n = 0; n < nCount; ++n )
  {
    LFscFile& item = mFile[n];
    if( item.nNumber == nFile )
    {
      //PostMessage( WM_USER, -1, n );
      GetDocument()->OnSelectFsc( (LPCSTR)-1, n );
      return;
    }
  }
}
