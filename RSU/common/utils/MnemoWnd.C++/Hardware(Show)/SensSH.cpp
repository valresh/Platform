#include "Queue.h"
#include "../Lang.h"
#include "units.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_SensSH : public SPROLOZ
{
	int nType;
	const char* szText;
	RECT un;
	double WorkMin, WorkMax;
	COLORREF color;
	COLORREF colorTrans;
	bool bBig;
	bool bUseErrValue;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//bool Eq_StateMes ( int nState,int Flag ) { return ( nState & Flag ) == Flag; };

// Функция отображения
//int PaintSensSH(DefinePaint)
//  {
//  QueueP(Queue_SensSH);
//  //
//  double dValue = 0;
//  CShSensor* sh=NULL;
//  if (p->nNumber!=-1)
//	sh = (CShSensor*)PipeSh;
//  else
//  if (p->nSocketNumber!=-1)
//	sh=(CShSensor*)q.m_pSocket->Sh(p->nSocketNumber);
//  //
//  if (sh==NULL)
//	  return 1;
//  if (p->bUseErrValue)dValue = sh->ErrValue;
//  else dValue = sh->ShowValue;
//  //double dValue = sh.ShowValue;
//
//  //Датчиков нет в Tegs.scn
//  //dValue = max( dValue, p->WorkMin );
//  //dValue = min( dValue, p->WorkMax );
//  //
//  if ( p->nType == 0 )
//    {
//  	//if( sh.m_nAlarm == 127 )
//   //   ::DrawText( hDC, "-----", 5, &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//   // else
//      {
//      //
//      char szText[64] = "\0";
//      int N = 3;
//      double d = fabs(dValue);
//           if ( d >=100 ) N = 0;
//      else if ( d >=10. ) N = 1;
//      else if ( d >= 1. ) N = 2;
//      else if ( d == 0. ) N = 0;
//      DoubleToStr( szText, dValue, N );
//      //DblToStr( szText, dValue );
//      ::DrawText( hDC, szText, lstrlen(szText), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//      ::DrawText( hDC, p->szText, lstrlen(p->szText), &p->un, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//      }
//    }
//  else
//  if ( p->nType == 1 ) //Вывод единиц измерения
//    {
//    ::TextOut( hDC, p->mRect.left, p->mRect.bottom, p->szText, lstrlen(p->szText) );
//    }
//  else
//  if ( p->nType == 2 ) //Вывод имени датчика
//    {
//    //::TextOut( hDC, p->mRect.left, p->mRect.bottom, p->szText, lstrlen(p->szText) );
//    ::DrawText( hDC, p->szText, lstrlen(p->szText), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//    }
//  else
//  if ( p->nType == 3 ) //Вывод показаний датчика IsTypeOK
//    {
//    if ( IsTypeOk )
//      {
//      char szText[64] = "\0";
//      mvaluef( szText, p->nFloat, dValue );
//      ::DrawText( hDC, szText, lstrlen(szText), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
//      }
//    }
//  else
//  if ( p->nType == 4 ) //Вывод показаний датчика IsTypeOK
//    {
//    if ( IsTypeOk )
//      {
//      char szText[64] = "\0";
//      mvaluef( szText, p->nFloat, dValue*100./p->WorkMax );
//      ::DrawText( hDC, szText, lstrlen(szText), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//      }
//    }
//  else
//  if ( p->nType == 5 ) //Вывод шкалы вертикальной
//    {
//    if ( IsTypeOk )
//      {
//      CMyPen pen( hDC, RGB(0xff,0,0) );
//      RECT rcDraw=p->mRect;
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nHeight=rcDraw.bottom-rcDraw.top;
//      rcDraw.top=p->mRect.bottom-(int)((m_nHeight/(p->WorkMax-p->WorkMin))*(dValue-p->WorkMin))-1;
//      rcDraw.bottom=rcDraw.top+1;
//	  if(p->bBig)
//	  {
//		  CMyBrush brush( hDC, RGB(0xff,0,0) );
//		  rcDraw.top--;
//		  rcDraw.bottom++;
//		  brush.Rect(rcDraw);
//	  }
//      pen.Rect( rcDraw );
//      }
//    }
//  else
//  if ( p->nType == 6 ) //Вывод хитрой шкалы горизонтальной x=a*y*y
//    {
//    if ( IsTypeOk )
//      {
//      CMyPen pen( hDC, RGB(0xff,0,0) );
//      RECT rcDraw=p->mRect;
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nWidth=rcDraw.right-rcDraw.left;
//      double a=0.0000000346; // коэффициент по которому приводим количество оборотов к давлению
//      double dValueNew=a*dValue*dValue;
//      rcDraw.left=p->mRect.left+(int)((m_nWidth/10.0)*dValueNew)-1;
//      rcDraw.right=rcDraw.left+1;
//      pen.Rect( rcDraw );
//      }
//    }
//  else
//  if ( p->nType == 7 ) //Вывод U манометра
//    {
//    if ( IsTypeOk )
//      {
//      CMyBrush brush( hDC, RGB(0,0xff,0) );
//      RECT rcDraw=p->mRect;
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nHeight=rcDraw.bottom-rcDraw.top;
//      double dValueMMVodSt=dValue*10000;
//      dValueMMVodSt=max( dValueMMVodSt, -90 );
//      dValueMMVodSt=min( dValueMMVodSt, 90 );
//      rcDraw.top=p->mRect.bottom-(int)((m_nHeight/180.0)*(90-dValueMMVodSt))-1;
//      brush.Rect( rcDraw );
//      rcDraw.top=p->mRect.bottom-(int)((m_nHeight/180.0)*(dValueMMVodSt+90))-1;
//      rcDraw.left+=20;
//      rcDraw.right+=20;
//      brush.Rect( rcDraw );
//      }
//    }
//  else
//  if ( p->nType == 8 ) //Вывод шкалы горизонтальной
//    {
//    if ( IsTypeOk )
//      {
//      CMyPen pen( hDC, RGB(0xff,0,0) );
//      RECT rcDraw=p->mRect;
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nWidth=rcDraw.right-rcDraw.left;
//      rcDraw.left=p->mRect.left+(int)((m_nWidth/(p->WorkMax-p->WorkMin))*(dValue-p->WorkMin))-1;
//      rcDraw.right=rcDraw.left+1;
//      pen.Rect( rcDraw );
//      }
//    }
//  if ( p->nType == 9 ) //Вывод значения
//  {
//    if ( IsTypeOk )
//    {
//		dValue = sh->Ventil;
//		DrawValue( hDC, p, dValue, "%" );
//	}
//  }
//  if ( p->nType == 10 ) //Вывод единиц измерения
//  {
//    if ( IsTypeOk )
//    {
//		char szText[64] = "\0";
//		bool bShowText=true;
//		if (Eq_StateMes(sh->Mes,UNIT_P))
//			lstrcpy(szText,"кгс/см2");
//		else
//			if (Eq_StateMes(sh->Mes,UNIT_T))
//				lstrcpy(szText,"C");
//			else
//				bShowText=false;
/////*
//		if (Eq_StateMes(sh.Mes,UNIT_MPA))
//			lstrcpy(szText,"МПа");
//		else
//			if (Eq_StateMes(sh.Mes,UNIT_KPA))
//				lstrcpy(szText,"КПа");
//			else
//				if (Eq_StateMes(sh.Mes,UNIT_PA)&&Eq_StateMes(sh.Mes,UNIT_P))
//					lstrcpy(szText,"Па");
//				else
//					if (Eq_StateMes(sh.Mes,UNIT_P))
//						lstrcpy(szText,"кгс/см2");
//					else
//						if (Eq_StateMes(sh.Mes,UNIT_C))
//							lstrcpy(szText,"C");
//						else
//							bShowText=false;
//*******/
//		if (bShowText)
//			::DrawText( hDC, szText, lstrlen(szText), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//	}
//  }
//  else
//  if ( p->nType == 11 ) //Вывод градусника
//    {
//    if ( IsTypeOk )
//      {
//      CMyBrush brush( hDC, p->color );
//      RECT rcDraw=p->mRect;
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nHeight=rcDraw.bottom-rcDraw.top;
//      rcDraw.top=p->mRect.bottom-(int)((m_nHeight/(p->WorkMax-p->WorkMin))*dValue)-1;
//      //rcDraw.bottom=rcDraw.top+1;
//      brush.Rect( rcDraw );
//      }
//    }
//  else
//  if ( p->nType == 12 ) //Вывод шкалы фэйсплейта
//    {
//    if ( IsTypeOk )
//      {
//	  CMyPen pen(hDC, p->color );
//
//      CMyBrush brush( hDC, p->color );
//      RECT rcDraw=p->mRect;
//	  pen.Rect(rcDraw);
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nHeight=rcDraw.bottom-rcDraw.top;
//      rcDraw.top=p->mRect.bottom-(int)((m_nHeight/(p->WorkMax-p->WorkMin))*dValue)-1;
//      brush.Rect( rcDraw );
//      }
//    }
//  else
//  if ( p->nType == 13 ) //Вывод треугольника
//    {
//    if ( IsTypeOk )
//      {
//	  CMyPen pen(hDC, p->color );
//
//      CMyBrush brush( hDC, p->color );
//      RECT rcDraw=p->mRect;
//	  //pen.Rect(rcDraw);
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nHeight=rcDraw.bottom-rcDraw.top;
//      int nValue=p->mRect.bottom-(int)((m_nHeight/(p->WorkMax-p->WorkMin))*dValue)-1;
//      //brush.Rect( rcDraw );
//	  POINT pt[3];
//	  pt[0].x=rcDraw.left;pt[0].y=nValue;
//	  int nSdvig=(rcDraw.right-rcDraw.left)/2;
//	  pt[1].x=rcDraw.right;pt[1].y=nValue-nSdvig;
//	  pt[2].x=rcDraw.right;pt[2].y=nValue+nSdvig;
//	  brush.Points(pt,3,1);
//      }
//    }
//  else
//  if ( p->nType == 14 ) //Вывод уровня
//    {
//    if ( IsTypeOk )
//      {
//      CMyBrush brush( hDC, p->color );
//      RECT rcDraw=p->mRect;
//      dValue = max( dValue, p->WorkMin );
//      dValue = min( dValue, p->WorkMax );
//      int m_nHeight=rcDraw.bottom-rcDraw.top;
//      rcDraw.top=p->mRect.bottom-(int)((m_nHeight/(p->WorkMax-p->WorkMin))*dValue)-1;
//      //rcDraw.bottom=rcDraw.top+1;
//	  TransparentRect(hDC,rcDraw,p->color,p->colorTrans);
//      /*brush.Rect( rcDraw );*/
//      }
//	else
//		if ( ::GetMnemoStyle() & MNEMO_EDIT )
//			TransparentRect(hDC,p->mRect,0x80ff,p->colorTrans);
//    }
//  else
//  if ( p->nType == 15 ) //Вывод уровня
//    {
//    if ( IsTypeOk )
//      {
//            char szText[64] = "\0";
//            int nUnits=sh->Mes;
//            //System\Ini\Размерности.csv
//            switch (nUnits)
//            {
//                case 2: lstrcpy(szText,"ppm"); break;
//                case 3: lstrcpy(szText,"сек"); break;
//                case 4: lstrcpy(szText,"кг/м3"); break;
//                case 5: lstrcpy(szText,"мг/л"); break;
//                case 6: lstrcpy(szText,"мг/м3"); break;
//                case 7: lstrcpy(szText,"%"); break;
//                case 11: lstrcpy(szText,"нм3/ч"); break;
//                case 12: lstrcpy(szText,"м3/ч"); break;
//                case 13: lstrcpy(szText,"кг/ч"); break;
//                case 14: lstrcpy(szText,"т/ч"); break;
//                case 15: lstrcpy(szText,"л/ч"); break;
//                case 16: lstrcpy(szText,"т.м3/ч"); break;
//                case 17: lstrcpy(szText,"м3/ч(газ)"); break;
//                case 21: lstrcpy(szText,"атм"); break;
//                case 22: lstrcpy(szText,"Па"); break;
//                case 23: lstrcpy(szText,"кПа"); break;
//                case 24: lstrcpy(szText,"МПа"); break;
//                case 25: lstrcpy(szText,"ммв.ст"); break;
//				case 26: lstrcpy(szText,"кгс/см2"); break;
//                case 31: lstrcpy(szText,"C"); break;
//                case 126: lstrcpy(szText,"ммрт.ст"); break;
//            }
//            ::DrawText( hDC, szText, lstrlen(szText), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//      }
//    }
//
//  //
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddSensSH(DefineTegs)
{
	//
	AddQueueEx(Queue_SensSH);
	//
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	//p->paint = &PaintSensSH;
	p->WorkMin = 0;
	p->WorkMax = 100;
	p->color = RGB(255, 0, 0);
	p->colorTrans = 0xcccccc;
	p->bBig = false;
	p->bUseErrValue = false;
	p->nNumber = -1;
	p->nSocketNumber = -1;
	p->nType = 0;
	//
	STegObj obj[] =
	{
	{ 'R', "unit" , &p->un },
	{ 'E', "un"   , &p->un },
	{ 'I', "type" , &p->nType  },
	{ 'D', "min" ,  &p->WorkMin },
	{ 'D', "max" ,  &p->WorkMax },
	{ 'A', "color", &p->color  },
	{ 'A', "colorTrans", &p->colorTrans  },
	{ 'B', "big" , &p->bBig },
	{ 'L', "useErrValue" , &p->bUseErrValue },
	// Дополнительная информация
	EXT_QUEUE
	};
	//
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	//
	if (*teg[0].value == '.')
	{
		p->nType = Atoi(teg[0].value + 1);
		teg[0].value[1] = 0;
	}
	else
	{
		//Sergej    StrCPY( p->szText, teg[0].value, sizeof(p->szText) );
	}
	//
	p->nSocketNumber = def.AddKindEx(id_Sensor, teg[0].value);
	int nNumber = def.GetMnemoTags(p->nSocketNumber);
	if (nNumber >= 0)
	{
		STeg teg;
		GetTag(nNumber, teg);
		p->WorkMin = teg.dWorkMin;
		p->WorkMax = teg.dWorkMax;
		if (p->nType == 0 || p->nType == 1) //Вывод единиц измерения
		{
			p->szText = teg.szMeas;
		}
		else
		{
			if (p->nType == 2 && p->szText == NULL) //Вывод имени датчика
			{
				p->szText = teg.szName;
			}
		}
	}
	//
	managed::RegisterElement(teg->name, p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
