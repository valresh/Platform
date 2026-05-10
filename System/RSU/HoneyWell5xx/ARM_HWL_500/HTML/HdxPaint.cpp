// #include "StdAfx.h"
// #include "Tag.h"
// #include "Hdx.h"
// #include "HTML.h"
// //
// bool CHdx::LocalD( HDC hDC )
//   {
//   if ( m_dAngleOld != m_dAngleNew )
//       Angles();
//   //
//   if ( m_dwFlags & HDX_ROTATE )
//     ::ModifyWorldTransform( hDC, &m_XF, MWT_LEFTMULTIPLY );
//   //
//   if ( !(m_dwFlags & HDX_INITcross::RECT) )
//     {
//     m_dwFlags |= HDX_INITcross::RECT;
//     XFORM xf;
//     ::GetWorldTransform( hDC, &xf );
//     Matrix matrix( xf.eM11, xf.eM12, xf.eM21, xf.eM22, xf.eDx, xf.eDy );
//     PointF p1( mcross::RECT.X, mcross::RECT.Y );
// 	PointF p2( mcross::RECT.X+mcross::RECT.Width, mcross::RECT.Y+mcross::RECT.Height );
// 	if(Version != ITH_NAK)
// 	{
// 		matrix.TransformPoints( &p1 );
// 		matrix.TransformPoints( &p2 );
//     }
//     m_rcTruecross::RECT.left   = (int)( min( p1.X, p2.X ) + 0.5 );
//     m_rcTruecross::RECT.right  = (int)( max( p1.X, p2.X ) + 0.5 );
//     m_rcTruecross::RECT.top    = (int)( min( p1.Y, p2.Y ) + 0.5 );
//     m_rcTruecross::RECT.bottom = (int)( max( p1.Y, p2.Y ) + 0.5 );
//     return true;
//     }
//   Border( hDC );
//   return false;
//   }
// //
// void CHdx::PostsD( HDC hDC )
//   {
//   if ( m_dwFlags & HDX_ROTATE )
//     {
//     XFORM XF;
//     XF.eM11= m_XF.eM11;
//     XF.eM22= m_XF.eM22;
//     XF.eM21= m_XF.eM12;
//     XF.eM12= m_XF.eM21;
//     XF.eDx = 0;
//     XF.eDy = 0;
//     ::ModifyWorldTransform( hDC, &XF, MWT_LEFTMULTIPLY );
//     //
//     XF.eM11= 1.0;
//     XF.eM22= 1.0;
//     XF.eM21= 0.0;
//     XF.eM12= 0.0;
//     XF.eDx =-m_XF.eDx;
//     XF.eDy =-m_XF.eDy;
//     ::ModifyWorldTransform( hDC, &XF, MWT_LEFTMULTIPLY );
//     }
//   }
// //
// void CHdx::Bezier( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   ASSERT( m_nFillStyle == 0 || m_nFillStyle == 1 || m_nFillStyle == 2 );
//   ASSERT( (m_dwFlags & HDX_POINTS ) != 0 );
// //  ASSERT( m_dwFlags & HDX_CLRLINE ); gar_gas
//   //
//   DrawBezierWithPenFromLineStyle( hDC, html, rc );
//   }
// //
// void CHdx::Arc( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   ASSERT( m_nFillStyle == 0 || m_nFillStyle == 1 || m_nFillStyle == 2 );
//   CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//   //
//   int h = rc.bottom-rc.top;
//   int w = rc.right-rc.left;
//   switch ( m_nQuadrant )
//     {
//     case 0: ::Arc( hDC, rc.left, rc.top, rc.right+w, rc.bottom+h, rc.right, rc.top, rc.left, rc.bottom ); break;
//     case 1: ::Arc( hDC, rc.left-w, rc.top, rc.right, rc.bottom+h, rc.right, rc.bottom, rc.left, rc.top ); break;
//     case 2: ::Arc( hDC, rc.left-w, rc.top-h, rc.right, rc.bottom, rc.left, rc.bottom, rc.right, rc.top ); break;
//     case 3: ::Arc( hDC, rc.left, rc.top-h, rc.right+w, rc.bottom, rc.left, rc.top, rc.right, rc.bottom ); break;
//     default:
//       {
//       CMyBrush brush( hDC, m_clrFill, true );
//       pen.Rc( rc );
//       }
//     }
//   //
//   }
// //
// void CHdx::Level( HDC hDC, CHTML& html, cross::RECT& rc, double dLevel, COLORREF color )
//   {
//   ASSERT( m_nFillStyle == 0 || m_nFillStyle == 8 );
//   if ( m_clrFill != 0xff000000 )
//     cross::RECT0( hDC, rc, m_clrFill, false );
//   if (IsNaN(dLevel)) return;
//   //
//   cross::RECT cross::RECT = rc;
//   if ( fillDicross::RECTion == 0 )
//     {
//     int h = int(dLevel*(rc.bottom-rc.top));
//     cross::RECT.top = rc.bottom-h;
//     if ( m_dwFlags & HDX_RANGEORIGIN && m_dwFlags & HDX_RANGELOHI )
//       {
//       if(IsNaN(m_dRangeOrigin))
//          return;
//       double dMin = m_dRangeLo;
//       double dMax = m_dRangeHi;
//       double dRange = ( m_dRangeOrigin - dMin ) / ( dMax - dMin );
//       int r = int(dRange*(rc.bottom-rc.top));
//       r = rc.bottom-r;
//       h = rc.bottom-h;
//       cross::RECT.top    = min( r, h );
//       cross::RECT.bottom = max( r, h );
//       }
//     }
//   else
//   if ( fillDicross::RECTion == 1 )
//     {
//     int w = int(dLevel*(rc.right-rc.left));
//     cross::RECT.right = rc.left+w;
//     }
//   else
//     {
//     ASSERT(0);
//     }
//   if ( color != 0xff000000 )
//     {
//     if ( m_nFillStyle == 0 )
//       {
//       if ( cross::RECT.top < cross::RECT.bottom )
//       if ( cross::RECT.left < cross::RECT.right )
//         {
//         CMyBrush brush( hDC, color );
//         brush.cross::RECT( cross::RECT );
//         }
//       }
//     else
//     if ( m_nFillStyle == 8 )
//       {
//       XFORM xf;
//       ::GetWorldTransform( hDC, &xf );
//       Matrix matrix( xf.eM11, xf.eM12, xf.eM21, xf.eM22, xf.eDx, xf.eDy );
//       Point p1( rc.left, rc.top );
//       Point p2( rc.right, rc.bottom );
//       matrix.TransformPoints( &p1 );
//       matrix.TransformPoints( &p2 );
//       HRGN hRgn = ::Createcross::RECTRgn( p1.X, p1.Y, p2.X, p2.Y );
//       if ( hRgn != NULL )
//         {
//         ::SelectClipRgn( hDC, hRgn );
//         if ( fillDicross::RECTion == 0 )
//          {
//           int w = cross::RECT.right-cross::RECT.left;
//           switch(m_PointerDicross::RECTion)
//           {
//           default:
//           case    pdRight:
//               {
//           POINT pt[3] = { {cross::RECT.right,cross::RECT.top}, {cross::RECT.left,cross::RECT.top-w/2}, {cross::RECT.left,cross::RECT.top+w/2}};
//           CMyRgn rgn( hDC, color );
//           rgn.Points( pt, 3, 1 );
//               }
//               break;
//           case    pdLeft:
//               {
//           POINT pt[3] = { {cross::RECT.right, cross::RECT.top}, {cross::RECT.left, cross::RECT.top+w/2}, {cross::RECT.right, cross::RECT.top+w}};
//           CMyRgn rgn( hDC, color );
//           rgn.Points( pt, 3, 1 );
//               }
//               break;
//           }
//          }
//         else
//         if ( fillDicross::RECTion == 1 )
//           {
//           int h = cross::RECT.bottom-cross::RECT.top;
//           POINT pt[3] = { {cross::RECT.right,cross::RECT.bottom}, {cross::RECT.right-h/2,cross::RECT.top}, {cross::RECT.right+h/2,cross::RECT.top}};
//           CMyRgn rgn( hDC, color );
//           rgn.Points( pt, 3, 1 );
//           }
//         else
//           {
//           ASSERT(0);
//           }
//         ::SelectClipRgn( hDC, NULL );
//         ::DeleteObject( hRgn );
//         }
//       }
//     else
//       {
//       ASSUNO
//       }
//     }
//   //
//   if ( m_eBorderStyle == eBorderInset )
//     {
// //    if ( m_nBorderWidth == 0 )
//       if ( m_clrLine != 0xff000000 )
//       ::DrawEdge( hDC, &rc, BDR_SUNKEN, BF_cross::RECT|BF_SOFT );
// //    else
//       {
//  //     { CMyPen pen( hDC, 0x404040, int(m_nBorderWidth), PS_ENDCAP_SQUARE|PS_JOIN_BEVEL ); pen.LU( rc );}
//  //     { CMyPen pen( hDC, 0xc0c0c0, int(m_nBorderWidth), PS_ENDCAP_SQUARE|PS_JOIN_BEVEL ); pen.RB( rc );}
//       }
//     }
//   else
//   if ( m_eBorderStyle == eBorderOutset )
//     {
//     { CMyPen pen( hDC, 0xc0c0c0, PS_ENDCAP_SQUARE|PS_JOIN_BEVEL ); pen.LU( rc );}
//     { CMyPen pen( hDC, 0x696969, PS_ENDCAP_SQUARE|PS_JOIN_BEVEL ); pen.RB( rc );}
//     }
//   //
//   }
// //
// void CHdx::cross::RECT0( HDC hDC, cross::RECT& rc, COLORREF fill, bool bLine )
//   {
//   if ( (m_dwFlags & HDX_CLRLINE) && (m_dwFlags & HDX_CLRFILL) )
//     {
//     CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//     CMyBrush brush( hDC, fill, true );
//     pen.Rc( rc );
//     }
//   else
//   if ( m_dwFlags & HDX_CLRFILL )
//     {
//     CMyBrush brush( hDC, fill );
//     brush.cross::RECT( rc );
//     }
//   else
//   if ( m_dwFlags & HDX_CLRLINE )
//     {
//     if ( bLine )
//       {
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//       pen.cross::RECT( rc );
//       }
//     }
//   else
//     {
//     //????? if ( bLine )
//     //?????   ASSERT(0);
//     }
//   }

