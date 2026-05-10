// #include "stdafx.h"
// #include <afxpriv.h>
// #include "ARM.h"
// #include "AplicFrm.h"
// //#include "SensorFrm.h"
// #include "InfoBox.h"
// #include "ViewTrend4.h"
// #include <algorithm>
// #include "Extensions.h"
// #include "SafetyManagerStruct.h"
// #include "SmShData.h"

// #define AFX_WNDFRAMEORVIEW_REG 0x00008
// BOOL AFXAPI AfxEndDeferRegisterClass(LONG fToRegister);
// // CAplicFrm
// IMPLEMENT_DYNAMIC(CAplicFrm, CButtonFrm)

// class KHoneyPipeClient& Client()
//   {
//   return GetAppWnd()->mPipe;
//   }

// CAplicFrm::CAplicFrm()
// : CButtonFrm( 0, true )
// , mPipe( "Главная", (Version>=DOTF_PM)? 900:0 )
// , mSocket("Главная")
// , m_bNoPCYWork(false)
// , m_wndOne ( 0, GetTouchHWND(this), m_wndPole.m_hWnd, 1 )
// , m_wndThird( theApp.m_bFourMonitors? 2 : 0, GetTouchHWND(this), m_wndPole.m_hWnd, 3 )
// , m_wndFourth( theApp.m_bFourMonitors? 3 : 0, GetTouchHWND(this), m_wndPole.m_hWnd, 4 )
// , m_wndTop( 0, GetTouchHWND(this), m_wndPole.m_hWnd, 5 )
// , m_wndLeft1( 0, GetTouchHWND(this), m_wndPole.m_hWnd, 6 )
// , m_wndLeft2( 0, GetTouchHWND(this), m_wndPole.m_hWnd, 7 )
// , m_wndLeft3( 0, GetTouchHWND(this), m_wndPole.m_hWnd, 8 )
// , m_wndLeft4( 0, GetTouchHWND(this), m_wndPole.m_hWnd, 9 )
// //, m_wndSensor( 1, m_wndOper.m_hWnd, m_wndPole.m_hWnd )
// , m_nTimer(0)
// , m_pActiveButtonFrm(NULL)
// , m_hActiveMnemo(NULL)
// , m_nActiveFaceplate(0)
// , m_hWndHW(NULL)
// , m_hWndMN(NULL)
// , m_bUpdate(false)
// , m_pFaceplate(NULL)
// , m_hLastActiveMnemoForPopups(NULL)
// , m_ptr_pipeForInfoBar(NULL)
// , m_nNumberForInfoBar(-1)
// , m_bMustClearPipeAndNumberForKillFocus(false)
// , m_dTimeOfKillFocus(0)
// , m_nOneDublThirdFourthCounter(0)
// , m_nLeftWindowsCounter(0)
// , m_pMnemoStates(NULL)
//   {
// 	if(Version == BERTH_ONPZ)
// 		m_wndDubl = new CAmmFrmDubl( theApp.m_bFourMonitors? 1 : 0, GetTouchHWND(this), m_wndPole.m_hWnd, 2 );
// 	else
// 		m_wndDubl = new CAmmFrmSTB( theApp.m_bFourMonitors? 1 : 0, GetTouchHWND(this), m_wndPole.m_hWnd, 2 );

//   memset( m_arrWnd , 0, sizeof(m_arrWnd ) );
//   memset( m_arrFace, 0, sizeof(m_arrFace) );
//   memset( m_arrFace4x4, 0, sizeof(m_arrFace4x4) );
//   memset( m_nActiveFaceplates, 0, sizeof(m_nActiveFaceplates) );
//   mPipe.AddLocalSytem();
//   mPipe.End();
//   mSocket.InitAndSendKey();

//   switch(Version)
//   {
//   case AM2_NAK:
//   case DMW_NAK:
//     CStyle::LoadcssSlyles(L"sp_Classic.css");
//     CStyle::LoadcssSlyles(L"sp.css");
//     break;

//   case BERTH_ONPZ:
//     CStyle::LoadcssSlyles(L"sp_Classic.css");
//     CStyle::LoadcssSlyles(L"sp.css");
//     break;
 
//   case ITH_NAK:
//     CStyle::LoadcssSlyles(L"sp_customColors.css");
//     CStyle::LoadcssSlyles(L"sp.css");
//     break;

//   case VSB_KF:
//     CStyle::LoadcssSlyles(L"sp.css");
//     CStyle::LoadcssSlyles(L"sp_Classic.css");
//     break;
//   }

//   if(Version >= AM2_NAK)
// 	  {  CTag::LoadCommomScripts(); m_bNotActive = false; }

//   //if (Version == LG35_8_KF)
//   //{
// 	 // SAlarmsOnMnemo sAlarms=GetAlarmsForMnemo("M02");
// 	 // KKK();
//   //}

//   if (theApp.IsAllInOne() && !theApp.DisableKirishiView())
//     {
//       m_bNotActive = false;
//       m_bGalkaOn = true;
//       m_bZamokOn = false;
//     }

//   }

// CAplicFrm::~CAplicFrm()
// {
//   if(m_wndDubl)
// 	  delete m_wndDubl; m_wndDubl = NULL;
//   if(m_pMnemoStates)
//       delete m_pMnemoStates;
// }

// BEGIN_MESSAGE_MAP(CAplicFrm, CButtonFrm)
//   ON_WM_CREATE()
//   ON_WM_TIMER()
//   ON_WM_DESTROY()
//   ON_WM_SYSCOMMAND()
//   ON_WM_ACTIVATE()
//   ON_WM_NCACTIVATE()
//   ON_WM_NCLBUTTONDOWN()
//   ON_WM_PAINT()
//   ON_WM_ACTIVATEAPP()
//   ON_WM_KEYUP()
//   ON_WM_MEASUREITEM()
//   ON_WM_DRAWITEM()
//   //
//   ON_COMMAND(IDC_BUTTONOK,OnButtonOk)
//   ON_COMMAND(IDC_BUTTONCANCEL,OnButtonCancel)
//   ON_COMMAND(IDOK,OnCommandChange)
//   // Обработка STB-меню
//   ON_COMMAND_RANGE(IDC_STB_MENU, IDC_STB_LAST, OnMenuR300)
//   // Способы перехода на технологическую схему
//   ON_COMMAND(ID_SWITCH_STD , OnSwitchStd)
//   ON_COMMAND(ID_SWITCH_FRM , OnSwitchFrm)
//   ON_COMMAND(ID_SWITCH_DAB , OnSwitchDab)
//   // Работа с виртуальными клавиатурами
//   ON_COMMAND(ID_CALL_HWPULT, OnCallKlawaDop)
//   ON_COMMAND(ID_CALL_HWMNEM, OnCallKlavaBot)
//   ON_COMMAND(ID_HIDE_HWPULT, OnHideKlawaDop)
//   ON_COMMAND(ID_KVIT_ALL, OnKvitAll)
//   // Сообщения от SCN-формата
// 	ON_REGISTERED_MESSAGE(nPressQueue,OnPressQueue)
//   ON_UPDATE_COMMAND_UI(ID_WORKER, OnUpdateWorker)
// END_MESSAGE_MAP()

// BOOL CAplicFrm::PreCreateWindow(CREATESTRUCT& cs)
//   {
//   if( !CFrameWnd::PreCreateWindow(cs) )
//     return FALSE;
//   cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
//   return TRUE;
//   }

// void CAplicFrm::PostNcDestroy()
//   {
//   CFrameWnd::PostNcDestroy();
//   }

// SAlarmsOnMnemo CAplicFrm::GetAlarmsForMnemo(CString strMnemoName,bool bFromSensor)
// {
// 	SAlarmsOnMnemo strAlarms;
// 	ZeroMemory(&strAlarms,sizeof(strAlarms));
//     int nNumber = -1;
//     if(Version == LG35_8_KF)
//     {
// 	nNumber=mPipe.AddMnemo(strMnemoName);
// 	if ((!bFromSensor) &&(strMnemoName=="bl53"))
// 		 mPipe.End();
//     }
//     else
//     {
//         std::map<CString, int>::iterator it = m_MnemosAlarm.find(strMnemoName);
//         if(it == m_MnemosAlarm.end())
//         {
//            m_MnemosAlarm[strMnemoName] = nNumber = mPipe.AddMnemo(strMnemoName);
//            if(L35_11600_KF == Version)
//            {
//               if(strMnemoName.CompareNoCase("TIME")==0) //по последеней с nav
//                  mPipe.End();
//            }
//            else mPipe.End();
//         }
//         else nNumber = it->second;
//     }
// 	//mPipe.Refresh();
// #ifdef _DEBUG
//     if(strMnemoName.CompareNoCase("hlor2")==0)
//        AAA();
// #endif

// 	if (nNumber!=-1)
// 	{
// 		CShOneMnemoHoney* sh = (CShOneMnemoHoney*)mPipe.Sh(nNumber);
// 		if (sh->RetCode==0)
// 		{
// 			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
// 				if (sh->count1[i]!=0)
// 				{
//                     if(Version !=  L35_11600_KF)
// 					  strAlarms.bBlueNotAck=true;
//                     else strAlarms.bRedNotAck=true;
// 					break;
// 				}
// 			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
// 				if (sh->count2[i]!=0)
// 				{
// 					strAlarms.bRedNotAck=true;
// 					break;
// 				}
// 			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
// 				if (sh->count3[i]!=0)
// 				{
//  					strAlarms.bYellowNotAck=true;
// 					break;
// 				}
// 			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
// 				if (sh->state1[i])
// 				{
//                     strAlarms.bBlueBlink=true;
// 					break;
// 				}
// 			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
// 				if (sh->state2[i])
// 				{
// 					strAlarms.bRedBlink=true;
// 					break;
// 				}
// 			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
// 				if (sh->state3[i])
// 				{
//                     strAlarms.bYellowBlink=true;
// 					break;
// 				}
// 		}
// 	}
// 	return strAlarms;
// }

// BOOL CAplicFrm::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
//   {
// 	ASSERT_VALID_IDR(nIDResource);
// 	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

// 	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

// 	CString strFullString;
// 	if (strFullString.LoadString(nIDResource))
// 		AfxExtractSubString(m_strTitle, strFullString, 0);    // first sub-string

// 	//VERIFY(AfxEndDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));

// 	// attempt to create the window
// 	LPCTSTR lpszClass = GetIconWndClass(dwDefaultStyle, nIDResource);

//   CRect rect = ::RectWnd(0, 0, 1);
//   if (!Create(lpszClass, NULL, dwDefaultStyle, rect,
// 	  pParentWnd, NULL, 0L, pContext))
//   return FALSE;   // will self destruct on failure normally

