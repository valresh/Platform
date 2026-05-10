// #include "StdAfx.h"
// #include "TagIMG.h"
// #include "HTML.h"

// CTagIMG::CTagIMG(void)
// : mBmp(NULL)
//   {
//   }
// //
// CTagIMG::~CTagIMG(void)
//   {
//   if ( mBmp )
//     delete mBmp;
//   }
// //
// #undef  STD_VAR
// #define STD_VAR CHTML& html, CTagIMG& a, char* value
// typedef void (*LPFuncs)( STD_VAR );
// //
// void imgStyle( STD_VAR )
//   {
//   a.AnalizStyle( html, value );
//   }
// //
// void imgID( STD_VAR )
//   {
//   CStyle& style = a.Style(html);
//   if ( !*value ) return;
//   style.NameID( html, value );
//   }
// //_ghostSrc="res://AlarmStateIcon/GIF/#245"

// //_urgentRTNBadQualitySrc="res://AlarmStateIcon/GIF/#233"
// //_urgentBadQualitySrc="res://AlarmStateIcon/GIF/#238"
// //_urgentRTNRespondedSrc="res://AlarmStateIcon/GIF/#219"
// //_urgentRespondedSrc="res://AlarmStateIcon/GIF/#218"
// //_urgentRTNDisabledSrc="res://AlarmStateIcon/GIF/#221"
// //_urgentRTNSrc="res://AlarmStateIcon/GIF/#211"
// //_urgentDisabledSrc="res://AlarmStateIcon/GIF/#220"
// //_urgentSrc="res://AlarmStateIcon/GIF/#210"

// //_high RTNBadQualitySrc="res://AlarmStateIcon/GIF/#235"
// //_high BadQualitySrc="res://AlarmStateIcon/GIF/#234"
// //_high RTNRespondedSrc="res://AlarmStateIcon/GIF/#215"
// //_high RespondedSrc="res://AlarmStateIcon/GIF/#214"
// //_high RTNDisabledSrc="res://AlarmStateIcon/GIF/#223"
// //_high RTNSrc="res://AlarmStateIcon/GIF/#205"
// //_high DisabledSrc="res://AlarmStateIcon/GIF/#222"
// //_high Src="res://AlarmStateIcon/GIF/#204"

// //_lowRTNBadQualitySrc="res://AlarmStateIcon/GIF/#237"
// //_lowBadQualitySrc="res://AlarmStateIcon/GIF/#236"
// //_lowRTNRespondedSrc="res://AlarmStateIcon/GIF/#217"
// //_lowRespondedSrc="res://AlarmStateIcon/GIF/#216"
// //_lowRTNDisabledSrc="res://AlarmStateIcon/GIF/#225"
// //_lowRTNSrc="res://AlarmStateIcon/GIF/#208"
// //_lowDisabledSrc="res://AlarmStateIcon/GIF/#224"
// //_lowSrc="res://AlarmStateIcon/GIF/#207"