//   void CHdx::AnalizHDX(CHTML &html, char *ptr)
//   {

//   }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CHdx::Gradient( cross::RECT& cross::RECT, TRIVERTEX* vert )//COLORREF light, COLORREF dark )
//   {
//   SetRGB(vert[0],m_clrFillGrd );
//   SetRGB(vert[1],m_clrFill    );
//   SetRGB(vert[2],m_clrFill    );
//   SetRGB(vert[3],m_clrFillGrd );
//   //
//   if ( m_nGradientFillStyle == 0 )
//     {
//     int center = (cross::RECT.left+cross::RECT.right)/2;
//     vert[1].x = center; vert[1].y = cross::RECT.bottom;
//     vert[2].x = center; vert[2].y = cross::RECT.top;
//     }
//   else
//     {
//     int center = (cross::RECT.top+cross::RECT.bottom)/2;
//     vert[1].x = cross::RECT.right; vert[1].y = center;
//     vert[2].x = cross::RECT.left ; vert[2].y = center;
//     }
//   vert[0].x = cross::RECT.left;
//   vert[0].y = cross::RECT.top;
//   vert[3].x = cross::RECT.right;
//   vert[3].y = cross::RECT.bottom;
//   //
//   }

// void CHdx::Gradient( HDC hDC, TRIVERTEX* vert )
//   {
//   _static GRADIENT_cross::RECT  gcross::RECT = {0,1};
//   DWORD dwMode = ( m_nGradientFillStyle == 0 )?GRADIENT_FILL_cross::RECT_H:GRADIENT_FILL_cross::RECT_V;
//   ::GradientFill( hDC, &vert[0], 2, &gcross::RECT, 1, dwMode );
//   ::GradientFill( hDC, &vert[2], 2, &gcross::RECT, 1, dwMode );
//   }

