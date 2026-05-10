// // APM.cpp : Defines the class behaviors for the application.
// //
#include "stdafx.h"
// #define __MAIN_FILE__
 #include "ARM.h"
 #include "FormatScn.h"
#include <wingdi.h>
// #include "AboutDlg.h"
// #include "AplicFrm.h"
// //
// // Глобальные переменные
 bool g_bGlobalEditPV = false;// Возможность редактирования PV
// //
 unsigned long dwMainTimer = 0;
// unsigned long dwTypeFrame = 0;
// const  double mWindSize = 0.8;
// //-------------------------------------------------------------------
// bool FindTheHID();
// bool Registration();
// //
// // BEGIN_MESSAGE_MAP(CAPMApp, CWinApp)
// // 	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
// //   ON_COMMAND(ID_TUNING, OnTuning)
// //   ON_UPDATE_COMMAND_UI(ID_TUNING, &CAPMApp::OnUpdateTuning)
// // END_MESSAGE_MAP()
// // //
 CAPMApp  theApp;

// #define NAME_CLASS_ARM_HWL  _T("АРМ Оператора Honeywell")
// //
// bool CAPMApp::m_bPole=false;
CAPMApp::CAPMApp()
: m_scnXML(NULL), m_nCountXML(0)
, m_scnEXT(NULL), m_nCountEXT(0)
, m_scnSCN(NULL), m_nCountSCN(0)
, m_nLevelSec(2)
// , m_hIcon(NULL)
// , m_hAccel(NULL)
// , m_hMenu(NULL)
, m_bDND(false)
// , m_bOnceAlarmIconMessageBox(false)
// , mSocketTexno("ТехноСхема")
// , sharedMemoryForPanoram(NULL)
, m_bNewPanoram(false)
//, m_bFourMonitors(false)
, udplib (NULL)
, fnStartUdpServer (NULL)
, fnSendUdpName (NULL)
, fnStopUdpServer (NULL)
, m_nFaceplatsPerScreen(2)
, m_bOlderSensorKbd(false)
{
  *m_szTempMessZoneText = 0;
 // memset( &m_cmd, 0, sizeof(m_cmd) );
  //MIHAIL_HWL
  //m_cmd.bFaceToolbar=true;//ключ -t убирает в false
  //m_cmd.nKlawaHoney=1;
  memset( bmpAlarm, 0, sizeof(bmpAlarm) );

  if( Version == AM2_NAK)
    m_nLevelSec = 3;
   
  if(Version == BERTH_ONPZ)
  {
    std::string file(::GetBasePath());
    file += "Abstract\\ArgosyDictionary.xml";
    if(FileExists(file))
    {
      m_DictionaryXML.LoadEx(file.c_str());
    }
  }

  m_bNewPanoram=(Version>=DOTF_PM);
  if (IsNewPanoram())
  {
      //blablabla
       //    sharedMemoryForPanoram=new ARMSharedMemory();
          // if (Version==L35_11600_KF)
          // {
                // udplib = LoadLibrary("UDPServer.dll");
                // if (udplib != NULL)
                // {
                // 	fnStartUdpServer = (StartUdpServer)GetProcAddress(udplib, "StartServer");
                // 	fnSendUdpName = (SendUdpName)GetProcAddress(udplib, "Send");
                // 	fnStopUdpServer = (StopUdpServer)GetProcAddress(udplib, "StopServer");
                // 	if (fnStartUdpServer != NULL)
                // 		fnStartUdpServer();
                // }
          // }
  }
}

// void CAPMApp::SetPanoramNameInSharedMemory(std::string write)
// {
//  //    if (sharedMemoryForPanoram!=NULL)
//     // 	sharedMemoryForPanoram->Write(write);
//     // if (Version==L35_11600_KF)
//     // {
//     // 		if (udplib != NULL)
//     // 		{
//     // 			if (fnSendUdpName!=NULL)
//     // 			{
//     // 				fnSendUdpName(write.c_str());
//     // 			}
//     // 		}
//     // }
// }

// void CAPMApp::StartNewPanoram3D()
// {
// 		cross::CString panoram3d;
//         // panoram3d.Format( "%sPanorams3D\\Panorams.exe", ::GetExePath() );
//         // panoram3d.MakeUpper();
//         // panoram3d.Replace("\\SYSTEM\\EXE\\","\\SYSTEM\\EXE_64\\");
// 		cross::CString strFolder="..\\EXE_64\\Panorams3D";
// 		std::string strExePanoramPath=GetExePanoramPath();
// 		if (strExePanoramPath!="")
// 		{
// 			panoram3d=cross::CString(strExePanoramPath.c_str())+"Panorams.exe";
// 			strFolder=".\\Panorams3D";
// 		}
// 		cross::CString strCmd=GetServerParamsForPanoram();
// 		::ShellExecute( NULL, "Open", panoram3d, strCmd, strFolder, SW_SHOWNORMAL );
// 		m_bPanoramaInTop=true;
// }

CAPMApp::~CAPMApp()
  {
          if (Version==L35_11600_KF)
          {
                if (udplib != NULL)
                {
                    if (fnStopUdpServer!=NULL)
                        fnStopUdpServer();
                    FreeLibrary(udplib);
                }
          }
          //some delete
         //  for ( INT_PTR n = 0; n < GetCount(); n++ )
            // {
            // SFontArray& s = GetAt(n);
            // ::DeleteObject( s.font );
            // }
          ::ParserFree(m_scnXML);
          ::ParserFree(m_scnEXT);
          ::ParserFree(m_scnSCN);
          for ( int n = 0; n < MAX_IMAGES_NUMBER; n++ )
            if ( bmpAlarm[n] ) ::DeleteObject( bmpAlarm[n] );
  }

