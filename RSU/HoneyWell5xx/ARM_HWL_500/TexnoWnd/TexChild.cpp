#include "stdafx.h"
#include "../ARM.h"
#include "TexnoFrm.h"
#include "BaseType.h"
#include "Queue.h"
#include "Resource.h"
//#include "InlineGDI.h"
#include "DialName.h"
#include "BaseWnd.h"
#include "CommProc.h"
#include "SoftGDI.h"
#include "TestTime.h"
#include "ZadvigkiFromKlapan.h"

#include "LogFile.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CTexChild
CTexChild::CTexChild(CTexMini* pMini,HWND& hParentUSB,int nMonitor)
: CArrayObj(nMonitor)
//, sockClient("ТехноСхема")
, m_pMini(pMini)
, m_Anchor(this)
, m_nScale(100)
, m_ptOffset(0,0)
, m_hBmpMeta(NULL)
, m_hBmpStat(NULL)
, m_nPressed(-1)
, m_ptMouse(0,0)
, m_nSelect(0)
, m_hEmf(NULL)
, m_hParentUSB(hParentUSB)
, m_bPaintStatic(true)
, m_hTread(INVALID_HANDLE_VALUE)
, m_psockClient(NULL)
{
  m_hEmf = EmfCreate();
  
  if(theApp.m_bFourMonitors)
  {
    if(nMonitor>0)
    {
    char name[64];
    sprintf_s(name, "ТехноСхема%d", nMonitor+1);
    m_psockClient = new CSocketClient(name);
    m_psockClient->InitAndSendKey();
    }
  }
  else
  {
    m_psockClient = new CSocketClient("ТехноСхема");
    m_psockClient->InitAndSendKey();
  }
}

CTexChild::~CTexChild()
  {
  SaveScale();
  ::EmfDelete(m_hEmf);
  if(m_hTread!=INVALID_HANDLE_VALUE)
    {
    ::TerminateThread(m_hTread, -1);
    ::CloseHandle(m_hTread);
    }
  if(m_psockClient)
    delete m_psockClient;
  }

BEGIN_MESSAGE_MAP(CTexChild, CWnd)
  ON_WM_CREATE()
  ON_MESSAGE(WM_MBUTTONDOWN, HandleMButtonDown2)
  ON_WM_MEASUREITEM()
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_SETCURSOR()
  ON_WM_KEYDOWN()
  ON_WM_CONTEXTMENU()
  ON_WM_DRAWITEM()
  ON_WM_MOUSEWHEEL()
	ON_REGISTERED_MESSAGE(nPressQueue,OnEventQueueMsg)
