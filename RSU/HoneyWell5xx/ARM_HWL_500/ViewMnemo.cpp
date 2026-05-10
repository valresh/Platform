#include "stdafx.h"
#include "ARM.h"
//#include "AmmFrm.h"
//#include "AplicFrm.h"
//#include "AliasCSV.h"
#include "ViewMnemo.h"
//#include "DlgDebug.h"
#include "HTML/TagSCRIPT.h"
#include "TestTime.h"
//#include "SafetyManagerStruct.h"
#include "SmShData.h"
//#include "QuickBuilderStruct.h"
//#include "QbShData.h"
#include "common.h"
#include "Function.h"
//#include "Queue.h"
#include "HTML/Style.h"
#include "Function.h"


extern bool g_bGlobalEditPV;// Возможность редактирования PV

CViewMnemo::CViewMnemo(KHoneyPipeClient& rClient,bool bChild)
: mDoc(rClient)
, m_bChild(bChild)
, m_hStatic(NULL)
, m_hBitmap(NULL)
, m_bPaintStat(true)
, m_pPressed(NULL)
, m_fScale(1.0)
, m_nScale( 3 )
, m_nTime(0)
//, m_hChild(NULL)
, m_lastPressed(NULL)
, m_pBoundTag(NULL)
, m_pSelectedTag(NULL)
  {
  }

  CViewMnemo::CViewMnemo()
  {

  }

CViewMnemo::~CViewMnemo()
  {
  if ( m_hStatic ) ::DeleteObject( m_hStatic );
  if ( m_hBitmap ) ::DeleteObject( m_hBitmap );
  }

// BEGIN_MESSAGE_MAP(CViewMnemo, CScrollView)
//   ON_WM_CTLCOLOR()
//   ON_WM_SETCURSOR()
//   ON_WM_PAINT()
//   ON_WM_ERASEBKGND()
//   ON_WM_LBUTTONDOWN()
//   ON_WM_LBUTTONUP()
//   ON_WM_LBUTTONDBLCLK()
//   ON_WM_MOUSEMOVE()
// 	ON_WM_CONTEXTMENU()
//   ON_MESSAGE( WM_XBUTTONDOWN, OnXButtonDown )
//   ON_WM_MBUTTONDOWN()
//   ON_WM_MOUSEACTIVATE()
//   ON_COMMAND(IDM_ALRM_ACK, OnAckOne)
//   ON_COMMAND(IDM_DET_DIS , OnDetail)
//   ON_COMMAND(IDM_ASS_DIS , OnAssocs)
//   ON_COMMAND(IDM_TRN_DIS , OnTrend)
//   ON_COMMAND(IDM_MINITRN_DIS, OnMiniTrend)
//   ON_COMMAND(IDM_GRP_DIS , OnGroup)
//   ON_COMMAND(IDM_MAN     , OnMan  )
//   ON_COMMAND(IDM_AUTO    , OnAuto )
//   ON_COMMAND(IDM_NORM    , OnNorm )
//   ON_COMMAND(IDM_RAISE   , OnRaise)
//   ON_COMMAND(IDM_LOWER   , OnLower)
//   ON_WM_MEASUREITEM()
//   ON_WM_DRAWITEM()
// END_MESSAGE_MAP()

// CViewMnemo message handlers
// BOOL CViewMnemo::PreCreateWindow(CREATESTRUCT& cs)
//   {
// 	if (!CScrollView::PreCreateWindow(cs))
// 		return FALSE;
//   cs.style = AFX_WS_DEFAULT_VIEW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
// 	cs.dwExStyle |= WS_EX_CLIENTEDGE;
// 	return TRUE;
//   }
//
//void CViewMnemo::PostNcDestroy()
//  {
  //__super::PostNcDestroy();
//  }
//
// HBRUSH CViewMnemo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//   {
//   HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);
// 	if ( nCtlColor == CTLCOLOR_EDIT )
//     pDC->SetBkMode( TRANSPARENT );
//   return hbr;
//   }

// BOOL CViewMnemo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//   {
//   if ( nHitTest == HTCLIENT )
//     {
//     POINT point;
//     GetCursorPos( &point );
//     ScreenToClient( &point );
//     CTag* pTag = LButtonDown(point);
//     if ( pTag != NULL )
//       {
//       _static HCURSOR hHand = ::LoadCursor( NULL, IDC_HAND );
//       ::SetCursor( hHand );
//       return 0;
//       }
//     pTag = TabIndexCur(point);
//     if ( pTag != NULL )
//       {
//       _static HCURSOR hBeam = ::LoadCursor( NULL, IDC_IBEAM );
//       ::SetCursor( hBeam );
//       return 0;
//       }
//     }
//   return CScrollView::OnSetCursor(pWnd, nHitTest, message);
//   }

// void CViewMnemo::OnPaint()
//   {
//   CPaintDC dc(this); // device context for painting
//   Draw( dc );
//   }