//   // save the default menu handle
// 	ASSERT(m_hWnd != NULL);
// 	m_hMenuDefault = ::GetMenu(m_hWnd);

// 	// load accelerator resource
// 	//LoadAccelTable(MAKEINTRESOURCE(nIDResource));
//   m_hAccelTable = theApp.AccelTable();

// 	if (pContext == NULL)   // send initial update
// 		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

// 	return TRUE;
//   }

// int CAplicFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
//   {
//   if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
//     return -1;
//   theApp.SetIcon( this );
//   ::SetMenu( m_hWnd, theApp.MenuWindow() );
//   //
//   int nKlawa=theApp.GetKlawaNumber();
//   CString strHoneyName;
//   if (theApp.IsAllInOne())
//     {
//     strHoneyName.Format("Операторная%d",nKlawa);
//     m_wndSens.MyCreate( this, strHoneyName, SW_SHOW, m_msgAL.m_ptCount );
//     }
//   else
//     {
// 	  if ((Version==AM2_NAK)&&(theApp.GetOlderSensorKbd()))
// 		strHoneyName = "ОператорнаяСтарший";
// 	  else
//       strHoneyName = "Операторная";
// 	  if ((Version==AM2_NAK)||(Version==DMW_NAK))
// 	  {
// 			m_pMnemoStates = new CMnemoStates("Операторная");
// 	  }
//       m_wndOper.MyCreate( this, strHoneyName, SW_SHOW, m_msgAL.m_ptCount );
//     }
//   m_wndPole.MyCreate( this, "Установка"  , SW_HIDE, NULL );
//   //
//   CMenu *pMenu = GetSystemMenu(0);
//   if ( pMenu )
//     {
//     pMenu->AppendMenu( MF_SEPARATOR );
//     UINT uBE = MF_BYCOMMAND | MF_ENABLED;
//     pMenu->AppendMenu( uBE, ID_APP_ABOUT    , "О Программе..."     );
//     int N = ::GetSystemMetrics(SM_CMONITORS);
//     if ( N > 1 )
//     pMenu->AppendMenu( uBE, ID_TUNING    , "Настройка Экранов..."  );
//     if ( ::IsWindow(m_wndOper) )
//     pMenu->AppendMenu( uBE, ID_KALIBROVKA, "Калибровка Клавиатуры" );
//     if ( ::IsMnemoStyle( MNEMO_EDIT ) )
//     pMenu->AppendMenu( uBE, ID_WORKER    , "Режим Разработчика"    );
//   	pMenu->RemoveMenu(SC_MOVE, MF_BYCOMMAND);
//     }
//   //
//   if ( !m_wndTool.CreateToolBor(this,false) )
//     return -1;

// 	if (!m_wndChoiseBar.Create(this, IDD_CHOISE_BAR,
// 		CBRS_TOP, AFX_IDW_DIALOGBAR))
// 	{
// 		TRACE0("Failed to create dialogbar\n");
// 		return -1;		// fail to create
// 	}
// 	if ( !m_wndTopReBar.Create(this,RBS_TOOLTIPS) )
//       return -1;
//   //
//   CRect rcClient;
//   MyClientRect( rcClient );
//   if ( !m_wndTopReBar.AddBar(&m_wndTool,0,0,RBBS_BREAK|RBBS_NOGRIPPER) )
//     return -1;
//   if ( !m_wndTopReBar.AddBar(&m_wndChoiseBar,0,0,RBBS_BREAK|RBBS_NOGRIPPER) )
//     return -1;
//   if ( !m_wndStatusBar.MyCreate(this, rcClient.Width()) )
//     return -1;
//   if (!m_wndDownBar.Create(this, rcClient.Width(), IDD_DOWN_BAR,CBRS_ALIGN_BOTTOM, AFX_IDW_DIALOGBAR))
// 	{
// 		TRACE0("Failed to create dialogbar\n");
// 		return -1;		// fail to create
// 	}
//   //
//   MyClientRect( rcClient );
//   ClientToScreen( rcClient );
//   //
//   if ( !m_wndOne.MyCreate( this, &rcClient ) )
// 		return -1;
//   //
//   if ( dwTypeFrame == 0 )
//     m_pFaceplate = CControlFrm::MyCreate(this);
//   //
//     if (( m_wndDubl->m_nMonitor > 0 ) || theApp.IsAllInOne())
//     {
//     if ( !m_wndDubl->MyCreate( this ) )
//       return -1;
//     }
//     m_wndOne .SetNeighbourFrame(m_wndDubl);
//     m_wndDubl->SetNeighbourFrame(&m_wndOne );
//     //}
// 	if ( theApp.IsAllInOne() && !theApp.DisableKirishiView())
//     if ( !m_wndThird .MyCreate(this)
// 	  || !m_wndFourth.MyCreate(this)
// 	  || !m_wndTop   .MyCreate(this)
// 	  || !m_wndLeft1 .MyCreate(this)
// 	  || !m_wndLeft2 .MyCreate(this)
// 	  || !m_wndLeft3 .MyCreate(this)
// 	  || !m_wndLeft4 .MyCreate(this)
// 	  )//|| !m_wndSensor.MyCreate(this))
// 	  return -1;

//     if(theApp.m_bFourMonitors)
//     {
//      if ( m_wndThird.m_nMonitor > 0 )
//        if ( !m_wndThird.MyCreate( this ) )
//          return -1;
//      if ( m_wndFourth.m_nMonitor > 0 )
//        if ( !m_wndFourth.MyCreate( this ) )
//          return -1;
//     }
//   //
//   if(theApp.bUseWDL())
//   {
//    std::string mnemoname = theApp.GetTexnoValueByKey("Honeywell_Start");
//    if(mnemoname.size())
//    {
//      std::string SafeviewCategory = CHTML::GetSafeviewCategory(mnemoname.c_str());
//      CAmmFrm* pafrm = theApp.GetCategotyFrm(SafeviewCategory);
//      if(pafrm)
//         pafrm->GoToPage(mnemoname.c_str(), NULL);
//    }
//   }

//   //Create & init Control Panel
//   m_wndPanel.MyCreate( this );
//   ChangeFast();
//   OpenMnemoWnd( m_wndOne, m_hWnd, m_hWndMN, "Мнемосхемы", false, false );
//   if ( ::IsWindow( m_hWndMN ) )
//     ::ShowWindow( m_hWndMN, SW_HIDE );
//   //
//   OnTimer((UINT)m_nTimer);
//   return 0;
//   }

// void CAplicFrm::OnDestroy()
//   {
//   KillTimer( m_nTimer );
//   CFrameWnd::OnDestroy();
//   }

// void CAplicFrm::MyClientRect( RECT& rect )
//   {
// 	AFX_SIZEPARENTPARAMS layout;
// 	HWND hWndLeftOver = NULL;

// 	layout.bStretch = TRUE;
// 	layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
// 	GetClientRect(&layout.rect);    // starting rect comes from client rect

//   for (HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
// 		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
//   	{
// 		UINT_PTR nIDC = ((UINT)(WORD)::GetDlgCtrlID(hWndChild));
// 		CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
// 		if (nIDC == AFX_IDW_PANE_FIRST)
// 			hWndLeftOver = hWndChild;
// 		else if (nIDC >= 0 && nIDC <= 0xffff && pWnd != NULL)
// 			::SendMessage(hWndChild, WM_SIZEPARENT, 0, (LPARAM)&layout);
// 	  }
//   rect = layout.rect;
//   }

// BOOL CAplicFrm::OnNcActivate(BOOL bActive)
//   {
//   CWnd* pWnd = GetForegroundWindow();
//   if(pWnd != NULL && ::IsWindow(pWnd->m_hWnd)
//     && pWnd->GetTopLevelParent() == this)
//     bActive = TRUE;

//   if (!IsWindowEnabled())
//     bActive = FALSE;
//   BOOL m_b= (BOOL)DefWindowProc(WM_NCACTIVATE, bActive, 0L);
//   PaintExtButton();
//   return m_b;
//   }
// //
// void CAplicFrm::OnNcLButtonDown(UINT nHitTest, CPoint point)
// {
//   if ((!theApp.IsAllInOne() && !theApp.m_bFourMonitors) || theApp.DisableKirishiView())
//   {
//       __super::OnNcLButtonDown(nHitTest, point);
//       return;
//   }

//   CFrameWnd::OnNcLButtonDown(nHitTest, point);
//   if (( nHitTest == HTCAPTION )||( nHitTest == HTCLOSE ))
//     {
//     m_bNotActive = false;
//     CRect rect = GalkaButton(true);
//     if ( rect.PtInRect( point ) )
//       {
//          if(m_bGalkaOn)
//             {
//             m_bGalkaOn = false;
//             m_wndOne.m_bGalkaOn = false;
//             SetOnliOneAmm(&m_wndOne);
//             }
//          else
// 		 {
// 			 m_bGalkaOn = true;
// 			 m_wndOne.m_bGalkaOn = true;
// 		 }
//       }
//     PaintExtButton();
//     }
// }
// //
// void CAplicFrm::OnTimer(UINT_PTR nIDEvent)
//   {

//   if (m_nTimer == nIDEvent )
//     {
//     if ( *theApp.m_szTempMessZoneText )
//       {
//       int yy = 90;
//       }
//     // Грубо, но надёжно
//     if ( !(dwMainTimer % 10 ) )
//       {
//       HWND hWnd;
//       if ( IsKeyCOM1( hWnd ) )
//         ::SendMessage( hWnd, nMessageKeyboard, 0x10000, (LPARAM)m_hWnd );
//       }
// 	  dwMainTimer++;
//     //

