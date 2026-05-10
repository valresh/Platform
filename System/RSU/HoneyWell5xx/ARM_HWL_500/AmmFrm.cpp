// #include "stdafx.h"
// #include <afxpriv.h>
// #include "ARM.h"
// #include "AplicFrm.h"
// #include "AmmFrm.h"
// #include "MnemoFrm.h"
// #include "blankview.h"

// static const char * GetClientName(int nMonitor, int nPosition)
// {
// 	if (theApp.IsAllInOne())
// 	{
// 		switch(nPosition)
// 		{
// 		case 1: return "Первый";
// 		case 2: return "Второй";
// 		case 3: return "Третий";
// 		case 4: return "Четвертый";
// 		case 5: return "Верхний";
// 		case 6: return "Левый1";
// 		case 7: return "Левый2";
// 		case 8: return "Левый3";
// 		case 9: return "Левый4";
// 		case 10: return "Сенсорная панель";
// 		}
// 	}
// 	else
// 	{
// 		switch(nMonitor)
// 		{
// 		case 0: return "Первый";
// 		case 1: return "Второй";
// 		case 2: return "Третий";
// 		case 3: return "Четвертый";
// 		case 4: return "Навигация";
// 		}
// 	}
//     return NULL;
// }


// static SHeader wndTopStdData[] =
// 	{
//   {""               ,   20, true, false, CMsgAL::Image       , 0},
//   {"Date && Time "  ,  100, true, false, CMsgAL::DateTime    , 1},
//   {"Source "        ,  150, true, false, CMsgAL::Source      , 3},
//   {"Condition "     ,   85, true, false, CMsgAL::Condition   , 4},
//   {"Priority "      ,   75, true, false, CMsgAL::Priority    , 5},
//   {"Description "   ,  230, true, false, CMsgAL::Description , 6},
//   {"Trip Value "    ,   90, true, false, CMsgAL::TripValue   , 7},
//   {"Live Value "    ,   90, true, false, CMsgAL::LiveValue   , 8},
//   {"Units "         ,10000, true, false, CMsgAL::Units       , 9},
//   {"END"            ,    0, true, false},
//   {NULL}
// 	};

// //
// // CAmmFrm
// IMPLEMENT_DYNAMIC(CAmmFrm, CButtonFrm)
// CAmmFrm::CAmmFrm( int nMonitor, HWND& hMnemo, HWND& hSxema, int nPosition )
// : CButtonFrm( nMonitor, false )
// , client( GetClientName(nMonitor, nPosition), (Version>=DOTF_PM)? 900:0)
// , m_pAmmFrame(NULL)
// , m_wndMnemo(hMnemo,client)
// , m_wndSxema(hSxema,nMonitor)
// , m_bNoPCYWork(false)
// , m_pGotoPole( NULL )
// , m_nPosition(nPosition)
// , m_pViewAlarm(NULL)
//   {
//   m_bGalkaOn = ( nMonitor == 0 );
//   if(theApp.m_bFourMonitors)
//     m_bGalkaOn = !m_bGalkaOn;
//   }

// CAmmFrm::~CAmmFrm()
//   {
//       if(m_pViewAlarm)
//         delete m_pViewAlarm;
//        m_pViewAlarm = NULL;
//   }

// BEGIN_MESSAGE_MAP(CAmmFrm, CButtonFrm)
//   ON_WM_CREATE()
//   ON_WM_SETFOCUS()
//   ON_WM_NCLBUTTONDOWN()
//   ON_WM_GETDLGCODE()
//   ON_WM_CLOSE()
//   ON_WM_NCACTIVATE()
// END_MESSAGE_MAP()

// BOOL CAmmFrm::PreCreateWindow(CREATESTRUCT& cs)
//   {
//   if( !CFrameWnd::PreCreateWindow(cs) )
//     return FALSE;
// 	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
//   return TRUE;
//   }

// void CAmmFrm::PostNcDestroy()
//   {
//   /*CFrameWnd::PostNcDestroy();*/
//   }