STegScn* CAPMApp::ParsXML( char*& ptr, int& nCount )
  {
  nCount = ::ParserXML( ptr, m_scnXML, m_nCountXML );
  return m_scnXML;
  }

// STegScn* CAPMApp::ParsEXT( char*& ptr, int& nCount )
//   {
//   nCount = ::ParserEXT( ptr, m_scnEXT, m_nCountEXT );
//   return m_scnEXT;
//   }

// STegScn* CAPMApp::ParsSCN( char*& ptr, int& nCount )
//   {
//   nCount = ::ParserSCN( ptr, m_scnSCN, m_nCountSCN );
//   return m_scnSCN;
//   }

// HFONT CAPMApp::GiveFont(LOGFONT& lf)
//   {
//   if ( lf.lfCharSet == 0 )
//     lf.lfCharSet = DEFAULT_CHARSET;
//   ////
//   /// only win functions, may be will be troubles
//   // for ( INT_PTR n = 0; n < GetCount(); n++ )
//   //   {
//   //   SFontArray& s = GetAt(n);
//   //   if ( memcmp( &s.lf, &lf, sizeof(LOGFONT) ) == 0 )
//   //     return s.font;
//   //   }
//   // SFontArray s;
//   // s.lf = lf;
//   // s.font = ::CreateFontIndicross::RECT( &lf );
//   // Add( s );
//   // return s.font;
//   }

// LOGFONT& CAPMApp::GiveLogF(HFONT hFont)
//   {
//   // for ( INT_PTR n = 0; n < GetCount(); n++ )
//   //   {
//   //   SFontArray& s = GetAt(n);
//   //   if ( s.font == hFont )
//   //     return s.lf;
//   //   }
//   // _static LOGFONT lf;
//   // return lf;
//   }

// // void CAPMApp::SetIcon( CWnd* pWnd )
// //   {
// //   if ( m_hIcon == NULL )
// //   m_hIcon = LoadIcon(IDR_MAINFRAME);
// //   pWnd->SetIcon(m_hIcon, TRUE );// Set big icon
// //   pWnd->SetIcon(m_hIcon, FALSE);// Set small icon
// //   }

bool CAPMApp::IsAllInOne()
{
    return Version == LG35_8_KF || Version == L35_11600_KF || Version == VSB_KF;
}
    
// bool CAPMApp::bUseWDL()
// {
//     if(!DisableKirishiView())
//       return Version == VSB_KF;
//     return false;
// }


// HWND Pults( HWND hParent, HWND hPostM, const char* pszPult, bool bPopup );
// void AnalizTAG();// Анализ
// void AnalizGUS();// EB-файлов
// void AnalizTXT();
// BOOL CAPMApp::InitInstance()
//   {
//   // fnReadInt  = &ReadInt;
//   // fnSaveInt  = &SaveInt;
//   // fnLoadPult = &Pults;
//   // CPipeClient::ms_bCopyNetworkFile = false;
//   // //
//   // int ss = sizeof(CEventEntry);
//   //FindTheHID();
//   //AnalizTXT();
//   if (Version!=RCOL_SI)
//   {
// 	  char* Q = setlocale( LC_ALL, "Russian" );
// 	  char *pdecpoint = localeconv()->decimal_point;

// 	  if( pdecpoint && *pdecpoint!='.' )
// 	  {
// 		setlocale( LC_NUMERIC, "C");
// 		if( pdecpoint && *pdecpoint!='.' )
// 		{
// 		  pdecpoint = localeconv()->decimal_point;
// //		  MessageBox( NULL, _T("Установите в системе десятичный разделитель в '.'"), _T("Дальнейшая работа не возможна"), MB_OK|MB_ICONERROR );
// 		  return FALSE;
// 		}
// 	  }
//   }
//   // if ( !ParserCmdLine( m_lpCmdLine, m_cmd ) )
//   //   return FALSE;

//   if (Version >= DOTF_PM)
//   {
//      if(!IsAllInOne() || DisableKirishiView())
//      if(!bUseWDL())
//        m_bFourMonitors = true;
//   }

//   SetupIniFile();
//   // texnoCSV.FillMapFromFile();
//   // mSocketTexno.InitAndSendKey();
//   //MIHAIL_HWL
//   dwTypeFrame = 1; //m_cmd.nTypeWindow;
//   if(Version == AM2_NAK || Version == DMW_NAK || Version == ITH_NAK)
//      m_nFaceplatsPerScreen = 4;
//   //
//   HWND hWnd = ::FindWindow(NAME_CLASS_ARM_HWL,NULL);
//   if(::IsWindow(hWnd))
//     {
//     ::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
//     ::SetForegroundWindow( hWnd );
//     return FALSE;
//     }
//   //
//   if ( IsThisModuleLoaded(NAME_CLASS_ARM_HWL) )
//     return FALSE;
//   //
//   // Выключаем все лампочки, кроме ALPHA SHIFT
//   if ( IsKeyCOM1( hWnd ) )
//     ::MessageCOM1( hWnd, 0x11ff );
//   //
//   // InitCommonControls() is required on Windows XP if an application
// 	// manifest specifies use of ComCtl32.dll version 6 or later to enable
// 	// visual styles.  Otherwise, any window creation will fail.
// 	//InitCommonControls();
//   INITCOMMONCONTROLSEX init = { sizeof(INITCOMMONCONTROLSEX), 0x000000FF };
//   InitCommonControlsEx(&init);

