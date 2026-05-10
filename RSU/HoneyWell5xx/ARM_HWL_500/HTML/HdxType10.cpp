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
// void CHdx::Type10( HDC hDC, CHTML& html, cross::RECT& cross::RECT )
//   {
//   ASSERT( (m_dwFlags & HDX_POINTS ) != 0 );
//   int nLineType=PS_DOT;
//   if (m_nLineStyle==eLineStyleSolid)
// 	 nLineType=PS_SOLID;
//   switch ( m_nFillStyle )
//     {
//     case 0:
//     if ( m_clrFill != 0xff000000 )
//       {
// 	  const int MAXPOINTS = 256;
// 	  Graphics g(hDC);

// 	  PointF points[MAXPOINTS];
// 	  unsigned char types[MAXPOINTS];
// 	  int cnt = 0;

// 	  POINT* pt = PT(html);
//       BYTE* wt = (BYTE*)(pt + m_nPointPT+1);
// 	  int* C = html.Int(m_nPlace10);
//       int* T = C + m_nPoint10;
// 	  for ( int n = 0; n < m_nPoint10; n++ )
// 	  {
// 		  for (int k = 0; k < C[n]; k++)
// 		  {
// 			  ASSERT( cnt < MAXPOINTS );
// 			  PointF p(pt[k].x, pt[k].y);
// 			  points[cnt] = p;
// 			  switch(wt[k])
// 			  {
// 			  case PT_CLOSEFIGURE: types[cnt] = PathPointTypeCloseSubpath; break;
// 			  case PT_LINETO: types[cnt] = PathPointTypeLine; break;
// 			  case PT_MOVETO: types[cnt] = PathPointTypeStart; break;
// 			  case PT_BEZIERTO: types[cnt] = PathPointTypeBezier; break;
// 			  }
// 			  cnt++;
// 		  }
// 		  if ((T[n] & 0x10) == 0x10)
// 			types[C[n]-1] |= PathPointTypeCloseSubpath;
// 		  pt += C[n];
// 		  wt += C[n];
// 	  }
// 	  GraphicsPath path( points, types, cnt, FillModeAlternate );
// 	  Color fill;
// 	  fill.SetFromCOLORREF(m_clrFill);
// 	  SolidBrush blackBrush(fill);
// 	  g.FillPath(&blackBrush, &path);
// 	  Color line;
// 	  line.SetFromCOLORREF(m_clrLine);
// 	  Pen pen(line, m_nLineWidth);
// 	  g.DrawPath(&pen, &path);
// 	}
//     break;
//     case 1:
//       {
//       POINT* pt = PT(html);
//       POINT* wt = pt + m_nPointPT+1;
//       //
//       int* C = html.Int(m_nPlace10);
//       int* T = C + m_nPoint10;
//       //
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth, nLineType );
//       CMyBrush brush( hDC, m_clrFill, true );
//       for ( int n = 0; n < m_nPoint10; n++ )
//         {
//         ::BeginPath( hDC );
//         pen.Points( pt, C[n], T[n] & 0xf );
//         if (T[n] & 0xf0) ::CloseFigure( hDC );
//         ::EndPath( hDC );
//         ::StrokePath( hDC );
//         pt += C[n];
//         }
//       }
//     break;
//     case 2:
//       {
//       POINT* pt0 = PT(html);
//       BYTE*  bt0 = (BYTE*)(pt0 + m_nPointPT+1);
//       _static POINT* pt;
//       _static BYTE * bt;
//       _static int    tt;
//       if ( tt < m_nPointPT )
//         {
//         tt = m_nPointPT+10;
//         ::BuffFree( pt ); pt = (POINT*)::BuffAlloc( tt*8, false );
//         ::BuffFree( bt ); bt = (BYTE *)::BuffAlloc( tt  , false );
//         }
//       memcpy( pt, pt0, 8*m_nPointPT );
//       memcpy( bt, bt0,   m_nPointPT );
//       //
//       if(Version == DOTF_PM)
//          {//пока так
//          if(mLineColorBlink && PulsTimer())
//             nLineType = PS_NULL;
//          }
//       CMyPen pen( hDC, m_clrLine, m_nLineWidth, nLineType );
//       ::BeginPath( hDC );
//       ::PolyDraw( hDC, pt, bt, m_nPointPT );
//       ::EndPath( hDC );
//       HRGN hRgn = ::PathToRegion( hDC );
//       if ( hRgn != NULL )
//         {
//         TRIVERTEX vert[4];
//         SetT(vert[0],m_clrFillGrd );
//         SetT(vert[1],m_clrFill    );
//         SetT(vert[2],m_clrFill    );
//         SetT(vert[3],m_clrFillGrd );
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

//         ::SelectClipRgn( hDC, hRgn );
//         _static GRADIENT_cross::RECT  gcross::RECT = {0,1};
//         DWORD dwMode = ( m_nGradientFillStyle==0 )?GRADIENT_FILL_cross::RECT_H:GRADIENT_FILL_cross::RECT_V;
//         ::GradientFill( hDC, &vert[0], 2, &gcross::RECT, 1, dwMode );
//         ::GradientFill( hDC, &vert[2], 2, &gcross::RECT, 1, dwMode );
//         ::SelectClipRgn( hDC, NULL );
//         //if ( dwFlags & HDX_CLRLINE )
//         ::PolyDraw( hDC, pt, bt, m_nPointPT );
//         ::DeleteObject( hRgn );
//         }
//       }
//     break;
//     default: ASSUNO
//     };
//   }
// //
