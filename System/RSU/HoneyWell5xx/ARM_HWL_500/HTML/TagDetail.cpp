// #include "StdAfx.h"
// #include "TagDetail.h"
// #include "HTML.h"
// #include "../Extensions.h"

// CTagDetail::CTagDetail(void)
// : SUniBuffer( sizeof(SItemWindow), 0x1000 )
//   {
//   }
// //
// #undef  STD_VAR
// #define STD_VAR CHTML& html, CTagDetail& a, char* value
// typedef void (*LPFuncs)( STD_VAR );
// //
// void detailStyle( STD_VAR )
//   {
//   a.AnalizStyle( html, value );
//   }
// //
// void CTagDetail::Analiz( ANALIZ )
//   {
//   struct SLocal { char* name; LPFuncs func; };
//   _static SLocal list[] =
//     {
//     { "style", detailStyle },
//     }InitLocal;
//   //
//   int n = 0;
//   while ( teg[n].name )
//     {
//     SLocal* find = FindLocal( teg[n].name );
//     ASSERT( find );
//     if ( find != NULL )
//     (*find->func)(html,*this,teg[n].value);
//     n++;
//     }
//   CStyle& style = Style(html);
//   style.m_dwFlags |= HDX_PRESENT;
//   }
// //
// _static const char* AliasNames( const char* name )
//   {
//   if ( lstrcmpi( name, "OUT SUBSLOT" ) == 0 ) return "OUTSSLT";
//   //
//   if ( lstrcmpi( name, "LCN NODE" ) == 0 ) return "NODE_NO" ;
//   if ( lstrcmpi( name, "PT TYPE"  ) == 0 ) return "ENT_TYPE";
//   if ( lstrcmpi( name, "PV ALGO"  ) == 0 ) return "PVALGID" ;
//   if ( lstrcmpi( name, "PROC NET" ) == 0 ) return "NTWKNUM" ;
//   if ( lstrcmpi( name, "UCN NODE" ) == 0 ) return "NODENUM" ;
//   if ( lstrcmpi( name, "DEV TYPE" ) == 0 ) return "NODETYP" ;
//   if ( lstrcmpi( name, "CTL ALGO" ) == 0 ) return "CTLALGID";
//   if ( lstrcmpi( name, "CTL ALG"  ) == 0 ) return "CTLALGID";
//   if ( lstrcmpi( name, "ALGO"     ) == 0 ) return "ALGIDDAC";
//   if ( lstrcmpi( name, "SLOT"     ) == 0 ) return "SLOTNUM" ;
//   if ( lstrcmpi( name, "MOD NUM"  ) == 0 ) return "MODNUM"  ;
//   if ( lstrcmpi( name, "MOD TYPE" ) == 0 ) return "PNTMODTY";
//   if ( lstrcmpi( name, "HIWAY"    ) == 0 ) return "HWYNUM"  ;
//   if ( lstrcmpi( name, "BOXTYPE"  ) == 0 ) return "PNTBOXTY";
//   if ( lstrcmpi( name, "BOX"      ) == 0 ) return "BOXNUM"  ;
//   if ( lstrcmpi( name, "SUBSLOT"  ) == 0 ) return "INPTSSLT";
//   if ( lstrcmpi( name, "OUT BOX"  ) == 0 ) return "OUTBOXNM";
//   if ( lstrcmpi( name, "OUT SLOT" ) == 0 ) return "OUTSLTNM";
//   if ( lstrcmpi( name, "SP "      ) == 0 ) return "PVTV"    ;
//   if ( lstrcmpi( name, "OP "      ) == 0 ) return "AV"      ;
//   if ( lstrcmpi( name, "CYCLE"    ) == 0 ) return "NORMCYCL";
//   if ( lstrcmpi( name, "VAR NUMB" ) == 0 ) return "INTVARNM";

