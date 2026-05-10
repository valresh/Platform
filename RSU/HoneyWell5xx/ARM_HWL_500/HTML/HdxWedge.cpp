// #include "StdAfx.h"
// #include "Tag.h"
// //
// void CHdx::Wedge( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   if ( m_nPointPT != 0 )
//     {
//     ASSUNO
//     return;
//     }
//   Arc( hDC, html, rc );
//   int h = rc.bottom-rc.top;
//   int w = rc.right-rc.left;
//   //
//   switch ( m_nFillStyle )
//     {
//     case 0:
//     if ( m_clrFill != 0xff000000 )
//       {
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//       CMyBrush brush( hDC, m_clrFill, true );
//       switch ( m_nQuadrant )
//         {
//         case 0: ::Pie( hDC, rc.left, rc.top, rc.right+w, rc.bottom+h, rc.right, rc.top, rc.left, rc.bottom ); break;
//         case 1: ::Pie( hDC, rc.left-w, rc.top, rc.right, rc.bottom+h, rc.right, rc.bottom, rc.left, rc.top ); break;
//         case 2: ::Pie( hDC, rc.left-w, rc.top-h, rc.right, rc.bottom, rc.left, rc.bottom, rc.right, rc.top ); break;
//         case 3: ::Pie( hDC, rc.left, rc.top-h, rc.right+w, rc.bottom, rc.left, rc.top, rc.right, rc.bottom ); break;
//         default:
//           {
//           CMyBrush brush( hDC, m_clrFill, true );
//           pen.Rc( rc );
//           }
//         }
//       }
//     break;
//     case 1:
//       {
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//       switch ( m_nQuadrant )
//         {
//         case 0: ::Arc( hDC, rc.left, rc.top, rc.right+w, rc.bottom+h, rc.right, rc.top, rc.left, rc.bottom ); break;
//         case 1: ::Arc( hDC, rc.left-w, rc.top, rc.right, rc.bottom+h, rc.right, rc.bottom, rc.left, rc.top ); break;
//         case 2: ::Arc( hDC, rc.left-w, rc.top-h, rc.right, rc.bottom, rc.left, rc.bottom, rc.right, rc.top ); break;
//         case 3: ::Arc( hDC, rc.left, rc.top-h, rc.right+w, rc.bottom, rc.left, rc.top, rc.right, rc.bottom ); break;
//         default:
//           {
//           CMyBrush brush( hDC, m_clrFill, true );
//           pen.Rc( rc );
//           }
//         }
//       }
//     break;
//     case 2:
//       {
//       ASSUNO
//       CMyPen pen( hDC, m_clrFill, 0 );
//       CMyBrush brush( hDC, m_clrFill, true );
//       pen.Ellipse( rc );
//       }
//     break;
//     default: ASSUNO
//     };
//   }