//   if(bUseWDL())
//     LoadWDL();

//   CWinApp::InitInstance();
// 	// Initialize OLE libraries
// 	if (!AfxOleInit())
// 	{
// 		AfxMessageBox(IDP_OLE_INIT_FAILED);
// 		return FALSE;
// 	}
// 	AfxEnableControlContainer();

// 	GdiplusStartupInput gdiplusStartupInput;
// 	ULONG_PTR gdiplusToken;
// 	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//   // Standard initialization
// 	// If you are not using these features and wish to reduce the size
// 	// of your final executable, you should remove from the following
// 	// the specific initialization routines you do not need
// 	// Change the registry key under which our settings are stored
// 	// TODO: You should modify this string to be something appropriate
// 	// such as the name of your company or organization
//   Monitors();
//   std::string strMenuSheme=theApp.GetHoneywellMenuNameCSV();
//   if (strMenuSheme=="")
// 		strMenuSheme="MENU";
//   bool bIsMenuSTBFile=LoadFile( strMenuSheme.c_str() );
//   if (!bIsMenuSTBFile)
//   {
// 	std::string strMessage="Файла меню "+strMenuSheme+".stb нет в папке R410 АРМ будет закрыт";
// 	::MessageBox( NULL, strMessage.c_str(), "Ошибка", MB_ICONSTOP );
// 	return FALSE;
//   }
//   //
//   m_hAccel = Tebu();
//   m_hMenu  = Menu();
//   //
//   CAplicFrm* pFrame = new CAplicFrm();
// 	if (!pFrame)
// 		return FALSE;
// 	m_pMainWnd = pFrame;
//   // create and load the frame with its resources
// 	pFrame->LoadFrame(IDR_MAINFRAME, WS_SYSMENU| WS_OVERLAPPED | WS_CAPTION|WS_CLIPCHILDREN |FWS_ADDTOTITLE, NULL, NULL);
//   if ( m_pMainWnd == NULL )
//     {
//     ::MessageBox( NULL, "Запуск программы невозможен!", "Ошибка", MB_ICONSTOP );
//     return FALSE;
//     }
// 	// The one and only window has been initialized, so show and update it
// 	pFrame->ShowWindow(SW_SHOW/*MAXIMIZED*/);
// 	pFrame->UpdateWindow();
// 	pFrame->SetFocus();
//   //
//   if ( m_pszHelpFilePath ) delete [] m_pszHelpFilePath;
//   char* szHelp = new char[_MAX_PATH];
//   sprintf_s( szHelp, _MAX_PATH, "%sHelp\\Help18.chm", GetBasePath() );
//   m_pszHelpFilePath = szHelp;
//   m_eHelpType = afxHTMLHelp;
//   //
//   // Регистрация только на рабочих станциях
//   if ( strcmp( CPipeClient::ms_szServer, "." ) )
//   if ( !theApp.Instructor()  )
//   if ( !Registration() )
//     return FALSE;
//   else
//   {
// 	  if (Version == LG35_8_KF)
// 	  {
// 		    if(!pFrame->mSocket.IsConnected())
// 				pFrame->mSocket.InitAndSendKey();
// 			pFrame->mSocket.SendOperator();
// 	  }
// 	  else
// 			pFrame->mSocket.SendOperatorToAllSocket(); //посылка имени оператора на сервер
//   }
//   pFrame->SetUpdate(true);

//   xml.ReadAllPointsInXMLFilesAndSaveInArrow(strPointsFromXMLArray,m_nPointsFromXMLArrayCount,GetSpecialPanoramsDir());
//   m_bPanoramaInTop=false;
//   //
//   return TRUE;
//   }