// 	mPipe.Refresh();
//     mSocket.Refresh();
//     //
//  	m_wndOne   .MainTimer(dwMainTimer);
//     m_wndDubl->MainTimer(dwMainTimer);
//     if(theApp.m_bFourMonitors)
//       {
//        m_wndThird.MainTimer(dwMainTimer);
//        m_wndFourth.MainTimer(dwMainTimer);
//       }
// 	if (theApp.IsAllInOne())
// 	{
// 		m_wndThird .MainTimer(dwMainTimer);
// 		m_wndFourth.MainTimer(dwMainTimer);
// 		m_wndTop   .MainTimer(dwMainTimer);
// 		m_wndLeft1 .MainTimer(dwMainTimer);
// 		m_wndLeft2 .MainTimer(dwMainTimer);
// 		m_wndLeft3 .MainTimer(dwMainTimer);
// 		m_wndLeft4 .MainTimer(dwMainTimer);
// 		//m_wndSensor.MainTimer(dwMainTimer);
// 	}
//     m_wndOper  .MainTimer(dwMainTimer);
//     m_wndSens  .MainTimer(dwMainTimer);
//     m_wndPole  .MainTimer(dwMainTimer);
//     //
//     ::MainTimer( m_hWndHW, dwMainTimer );
//     ::MainTimer( m_hWndMN, dwMainTimer );
//     //
//     m_wndPanel.UpdateLamp();
//     //
//     for ( int n = 0; n < _MAX_FACE; n++ )
//     if  ( m_arrFace[n] )
//       m_arrFace[n]->MainTimer(dwMainTimer);
//     //
//     //
//     for ( int pp = 0; pp < 4; pp++ )
//       for ( int nn = 0; nn < theApp.m_nFaceplatsPerScreen; nn++ )
//         if ( m_arrFace4x4[pp][nn] )
//           m_arrFace4x4[pp][nn]->MainTimer(dwMainTimer);
//     //
//     if (m_pFaceplate!=NULL)
//       m_pFaceplate->MainTimer(dwMainTimer);
//     //
//     for ( UINT n  = 0; n < _MAX_WND; n++ )
//     if  ( m_arrWnd[n] )
//       m_arrWnd[n]->MainTimer(dwMainTimer);
//     //
//     if (m_bUpdate)
//     m_msgAL.MainTimer();
//     m_msgSA.MainTimer();
//     m_msgOG.MainTimer();
//     m_wndDownBar.MainTimer(dwMainTimer);
//     //
//     if ( IsWorkModel() )
//       {
// //Sergej      int nFlag = ViewEventBox(GiveSystem().SysMsg,m_qwKaputFlag);
//       }
//     m_wndStatusBar.SetAlarm(m_msgAL);
// 	m_wndStatusBar.SetEvent(m_msgSA);
//     m_wndStatusBar.ChangeTime(GiveModelTime());
// 	if (m_bMustClearPipeAndNumberForKillFocus)
// 	{
// 		if ((m_dTimeOfKillFocus+3)<dwMainTimer)
// 			SetPipeAndNumberForInfoBar(NULL,-1);
// 	}
// 	if (theApp.m_bPanoramaInTop)
// 	{
// 			CWnd* wnd=FindWindow("Panorama3DClass",NULL);
// 			if (theApp.IsNewPanoram())
// 				wnd=FindWindow(NULL, "Panorams");
// 			if (wnd!=NULL)
// 			{
// 				wnd->SetForegroundWindow();
// 				wnd->ShowWindow(SW_SHOWNORMAL);
// 				if (wnd->IsWindowVisible())
// 					theApp.m_bPanoramaInTop=false;
// 			}
// 	}
//   }
//   if (m_pMnemoStates!=NULL)
// 		m_pMnemoStates->OnTimer();
//   CFrameWnd::OnTimer(nIDEvent);
//   }
// //
// BOOL CAplicFrm::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//   {
//   if ( nID == 999 )// От диалога электродов ЭДГ
//   if ( m_wndPanel.IsWindowVisible() )
//   if ( m_wndPanel.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//     return TRUE;
//   //
//   CButtonFrm* pFrm = GetActiveButtonFrm();
//   if( theApp.IsAllInOne() || theApp.m_bFourMonitors)
//      pFrm = &m_wndOne; //в этом случае, а для других frm при m_bFourMonitors сообщения приходят в них
//   if ( pFrm && pFrm != this && ::IsWindow(*pFrm) )
//   if ( pFrm->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//     return TRUE;
//   //
//   if ( ::IsWindow( m_wndOper ) )
//   if ( m_wndOper.IsWindowVisible() )
//   if ( m_wndOper.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//     return TRUE;
//   //
//   if ( ::IsWindow( m_wndPole ) )
//   if ( m_wndPole.IsWindowVisible() )
//   if ( m_wndPole.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//     return TRUE;
//   //
//   return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//   }

// void CAplicFrm::OnUpdateWorker(CCmdUI *ui)
//   {
//   if ( ::IsMnemoStyle(MNEMO_EDIT) )
//     ui->SetText("Режим Оператора");
//   else
//     ui->SetText("Режим Разработчика");
//   }

// void CAplicFrm::OnSysCommand(UINT nID, LPARAM lParam)
//   {
//   switch ( nID )
//     {
//     case ID_APP_ABOUT : theApp.OnAppAbout();break;
//     case ID_TUNING    : theApp.OnTuning  ();break;
//     case ID_KALIBROVKA:
//       if ( ::IsWindow( m_wndOper ) )
//       if ( m_wndOper.IsWindowVisible() )  m_wndOper.OnKalibrovka();
//       if ( ::IsWindow( m_wndPole ) )
//       if ( m_wndPole.IsWindowVisible() )  m_wndPole.OnKalibrovka();
//     break;
//     case ID_WORKER:
//       ::MnemoStyleXOR( MNEMO_EDIT );
//     break;
//     };
//   //
//   if ( nID == SC_CLOSE ) // Блин, надоело по ошибке закрывать программу
//     {
//     int nRet = MessageBox("Вы уверены в выходе из программы?",
//                 NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST );
//     if ( nRet != IDYES )
//       return;
// //Serg		m_wndSxema.ShowOrHide(SW_HIDE,false);
//     }
//   CFrameWnd::OnSysCommand(nID, lParam);
//   }

// BOOL CAplicFrm::DestroyWindow()
//   {
// 	m_wndPanel.bCanDestroy = true;
// 	return CFrameWnd::DestroyWindow();
//   }

// void CAplicFrm::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
// {
//   CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
//   if(::IsWindow(m_wndPanel) && nState == WA_ACTIVE){
// 	m_wndPanel.ShowWindow(SW_SHOW );
//   }
//   if ( theApp.Instructor() )
//     {
//     if ( nState == WA_INACTIVE )
//       { // Выключаем сирену, если это надо
//       }
//     else
//     if ( nState == WA_ACTIVE )
//       { // Включаем сирену, если это надо
//       if ( ::IsWindow(m_wndPanel) )
//       m_wndPanel.ShowWindow( SW_SHOW );
//       }
//     }
//   }

// void CAplicFrm::OnActivateApp(BOOL bActive, DWORD dwThreadID)
//   {
//   CFrameWnd::OnActivateApp(bActive, dwThreadID);
//   if ( bActive && ::IsWindow(m_wndPanel) )
//     m_wndPanel.ShowWindow( SW_SHOW );
//   }
// //
// void CAplicFrm::OnPaint()
//   {
//   CPaintDC dc(this);
//   CRect rc;
//   GetClientRect(rc);
//   dc.FillSolidRect(rc,RGB(0,0,0));
//   }
// //
// void CAplicFrm::ShowBar( BARS bar )
//   {
//   switch ( bar )
//     {
//     case barGROUP :
//       if ( CallGroup() )
//         return;
//       break;
//     case barTREND:
// 	  if (  CallTrendGroupFromPoint())
//         return;
// 	  else
//       if (  CallTrendDetail(bar) )
//         return;
//       break;
//      case barACK :
// 	 {
// 		 if (IsGoodPointFromInfo())
// 		 {
// 			 AckTag(*m_ptr_pipeForInfoBar,m_nNumberForInfoBar);
// 			 return;
// 		 }
// 		 break;
// 	  }
//     }
//   m_wndChoiseBar.ShowBar(bar);
//   }
// //
// CAmmFrm* CAplicFrm::GiveAmm(HWND hWnd, bool exceptOne)
//   {
//   if(theApp.bUseWDL())
//   {
//      CAmmFrm* pamm = NULL;
//      return pamm;
//   }
//   if ( ::IsWindow(hWnd) )
//     {
//     do
//       {
//       if ( hWnd == m_wndOne .GetMnemo()->m_hWnd )
// 	  {
// 		  if (theApp.IsAllInOne() && exceptOne && ::IsWindow(m_wndThird.m_hWnd)) return &m_wndThird;
// 		  else return &m_wndOne ;
// 	  }
//       //if ( hWnd == m_wndDubl->GetMnemo()->m_hWnd ) return m_wndDubl;
//       if ( m_wndDubl->IsMnemo(hWnd)) return m_wndDubl;
//       if (theApp.m_bFourMonitors)
//       {
//           if ( hWnd == m_wndThird .GetMnemo()->m_hWnd ) return &m_wndThird ;
// 		  if ( hWnd == m_wndFourth.GetMnemo()->m_hWnd ) return &m_wndFourth;
//       }
//       else
// 	  if (theApp.IsAllInOne())
// 	  {
// 		  if ( hWnd == m_wndThird .GetMnemo()->m_hWnd ) return &m_wndThird ;
// 		  if ( hWnd == m_wndFourth.GetMnemo()->m_hWnd ) return &m_wndFourth;
// 		  if ( hWnd == m_wndTop   .GetMnemo()->m_hWnd ) return &m_wndTop   ;
// 		  if ( hWnd == m_wndLeft1 .GetMnemo()->m_hWnd ) return &m_wndLeft1 ;
// 		  if ( hWnd == m_wndLeft2 .GetMnemo()->m_hWnd ) return &m_wndLeft2 ;
// 		  if ( hWnd == m_wndLeft3 .GetMnemo()->m_hWnd ) return &m_wndLeft3 ;
// 		  if ( hWnd == m_wndLeft4 .GetMnemo()->m_hWnd ) return &m_wndLeft4 ;
// 		  //if ( hWnd == m_wndSensor.GetMnemo()->m_hWnd ) return (CAmmFrm*)&m_wndSensor;
// 	  }
//       hWnd = ::GetParent(hWnd);
//       }
//     while ( ::IsWindow(hWnd) );
//     }
//   if (theApp.IsAllInOne() && exceptOne && ::IsWindow(m_wndThird.m_hWnd)) return &m_wndThird;
//   return ActiveFrm();
//   }
// //
// CAmmFrm* CAplicFrm::GiveRegularAmm(HWND hWnd)
// {
//     if(CAmmFrm*frm = GetSelectedMnemoFrm())
//       return frm; //есть выбранная

//     if (hWnd!=NULL)
//     if ( hWnd != m_wndTop.GetMnemo()->m_hWnd )
//         return GiveAmm(hWnd);
   
