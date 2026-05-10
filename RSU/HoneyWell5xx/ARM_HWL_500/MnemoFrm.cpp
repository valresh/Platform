// #include "stdafx.h"
// #include "afxpriv.h"
// #include "ARM.h"
// #include "AplicFrm.h"
// #include "MnemoFrm.h"
// #include <atltime.h> //for CTime && CTimeSpan
// #include "ViewTrend3.h"
// #include "DlgDebug.h"

// #define ID_WND_FACEPLATE	5001
// #define ID_WND_ALARM			5002

// #define ID_KILL_DIAGNOZE		1001
// #define KILL_DIAGNOZE_TO		250

// CMnemoFrm::CMnemoFrm(HWND& hTouchWnd,KHoneyPipeClient& rClient)
// : CViewMnemo(rClient)
// , m_hTouchWnd(hTouchWnd)
// , m_nShowRevim(0)
// , m_hGlobal(NULL)
// , m_bPriorDisp(false)
//   {
//   memset( &m_ptMouse, 0, sizeof(m_ptMouse) );
//   }

// CMnemoFrm::~CMnemoFrm()
//   {
//   if ( m_hGlobal ) ::GlobalFree( m_hGlobal );
//   }

// // BEGIN_MESSAGE_MAP(CMnemoFrm, CViewMnemo)
// //   ON_WM_CREATE()
// //   ON_WM_MOUSEWHEEL()
// //   ON_WM_LBUTTONDOWN()
// //   ON_WM_LBUTTONUP()
// //   ON_WM_LBUTTONDBLCLK()
// //   ON_WM_MOUSEMOVE()
// //   //
// //   ON_COMMAND(ID_SHOW_WORK_1, OnValueWork1)
// // 	ON_COMMAND(ID_SHOW_WORK_2, OnValueWork2)
// // 	ON_COMMAND(ID_SHOW_WORK_3, OnValueWork3)
// //   //
// // END_MESSAGE_MAP()
// //
// int CMnemoFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
//   {
//   if (CViewMnemo::OnCreate(lpCreateStruct) == -1)
//     return -1;
//   //
//   std::string strFirstSheme=theApp.GetHoneywellStartNameCSV();
//   if (theApp.IsAllInOne())
//     {
//      CAmmFrm *pParentAmmFrm;
//      if(pParentAmmFrm = dynamic_cast<CAmmFrm*>(GetParent()))
//        {
//          CAplicFrm *pParentAplicFrm = dynamic_cast<CAplicFrm*>(pParentAmmFrm->GetParent());
//          if(pParentAplicFrm)
//          {
//              strFirstSheme = pParentAplicFrm->GetHoneywellStartNameCSVbyCAmmFrm(pParentAmmFrm);
//          }
//        }
//      }
//   //
//   if (strFirstSheme=="")
// 		strFirstSheme="MENU";
//   CString strCurDisp = ReadString( mDoc.pipe.m_szPipeName, _T(strFirstSheme.c_str()));
//   //strCurDisp = _T("sysAlarmSummary");
//   // Для теста - проход по всем мнемосхемам
//   if ( ::IsMnemoStyle(MNEMO_COMMENT) )
//     {
//     CommentRec("\\");
//     ::MessageBox(NULL, "Перебор схем окончен", "Готово", MB_OK);
//     mDoc.Print( true );
//     }
//   EventPanel(strCurDisp,NULL,true);
//   //
//   return 0;
//   }

// void CMnemoFrm::CommentRec(const char* pszDir)
//   {
//   _static CharMP szAbs;
//   if ( !*szAbs )
//   sprintf_s( szAbs, SizeMP, "%sAbstract", ::GetBasePath() );
//   //
//   CharMP szPath;
//   sprintf_s( szPath, SizeMP, "%s%s*.htm", szAbs, pszDir );
//   //
//   WIN32_FIND_DATA Find;
//   HANDLE hFind = ::FindFirstFile( szPath, &Find );
//   if ( hFind != INVALID_HANDLE_VALUE )
//     {
//     BOOL Res = TRUE;
//     while ( Res )
//       {
//       if ( !::IsDir( Find ) )
//         {
//         int dl = lstrlen(Find.cFileName);
//         Find.cFileName[dl-4] = 0;
//         sprintf_s( szPath, SizeMP, "%s%s", pszDir+1, Find.cFileName );
//         BuildQueue( szPath, NULL );
//         mDoc.Print( false );
//         }
//       Res = ::FindNextFile( hFind, &Find );
//       }
//     ::FindClose( hFind );
//     }
//   //
//   sprintf_s( szPath, SizeMP, "%s%s*.*", szAbs, pszDir );
//   hFind = ::FindFirstFile( szPath, &Find );
//   if ( hFind != INVALID_HANDLE_VALUE )
//     {
//     BOOL Res = TRUE;
//     while ( Res )
//       {
//       if ( ::IsDir( Find, true ) )
//         {
//         sprintf_s( szPath, SizeMP, "%s%s\\", pszDir, Find.cFileName );
//         if ( lstrlen(pszDir) == 1 )
//         CommentRec(szPath);
//         }
//       Res = ::FindNextFile( hFind, &Find );
//       }
//     ::FindClose( hFind );
//     }
//   }