// BOOL CViewMnemo::OnEraseBkgnd(CDC* pDC)
//   {
//   return FALSE;//CScrollView::OnEraseBkgnd(pDC);
//   }
//
  bool CViewMnemo::Build(const  std::string & htmlContent)
  {

      return true;
  }

  bool CViewMnemo::Build( LPCTSTR pszName, LPCTSTR szTagGrp, bool bResize )
  {
  CTestTime test;
 // ::DestroyWindow(m_hChild);
  
  if(m_pPressed)
    ReleaseCapture(); //если по mouseDown открываеетяся новая мнемосхема

  m_pPressed = NULL;
  m_lastPressed = NULL;
  m_pBoundTag = NULL;
  m_pSelectedTag = NULL;
  // Для начала надо рассчитать регионы нажатия,
  // ( это можно сделать по всякому, но мы сделаем так )
  bool bReturn = false;
  // HDC hDC = ::GetDC( m_hWnd );
  // if ( hDC != NULL )
  //   {
  //   mDoc.m_hHDC = hDC;
  //   if ( bResize ) mDoc.pipe.Begin();
  //   bReturn = mDoc.LoadHTM( m_hWnd,pszName, szTagGrp );
  //   if ( !bReturn && ::IsMnemoStyle(MNEMO_COMMENT) )
  //     {
  //     _static HFILE hFile = _lcreat( "c:/ForDebug/return.false", 0 );
  //     if ( hFile != HFILE_ERROR )
  //       {
  //       _hwrite( hFile, pszName, lstrlen(pszName) );
  //       _hwrite( hFile, "\r\n", 2 );
  //       }
  //     }
  //   else
  //     {
  //     ASSERT(bReturn);
  //     }
  //   mDoc.pipe.End();
  //   mDoc.m_LastTagCount = mDoc.pipe.Count();
  //   //
  //   if ( !bResize )
  //     {
  //     cross::RECT cross::RECT;
  //     GetClientcross::RECT( &cross::RECT );
  //     m_fScale = GetScale( m_nScale );
  //     mDoc.ApplyLayout( m_fScale, CSize( cross::RECT.right, cross::RECT.bottom ) );
  //     }
  //   //
  //   mDoc.m_hHDC = NULL;
  //   Ccross::RECT cross::RECT;
  //   GetClientcross::RECT( cross::RECT );
  //   GiveBitmap( hDC, cross::RECT.Width (), cross::RECT.Height(), m_hBitmap );
  //   HDC hdc = ::CreateCompatibleDC( hDC );
  //   if ( hdc != NULL )
  //     {
  //     HBITMAP oldBmp = (HBITMAP)::SelectObject( hdc, m_hBitmap );
  //     mDoc.Paint( hdc );
  //     ::SelectObject( hdc, oldBmp );
  //     ::DeleteDC(hdc);
  //     }
  //   ::ReleaseDC( m_hWnd, hDC );
  //   if ( bReturn ) mDoc.SaveBin();
  //   }
  //
  if ( !bReturn )
    m_nTime = -test.MS();
  else
    m_nTime =  test.MS();
  //
  // if ( bResize )
  //   GetAppWnd()->SetActiveMnemo(m_hWnd);
  //
  SetSizes();
  
  //TODO need same controll
  // if(!m_ToolTip)
  //   {
  //     m_ToolTip.Create(this);
  //     m_ToolTip.AddTool(this, _T(""));
  //     m_ToolTip.Activate(TRUE);
  //   }

  return bReturn;
  }
//
void CViewMnemo::BuildQueue( LPCTSTR pszName, LPCTSTR pszTagGrp )
  {
  Build( pszName, pszTagGrp, true );
  //
  cross::CString strName;
  strName.Format( "%s (%s.htm)", mDoc.Title(), mDoc.m_strFile );
  if (mDoc.GetPopup())
  {
    // if ( ::IsMnemoStyle(MNEMO_EDIT))
    // 	strName.Format( "(%s.htm)", mDoc.m_strFile );
 //    else
    // 	strName=mDoc.Title();
  }
  // SetWindowText(strName);
  // GetParent()->SetWindowText(strName);

  // if (GetAppWnd()->GetFirstFrame()->GetMnemo()==this)
     //  {
  //   //только для первого фрэйма
        // strName.Format( "Station - Default - %s (%s.htm)", mDoc.Title(), mDoc.m_strFile );
        // GetAppWnd()->SetWindowText(strName);
     //  }
  // //
  // GetAppWnd()->PaintExtButton();
  // Invalidate(true);
  }

  bool CViewMnemo::DrawValue(HDC hDC, cross::RECT &cross::RECT)
  {
      return true;
  }

  bool CViewMnemo::Keyboard(WORD nCode, UINT nChar)
  {
      return true;
  }