//     for(int ii=0; ii < 4; ii++)
//     switch(m_nOneDublThirdFourthCounter)
//     {
//       default:
//       case  0:
//             m_nOneDublThirdFourthCounter = 1;
//             if(theApp.m_bFourMonitors && !m_wndOne)
//                continue;
//             return &m_wndOne;
//       case  1:
//             m_nOneDublThirdFourthCounter = 2;
//             if(theApp.m_bFourMonitors && !m_wndDubl)
//               continue;
//             return m_wndDubl->m_hWnd?m_wndDubl:&m_wndOne;
//       case  2:
//             m_nOneDublThirdFourthCounter = 3;
//             if(theApp.m_bFourMonitors && !m_wndFourth)
//                continue;
//             return m_wndFourth.m_hWnd?&m_wndFourth:&m_wndOne;
//       case  3:
//             m_nOneDublThirdFourthCounter = 0;
//             if(theApp.m_bFourMonitors && !m_wndThird)
//                continue;
// 			return m_wndThird.m_hWnd?&m_wndThird:&m_wndOne;
//     }

//     return NULL;
// }
// //
// CAmmFrm* CAplicFrm::GivePrevRegularAmm()
// {
//     if(CAmmFrm*frm = GetSelectedMnemoFrm())
//       return frm; //есть выбранная
 
//     switch(m_nOneDublThirdFourthCounter)
//     {
//       default:
//       case  0:
//             return m_wndDubl;
//       case  1:
//             return &m_wndThird;
//       case  2:
//             return &m_wndOne;
//       case  3:
//             return &m_wndFourth;
//     }
// }
// //четыре левых небольших окна
// CAmmFrm* CAplicFrm::GiveRegularLeftAmm(HWND hWnd)
// {
//     if(CAmmFrm*frm = GetSelectedLeftFrm())
//       return frm; //есть выбранная

//     m_nLeftWindowsCounter = ++m_nLeftWindowsCounter%4;
//     switch(m_nLeftWindowsCounter)
//     {
//       default:
//       case  0:
//             return &m_wndLeft4;
//       case  1:
//             return &m_wndLeft1;
//       case  2:
//             return &m_wndLeft2;
//       case  3:
//             return &m_wndLeft3;
//     }
// }
// //
// CAmmFrm* CAplicFrm::GiveAmmByhWnd(HWND hWnd)
// {
//     CAmmFrm *MnemoFrms[] = {&m_wndOne, m_wndDubl, &m_wndThird, &m_wndFourth, NULL};
//     for(int ii=0; ii<_countof(MnemoFrms) && MnemoFrms[ii]; ii++)
//       {
//           if(MnemoFrms[ii]->m_wndMnemo.m_hWnd == hWnd)
//             return MnemoFrms[ii];
//       }
//     return NULL;
// }
// //
// CAmmFrm* CAplicFrm::GiveAmmByPosition(int nPosition)
// {
//     CAmmFrm *MnemoFrms[] = {&m_wndOne, m_wndDubl, &m_wndThird, &m_wndFourth, NULL};
//     for(int ii=0; ii<_countof(MnemoFrms) && MnemoFrms[ii]; ii++)
//       {
//           if(MnemoFrms[ii]->m_nPosition == nPosition)
//             return MnemoFrms[ii];
//       }
//     return NULL;
// }
// //
// void CAplicFrm::GlobalUndo()
// {
//   if( m_UndoList.size())
//     {
//      UndoRedoRec rec = m_UndoList.back();
//      m_UndoList.pop_back();
//      if(rec.nPosition == 11)
//        {//запись из сенсорной панели
//          if(IsWindow(m_wndSens.m_wndMnemo->m_hWnd))
//            {
//               if(!m_wndSens.m_wndMnemo->mDoc.m_strFile.IsEmpty())
//                 {
//                   UndoRedoRec rec2 = {m_wndSens.m_wndMnemo->mDoc.m_strFile, m_wndSens.m_wndMnemo->mDoc.m_strGroup, 11};
//                   m_RedoList.push_back(rec2);
//                 }
//               m_wndSens.m_wndMnemo->BuildQueue( rec.name.c_str(), rec.param.c_str() );
//               SaveString( m_wndSens.m_SensorName.c_str(), rec.name.c_str() ); //в ini
//            }
//         return;
//        }
//      CAmmFrm* amm = GiveAmmByPosition(rec.nPosition);
//      if(amm)
//        {
//        if(!amm->m_wndMnemo.mDoc.m_strFile.IsEmpty())
//          {
//           UndoRedoRec rec2;
//           rec2.name = amm->m_wndMnemo.mDoc.m_strFile;
//           rec2.param = amm->m_wndMnemo.mDoc.m_strGroup;
//           rec2.nPosition = amm->m_nPosition;
//           m_RedoList.push_back(rec2);
//          }
//        amm->m_wndMnemo.BuildQueue( rec.name.c_str(), rec.param.c_str() );
//        SaveString( amm->m_wndMnemo.mDoc.pipe.m_szPipeName, rec.name.c_str() ); //в ini
//        }
//     }
// }
// //
// void CAplicFrm::GlobalRedo()
// {
//   if( m_RedoList.size())
//     {
//      UndoRedoRec rec = m_RedoList.back();
//      m_RedoList.pop_back();
//      if(rec.nPosition == 11)
//        {//запись из сенсорной панели
//          if(IsWindow(m_wndSens.m_wndMnemo->m_hWnd))
//            {
//               if(!m_wndSens.m_wndMnemo->mDoc.m_strFile.IsEmpty())
//                 {
//                   UndoRedoRec rec2 = {m_wndSens.m_wndMnemo->mDoc.m_strFile, m_wndSens.m_wndMnemo->mDoc.m_strGroup, 11};
//                   m_UndoList.push_back(rec2);
//                 }
//               m_wndSens.m_wndMnemo->BuildQueue( rec.name.c_str(), rec.param.c_str() );
//               SaveString( m_wndSens.m_SensorName.c_str(), rec.name.c_str() ); //в ini
//            }
//         return;
//        }
//      CAmmFrm* amm = GiveAmmByPosition(rec.nPosition);
//      if(amm)
//        {
//        if(!amm->m_wndMnemo.mDoc.m_strFile.IsEmpty())
//          {
//           UndoRedoRec rec2;
//           rec2.name = amm->m_wndMnemo.mDoc.m_strFile;
//           rec2.param = amm->m_wndMnemo.mDoc.m_strGroup;
//           rec2.nPosition = amm->m_nPosition;
//           m_UndoList.push_back(rec2);
//          }
//         amm->m_wndMnemo.BuildQueue( rec.name.c_str(), rec.param.c_str() );
//         SaveString( amm->m_wndMnemo.mDoc.pipe.m_szPipeName, rec.name.c_str() ); //в ini
//        }
//     }
// }
// //
// void CAplicFrm::GlobalUndoListPushBack(UndoRedoRec &rec)
// {
//     if( m_UndoList.size()>=99) //99 - предельное число записей в списке undo
//       {
//        int N = (int)m_UndoList.size() - (99-1);
//        m_UndoList.erase(m_UndoList.begin(), m_UndoList.begin()+N);
//       }
//     m_UndoList.push_back(rec);
// }
// //
// void CAplicFrm::SetOnliOneAmm(CAmmFrm *ammFrm)
// {
//     if(!ammFrm)
//       return;
//     CAmmFrm *LeftFrms[] = {&m_wndLeft1, &m_wndLeft2, &m_wndLeft3, &m_wndLeft4, NULL};
//     CAmmFrm *MnemoFrms[] = {&m_wndOne, m_wndDubl, &m_wndThird, &m_wndFourth, NULL};
//     CAmmFrm **Frms = NULL;
//     //находим к какой из двух групп относится форма
//     for(int ii=0; ii<_countof(LeftFrms); ii++)
//       if(LeftFrms[ii]==ammFrm)
//         {
//             Frms = LeftFrms;
//             break;
//         }
//     if(!Frms)
//      for(int ii=0; ii<_countof(MnemoFrms); ii++)
//        if(MnemoFrms[ii]==ammFrm)
//         {
//             Frms = MnemoFrms;
//             break;
//         }
    
//     if(Frms)
//       //меняем галки оставляя одну в группе
//       for(int ii=0; Frms[ii]; ii++)
//       {
//          if(Frms[ii] == ammFrm)
//             Frms[ii]->m_bGalkaOn = false;
//          else {
//                Frms[ii]->m_bGalkaOn = true;
//                if(Frms[ii] == &m_wndOne)
//                  {
//                  m_bGalkaOn = true;
//                  PaintExtButton();
//                  }
//               }
//          Frms[ii]->PaintExtButton();
//       }
// }
// //
// CAmmFrm *CAplicFrm::GetSelectedLeftFrm()
// {
//     if(!m_wndLeft1.m_bGalkaOn)
//       return &m_wndLeft1;
//     if(!m_wndLeft2.m_bGalkaOn)
//       return &m_wndLeft2;
//     if(!m_wndLeft3.m_bGalkaOn)
//       return &m_wndLeft3;
//     if(!m_wndLeft4.m_bGalkaOn)
//       return &m_wndLeft4;
//     return NULL;
// }
// //
// CAmmFrm *CAplicFrm::GetSelectedMnemoFrm()
// {
//     if(m_wndOne && !m_wndOne.m_bGalkaOn)
//       return &m_wndOne;
//     if(m_wndDubl && !m_wndDubl->m_bGalkaOn)
//       return m_wndDubl;
//     if(m_wndThird && !m_wndThird.m_bGalkaOn)
//       return &m_wndThird;
//     if(m_wndFourth && !m_wndFourth.m_bGalkaOn)
//       return &m_wndFourth;
//     return NULL;
// }
// //
// void CAplicFrm::ButtonOk( enum BARS bar, const char* pszText, HWND hWnd )
//   {
// 	switch (bar)
// 	  {
//     case barPAGE:
//       {
//       //bool N = TrueMnemoName( pszText );
//       bool N = CHTML::IsTrueMemoname( pszText );
//       if ( !N )
// 	  {
//         if (lstrcmp(pszText,""))
// 			AfxMessageBox("ERROR: WRONG PAGE NAME");
// 	  }
//       else
//       {
//         if(theApp.bUseWDL())
//         {
//             std::string SafeviewCategory = CHTML::GetSafeviewCategory(pszText);
//             CAmmFrm* pafrm = theApp.GetCategotyFrm(SafeviewCategory);
//             if(pafrm)
//               pafrm->GoToPage(pszText, NULL);
//         }
//         else
//         {
//         if (!theApp.IsAllInOne() && !theApp.m_bFourMonitors)
//            GiveAmm(hWnd)->GoToPage(pszText,NULL);
//         else{
//             CAplicFrm *aplcFrm = GetAppWnd();
//             if ( aplcFrm->m_wndSens && hWnd == aplcFrm->m_wndSens.m_wndMnemo->m_hWnd )
//                 GetAppWnd()->m_wndSens.m_wndMnemo->BuildQueue(pszText, NULL);
//             else GiveAmm(hWnd)->GoToPage(pszText, NULL);
//             }
//         }
//       }
//       }
//     break;
// 	  case barTREND:
//       {
//       int N = atoi( pszText );
//       if ( !(0 < N && N <= 3000) )
//         AfxMessageBox("ERROR: WRONG TREND NUMBER");
//       else
//         GetAppWnd()->GiveRegularAmm(hWnd)->GoToPage("sysTnd03",pszText);
//       }
//     break;
//     case barGROUP:
//       {
//       int N = TrueGroupNumber( pszText );
//       if ( N == -1 )
//         AfxMessageBox("ERROR: WRONG GROUP NUMBER");
//       else
//         {
//         int n = ReadInt( "GroupView", 0 ) % 3;
//         _static char* file[] = { "sysGrpDetail", "sysGrpTrend", "sysGrpNumeric" };
//         GiveAmm(hWnd)->GoToPage(file[n],pszText);
//         }
//       }
//     break;
//     case barUnitTrend:
//       {
//       int N = TrueGroupNumber( pszText );
//       if ( N == -1 )
//         AfxMessageBox("ERROR: WRONG GROUP NUMBER");
//       else
//         {
//         SaveInt( "GroupView", 1 );
//         int n = ReadInt( "GroupView", 0 ) % 3;
//         _static char* file[] = { "sysGrpDetail", "sysGrpTrend", "sysGrpNumeric" };
//         GiveAmm(hWnd)->GoToPage(file[n],pszText);
//         }
//       }
//     break;
//     case barDETAIL:
//       {
//       UINT nType;
//       char szObj[_MAX_PATH]="";
//       char szHoney[_MAX_PATH];
//       LPCTSTR szTag = QueryFaceplate( pszText, szHoney, &nType );
//       if ( szTag && ::IsAcyEx(nType) )
//         {
//         if(strcmp(szTag, "SCANGRPDTL")==0)
//           {
//           CShCONTROLMODULE sh;
//           char *pp = NULL;
//           if(pp = strchr(szHoney, '.'))
//             *pp = 0;
//           if(mPipe.GetData( id_CONTROLMODULE, szHoney, &sh ))
//             strcpy_s(szObj, sh.SCANPNTDTL);
//           if(szObj[0]==0)
//             {
//                sprintf_s( szObj, "sysdtl%s", szTag );
//             }
//           if(pp)
//             *pp = '.';
//           }
//         else
//         if(strcmp(szTag, "GROUPDETAILDISPLAY")==0)
//           {
//           char *pp = NULL;
//           if(pp = strchr(szHoney, '.'))
//             *pp = 0;
 