END_MESSAGE_MAP()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct CRestSaveScheme
{
	int nScale;
	int x;
	int y;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::SaveScale()
  {
  CRestSaveScheme rd = {m_nScale,m_ptOffset.x,m_ptOffset.y};
  if ( !m_strName.IsEmpty() )
    {
    WriteBinary(_T("Sheme"),KeyName(m_strName ),(LPBYTE)&rd,sizeof(rd));
    WriteString(_T("Sheme"),KeyName("LastView"),m_strName);
    }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if ( m_hBmpMeta ) ::DeleteObject( m_hBmpMeta );m_hBmpMeta = NULL;
  if ( m_hBmpStat ) ::DeleteObject( m_hBmpStat );m_hBmpStat = NULL;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::RestScale(LPCTSTR pszGoto)
  {
  LPBYTE pData = NULL;
	UINT size = 0;
  BOOL bSuc = GetBinary(_T("Sheme"),KeyName(m_strName),&pData,&size);
	if ( bSuc && pData != NULL && size == sizeof(CRestSaveScheme))
	  {
		CRestSaveScheme* prd = (CRestSaveScheme*)pData;
		m_ptOffset.x = prd->x;
		m_ptOffset.y = prd->y;
    int nMin = MinScale();
    int nMax = 500;
    //
    //prd->nScale = max(prd->nScale,nMin);
    //prd->nScale = min(prd->nScale,nMax);
    m_nScale = prd->nScale;
    delete [] pData;
    }
  else
    {
    m_nScale = MinScale(1);
    }
  //
  // Подсвечиваем регион, откуда пришли
  if ( pszGoto != NULL )
    {
    int nCnt = mTags.Count();
    STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
	  for( int n = 0; n < nCnt; n++ )
    if ( obj[n].m_eType == id_Goto )
    if ( lstrcmpi(pszGoto,mTags.Name(n)) == 0 )
	    {
      SetObjToCenter(n);
      return;
      }
    }
  LimitShift( NULL, NULL );
  Invalidate( false );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* CTexChild::SetActive(const char* pszFile)
  {
  const char* menu = NULL;
  if ( ChangeMenu( pszFile ) )
    {
    const char* file = GetString(_T("Sheme"),KeyName(_T("LastView")),"");
    if ( !*file )
      {
      HXML nFirst = NextMenu();
      if ( nFirst == INVALID_HXML ) return menu;
      menu = Param( nFirst, "menu" );
      file = Param( nFirst, "file" );
      }
    else
      menu = FileToMenu( file );
    LoadData( file );
    }
  return menu;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int CTexChild::LimitShift( int* dx, int* dy, bool bInvalidate )
  {
  RECT rect;
  GetClientRect( &rect );

  int mx = MulDiv(EmfW(m_hEmf),m_nScale,100)-rect.right;
  if ( m_ptOffset.x > mx )
    {
    if ( dx ) *dx -= (mx-m_ptOffset.x);
    m_ptOffset.x = mx;
    }
  int my = MulDiv(EmfH(m_hEmf),m_nScale,100)-rect.bottom;
  if ( m_ptOffset.y > my )
    {
    if ( dy ) *dy -= (my-m_ptOffset.y);
    m_ptOffset.y = my;
    }
  if ( m_ptOffset.x < 0 )
    {
    if ( dx ) *dx += m_ptOffset.x;
    m_ptOffset.x = 0;
    }
  if ( m_ptOffset.y < 0 )
    {
    if ( dy ) *dy += m_ptOffset.y;
    m_ptOffset.y = 0;
    }
  //
  m_pMini->Invalidate(false);
  //
  if ( dy == NULL || dx == NULL ) return 1;

  if ( *dx == 0 && *dy == 0 )
    return 0;
//
  RECT rc = rect;
  if ( *dx < 0 ) {rc.left = rect.right + *dx;}
  if ( *dx > 0 ) {rc.right = rect.left + *dx;}
  rc = rect;
  if ( *dy < 0 ) rc.top = rect.bottom + *dy;
  if ( *dy > 0 ) rc.bottom = rect.top + *dy;
  if ( bInvalidate )
    {
    HRGN hRgn = ::CreateRectRgnIndirect( &rc );
    if ( hRgn != NULL )
      {
      HRGN hrgn = ::CreateRectRgnIndirect( &rect );
      if ( hrgn != NULL )
        {
        ::CombineRgn( hRgn, hRgn, hrgn, RGN_OR );
        ::DeleteObject( hrgn );
        ::ScrollWindowEx( m_hWnd, *dx, *dy, NULL, NULL,hRgn,NULL,SW_INVALIDATE);
        }
      ::DeleteObject( hRgn );
      }
    }
  else
    {
    ::ScrollWindowEx( m_hWnd, *dx, *dy, NULL, NULL,NULL,NULL,SW_SMOOTHSCROLL);
    }
  return 0;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Определяем минимальный масштаб
int CTexChild::MinScale(int n)
  {
  // 1 - по ширине
  // 2 - по высоте
  RECT rect;
  ::GetClientRect( m_hWnd, &rect );
  //
  int hw = EmfW(m_hEmf);
  int hh = EmfH(m_hEmf);
  //
  int ww = rect.right-rect.left;
  if ( ww == 0 ) ww = ::GetSystemMetrics(SM_CXSCREEN);
  int wh = rect.bottom-rect.top;
  //
  if ( wh == 0 ) { n = 2; wh = ::GetSystemMetrics(SM_CYSCREEN);}
  //
  int mx = MulDiv(ww,100,hw);
  int xm = MulDiv(mx,hw,100);
  if ( xm > ww ) mx--;
  int my = MulDiv(wh,100,hh);
  int ym = MulDiv(my,hh,100);
  if ( ym > wh ) my--;
  if ( n == 1 ) return mx;
  if ( n == 2 ) return my;
  return min(mx,my);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int CTexChild::SetScale(int nScale)
  {
  int nMin = MinScale();
  int nMax = 500;
  //
  nScale = max(nScale,nMin);
  nScale = min(nScale,nMax);
  //
  if ( m_nScale == nScale )
    return 0;// Используется какой-то предельный масштаб
  //
  POINT point;
  ::GetCursorPos( &point );
  ::ScreenToClient( m_hWnd, &point );
  //
  m_ptOffset.x += MulDiv(m_ptOffset.x+point.x,nScale-m_nScale,m_nScale);
  m_ptOffset.y += MulDiv(m_ptOffset.y+point.y,nScale-m_nScale,m_nScale);
  //
  m_nScale = nScale;
  LimitShift( NULL, NULL );
  Invalidate( false );
  return 0;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void ShowHide( CWnd* pWnd, CWnd* This, int nCmdShow )
  {
  pWnd = pWnd->GetWindow( GW_HWNDFIRST );
  while ( pWnd )
    {
    if ( pWnd->GetOwner() == This )
      pWnd->ShowWindow( nCmdShow );
    pWnd = pWnd->GetNextWindow();
    }
  }
//
void CTexChild::ShowOrHide(int nCmdShow)
  {
  CWnd* pWnd = this;
  while ( pWnd )
    {
    ShowHide( pWnd, this, nCmdShow );
    pWnd = pWnd->GetParent();
    }
  }

DWORD WINAPI TexnoShem(LPVOID lpData)
{
	Addr* pAddr = (Addr*)lpData;
	while(true)
	{
		WaitForSingleObject(pAddr->hEvent, INFINITE);
		if (pAddr->pSockClient->Count()>0) 
		{
			pAddr->pSockClient->Refresh();
			int nCnt = pAddr->pSTagsObj->Count();
			STagsObj* obj = (STagsObj*)pAddr->pSTagsObj->m_szBuffer;
			for( int n = 0; n < nCnt; n++ )
			{
				if ( obj[n].m_nSocketNumber != -1 )
				{
					UINT eType=pAddr->pSockClient->GetMnemoType(obj[n].m_nSocketNumber );
					if (eType==id_ModelData)
					{
						LPCTSTR szName=pAddr->pSUniChar->Name(obj[n].m_nName);
						LPCTSTR szComm=pAddr->pSUniChar->Name( obj[n].m_nComm );
						pAddr->pSockClient->GetModelData(eType,obj[n].m_nSocketNumber, szName, szComm);
					}
				}
			}
		}
	}
	return 0;
}

int CTexChild::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
  //
  if ( !::IsWindow(m_ToolTip) )
	m_ToolTip.Create(this, TTS_ALWAYSTIP|TTS_NOPREFIX);
	m_ToolTip.AddTool(this,_T(""));
  //
  if ( !::IsWindow(m_Anchor) )
  m_Anchor.Create();
  if(m_psockClient)
     addr.pSockClient = m_psockClient;
  else addr.pSockClient = &theApp.mSocketTexno;
  addr.pSTagsObj =&mTags;
  addr.pSUniChar =&mName;
  addr.hEvent = hEvent;
  m_hTread = CreateThread(NULL, 0, TexnoShem, &addr, 0, NULL);
  //
	return 0;
}

LRESULT CTexChild::HandleMButtonDown2(WPARAM wParam, LPARAM lParam)
  {
	UINT nFlags = static_cast<UINT>(wParam);
	CPoint point(lParam);

	if (nFlags & (MK_SHIFT | MK_CONTROL) )
	  {
		CWnd::OnMButtonDown(nFlags, point);
		return FALSE;
	  }
  //
  if ( !::IsWindow(m_Anchor) )
    return TRUE;
  //
  if ( m_Anchor.IsWindowVisible() )
    m_Anchor.Anchord(NULL);
  else
    m_Anchor.Anchord(&point);
  //
	return TRUE;
  }

BOOL CTexChild::PreTranslateMessage(MSG* pMsg)
  {
  m_ToolTip.RelayEvent(pMsg);
  return CWnd::PreTranslateMessage(pMsg);
  }

const char* CTexChild::LoadData( HXML nItem )
  {
  const char* menu = Param( nItem, "menu" );
  const char* file = Param( nItem, "file" );
  LoadData( file );
  return menu;
  }

void CTexChild::LoadData( LPCTSTR pszName, LPCTSTR pszGoto )
  {
  if ( pszName == NULL )
    {
	static char* def;
	std::string strTexName=theApp.GetTexnoshemeNameCSV().c_str();
	if (strTexName!="")
		def=_strdup(strTexName.c_str());
	else
		def= "К-1";
    pszName = def;
    }
  //
  if ( pszName && lstrcmp(m_strName,pszName) == 0 )
    return;
  m_nSelect = 0;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  CModelInfo& model = ::ModelInfo();
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SaveScale();
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //::ShowWindow( m_hWnd, SW_HIDE );
  //
  int nError;
  HMyEMF hEmf = LoadCrd( pszName, nError );
  if ( hEmf == NULL )
    {
    CString str;
    if ( nError == -1 )
    str.Format( "'%s'. Ошибка создания EMF объекта.", pszName );
    else
    if ( nError == -2 )
    str.Format( "Ошибка открытия '%s.emf'.", pszName );
    else
    if ( nError == -3 )
    str.Format( "Ошибка открытия '%s.crd'.", pszName );
    else
    if ( nError > 0 )
    str.Format( "У файла '%s' ошибка в строке %d!", pszName, nError );
    else
    if ( nError < 0 )
    str.Format( "'%s'. Ошибка заказа памяти %d", pszName, nError );
    AfxMessageBox( str, MB_ICONEXCLAMATION );
    return;
    }
  else
    {
    ::EmfDelete(m_hEmf);
    m_hEmf = hEmf;
	if (m_pMini->IsWindowVisible())
		m_pMini->BuildBmp();
    }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if(m_psockClient == NULL)
    BuildClient(theApp.mSocketTexno);
  else BuildClient(*m_psockClient);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  char szBuffer[_MAX_PATH];
  if ( lstrcmpi( pszName, model.namScheme ) )
    sprintf_s(szBuffer,sizeof(szBuffer),"Технологическая схема '%s'",FileToMenu(pszName));
  else
    sprintf_s(szBuffer,sizeof(szBuffer),"Общая технологическая схема %s",model.Name);
  //
  char* ptr = szBuffer;
  while ( *ptr )
    {
    if ( *ptr == '&' )
      {
      char* str = ptr;
      while ( *str ) str[0] = str[1],str++;
      }
    ptr++;
    }
  //
  ::ShowWindow( m_hWnd, SW_SHOW );
  if ( pszName == NULL ) pszName = model.namScheme;
  m_strName = pszName;
  RestScale(pszGoto);
  ::SetCurrentTex( m_pMini->m_hChild, m_strName );
  ::MainTimer( m_pMini->m_hChild, 0 );//Invalidate(false);
  if ( ::IsWindow( m_hParentUSB ) )
    {
    ::SetCurrentTex( m_hParentUSB, m_strName );
    ::MainTimer( m_hParentUSB, 0 );//Invalidate(false);
    }
  if ( Version == LG35_8_KF)
		SetScale(MinScale());
  SetFocus();
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  }

void CTexChild::OnPaint()
  {
  CPaintDC dc(this); // device context for painting
  Paint( dc );
  }

void CTexChild::Paint(CDC& dc)
  {
  CTestTime test;
  int c2,c3,c4,c5,c6,c7;
  //
  RECT rect;
  GetClientRect( &rect );
  //
  int nWidth  = rect.right-rect.left;
  int nHeight = rect.bottom-rect.top;
  //
  ::GiveBitmap( dc, nWidth, nHeight, m_hBmpMeta );
  ::GiveBitmap( dc, nWidth, nHeight, m_hBmpStat );
  //
  int t1 = 0;
  //
  if ( m_hBmpMeta != NULL && m_hBmpStat != NULL )
    {
    HDC hdc = ::CreateCompatibleDC( dc );
    if ( hdc != NULL )
      {
      HBITMAP oldBmp = (HBITMAP)::SelectObject( hdc, m_hBmpMeta );
      //
      HDC idc = ::CreateCompatibleDC( dc );
      if ( idc != NULL )
        {
        c2 = test.MS();
        HBITMAP ildBmp = (HBITMAP)::SelectObject( idc, m_hBmpStat );
        if ( m_bPaintStatic )
          {
          ::FillRect( idc, &rect, (HBRUSH)::GetStockObject(BLACK_BRUSH) );
          CMyWT wt( idc, FLOAT(m_nScale*0.01), m_ptOffset );
          bool bEmf  = ::IsMnemoStyle(MNEMO_METAFILE);
          bool bRect = ::IsMnemoStyle(MNEMO_BITMAPS );
		  if((Version==T1163_ONPZ)&&(m_strName=="1163"))
		  {
		     bEmf=true;
		  }
          t1 = EmfPaint( m_hEmf, idc, bEmf, bRect );
          }
        ::BitBlt( hdc, 0,0, nWidth, nHeight, idc, 0, 0, SRCCOPY );
        ::SelectObject( idc, ildBmp );
        ::DeleteDC(idc);
        c3 = test.MS();
        }
      //
      {
      CMyWT wt( hdc, FLOAT(m_nScale*0.01), m_ptOffset );
      if(m_psockClient == NULL)
        DrawAllShapes( hdc, theApp.mSocketTexno );
      else DrawAllShapes( hdc, *m_psockClient );
      Select( hdc );
      c4 = test.MS();
      }
      c5 = test.MS();
      ::BitBlt( dc, 0,0, nWidth, nHeight, hdc, 0, 0, SRCCOPY );
      c6 = test.MS();
      ::SelectObject( hdc, oldBmp );
      ::DeleteDC(hdc);
      }
    }
  c7 = test.MS();
  //
  if ( ::IsMnemoStyle(MNEMO_TIME) )
    {
    char szTime[128];
    sprintf_s(szTime,sizeof(szTime),"Время %.2f Play %.2f Shapes %.2f BitBlt %.2f K=%d Фокус=%08lX",
      c7,c3-c2,c4-c3,c6-c5,t1, (int)(INT_PTR)::GetFocus() );
    COLORREF ss = dc.SetTextColor(0xc00000);
    dc.TextOut( 20, 40, szTime, lstrlen(szTime) );
    dc.SetTextColor(ss);
    }
  }

void CTexChild::Select( HDC hDC )
  {
  if ( (m_nSelect % 2) == 0 ) return;
#if 0
  //::InvertRect( hdc, &m_rcSelect );
  TransparentRect( hdc, m_rcSelect, 0x800080, 0xffffff );
#else
  static HBITMAP hSelect = NULL;
  int nSrcW = m_rcSelect.Width();
  int nSrcH = m_rcSelect.Height();
  GiveBitmap( hDC, nSrcW, nSrcH, hSelect );
  if ( hSelect )
    {
    HDC idc = ::CreateCompatibleDC( hDC );
    if ( idc )
      {
      HBITMAP hBitmap = (HBITMAP)::SelectObject( idc, hSelect );
      HBRUSH hBrush = ::CreateSolidBrush( 0x800080 );
      if ( hBrush )
        {
        RECT rect = {0,0,nSrcW,nSrcH};
        ::FillRect( idc, &rect, hBrush );
        ::DeleteObject( hBrush );
        }
      ::BitBlt( hDC, m_rcSelect.left, m_rcSelect.top, nSrcW, nSrcH, idc, 0, 0, SRCAND );
      ::SelectObject( idc, hBitmap );
      ::DeleteDC( idc );
      }
    }
#endif
  }

BOOL CTexChild::OnEraseBkgnd(CDC* pDC)
  {
  return FALSE;
  }

void CTexChild::OnLButtonDown(UINT nFlags, CPoint point)
  {
  if ( ::IsWindow( m_hParentUSB ) )
    {
    ::SetCurrentTex( m_hParentUSB, m_strName );
    ::MainTimer( m_hParentUSB, 0 );//Invalidate(false);
    }
  POINT pt;
  pt.x = MulDiv(point.x+m_ptOffset.x,100,m_nScale);
  pt.y = MulDiv(point.y+m_ptOffset.y,100,m_nScale);
  int nPress = FindShapeFromPoint(pt);
  if ( nPress >= 0 )
    {
    m_nPressed = nPress;
    }
  else
    {
    m_ptMouse = point;
    SetLapa();
    }
  CWnd::OnLButtonDown(nFlags, point);
  }

void CTexChild::OnLButtonUp(UINT nFlags, CPoint point)
  {
  POINT pt;
  pt.x = MulDiv(point.x+m_ptOffset.x,100,m_nScale);
  pt.y = MulDiv(point.y+m_ptOffset.y,100,m_nScale);
  if ( m_nPressed >= 0 )
    {
    int nPress = FindShapeFromPoint(pt);
    if ( m_nPressed == nPress )
      {
      STagsObj& obj   = mTags.Tags(nPress);
      CString strComm = mTags.Comm(nPress);
      CString strName = mTags.Name(nPress);
      switch ( obj.m_eType )
        {
        case id_Goto://Переход на другую мнемосхему
          {
          int Dvd = strComm.Find ( '|' );//Переход
          if ( Dvd >= 0 )
            strComm = strComm.Left ( Dvd );
	        if ( strComm.IsEmpty() )
            AfxMessageBox("Имя файла перехода пустое");
          else
            LoadData( strComm, strName );
          }
          break;
        case id_Library://Вызов моделеспецифичного диалога
		  if ( (GetAsyncKeyState ( VK_CONTROL ) & 0x8000) == 0 )
				CallDialog(strName);
		  else
		  {
					CString strFind=theApp.FindPointInArray(obj.m_eType,strName);
					if (strFind!="Не найдены")
							GoToPanoram3D(strFind);
					else
						AfxMessageBox("Точка не найдена на панорамах");
		  }
          break;
        case id_SensPlace://Эти датчики сами показывают себя
        case id_Level    ://Уровень только показывает данные
          if ( (GetAsyncKeyState ( VK_CONTROL ) & 0x8000) == 0 )
          break;
        default:
            {             
			EDataTypes eType;
            if(m_psockClient == NULL)
               eType  = (EDataTypes)theApp.mSocketTexno.GetMnemoType( obj.m_nSocketNumber );
            else eType  = (EDataTypes)m_psockClient->GetMnemoType( obj.m_nSocketNumber );
            if (( obj.m_nSocketNumber < 0 )||(obj.m_eType==id_GasAnalyz))
              eType = (EDataTypes)obj.m_eType;
		if ( obj.m_eType == id_Klapan )
			  {
				  if ( strName[0] == '.' )
					{
						if (CZadvigkiFromKlapan::IsZadvigkaFromKlapanSborka(strName.GetBuffer()))
						{
							eType=id_Klapan;
							strName=GetKlapanName(strName);
						}
					}
			  }
				if ( (GetAsyncKeyState ( VK_CONTROL ) & 0x8000) == 0 )
				{
					int nDialog=0;
					nDialog=theApp.GetDialogNumber( strName,nDialog);
					if (nDialog!=-2)
					{
						if (eType==id_ModelData)
							CreateObject( eType, obj.nModelDialog, strName, strComm );
						else
							CreateObject( eType, obj.m_eType, strName, strComm );
					}
					else
					{
						CString strPultName=theApp.GetPultName(strName);
						if (strPultName!="")
						{
							CString strName="Pults,"+strPultName+",0";
							CallDialog(strName);
						}
					}
				}
				else
				{
					CString strFind;
					if ((obj.m_eType==id_ModelData)&&(obj.nModelDialog>id_ModelData))
						strFind=theApp.FindPointInArray(obj.nModelDialog,strName);
					else
						strFind=theApp.FindPointInArray(obj.m_eType,strName);
					if (strFind!="Не найдены")
							GoToPanoram3D(strFind);
					else
						AfxMessageBox("Точка не найдена на панорамах");
				}
            }
          break;
        };
      }
    }
  else SetArrow();
  m_nPressed = -1;
  CWnd::OnLButtonUp(nFlags, point);
  }

void CTexChild::GoToPanoram3D(CString strFind)
{
	if (theApp.IsNewPanoram())
	{
		CString strPointName;
		std::string strPoint;
		//"Шаровая задвижка;200-V-0216;1356.xml"
		int nIndexOf1=-1;
		nIndexOf1=strFind.Find(";");
		int nIndexOf2=-1;
		if (nIndexOf1!=-1)
			nIndexOf2=strFind.Find(";",nIndexOf1+1);
		if (nIndexOf2!=-1)
		{
			strPointName=strFind.Mid(nIndexOf1+1,nIndexOf2-nIndexOf1-1);
			std::string strTemp(strPointName.GetBuffer());
			strPoint=strTemp;
			theApp.SetPanoramNameInSharedMemory(strPoint);
		}
		HWND hwnd=::FindWindow(NULL,"Panorams");
		if (hwnd!=NULL)
		{
			::SetForegroundWindow(hwnd);
			::ShowWindow(hwnd,SW_SHOWNORMAL);
		}
		else
		{
			theApp.StartNewPanoram3D();
		}
	}
	else
	{
		HWND hwnd=::FindWindow("Panorama3DClass",NULL);
		if (hwnd!=NULL)
		{
			::SetForegroundWindow(hwnd);
			::ShowWindow(hwnd,SW_SHOWNORMAL);
			if (::IsWindowVisible(hwnd))
			{
				COPYDATASTRUCT cd;
				char buffer[128];
				strcpy_s(buffer,strFind);
				cd.lpData=&buffer;
				cd.cbData= sizeof(buffer);
				::SendMessage(hwnd,WM_COPYDATA, 0, (LPARAM) &cd);
			}
		}
		else
			  theApp.StartPanoram3D(strFind);
	}
}


CString CTexChild::GetKlapanName(CString strName)
{
	int nLength=strName.GetLength();
	int nShift=CZadvigkiFromKlapan::GetKlapanNameShift(strName);
	return strName.Right(nLength-nShift);
}

int g_nForsDialog = -1;
LRESULT CTexChild::OnEventQueueMsg(WPARAM wParam,LPARAM lParam)
  {
  switch ( LOWORD(wParam) )
    {
    case QUEUE_NO:
      break;
    case QUEUE_GOTO:
      {
      const char* pszName = (const char*)lParam;
	  CString strScheme(pszName);
	  if (Version==BERTH_ONPZ)
	  {
		   if (strScheme.Find("LCP")==0)
			   return 0;
	  }
	  if ((strScheme=="Схема")||(strScheme.Find("Схема_меню")==0))
          ControlTexMini( pszName );
      else
          LoadData( pszName );
      }
      break;
    case QUEUE_FORSUNKA:
      {
      const char* pszName = (const char*)lParam;
      WORD hi = HIWORD(wParam);
	  g_nForsDialog=(int)hi;
	  bool bAviFors=(g_nForsDialog==8);
      CreateObject( id_Forsunka, id_Forsunka, pszName, pszName,bAviFors );
      }
      break;
    };
  return 0;
  }

void CTexChild::OnMouseMove(UINT nFlags, CPoint point)
  {
  if ( m_nPressed == -1 )
  if ( nFlags & MK_LBUTTON )
    {
    int dx = point.x-m_ptMouse.x;
    int dy = point.y-m_ptMouse.y;
    if ( !(dx == 0 && dy == 0) )
      {
      m_ptMouse = point;
      m_ptOffset.x -= dx;
      m_ptOffset.y -= dy;
      LimitShift( &dx, &dy );
      }
    }
  CWnd::OnMouseMove(nFlags, point);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int CTexChild::FindShape( LPPOINT lpPt )
  {
	POINT pt;
  if ( lpPt == NULL )
  	GetCursorPos(&pt);
  else
    pt = *lpPt;
  //
	ScreenToClient(&pt);
  pt.x = MulDiv(pt.x+m_ptOffset.x,100,m_nScale);
  pt.y = MulDiv(pt.y+m_ptOffset.y,100,m_nScale);
  return FindShapeFromPoint(pt);
  }

BOOL CTexChild::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
  {
  static int gsToolTip = -1;
         int lsToolTip = -1;
         EDataTypes eType=id_Dialog;
  if ( nHitTest == HTCLIENT )
    {
    STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
    lsToolTip = FindShape();
	  if( lsToolTip >= 0 )
	  {
		  eType=(EDataTypes)obj[lsToolTip].m_eType;
		  if ( obj[lsToolTip].m_eType == id_ShSens )
			lsToolTip = -1;
	  }
    //
    if ( lsToolTip  == -1 && m_nPressed == -1 )
    if ( (::GetKeyState( VK_LBUTTON ) & 0x8000) )
      return SetLapa();
    // Не показывать надписи на переходах
    if ( lsToolTip  != -1 )
      if ( obj[lsToolTip].m_eType == id_Goto )
        return SetGoto();
    }
  //
  if ( gsToolTip != lsToolTip )
    {
    CWnd* pWnd = CWnd::FromHandle(m_hWnd);
    if ( lsToolTip == -1 )
      {
      m_ToolTip.UpdateTipText(_T(""),pWnd);
      }
    else
      {
      char* pszName = mTags.Comm(lsToolTip);
	  if (theApp.IsModelDataType( eType))
		  m_ToolTip.UpdateTipText("",pWnd);
	  else
		 m_ToolTip.UpdateTipText(pszName,pWnd);
      }
    m_ToolTip.Activate(lsToolTip != -1);
    gsToolTip = lsToolTip;
    }
  if( lsToolTip >= 0 ) return SetHand();
  //
  return CWnd::OnSetCursor(pWnd, nHitTest, message);
  }

void CTexChild::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
  {
  //
  bool bShift = ( GetAsyncKeyState( VK_SHIFT   ) & 0x8000 ) != 0;
  bool bCtrl  = ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) != 0;
  //
  int nStepX = MulDiv(8,m_nScale,100);if ( bShift ) nStepX *= 8;
  int nStepY = MulDiv(8,m_nScale,100);if ( bShift ) nStepY *= 8;
  //
  #undef  MUMU
  #define MUMU( mu ) \
  if ( ::IsMnemoStyle(MNEMO_EDIT) ) \
    {MnemoStyleXOR( mu ); Invalidate( false );}
  //
  switch ( nChar )
    {
    case 'S': MnemoStyleXOR( MNEMO_BBOX     ); Invalidate( false );break;
    case 'M': MnemoStyleXOR( MNEMO_ANCHOR   ); break;
    //
    case 'T': MUMU( MNEMO_TIME     );break;
    case 'O': MUMU( MNEMO_NOTIME   );break;
    case 'P': MUMU( MNEMO_METAFILE );break;
    case 'B': MUMU( MNEMO_BITMAPS  );break;
    //
    case 'W': SetScale(MinScale(1));break;
    case 'H': SetScale(MinScale(2));break;
    case '0': SetScale( 50);break;
    case '1': SetScale(100);break;
    case '2': SetScale(200);break;
    case '3': SetScale(300);break;
    case '4': SetScale(400);break;
    case '5': SetScale(500);break;
    case VK_RIGHT:
      nStepX = -nStepX;
    case VK_LEFT:
      m_ptOffset.x -= nStepX;nStepY = 0;
      LimitShift( &nStepX, &nStepY );
    break;
    case VK_DOWN:
      nStepY = -nStepY;
    case VK_UP:
      m_ptOffset.y -= nStepY;nStepX = 0;
      LimitShift( &nStepX, &nStepY );
    break;
    case VK_ADD      :
    case VK_OEM_PLUS : SetScale(m_nScale+10); break;
    case VK_SUBTRACT :
    case VK_OEM_MINUS: SetScale(m_nScale-10); break;
    }
  CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BOOL CTexChild::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
  {
  if ( zDelta > 0 )
    SetScale(m_nScale+10);
  else
    SetScale(m_nScale-10);
  return CWnd::OnMouseWheel(nFlags, zDelta, pt);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::AnchorScroll(LPSIZE sz)
  {
  int cx = -sz->cx;
  int cy = -sz->cy;
  m_ptOffset.x -= cx;
  m_ptOffset.y -= cy;
  LimitShift( &cx, &cy, !::IsMnemoStyle(MNEMO_ANCHOR) );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Refresh( CWnd* pWnd, CWnd* This )
  {
  pWnd = pWnd->GetWindow( GW_HWNDFIRST );
  while ( pWnd )
    {
    if ( pWnd->GetOwner() == This )
      {
      CRuntimeClass* cls = pWnd->GetRuntimeClass();
      if ( cls != NULL )
        {
        #ifdef _AFXDLL
        CRuntimeClass* bas = cls->m_pfnGetBaseClass();
        #else
        CRuntimeClass* bas = cls->m_pBaseClass;
        #endif
        if ( bas != NULL )
        if ( lstrcmp( bas->m_lpszClassName, "CBaseWnd" ) == 0 )
          ((CBaseWnd*)pWnd)->LoadProps();
        }
      }
    pWnd = pWnd->GetNextWindow();
    }
  }
//
void CTexChild::MainTimer()
  {
  if ( !::IsWindowVisible( m_hWnd ) )
    return;
  //
  CTestTime test;
  if(m_psockClient==NULL)
  {
  if (theApp.mSocketTexno.Count()>0) 
	  SetEvent(hEvent);
  }
  else
  {
   if(m_psockClient->Count()>0)
       SetEvent(hEvent);
  }
  //if (sockClient.Count()>0) 
  //{
	  //sockClient.Refresh();
	  //RefreshModelData();
  //}
  int ms1 = test.MS();
  CDC* pDC = GetDC();
  if ( pDC )
    {
    if ( m_nSelect > 0 ) m_nSelect--;
    m_bPaintStatic = false;
    Paint( *pDC );
    m_bPaintStatic = true;
    int ms2 = test.MS();
    if ( ::IsMnemoStyle(MNEMO_TIME) )
      {
      char szTime[128];
      //sprintf_s(szTime,sizeof(szTime),"Кол-во %d Сервер %dms Вывод %dms",client.Count(),ms1,ms2-ms1);
      COLORREF ss = pDC->SetTextColor(0xFF0000);
      pDC->TextOut( 20, 20, szTime, lstrlen(szTime) );
      pDC->SetTextColor(ss);
      }
    ReleaseDC( pDC );
    }
  //
  CWnd* pWnd = this;
  while ( pWnd )
    {
    Refresh( pWnd, this );
    pWnd = pWnd->GetParent();
    }
  //
  }
//
void CTexChild::RefreshModelData()
{
	  int nCnt = mTags.Count();
	  STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
	  for( int n = 0; n < nCnt; n++ )
	  {
		    if ( obj[n].m_nSocketNumber != -1 )
			{
              if(m_psockClient == NULL)
              {
				UINT eType=theApp.mSocketTexno.GetMnemoType(obj[n].m_nSocketNumber );
				if (eType==id_ModelData)
				{
					LPCTSTR szName=mName.Name(obj[n].m_nName);
					LPCTSTR szComm=mName.Name( obj[n].m_nComm );
					theApp.mSocketTexno.GetModelData(eType,obj[n].m_nSocketNumber, szName, szComm);
				}
              }
              else
              {
				UINT eType=m_psockClient->GetMnemoType(obj[n].m_nSocketNumber );
				if (eType==id_ModelData)
				{
					LPCTSTR szName=mName.Name(obj[n].m_nName);
					LPCTSTR szComm=mName.Name( obj[n].m_nComm );
					m_psockClient->GetModelData(eType,obj[n].m_nSocketNumber, szName, szComm);
				}
              }
			}
	  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::DrawMenuItem(HDC hDC,char* name,RECT& rect,DWORD dwFlags)
  {
  if ( *name == '&' ) name++;
  //
  HFONT hFont = (HFONT)::SelectObject(hDC,GetStockObject(ANSI_VAR_FONT));
  LOGFONT lf;
  GetObject(hFont,sizeof(LOGFONT),&lf);
  lf.lfWeight = 1000;
  lf.lfHeight = MulDiv(lf.lfHeight,4,3);
  HFONT hFontB = CreateFontIndirect(&lf);
  ::SelectObject(hDC,hFontB);
  //
  int nMode = ::SetBkMode(hDC,TRANSPARENT);
  COLORREF color = ::SetTextColor( hDC, 0x00 );
  ::DrawText( hDC, name, lstrlen(name), &rect, DT_SINGLELINE|dwFlags );
//  if ( dwFlags != DT_CALCRECT )
    {
    ::OffsetRect( &rect, -1, -1 );
    ::SetTextColor( hDC, 0x800000 );
    ::DrawText( hDC, name, lstrlen(name), &rect, DT_SINGLELINE|dwFlags );
    }
  ::SetTextColor( hDC, color );
  ::SetBkMode( hDC, nMode );
  //
  ::SelectObject(hDC,hFont);
  ::DeleteObject(hFontB);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT item)
  {
	if ( item->CtlType == ODT_MENU && item->itemID == 1100 && item->itemData != NULL )
    {
    item->itemHeight = MulDiv(item->itemHeight,4,3);
    HDC hDC = ::GetDC( m_hWnd );
    if ( hDC != NULL )
      {
      RECT rect = {0,0,0,0};
      DrawMenuItem(hDC,(char*)item->itemData,rect,DT_CALCRECT);
      item->itemHeight = rect.bottom-rect.top+4;
      item->itemWidth  = rect.right-rect.left;
      ::ReleaseDC( m_hWnd, hDC );
      }
    return;
    }
  CWnd::OnMeasureItem(nIDCtl,item);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::OnDrawItem(int nIDCtl,LPDRAWITEMSTRUCT item)
  {
	if ( item->CtlType == ODT_MENU && item->itemID == 1100 && item->itemData != NULL )
    {
    RECT rect = item->rcItem;
    DrawMenuItem(item->hDC,(char*)item->itemData,rect,DT_VCENTER|DT_CENTER);
    }
  __super::OnDrawItem(nIDCtl,item);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
  {
  if ( CountMenu() == 0 )
    return;
  CPoint point;
  ::GetCursorPos( &point );
  //
  HMENU hMenu = ::CreatePopupMenu();
  if ( hMenu == NULL ) return;
  //
  UINT uBE = MF_BYCOMMAND | MF_ENABLED;
  //
  const char* name = FileToMenu(m_strName);
  if ( name == NULL ) name = m_strName;
  ::AppendMenu(hMenu,MF_BYCOMMAND | MF_DISABLED | MF_OWNERDRAW,1100,name);
  ::AppendMenu(hMenu,MF_SEPARATOR,NULL,NULL);
  //
  HMENU hScale = ScaleMenu( hMenu );
  HMENU hGoto = ::CreatePopupMenu();
  if ( hGoto )
    {
    AppendMenu(hMenu,MF_POPUP | MF_ENABLED,(UINT_PTR)(hGoto),"Переходы");
    const char* menu = FileToMenu(m_strName);
    Menu( hGoto, 3000, menu, false );
    }
  //
  ::AppendMenu(hMenu,MF_SEPARATOR,NULL,NULL);
  ::AppendMenu(hMenu,uBE,1114,"Поиск объекта...");
  ::AppendMenu(hMenu,uBE,1113,"Печать схемы...");
  //
  #define TPM (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON)
  UINT nReturn = ::TrackPopupMenu(hMenu,TPM|TPM_RETURNCMD,point.x,point.y,0,m_hWnd,NULL);
  if ( hScale ) ::DestroyMenu(hScale);
  if ( hGoto  ) ::DestroyMenu(hGoto );
  ::DestroyMenu(hMenu);
  if ( nReturn >= 3000 )
    LoadData( HXML(nReturn - 3000) );
  else
  if ( nReturn > 1000 )
    MenuContext(nReturn);
  }
//
HMENU CTexChild::ScaleMenu( HMENU hMenu )
  {
  static int nRes[10] = {25,50,75,100,150,200,300,400,500,100000};
  HMENU hScale = ::CreatePopupMenu();
  if ( hScale == NULL ) return NULL;
  //
  UINT uBE = MF_BYCOMMAND | MF_ENABLED;
  int nMin = MinScale();
  char szItem[256];
  sprintf_s( szItem, sizeof(szItem), "Масштаб\t%d%%", m_nScale );
  AppendMenu(hMenu,MF_POPUP | MF_ENABLED,(UINT_PTR)(hScale),szItem);
  for ( int n = 0; n < 9; n++ )
    {
    if ( ( nRes[n] % 100 ) == 0 )
      sprintf_s( szItem, sizeof(szItem), "%d%%\t%d",nRes[n],( nRes[n] / 100 ) );
    else
      sprintf_s( szItem, sizeof(szItem), "%d%%",nRes[n] );
    if ( nRes[n] == m_nScale )
      ::AppendMenu(hScale,uBE|MF_CHECKED,1101+n,szItem);
    else
    if ( nMin <= nRes[n] )
      ::AppendMenu(hScale,uBE,1101+n,szItem);
    }
  //
  ::AppendMenu(hScale,MF_SEPARATOR,NULL,NULL);
  ::AppendMenu(hScale,uBE,1122,"По ширине\tW");
  ::AppendMenu(hScale,uBE,1123,"По высоте\tH");
  //
  ::AppendMenu(hScale,MF_SEPARATOR,NULL,NULL);
  if ( m_nScale < 500 )
  ::AppendMenu(hScale,uBE,1120,"Увеличить\t+");
  if ( m_nScale > MinScale() )
  ::AppendMenu(hScale,uBE,1121,"Уменьшить\t-");
  return hScale;
  }
//
static CDialName dlgName;
void CTexChild::MenuContext(UINT nID)
  {
  int nScale = m_nScale;

  switch ( nID )
    {
    case 1101: nScale =  25;break;
    case 1102: nScale =  50;break;
    case 1103: nScale =  75;break;
    case 1104: nScale = 100;break;
    case 1105: nScale = 150;break;
    case 1106: nScale = 200;break;
    case 1107: nScale = 300;break;
    case 1108: nScale = 400;break;
    case 1109: nScale = 500;break;
    case 1113:
      PrintSxema();
    return;
    case 1114:
      if ( dlgName.Create( this ) )
        dlgName.ShowWindow( SW_SHOW );
    return;
    case 1120: nScale = m_nScale+10; break;
    case 1121: nScale = m_nScale-10; break;
    case 1122: nScale = MinScale(1); break;
    case 1123: nScale = MinScale(2); break;
    default: return;
    }
  SetScale( nScale );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PrintEMF( HDC hDC, HMyEMF hEmf, int w, int h )
  {
  int nWidth  = ::GetDeviceCaps( hDC, PHYSICALWIDTH  );
  int nHeight = ::GetDeviceCaps( hDC, PHYSICALHEIGHT );
  int nOffX   = ::GetDeviceCaps( hDC, PHYSICALOFFSETX );
  int nOffY   = ::GetDeviceCaps( hDC, PHYSICALOFFSETY );
  nWidth  -= 2*nOffX;
  nHeight -= 2*nOffY;
  //
  double kx = double(nWidth )/w;
  double ky = double(nHeight)/h;
  if ( kx < ky )
    nHeight = MulDiv( nWidth, h, w );
  else
    nWidth = MulDiv( nHeight, w, h );
  //
  RECT rect = {0, 0, nWidth, nHeight };
  HBITMAP hBitmap = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
  if ( hBitmap == NULL  )
    {
    AfxMessageBox( "Ошибка печати", MB_ICONSTOP );
    return;
    }
  //
  int nError = 1;
  //
  HDC hdc = ::CreateCompatibleDC( hDC );
  if ( hdc != NULL )
    {
    HBITMAP hOld = (HBITMAP)::SelectObject( hdc, hBitmap );
    //
    HBRUSH hBrush = ::CreateSolidBrush( 0x000000 );
    if ( hBrush != NULL )
      {
      ::FillRect( hdc, &rect, hBrush );
      ::DeleteObject( hBrush );
      }
    //
    ::EmfPaint(hEmf,hdc,rect);
    ::InvertRect( hdc, &rect );
    //
    DOCINFO di;
    memset( &di, 0, sizeof(DOCINFO) );
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = "Печать технологической схемы";
    di.lpszOutput = (LPTSTR) NULL;
    di.lpszDatatype = (LPTSTR) NULL;
    di.fwType = 0;
    if ( ::StartDoc( hDC, &di) != SP_ERROR )
      {
      if ( StartPage(hDC) > 0 )
        {
        ::BitBlt( hDC, 0, 0, nWidth, nHeight, hdc, 0, 0, SRCCOPY );
        nError = 0;
        if ( EndPage(hDC) <= 0 ) nError = 1;
        }
      if ( EndDoc(hDC) <= 0 ) nError = 1;
      }
    //
    ::SelectObject( hdc, hOld );
    ::DeleteDC( hdc );
    }
  ::DeleteObject( hBitmap );
  //
  if ( nError != 0 )
    AfxMessageBox( "Ошибка печати", MB_ICONSTOP );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::PrintSxema()
  {
  if ( m_hEmf == NULL )
    {
    AfxMessageBox( "Не найден файл.\nПечать невозможна", MB_ICONSTOP );
    return;
    }
  //
  PRINTDLG pd;
  memset( &pd, 0, sizeof( pd ) );
  pd.lStructSize = sizeof( pd );
  pd.hwndOwner   = m_hWnd;
  //
  int w = EmfW(m_hEmf);
  int h = EmfH(m_hEmf);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Меняем ориентацию на альбомную, если ширина больше высоты
  if ( h < w )
    {
    pd.Flags = PD_RETURNDEFAULT;
    BOOL bDefault = PrintDlg( &pd );
    if ( !bDefault )
      {
      AfxMessageBox( "Отсутствует принтер по умолчанию.\nПечать невозможна", MB_ICONSTOP );
      return;
      }
    LPDEVMODE mode = (LPDEVMODE)::GlobalLock( pd.hDevMode );
    if ( mode != NULL )
      {
      mode->dmOrientation = DMORIENT_LANDSCAPE;
      ::GlobalUnlock( pd.hDevMode );
      }
    }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  pd.Flags = PD_HIDEPRINTTOFILE | PD_NOPAGENUMS | PD_NOSELECTION | PD_RETURNDC;
  BOOL bResult = PrintDlg( &pd );
  if ( !bResult ) return;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LPDEVMODE mode = (LPDEVMODE)::GlobalLock( pd.hDevMode );
  if ( mode != NULL )
    {
    PrintEMF( pd.hDC, m_hEmf, w, h );
    ::GlobalUnlock( pd.hDevMode );
    }
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::SetPointToCenter(LPPOINT pt)
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  RECT rect;
  ::GetClientRect( m_hWnd, &rect );
  int xc = (rect.right+rect.left)/2;
  int yc = (rect.bottom+rect.top)/2;
  m_ptOffset.x = pt->x-xc;
  m_ptOffset.y = pt->y-yc;
  LimitShift( NULL, NULL );
  Invalidate( false );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::SetObjToCenter(int nIndex)
  {
  POINT pt;
  STagsObj& obj = mTags.Tags(nIndex);
  pt.x = MulDiv((obj.rc.right+obj.rc.left),m_nScale,200);
  pt.y = MulDiv((obj.rc.bottom+obj.rc.top),m_nScale,200);
  m_rcSelect = obj.rc;
  m_nSelect  = 20;
  SetPointToCenter(&pt);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::GetRect(LPRECT rc)
  {
  //
  RECT rect;
  ::GetClientRect( m_hWnd, &rect );
  //
  rc->left   = MulDiv(m_ptOffset.x,100,m_nScale);
  rc->top    = MulDiv(m_ptOffset.y,100,m_nScale);
  rc->right  = MulDiv(m_ptOffset.x + rect.right ,100,m_nScale);
  rc->bottom = MulDiv(m_ptOffset.y + rect.bottom,100,m_nScale);
  rc->right  = min(rc->right ,EmfW(m_hEmf));
  rc->bottom = min(rc->bottom,EmfH(m_hEmf));
  //
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* Give( HWND hWnd, const char* name, char*& ptr )
  {
  if ( ptr == NULL )
    return NULL;
  char* str = strchr( ptr, ',' );
  if ( str == NULL )
    {
    MessageBox( hWnd, name, "Ошибка формата", MB_OK | MB_ICONSTOP );
    ptr = NULL;
    return NULL;
    }
  char* q = ptr;
  *str = 0; ptr = str + 1;
  return q;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HWND Pults( HWND hParent, HWND hPostM, const char* pszPult, bool bPopup );
void CTexChild::CallDialog(const char* name)
  {
  char szPath[256];
  StrCpy( szPath, name );
  char* str = szPath;
  //
  char* szLibr = Give( m_hWnd, name, str );
  char* szFunc = Give( m_hWnd, name, str );
  if ( szLibr == NULL || szFunc == NULL )
    return;
  int nVar = atoi(str);
  //
  HWND hParent = m_hWnd;
  BOOL bChild = ::IsWindow(m_hParentUSB);
  if ( bChild )  hParent = m_hParentUSB;
  if ( nVar == 0 )
    {
    HWND hOld = ( bChild ) ? ::GetWindow( m_hParentUSB, GW_CHILD ) : NULL;
    HWND hWnd = Pults( hParent, NULL, szFunc, !bChild );
    if ( bChild && ::IsWindow( hWnd ) )
      ::SendMessage( m_hParentUSB, nPressQueue, QUEUE_CHILD_NEW, (LPARAM) hOld );
    }
  else
  if ( nVar == 1 )
    {
    }
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTexChild::ControlTexMini(const char* pszName )
{
  if (m_pMini!=NULL)
     ::SendMessage( m_pMini->GetMnemoHWND(), nBuildQueue, BUILD_MNEMO, (LPARAM)pszName );
}

 void CTexChild::HideDialog()
 {
   if( dlgName )
     dlgName.ShowWindow( SW_HIDE );
 }