// int CAPMApp::ExitInstance()
//   {
//   // Выключаем все лампочки
//   HWND hWnd;
//   if ( IsKeyCOM1( hWnd ) )
//     ::MessageCOM1( hWnd, 0x1ff );
//   //
//   if ( m_hAccel )
//     ::DestroyAcceleratorTable( m_hAccel );
//   if ( m_hMenu )
//     ::DestroyMenu( m_hMenu );
//   if (sharedMemoryForPanoram!=NULL)
// 	delete sharedMemoryForPanoram;
//   return CWinApp::ExitInstance();
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// const char* Empty()
//   {
//   _static char* empty = "";
//   return empty;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// const char* ItemText( UINT id, HBITMAP& hBitmap, SIZE& sz, int& nFont )
//   {
//   _static SIZE size = { ::GetSystemMetrics(SM_CXMENUCHECK), ::GetSystemMetrics(SM_CYMENUCHECK) };
//   struct SLocal { const char* name; const char* image; UINT id; int fnt; HBITMAP bmp; };
//   _static SLocal list[] =
//     {
//     { "Acknowledge Alarm" , "#249.gif" , IDM_ALRM_ACK, SYSTEM_FONT },
//     { "Detail"            , "#259.gif" , IDM_DET_DIS , SYSTEM_FONT },
//     { "Associated Display", "#251.gif" , IDM_ASS_DIS , SYSTEM_FONT },
//     { "Trend"             , "tb010_s.bmp", IDM_TRN_DIS , SYSTEM_FONT },
//     { "MiniTrend"         , "tb010_s.bmp", IDM_MINITRN_DIS , SYSTEM_FONT },
//     { "Group"             , "tb011_s.bmp", IDM_GRP_DIS , SYSTEM_FONT },
//     { "MAN"               , NULL       , IDM_MAN     , SYSTEM_FONT },
//     { "AUTO"              , NULL       , IDM_AUTO    , SYSTEM_FONT },
//     { "NORM"              , NULL       , IDM_NORM    , SYSTEM_FONT },
//     { "Raise"             , "tb012_s.bmp", IDM_RAISE   , SYSTEM_FONT },
//     { "Lower"             , "tb013_s.bmp", IDM_LOWER   , SYSTEM_FONT },
//     { "Enable/Disable"    , "tb016_s.bmp", IDM_EDDIS   , SYSTEM_FONT },
//     //
//     { "Individual scales in EU"     , "ScaleEU.png", IDM_TND_IND_EU  , ANSI_VAR_FONT },
//     { "Individual scales in %"      , "ScalePR.png", IDM_TND_IND_PR  , ANSI_VAR_FONT },
//     { "Single scale for all plots"  , "ScaleXZ.png", IDM_TND_ALL_SN  , ANSI_VAR_FONT },
//     { "Auto scale current plot"     , "silver.png" , IDM_TND_AUTO_IND, ANSI_VAR_FONT },
//     { "Auto scale all plots"        , "silver.png" , IDM_TND_AUTO_ALL, ANSI_VAR_FONT },
//     { "Scale to point ranges in EU" , "silver.png" , IDM_TND_SCAL_EU , ANSI_VAR_FONT },
//     { "Scale to point ranges in %"  , "silver.png" , IDM_TND_SCAL_PR , ANSI_VAR_FONT },
//     { "Revert to saved ranges"      , "silver.png" , IDM_TND_REVERT  , ANSI_VAR_FONT },
//     { NULL }
//     };
//   int n = 0;
//   while ( list[n].id )
//     {
//     if ( list[n].id == id )
//       break;
//     n++;
//     }
//   if ( list[n].id == 0 )
//     return NULL;
//   sz = size;
//   nFont   = list[n].fnt;
//   hBitmap = list[n].bmp;
//   if ( hBitmap != NULL )
//     return list[n].name;
//   if ( list[n].image == NULL )
//     return list[n].name;
//   //Грузим картинку
//   HGLOBAL hGlobal = GlobalR300( list[n].image, true );
//   if ( hGlobal != NULL )
//     {
//     hBitmap = BitmapR300( hGlobal, size  );
//     ::GlobalFree( hGlobal );
//     sz = size;
//     }
//   //
//   list[n].bmp = hBitmap;
//   return list[n].name;
//   }
// //
// const char* ItemText_98( UINT id, HBITMAP& hBitmap, SIZE& sz, int& nFont )
//   {
//   _static SIZE size = { ::GetSystemMetrics(SM_CXMENUCHECK), ::GetSystemMetrics(SM_CYMENUCHECK) };
//   struct SLocal { const char* name; const char* image; UINT id; int fnt; HBITMAP bmp; };
//   _static SLocal list[] =
//     {
//     { "Acknowledge Alarm" , "tb002_s.bmp" , IDM_ALRM_ACK, SYSTEM_FONT },
//     { "Detail"            , "tb017_s.bmp" , IDM_DET_DIS , SYSTEM_FONT },
//     { "Associated Display", "tb003_s.bmp" , IDM_ASS_DIS , SYSTEM_FONT },
//     { "Trend"             , "tb010_s.bmp", IDM_TRN_DIS , SYSTEM_FONT },
//     { "Group"             , "tb011_s.bmp", IDM_GRP_DIS , SYSTEM_FONT },
//     { "MAN"               , NULL       , IDM_MAN     , SYSTEM_FONT },
//     { "AUTO"              , NULL       , IDM_AUTO    , SYSTEM_FONT },
//     { "NORM"              , NULL       , IDM_NORM    , SYSTEM_FONT },
//     { "Raise"             , "tb012_s.bmp", IDM_RAISE   , SYSTEM_FONT },
//     { "Lower"             , "tb013_s.bmp", IDM_LOWER   , SYSTEM_FONT },
//     { "Enable/Disable"    , "tb016.bmp", IDM_EDDIS   , SYSTEM_FONT },
//     //
//     { "Individual scales in EU"     , "ScaleEU.png", IDM_TND_IND_EU  , ANSI_VAR_FONT },
//     { "Individual scales in %"      , "ScalePR.png", IDM_TND_IND_PR  , ANSI_VAR_FONT },
//     { "Single scale for all plots"  , "ScaleXZ.png", IDM_TND_ALL_SN  , ANSI_VAR_FONT },
//     { "Auto scale current plot"     , "silver.png" , IDM_TND_AUTO_IND, ANSI_VAR_FONT },
//     { "Auto scale all plots"        , "silver.png" , IDM_TND_AUTO_ALL, ANSI_VAR_FONT },
//     { "Scale to point ranges in EU" , "silver.png" , IDM_TND_SCAL_EU , ANSI_VAR_FONT },
//     { "Scale to point ranges in %"  , "silver.png" , IDM_TND_SCAL_PR , ANSI_VAR_FONT },
//     { "Revert to saved ranges"      , "silver.png" , IDM_TND_REVERT  , ANSI_VAR_FONT },
//     { NULL }
//     };
//   int n = 0;
//   while ( list[n].id )
//     {
//     if ( list[n].id == id )
//       break;
//     n++;
//     }
//   if ( list[n].id == 0 )
//     return NULL;
//   sz = size;
//   nFont   = list[n].fnt;
//   hBitmap = list[n].bmp;
//   if ( hBitmap != NULL )
//     return list[n].name;
//   if ( list[n].image == NULL )
//     return list[n].name;
//   //Грузим картинку
//   HGLOBAL hGlobal = GlobalR300( list[n].image, true );
//   if ( hGlobal != NULL )
//     {
//     hBitmap = BitmapR300( hGlobal, size  );
//     ::GlobalFree( hGlobal );
//     sz = size;
//     }
//   //
//   list[n].bmp = hBitmap;
//   return list[n].name;
//   }
// //
// bool  FloatMenu( LPMEASUREITEMSTRUCT item, HWND hWnd )
//   {
// 	if ( item->CtlType != ODT_MENU )
//     return false;
//   SIZE sz; HBITMAP hBitmap; int nFont;
//   const char* text;
//   if (Version == AT_PRP) text = ItemText_98( item->itemID, hBitmap, sz, nFont );
//   else text = ItemText( item->itemID, hBitmap, sz, nFont );
//   if ( text == NULL )
//     return false;
//   //
//   HDC hDC = ::GetDC( hWnd );
//   cross::RECT cross::RECT = {0,0,0,0};
//   if ( hDC != NULL )
//     {
//     HFONT hFont = (HFONT)::GetStockObject(nFont);
//       {
//       CMyFont font( hDC, hFont, 0 );
//       ::DrawText( hDC, text, lstrlen(text), &cross::RECT, DT_SINGLELINE|DT_CALCcross::RECT|DT_EXPANDTABS);
//       }
//     ::ReleaseDC( hWnd, hDC );
//     }
//   item->itemHeight = max( sz.cy, cross::RECT.bottom-cross::RECT.top )+4;
//   item->itemWidth  = cross::RECT.right-cross::RECT.left + sz.cx;
//   if ( nFont == SYSTEM_FONT ) item->itemHeight += 4;
//   return true;
//   }
// //
// bool FloatMenu( LPDRAWITEMSTRUCT item )
//   {
// 	if ( item->CtlType != ODT_MENU )
//     return false;
//   SIZE sz; HBITMAP hBitmap; int nFont;
//   const char* text;
//   if (Version == AT_PRP) text = ItemText_98( item->itemID, hBitmap, sz, nFont );
//   else text = ItemText( item->itemID, hBitmap, sz, nFont );
//   if ( text == NULL )
//     return false;
//   //
//   COLORREF colorText;
//   COLORREF colorFon;
//   if ( item->itemState & ODS_SELECTED )
//     {
//     colorText = GetSysColor(COLOR_HIGHLIGHTTEXT);
//     colorFon  = GetSysColor(COLOR_HIGHLIGHT);
//     CMyBrush brush( item->hDC, colorFon );
//     brush.cross::RECT( item->rcItem );
//     }
//   else
//     {
//     colorText = GetSysColor(COLOR_MENUTEXT);
//     colorFon  = GetSysColor(COLOR_BTNFACE);//COLOR_MENU);
//     CMyBrush brush( item->hDC, colorFon );
//     brush.cross::RECT( item->rcItem );
//     //
//     if ( item->itemState & ODS_SELECTED )
//       ::DrawEdge( item->hDC, &item->rcItem, BDR_SUNKEN, BF_cross::RECT|BF_SOFT );
//     }
//   //
//   cross::RECT cross::RECT = item->rcItem;
//   cross::RECT.left += sz.cx+4;
//   int nMode = ::SetBkMode( item->hDC, TRANSPARENT );
//   //COLORREF color = ::SetTextColor(item->hDC,colorText);
//   {
//   char* tab = strchr( (char*)text, '\t' );
//   if ( tab != NULL ) *tab = 0;
//   HFONT hFont = (HFONT)::GetStockObject(nFont);
//   CMyFont font( item->hDC, hFont, colorText );
//   ::DrawText( item->hDC, text, lstrlen(text), &cross::RECT, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
//   if ( tab != NULL )
//     {
//     *tab = '\t';
//     cross::RECT.right -= 10;
//     ::DrawText( item->hDC, tab+1, lstrlen(tab+1), &cross::RECT, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
//     }
//   }
//   //  ::SetTextColor(item->hDC,color);
//   ::SetBkMode( item->hDC, nMode );
//   //
//   MENUITEMINFO inf;
//   memset( &inf, 0, sizeof(inf) );
//   inf.cbSize = sizeof(inf);
//   inf.fMask = MIIM_CHECKMARKS;
//   ::GetMenuItemInfo( (HMENU)item->hwndItem, item->itemID, FALSE, &inf );
//   if ( hBitmap == NULL )
//     return true;
//   //
//   HDC hdc = ::CreateCompatibleDC( item->hDC );
//   if ( hdc )
//     {
//     BITMAP bmp;
//     ::GetObject( hBitmap, sizeof(BITMAP), &bmp );
//     int x = item->rcItem.left + 2 + (sz.cx-bmp.bmWidth )/2;
//     int y = item->rcItem.top  + 4 + (sz.cy-bmp.bmHeight)/2;
//     //
//     HGDIOBJ oBitmap = ::SelectObject( hdc, hBitmap );
//     ::BitBlt( item->hDC, x, y, bmp.bmWidth, bmp.bmHeight,
//                     hdc, 0, 0, SRCCOPY );
//     ::SelectObject( hdc, oBitmap );
//     ::DeleteDC( hdc );
//     ::Setcross::RECT( &cross::RECT, x, y, x+bmp.bmWidth, y+bmp.bmHeight );
//     ::Transparentcross::RECT( item->hDC, cross::RECT, colorFon, 0xc0c0c0 );
//     ::Transparentcross::RECT( item->hDC, cross::RECT, colorFon, 0xc6c3c6 );// Надо разбираться
//     }
//   //
//   return true;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void ContextMenu( HWND hWnd, bool bAlarm )
//   {
//   POINT pt;
//   ::GetCursorPos( &pt );
//   HMENU hMenu = ::CreatePopupMenu();
//   if ( hMenu )
//     {
//     UINT uBE = MF_BYCOMMAND | MF_ENABLED | MF_OWNERDRAW;
//     ::AppendMenu(hMenu,uBE,IDM_ALRM_ACK,"Acknowledge Alarm" );
//     if ( !bAlarm )
//       {
//       ::AppendMenu(hMenu,MF_SEPARATOR,-1 , NULL             );
//       ::AppendMenu(hMenu,uBE,IDM_RAISE ,"Raise"           );
//       ::AppendMenu(hMenu,uBE,IDM_LOWER ,"Lower"           );
//       ::AppendMenu(hMenu,uBE,IDM_EDDIS ,"Enable/Disable"  );
//       }
//     ::AppendMenu(hMenu,MF_SEPARATOR,-1 , NULL               );
//     ::AppendMenu(hMenu,uBE,IDM_DET_DIS ,"Detail"            );
//     ::AppendMenu(hMenu,uBE,IDM_TRN_DIS ,"Trend"             );
//     ::AppendMenu(hMenu,uBE,IDM_MINITRN_DIS ,"MiniTrend"     );
//     ::AppendMenu(hMenu,uBE,IDM_GRP_DIS ,"Group"             );
//     ::AppendMenu(hMenu,uBE,IDM_ASS_DIS ,"Associated Display");
//     if ( !bAlarm )
//       {
//       ::AppendMenu(hMenu,MF_SEPARATOR,-1 , NULL );
//       ::AppendMenu(hMenu,uBE,IDM_MAN ,"MAN"  );
//       ::AppendMenu(hMenu,uBE,IDM_AUTO,"AUTO" );
//       ::AppendMenu(hMenu,uBE,IDM_NORM,"NORM" );
//       }
//     #define TPM (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON)
//     ::TrackPopupMenu(hMenu,TPM,pt.x,pt.y,0,hWnd,NULL);
//     DWORD dwError = ::GetLastError();
//     ::DestroyMenu(hMenu);
//     }
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void ScaleMenu( HWND hWnd, int x, int y )
//   {
//   HMENU hMenu = ::CreatePopupMenu();
//   if ( hMenu )
//     {
//     UINT uBE = MF_BYCOMMAND | MF_ENABLED | MF_OWNERDRAW;
//     ::AppendMenu(hMenu,uBE,IDM_TND_IND_EU  ,"Individual scales in EU"    );
//     ::AppendMenu(hMenu,uBE,IDM_TND_IND_PR  ,"Individual scales in %"     );
//     ::AppendMenu(hMenu,uBE,IDM_TND_ALL_SN  ,"Single scale for all plots" );
//     ::AppendMenu(hMenu,MF_SEPARATOR,0,0);
//     ::AppendMenu(hMenu,uBE,IDM_TND_AUTO_IND,"Auto scale current plot"    );
//     ::AppendMenu(hMenu,uBE,IDM_TND_AUTO_ALL,"Auto scale all plots"       );
//     ::AppendMenu(hMenu,uBE,IDM_TND_SCAL_EU ,"Scale to point ranges in EU");
//     ::AppendMenu(hMenu,uBE,IDM_TND_SCAL_PR ,"Scale to point ranges in %" );
//     ::AppendMenu(hMenu,uBE,IDM_TND_REVERT  ,"Revert to saved ranges"     );
//     #define TPM (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON)
//     ::TrackPopupMenu(hMenu,TPM,x,y,0,hWnd,NULL);
//     DWORD dwError = ::GetLastError();
//     ::DestroyMenu(hMenu);
//     }
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int SetLapa()
//   {
//   _static HCURSOR hLapa = theApp.LoadCursor( IDC_YES_TRACK );
//   ::SetCursor( hLapa );
//   return 0;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int SetGoto()
//   {
//   _static HCURSOR hGoto = theApp.LoadCursor( IDC_YES_GOTO );
//   ::SetCursor( hGoto );
//   return 0;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int SetArrow()
//   {
//   _static HCURSOR hArrow = ::LoadCursor( NULL, IDC_ARROW );
//   ::SetCursor( hArrow );
//   return 0;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int SetHand()
//   {
//   _static HCURSOR hHand = ::LoadCursor( NULL, IDC_HAND );
//   ::SetCursor( hHand );
//   return 0;
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CAPMApp::DrawAlarmIcon( HDC hDC, POINT pt, SIZE* sz, BYTE btEHLN, BYTE btKvit, BYTE btOnOf, int nShelved )
//   {
//   //EnumSTATUS(None     ,0,"N 00")
//   //EnumSTATUS(Journal  ,1,"J 00")
//   //EnumSTATUS(Low      ,2,"L 00")
//   //EnumSTATUS(High     ,3,"H 00")
//   //EnumSTATUS(Emergncy ,4,"U 00")
//   //
//   // Вычисляем номер иконки, которую надо вывести.
//   int N = -1;
//   if ( 0 < btEHLN && btEHLN < 5 )
//     {
//     if ( PulsTimer() || btKvit )
//       {
//       if ( 1 < btEHLN && btEHLN < 5 )
//         N = btEHLN*2-4+btOnOf;
//       else
//         N = 6;
//       }
//     }
//    if (nShelved>0)
// 	   N=7;
//   static std::string ID[] =
//     {
// 	"blueNotAck.png", "blueAck.png",// Low
//     "yellNotAck.png", "yellAck.png",// High
//     "redNotAck.png" , "redAck.png" ,// Emergency
//     "ghost.png"     ,// Всё остальное
// 	"shelved.png"
//     };
//   static int nCount = sizeof(ID)/sizeof(ID[0]);
//   // Не выход за пределы массива
//   if ( !( 0 <= N && N < nCount ) ) return;
//   	 Graphics graphics( hDC );
//      graphics.SetSmoothingMode(SmoothingModeHighQuality);
//      graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
//      graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
// 	 CModelInfo& model = ::ModelInfo();
// 	 std::string strPath=GetBasePath();
// 	 strPath+="Data\\Honeywell_Alarms\\";