//           //CShSMANALOG sh;
//           //CShSMDISCRET sh;
//           switch(nType)
//           {
//           case id_SMFLD:
//             {
//             CShSMFLD sh;
//             if(mPipe.GetData( nType, szHoney, &sh ))
//               strcpy_s(szObj, sh.POINTDETAILDISPLAY);
//             if(szObj[0]==0)
//              {
//                sprintf_s( szObj, "sysdtl%s", szTag );
//              }
//             if(pp)
//               *pp = '.';
//             }
//             break;
//           }
//          }
//         else
//         sprintf_s( szObj, "sysdtl%s", szTag );
//         GiveAmm(hWnd)->GoToPage( szObj, pszText );
//         }
//       else
//         AfxMessageBox("ERROR: WRONG TAG NAME");
//       }
//     break;
//     case barACK:
//       AckMnemo();
//     break;
//     }
//   }
// //
// void CAplicFrm::OnButtonOk()
//   {
//   CString strName="";
//   if(m_wndChoiseBar.m_barType<barACK)
//     m_wndChoiseBar.GetDlgItem(IDC_EDITNAME)->GetWindowTextA(strName);
//   ButtonOk( m_wndChoiseBar.m_barType, strName );
//   m_wndChoiseBar.HideItems();
//   m_wndChoiseBar.m_barType=barINFO;
//   }
// //
// void CAplicFrm::OnButtonCancel()
//   {
//   m_wndChoiseBar.HideItems();
//   m_wndChoiseBar.m_barType=barINFO;
//   }

// void CAplicFrm::ShowInfoBar(BARS barType,CString strInfo)
// {
// 	m_wndChoiseBar.ShowBar(barType,strInfo);
// }

// void CAplicFrm::HideInfoBar(bool bFromKillFocus)
// {
// 	m_wndChoiseBar.HideItems(bFromKillFocus);
// 	m_bMustClearPipeAndNumberForKillFocus=bFromKillFocus;
// 	m_dTimeOfKillFocus=dwMainTimer;
// }
// //
// void CAplicFrm::ZoomMnemo(int nChoise)
//   {
//   m_wndOne.ZoomMnemo(nChoise);
//   }
// //
// CAmmFrm* CAplicFrm::ActiveFrm()
// {
//   if(!theApp.m_bFourMonitors
// 	  || Version == BERTH_ONPZ)
//      return &m_wndOne;

//   static bool isOn = false; isOn ^= true;
//   if ( (m_wndOne.m_bGalkaOn && isOn)
// 	  || (!::IsWindow(*m_wndDubl) || !m_wndDubl->m_bGalkaOn) )
// 		return &m_wndOne;
// 	return m_wndDubl;
// }
// //
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::DelMnemo( CPopupMnemo* wnd )
//   {
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//   if  ( m_arrWnd[n] == wnd )
//     {
//     m_arrWnd[n] = NULL;
//     break;
//     }
//   }
// //
// void CAplicFrm::AddMnemo( CPopupMnemo* wnd )
//   {
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//   if  ( m_arrWnd[n] == NULL )
//     {
//     m_arrWnd[n] = wnd;
//     break;
//     }
//   }
// //
// void CAplicFrm::ShowMnemos(int nShow)
//   {
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//   if  ( m_arrWnd[n] != NULL )
//     {
//         m_arrWnd[n]->ShowWindow(nShow);
//     break;
//     }
//   else break;
//   }
// //
// CPopupMnemo* CAplicFrm::FindMnemoByTitle( LPCSTR title )
//   {
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//    if  ( m_arrWnd[n] == NULL && m_arrWnd[n]->GetTitle().CompareNoCase(title)==0)
//     {
//     return m_arrWnd[n];
//     break;
//     }
//    return NULL;
//   }
// //

// void CAplicFrm::HideNotPinPopups(CButtonFrm * parent)
// {
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//   {
// 	  if  ( m_arrWnd[n] != NULL && !m_arrWnd[n]->m_bPinned && m_arrWnd[n]->m_pParent == parent)
// 	  {
// 		  m_arrWnd[n]->DestroyWindow();
// 		  m_arrWnd[n] = NULL;
// 	  }
//   }
// }
// //
// bool CAplicFrm::FndMnemo( const char* pszName, const char* pszPointName  )
//   {
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//   if  ( m_arrWnd[n] != NULL )
//   if  ( lstrcmpi( m_arrWnd[n]->GetName(), pszName ) == 0 )
//     {
// 	if (pszPointName!=NULL)
// 	{
// 		CString strPointName=m_arrWnd[n]->GetPointName();
// 		if ( lstrcmpi( strPointName.GetBuffer(), pszPointName ) == 0 )
// 		{
// 			m_arrWnd[n]->SetForegroundWindow();
// 			strPointName.ReleaseBuffer();
// 			return true;
// 		}