const char* CViewMnemo::Faceplate( const char* pszName, const char * fp, const char* pv, const char * rst, const char * desc )
  {
  if (pv && rst && desc)
  {
	  char tmp[64];
	  EDataTypes nType;
	  QueryFaceplate( pszName, tmp, (UINT*)&nType, fp );
	  mDoc.m_strObj = pszName;
	  mDoc.setPPVals(pv, rst, desc);
	  Build(fp, pszName, false);
	  return pszName;
  }
  else if (fp && !strcmpi(fp, "sysdtlmos_fp.htm")){
	  static char tmp[64], tmpName[64];
	  EDataTypes nType;
	  strcpy(tmpName, pszName);
	  if (strstr(tmpName, ".MOS")) *(strstr(tmpName, ".MOS")) = 0;
	  QueryFaceplate( tmpName, tmp, (UINT*)&nType, fp );
	  mDoc.m_strObj = tmpName;
	  Build("sysdtlmos_fp", tmpName, false);
	  return tmpName;
  }/**/
  else if (fp && !strcmpi(fp, "sysdtlpos_fp.htm")){
	  static char tmp[64], tmpName[64];
	  EDataTypes nType;
	  strcpy(tmpName, pszName);
	  if (strstr(tmpName, ".POS")) *(strstr(tmpName, ".POS")) = 0;
	  QueryFaceplate( tmpName, tmp, (UINT*)&nType, fp );
	  mDoc.m_strObj = tmpName;
	  Build("sysdtlpos_fp", tmpName, false);
	  return tmpName;
  }/**/
  else
  {
  EDataTypes nType;
  static CharMP szHoney;
  LPCTSTR szQuery = QueryFaceplate( pszName, szHoney, (UINT*)&nType, fp );
  if ( szQuery == NULL ) return NULL;
  //
  if (szQuery=="DB_LSL")
	szQuery="DBLSL";

   cross::CString str;
  //
  mDoc.m_strObj = szHoney;
//  mDoc.pipe.FindObj(nType,szHoney,szHoney);

  if(strcmp(szQuery, "SCANGRPDTL")==0)
    {
    // CShCONTROLMODULE sh;
    // char *pp = NULL;
    // if(pp = strchr(szHoney, '.'))
    //   *pp = 0;
    // if(mDoc.pipe.GetData( id_CONTROLMODULE, szHoney, &sh ))
    //  {
    //  str = sh.SCANGRPDTL;
    //  if(str.IsEmpty())
    //   {
    //    if(pp)
    //      str.Format( "sysdtl%s_fp", pp+1 );
    //   }
    //  else
    //  if(str=="sncDtlSPMCa_fp")
    //   str = "sncdtlSPCMa_fp";
    //  else
    //  if( stricmp(str.GetBuffer() + str.GetLength() - _countof("_fp") +1, "_fp") != 0
    //     && stricmp(str.GetBuffer() + str.GetLength() - _countof(".htm") +1, ".htm") != 0
    //    )
    //    str += "_fp";

    //  if(stricmp(str.GetBuffer() + str.GetLength() - _countof(".htm") +1, ".htm") != 0)
    //   {// если нет берем из SCANPNTDTL
    //   char szPath[MAX_PATH];
    //   sprintf_s( szPath, SizeMP, "%s%s\\%s.htm", ::GetBasePath(), R300, str.GetBuffer() );
    //   DWORD dwAttr = GetFileAttributes( szPath );
    //   if ( dwAttr == -1 )
    //     {
    //       sprintf_s( szPath, SizeMP, "%s%s\\%s.htm", ::GetBasePath(), "Abstract\\Faceplates", str.GetBuffer() );
    //       DWORD dwAttr = GetFileAttributes( szPath );
    //       if ( dwAttr == -1 )
    //       {
    //         if(sh.SCANPNTDTL[0])
    //         {
    //           str = sh.SCANPNTDTL;
    //           str += "_fp";
    //         }
    //       }
    //     }
    //   }
    //  }
    // if(pp)
    //   *pp = '.';
    }
  else 
  if(strcmp(szQuery, "GROUPDETAILDISPLAY")==0)
    {
    char *pp = NULL;
    if(pp = strchr(szHoney, '.'))
      *pp = 0;

    //CShSMANALOG sh;
    //CShSMDISCRET sh;
    //CShSMDO sh;
    // switch(nType)
    // {
    // case id_SMFLD:
    //   {
    //   CShSMFLD sh;
    //   // if(mDoc.pipe.GetData( nType, szHoney, &sh ))
    //   //    str = sh.GROUPDETAILDISPLAY;
    //    if(str.IsEmpty())
    //      {
    //         str = "sysdtlsmflda_fp";
    //      }
    //   if(pp)
    //     *pp = '.';
    //   }
    //   break;
      

    // case id_SMBICOM: AAA();//эта пока не попалась
    // case id_SMBOCOM:
    // case id_SMDOCOM:
    // case id_SMDI:
    // case id_SMAI:
    // case id_SMDO:
    //   {
    //   CShSMFLD sh;
    //   // if(mDoc.pipe.GetData( id_SMFLD, szHoney, &sh ) && sh.RetCode != 1)
    //   //    str = sh.GROUPDETAILDISPLAY;
    //   // else str = "sysdtlsmflda_fp";
    //   if(str.CompareNoCase("sysdtlSMdevctla_3SVLV_fp") == 0 || str.CompareNoCase("sysdtlSMdevctla_PUMP_fp")==0)
    //      mDoc.m_strObj = szHoney;
    //   }
    //   break;
    }

    // if(pp)
    //   *pp = '.';
    // }
  // else
  // if(strcmp(szQuery, "INSTRUCTIONDISPLAY")==0)
  // {
  //   char *pp = NULL;
  //   if(pp = strchr(szHoney, '.'))
  //     *pp = 0;
    
  //   // CShQBANALOG sh;
  //   // if(mDoc.pipe.GetData( id_QBANALOG, szHoney, &sh ))
  //   // {
  //   //      str = sh.INSTRUCTIONDISPLAY;
  //   // }

  //   /*{
  //   CShQBSTATUS sh;
  //   if(mDoc.pipe.GetData( id_QBSTATUS, szHoney, &sh ))
  //   {
  //       AAA();
  //       // str = sh.INSTRUCTIONDISPLAY;
  //   }v
  //   */

  //   if(pp)
  //     *pp = '.';
  // }
  // else
  // str.Format( "sysdtl%s_fp", szQuery );

  // char* p = strchr( szHoney, '.' );
  // bool bSave_g_bGlobalEditPV = g_bGlobalEditPV;
  // if ( nType != id_NUMERIC )
  // if ((Version == LG35_8_KF) && (strcmp(szHoney, "PK2E1.START") == 0) || (strcmp(szHoney, "PK1E1.START") == 0))
  //    {
  //     g_bGlobalEditPV = true;
  //    }
  // else
  // if ( p ) *p = 0;
  // Build( str, szHoney, false );
  // g_bGlobalEditPV = bSave_g_bGlobalEditPV;
  // return szHoney;
  // }
  }
  }

  BOOL CViewMnemo::PreTranslateMessage(MSG *pMsg)
  {
      return true;
  }

  void CViewMnemo::SetSizes()
  {

  }

// const char* CViewMnemo::FacePlate( CHTML& html, int nNumber )
//   {
//   if ( nNumber < 0 ) return NULL;
//   const char* name;

//       //= html.pipe.GetMnemoName( nNumber );
//   return Faceplate( name );
//   }

// void CViewMnemo::Graf( HDC hDC )
//   {
//   POINT ptOffset;
// //      = GetDeviceScrollPosition();//От полос прокрутки
//   CMyWT wt( hDC, m_fScale, ptOffset );
//   int nMode = ::SetBkMode( hDC, TRANSPARENT );
//   mDoc.Paint( hDC );

//   if(Version>LG35_8_KF && mDoc.m_szTitle[0])
//     {
//      // CWnd* pWnd = GetParent();
//      // if(pWnd)
//      // {
//      // const char* cmame = typeid(*pWnd).name();
//      // if(strcmp(cmame, "class CPopupMnemo")==0)
//      //    pWnd->SetWindowTextA(mDoc.m_szTitle);
//      // }
//     }

//   ::SetBkMode( hDC, nMode );
//   }

//callback from win api
// void CViewMnemo::OnDraw(CDC* pDC)
//   {
//   if ( pDC ) Draw( *pDC );
//   }

//  bool CViewMnemo::DrawValue( HDC hDC, cross::RECT& cross::RECT )
//   {
//   return true;
//   }

void CViewMnemo::MainTimer(ULONG dwTimer)
  {
  if ( mDoc.m_bFileLoaded )
    return;
    
  // if (!theApp.m_bPole)
  //   mDoc.pipe.RefreshEx();

  // if(mDoc.m_bNeedPipeEnd &&  mDoc.pipe.Count() > mDoc.m_LastTagCount)
  // {
  //     mDoc.m_LastTagCount = mDoc.pipe.Count();
  //     mDoc.m_bNeedPipeEnd =false;
  //     mDoc.pipe.End();
  // }

  // По таймеру выводим только динамические объекты
  // if ( ::IsWindow(m_hWnd) && ::IsWindowVisible(m_hWnd) )
  //   Draw();
  // mDoc.m_nTimerCount++;
  // if(theApp.m_bFourMonitors)
  //  {
  //  if( mDoc.m_nTimerCount==2)
  //   {
  //     Zoom(m_nScale);
  //   }
  //  }
  }