// void CMnemoFrm::MainTimer(ULONG dwTimer)
//   {
//   CViewMnemo::MainTimer(dwTimer);
//   }

// void CMnemoFrm::EventPanel( LPCTSTR pszFile, LPCTSTR pszText, bool bAddUndoRedo )
//   {
//   char szFile[256]="";
//   char szText[512]="";
//   strcpy_s(szFile, pszFile);
//   if(pszText)
//     strcpy_s(szText, pszText);

//   SUndoRedo ur = mUndo.Curr();
//   bool same = (!lstrcmpi(pszFile, ur.szFile) && !lstrcmpi(pszText, ur.szText));
//   if (mDoc.m_strFile=="sysAlarmSummary")
//   {
// 	  if (GetAppWnd()!=NULL)
// 		GetAppWnd()->HideInfoBar();
//   }

//   if (theApp.IsAllInOne() || theApp.m_bFourMonitors)
//     {
//        if(bAddUndoRedo)
//        {
//        if(!mDoc.m_strFile.IsEmpty() && mDoc.m_strFile != pszFile)
//          {
//          CAmmFrm *amm = GetAppWnd()->GiveAmmByhWnd(m_hWnd);
//          if(amm)
//            {
//                CAplicFrm::UndoRedoRec rec;
//                rec.name = mDoc.m_strFile;
//                rec.param = mDoc.m_strGroup;
//                rec.nPosition = amm->m_nPosition;
//                GetAppWnd()->GlobalUndoListPushBack(rec);
//                GetAppWnd()->m_RedoList.clear();
//            }
//          }
//        }
//     }

//   BuildQueue( pszFile, pszText );
//   if ( !mDoc.m_bR300 )
//     SaveString( mDoc.pipe.m_szPipeName, mDoc.m_strFile );
//   GetParent()->SetForegroundWindow();
//   if ( bAddUndoRedo && !same )
//     {
//     m_bPriorDisp = false;
//     if (!theApp.IsAllInOne())
//     {
//     if(pszText==NULL)
// 	  mUndo.Add( mDoc.m_strFile, mDoc.Title() );
//     else mUndo.Add( mDoc.m_strFile, szText );
//     }
//     }
//   }
// //
// // Показывать на мнемосхеме значения рабочего режима
// void CMnemoFrm::ShowWorkValue( int n )
//   {
//   if ( !::IsMnemoStyle(MNEMO_EDIT) )
//     return;
//   char szPath[256];
//   CModelInfo& model = ::ModelInfo();
//   sprintf_s( szPath, sizeof(szPath), "%sРежим%d", model.dirScript, n );
//   DWORD dwAttr = ::GetFileAttributes( szPath );
//   if ( dwAttr == -1 ) return;
//   if ( (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 ) return;
//   //
//   m_nShowRevim = ( abs(m_nShowRevim) == n ) ? 0 : n;
//   if ( m_hGlobal ) ::GlobalFree( m_hGlobal ); m_hGlobal = NULL;
//   //
//   HDC hDC = ::GetDC( m_hWnd );
//   if ( hDC )
//     {
//     m_bPaintStat = true;
//     Draw( hDC );
//     ::ReleaseDC( m_hWnd, hDC );
//     }
//   }
// void CMnemoFrm::OnValueWork1() { ShowWorkValue(1); }
// void CMnemoFrm::OnValueWork2() { ShowWorkValue(2); }
// void CMnemoFrm::OnValueWork3() { ShowWorkValue(3); }
// //
// // Если окно клавиатуры видимо, то делаем ему скроллинг.
// // Если окно переходов по мнемосхемам видимо, то делаем ему скроллинг.
// // Если окно переходов по мнемосхемам не видимо, то просто листаем
// // мнемосхемы по-порядку, как они прописаны в файле 'Список'.
// // 259 - Не листаем для двойного окна
// BOOL CMnemoFrm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//   {
//   if(Version != BERTH_ONPZ
// 	  || !GetAppWnd()->m_wndDubl->IsMnemo(m_hWnd))
//   {
// 	if ( GetAsyncKeyState(VK_CONTROL)& 0x8000 )
//     {
// 		const char* name = "";
// 		if ( zDelta > 0 )
// 		  name = DispPrev(mUndo.Curr().szFile);
// 		else
// 		  name = DispNext(mUndo.Curr().szFile);
// 		if ( *name )
// 		  EventPanel(name,NULL,true);
// 		return FALSE;
//     }
//   }
//   return CViewMnemo::OnMouseWheel(nFlags, zDelta, pt);
//   }