// 	}
// 	else
// 	{
// 		m_arrWnd[n]->SetForegroundWindow();
// 		return true;
// 	}
//     }
//   return false;
//   }
// //
// bool CAplicFrm::FndMnemoAndName( const char* pszMnemoName, const char* pszHeader)
//   {
//   char szText[512];
//   for ( UINT n  = 0; n < _MAX_WND; n++ )
//   if  ( m_arrWnd[n] != NULL )
//   if  ( lstrcmpi( m_arrWnd[n]->GetName(), pszMnemoName ) == 0 )
//   if  ( (m_arrWnd[n]->GetWindowText(szText, 512)>0) && lstrcmpi( szText, pszHeader ) == 0 )
//     {
//     m_arrWnd[n]->SetForegroundWindow();
//     return true;
//     }
//   return false;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// //при нажатии на Enter в верхних тулбарах
// void CAplicFrm::OnCommandChange()
//   {
//   if (m_wndChoiseBar.GetDlgItem(IDC_BUTTONOK)->IsWindowVisible())
//     OnButtonOk();
//   }
// //
// void CAplicFrm::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
//   {
//   if ((nChar==VK_RETURN)&&(m_wndChoiseBar.GetDlgItem(IDC_BUTTONOK)->IsWindowVisible()))
//     if (m_wndChoiseBar.m_barType==barACK)
//       OnButtonOk();
//   //все изменения при нажатии Enter обрабатываются в OnCommandChange
//   //но иногда IDOK не приходит, а приходит VK_RETURN
//   CButtonFrm::OnKeyUp(nChar, nRepCnt, nFlags);
//   }
// //
// void CAplicFrm::ChoiseFaceplate( const char* name, CAmmFrm* ammFrm, const char * fp, const char* pv, const char * rst, const char * desc )
//   {
//   CharMP szHoney;
//   LPCTSTR szQuery = QueryFaceplate( name, szHoney, NULL, fp );
//   if ( szQuery == NULL && fp == NULL ) return;
//   if (!szQuery && Version == LG35_8_KF && !strcmp(fp, "sysdtlflaga_rr_fp.htm"))
//   {
// 	  strcpy(szHoney, name);
//   }
//   else{
//   //
//   // Запрет на дублирование
//   for ( int n = 0; n < _MAX_FACE; n++ )
//   if ( m_arrFace[n] != NULL )
//   if ( m_arrFace[n]->IsYou( szHoney ) )
//     return;
//   }
//   //
//   switch ( dwTypeFrame )
//     {
//     case 0: // Новгород Аммиак
//     if (m_pFaceplate != NULL )
//       {
//       m_pFaceplate->GetMnemo()->Faceplate( name );
//       m_pFaceplate->SetWindowText(name);
//       m_pFaceplate->SetInfoBar(name);
//       m_pFaceplate->SetForegroundWindow();
//       m_pFaceplate->SetCross();
//       }
//     break;
//     case 1: // Гродно Аммиак
//       {
//       if ((!theApp.IsAllInOne() && !theApp.m_bFourMonitors) || !ammFrm)
//       {
//       for ( int n = 0; n < 2; n++ )
//       if  ( m_arrFace[n] == NULL )
//         {
//         m_arrFace[n] = CControlFrm::MyCreate( this, n );
//         if ( m_arrFace[n] == NULL ) return;
//         m_nActiveFaceplate = n;
//         break;
//         }
//       //
//       if ( 0 <= m_nActiveFaceplate && m_nActiveFaceplate < 2 )
//         {
//         int n = m_nActiveFaceplate;
//         if ( m_arrFace[n] != NULL )
//           {
//           m_arrFace[n]->Activate( szHoney );
//           m_arrFace[n]->MainTimer(dwMainTimer); //чтобы не по таймеру а сразу
//           }
//         m_nActiveFaceplate = 1-n;
//         }
//       }
//       else
//         {
//         int pp = ammFrm->m_nPosition-1;
//         // Запрет на дублирование
// 		if (/*!szQuery && */(Version == LG35_8_KF || theApp.m_bFourMonitors) && (!fp || strcmp(fp, "sysdtlflaga_rr_fp.htm")))
//         {
//         for ( int n = 0; n < theApp.m_nFaceplatsPerScreen; n++ )
//           if ( m_arrFace4x4[pp][n] != NULL )
//             if ( m_arrFace4x4[pp][n]->IsYou( szHoney ) )
//                return;
// 		}
//         //
//         for ( int n = 0; n < theApp.m_nFaceplatsPerScreen; n++ )
//         if  ( m_arrFace4x4[pp][n] == NULL )
//           {
//           m_arrFace4x4[pp][n] = CControlFrm::MyCreateByParent( ammFrm, n );
//           if ( m_arrFace4x4[pp][n] == NULL )
//              return;
//           m_nActiveFaceplates[pp] = n;
//           break;
//           }
//         //
//         if ( 0 <= m_nActiveFaceplates[pp] && m_nActiveFaceplates[pp] < theApp.m_nFaceplatsPerScreen )
//           {
//           int n = m_nActiveFaceplates[pp];
//           if ( m_arrFace4x4[pp][n] != NULL )
//             {
//             m_arrFace4x4[pp][n]->Activate( szHoney, fp, pv, rst, desc );
//             m_arrFace4x4[pp][n]->MainTimer(dwMainTimer); //чтобы не по таймеру а сразу
//             }
//           m_nActiveFaceplates[pp] =  (n+1)%theApp.m_nFaceplatsPerScreen;
//           }
//         }
//       }
//     break;
//     case 2: // Гродно Циклогексанон
//       {
//       int n;
//       for ( n = 0; n < _MAX_FACE; n++ )
//       if  ( m_arrFace[n] == NULL )
//         {
//         m_arrFace[n] = CControlFrm::MyCreate( &m_wndOne );
//         if ( m_arrFace[n] == NULL ) return;
//         break;
//         }
//       //
//       if ( n == _MAX_FACE )
//         {
//         CControlFrm* face = m_arrFace[0];
//         for ( n = 0; n < (_MAX_FACE-1); n++ )
//           m_arrFace[n] = m_arrFace[n+1];
//         m_arrFace[n] = face;
//         }
//       //
//       if ( m_arrFace[n] != NULL )
//         m_arrFace[n]->Activate( szHoney );
//       }
//     break;
//     }
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::IntellectGotoPage( const char* pszName )
//   {
//   if ( IsShowSxeme() )
//   if ( ::IsWindow( m_wndPole ) && m_wndPole.IsWindowVisible() )
//     {
//          if (m_wndOne   .IsShowSxeme()) m_wndOne   .GoToPage(pszName, NULL);
//     else if (m_wndDubl->IsShowSxeme()) m_wndDubl->GoToPage(pszName, NULL);
// 	if (theApp.IsAllInOne())
// 	{
// 		     if (m_wndThird .IsShowSxeme()) m_wndThird .GoToPage(pszName, NULL);
// 		else if (m_wndFourth.IsShowSxeme()) m_wndFourth.GoToPage(pszName, NULL);
// 		else if (m_wndTop   .IsShowSxeme()) m_wndTop   .GoToPage(pszName, NULL);
// 		else if (m_wndLeft1 .IsShowSxeme()) m_wndLeft1 .GoToPage(pszName, NULL);
// 		else if (m_wndLeft2 .IsShowSxeme()) m_wndLeft2 .GoToPage(pszName, NULL);
// 		else if (m_wndLeft3 .IsShowSxeme()) m_wndLeft3 .GoToPage(pszName, NULL);
// 		else if (m_wndLeft4 .IsShowSxeme()) m_wndLeft4 .GoToPage(pszName, NULL);
// 		//else if (m_wndSensor.IsShowSxeme()) m_wndSensor.GoToPage(pszName, NULL);
// 	}
//     return;
//     }
//   //
//   ActiveFrm()->GoToPage( pszName, NULL );
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::IntellectForsunka( WPARAM wParam, LPARAM lParam )
//   {
//   if ( IsShowSxeme() )
//   if ( ::IsWindow( m_wndPole ) && m_wndPole.IsWindowVisible() )
//     {
//     if ( m_wndOne.IsShowSxeme() )
//       m_wndOne.Forsunka( wParam, lParam );
//     else
//     if ( m_wndDubl->IsShowSxeme() )
//       m_wndDubl->Forsunka( wParam, lParam );
//     return;
//     }
//   ActiveFrm()->Forsunka( wParam, lParam );
//   }
// void CAplicFrm::SetLastActiveMnemoForPopups(HWND hWnd)
// {
// 	m_hLastActiveMnemoForPopups = hWnd;
// }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// LRESULT CAplicFrm::OnPressQueue(WPARAM wParam,LPARAM lParam)
//   {
//   WORD nCode = HIWORD(wParam);
//   switch ( LOWORD(wParam) )
//     {
//     case QUEUE_NO:
//       break;
//     case QUEUE_GOTO:
//       {
//       const char* pszName = reinterpret_cast<const char*>(lParam);
// 	  if (Version==BERTH_ONPZ)
// 	  {
// 	       CString strName(pszName);
// 		   if (strName.Find("LCP")==0)
// 			   return 0;
// 	  }
//       IntellectGotoPage( pszName );
//       }
//     break;
//     case QUEUE_SENSGETLAMPSTATE:
//         {
// 		BYTE nLampNumber = (BYTE)(lParam);
//         LRESULT nState = GetLampState(nLampNumber);
// 		return nState;
//         }
//     break;
//     case QUEUE_HWKBD:
//       HoneywellKBD( nCode, lParam );
//       break;
//     case QUEUE_POPUP:
//       {
// 	  const char* pszPopupWithCurrentPoint = reinterpret_cast<const char*>(lParam);
// 	  //Popup?CurrentPoint или просто Popup
// 	  std::string strPopupWithCurrent=pszPopupWithCurrentPoint;
//       std::string strPopup=strPopupWithCurrent;
// 	  std::string strPopupCurrentPoint="";
// 	  size_t findCurrent=strPopupWithCurrent.find("?");
// 	  if (findCurrent!=std::string::npos)
//       {
// 		std::size_t substrBegin=findCurrent+1;
//         size_t findCurrent2=strPopupWithCurrent.find("?", findCurrent+1);
//         std::size_t substrEnd;
//         if (findCurrent2!=std::string::npos)
// 		    substrEnd = findCurrent2-findCurrent-1;
//         else substrEnd = strPopupWithCurrent.length()-substrBegin;
// 		if((Version == T1165P_ONPZ || Version == T1163_ONPZ)
// 			&& strPopupWithCurrent[substrBegin] != '&')
// 			strPopupCurrentPoint = "&";
// 		strPopupCurrentPoint += strPopupWithCurrent.substr(substrBegin,substrEnd);
// 		strPopup=strPopupWithCurrent.substr(0,findCurrent);
// 	  }
// 	  if ( !FndMnemo(strPopup.c_str(), strPopupCurrentPoint.c_str()) )
//         {
//         CPopupMnemo* wnd = new CPopupMnemo();
//         if ( wnd != NULL )
//           {
// 			const char* szCurPoint=NULL;
// 			if (strPopupCurrentPoint!="")
// 				//szCurPoint=_strdup(strPopupCurrentPoint.c_str());
//                 szCurPoint=strPopupCurrentPoint.c_str();
// 			CButtonFrm * par = this;
// 			if (Version == AT_PRP)
// 			{
// 				par = this->GiveAmm(m_hLastActiveMnemoForPopups);
// 				m_hLastActiveMnemoForPopups = NULL;
// 			}
// 			if ( !wnd->MyCreate(par,strPopup.c_str(),NULL,szCurPoint) )
//                 delete wnd;
//             else
//             {
//             AddMnemo( wnd );
//             return (LRESULT)wnd->m_hWnd;
//             }
//           }
//         }
//       }
//     break;
//     case QUEUE_FORSUNKA:
//       IntellectForsunka( wParam, lParam );
//     break;
//     default:
//       ASSERT(0);
//     break;
//     }
//   return 0;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::OnCallKlavaBot()
//   {
//   if ( IsShowSxeme() )
//     return;// Признак того, что находимся в поле
//   if ( ::IsWindow( m_wndOper ) )
//     return;// Признак использования сенсорной клавиатуры
//   if ( ::IsWindow( m_hWndHW ) )
//     ::ShowWindow( m_hWndHW, SW_HIDE );
//   OpenMnemoWnd( m_wndOne, m_hWnd, m_hWndMN, "Мнемосхемы", false );
//   }

// void CAplicFrm::OnCallKlawaDop()
//   {
//   if ( IsShowSxeme() )
//     return;// Признак того, что находимся в поле
//   if ( ::IsWindow( m_wndOper ) )
//     return;// Признак использования сенсорной клавиатуры
//   if ( ::IsWindow( m_hWndMN ) )
//     ::ShowWindow( m_hWndMN, SW_HIDE );
//   // Это для того, чтобы окно не проваливалось под окно мнемосхемы
//   OpenMnemoWnd( m_wndOne, m_hWnd, m_hWndHW, "HWKBD", false );
//   }

