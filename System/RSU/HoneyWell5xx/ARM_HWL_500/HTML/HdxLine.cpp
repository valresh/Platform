// #include "StdAfx.h"
// #include "HTML.h"
// //
// void CHdx::Line( HDC hDC, CHTML& html, cross::RECT& rc )
//   {
//   ASSERT( m_nFillStyle == 0 || m_nFillStyle == 1 || m_nFillStyle == 2 );
//   ASSERT( (m_dwFlags & HDX_POINTS ) != 0 );
//   if ( m_clrLine == 0xff000000 )
//     return;
//   DrawLinesWithPenFromLineStyle(hDC,html,rc);
//   //
//   }
// //
// void CHdx::ChangePenFromLineStyle( Gdiplus::Pen& pen )
// {
// 	switch (m_nLineStyle)
// 	{
// 		case eLineStyle2_2:
// 		{
// 			REAL dashValues[2] = { 2.0f, 2.0f};
// 			pen.SetDashStyle(DashStyleCustom);
// 			pen.SetDashPattern (dashValues,2);
// 			break;
// 		}
// 		case eLineStyle4_2:
// 		{
// 			REAL dashValues[2] = { 4.0f, 2.0f};
// 			pen.SetDashStyle(DashStyleCustom);
// 			pen.SetDashPattern (dashValues,2);
// 			break;
// 		}
// 		case eLineStyle8_2:
// 		{
// 			REAL dashValues[2] = { 8.0f, 2.0f};
// 			pen.SetDashStyle(DashStyleCustom);
// 			pen.SetDashPattern (dashValues,2);
// 			break;
// 		}
// 		case eLineStyle1_2:
// 		{
// 			REAL dashValues[2] = { 1.0f, 2.0f};
// 			pen.SetDashStyle(DashStyleCustom);
// 			pen.SetDashPattern (dashValues,2);
// 			break;
// 		}
// 		case eLineStyle4_2_1_2:
// 		{
// 			REAL dashValues[4] = { 4.0f, 2.0f, 1.0f, 2.0f};
// 			pen.SetDashStyle(DashStyleCustom);
// 			pen.SetDashPattern (dashValues,4);
// 			break;
// 		}
// 	}
// }

// void CHdx::DrawLinesWithPenFromLineStyle( HDC hDC, CHTML& html, cross::RECT& rc )
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
// 	 if (m_nPointPT>0)
// 	 {
//          if(ptArray->y < -10000000)  //какие-то левые линии
//            return;

// 		 Arrows( hDC, ptArray );
// 		 graphics.DrawLines( &pen, (Gdiplus::Point*)ptArray,m_nPointPT );
// 	 }
// }

// void CHdx::Arrows( HDC hDC, LPPOINT pt )
//   {
//   // На всякий случай, чтоб не ёбнулось
//   if ( m_nPointPT < 2 ) return;
//   if (m_nLineStyle==eLineStyleNone)
// 	  return;
//   //                      EndArrow StartArrow
//   // 1 - стрелка фигурная в конце  в начале
//   // 2 - стрелка          в конце  в начале
//   // 3 - круг             в конце  в начале
//   POINT p, t;
//   if ( m_nEndArrow )
//     {
//     p = pt[m_nPointPT-1], t = pt[m_nPointPT-2];
//     Arrow( hDC, p, t, m_nEndArrow );
//     }
//   //
//   if ( m_nStartArrow )
//     {
//     p = pt[0], t = pt[1];
//     Arrow( hDC, p, t, m_nStartArrow );
//     }
//   }

// void CHdx::Arrow   ( HDC hDC, POINT pt, POINT vc, int nArrowType )
// {
//   int nShiftArrowSize=8;
//   if (nArrowType==eArrowTypeLine)
// 	nShiftArrowSize=4;
//   POINT ptShiftLengthWidth=GetArrowShift(m_nLineWidth,nArrowType);
//   switch (nArrowType)
//   {
// 		case eArrowTypeCircle: DrawEllipseArrow(hDC,pt,ptShiftLengthWidth);break;
// 		case eArrowTypeLine: DrawLineArrow(hDC,pt,vc,ptShiftLengthWidth);break;
// 		case eArrowTypeFill: DrawFillTriangleArrow(hDC,pt,vc,ptShiftLengthWidth);break;
//   }
// }
// //
// void CHdx::DrawEllipseArrow(HDC hDC,POINT& pt,POINT& ptShiftLengthWidth)
// {
// 		CMyPen   pen( hDC, m_clrLine, 0 );
// 		CMyBrush brush( hDC, m_clrLine, true );
// 		cross::RECT r = { pt.x-ptShiftLengthWidth.x, pt.y-ptShiftLengthWidth.y, pt.x+ptShiftLengthWidth.x, pt.y+ptShiftLengthWidth.y };
// 		pen.Ellipse( r );
// }