// void CMnemoFrm::BuildQueue( LPCTSTR pszName, LPCTSTR pszTagGrp )
//   {
//   if ( m_hGlobal ) ::GlobalFree( m_hGlobal ); m_hGlobal = NULL;
//   m_nShowRevim = abs(m_nShowRevim);
//   CViewMnemo::BuildQueue(pszName,pszTagGrp);
//   }

// bool CMnemoFrm::DrawValue( HDC hDC, RECT& rect )
//   {
//   if ( m_nShowRevim == 0 ) return true;

//   if ( m_hGlobal == NULL && m_nShowRevim > 0 )
//     {
//     CharMP szPath;
// 	sprintf_s( szPath, sizeof(szPath), "Режим%d\\%s", m_nShowRevim, /*mUndo.Curr()*/ mDoc.m_strFile );
// 	CString strFileName(szPath);
// 	strFileName.Replace(".htm","");
// 	m_hGlobal = ImageFile(strFileName.GetBuffer() );
// 	strFileName.ReleaseBuffer();
//     if ( m_hGlobal == NULL )
//       {
//       lstrcat( szPath, ".jpg" );
//       m_hGlobal = ImageFile( szPath );
//       }
//     if ( m_hGlobal == NULL )
//       {
//       sprintf_s( szPath, sizeof(szPath), "Режим%d\\%s", m_nShowRevim, mDoc.m_strFile );
//       lstrcat( szPath, ".bmp" );
//       m_hGlobal = ImageFile( szPath );
//       }
//     m_nShowRevim = -m_nShowRevim;
//     }
//   //
//   HDC idc = ::CreateCompatibleDC( hDC );
//   if ( idc != NULL )
//     {
//     HBITMAP ildBmp = (HBITMAP)::SelectObject( idc, m_hStatic );
//     if ( m_bPaintStat )
//       {
//       CMyBrush brush( idc, mDoc.Color() );
//       brush.Rect( rect );
//       if ( m_hGlobal == NULL )
//         {
//         int nMode = ::SetBkMode( idc, TRANSPARENT );
//         CMyFont font( idc, NULL, 0x80 );
//         font.Draw( rect, "Нет данных" );
//         ::SetBkMode( hDC, nMode );
//         }
//       else
//         {
//         LPBITMAPINFO info = (LPBITMAPINFO)::GlobalLock( m_hGlobal );
//         if ( info != NULL )
//           {
//           LPBYTE bits = (LPBYTE)info;
//           bits += sizeof( BITMAPINFOHEADER )+info->bmiHeader.biClrUsed*4;
//           //
// 	        SetStretchBltMode( idc, HALFTONE);
//           //
//           POINT ptOffset = GetDeviceScrollPosition();//От полос прокрутки
//           SIZE sz = mDoc.Size();
//           FLOAT fScaleX = (m_fScale*sz.cx)/info->bmiHeader.biWidth ;
//           FLOAT fScaleY = (m_fScale*sz.cy)/info->bmiHeader.biHeight;
//           FLOAT fScale  = min( fScaleX, fScaleY );
//           CMyWT wt( idc, fScale, ptOffset );
//           ::StretchDIBits( idc, 0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
//                                 0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
//                 bits, info, DIB_RGB_COLORS,SRCCOPY);
//           ::GlobalUnlock( m_hGlobal );
//           }
//         }
//       }
//     // Копируем битмап со статическими элементами в полный битмап
//     ::BitBlt( hDC, 0,0, rect.right, rect.bottom, idc, 0, 0, SRCCOPY );
//     ::SelectObject( idc, ildBmp );
//     ::DeleteDC(idc);
//     if ( ::PulsTimer() )
//     ::PaintRegimChar( hDC, 0, 0, m_nShowRevim );
//     }
//   return false;
//   }