//      if(Version == DOTF_PM)
//        {//тут только красный и желтый
//          if(N<4)
//            N += 2;
//        }
// 	 strPath+=ID[N];
//      if (!FileExists(strPath))
// 	 {
// 		 std::string strMessage="Файл иконки аларма: "+strPath+" не найден\nНе скопировали папку Data\\Honeywell_Alarms?";
// 		 if (!m_bOnceAlarmIconMessageBox)
// 		 {
// 			AfxMessageBox(strMessage.c_str(),MB_OK | MB_ICONSTOP );
// 			m_bOnceAlarmIconMessageBox=true;
// 		 }
// 		 return;
// 	 }
// 	 wchar_t wPath[_MAX_PATH];
// 	 MultiByteToWideChar(CP_ACP, 0, strPath.c_str(), -1, wPath, _MAX_PATH);
// 	 Image image(wPath);
// 	 int nHeight=image.GetHeight();
// 	 int nWidth=image.GetWidth();
// 	 SIZE SZ = { nWidth, nHeight };
//      if ( sz == NULL ) sz = &SZ;
// 	 graphics.DrawImage(&image,pt.x,pt.y,sz->cx,sz->cy);
//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool CAPMApp::FileExists(std::string strFileName)
{
    int Code = GetFileAttributes(strFileName.c_str());
    return ((Code!=INVALID_FILE_ATTRIBUTES) && ((FILE_ATTRIBUTE_DIRECTORY & Code) == 0));
}

