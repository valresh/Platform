// #include "StdAfx.h"
// #include "HTML.h"
// //
// void CBorder::Parsers( char* value )
//   {
//   char* p = strchr( value, ' ' );
//   if ( p ) *p = 0;
//   //
//   if ( *value == '#' )
//     color = ColorDiez(value);
//   else
//   if ( lstrcmpi( value, "transparent" ) == 0 ) { trans = true;}
//   else
//   if ( lstrcmpi( value, "none" ) == 0 ) { eStyle = eBorderNone;}
//   else
//   if ( lstrcmpi( value, "inset" ) == 0 ) { eStyle = eBorderInset;}
//   else
//   if ( lstrcmpi( value, "outset" ) == 0 ) { eStyle = eBorderOutset;}
//   else
//     {
//     char* s = strstr( value, "px" );
//     if ( s )
//       {
//       *s = 0;
//       ASSERT( IsInt(value) );
//       width = atoi(value);
//       }
//     else
//     if ( lstrcmpi( value, "solid" ) == 0 ) mStyle |= PS_SOLID;
//     else
//     if ( lstrcmpi( value, "ridge" ) == 0 )
//     {
//         mStyle |= PS_SOLID;//Надо сделать правильно
//         eStyle = eBorderRidge;
//     }
//     else
//     if ( lstrcmpi( value, "groove" ) == 0 )
//     {
//         mStyle |= PS_SOLID;
//         eStyle = eBorderGroove;
//     }
//     else
//       {
//       color = ColorDiez(value);
//       }
//     }
//   if ( p ) Parsers( p+1 );
//   }
// //
// void CBorder::Padding( char* value, DWORD set, DWORD& flag )
//   {
//   flag |= set;
//   padding = Number( value );
//   ASSERT( lstrcmpi( value, "px" ) == 0 );
//   }

// //
// void CHdx::Border( HDC hDC )
//   {
//   if ( !(mL.width || mT.width || mR.width || mB.width) ) return;

//   cross::RECT rc = { int(mcross::RECT.X+0.5),
//               int(mcross::RECT.Y+0.5),
//               int(mcross::RECT.X+mcross::RECT.Width +0.5),
//               int(mcross::RECT.Y+mcross::RECT.Height+0.5) };
// //  if ( mL.width && !mL.trans ) { CMyPen pen( hDC, mL.color, mL.width ); pen.Vert( rc.left  , rc.top , rc.bottom );}
// //  if ( mT.width && !mT.trans ) { CMyPen pen( hDC, mT.color, mT.width ); pen.Horz( rc.top   , rc.left, rc.right  );}
// //  if ( mR.width && !mR.trans ) { CMyPen pen( hDC, mR.color, mR.width ); pen.Vert( rc.right , rc.top , rc.bottom );}
// //  if ( mB.width && !mB.trans ) { CMyPen pen( hDC, mB.color, mB.width ); pen.Horz( rc.bottom, rc.left, rc.right  );}
//   }

// void CBorder::Left  ( HDC hDC, cross::RECT& cross::RECT, bool bPress )
//   {
//   if ( eStyle == eBorderNone || trans ) return;
//   switch ( eStyle )
//     {
//     case eBorderInset :
//       break;
//     case eBorderOutset:
//       {
//       CMyPen pen( hDC, color, width, mStyle );
//       pen.Vert( cross::RECT.left+width/2, cross::RECT.top, cross::RECT.bottom );
//       }
//       break;
//     }
//   //cross::RECT.left += width;
//   }

// void CBorder::Right ( HDC hDC, cross::RECT& cross::RECT, bool bPress )
//   {
//   if ( eStyle == eBorderNone || trans ) return;
//   switch ( eStyle )
//     {
//     case eBorderInset :
//       break;
//     case eBorderOutset:
//       {
//       CMyPen pen( hDC, color, width, mStyle );
//       pen.Vert( cross::RECT.right-width/2, cross::RECT.top, cross::RECT.bottom );
//       }
//       break;
//     }
//   //cross::RECT.right -= width;
//   }

// void CBorder::Top   ( HDC hDC, cross::RECT& cross::RECT, bool bPress )
//   {
//   if ( eStyle == eBorderNone || trans ) return;
//   switch ( eStyle )
//     {
//     case eBorderInset :
//       break;
//     case eBorderOutset:
//       {
//       CMyPen pen( hDC, color, width, mStyle );
//       pen.Horz( cross::RECT.top+width/2, cross::RECT.left, cross::RECT.right );
//       }
//       break;
//     }
//   //cross::RECT.top += width;
//   }

// void CBorder::Bottom( HDC hDC, cross::RECT& cross::RECT, bool bPress )
//   {
//   if ( eStyle == eBorderNone || trans ) return;
//   switch ( eStyle )
//     {
//     case eBorderInset :
//       break;
//     case eBorderOutset:
//       {
//       CMyPen pen( hDC, color, width, mStyle );
//       pen.Horz( cross::RECT.bottom-width/2, cross::RECT.left, cross::RECT.right );
//       }
//       break;
//     }
//   //cross::RECT.bottom -= width;
//   }