// void CHdx::DrawFillTriangleArrow(HDC hDC,POINT& pt,POINT& vc,POINT& ptShiftLengthWidth)
// {
// 	strPointArray3 strArray=GetPointArrayFillTriangleArrow(pt,vc,ptShiftLengthWidth);
// 	if (strArray.bIsEmpty)
// 		return;
// 	CMyPen   pen( hDC, m_clrLine, 0 );
// 	CMyBrush brush( hDC, m_clrLine, true );
// 	pen.Points( strArray.ptArray, 3, 1 );
// }

// void CHdx::DrawLineArrow(HDC hDC,POINT& pt,POINT& vc,POINT& ptShiftLengthWidth)
// {
// 		strPointArray3 strArray=GetPointArrayLineArrow(pt,vc,ptShiftLengthWidth);
// 		if (strArray.bIsEmpty)
// 			return;
// 	  	CMyPen   pen( hDC, m_clrLine, m_nLineWidth,PS_SOLID );
// 		pen.Line(strArray.ptArray[1].x,strArray.ptArray[1].y,strArray.ptArray[0].x,strArray.ptArray[0].y);
// 		pen.Line(strArray.ptArray[0].x,strArray.ptArray[0].y,strArray.ptArray[2].x,strArray.ptArray[2].y);
// }

// POINT CHdx::GetArrowShift(int& nArrowWidth, int& nArrowType)
// {
// 	POINT ptShiftLengthWidth;
// 	FillPoint(ptShiftLengthWidth,0,0);
// 	switch (nArrowType)
// 	{
// 		case eArrowTypeCircle:
// 			{
// 				switch (nArrowWidth)
// 				{
// 					case 1: FillPoint(ptShiftLengthWidth,5,5); break;
// 					case 2: FillPoint(ptShiftLengthWidth,7,7); break;
// 					case 3: FillPoint(ptShiftLengthWidth,8,8); break;
// 					case 4: FillPoint(ptShiftLengthWidth,10,10); break;
// 					case 6: FillPoint(ptShiftLengthWidth,13,13); break;
// 					case 8: FillPoint(ptShiftLengthWidth,15,15); break;
// 				}
// 			}
// 			break;
// 		case eArrowTypeLine:
// 			{
// 				switch (nArrowWidth)
// 				{
// 					case 1: FillPoint(ptShiftLengthWidth,10,8); break;
// 					case 2: FillPoint(ptShiftLengthWidth,14,10); break;
// 					case 3: if(Version != BERTH_ONPZ)
//                               FillPoint(ptShiftLengthWidth,18,12);
//                             else FillPoint(ptShiftLengthWidth,11,9);
//                             break;
// 					case 4: FillPoint(ptShiftLengthWidth,22,16); break;
// 					case 6: FillPoint(ptShiftLengthWidth,29,20); break;
// 					case 8: FillPoint(ptShiftLengthWidth,34,26); break;
// 				}
// 			}
// 			break;
// 		case eArrowTypeFill:
// 			{
// 				switch (nArrowWidth)
// 				{
// 					case 1: FillPoint(ptShiftLengthWidth,11,10); break;
// 					case 2: FillPoint(ptShiftLengthWidth,14,12); break;
// 					case 3: if(Version != BERTH_ONPZ)
//                               FillPoint(ptShiftLengthWidth,18,14);
//                             else FillPoint(ptShiftLengthWidth,11,9);
//                             break;
// 					case 4: FillPoint(ptShiftLengthWidth,22,18); break;
// 					case 6: FillPoint(ptShiftLengthWidth,29,24); break;
// 					case 8: FillPoint(ptShiftLengthWidth,34,32); break;
// 				}
// 			}
// 			break;
// 	};
// 	return ptShiftLengthWidth;
// }
// //
// void CHdx::FillPoint(POINT& pt, int nShiftX, int nShiftY)
// {
// 	pt.x=nShiftX;
// 	pt.y=nShiftY;
// }