// void CAPMApp::SetupIniFile()
// {
//     //need move into sharp

//   // free((void*)m_pszProfileName);

//   // TCHAR buf[_MAX_PATH], name[_MAX_FNAME];

//   // ::GetModuleFileName(NULL, buf, _countof(buf) );

//   // _tsplitpath_s(buf, NULL, 0, NULL, 0, name, _countof(name), NULL, 0);


//   // LPCSTR pszMemPath = GetMemoryPath();
//   // _tmakepath_s(buf, _countof(buf)-1, NULL, pszMemPath, name, _T(".ini") );

//   // m_pszProfileName = _tcsdup(buf);
// }

// int CAPMApp::GetDialogNumber(cross::CString strKlapanName, int nDialog)
// {
// //  return dlgFromCSV.GetDialogNumber(strKlapanName, nDialog);
// }

// std::string CAPMApp::GetTexnoshemeNameCSV()
// {
// //	return texnoCSV.GetTexnoshemeNameCSV();
// }

// std::string CAPMApp::GetHoneywellStartNameCSV()
// {
// //	return texnoCSV.GetHoneywellStartNameCSV();
// }

// std::string CAPMApp::GetHoneywellMenuNameCSV()
// {
// //	return texnoCSV.GetHoneywellMenuNameCSV();
// }