// void CMnemoFrm::OnLButtonDblClk(UINT nFlags, CPoint point)
//   {
//   CViewMnemo::OnLButtonDblClk(nFlags, point);
//   }

// void CMnemoFrm::OnLButtonDown(UINT nFlags, CPoint point)
//   {
//   if ( !(GetAsyncKeyState( VK_CONTROL ) & 0x8000) )
//     m_pPressed = LButtonDown(point,true);
//   //
//   if ( m_pPressed == NULL )
//     {
//     DWORD dwStyle = GetStyle();
//     if ( dwStyle & (WS_VSCROLL|WS_HSCROLL) )
//       {
//       m_ptMouse = Scroll(point,false);
//       _static HCURSOR hLapa = NULL;
//       if ( hLapa == NULL )
//         {
//         HINSTANCE hInst = AfxGetResourceHandle( );
//         AfxSetResourceHandle( theApp.m_hInstance );
//         hLapa = ::LoadCursor( theApp.m_hInstance, MAKEINTRESOURCE(IDC_YES_TRACK) );
//         AfxSetResourceHandle( hInst );
//         }
//       ::SetCapture( m_hWnd );
//       ::SetCursor( hLapa );
//       }
//     }
//   //
//   CViewMnemo::OnLButtonDown(nFlags, point);
//   }

// void CMnemoFrm::OnLButtonUp(UINT nFlags, CPoint point)
//   {
//   if ( m_ptMouse.x != 0 && m_ptMouse.y != 0 )
//     {
//     ::ReleaseCapture();
//     m_ptMouse.x = 0;
//     m_ptMouse.y = 0;
//     }
//   CViewMnemo::OnLButtonUp(nFlags, point);
//   }

// void CMnemoFrm::OnMouseMove(UINT nFlags, CPoint point)
//   {
//   if ( m_ptMouse.x != 0 && m_ptMouse.y != 0 )
//     {
//     CSize sz;
// 		sz.cx = GetScrollLimit(SB_HORZ);
// 		sz.cy = GetScrollLimit(SB_VERT);
//     CPoint pt;
//     pt.x = min(max(m_ptMouse.x-point.x,0),sz.cx);
//     pt.y = min(max(m_ptMouse.y-point.y,0),sz.cy);
//     ScrollToDevicePosition( pt );
//     }
//   CViewMnemo::OnMouseMove(nFlags, point);
//   }
// //
// void CMnemoFrm::PageUndo()
//   {
//   if ( Keyboard(  HW_KEYDOWN, hw_STEP_UP ) )
//     return;
//   //
//   SUndoRedo ur = mUndo.Curr();
//   const char* name = DispPrev(ur.szFile);
//   if ( *name )
//     EventPanel(name,NULL,true);
//   }
// //
// void CMnemoFrm::PageRedo()
//   {
//   if ( Keyboard(  HW_KEYDOWN, hw_STEP_DOWN ) )
//     return;
//   //
//   SUndoRedo ur = mUndo.Curr();
//   const char* name = DispNext(ur.szFile);
//   if ( *name )
//     EventPanel(name,NULL,true);
//   }
// //
// void CMnemoFrm::DispUndo()
//   {
//   if ( mUndo.IsUndo() )
//     {
//     SUndoRedo ur = mUndo.Undo();
//     EventPanel(ur.szFile,ur.szText,false);
//     }
//   }
// //
// void CMnemoFrm::DispRedo()
//   {
//   if ( mUndo.IsRedo() )
//     {
//     SUndoRedo ur = mUndo.Redo();
//     EventPanel(ur.szFile,ur.szText,false);
//     }
//   }
// //
// void CMnemoFrm::PriorDsp()
//   {
//   if ( m_bPriorDisp )
//     m_bPriorDisp = false,DispRedo();
//   else
//     m_bPriorDisp = true ,DispUndo();
//   }