// strPointArray3 CHdx::GetPointArrayFillTriangleArrow(POINT& pt,POINT& vc,POINT& ptShiftLengthWidth)
// {
//       return GetPointArray(pt,vc, ptShiftLengthWidth, true);
// }

// strPointArray3 CHdx::GetPointArrayLineArrow(POINT& pt,POINT& vc,POINT& ptShiftLengthWidth)
// {
//       return GetPointArray(pt,vc, ptShiftLengthWidth, false);
// }

// strPointArray3 CHdx::GetPointArray(POINT& pt,POINT& vc,POINT& ptShiftLengthWidth, bool bIsStartPoint)
// {
//     strPointArray3 strArray;
// 	POINT ptStartPoint=pt;
// 	if (( pt.y == vc.y )||( pt.y == vc.y-1 )||( pt.y == vc.y+1 ))//Горизонтальная стрелка
//       {
// 			if (bIsStartPoint)
// 			{
// 				if ( pt.x <= vc.x )//Влево
// 					ptStartPoint.x-=ptShiftLengthWidth.x/4;
// 				else
// 					ptStartPoint.x+=ptShiftLengthWidth.x/4;
// 			}
// 			POINT p[3] = { ptStartPoint.x, ptStartPoint.y, ptStartPoint.x, ptStartPoint.y-ptShiftLengthWidth.y/2,
// 				ptStartPoint.x, ptStartPoint.y+ptShiftLengthWidth.y/2 };
// 			if ( pt.x <= vc.x )//Влево
// 			  p[1].x+=ptShiftLengthWidth.x,p[2].x+=ptShiftLengthWidth.x;
// 			else// Вправо
// 			  p[1].x-=ptShiftLengthWidth.x,p[2].x-=ptShiftLengthWidth.x;
// 			strArray.ptArray[0].x = p[0].x; strArray.ptArray[0].y=p[0].y;
// 			strArray.ptArray[1].x= p[1].x; strArray.ptArray[1].y=p[1].y;
// 			strArray.ptArray[2].x =p[2].x; strArray.ptArray[2].y=p[2].y;
// 			strArray.bIsEmpty=false;
// 			return strArray;
// 	}
// 	  else
// 	  if (( pt.x == vc.x )||( pt.x == vc.x+1 )||( pt.x == vc.x-1 ))//Вертикальная стрелка
// 	  {
// 		      if (bIsStartPoint)
// 			  {
// 				  if ( pt.y <= vc.y )//Вверх
// 					ptStartPoint.y-=ptShiftLengthWidth.x/4;
// 				  else
// 					ptStartPoint.y+=ptShiftLengthWidth.x/4;
// 			  }
// 			  POINT p[3] = { ptStartPoint.x, ptStartPoint.y, ptStartPoint.x-ptShiftLengthWidth.y/2,
// 			  ptStartPoint.y, ptStartPoint.x+ptShiftLengthWidth.y/2, ptStartPoint.y };
// 			  if ( pt.y <= vc.y )//Вверх
// 				  p[1].y+=ptShiftLengthWidth.x,p[2].y+=ptShiftLengthWidth.x;
// 			  else// Вниз
// 				  p[1].y-=ptShiftLengthWidth.x,p[2].y-=ptShiftLengthWidth.x;
// 			strArray.ptArray[0].x = p[0].x; strArray.ptArray[0].y=p[0].y;
// 			strArray.ptArray[1].x= p[1].x; strArray.ptArray[1].y=p[1].y;
// 			strArray.ptArray[2].x =p[2].x; strArray.ptArray[2].y=p[2].y;
// 			strArray.bIsEmpty=false;
// 			return strArray;
// 	  }
// 	  else
// 	  {
// 		POINT p[3] = { 0, 0, 0, 0, 0, 0 };
// 		strArray.ptArray[0].x = p[0].x; strArray.ptArray[0].y=p[0].y;
// 		strArray.ptArray[1].x= p[1].x; strArray.ptArray[1].y=p[1].y;
// 		strArray.ptArray[2].x =p[2].x; strArray.ptArray[2].y=p[2].y;
// 		strArray.bIsEmpty=true;
// 		return strArray;
// 	  }
// }