// std::string CAPMApp::GetHoneywellInfoName()
// {
// //	return texnoCSV.GetHoneywellInfoName();
// }

// std::string CAPMApp::GetTexnoValueByKey(std::string key)
// {
//  //   return texnoCSV.GetTexnoValueByKey(key);
// }

// int CAPMApp::GetStandartDialogNumberFromCSVFile(EDataTypes eData, UINT eScheme, std::string strName)
// {
// //	return texnoCSV.GetStandartDialogNumberFromCSVFile(eData,eScheme,strName);
// }

// std::string CAPMApp::GetElectroZadvDialogCSV()
// {
// //	return texnoCSV.GetElectroZadvDialogCSV();
// }

// std::string CAPMApp::GetSpecialPanoramsDir()
// {
// //	return texnoCSV.GetPanoramDirFromCSV();
// }

// std::string CAPMApp::GetExePanoramPath()
// {
// //	return texnoCSV.GetPanoramDirExeFromCSV();
// }

// bool CAPMApp::IsDefaultKlapanDialog(std::string strDialogName)
// {
// //	return texnoCSV.IsDefaultKlapanDialog(strDialogName);
// }

// cross::CString CAPMApp::FindPointInArray(UINT nType, cross::CString strName)
// {
// 	cross::CString strFind="Не найдены";
// 	cross::CString strType=GetPointNameByType(nType);
// 	for (int i=0;i<m_nPointsFromXMLArrayCount;i++)
// 	{
// 		if (strPointsFromXMLArray[i][1]==strName)
// 		{
// 			if (strPointsFromXMLArray[i][0]==strType)
// 			{
// 				strFind=strPointsFromXMLArray[i][0]+";"+strPointsFromXMLArray[i][1]+";"+
// 				strPointsFromXMLArray[i][2];
// 				return strFind;
// 			}
// 		}
// 	}
// 	return strFind;
// }