// void CViewMnemo::Corcross::RECTFirstMonitorWindowSize()
// {
//  CAmmFrm* parent = (CAmmFrm*)GetParent();
 //  if (GetAppWnd()->GetFirstFrame()->GetMnemo()==this)
 //  {
    // cross::RECT pR, myR;
    // parent->GetClientcross::RECT(&pR);
    // this->GetWindowcross::RECT(&myR);
    // int pWidth = pR.right - pR.left;
    // int myWidth = myR.right - myR.left;
    // int pHeight = pR.bottom - pR.top;
    // int myHeight = myR.bottom - myR.top;
    // if ((pHeight != myHeight) && (pWidth != myWidth))
    //   SetWindowPos(NULL, 0,0,pR.right,pR.bottom,SWP_SHOWWINDOW);
 //  }
//}

void CViewMnemo::Draw( HDC _hDC )
  {
//   Corcross::RECTFirstMonitorWindowSize();
//   HDC hDC = _hDC;
//  if ( _hDC == NULL ) hDC = ::GetDC( m_hWnd ), m_bPaintStat = false;
  // Ccross::RECT cross::RECT;
  // GetClientcross::RECT( cross::RECT );
  // cross::RECT.bottom+=100;
  // int nWidth  = cross::RECT.Width ();
  // int nHeight = cross::RECT.Height();
  // if ( GiveBitmap( hDC, nWidth, nHeight, m_hStatic ) ) m_bPaintStat = true;
  // if ( GiveBitmap( hDC, nWidth, nHeight, m_hBitmap ) ) m_bPaintStat = true;
  // //
  // HDC hdc = ::CreateCompatibleDC( hDC );
  // if ( hdc != NULL )
  //   {
  //   _static LARGE_INTEGER fr,c0,c1,c2,c3;
  //   QueryPerformanceFrequency(&fr);
  //   HBITMAP oldBmp = (HBITMAP)::SelectObject( hdc, m_hBitmap );
  //   if ( DrawValue(hdc,cross::RECT) )
  //     {
  //     HDC idc = ::CreateCompatibleDC( hdc );
  //     if ( idc != NULL )
  //       {
  //       HBITMAP ildBmp = (HBITMAP)::SelectObject( idc, m_hStatic );
  //       if ( m_bPaintStat )
  //         {
  //         COLORREF color = mDoc.Color();
  //         CMyBrush brush( idc, color );
  //         brush.cross::RECT( cross::RECT );
  //         }
  //       // Копируем битмап со статическими элементами в полный битмап
  //       ::BitBlt( hdc, 0,0, nWidth, nHeight, idc, 0, 0, SRCCOPY );
  //       ::SelectObject( idc, ildBmp );
  //       ::DeleteDC(idc);
  //       }
  //     //
  //     Graf( hdc );
  //     if ( m_bChild )
  //     if ( GetAppWnd()->GetActiveMnemo() == m_hWnd )
  //       {
  //       cross::RECT.right--;
  //       cross::RECT.bottom--;
  //       CMyPen pen( hdc, 0, 3 );
  //       pen.cross::RECT( cross::RECT );
  //       }
  //     //
  //     if ( ::IsMnemoStyle(MNEMO_HIDE_SCN) )
  //       {
  //       POINT pt;
  //       ::GetCursorPos( &pt );
  //       ScreenToClient( &pt );
  //       pt = Scroll( pt, true );
  //       char szText[64];
  //       sprintf_s( szText, sizeof(szText), "%d ms (%d-%d)", m_nTime, pt.x, pt.y );
  //       int nMode = ::SetBkMode( hdc, TRANSPARENT );
  //       if ( mDoc.Color() == 0 ) ::SetTextColor( hdc, 0xffffff );
  //       ::TextOut( hdc, 10, 10, szText, lstrlen( szText ) );
  //       ::SetBkMode( hdc, nMode );
  //       }
  //     }
  //   //
  //   // Копируем битмап со всеми элементами на экран
  //   ::BitBlt( hDC, 0, 0, nWidth, nHeight, hdc, 0, 0, SRCCOPY );
  //   ::SelectObject( hdc, oldBmp );
  //   ::DeleteDC(hdc);
  //   }
//  if ( _hDC == NULL ) ::ReleaseDC( m_hWnd, hDC ), m_bPaintStat = true;
}

//   void CViewMnemo::Draw(IRenderContext RC)
//   {
//       WrapperSolidBrush sb;
//       sb.A = 0;
//       sb.B = 0;
//       sb.G = 0;
//       sb.R = 0;
//       RC.Fillcross::RECTangle(sb, 10, 10, 100, 100);
//   }

// POINT CViewMnemo::Scroll( POINT point, bool bScale )
//   {
//   // Надо трансформировать pt в соответствии со Scroll & Scale
//       POINT pt;
//   //     = GetDeviceScrollPosition();//От полос прокрутки
//   // point.x += pt.x;
//   // point.y += pt.y;
//   // if ( bScale )
//   //   {
//   //   point.x = int( point.x/m_fScale );
//   //   point.y = int( point.y/m_fScale );
//   //   }
//   return pt;
//   }

// CTag* CViewMnemo::LButtonDown(POINT point,bool bDraw)
//   {
//     CPoint pt;
//           //= Scroll(point, true);
//   CTag* pPressed = mDoc.PtInTag( point, false );
//   if  ( pPressed != NULL && bDraw )
//     {
// //    SetCapture();
//     pPressed->m_bPressed = true;
//     if(Version >= AM2_NAK)
//     {
//     // if(typeid(*pPressed) != typeid(CTagINPUT))
//     //   {//скрипт привязан не к button
//     //   // CTag *pTag = pPressed->FindType(mDoc, typeid(*pPressed));
//       // if(pTag)
//       //    pTag->m_bPressed = true;
//   //    }
//     }
//     Draw();
//     }
//   return pPressed;
//   }

// CTag* CViewMnemo::FindBoundTag(POINT point)
// {
//   POINT pt = Scroll(point, true);
//   return mDoc.PtInBoundTag( pt );
// }

