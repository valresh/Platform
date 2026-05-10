// #include "StdAfx.h"
// #include "HTML.h"
// //
// bool CHdx::EqBorder()
//   {
//   if ( memcmp( &mL, &mR, sizeof(mL) ) ) return false;
//   if ( memcmp( &mL, &mT, sizeof(mL) ) ) return false;
//   if ( memcmp( &mL, &mB, sizeof(mL) ) ) return false;
//   return true;
//   }
// //
// bool CHdx::IsEq (EBorderStyle e)
//   {
//   return ( mT.eStyle == e && mL.eStyle == e && mR.eStyle == e && mB.eStyle == e );
//   }
// //
// bool CHdx::IsNeq(EBorderStyle e)
//   {
//   return ( mT.eStyle != e && mL.eStyle != e && mR.eStyle != e && mB.eStyle != e );
//   }
// //
// void CHdx::Button( HDC hDC, cross::RECT& rc, bool bPress )
//   {
//   ASSERT( m_nFillStyle == 0 );
//   if ( m_clrFill != -1 )
//     {
//     CMyBrush brush( hDC, m_clrFill );
//     brush.cross::RECT( rc );
//     }
//   else
//     {
//     AAA();
//     }
// //  cross::RECT0( hDC, rc, m_clrFill, false );
//   if ( EqBorder() )
//   switch ( mL.eStyle )
//     {
//     case eBorderUnk:
//       {
//       if ( bPress )
//       ::DrawEdge( hDC, &rc, BDR_SUNKEN, BF_cross::RECT|BF_SOFT );
//       else
//       ::DrawEdge( hDC, &rc, BDR_RAISED, BF_cross::RECT );
//       }
//       return;
//     case eBorderOutset:
//       {
//       cross::RECT cross::RECT = rc;
//       //::Inflatecross::RECT( &cross::RECT, -mL.width/2, -mL.width/2 );
//       CMyPen pen( hDC, mL.color, mL.width, mL.mStyle );
//       pen.cross::RECT( cross::RECT );
//       }
//     return;
//     case eBorderNone:
//     return;
//     }
//   //
//   ASSUNO
//   //
//   cross::RECT rc0 = rc;
//   cross::RECT rc1 = rc; ::Inflatecross::RECT( &rc1, -1, -1 );
//   //
//   mL.Left  ( hDC, rc, bPress );
//   mR.Right ( hDC, rc, bPress );
//   mT.Top   ( hDC, rc, bPress );
//   mB.Bottom( hDC, rc, bPress );
//   //
//   if ( IsEq(eBorderNone) )
//     {
//     }
//   else
//   if ( IsEq(eBorderUnk) )
//     {
//     }
//   else
//   if ( IsNeq(eBorderOutset) )
//     {
//     if ( bPress )
//       {
//       //
//       COLORREF clrL0 = M_12(mL.color);
//       COLORREF clrT0 = M_12(mT.color);
//       COLORREF clrR0 =      mR.color;
//       COLORREF clrB0 =      mB.color;
//       //
//       COLORREF clrL1 = M_34(mL.color);
//       COLORREF clrT1 = M_34(mT.color);
//       COLORREF clrR1 = M_14(mR.color);
//       COLORREF clrB1 = M_14(mB.color);
//       //
//       { CMyPen pen( hDC, clrL0 ); pen.Vert( rc0.left  , rc0.top, rc0.bottom );}
//       { CMyPen pen( hDC, clrT0 ); pen.Horz( rc0.top   , rc0.left, rc0.right );}
//       { CMyPen pen( hDC, clrR0 ); pen.Vert( rc0.right , rc0.top, rc0.bottom );}
//       { CMyPen pen( hDC, clrB0 ); pen.Horz( rc0.bottom, rc0.left, rc0.right );}
//       //
//       { CMyPen pen( hDC, clrL1 ); pen.Vert( rc1.left  , rc1.top, rc1.bottom );}
//       { CMyPen pen( hDC, clrT1 ); pen.Horz( rc1.top   , rc1.left, rc1.right );}
//       { CMyPen pen( hDC, clrR1 ); pen.Vert( rc1.right , rc1.top, rc1.bottom );}
//       { CMyPen pen( hDC, clrB1 ); pen.Horz( rc1.bottom, rc1.left, rc1.right );}
//       //
//       }
//     else
//       {
//       //
//       COLORREF clrL0 =      mL.color;
//       COLORREF clrT0 =      mT.color;
//       COLORREF clrR0 = M_12(mR.color);
//       COLORREF clrB0 = M_12(mB.color);
//       //
//       COLORREF clrL1 = M_14(mL.color);
//       COLORREF clrT1 = M_14(mT.color);
//       COLORREF clrR1 = M_34(mR.color);
//       COLORREF clrB1 = M_34(mB.color);
//       //
//       { CMyPen pen( hDC, clrL0 ); pen.Vert( rc0.left  , rc0.top, rc0.bottom );}
//       { CMyPen pen( hDC, clrT0 ); pen.Horz( rc0.top   , rc0.left, rc0.right );}
//       { CMyPen pen( hDC, clrR0 ); pen.Vert( rc0.right , rc0.top, rc0.bottom );}
//       { CMyPen pen( hDC, clrB0 ); pen.Horz( rc0.bottom, rc0.left, rc0.right );}
//       //
//       { CMyPen pen( hDC, clrL1 ); pen.Vert( rc1.left  , rc1.top, rc1.bottom );}
//       { CMyPen pen( hDC, clrT1 ); pen.Horz( rc1.top   , rc1.left, rc1.right );}
//       { CMyPen pen( hDC, clrR1 ); pen.Vert( rc1.right , rc1.top, rc1.bottom );}
//       { CMyPen pen( hDC, clrB1 ); pen.Horz( rc1.bottom, rc1.left, rc1.right );}
//       //
//       }
//     }
//   }
// //