// void CAplicFrm::OnHideKlawaDop()
//   {
//   if ( IsShowSxeme() )
//     {
//     m_wndOne .ShowOrHideMini();
//     m_wndDubl->ShowOrHideMini();
//     if(theApp.m_bFourMonitors)
//       {
//       m_wndThird.ShowOrHideMini();
//       m_wndFourth.ShowOrHideMini();
//       }
//     }
//   else
//     {
//     if ( ::IsWindow( m_hWndHW ) )
//       ::ShowWindow( m_hWndHW, SW_HIDE );
//     if ( ::IsWindow( m_hWndMN ) )
//       ::ShowWindow( m_hWndMN, SW_HIDE );
//     }
//   }
// void CAplicFrm::OnKvitAll()
// {
//      GetAppWnd()->m_msgAL.ALL();
// }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::HoneywellKBD(WORD nCode,LPARAM lParam)
//   {
//   //
//   CWnd* wnd = GetForegroundWindow();
//   //
//   switch ( lParam )
//     {
//     // Сообщения для Faceplate
//     case hw_MAN      :
//     case hw_AUTO     :
//     case hw_NORM     :
//     case hw_SP       :
//     case hw_OUT      :
//     if ( nCode == HW_KEYDOWN )
//       {
//       HWND hWnd = GetActiveMnemo();
//       if ( ::IsWindow(hWnd) )
//       if ( ::IsWindowVisible(hWnd) )
//         {
//         CViewMnemo* mnemo = (CViewMnemo*)CWnd::FromHandle(hWnd);
//         mnemo->Keyboard(nCode,(UINT)lParam);
//         }
//       }
//       return;
//     case hw_STEP_UP  :
//     case hw_STEP_DOWN:
//     case hw_MOVE_UP  :
//     case hw_MOVE_DOWN:
//       {
//       HWND hWnd = GetActiveMnemo();
//       if ( ::IsWindow(hWnd) )
//       if ( ::IsWindowVisible(hWnd) )
//         {
//         CViewMnemo* mnemo = (CViewMnemo*)CWnd::FromHandle(hWnd);
//         mnemo->Keyboard(nCode,(UINT)lParam);
//         }
//       }
//       return;
//     }
//   //
//   if ( nCode == HW_KEYDOWN )
//   switch ( lParam )
//     {
//     case hw_GOTO            :theApp.Command("Request Page");break;
//     case hw_PRINT_DISP      :break;
//     case hw_HELP            :break;
//     case hw_ASSOC_DISP      :break;
//     case hw_GROUP           :theApp.Command("Request Group");break;
//     case hw_TREND           :theApp.Command("Request Trend");break;
//     case hw_HOUR_AVG        :break;
//     case hw_DETAL           :if (!CallTrendDetail(barDETAIL))
// 								theApp.Command("Request Point Detail");break;
//     case hw_PRINT_TREND     :break;
//     case hw_CANCEL_PRINT    :break;
//     case hw_RECORD          :break;
//     case hw_SYS_STATS       :break;
//     case hw_PROC_NETWK_STATS:break;
//     case hw_ORG_SUMM        :break;
//     case hw_CONS_STATS      :break;
//     case hw_COMM_NETWK_STATS:break;
//     // При нажатии на UNIT ASGN окно NativeWindow всегда активируется
//     case hw_UNIT_ASGN       :/*m_frmNative.OnUnitAsgn();*/break;
//     // При нажатии на SYST MENU окно NativeWindow всегда активируется
//     case hw_SYST_MENU       :/*m_frmNative.OnSysMenu();*/break;
//     case hw_AM_STATS        :break;
//     case hw_UNIT_TREND      :theApp.Command("Server Pushbutton");break;
//     case hw_UNIT_SUMM       :break;
//     case hw_ALM_ANNC        :break;
//     case hw_MSG_SUMM        :theApp.Command("Alarm Message Summary"); break;
//     case hw_MSG_CONF        :break;
//     case hw_MSG_CLEAR       :break;
//     case hw_LOAD            :theApp.Command("[Navigation] Refresh");break;//?????
//     case hw_ALM_SUMM        :theApp.Command("Alarms"              );break;
//     case hw_PAGE_FWD        :theApp.Command("Page Down"           );break;
//     case hw_PAGE_BACK       :theApp.Command("Page Up"             );break;
//     case hw_DISP_BACK       :theApp.Command("[Navigation] Back"   );break;
//     case hw_DISP_FWD        :theApp.Command("[Navigation] Forward");break;
//     case hw_DISP_PRIOR      :theApp.Command("OKB PRIOR DISP"      );break;//?????
//     //
//     case hw_POLE     :GotoPole(2);break;
//     case hw_OPER     :GotoOper(2);break;
//     //
//     case hw_WINDOW   :
//     case hw_REGION   :
//     case hw_CLOSE    :
//       m_wndOper.HoneywellKBD(nCode,lParam);
//       m_wndPole.HoneywellKBD(nCode,lParam);
//       break;
//     //
//     case hw_FAST       : ChangeFast(); break;
//     case hw_ALPHASHIFT : ChangeAlphaShift(); break;
//     case hw_SCHEM      : GotoScheme(); break;
//     // Квитирование и снятие звуковой сигнализации
//     case hw_ACK      : AckMnemo      (); break;
//     case hw_SIL      : mPipe.AlarmSil(id_Rsu_HoneyWell5xx); break;
//     };
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::AckMnemo()
//   {
//   if ( m_bNoPCYWork ) return;
//   HWND hWnd = GetActiveMnemo();
//   if ( ::IsWindow(hWnd) )
//   if ( ::IsWindowVisible(hWnd) )
//     {
//     CViewMnemo* mnemo = (CViewMnemo*)CWnd::FromHandle(hWnd);
//     mnemo->Keyboard(HW_KEYDOWN,hw_ACK);
//     }
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::ChangeFast()
//   {
//   if ( m_nTimer )
//     KillTimer( m_nTimer );
//   //
//   bool fff = g_bFastTime;
//   g_bFastTime = !g_bFastTime;
//   m_nTimer = SetTimer( ID_MAIN_TIMER, g_bFastTime ? 500 : 1000, NULL );
//   //
//   HWND hWnd;
//   if ( IsKeyCOM1( hWnd ) )
//     ::MessageCOM1( hWnd, g_bFastTime ? 0x2fb : 0x1fb );
//   }

// void CAplicFrm::ChangeAlphaShift()
// {
// 	g_bAlphaShift = !g_bAlphaShift;
// }

// void CAplicFrm::GotoScheme()
//   {
//   _static bool ya = false;
//   if ( ya ) return;
//   ya = true;
//   /*
//   Sergej
//   Надо определиться для кого переход
//   //
//   CSchemDlg dlg(this);
//   dlg.m_strName = m_frameL.CurrMnemo();
//   if ( dlg.DoModal() == IDOK )
//   m_frameL.OpenMnemo(dlg.m_strName,true);
//   */
//   ya = false;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAplicFrm::PoleOper(bool bOper)
//   {
//   //if ( bOper )
//   //  {
//   //  m_wndOper.ShowOrHide(SW_SHOW);
//   //  m_wndPole.ShowOrHide(SW_HIDE);
//   //  }
//   //else
//   //  {
//   //  m_wndPole.ShowOrHide(SW_SHOW);
//   //  m_wndOper.ShowOrHide(SW_HIDE);
//   //  }
//   }

// bool CAplicFrm::IsShowSxeme()
//   {
//   return m_wndOne.IsShowSxeme() || m_wndDubl->IsShowSxeme() || (theApp.m_bFourMonitors && (m_wndThird.IsShowSxeme() || m_wndFourth.IsShowSxeme()));
//   }

// void CAplicFrm::GotoPole(int nCorrelate)
// {
// 	if (Version!=LG35_8_KF)
//     {
// 		m_wndDubl->GotoPole(nCorrelate);
//         if(theApp.m_bFourMonitors)
//           {
//           m_wndThird.GotoPole(nCorrelate);
//           m_wndFourth.GotoPole(nCorrelate);
//           }
//     }
// 	m_wndOne .GotoPole(nCorrelate);
// 	m_wndPole.ShowOrHide( SW_SHOW);
// 	m_wndOper.ShowOrHide( SW_HIDE);
// 	if (m_wndSens)
// 		m_wndSens.ShowOrHide(SW_HIDE);
// 	if ( m_wndOne.IsShowSxeme() )
// 	{
// 		if ( m_pFaceplate->GetSafeHwnd() )
// 		{
// 			if ( ::IsWindow(m_pFaceplate->m_hWnd) )
// 				m_pFaceplate ->ShowWindow(SW_HIDE);
// 		}
// 		for ( int n = 0; n < _MAX_FACE; n++ )
// 		{
// 			if ( m_arrFace[n] != NULL )
// 			{
// 				if ( ::IsWindow(m_arrFace[n]->m_hWnd) )
// 					m_arrFace[n] ->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}

//     ShowMnemos(SW_HIDE);
// }

// void CAplicFrm::GotoOper(int nCorrelate)
// {
// 	m_wndOne .GotoOper(nCorrelate);
// 	m_wndDubl->GotoOper(nCorrelate);
//     if(theApp.m_bFourMonitors)
//       {
//       m_wndThird.GotoOper(nCorrelate);
//       m_wndFourth.GotoOper(nCorrelate);
//       }
// 	m_wndOper.ShowOrHide( SW_SHOW);
// 	m_wndPole.ShowOrHide( SW_HIDE);
// 	if (m_wndSens)
// 		m_wndSens.ShowOrHide(SW_SHOW);
// 	if ( m_pFaceplate->GetSafeHwnd() )
// 	{
// 		if ( ::IsWindow(m_pFaceplate->m_hWnd) )
// 			m_pFaceplate ->ShowWindow(SW_SHOW);
// 	}
// 	for ( int n = 0; n < _MAX_FACE; n++ )
// 	{
// 		if ( m_arrFace[n] != NULL )
// 		{
// 			if ( ::IsWindow(m_arrFace[n]->m_hWnd) )
// 				m_arrFace[n] ->ShowWindow(SW_SHOW);
// 		}
// 	}
//     ShowMnemos(SW_SHOW);
// }

// void CAplicFrm::SwitchWnd(int nCorrelate)
//   {
//   if ( IsShowSxeme() )
//     {// Показываем мнемосхемы, прячем технологическую схему
//     GotoOper(nCorrelate);
//     }
//   else
//     {// Показываем технологическую схему, прячем мнемосхемы
//     GotoPole(nCorrelate);
//     }
//   }
// void CAplicFrm::OnSwitchStd() {SwitchWnd(0);}
// void CAplicFrm::OnSwitchFrm() {SwitchWnd(1);}
// void CAplicFrm::OnSwitchDab() {SwitchWnd(2);}
// //
// void CAplicFrm::SetInfoToChoiseBar(CString strName,CString strValue)
//   {
//   CString strNameARM(theApp.GetHoneywellInfoName().c_str());
//   CString strInfo;
//   strInfo=strNameARM+":"+strName+".PV="+strValue;
//   m_wndChoiseBar.ShowBar(barINFO,strInfo);
//   }
// //
// void CAplicFrm::SetInfoToDownBar(CString strInfo,int nNumber)
//   {
//   m_wndDownBar.SetLastAlarm(strInfo,nNumber);
//   }

// void CAplicFrm::OnMenuR300(UINT nCmdID)
//   {
//   theApp.Command(nCmdID);
//   }

// void CAplicFrm::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT item)
//   {
//   theApp.MeasureItem( item );
//   }