// //поиск всех тегов под курсором
// int CViewMnemo::FindTags(POINT point, std::vector<CTag*>& tags)
// {
//    POINT pt = Scroll(point, true);
//    return mDoc.FindTags( pt, tags );
// }

// CTag* CViewMnemo::TabIndexCur(POINT point,bool bDraw)
//   {
//    POINT pt = Scroll(point, true);
//   CTag* pTag = mDoc.TabIndex( pt );
//   if  ( pTag != NULL && bDraw )
//     {
// 		bool bEditTag=false;
// 		//MIHAIL_HWL много ложных срабатывний, EditBox там,
// 		//где его не должно быть, там где нужно, надо будем включить вручную
// 		if (bEditTag)
// 		{
//         //	::DestroyWindow(m_hChild);
//             POINT ptOffset;
// //        = GetDeviceScrollPosition();//От полос прокрутки
//             // CTagEdit* edt = new CTagEdit( mDoc );
//             // if ( edt ) m_hChild = edt->MyCreate( m_hWnd, pTag, m_fScale, ptOffset );
// 		}
//     }
//   return pTag;
//   }

// void CViewMnemo::OnLButtonDblClk(UINT nFlags, CPoint point)
//   {
//   ::DestroyWindow(m_hChild);
//   if ( mDoc.mBody )
//     {
// #if 1 // Гродно-Циклогексанон
// 		//MIHAIL_HWL здесь нет вызова деталей по двойному клику
// /*    CPoint pt = Scroll(point, true);
//     CTag* pTag = mDoc.PtInTag( pt, false );
//     if ( pTag )
//       {
//       const char* name = mDoc.pipe.GetMnemoName( pTag->mVal.nNumber );
//       CharMP szName;
//       lstrcpy( szName, name );
//       AntiAlias(szName);
// 	  GetAppWnd()->ButtonOk( barDETAIL, szName );
//       }*/
// #else
//     CTag* pTag = mDoc.mBody->LinkScript( mDoc, "GrpUpperPanel" );
//     if ( pTag )
//       {
//       CPoint pt = Scroll(point, true);
//       CStyle& style = pTag->Style(mDoc);
//       if ( ::PtIncross::RECT( &style.m_rcTruecross::RECT, pt ) )
//         {
// #if 0 // Гродно-Аммиак вариант
//         int nTag = mDoc.pipe.GetMnemoTegs( pTag->mVal.nNumber );
//         GetAppWnd()->m_frmNative.Detail( nTag );
// #else // Новгород-Аммиак вариант
//         const char* pszText = mDoc.pipe.GetMnemoName( pTag->mVal.nNumber );
//         GetAppWnd()->ButtonOk( barDETAIL, pszText );
//         return;
// #endif
//         }
//       }
// #endif
//     }
//   if ( Version==LG35_8_KF )
//     {
//     m_pPressed = LButtonDown(point,true);
// 	if (m_pPressed)
// 	{
// 		m_pPressed->Action(mDoc,m_hWnd,true);
// 		Draw();
// 		if ( m_pPressed == NULL )
// 		  ReleaseCapture();
// 	}
//     }
//   else
//   if ( Version==DOTF_PM )
//   { //детальное окно по двойному нажатию
//    m_pPressed = LButtonDown(point,false);
//    if ( m_pPressed == NULL )
//    {
//        m_pBoundTag = FindBoundTag(point);
//    }
//    if(m_pPressed || m_pBoundTag)
//      OnDetail();
//   }
//   CScrollView::OnLButtonDblClk(nFlags, point);
//   }

// void CViewMnemo::OnLButtonDown(UINT nFlags, CPoint point)
//   {
//   ::DestroyWindow(m_hChild);
//   HWND hWnd = GetAppWnd()->GetActiveMnemo();
//   if ( hWnd != m_hWnd )
//     {
//     GetAppWnd()->SetActiveMnemo(m_hWnd);
//     ::Invalidatecross::RECT( m_hWnd, NULL, false );
//     if ( ::IsWindow(hWnd) ) ::Invalidatecross::RECT( hWnd, NULL, false );
//     }
//   if ( m_pPressed == NULL )
//   m_pPressed = LButtonDown(point,true);
//   if ( m_pPressed != NULL )
//   if ( m_pPressed->m_pMouseD != NULL )
//     {
//     m_pPressed->Action(mDoc,m_hWnd,true);
//     Draw();
//     }
//   if ( m_pPressed == NULL )
//     TabIndexCur(point,true);
//   CScrollView::OnLButtonDown( nFlags, point);
//   }

//void CViewMnemo::OnContextMenu(CWnd* pWnd, CPoint point)
  // void CViewMnemo::OnContextMenu( CPoint point)
  // {
  // ::DestroyWindow(m_hChild);
  // ::GetCursorPos( &point );
  // ::ScreenToClient( m_hWnd, &point );
  // m_pBoundTag = NULL;
  // m_pPressed = LButtonDown(point,false);
  // if ( m_pPressed == NULL || m_pPressed->mVal.nNumber==-1)
  // {
  //   if ( Version==LG35_8_KF )
  //     return;
  //   m_pBoundTag = FindBoundTag(point);
  //   if(m_pBoundTag == NULL)
  //     return;
  // }
  // ContextMenu( m_hWnd, false );
  // }

// bool CViewMnemo::RunPageScriptClick(POINT& point)
// {
//     bool bOpen = false;
//     if( mDoc.m_pScriptClick)
//     {
//       std::vector<CTag*> tags;
//       int num = FindTags(point, tags);
//       memset(mDoc.WINDOWS_EXTERNAL_CURRENTPAGE, 0, sizeof mDoc.WINDOWS_EXTERNAL_CURRENTPAGE);
//       mDoc.m_eventpoint = Scroll(point, true);
//       for(std::vector<CTag*>::iterator it = tags.begin(); it!=tags.end(); it++)
//       {
//           CStyle& style = (*it)->Style( mDoc );
//           const char* id = style.NameID( mDoc );
//           mDoc.m_psrcelement = *it;
//       //    mDoc.m_pScriptClick->RunScript();
//           if(mDoc.WINDOWS_EXTERNAL_CURRENTPAGE[0])
//           {
// //              mDoc.m_pScriptClick->NaviGo(mDoc, *this, mDoc.WINDOWS_EXTERNAL_CURRENTPAGE);
//               bOpen = true;
//               break;
//           }
//       }
//     }
//     return bOpen;
// }