// BOOL CAmmFrm::MyCreate( CAplicFrm* pParentWnd, LPcross::RECT rcApp )
//   {
//   m_pParent = pParentWnd;
//   cross::RECT cross::RECT = cross::RECTWnd(m_nMonitor,&m_pGotoPole,m_nPosition);
//   // Окно отключено в TuningDlg
//   if ( ::Iscross::RECTEmpty( &cross::RECT ) )
//     return TRUE;
//   // Основное окно смотрим отдельно
//   if ( rcApp ) cross::RECT = *rcApp;
//   //
//   if ( m_nMonitor == 0 )
//   if(!theApp.bUseWDL())
//   switch ( dwTypeFrame )
//     {
//     case 0: // Как в В.Новгороде на Ammiak-2
//       cross::RECT.right = int( cross::RECT.left*(1.-mWindSize)+cross::RECT.right*mWindSize );
//     break;
//     case 1: // Как в Гродно на Аммиаке
//       if (Version == AT_PRP || Version == LG35_8_KF ||  Version == L35_11600_KF){}
// 	  else
// 	  {
// 		cross::RECT.left +=10;
// 		cross::RECT.right-= 5;
// 	  }
//     break;
//     case 2: // Как в Гродно на Циклогексаноне
//     break;
// //    Ccross::RECT cross::RECT = Workcross::RECT( m_wndDubl->m_nMonitor );
//     }
//   //
//   if ( !Create(NULL,NULL,WS_POPUP|WS_SYSMENU|WS_CAPTION| WS_CLIPCHILDREN| WS_CLIPSIBLINGS, cross::RECT, pParentWnd ) )
//     return FALSE;
//   //
//    if(theApp.bUseWDL())
//    {
//        if(m_nPosition <= theApp.m_wdl.staticWindows.size())
//          theApp.m_wdl.staticWindows[m_nPosition-1]->pAmmFrm = this;
//    }
//   //
  
//   if ( ( m_nMonitor == 0 && !theApp.IsAllInOne() ) || ( theApp.IsAllInOne() && m_nPosition == 1) )
//   {
//     LONG lStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE);
//     lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
//     SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
//   }

//   ZoomMnemo(2);//Fit to Screen
//   ShowWindow(SW_SHOW);
//   UpdateWindow();

//   if( theApp.IsAllInOne() && (m_nPosition==5) )
//   if( !theApp.bUseWDL() )
//     {
//     cross::RECT cross::RECT_wndalarmClient;
//     m_wndMnemo.mDoc.mWnd->GetClientcross::RECT(&cross::RECT_wndalarmClient);
//     int Width = 41*(cross::RECT_wndalarmClient.right-cross::RECT_wndalarmClient.left)/100;
//     int Height = cross::RECT_wndalarmClient.bottom-cross::RECT_wndalarmClient.top;
//     m_pViewAlarm = new CViewAlarm();
//     m_pViewAlarm->SetHeader( wndTopStdData, RGB(255,255,255), RGB(0,0,0) );
//     m_pViewAlarm->MyCreate( m_wndMnemo.mDoc.mWnd, 0 );

//     ::SetWindowPos( m_pViewAlarm->m_hWnd, NULL, 2, 0, Width, Height, SWP_NOZORDER );
//     ::ShowWindow(m_pViewAlarm->m_hWnd, SW_SHOW);
//     ::UpdateWindow(m_pViewAlarm->m_hWnd);
//     //
//     LOGFONT lf = {0};
//     strcpy_s(lf.lfFaceName, "'Helvetica'");
//     lf.lfWeight = 400;
//     lf.lfHeight = (int)(-13*m_wndMnemo.m_fScale-0.5);
//     m_pViewAlarm->SetMyFont( lf );

//     m_pViewAlarm->SetScaleFromSmall(m_wndMnemo.m_fScale);
//     }

//   return TRUE;
//   }

// int CAmmFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
//   {
//   if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
//     return -1;
//   theApp.SetIcon( this );

