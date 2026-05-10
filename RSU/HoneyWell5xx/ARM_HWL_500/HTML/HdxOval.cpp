// #include "StdAfx.h"
// #include "Tag.h"
// //
// void CHdx::Oval( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   switch ( m_nFillStyle )
//     {
//     case 0:
//     if ( m_clrFill != 0xff000000 )// != transparent
//       {
//       CMyBrush brush( hDC, m_clrFill,true);
// 	  CMyPen pen( hDC, m_clrFill);
//       pen.Ellipse( rc );
//       }
// 	DrawOvalWithPenFromLineStyle( hDC, html, rc );
//     break;
//     case 1:
// 			DrawOvalWithPenFromLineStyle( hDC, html, rc );
//     break;
//     case 2:
//       {
// 		  CMyBrush brush( hDC, m_clrFill,true);
// 		  CMyPen pen( hDC, m_clrFill);
// 		  pen.Ellipse( rc );
// 		  DrawOvalWithPenFromLineStyle( hDC, html, rc, true );
//       }
//     break;
//     default: ASSUNO
//     };
//   }

// void CHdx::DrawOvalWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc, bool gradient )
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
// 	 Gdiplus::cross::RECT cross::RECT(rc.left,rc.top,abs(rc.right-rc.left),abs(rc.bottom-rc.top));
// 	 graphics.DrawEllipse(&pen,cross::RECT);
// 	 if (gradient)
// 	 {
// 	   if ((m_nGradientFillStyle == 0 )||( m_nGradientFillStyle == 1 ))
// 	   {
//            Color clrFillGrd;  clrFillGrd.SetFromCOLORREF(m_clrFillGrd);
//            Color clrFill; clrFill.SetFromCOLORREF(m_clrFill);
// 		   Point pt1;
// 		   Point pt2;
// 		   if ( m_nGradientFillStyle == 1 )
// 		   {
// 			   pt1=Point(cross::RECT.Width, rc.top);
// 			   pt2=Point(cross::RECT.Width, rc.bottom);
// 		   }
// 		   else if ( m_nGradientFillStyle == 0 )
// 		   {
// 			   pt1=Point(rc.left, cross::RECT.Height);
// 			   pt2=Point(rc.right, cross::RECT.Height);
// 		   }
// 			LinearGradientBrush lgb(pt1,pt2,clrFillGrd, clrFill);
// 	   		REAL relativeIntensities[] = {0.0f, 1.0f, 0.0f};
// 			REAL relativePositions[]   = {0.0f, 0.5f, 1.0f};
// 			lgb.SetBlend(relativeIntensities, relativePositions, 3);
// 			graphics.FillEllipse(&lgb, cross::RECT);
// 	   }
// 	 }
// }
