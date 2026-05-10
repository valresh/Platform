// #include "StdAfx.h"
// #include "../ARM.h"
// #include "../AplicFrm.h"
// #include "TagDIV.h"
// #include "HTML.h"
// #include "_AlarmEntry.h"
// #define _USE_MATH_DEFINES
// #include <math.h>
// #include "Shape.h"
// //
// // Специфика окна sysAlarmSummary
// void CTagDIV::SpecificAlarm( CHTML& html )
//   {
//   CStyle& style = Style(html);
//   if ( !(style.m_dwTypeObj & TYPE_ALARMSPEC) )
//     return;
//   //
//   if ( !SetLink( html, TYPE_ALARM_TABLE ) )
//     return;
//   //
//   if ( !m_pLink->IsAlarmWnd() )
//     return;
//   //
//   CViewBase* wnd = ((CHTML_Alarm*)m_pLink)->wnd;
//   if ( wnd == NULL ) return;
//   //
//   bool bBlue = wnd->IsFilterApplied();
//   //
//   // При включённом фильтре - надо включить эти объекты
//   switch ( TYPE_PNT )
//     {
//     case TYPE_ACK_TOTAL_AL: // Квитированные сигнализации все
//     case TYPE_UNA_TOTAL_AL: // Неквитированные сигнализации все
//     case TYPE_ACK_TOTAL_OF: // Квитированные сигнализации of
//     case TYPE_UNA_TOTAL_OF: // Неквитированные сигнализации of
//     case TYPE_FILTER_APPLIED:// Наличие фильтра
//       style.m_eVisible = bBlue ? enumVisible : enumHidden;
//     break;
//     case TYPE_PAUSE_ALARM: // Пауза обновлений
//       style.m_eVisible = wnd->IsPause() ? enumVisible : enumHidden;
//     break;
//     }
//   //
//   switch ( TYPE_PNT )
//     {
//     case TYPE_ACK_TOTAL_AL: // Квитированные сигнализации все
//       m_constValue.Format( "%d", wnd->AckCountAL() );
//     break;
//     case TYPE_UNA_TOTAL_AL: // Неквитированные сигнализации все
//       m_constValue.Format( "%d", wnd->UnaCountAL() );
//     break;
//     case TYPE_ACK_TOTAL_FL: // Квитированные сигнализации отфильтрованные
//       m_constValue.Format( "%d", wnd->AckCountFL() );
//       style.m_clrText = bBlue ? 0xff0000 : 0;
//     break;
//     case TYPE_UNA_TOTAL_FL: // Неквитированные сигнализации отфильтрованные
//       m_constValue.Format( "%d", wnd->UnaCountFL() );
//       style.m_clrText = bBlue ? 0xff0000 : 0;
//     break;
//     }
//   }

// void CTagDIV::SpecificButton( CHTML& html )
//   {
//   CStyle& style = Style(html);
//   if ( !(style.m_dwTypeObj & TYPE_SYS_BUTTON) )
//     return;
//   //
//   if ( !SetLink( html, TYPE_ALARM_TABLE ) )
//     return;
//   //
//   if ( !m_pLink->IsAlarmWnd() )
//     return;
//   //
//   CViewBase* wnd = ((CHTML_Alarm*)m_pLink)->wnd;
//   if ( wnd == NULL ) return;
//   //
//   switch ( TYPE_PNT )
//     {
//     case TYPE_ALARM_PAUSE :// Pause
//       //wnd->Pause();
//       break;
//     case TYPE_ALARM_RESUME:// Resume
//       //wnd->Resume();
//       break;
//     case TYPE_ALARM_ACK   :// Acknowledge page
//  		  //wnd->AckPage();
//       break;
//     }
//   }

// void SetSpanColor( CHTML& html, CTag* tag, COLORREF color )
//   {
// 	INT_PTR nCount = tag->GetSize();
//   CTag**     obj = tag->GetData();
// 	for ( INT_PTR n = 0; n < nCount; n++ )
//     {
//     CStyle& style = obj[n]->Style(html);
//     const char* name    = style.GetName(html);// SCRIPT
//     if ( lstrcmpi( name, "SPAN" ) == 0 )
//       {
//       style.m_clrText = color;
//       break;
//       }
//     }
//   }