//   CMenu *pSysMenu = GetSystemMenu(0);
//   pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
//   pSysMenu->RemoveMenu(SC_MOVE, MF_BYCOMMAND);
//   //
//   cross::RECT cross::RECT = {0,0,0,0};
//   if ( !m_wndMnemo.Create( NULL, NULL, 0, cross::RECT, this, AFX_IDW_PANE_FIRST ) )
//     return -1;
//   //
// 	if (!m_wndSxema.MainCreate( this ) )
// 		return -1;
//   //
// 	CCreateContext cc;
// 	cc.m_pCurrentFrame = this;
// 	cc.m_pNewViewClass = RUNTIME_CLASS(CBlankView);
// 	m_pBlankView = static_cast<CBlankView*>(CreateView(&cc));
// 	if (NULL == m_pBlankView)
//     {
//     TRACE0("Failed to create CBlankView \n");
//     return -1;
//     }
// 	ASSERT_VALID(m_pBlankView);
// 	m_pBlankView->SetDlgCtrlID(0);
//   //
//   OnInitMenu(NULL);
//   //
//   return 0;
//   }
// //
// void CAmmFrm::MainTimer(DWORD dwTimer)
//   {
//   if ( !::IsWindow(m_hWnd) ) return;
//   if ( !theApp.m_bPole)
// 		client.Refresh();
//   //
//   bool bNoPCYWork = IsNoPCY();
//   //
//   if ( !bNoPCYWork )
//     m_wndMnemo.MainTimer(dwTimer);
//   m_wndSxema.MainTimer();
//   //
//   if ( m_bNoPCYWork != bNoPCYWork || dwMainTimer == 1 )
// 	  ChangePCY(bNoPCYWork);
//   //
//   if(m_pViewAlarm)
//   {
//      m_pViewAlarm->MainTimer();
//   }
//   //
//   }
// //
// void CAmmFrm::ChangePCY(bool bSet)
//   {
// 	m_bNoPCYWork = bSet;
//   if ( ::IsWindow(m_wndSxema) )
// 	if ( !m_wndSxema.IsWindowVisible() )
//     {
//     //
//     m_wndMnemo.SetDlgCtrlID( m_bNoPCYWork ? 0 : AFX_IDW_PANE_FIRST );
//     m_pBlankView->SetDlgCtrlID( m_bNoPCYWork ? AFX_IDW_PANE_FIRST : 0 );
//     m_wndMnemo.ShowWindow( m_bNoPCYWork ? SW_HIDE : SW_SHOW );
//     m_pBlankView->ShowWindow( m_bNoPCYWork ? SW_SHOW : SW_HIDE );
//     //
//   	//Sergej SetActiveView(GetTop());
// 	  RecalcLayout();
//     //
//     }
//   }
// //
// inline bool IsYou( CWnd* pWnd, POINT& point )
//   {
//   if ( pWnd != NULL )
//   if ( ::IsWindow(*pWnd) )
//   if ( pWnd->IsWindowVisible() )
//     {
//     cross::RECT cross::RECT;
//     pWnd->GetWindowcross::RECT( &cross::RECT );
//     return ::PtIncross::RECT( &cross::RECT, point ) != FALSE;
//     }
//   return false;
//   }
// //
// CWnd* CAmmFrm::GetActive()
//   {
//   POINT point;
//   ::GetCursorPos( &point );
//   if ( IsYou( GetTop(), point ) )
//     return GetTop();
//   if ( IsYou( &m_wndSxema, point ) )
//     return &m_wndSxema;
//   return NULL;
//   }

// void CAmmFrm::OnSetFocus(CWnd* pOldWnd)
//   {
//   CButtonFrm::OnSetFocus(pOldWnd);
//   CWnd* pWnd = GetActive();
//   if ( pWnd != NULL )
// 	  pWnd->SetFocus();
// //GetAppWnd()->PoleOper(!IsShowSxeme());
//   //
//   HWND hWnd = GetAppWnd()->GetActiveMnemo();
//   if ( hWnd != m_wndMnemo.m_hWnd )
//     {
//     GetAppWnd()->SetActiveMnemo(m_wndMnemo);
//     ::Invalidatecross::RECT( m_wndMnemo.m_hWnd, NULL, false );
//     if ( ::IsWindow(hWnd) ) ::Invalidatecross::RECT( hWnd, NULL, false );
//     }
//   }