// cross::CString CAPMApp::GetPointNameByType(UINT eData)
// {
// 	switch (eData)
// 	{
// 	case id_Sensor: return "Датчик";
// 	case id_Pump: return "Насос";
// 	case id_Klapan: return "Клапан";
// 	case id_Zadv: return "Задвижка";
// 	case id_ShZaslonka: return "HS";
// 	case id_HS: return "HS";
//     case id_PHS: return "ППК";
// 	case id_KVO: return "КВО";
// 	case id_ShBallZadv: return "Шаровая задвижка";
// 	case id_Ventil: return "Вентиль";
// 	case id_Sensor_45: return "Датчик_45";
// 	case id_Sensor_46: return "Датчик_46";
// 	case id_Sensor_47: return "Датчик_47";
// 	case id_3xZadv: return "3х_задвижка";
// 	case id_ZadvZaglushka: return 	"Задвижка с заглушкой";
// 	case id_Library: return "Пульт по месту";
// 	case id_Reductor: return "Редуктор";
// 	default: 	return "";
// 	}
// }

// cross::CString CAPMApp::GetServerParamsForPanoram()
// {
// 	cross::CString strParams="";
// 	cross::CString strServer=CPipeClient::ms_szServer;
// 	if (strServer!=".")
// 	{
// 		strParams="-s__oper__";
// 		strParams+="\"";
// 		strParams+=CPipeClient::ms_szServer;
// 		strParams+=";";
// 		strParams+=CPipeClient::ms_szOperator;
// 		strParams+=";";
// 		strParams+=CPipeClient::ms_szLockArea;
// 		strParams+="\"";
// 	}
// 	return strParams;
// }

// cross::CString CAPMApp::GetPointParamsForPanoram(cross::CString strPoint)
// {
// 	cross::CString strParams="";
// 	if (strPoint!="")
// 		strParams="-p"+strPoint;
// 	return strParams;
// }

// void CAPMApp::StartPanoram3D(cross::CString strPoint)
// {
// 		cross::CString panoram3d;
// 		panoram3d.Format( "%sPanoram3D.exe", ::GetExePath() );
// 		cross::CString strCmd="";
// 		cross::CString strPointParams=GetPointParamsForPanoram(strPoint);
// 		if ( strcmp( CPipeClient::ms_szServer, "." ) ==0)
// 		{
// 			strCmd=strPointParams;
// 			::ShellExecute( NULL, "Open", panoram3d, strCmd, "", SW_SHOWNORMAL );
// 			m_bPanoramaInTop=true;
// 		}
// 		else
// 		{
// 			cross::CString strServerParams=GetServerParamsForPanoram();
// 			if (strServerParams=="")
// 				strCmd=strPointParams;
// 			else
// 				strCmd=strServerParams+" "+strPointParams;
// 			::ShellExecute( NULL, "Open", panoram3d, strCmd, "", SW_SHOWNORMAL );
// 			m_bPanoramaInTop=true;
// 		}
// }

// bool CAPMApp::IsModelDataType(UINT type)
// {
// 	return ((type==id_Ventil)||(type==id_3xZadv)||(type==id_ElectroNagrev)) ;
// }

// bool CAPMApp::IsSpecificModelDataType(UINT type)
// {
// 	return ((type==id_Ventil)||(type==id_3xZadv)) ;
// }

// int CAPMApp::GetWindow1FromCSV()
// {
// 	return texnoCSV.GetWindow1FromCSV();
// }

// int CAPMApp::GetWindow4FromCSV()
// {
// 	return texnoCSV.GetWindow4FromCSV();
// }

// std::string CAPMApp::GetSensorWndcross::RECTString()
// {
// 	return texnoCSV.GetSensorWndcross::RECTString();
// }

// cross::CString CAPMApp::GetPultName(cross::CString strName)
// {
// 	return dlgFromCSV.GetPultName(strName);
// }

// bool CAPMApp::IsSensorNameInFile(cross::CString strName)
// {
// 	return sensorsFromCSV.IsSensorNameInFile(strName);
// }
