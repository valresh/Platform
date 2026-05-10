// #include "StdAfx.h"
// #include "Tag.h"
// #include "Hdx.h"
// #include "HTML.h"
// //
// void CHdx::Polygon( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   const int MAXPOINTS = 256;
//   ASSERT( (m_dwFlags & HDX_POINTS ) != 0 );
//   //
//   switch ( m_nFillStyle )
//     {
//     case 0:
//     if ( m_clrFill != 0xff000000 )
//       {
// 		  if (this->mLevelFilled)
// 		  {
// 			  Graphics g(hDC);

// 			  PointF points[MAXPOINTS];
// 			  unsigned char types[MAXPOINTS];

// 			  //заполняем массив точек
// 			  POINT* pt = PT(html);
// 			  for ( int n = 0; n < m_nPointPT; n++ )
// 			  {
// 				  PointF p(pt[n].x, pt[n].y);
// 				  points[n] = p;
// 				  if (n == 0) types[n] = PathPointTypeStart;
// 				  else types[n] = PathPointTypeLine;
// 			  }
// 			  // создаем матрицу преобразования
// 			  Gdiplus::Matrix matrix( this->m_XF.eM11, this->m_XF.eM12, this->m_XF.eM21, this->m_XF.eM22, this->m_XF.eDx, this->m_XF.eDy );

// 			  // создаем полигон
// 			  GraphicsPath path( points, types, m_nPointPT, FillModeAlternate );
// 			  if ( this->m_XF.eM11!=0 && this->m_XF.eM12!=0 && this->m_XF.eM21!=0 && this->m_XF.eM22!=0 )
// 			  {
// 				  // трансформацию выполняем здесь
// 				  path.Transform(&matrix);
// 				  // и сбрасываем флаг поворота, чтобы поворот не выполнился в дальнейшем
// 				  this->m_dwFlags &= ~HDX_ROTATE;
// 			  }

// 			  // прямоугольник, в который вписан полигон
// 			  Gdiplus::cross::RECT bounds;
// 			  path.GetBounds(&bounds, NULL, NULL);

// 			  double dv = 0;
// 			  if (m_dRangeHi != m_dRangeLo) dv = (m_dValue - m_dRangeLo)/(m_dRangeHi - m_dRangeLo);

// 			  // определяем два прямоугольника, которые будут заливаться разными цветами - верхний и нижний
// 			  Gdiplus::cross::RECT uppercross::RECT(bounds.X, bounds.Y, bounds.Width, bounds.Height * (1 - dv));
// 			  Gdiplus::cross::RECT lowercross::RECT(bounds.X, bounds.Y + bounds.Height * (1 - dv), bounds.Width,  bounds.Height * dv);
			  
// 			  // создаем два одинаковых региона на основе данного полигона - верхний и нижний
// 			  Region upper(&path);
// 			  Region lower(&path);

// 			  // каждый регион пересекаем своим прямоугольником
// 			  upper.Intersect(uppercross::RECT);
// 			  lower.Intersect(lowercross::RECT);

// 			  // заливаем регионы
// 			  Color fill;
// 			  fill.SetFromCOLORREF(m_clrFillEmpty);
// 			  SolidBrush upperBrush(fill);
// 			  g.FillRegion(&upperBrush, &upper);

// 			  fill.SetFromCOLORREF(m_clrFill);
// 			  SolidBrush lowerBrush(fill);
// 			  g.FillRegion(&lowerBrush, &lower);

//   			  // рисуем границу
// 			  Color line;
// 			  line.SetFromCOLORREF(m_clrLine);
// 			  Pen pen(line, m_nLineWidth);
// 			  g.DrawPath(&pen, &path);
// 		  }
// 		  else
// 		  {
// 			  CMyPen pen( hDC, m_clrLine, m_nLineWidth );
// 			  CMyBrush brush( hDC, m_clrFill, true );
// 			  pen.Points( PT(html), m_nPointPT, 1 );
// 		  }
//       }
//     break;
//     default:
//     case 1:
// 		{
// 			Graphics g(hDC);
			
// 			PointF points[MAXPOINTS];
// 			unsigned char types[MAXPOINTS];
			
// 			POINT* pt = PT(html);
// 			for ( int n = 0; n < m_nPointPT; n++ )
// 			{
// 				PointF p(pt[n].x, pt[n].y);
// 				points[n] = p;
// 				if (n == 0) types[n] = PathPointTypeStart;
// 				else types[n] = PathPointTypeLine;
// 			}
			
// 			GraphicsPath path( points, types, m_nPointPT, FillModeAlternate );
// 			path.CloseAllFigures();
			  
// 			Color line;
// 			line.SetFromCOLORREF(m_clrLine);
// 			Pen pen(line, m_nLineWidth);
// 			if (m_clrFill != 0xff000000)
// 			{
// 				Color fill; fill.SetFromCOLORREF(m_clrFill);
// 				SolidBrush brush(fill);
// 				g.FillPath(&brush, &path);
// 			}
// 			g.DrawPath(&pen, &path);
// 		}
//     break;
//     case 2:
//     if ( m_clrFill != 0xff000000 )
//       Polygon2( hDC, html, rc );
//     break;
//     };
//   }
// //
// void CHdx::Polygon2( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   TRIVERTEX vert[4];
//   Gradient( rc , vert );
//   LPPOINT pt = PT(html);
//   LPPOINT m_PT = pt+m_nPointPT+1;
//   //
//   XFORM xf;
//   ::GetWorldTransform( hDC, &xf );
//   Matrix matrix( xf.eM11, xf.eM12, xf.eM21, xf.eM22, xf.eDx, xf.eDy );
//   for ( int n = 0; n < m_nPointPT; n++ )
//     {
//     Point p1( pt[n].x, pt[n].y );
//     matrix.TransformPoints( &p1 );
//     m_PT[n].x = p1.X;
//     m_PT[n].y = p1.Y;
//     }
//   //
//   HRGN hRgn = ::CreatePolyPolygonRgn( m_PT, &m_nPointPT, 1, ALTERNATE );
//   if ( hRgn != NULL )
//     {
//     ::SelectClipRgn( hDC, hRgn );
//     Gradient( hDC, vert );
//     ::SelectClipRgn( hDC, NULL );
//     ::DeleteObject( hRgn );
//     }
//   if ( m_dwFlags & HDX_CLRLINE )
//     {
//     CMyPen pen( hDC, m_clrLine, m_nLineWidth );
//     ::Polyline( hDC, pt, m_nPointPT );
//     }
//   }
// //