// void CViewMnemo::OnLButtonUp(UINT nFlags, CPoint point)
//   {
//   if ( m_pPressed != NULL )
//     {
//     ReleaseCapture();
// 	if (m_lastPressed != NULL)
// 	{
// 		m_lastPressed->Release();
// 		m_lastPressed = NULL;
// 	}
//     bool bOpen = RunPageScriptClick(point);
//     if(!bOpen)
//     {
//     if ( m_pPressed->m_pMouseD != NULL )
//       {
//       m_pPressed->Action(mDoc,m_hWnd,false);
//       }
//     else
//       {
//       CTag* pPressed = LButtonDown(point);
//       if ( pPressed != m_pPressed ) pPressed = NULL;
//       if ( pPressed )
//         {
//         if(Version == LG35_8_KF || Version == L35_11600_KF)
//          {
//          if (strcmp("ACK ", pPressed->m_szScriptValue)==0)
//             mDoc.AckAll();
//          else
//           {
// struct _BTNS
// {
//     char *nm;
//     char *id;
//     char *key;
// };
//           _BTNS *pBtns=NULL;
//           if(mDoc.m_strFile.CollateNoCase("sncdtlCPCa_fp")==0)
//            {
//             static _BTNS Btns[] = {{"LSel","btnSel","LSEL"}, {"Ctrl","btnSeq","SEQ"}, {"L1","btnL1","L1"}
// 						 , {"L2","btnL2","L2"}, {"L3","btnL3","L3"},{NULL,NULL}};
//             pBtns = (_BTNS*)&Btns;
//            }
//           else
//           if(mDoc.m_strFile.CollateNoCase("sncdtlSPCMa_fp")==0)
//           {
//             static _BTNS Btns[] = {{"Seq ","btnSel","SELH"}, {"AS ","btnL1","AS"}
// 						, {"Lim1 ","btnL2","LIM1"}, {"Lim2 ","btnL3","LIM2"}, {NULL,NULL}};
//             pBtns = (_BTNS*)&Btns;
//           }
//           char *key = NULL;
//           char *id = NULL;
//           if(pBtns)
//            {
//            if(mDoc.strCtrlBlockName.empty())
//            {
// 				CTag *pTag = mDoc.FindTagInBodyById(mDoc, pBtns[0].id);
// 				if(pTag)pTag->m_bPressed = true;
// 				key = pBtns[0].key;
//            }
//            for(int ii=0; pBtns[ii].nm; ii++)
//             if (strcmp(pBtns[ii].nm, pPressed->m_szScriptValue)==0)
//               {
//                 key = pBtns[ii].key;
//                 id =  pBtns[ii].id;
//                 break;
//               }
//            if(key)
//             {
//             if(mDoc.strCtrlBlockName != key)
//               {
//               mDoc.strCtrlBlockName = key;
//               CTag *pTagSel = mDoc.FindTagById(mDoc, "txtSelectedCtrlBlock");
//               if(pTagSel)
//                 pTagSel->m_constValue = key;
//               m_pPressed = NULL; //чтоб кнопка не отжималась
//               //установка нажатий-отжатий кнопок в группе:
//               for(int ii=0; pBtns[ii].nm; ii++)
//               {
//                 CTag *pTag = mDoc.FindTagInBodyById(mDoc, pBtns[ii].id);
//                 if(pTag)
//                   if(strcmp(key, pBtns[ii].key)!=0)
//                     pTag->m_bPressed = false;
//                   else{
//                       pTag->m_bPressed = true;
//                       mDoc.CtrlBlockSet140(pBtns[ii].id);
//                       }
//               }
//              }
//             //else mDoc.strCtrlBlockName = "";
//             }
//            else  pPressed->Action(mDoc,m_hWnd,false,&point);
//            }
//           else  pPressed->Action(mDoc,m_hWnd,false,&point);
//           }
//          }
//         else pPressed->Action(mDoc,m_hWnd,false,&point);
//         }
//       }
//     }

//     if ( m_pPressed )
//     {
//     m_pPressed->m_bPressed = false;
//     if(Version >= AM2_NAK)
//     {//скрипт привязан не к button
//     if(typeid(*m_pPressed) != typeid(CTagINPUT))
//       {
//       CTag *pTag = m_pPressed->FindType(mDoc, typeid(*m_pPressed));
//       if(pTag)
//          pTag->m_bPressed = false;
//       }
//     }
//     }
// 	m_lastPressed = m_pPressed;
//     m_pPressed = NULL;
//     Draw();
//     }
//   else
//   {
// 	if (m_lastPressed != NULL)
// 	{
// 		m_lastPressed->Release();
// 		m_lastPressed = NULL;
// 	}

//    bool bOpen = RunPageScriptClick(point);

//     // установка фокуса на поле Command:
//     CWnd* pParent = GetParent();
//     if(typeid(*pParent) != typeid(CAmmFrmSTB))
//        GetAppWnd()->SetFocusInpunZone();
//     else ((CAmmFrmSTB*)pParent)->SetFocusInpunZone();
//   }
//   CScrollView::OnLButtonUp(nFlags, point);
//   }

// void CViewMnemo::OnMouseMove(UINT nFlags, CPoint point)
//   {
//   if ( m_pPressed != NULL )
//     {
// 	theApp.DND(m_pPressed);
//     CTag* pPressed = LButtonDown(point);
//     if ( pPressed == m_pPressed )
//       {
//       if ( !m_pPressed->m_bPressed )
//         {
//         m_pPressed->m_bPressed = true;
//         Draw();
//         }
//       }
//     else
//       {
//       if ( m_pPressed->m_bPressed )
//         {
//         m_pPressed->m_bPressed = false;
//         Draw();
//         }
//       }
//     }

//   if(m_ToolTip)
//   {
//   char title[256]="";
//   CPoint pt = Scroll(point, true);
//   mDoc.FindTitle(pt, title);
//   m_ToolTip.UpdateTipText(title, this);
//   }
 
//   CScrollView::OnMouseMove(nFlags, point);
//   }