// BOOL CAmmFrm::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//   {
//   //
//   CWnd* pWnd = GetActive();
//   if ( pWnd != NULL )
//     if ( pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//       return TRUE;
//   //
//   return CButtonFrm::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//   }

// bool CAmmFrm::IsShowSxeme()
//   {
//   return ::IsWindow(m_hWnd) && m_wndSxema.IsWindowVisible() == TRUE;
//   }

// void CAmmFrm::GotoPole(int nCorrelate)
//   {
//   theApp.m_bPole=true;
//   if ( !::IsWindow(m_hWnd) )
//     return;
//   if ( m_pGotoPole && !*m_pGotoPole )
//     return;
//   //
//   m_wndSxema.GotoPole( m_wndMnemo.mDoc.m_strFile, nCorrelate );
//   //Sergej SetActiveView(NULL);
//   GetTop()->ShowWindow(SW_HIDE);
//   }

// void CAmmFrm::GotoOper(int nCorrelate)
//   {
// 	theApp.m_bPole=false;
//   if ( !::IsWindow(m_hWnd) )
//     return;
//   m_wndSxema.GotoOper();
//   //Sergej SetActiveView(GetTop());
//   GetTop()->ShowWindow(SW_SHOW);
//   }
// //
// void CAmmFrm::ShowOrHideMini()
//   {
//   if ( IsShowSxeme() )
//   m_wndSxema.ShowOrHideMini();
//   }
// //
// UINT CAmmFrm::OnGetDlgCode()
//   {
//   UINT d = CFrameWnd::OnGetDlgCode();
//   return DLGC_WANTALLKEYS;//CFrameWnd::OnGetDlgCode();
//   }

// void CAmmFrm::RecalcLayout(BOOL bNotify)
//   {
// 	if (m_bInRecalcLayout)
// 		return;

// 	m_bInRecalcLayout = TRUE;
// 	// clear idle flags for recalc layout if called elsewhere
// 	if (m_nIdleFlags & idleNotify)
// 		bNotify = TRUE;
// 	m_nIdleFlags &= ~(idleLayout|idleNotify);

// 	// reposition all the child windows (regardless of ID)
// 	if (GetStyle() & FWS_SNAPTOBARS)
// 	{
// 		Ccross::RECT cross::RECT(0, 0, 32767, 32767);
// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,
// 			&cross::RECT, &cross::RECT, FALSE);
// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra,
// 			&m_cross::RECTBorder, &cross::RECT, TRUE);
// 		CalcWindowcross::RECT(&cross::RECT);
// 		SetWindowPos(NULL, 0, 0, cross::RECT.Width(), cross::RECT.Height(),
// 			SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
// 	}
// 	else
//     {
//     // Пересчитываем размеры дочерних окон
// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST  , reposExtra, &m_cross::RECTBorder);
// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST+1, reposExtra, &m_cross::RECTBorder);
//     }
// 	m_bInRecalcLayout = FALSE;
// //  CFrameWnd::RecalcLayout(bNotify);
//   }
// //
// void CAmmFrm::OnClose()
//   {
//   }
// //
// void CAmmFrm::OnNcLButtonDown(UINT nHitTest, CPoint point)
//   {
// 	CFrameWnd::OnNcLButtonDown(nHitTest, point);
//   if (( nHitTest == HTCAPTION )||( nHitTest == HTCLOSE ))
//     {
//     Ccross::RECT cross::RECT = GalkaButton(true);
//     if ( cross::RECT.PtIncross::RECT( point ) )
//       {
//         if (theApp.IsAllInOne() && !theApp.DisableKirishiView() || theApp.m_bFourMonitors)
//         {
//          m_bZamokOn=false;
//          if(m_bGalkaOn)
//             GetAppWnd()->SetOnliOneAmm(this); //m_bGalkaOn = false;
//          else m_bGalkaOn = true;
//         }
//       else
//       {
//       m_bGalkaOn=true;
//       m_bZamokOn=false;
//       if ( m_pAmmFrame != NULL )
//         {
//         m_pAmmFrame->m_bGalkaOn=false;
//         m_pAmmFrame->m_bZamokOn=false;
//         m_pAmmFrame->PaintExtButton();
//         }
//       }
//       PaintExtButton();
//       return ;
//       }
//     //
//     cross::RECT = ZamokButton(true);
//     if ( cross::RECT.PtIncross::RECT( point ) )
//       {
//       if ( !m_bZamokOn && 0 )// В Гродно этого нет
//         {
//         m_bGalkaOn=false;
//         m_bZamokOn=true;
//         if ( m_pAmmFrame != NULL )
//           {
//           m_pAmmFrame->m_bGalkaOn=true;
//           m_pAmmFrame->m_bZamokOn=true;
//           m_pAmmFrame->PaintExtButton();
//           }
//         PaintExtButton();
//         }
//       return ;
//       }
//     }
//   }