//   if ( lstrcmpi( name, "I1 "  ) == 0 ) return "I(2)";
//   if ( lstrcmpi( name, "I0 "  ) == 0 ) return "I(1)";
//   if ( lstrcmpi( name, "I2 "  ) == 0 ) return "I(3)";
//   if ( lstrcmpi( name, "P1 "  ) == 0 ) return "P(2)";
//   if ( lstrcmpi( name, "P0 "  ) == 0 ) return "P(1)";
//   if ( lstrcmpi( name, "P2 "  ) == 0 ) return "P(3)";
//   return name;
//   }
// //
// void CTagDetail::Kerdyk( KERDYK )
//   {
//   CTag::Kerdyk( hWnd, html );
//   //
//   CStyle& style = Style(html);
//   //
//   cross::CString strValue = style.GetValue(html);// код
//   char* ptr = strValue.GetBuffer();
//   bool bNoError = true;
//   while ( *ptr )
//     {
//     int nReturn;
//     STegScn* teg = theApp.ParsSCN( ptr, nReturn );
//     if ( nReturn > 0 ) //Все нормально
//       {
//       SItemWindow item;
//       memset( &item, 0, sizeof(item) );
//       item.m_nDot  = -1;
//       item.nShiftY = 18;
//       item.nLimitY = 10000;
//       STegObj obj[] =
//         {
//         { 'P', "pt"     , &item         },
//         { 'I', "x2"     , &item.curr.x  },
//         { 'I', "y2"     , &item.curr.y  },
//         { 'L', "left"   , &item.m_bLefts},
//         { 'L', "alm"    , &item.m_bAlarm},
//         { 'L', "cmb"    , &item.m_bCombo},
//         { 'I', "edit"   , &item.m_nEdit },
//         { 'I', "moda"   , &item.m_nEdit },//Установка моды
//         { 'I', "x3"     , &item.m_nDot  },
//         { 'I', "dot"    , &item.m_nDot  },
//         { 'S', "def"    , item.value, sizeof(item.value) },
//         { 'S', "fmt"    , item.formt, sizeof(item.formt) },
//         //
//         { 'I', "limitY" , &item.nLimitY },
//         { 'I', "shiftX" , &item.nShiftX },
//         { 'I', "shiftY" , &item.nShiftY },
//         };
//       //
//       ::ParserObjScn( teg+1, obj, sizeof(obj)/sizeof(STegObj) );
//       //
//       lstrcpy( item.name, teg[0].value );
//            if ( lstrcmpi( teg[0].name, "item"   ) == 0 ) item.m_eType = itemItem  ;
//       else if ( lstrcmpi( teg[0].name, "if"     ) == 0 ) item.m_eType = itemIF    ;
//       else if ( lstrcmpi( teg[0].name, "elseif" ) == 0 ) item.m_eType = itemELSEIF;
//       else if ( lstrcmpi( teg[0].name, "else"   ) == 0 ) item.m_eType = itemELSE  ;
//       else if ( lstrcmpi( teg[0].name, "endif"  ) == 0 ) item.m_eType = itemENDIF ;
//       else if ( lstrcmpi( teg[0].name, "AutoXY" ) == 0 ) item.m_eType = itemAutoXY;
//       else if ( lstrcmpi( teg[0].name, "/AutoXY") == 0 ) item.m_eType = itemAutoZZ;
//       //
//       if ( item.m_eType == itemXPEH )
//         continue;
//       //
//       item.mVar.nNumber = html.pipe.GetActive();
//       int nTag = html.pipe.GetMnemoTags( item.mVar.nNumber );
//       EDataTypes eType  = (EDataTypes)html.pipe.GetMnemoType( item.mVar.nNumber );
//       if ( item.m_eType == itemItem )
//         {
//         const char* alias = AliasNames(teg[0].value);
// 		item.mVar.def = ::NameToValueEx( eType, alias );
//         if ( !item.FillItem( html.pipe, nTag, alias ) )
//           {
//           if ( lstrcmpi( alias, "NODE_NO" ) )
//           continue;
//           lstrcpy( item.value, "21" );
//           item.m_nEdit = 1;
//           item.m_eType = itemValue;
//           }
//         lstrcat( item.name, ":" );
//         }
//       else
//       if ( item.m_eType == itemIF || item.m_eType == itemELSEIF )
//         {
//         item.nPlaceIF = mPol.AddCurr(0);
//         item.nCountIF = mPol.Recursiv( eType, teg[0].value );
//         }
//       else
//       if ( item.m_eType == itemInput || item.m_eType == itemOutput )
//         {
//         item.mVar.def = ::NameToValueEx( eType, teg[0].value );
//         }
//       AddObj( &item );
//       //
//       }
//     else
//     if ( nReturn == -1 ) //Текстовое оформление
//       {
//       bNoError = false;
//       break;
//       }
//     else
//     if ( nReturn == 0 ) //Конец документа
//       {
//       break;
//       }
//     else //Ошибка
//       {
//       bNoError = false;
//       break;
//       }
//     }
//   }
// //
// void CTagDetail::Matrix( LOCALS )
//   {
//   CStyle& style = Style(html);
//   char* id = style.NameID( html );
//   if ( style.LocalD( hDC ) ) return;
//   if ( StopDebug( html, "pushbutton003" ) )
//     AAA();
//   //
//   CMyFont font( hDC, style.m_hFont, style.m_clrText );
//   SItemValue* Auto = NULL;
//   int nStack = 0;
//   int stack[128] = {0};
//   int nCount = Count();
//   SItemWindow* item = (SItemWindow*)m_szBuffer;
//   for ( int n = 0; n < nCount; n++ )
//     {
//     if ( mPol.RetStack( &item[n], html.pipe, nStack, stack, &Auto ) )
//       continue;
//     item[n].PaintItem( html.pipe, font );
//     }
//   }
// //
// bool SItemWindow::FillItem( KHoneyPipeClient& pipe, int nTag, const char* alias )
//   {
//   ASSERT(0);
//   //Sergej26.03
//   /*
//   if ( mVar.def )
//     {
//     if ( m_nDot == -1 && mVar.def->eVal == enumValueChr )
//       {
//       m_nDot = ::NameToString( alias, 0, NULL );
//       if ( m_nDot == -1 ) m_nDot = 8;//FLAGS;
//       }
//     if ( m_bAlarm )
//       {
//       lstrcpy( value, alias );
//       int dl = lstrlen( value );
//       if ( dl > 3 )
//         {
//         value[dl-2] = 0;
//         lstrcat( value, "PR" );
//         mPr = ::NameToValueH( pipe.GetMnemoType( mVar.nNumber ), value );
//         }
//       }
//     return true;
//     }
//   m_nEdit = 1;
//   m_eType = itemValue;
//   const char* d = ::TegValueS( pipe.GetMnemoTags( mVar.nNumber ), alias );
//   if ( d == NULL ) return false;
//   //
//   lstrcpy( value, d );
//   if ( ::IsMnemoStyle(MNEMO_EDIT) )
//     lstrcat( name, "#" );
//     */
//   return true;
//   }
// //
// void SItemWindow::PaintItem( KHoneyPipeClient& pipe, CMyFont& font )
//   {
//   //Sergej26.03
//   /*
//   char text[512] = "XXX";
//   if ( m_eType == itemValue )
//     lstrcpy( text, value );
//   else
//     pipe.FillValue(text,mVar,m_nDot,formt);
//   //
//   if ( m_bLefts )
//     {
//     font.Text( x, y, name );
//     cross::RECT rc = {0,0,0,0};
//     ::DrawText( font, text, lstrlen(text), &rc, DT_CALCcross::RECT );
//     font.Text( x+rc.right+20, y, text );
//     }
//   else
//     {
//     font.Text( x, y, name );
//     int nAlign = ::SetTextAlign( font, TA_TOP | TA_RIGHT );
//     font.Text( x+175, y, text );
//     ::SetTextAlign( font, nAlign );
//     }
//   if ( mPr == NULL ) return;
//   SParamValueH var( mVar.nNumber, mPr );
//   BYTE val = pipe.ValueB( var );
//   if ( val == 0xff ) return;
//   if ( val >     7 ) return;
//   _static HBITMAP hAlarm[8];
//   _static bool bOk[8];
//   _static SIZE size;
//   if ( hAlarm[val] == NULL && !bOk[val] )
//     {
//     bOk[val] = true;//Отрезаем повторное чтение
//     ::NameToString( mPr->name, val, text );
//     lstrcat( text, ".bmp" );
//     HGLOBAL hGlobal = GlobalR300( text );
//     if ( hGlobal != NULL )
//       {
//       hAlarm[val] = BitmapR300( hGlobal, size  );
//       ::GlobalFree( hGlobal );
//       }
//     }
//   if ( hAlarm[val] == NULL ) return;
//   //
//   HDC hdc = ::CreateCompatibleDC( font );
//   if ( hdc )
//     {
//     BITMAP bmp;
//     ::GetObject( hAlarm[val], sizeof(BITMAP), &bmp );
//     //
//     HBITMAP hOld = (HBITMAP)::SelectObject( hdc, hAlarm[val] );
//     ::BitBlt( font, x-bmp.bmWidth-2, y, bmp.bmWidth, bmp.bmHeight,
//                hdc, 0, 0, SRCCOPY );
//     ::SelectObject( hdc, hOld );
//     ::DeleteDC( hdc );
//     }
//     */
//   }