// void CViewMnemo::OnMButtonDown(UINT nFlags, CPoint point)
//   {
//   ::DestroyWindow(m_hChild);
//   CPoint pt = Scroll(point,true);
//   if ( ::IsMnemoStyle(MNEMO_EDIT) )
//     {
//     CDlgDebug dlg(mDoc);
//     mDoc.Dlg(dlg,pt,1);
//     if ( dlg.m_nCount > 0 )
//       dlg.DoModal();
//     }
//   CScrollView::OnMButtonDown(nFlags, point);
//   }

// inline POINT Lparam( LPARAM l )
//   {
//   POINT  point;
//   point.x = ((short int)((DWORD_PTR)(l) & 0xffff));
//   point.y = ((short int)((DWORD_PTR)(l) >> 16));
//   return point;
//   }
//any mouse keys, X Y
// LRESULT CViewMnemo::OnXButtonDown(WPARAM wParam,LPARAM lParam)
//   {
//   CPoint point = Lparam(lParam);
//   OnMButtonDown( UINT(wParam), point );
//   return 0;
//   }
//
// FLOAT CViewMnemo::GetScale( int nChoise )
//   {
//   if ( nChoise == -1 )//Для Faceplate
//     {
//     cross::RECT cross::RECT;
//     // GetClientcross::RECT( &cross::RECT );
//     SIZE page = mDoc.Size();
//     float fScale = float(cross::RECT.right)/(float)(page.cx);
//     return fScale;
//     }
//   //
//   static float fScaleMult[7]={ 0.25, 0.5, 1000.0, 1.0, 1.5, 2.0, 5.0 };
//   // 1000.0 - FitToScreen
//   float fScale = fScaleMult[nChoise];
//   if ( fScale > 100.0 )
//     {
//    //  CSize size,sizeSb;
//     // GetTrueClientSize( size, sizeSb );
//    //  SIZE page = mDoc.Size();
//    //  float tempX=float(size.cx)/(float)(page.cx);
//    //  float tempY=float(size.cy)/(float)(page.cy);
//    //  fScale = min(tempX,tempY);
//     }
//   return fScale;
//   }
// //
// void CViewMnemo::Zoom(int nChoise)
//   {
//   m_nScale = nChoise;
//   SetSizes();
//   // if (IsWindowVisible())
//   //   Invalidate(false);
//   }
// //
// void CViewMnemo::SetSizes()
//   {
//   m_fScale = GetScale( m_nScale );
//   if ( m_nScale != -1 )
//     {
//     SIZE sz = mDoc.Size();
//     sz.cx = int( m_fScale*sz.cx );
//     sz.cy = int( m_fScale*sz.cy );
//  //   SetScrollSizes(MM_TEXT,sz);
//     }
//   // POINT ptOffset = GetDeviceScrollPosition();
//   // mDoc.WindowPos( m_hWnd, mDoc, m_fScale, ptOffset );
//   }
//
// void CViewMnemo::ClearPage()
//   {
//   // if ( ::IsWindow(m_hWnd) )
//   //   mDoc.ClearPage();
//   }
//
// void CViewMnemo::OnKillFocus(CWnd* pNewWnd)
//   {
//   CScrollView::OnKillFocus(pNewWnd);
//   if ( GetAppWnd()->GetActiveMnemo() == m_hWnd )
//     GetAppWnd()->SetActiveMnemo(NULL);
//   ::Invalidatecross::RECT( m_hWnd, NULL, false );
//   }
// //
// void CViewMnemo::OnSetFocus(CWnd* pOldWnd)
//   {
//   CScrollView::OnSetFocus(pOldWnd);
//   GetAppWnd()->SetActiveMnemo(m_hWnd);
//   ::Invalidatecross::RECT( m_hWnd, NULL, false );
//   }
//
// bool CViewMnemo::MnemoAck()
//   {
//   SSendToModel send;
//   lstrcpy( send.szName, mDoc.m_strFile );
//   ::CharUpper( send.szName );
//   lstrcpy( send.szValue, "Квитирование" );
//   send.eData = (EDataTypes)id_OneMnemoHoney;
//   //
//   //MIHAIL_HWL клиент-сервер
//   //было   BYTE Old = 0; BYTE New = ::AreaNumb()+1;
//   //похоже квитирование по Area
//   BYTE Old = 0; BYTE New = 1;
//   send.Set( enumValueChr, &Old, &New );
//   //mDoc.pipe.SendData( send, true );
//   //
//   return true;
//   }
// //
// bool CViewMnemo::Keyboard( WORD nCode, UINT nChar )
//   {
//   // if ( nChar == hw_ACK )
//   //   if ( !mDoc.m_bR300 )
//   //     return MnemoAck();
//   //
//   int nReturn = mDoc.HWBTN(mDoc,nCode,nChar);
//   return nReturn != 0;
//   }
//
// int CViewMnemo::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
//   {
//   // Именно так и надо.
//   return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
//   }

// void CViewMnemo::OnAckOne()
//   {
//   if ( m_pPressed == NULL )
//   {
//     if( m_pBoundTag == NULL)
//     return;
//     m_pPressed = m_pBoundTag;
//     m_pBoundTag = NULL;
//   }
//   AckTag( mDoc.pipe, m_pPressed->mVal.nNumber );
//   m_pPressed = NULL;
//   }
//
// void CViewMnemo::OnDetail()
//   {
//   if ( m_pPressed == NULL ||  m_pPressed->mVal.nNumber == -1)
//   {
//     if( m_pBoundTag == NULL)
//     return;
//     m_pPressed = m_pBoundTag;
//     m_pBoundTag = NULL;
//   }
//   const char* tag = mDoc.pipe.GetMnemoName( m_pPressed->mVal.nNumber );
//   if ( tag )
//     GetAppWnd()->Detail(tag);
//   m_pPressed = NULL;
//   }
//
// static int _find_mval(CTag * tag, CHTML& html)
// {
// 	if (tag->mVal.nNumber < 0)
// 	{
// 		INT_PTR nCount = tag->GetSize();
// 		CTag** obj = tag->GetData();
// 		for ( INT_PTR n = 0; n < nCount; n++ )
// 		{
// 			int mval = _find_mval(obj[n], html);
// 			if (mval >= 0) return mval;
// 		}
// 		return -1;
// 	}
// 	else
// 	{
// 		return tag->mVal.nNumber;
// 	}
// }