// void CAplicFrm::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT item)
//   {
//   theApp.DrawItem( item );
//   }
// //
// bool CAplicFrm::CallGroup()
//   {
//   CString strGroup;
//   CString strName;
//   if (m_pFaceplate!=NULL)
//     {
//     strName=m_pFaceplate->GetFaceName();
//     if (strName!="")
//       {
//       int nGroup=::FindGroupNumber(strName);
//       if (nGroup!=-1)
//         {
//         strGroup.Format("%d",nGroup);
//         ButtonOk( barGROUP, strGroup );//вызов Group для фэйсплейта
//         return true;
//         }
//       }
//     }
//   // Sergej 07.04
//   //if (ActiveFrm()->GetMnemo()->GetGroupDetail()!=-1)
//   //  {
//   //  strGroup.Format("%d",ActiveFrm()->GetMnemo()->GetGroupDetail());
//   //  ButtonOk( BAR_GROUP, strGroup );//вызов Group для окна Detail
//   //  return true;
//   //  }
//   return false;
//   }

// bool CAplicFrm::CallTrendDetail(BARS bar)
//   {
//   CString strName;
//   if (m_pFaceplate!=NULL)
//     {
//       ASSERT(0);
//   //  Sergej 07.04
//   //  strName=m_pFaceplate->GetFaceName();
//   //  if (strName!="")
//   //    {
//   //    int N = -1;//::FindHoneywell( strName );
//   //    if ( N != -1 )
//   //      {
//   //      if (bar==BAR_TREND)
//   //      GiveAmm(NULL)->GotoPage("sysdtltps-tabtrend",strName);//вызов Trend для фэйсплейта
//   //      if (bar==BAR_DETAIL)
//   //      GiveAmm(NULL)->GotoPage("sysdtltps-tabgeneral",strName);//вызов Detail для фэйсплейта
//   //      return true;
//   //      }
//   //    }
//     }
//   return false;
//   }

// void CAplicFrm::ToolbarR( const char* pszComm )
//   {
//   if ( pszComm == NULL ) return;
//   CWnd* pWnd = GetForegroundWindow();
//   if ( pWnd == NULL ) return;
//   int n = 0;
//   for ( ; n < _MAX_FACE; n++ )
//   if  ( m_arrFace[n] )
//   if  ( m_arrFace[n]->m_hWnd == pWnd->m_hWnd )
//     break;
//   if ( n == _MAX_FACE ) return;
//   CHTML& doc = m_arrFace[n]->Doc();
//   LPCTSTR pszTag = doc.Group();
//   //
//   if ( lstrcmpi( pszComm, "Request Trend" ) == 0 )
//     Trend(pszTag);
//   else
//   if ( lstrcmpi( pszComm, "Request Group" ) == 0 )
//     Group(pszTag);
//   else
//   if ( lstrcmpi( pszComm, "Request Point Detail" ) == 0 )
//     Detail(pszTag);
//   //void ButtonOk( barPAGE, const char* pszText );
//   }

// void CAplicFrm::Group(LPCTSTR pszTag)
//   {
//   CharMP szTag;
//   lstrcpy( szTag, pszTag );
//   char* p = strchr( szTag, '.' );
//   if ( p ) *p = 0;
//   int n = FindGroupNumber( szTag );
//   if ( n >= 0 )
//     {
//     char szGroup[32];
//     sprintf_s( szGroup, sizeof(szGroup), "%d", n );
//     ButtonOk( barGROUP, szGroup );
//     }
//   }

// void CAplicFrm::Trend(LPCTSTR pszTag)
//   {
//   CharMP szTag;
//   lstrcpy( szTag, pszTag );
//   char* p = strchr( szTag, '.' );
//   if ( p ) *p = 0;
//   int n = FindTrendNumber( szTag );
//   if ( n >= 0 )
//     {
//     char szGroup[32];
//     sprintf_s( szGroup, sizeof(szGroup), "%d", n );
//     ButtonOk( barTREND, szGroup );
//     }
//   }

// void CAplicFrm::Detail(LPCTSTR pszTag)
//   {
//   CharMP szTag;
//   lstrcpy( szTag, pszTag );
//   char* p = strchr( szTag, '.' );
//   if ( p ) *p = 0;
//   ButtonOk( barDETAIL, szTag );
//   }

// class Associated
// {
// private:
// 	std::map<std::string, std::string> _assoc_map;
// 	bool mbReady;
// public:
// 	Associated(): _assoc_map(), mbReady(true)
// 	{
// 		char fname[255];
// 		sprintf_s(fname, "%sassoc_disp.csv", ::GetAcyPath());
// 		HFILE file = _lopen (fname, OF_READ);
// 		if ((HFILE)INVALID_HANDLE_VALUE == file)
// 		{
// 			this->mbReady = false;
// 			return;
// 		}
// 		LONG size = _llseek(file, 0, 2); _llseek(file, 0, 0);
// 		char * content = new char[size+1];
// 		_hread(file, content, size);
// 		_lclose(file);
// 		char * _content = content;
// 		content[size] = 0;

// 		while(true)
// 		{
// 			char * tag = _content;
// 			if (!*tag) break;
// 			while(*_content != '\r' && *_content != '\n') _content++;
// 			*_content = 0;
// 			_content++;
// 			while(*_content == '\r' || *_content == '\n') _content++;

// 			char * schema = strchr(tag, ';');
// 			*schema = 0;
// 			schema++;
// 			while(*schema != ',' && *schema != 0) schema++;
// 			*schema = 0;
// 			schema = tag + strlen(tag) + 1;
// 			for (int i = strlen(tag)-1; i>0; i--)
// 			{
// 				if (tag[i] == '.')
// 				{
// 					tag[i] = 0;
// 					break;
// 				}
// 			}
// 			if (_assoc_map.count(tag) != 1)
// 			{
// 				_assoc_map.insert(std::pair<std::string, std::string>(tag, schema));
// 			}
// 		}

// 		delete[] content;
// 	}
// 	bool IsReady() { return this->mbReady; }
// 	const char * GetSchema(const char * tag)
// 	{
// 		if (_assoc_map.count(tag) == 1)
// 		{
// 			return _assoc_map[tag].c_str();
// 		}
// 		return 0;
// 	}
// 	const char * GetSchemaForShortName(const char * tag)  //для нахождения по части имени
// 	{
//         std::map<std::string, std::string>::iterator it;
//         for (it = _assoc_map.begin(); it!=_assoc_map.end(); it++)
//           {
//               if(_strnicmp(it->first.c_str(), tag, strlen(tag)) == 0)
//                  return it->second.c_str();
//           }
// 		return 0;
// 	}
// };
 
// void CAplicFrm::Assocs(LPCTSTR pszTag)
//   {
//   static Associated associated;
//   if (associated.IsReady())
//   {
// 	  const char * schema = associated.GetSchema(pszTag);
// 	  if(!schema)
//         schema = associated.GetSchemaForShortName(pszTag);
// 	  if (schema)
// 	  {
// 		if (theApp.IsAllInOne() && !theApp.DisableKirishiView())
// 			GiveRegularAmm(NULL)->GoToPage(schema,NULL);
// 		else
// 			GiveAmm(this->m_hWnd)->GoToPage(schema, NULL);
// 	  }
//   }
//   else
//     {// имя ассоциированного дисплея лежит в CONTROLMODULE
//     CShCONTROLMODULE sh;
//     char *pp = NULL;
//     if(pp = (char*)strchr(pszTag, '.'))
//       *pp = 0;
//     if(GiveAmm(m_hWnd)->m_wndMnemo.mDoc.pipe.GetData( id_CONTROLMODULE, pszTag, &sh ))
//      {
//      LPCSTR lpAssocDsp = sh.SCANASSOCDSP;
//      if(lpAssocDsp[0])
//         GiveAmm(m_hWnd)->GoToPage(lpAssocDsp, NULL);
//      }
//     if(pp)
//       *pp = '.';
//     }
//   }

// void CAplicFrm::SetPipeAndNumberForInfoBar(KHoneyPipeClient* ptr,int nNumber)
// {
// 	m_ptr_pipeForInfoBar=ptr;
// 	m_nNumberForInfoBar=nNumber;
// 	m_bMustClearPipeAndNumberForKillFocus=false;
// }

// bool CAplicFrm::IsGoodPointFromInfo()
// {
// 	return ((m_nNumberForInfoBar!=-1)&&(m_ptr_pipeForInfoBar!=NULL));
// }

// bool CAplicFrm::CallTrendGroupFromPoint()
// {
// 	if (IsGoodPointFromInfo())
// 	{
// 		std::string nameKey(m_ptr_pipeForInfoBar->GetMnemoName(m_nNumberForInfoBar));
//         std::transform(nameKey.begin(), nameKey.end(),nameKey.begin(), ::toupper);
// 		if (mapPointInTrendsGroup.empty())
// 		    mapPointInTrendsGroup=CViewTrend4::Data().FillMap();
// 		int nGroup=-1;
// 		std::map<std::string,int>:: iterator it=mapPointInTrendsGroup.find(nameKey);
// 		if (it!=mapPointInTrendsGroup.end())
// 			nGroup=it->second;
// 		if (nGroup!=-1)
// 		{
// 			char szGroup[32];
// 			sprintf_s( szGroup, sizeof(szGroup), "%d", nGroup );
// 			ButtonOk( barTREND, szGroup );
// 			return true;
// 		}
// 	}
// 	return false;
// }

// std::string CAplicFrm::GetHoneywellStartNameCSVbyCAmmFrm(CAmmFrm *pParentAmmFrm)
// {
//     	if(m_wndOne == *pParentAmmFrm)
//            return(theApp.GetTexnoValueByKey("Honeywell_Start_1"));
//         if(*m_wndDubl == *pParentAmmFrm)
//            return(theApp.GetTexnoValueByKey("Honeywell_Start_2"));
// 		if(m_wndThird == *pParentAmmFrm)
//            return(theApp.GetTexnoValueByKey("Honeywell_Start_3"));
// 		if(m_wndFourth == *pParentAmmFrm)
//            return(theApp.GetTexnoValueByKey("Honeywell_Start_4"));
// 		if(m_wndTop == *pParentAmmFrm)
//            return(theApp.GetTexnoValueByKey("Honeywell_Start_Top"));
//         return "";
// }

// BYTE CAplicFrm::GetLampState(BYTE nArrayNumber)
// {
//     BYTE lampState =0;
//     if(m_pMnemoStates && (nArrayNumber < _countof(m_pMnemoStates->m_ManemoData)))
//       {
//           lampState = (BYTE)m_pMnemoStates->m_ManemoData[nArrayNumber].mnemoState;
//       }
// 	return lampState;
// }