// void CStyle::BadValue( HDC hDC, LPCTSTR out )
//   {
//   if ( out == NULL ) return;
//   //
//   cross::RECT cross::RECT = Fullcross::RECT();
//   CMyBrush brush( hDC, 0xff );
//   brush.cross::RECT( cross::RECT );
//   ::Inflatecross::RECT( &cross::RECT, -2, 0 );
//   int dl = lstrlen(out);
//   int ld = dwMainTimer % dl;
//   char txt[128];
//   lstrcpy( txt, out+ld );
//   lstrcat( txt, out );
//   CMyFont font( hDC, m_hFont, 0xffffff );
//   ::DrawText( hDC, txt, lstrlen(txt), &cross::RECT, DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER );
//   }

// void CHdx::DrawBezierWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc )
// {
// 	if ( m_nLineStyle == eLineStyleNone )
// 	  return;
// 	Gdiplus::Color color;
//     color.SetFromCOLORREF( m_clrLine );
// 	Gdiplus::Pen pen( color, (REAL)m_nLineWidth);
// 	ChangePenFromLineStyle(pen);
// 	 Graphics graphics( hDC );
//      graphics.SetSmoothingMode(SmoothingModeHighQuality);
//      graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
//      graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
// 	 POINT* ptArray = PT(html);
// 	 if (m_nPointPT==4)
// 	 {
// 		 Arrows( hDC, ptArray );
// 		 Gdiplus::Point ptArrayGDI[4]=
// 		 {	Gdiplus::Point(ptArray[0].x,ptArray[0].y),
// 			Gdiplus::Point(ptArray[1].x,ptArray[1].y),
// 			Gdiplus::Point(ptArray[2].x,ptArray[2].y),
// 			Gdiplus::Point(ptArray[3].x,ptArray[3].y)
// 		 };
// 		 graphics.DrawBezier( &pen, ptArrayGDI[0], ptArrayGDI[1],ptArrayGDI[2],ptArrayGDI[3]);
// 	 }
// }