// int find_mval(CTag * tag, CHTML& html)
// {
// 	if (tag->mVal.nNumber < 0)
// 	{
// 		return _find_mval(tag->m_pPar->m_pPar, html);
// 	}
// 	else
// 	{
// 		return tag->mVal.nNumber;
// 	}
// }

// void CViewMnemo::OnTrend()
//   {
//   if ( m_pPressed == NULL )
//   {
//     if( m_pBoundTag == NULL)
//     return;
//     m_pPressed = m_pBoundTag;
//     m_pBoundTag = NULL;
//   }
//   const char* tag = mDoc.pipe.GetMnemoName(find_mval(m_pPressed, mDoc));
//   if ( tag )
//     GetAppWnd()->Trend(tag);
//   m_pPressed = NULL;
//   }
//
// void CViewMnemo::OnMiniTrend()
//   {
//   if ( m_pPressed == NULL )
//   {
//     if( m_pBoundTag == NULL)
//     return;
//     m_pPressed = m_pBoundTag;
//     m_pBoundTag = NULL;
//   }
//   const char* tag = mDoc.pipe.GetMnemoName(find_mval(m_pPressed, mDoc));
//   if ( tag && strcmp(tag, "Фёдор"))
//    {
// static char name[] = "Mini_Trend_Popup";
//     CAplicFrm* frm = GetAppWnd();
//     if (theApp.IsAllInOne() && !theApp.DisableKirishiView())
//        {
//        CAmmFrm* ammFrm = GetAppWnd()->GiveRegularLeftAmm(m_hWnd);
//        cross::RECT rc;
//        ammFrm->GetMnemo()->GetClientcross::RECT(&rc);
//        ammFrm->GoToMiniTrend(name, tag);
//        ammFrm->PaintExtButton();
//        }
//     else
//     {
//     CPopupMnemo* wnd = new CPopupMnemo();
//     if ( wnd != NULL )
//       {
//       SIZE sz = {662,530};

//       cross::CString header(tag);
//       int pp = header.Find('.');
//       if(pp != -1)
//         header = header.Left(pp);
//       if( !frm->FndMnemoAndName( name, header.GetBuffer()) )
//         {
//         if ( !wnd->MyCreate(this, name, &sz, WS_THICKFRAME | WS_EX_TOPMOST, (LPSTR)tag) )
//           delete wnd;
//         else
//           {
//           wnd->SetWindowText(header.GetBuffer());
//           //POINT point;
//           //if(::GetCursorPos(&point))
//             {
//              cross::RECT wndcross::RECT, rc;
//              ::GetWindowcross::RECT(wnd->m_hWnd, &wndcross::RECT);
//              GetWindowcross::RECT(&rc);
//              int ww=wndcross::RECT.right - wndcross::RECT.left;
//              int hh=wndcross::RECT.bottom - wndcross::RECT.top;
//              /*if((point.x > rc.left) && ((point.x + ww) < rc.right))
//                 wndcross::RECT.left = point.x;
//              else*/ wndcross::RECT.left = rc.left;
//              /*point.y -= 120; //высота меню
//              if((point.y > rc.top) && ((point.y + hh) < rc.bottom))
//                  wndcross::RECT.top = point.y;
//              else*/  wndcross::RECT.top = rc.top;
//              wndcross::RECT.right = wndcross::RECT.left + ww;
//              wndcross::RECT.bottom = wndcross::RECT.top + hh;
//              Ccross::RECT Screencross::RECT = ::cross::RECTWnd(0);
//              wnd->MoveWindow(&wndcross::RECT);
//             }
//           frm->AddMnemo( wnd );
//           }
//         }
//       }
//     }
//    }
//   m_pPressed = NULL;
//   }
//
// void CViewMnemo::OnGroup()
//   {
//   if ( m_pPressed == NULL )
//   {
//     if( m_pBoundTag == NULL)
//     return;
//     m_pPressed = m_pBoundTag;
//     m_pBoundTag = NULL;
//   }
//   const char* tag = mDoc.pipe.GetMnemoName( m_pPressed->mVal.nNumber );
//   if ( tag )
//     GetAppWnd()->Group(tag);
//   m_pPressed = NULL;
//   }
//
// void CViewMnemo::OnAssocs()
//   {
//   if ( m_pPressed == NULL )
//   {
//     if( m_pBoundTag == NULL)
//     return;
//     m_pPressed = m_pBoundTag;
//     m_pBoundTag = NULL;
//   }
//   const char* tag = mDoc.pipe.GetMnemoName( m_pPressed->mVal.nNumber );
//   if ( tag )
//     GetAppWnd()->Assocs(tag);
//   m_pPressed = NULL;
//   }

// void CViewMnemo::OnMan  ()
//   {
//   Keyboard(HW_KEYDOWN,hw_MAN);
//   }

// void CViewMnemo::OnAuto ()
//   {
//   Keyboard(HW_KEYDOWN,hw_AUTO);
//   }

// void CViewMnemo::OnNorm ()
//   {
//   Keyboard(HW_KEYDOWN,hw_NORM);
//   }

// void CViewMnemo::OnRaise()
//   {
//   }

// void CViewMnemo::OnLower()
//   {
//   }

// void CViewMnemo::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT item)
//   {
// 	if ( FloatMenu( item, m_hWnd ) ) return;
//   CScrollView::OnMeasureItem(nIDCtl, item);
//   }

// void CViewMnemo::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT item)
//   {
// 	if ( FloatMenu( item ) ) return;
//   CScrollView::OnDrawItem(nIDCtl, item);
//   }

// BOOL CViewMnemo::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pInfo)
//   {
//   HWND hWnd = ::GetWindow( m_hWnd, GW_CHILD );
//   while ( ::IsWindow( hWnd ) )
//     {
//     CWnd* pWnd = CWnd::FromHandle(hWnd);
//     if ( pWnd && pWnd->OnCmdMsg( nID, nCode, pExtra, pInfo ) )
//       return TRUE;
//     hWnd = ::GetWindow( hWnd, GW_HWNDNEXT );
//     }
//   return CScrollView::OnCmdMsg(nID, nCode, pExtra, pInfo);
//   }

// BOOL CViewMnemo::PreTranslateMessage(MSG* pMsg)
//   {
//   // if(m_ToolTip)
//   //   m_ToolTip.RelayEvent(pMsg);
//   // return __super::PreTranslateMessage(pMsg);
//   return true;
//   }