// void SetImageToImg( CHTML& html, CTag* tag, char* name )
//   {
//   if ( tag == NULL ) return;
//   CharMP szPath,szCurr;
//   html.FindName( szPath );
//   GetCurrentDicross::RECTory(SizeMP,szCurr);
//   SetCurrentDicross::RECTory(szPath);
//   //
//   ASSERT(0);//Xaxa
//   tag->mImg.Clear();
//   LoadImages( name, tag->mImg );
//   //
//   SetCurrentDicross::RECTory(szCurr);
//   }

// bool CTag::ActionSysBtn( CHTML& html, HWND hWnd )
//   {
//   CStyle& style = Style(html);
//   if ( !(style.m_dwTypeObj & TYPE_SYS_BUTTON) )
//     return false;
//   //
//   if ( !SetLink( html, TYPE_ALARM_TABLE ) )
//     return false;
//   //
//   if ( !m_pLink->IsAlarmWnd() )
//     return false;
//   //
//   CViewBase* wnd = ((CHTML_Alarm*)m_pLink)->wnd;
//   if ( wnd == NULL ) return false;
//   //
//   switch ( TYPE_PNT )
//     {
//     case TYPE_CLEAR_FILTR :// Clear All Filters
//       wnd->ClearAllFilters();
//       break;
//     case TYPE_RESET_VIEW  :// Reset View
//       wnd->ResetView();
//       break;
//     case TYPE_ALARM_PRINT :// PrintReset View
//       wnd->PrintAlarm();
//       break;
//     case TYPE_ALARM_PAUSE :// Pause
//       //onhdxclick=
//       //divPaused.style.display = 'inline';
//       //btnResume.disabled = false;
//       //ImgResume.src='Images/Resume.gif';
//       //this.disabled = true;
//       //ImgPause.src='Images/PauseDisabled.gif'
//       if ( !wnd->IsPause() )
//         {
//         style.mDisabled = true;
//         SetSpanColor( html, this, ColorDiez("threedshadow") );
//         CTag* tag0 = html.LinkScript( html, "btnResume" );
//         if ( tag0 )
//           {
//           CStyle& s0 = tag0->Style(html);
//           s0.mDisabled = false;
//           SetSpanColor( html, tag0, 0 );
//           }
//         CTag* tag1 = html.LinkScript( html, "ImgResume" );
//         SetImageToImg( html, tag1, "Images/Resume.png" );
//         CTag* tag2 = html.LinkScript( html, "ImgPause"  );
//         SetImageToImg( html, tag2, "Images/PauseDisabled.png" );
//         }
//       wnd->Pause();
//       break;
//     case TYPE_ALARM_RESUME:// Resume
//       //onhdxclick=
//       //divPaused.style.display = 'none';
//       //btnPause.disabled = false;
//       //ImgPause.src='Images/Pause.gif';
//       //this.disabled = true;
//       //ImgResume.src='Images/ResumeDisabled.gif'
//       if ( wnd->IsPause() )
//         {
//         style.mDisabled = true;
//         SetSpanColor( html, this, ColorDiez("threedshadow") );
//         CTag* tag0 = html.LinkScript( html, "btnPause"  );
//         if ( tag0 )
//           {
//           CStyle& s0 = tag0->Style(html);
//           s0.mDisabled = false;
//           SetSpanColor( html, tag0, 0 );
//           }
//         CTag* tag1 = html.LinkScript( html, "ImgResume" );
//         SetImageToImg( html, tag1, "Images/ResumeDisabled.png" );
//         CTag* tag2 = html.LinkScript( html, "ImgPause"  );
//         SetImageToImg( html, tag2, "Images/Pause.png" );
//         }
//       wnd->Resume();
//       break;
//     case TYPE_ALARM_ACK   :// Acknowledge page
//  		  wnd->AckPage();
//       break;
// 	case TYPE_ALARM_SHELVE :
// 		wnd->Shelve();
// 		break;
//     case TYPE_ALARM_UNSHELVE :
// 		wnd->UnShelve();
// 		break;
//     default:
//       return false;
//     }
//   return true;
//   }
