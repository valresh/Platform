// #include "StdAfx.h"
// #include "HTML.h"
// //
// inline void SetT(TRIVERTEX& val,COLORREF color)
//   {
//   val.Red = GetRValue(color)<<8;
//   val.Blue = GetBValue(color)<<8;
//   val.Green = GetGValue(color)<<8;
//   }
// //
// void CHdx::Round  ( HDC hDC, CHTML& html, cross::RECT& cross::RECT )
//   {
//   if ( m_nRoundness == 0 )
//   if ( !(m_dwFlags & HDX_ROUND) )
//     {
//     int w = cross::RECT.right-cross::RECT.left;
//     int h = cross::RECT.bottom-cross::RECT.top;
//     m_nRoundness = MulDiv( max(w,h), 1, 6 );
//     m_nRoundness = max(m_nRoundness,10);
//     }
//   //
//   switch ( m_nFillStyle )
//     {
//     case 0:
//       {
// 		  if (Version  == LG35_8_KF && html.m_strFile == "sysAlarmSummary" && m_nRoundness == 3)
// 		  {
//       CMyPen pen( hDC, m_clrFill, m_nLineWidth );
//       CMyBrush brush( hDC, m_clrFill, true );
//       pen.Round( cross::RECT, m_nRoundness );
// 		  } else{
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//       CMyBrush brush( hDC, m_clrFill, true );
//       pen.Round( cross::RECT, m_nRoundness );
// 		  }}
//     break;
//     case 1:
//       if(m_clrLine != 0xff000000)
//       {
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//       ::BeginPath( hDC );
//       pen.Round( cross::RECT, m_nRoundness );
//       ::EndPath( hDC );
//       ::StrokePath( hDC );
// 	  if ( Version == LG35_8_KF && m_eBorderStyle == eBorderSolid && html.bluecross::RECT)
// 		{
// 		ASSUNO
// 		{ CMyPen pen( hDC, 0xff0000 ); pen.cross::RECT( cross::RECT );}
// 		}
//       }
//     break;
//     case 2:
//       {
//       // Судя по 102D
//       if ( !(m_dwFlags & HDX_CLRLINE) ) m_dwFlags |= HDX_CLRLINE, m_clrLine = 0x000000;
//       if ( !(m_dwFlags & HDX_CLRFILL) ) m_dwFlags |= HDX_CLRFILL, m_clrFill = 0xffffff;
//       //
//       //if ( m_pGDI == NULL )
//       //  {
//       //  m_pGDI = new CFillRound( m_nGradientFillStyle==0, m_clrLine, m_nLineWidth, m_nRoundness );
//       //  if ( m_pGDI ) ((CFillRound*)m_pGDI)->Create( rc, m_clrFill, m_clrFillGrd );
//       //  }
//       //if ( m_pGDI != NULL )
//       //  m_pGDI->Paint( hDC, html, rc, m_dwFlags );
//       XFORM xf;
//       ::GetWorldTransform( hDC, &xf );
//       Matrix matrix( xf.eM11, xf.eM12, xf.eM21, xf.eM22, xf.eDx, xf.eDy );
//       Point p1( cross::RECT.left, cross::RECT.top );
//       Point p2( cross::RECT.right, cross::RECT.bottom );
//       Point p3( m_nRoundness, m_nRoundness );
//       matrix.TransformPoints( &p1 );
//       matrix.TransformPoints( &p2 );
//       {
//       Matrix matrix( xf.eM11, xf.eM12, xf.eM21, xf.eM22, 0, 0 );
//       matrix.TransformPoints( &p3 );
//       }
//       //
//       HRGN hRgn = ::CreateRoundcross::RECTRgn( p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y );
//       if ( hRgn != NULL )
//         {
//         DWORD clrFill = m_clrFill;
//         DWORD clrFillGrd = m_clrFillGrd;
//         TRIVERTEX vert[4];
//         SetT(vert[0],clrFillGrd );
//         SetT(vert[1],clrFill    );
//         SetT(vert[2],clrFill    );
//         SetT(vert[3],clrFillGrd );
//         //
//         if ( m_nGradientFillStyle==0 )
//           {
//           int center = (cross::RECT.left+cross::RECT.right)/2;
//           vert[1].x = center; vert[1].y = cross::RECT.bottom;
//           vert[2].x = center; vert[2].y = cross::RECT.top;
//           }
//         else
//           {
//           int center = (cross::RECT.top+cross::RECT.bottom)/2;
//           vert[1].x = cross::RECT.right; vert[1].y = center;
//           vert[2].x = cross::RECT.left ; vert[2].y = center;
//           }
//         vert[0].x = cross::RECT.left;
//         vert[0].y = cross::RECT.top;
//         vert[3].x = cross::RECT.right;
//         vert[3].y = cross::RECT.bottom;
//         //
//         ::SelectClipRgn( hDC, hRgn );
//         _static GRADIENT_cross::RECT  gcross::RECT = {0,1};
//         DWORD dwMode = ( m_nGradientFillStyle==0 )?GRADIENT_FILL_cross::RECT_H:GRADIENT_FILL_cross::RECT_V;
//         ::GradientFill( hDC, &vert[0], 2, &gcross::RECT, 1, dwMode );
//         ::GradientFill( hDC, &vert[2], 2, &gcross::RECT, 1, dwMode );
//         ::SelectClipRgn( hDC, NULL );
//         ::DeleteObject( hRgn );
//         }
//       //
//       bool bDrawLine = true;
//       if(Version == DOTF_PM)
//          {//пока так
//          if(mLineColorBlink && PulsTimer())
//            bDrawLine = false;
//          }
//       if (bDrawLine)
//       if ( m_dwFlags & HDX_CLRLINE )
//         {
//         CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//         ::BeginPath( hDC );
//           pen.Round( cross::RECT, m_nRoundness );
//         ::EndPath( hDC );
//         ::StrokePath( hDC );
//         }
//       }
//     break;
//     default: ASSUNO
//     };
//   }
// //
