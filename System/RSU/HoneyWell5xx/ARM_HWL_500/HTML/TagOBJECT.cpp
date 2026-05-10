// #include "StdAfx.h"
// #include "HTML.h"
// #include "TagOBJECT.h"

// CTagOBJECT::CTagOBJECT(void)
//   {
//   }
// //
// #undef  STD_VAR
// #define STD_VAR CHTML& html, CTagOBJECT& a, char* value
// typedef void (*LPFuncs)( STD_VAR );
// //
// void objectStyle( STD_VAR )
//   {
//   a.AnalizStyle( html, value );
//   }
// //
// void objectClass( STD_VAR )
//   {
//   CStyle& style = a.Style(html);
//   if ( lstrcmpi( value, "hsc.trend.1" ) == 0 )
//     {
//     style.AddFlag( TYPE_TREND_GRP );
//     }
//   if ( lstrcmpi( value, "hsc.chart.1" ) == 0 )
//     {
//     style.AddFlag( TYPE_TREND_GRP );
//     }
//   }
// //
// void objectHdxProperties( STD_VAR )
//   {
//   a.AnalizHdx( html, value );
//   }
// //
// void objectID( STD_VAR )
//   {
//   CStyle& style = a.Style(html);
//   if ( lstrcmpi( value, "trend001" ) == 0 )
//     {
//     style.AddFlag( TYPE_TREND_GRP );
//     style.m_dwFlags |= HDX_PRESENT;
//     }
//   else
//   if ( lstrcmpi( value, "SystemTrend" ) == 0 )
//     {
//     style.AddFlag( TYPE_TREND_SYS );
//     style.m_dwFlags |= HDX_PRESENT;
//     }
//   else
//   if ( lstrcmpi( value, "CmActiveX1" ) == 0 )
//     {
//     style.AddFlag( TYPE_CHART_TAG );
//     style.m_dwFlags |= HDX_PRESENT;
//     }
//   }
// //
// void CTagOBJECT::Analiz( ANALIZ )
//   {
//   struct SLocal { char* name; LPFuncs func; };
//   _static SLocal list[] =
//     {
//     { "style"        , objectStyle         },
//     { "class"        , objectClass         },
//     { "id"           , objectID            },
//     { "hdxproperties", objectHdxProperties },
//     }InitLocal;
//   //
//   int n = 0;
//   while ( teg[n].name )
//     {
//     SLocal* find = FindLocal( teg[n].name );
//     //ASSERT( find );
//     if ( find != NULL )
//     (*find->func)(html,*this,teg[n].value);
//     n++;
//     }
//   //
//   }
// //
// void CTagOBJECT::Kerdyk ( KERDYK )
//   {
//   int nDsd = -1;
//   SBaseDSD* dsd = Dsd(html);
//   if ( dsd != NULL )
//     nDsd = dsd->m_nID;
//   //
//   CStyle& style = Style(html);
//   if ( style.GetFlag( TYPE_TREND_GRP ) )
//     m_pWnd = new CHTML_Trend( html, style.colorTxt, style.colorBkg, nDsd );
//   else
//   if ( style.GetFlag( TYPE_TREND_SYS ) )
//     m_pWnd = new CHTML_Trend( html, style.colorTxt, style.colorBkg, nDsd );
//   else
//   if ( style.GetFlag( TYPE_CHART_TAG ) )
//     m_pWnd = new CHTML_Chart( html );
//   }
// //
// void CTagOBJECT::Matrix( LOCALS )
//   {
//   CStyle& style = Style(html);
//   char* id = style.NameID( html );
//   if ( style.LocalD( hDC ) ) return;
//   if ( StopDebug( html, "pushbutton003" ) )
//     AAA();
//   //if ( ::IsMnemoStyle(MNEMO_EDIT) )
//   //  {
//   //  cross::RECT cross::RECT = Style(html).Fullcross::RECT();
//   //  COLORREF color = 0x800000;
//   //  CMyPen pen( hDC, 0x800000 );
//   //  pen.cross::RECT( cross::RECT );
//   //  }
//   }
// //