// BOOL CAmmFrm::OnNcActivate(BOOL bActive)
// {
// 	// TODO: Add your message handler code here and/or call default
// 	BOOL m_b=CFrameWnd::OnNcActivate(bActive);
// 	PaintExtButton();
// 	return m_b;
// }
// //
// void CAmmFrm::ZoomMnemo(int nChoise)
//   {
//   m_wndMnemo.Zoom(nChoise);
//   }
// //
// void CAmmFrm::PageRedo() { m_wndMnemo.PageRedo(); }
// void CAmmFrm::PageUndo() { m_wndMnemo.PageUndo(); }
// void CAmmFrm::DispUndo() { m_wndMnemo.DispUndo(); }
// void CAmmFrm::DispRedo() { m_wndMnemo.DispRedo(); }
// void CAmmFrm::PriorDsp() { m_wndMnemo.PriorDsp(); }
// //
// void CAmmFrm::GoToPage(LPCTSTR szName,LPCTSTR szTagGrp)
//   {
//   this->m_pParent->HideNotPinPopups(this);
//   if ( IsShowSxeme() )
//     m_wndSxema.GoToSchem(szName);
//   else
//     m_wndMnemo.EventPanel(szName,szTagGrp,true);
//   PaintExtButton();
//   }
// //
// void CAmmFrm::GoToMiniTrend(LPCTSTR szName, LPCTSTR szTag)
//   {
//   m_wndMnemo.BuildQueue(szName, szTag);
//   SIZE sz = m_wndMnemo.mDoc.Size();
// /*
//   if ( size != NULL )
//     sz = *size;

//   if ( sz.cx == 1 ) sz.cx = 100;
//   if ( sz.cy == 1 ) sz.cy = 100;
//   ::Setcross::RECT( &cross::RECT, 0, 0, sz.cx, sz.cy );

//   DWORD dwExStyle = (DWORD)GetWindowLong(m_hWnd, GWL_EXSTYLE);
//   DWORD dwStyle   = (DWORD)GetWindowLong(m_hWnd, GWL_STYLE);
//   ::AdjustWindowcross::RECTEx(&cross::RECT, dwStyle, FALSE, dwExStyle);
//   ::SetWindowPos( m_hWnd, HWND_TOP, 0,0, cross::RECT.right-cross::RECT.left+1, cross::RECT.bottom-cross::RECT.top+1, SWP_NOMOVE|SWP_SHOWWINDOW );
//   m_wndMnemo.Zoom(2);
//   //
//   UpdateWindow();
//   if(param)
//     m_bCreated = true;
//   return TRUE;
// */
//   }
// //
// void CAmmFrm::Forsunka(WPARAM wParam,LPARAM lParam)
//   {
//   if ( IsShowSxeme() )
//     m_wndSxema.EventQueueMsg( wParam, lParam );
//   }
// //
