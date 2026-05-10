// //#include "StdAfx.h"
// #include "HTML.h"
// #include "Tag.h"
// //
// #undef  STD_VAR
// #define STD_VAR CHTML& html, CTag& a, char* value
// typedef void (*LPFuncs)( STD_VAR );
// //
// void spanClass( STD_VAR )
// {
// }
// //
// void spanStyle( STD_VAR )
//   {
//   a.AnalizStyle( html, value );
//   }
// //
// void spanID( STD_VAR )
//   {
//   CStyle& style = a.Style(html);
//   style.NameID( html, value );
//   }
// //
// void spanTabIndex( STD_VAR )
//   {
// //  ASSERT( ::IsInt(value) );
// //  style.nTabIndex = atoi(value);
//   }
// //
// void spanTitle( STD_VAR )
//   {
//    CStyle& style = a.Style(html);
//    strcpy_s(style.m_szTitle, value);
//   }
// //
// void spanHdxProperties( STD_VAR )
//   {
//   a.AnalizHdx( html, value );
//   }
// //
// void spanHDX_LOCK( STD_VAR )
//   {
//   ASSERT( IsInt(value) );
//   int b = atoi(value);
//   ASSERT( b == -1 );
//   }
// //
// void spanOnClick( STD_VAR )
//   {
//   }
// //
// void spanOnPropertyChange   ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
// void spanLinkDisabled       ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
// //
// void spanGlobalscripts      ( STD_VAR ){/*Встречается в sysdtltpsdevctl_fp*/}
// void spanPrintValuePercent  ( STD_VAR ){/*Встречается в sysdtltpsdevctl_fp*/}
// void spanPrintFillColor     ( STD_VAR ){/*Встречается в sysdtltpsdevctl_fp*/}
// void spanPrintLevelFillColor( STD_VAR ){/*Встречается в sysdtltpsdevctl_fp*/}
// void spanPrintInverse       ( STD_VAR ){/*Встречается в sysdtltpsdevctl_fp*/}
// void spanPrintValue         ( STD_VAR ){/*Встречается в sysdtlpida_fp*/}
// //
// void AnalizSPAN( CTag* pTag, ANALIZ )
//   {
//   //
//   struct SLocal { char* name; LPFuncs func; };
//   _static SLocal list[] =
//     {
//     { "class"              , spanClass              },
//     { "style"              , spanStyle              },
//     { "title"              , spanTitle              },
//     { "id"                 , spanID                 },
//     { "tabIndex"           , spanTabIndex           },
//     { "hdxproperties"      , spanHdxProperties      },
//     { "HDX_LOCK"           , spanHDX_LOCK           },
//     { "onclick"            , spanOnClick            },
//     { "onpropertychange"   , spanOnPropertyChange   },
//     { "linkDisabled"       , spanLinkDisabled       },
//     { "globalscripts"      , spanGlobalscripts      },
//     { "printValuePercent"  , spanPrintValuePercent  },
//     { "printFillColor"     , spanPrintFillColor     },
//     { "printLevelFillColor", spanPrintLevelFillColor},
//     { "printInverse"       , spanPrintInverse       },
//     { "printValue"         , spanPrintValue         },
//     }InitLocal;
//   //
//   {
//   CStyle& style = pTag->Style(html);
//   style.m_clrFillLvl = 0x808000;
//   }
//   int n = 0;
//   while ( teg[n].name )
//     {
//     SLocal* find = FindLocal( teg[n].name );
//     ASSERT( find );
//     if ( find != NULL )
//     (*find->func)(html,*pTag,teg[n].value);
//     n++;
//     }
//   //
//   CStyle& style = pTag->Style(html);
//   char* id = style.NameID( html );
//   // Компенсация поворота
//   style.m_dwFlags |= HDX_PRESENT;
//   //
//   if ( style.mcross::RECT.X == 0 && style.mcross::RECT.Width == 0
//   &&   style.mcross::RECT.Y == 0 && style.mcross::RECT.Height == 0 )
//     {
//     if  ( pTag->m_pPar )
//       {
//       CStyle& par = pTag->m_pPar->Style(html);
//       style.mcross::RECT = par.mcross::RECT;
//       }
//     }
//   }
// //
