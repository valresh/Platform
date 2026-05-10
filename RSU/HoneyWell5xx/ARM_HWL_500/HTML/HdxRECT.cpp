// #include "StdAfx.h"
// #include "Tag.h"
// //
// void CHdx::cross::RECT( HDC hDC, CHTML& html, cross::RECT& rc, bool bOther )
//   {
//   switch ( m_nFillStyle )
//     {
//     case 0:
//     if ( m_clrLine != 0xff000000 )// != transparent
//       {
// 		  if ( m_clrFill != 0xff000000 && (m_dwFlags & HDX_CLRFILL) )// != transparent
// 			{
// 				  CMyBrush brush( hDC, m_clrFill );
// 				  brush.cross::RECT( rc );
// 			}
// 		  Drawcross::RECTWithPenFromLineStyle( hDC, html, rc );
// 	  }
//       break;
//     case -2:// От TextArea
//     if ( m_clrFill != 0xff000000 )
//       {
// 		  CMyBrush brush( hDC, m_clrFill );
// 		  brush.cross::RECT( rc );
//       }
//     break;
//     case 1:
//     if( m_bClrFillCanChangedByScript)
//       if ( m_clrFill != 0xff000000 )
//       {
// 		  CMyBrush brush( hDC, m_clrFill );
// 		  brush.cross::RECT( rc );
//       }
//     if ( m_clrLine != 0xff000000 )
// 		Drawcross::RECTWithPenFromLineStyle( hDC, html, rc );
//     break;
//     case 2:
//       {
// 		  TRIVERTEX vert[4];
// 		  Gradient( rc , vert );
// 		  Gradient( hDC, vert );
// 		  if ( m_dwFlags & HDX_CLRLINE )
// 				Drawcross::RECTWithPenFromLineStyle( hDC, html, rc );
//       }
//     break;
//     default: ASSUNO;
//     };
//   //
//   if(bOther && m_nFillStyle==-1)
//   {
//     if ( m_nBorderWidth <= 0 )
//       return;
//     if ( m_eBorderStyle == eBorderInset )
//     {
//         CMyPen pen( hDC, m_clrLine, (int)m_nBorderWidth );
//         pen.LU( rc );
//         pen.RB( rc );
//     }
//     return;
//   }
//   //
//   if ( m_eBorderStyle == eBorderInset )
//     {
//     if ( mL.trans && mT.trans )
//       {}
//     else
//     if ( !mL.trans || !mT.trans )
//     { CMyPen pen( hDC, 0x404040 ); pen.LU( rc );}
//     else
//       ASSUNO;
//     //
//     if ( mR.trans && mB.trans )
//       {}
//     else
//     if ( !mR.trans || !mB.trans )
//     { CMyPen pen( hDC, 0xffffff ); pen.RB( rc );}
//     else
//       ASSUNO;
//     }
//   else
//   if ( m_eBorderStyle == eBorderOutset )
//     {
//     ASSUNO
//     { CMyPen pen( hDC, 0xc0c0c0 ); pen.LU( rc );}
//     { CMyPen pen( hDC, 0x696969 ); pen.RB( rc );}
//     }
//   //
//   if ( (m_dwFlagObj & STYLE_PAD_L) && mL.padding != 0 )
//   if ( (m_dwFlagObj & STYLE_PAD_R) && mR.padding != 0 )
//   if ( (m_dwFlagObj & STYLE_PAD_T) && mT.padding != 0 )
//   if ( (m_dwFlagObj & STYLE_PAD_B) && mB.padding != 0 )
//     {
//     { CMyPen pen( hDC, 0x303030 ); pen.LU( rc ); }
//     { CMyPen pen( hDC, 0xb0b0b0 ); pen.RB( rc ); }
//     }
//   }
// //
// void CHdx::Drawcross::RECTWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc )
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
// 	 graphics.Drawcross::RECTangle(&pen,cross::RECT);
// }