// //_alertRTNBadQualitySrc="res://AlarmStateIcon/GIF/#244"
// //_alertBadQualitySrc="res://AlarmStateIcon/GIF/#241"
// //_alertRTNDisabledSrc="res://AlarmStateIcon/GIF/#239"
// //_alertRTNSrc="res://AlarmStateIcon/GIF/#243"
// //_alertDisabledSrc="res://AlarmStateIcon/GIF/#242"
// //_alertSrc="res://AlarmStateIcon/GIF/#240"
// //
// void imgSrc( STD_VAR )
//   {
//   if ( !*value ) return;
//   CStyle& styl = a.Style(html);
//   char* id = styl.NameID( html );
// #ifdef _DEBUG
//   if ( _stricmp( id, "imgClearFilter" ) == 0 )
//     AAA();
// #endif
//   if ( !a.mImg.Yes() )
//     {
//     CharMP szPath = "";
//     _static char* src = "res://AlarmStateIcon/GIF/";
//     _static int   dln = lstrlen(src);
//     if ( _strnicmp( src, value, dln ) == 0 )
//       {
//       CharMP text;
//       if ( styl.m_dwFlagObj & STYLE_PROGID )
//         {
//         sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\emergency.bmp", ::GetBasePath(), R300 );
//         LoadImages( text, a.mImgU, szPath );//_urgentSrc="res://AlarmStateIcon/GIF/#210"
//         sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\high.bmp", ::GetBasePath(), R300 );
//         LoadImages( text, a.mImgH, szPath );//_high Src ="res://AlarmStateIcon/GIF/#204"
//         sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\low.bmp", ::GetBasePath(), R300 );
//         LoadImages( text, a.mImgL, szPath );//_lowSrc   ="res://AlarmStateIcon/GIF/#207"
//         sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\journal.bmp", ::GetBasePath(), R300 );
//         LoadImages( text, a.mImgA, szPath );//_alertSrc ="res://AlarmStateIcon/GIF/#240"
//         }
//       else
//         {
//         sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\%s.gif", ::GetBasePath(), R300, value+dln );
//         LoadImages( text, a.mImg, szPath );
//         }
//       //typedef short (WINAPI* GsrvW)(char* path, HANDLE bitmap);
//       //HINSTANCE hInst = LoadLibrary("Scn.Bmp.dll");
//       //GsrvW gsrv = (GsrvW)GetProcAddress(hInst,"WriteGraphicFile");
//       //(*gsrv)("c:/qqq.bmp",a.mImg.hBmp);
//       }
//     else
//       {
//       LoadImages( value, a.mImg, szPath );
//       }
//     CStyle& style = a.Style(html);
//     if ( *szPath )
//     style.SrcFile( html, szPath );
//     }
//   }
// //
// void imgXren( STD_VAR )
//   {
//   }
// //
// void imgClass( STD_VAR )
//   {
//   }
// //
// void imgBorder( STD_VAR )
//   {
//   ASSERT( IsInt(value) );
//   CStyle& style = a.Style(html);
//   style.mBorderImage = atoi(value);
//   ASSERT( style.mBorderImage == 0 );
//   }
// //
// void imgTabIndex( STD_VAR )
//   {
//   //ASSERT( ::IsInt(value) );
//   //style.nTabIndex = atoi(value);
//   }
// //
// void CTagIMG::Analiz( ANALIZ )
//   {
//   struct SLocal { char* name; LPFuncs func; };
//   _static SLocal list[] =
//     {
//     { "id"      , imgID       },
//     { "style"   , imgStyle    },
//     { "src"     , imgSrc      },
//     { "shapesrc", imgSrc      },
//     { "class"   , imgClass    },
//     { "border"  , imgBorder   },
//     { "tabIndex", imgTabIndex },
//     { "/"       , imgXren     },
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
//   //
//   CStyle& style = Style(html);
//   char* nm = style.NameID( html );
//   if ( lstrcmp( nm, "picture3" ) == 0 )
//   if ( lstrcmpi( html.m_strFile, "100\\d108" ) == 0 )
//     style.mcross::RECT.Width+=76,style.mcross::RECT.Height+=28;
//   //
//   if ( mImg.hBmp )
//     {
//     LPBITMAPINFO info = (LPBITMAPINFO)::GlobalLock( mImg.hBmp );
//     if ( info != NULL )
//       {
//       if ( !(style.m_dwFlagObj & STYLE_WIDTH) )
//       style.mcross::RECT.Width  = (REAL)info->bmiHeader.biWidth;
//       if ( !(style.m_dwFlagObj & STYLE_HEIGHT) )
//       style.mcross::RECT.Height = (REAL)info->bmiHeader.biHeight;
//       ::GlobalUnlock( mImg.hBmp );
//       }
//     }
//   style.m_dwFlags |= HDX_PRESENT;
//   //
//   }
// //
// void CTagIMG::Matrix( LOCALS )
//   {
//   CStyle& style = Style(html);
//   char* id = style.NameID( html );
//   if ( style.LocalD( hDC ) ) return;
// #ifdef _DEBUG
//   if ( StopDebug( html, "ShpGOPState5_picture001" ) )
//     AAA();
// #endif
//   if ( style.m_dwFlagObj & STYLE_PROGID )
//     {
//     AAA();
//     return;
//     //sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\emergency.bmp", ::GetBasePath(), R300 );
//     //LoadImages( text, mImgU, szPath );//_urgentSrc="res://AlarmStateIcon/GIF/#210"
//     //sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\high.bmp", ::GetBasePath(), R300 );
//     //LoadImages( text, mImgH, szPath );//_high Src ="res://AlarmStateIcon/GIF/#204"
//     //sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\low.bmp", ::GetBasePath(), R300 );
//     //LoadImages( text, mImgL, szPath );//_lowSrc   ="res://AlarmStateIcon/GIF/#207"
//     //sprintf_s( text, SizeMP, "%s%s\\AlarmStateIcon\\journal.bmp", ::GetBasePath(), R300 );
//     //LoadImages( text, mImgA, szPath );//_alertSrc ="res://AlarmStateIcon/GIF/#240"
//     }
//   //
//   if ( style.mBorderImage == 0 )
//     {
//     style.mBorderImage = -1;
//     if ( mBmp == NULL )
//       {
//       char* src = style.SrcFile( html );
//       if ( *src )
//         {
//         CharMP ext;
//         SplitPath( src, 0, 0, 0, ext );
//         if ( lstrcmpi( ext, ".png" ) == 0 )
//           mBmp = BMP(src);
//         }
//       }
//     }
//   //
//   cross::RECT cross::RECT = style.Fullcross::RECT();
//   if ( mBmp != NULL )
//     {
//   	Graphics gr( hDC );
//     gr.DrawImage( mBmp, style.mcross::RECT );
//     }
//   else
//   if ( mImg.Yes() )
//     OutImage( hDC, mImg, cross::RECT);
//   else
//     {
//     CMyBrush brush( hDC, 0xff00 );
//     brush.cross::RECT( cross::RECT );
//     }
//   }
