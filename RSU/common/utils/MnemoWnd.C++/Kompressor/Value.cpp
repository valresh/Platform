//#include "..\MnemoWnd.h"
#include "Queue.h"
#include "../Lang.h"
#include <string>
#include "CommProc.h"

//
#define PAINTEX PAINT, char* szValue
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с переменными
struct Queue_Value : public SPROLOZ
{
	SValueDef* defOut;
	char szText[32];//Для настройки
	char szMeas[32];//Единицы измерения
	char szUnits[32];//Единицы измерения
	RECT rect;
	int    mAlign;
	int     mEdge;// Вывод рамки
	COLORREF mTxt;// Вывод текста
	COLORREF mBkg;// Вывод фона
	COLORREF mColor;//фон для моих, зеленый по умолчанию
	void PaintDbl(PAINTEX, double dScale);
	void PaintInt(PAINTEX);
	void PaintI64(PAINTEX);
	void PaintChr(PAINTEX);
	void PaintBol(PAINTEX);
	void PaintFlt(PAINTEX);
	void PaintStr(PAINTEX);
	int  nTeg;
	UINT eType;
	bool bColorEdge;
	bool bRamka;
	int  nHS;
	int nEqual;
	int nNoEqual;
	bool bGreen;
	bool bIsGreen;
	int nKlapan;
	bool bHS;
	bool bNoFillAlarm;
	bool bValueBoldPlus2;
	bool bGray;
	double dScale;
	bool showAlways;
	bool bDubler;
	bool bShowValue;
	// For C# use only!
	char DrawValue[512];
	bool NoDraw;
	bool DrawGreyRect;
	bool ShowElement;
	COLORREF EdgeColor;
	COLORREF TextColor;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
void Queue_Value::PaintDbl(PAINTEX, double dScale)
{
	SParamValue var(nNumber, defOut);
	q.m_pPipe->FillValue(szValue, var, nFloat, NULL, dScale);
}
////
void Queue_Value::PaintInt(PAINTEX)
{
	SParamValue var(nNumber, defOut);
	int i = q.m_pPipe->ValueI(var);
	sprintf_s(szValue, 512, "%d", i);
}
//
void Queue_Value::PaintI64( PAINTEX )
  {
  ASSERT(0);
  }
//
void Queue_Value::PaintChr( PAINTEX )
  {
  ASSERT(0);
  }
//
void Queue_Value::PaintBol( PAINTEX )
  {
  ASSERT(0);
  }
//
void Queue_Value::PaintFlt( PAINTEX )
  {
  ASSERT(0);
  }
//
void Queue_Value::PaintStr( PAINTEX )
  {
  ASSERT(0);
  }
////
//// Функция отображения
int PaintValue(DefinePaint)
{
	QueueP(Queue_Value);
	bool bAlarm = false;
	COLORREF color = p->mTxt;
	char szValue[512] = "###";
	// For C# use only
	strcpy(p->DrawValue, szValue);
	p->NoDraw = false;
	p->DrawGreyRect = false;
	p->ShowElement = false;
	p->EdgeColor = 0;
	p->TextColor = color;
	//
	CShBase* sh = PipeSh;
	if (p->defOut == NULL)
	{
		// In C#
		//if (!p->bHS)
		//{
		//	CMyBrush brush(hDC, 0xff00ff);
		//	brush.Rect(p->rect);
		//}
	}
	else if (*p->szText)
	{
		strcpy(szValue, p->szText);
		// In C#
		//if (::IsMnemoStyle(MNEMO_EDIT))
		//{
		//	color = 0x80ff;
		//}
	}
	else if (IsTypeOk)
	{
		if (p->nKlapan >= 0)
		{
			CShKlapan& Sh = (CShKlapan&)q.m_pPipe->SH(p->nKlapan);
			if (Sh.RetCode == CShBase::rcOK)
			{
				bool bManual = (Sh.LocalRemoteDouble == 0) && !p->showAlways;
				if (strstr(p->defOut->name, "Задание") != NULL)
				{
					if (bManual)
					{
						p->NoDraw = true;
						return 1;
					}
				}
				else if (strstr(p->defOut->name, "Поворот") != NULL)
				{
					if (!bManual)
					{
						p->NoDraw = true;
						return 1;
					}
				}
			}
		}

		color = p->mTxt;
		switch (p->defOut->eVal)
		{
		case enumValueDbl: p->PaintDbl(q, szValue, p->dScale); break;
		case enumValueInt: p->PaintInt(q, szValue); break;
		case enumValueI64: p->PaintI64(q, szValue); break;
		case enumValueChr: p->PaintChr(q, szValue); break;
		case enumValueBol: p->PaintBol(q, szValue); break;
		case enumValueFlt: p->PaintFlt(q, szValue); break;
		case enumValueStr: p->PaintStr(q, szValue); break;
		};

		if (*p->szMeas)
		{
			strcat(szValue, " ");
			strcat(szValue, p->szMeas);
		}
	}

	if (p->mBkg != 0xffffffff)
	{
		// In C#
		//CMyBrush brush(hDC, p->mBkg);
		//brush.Rect(p->rect);
	}

	if (p->bColorEdge)
	{
		const double dBadValue = -1021;
		double dHH = TegValueD(p->nTeg, "HH", dBadValue);
		double dH = TegValueD(p->nTeg, "H", dBadValue);
		double dLL = TegValueD(p->nTeg, "LL", dBadValue);
		double dL = TegValueD(p->nTeg, "L", dBadValue);
		char* szValueTemp = (char*)::TegValueS(p->nTeg, "value", "0");
		if (strcmp(szValueTemp, "0") != 0)
		{
			strcpy(szValue, szValueTemp);
		}

		COLORREF clr = p->mColor;
		if (p->bGray)
		{
			clr = RGB(217, 217, 217);
		}

		COLORREF clrLL_HH = RGB(255, 0, 0);
		COLORREF clrL_H = RGB(255, 255, 0);
		double dValue = atof(szValue);
		if (!p->bNoFillAlarm)
		{
			if (dL != dBadValue)
			{
				if (dValue <= dL)
				{
					clr = clrL_H;
					bAlarm = true;
				}
			}

			if (dLL != dBadValue)
			{
				if (dValue <= dLL)
				{
					clr = clrLL_HH;
					bAlarm = true;
				}
			}

			if (dH != dBadValue)
			{
				if (dValue >= dH)
				{
					clr = clrL_H;
					bAlarm = true;
				}
			}

			if (dHH != dBadValue)
			{
				if (dValue >= dHH)
				{
					clr = clrLL_HH;
					bAlarm = true;
				}
			}
		}

		p->bIsGreen = p->bGreen;
		if (p->nHS >= 0)
		{
			CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nHS);
			int nState;
			if (Sh.RetCode == CShBase::rcOK)
			{
				if (Sh.mOn)
				{
					nState = 1;
				}
				else
				{
					nState = 0;
				}

				if (p->nEqual != -1)
				{
					p->bIsGreen = (nState == p->nEqual);
				}

				if (p->nNoEqual != -1)
				{
					p->bIsGreen = (nState != p->nNoEqual);
				}
			}
		}

		// In C#
		//if (p->bIsGreen)
		//{
		//	clr = RGB(0, 255, 0);
		//}

		// In C#
		//CMyBrush brush(hDC, clr);
		//brush.Rect(p->rect);
		p->EdgeColor = clr;
		//if (p->bRamka)
		//{
		//	clr = 0x000000;
		//}

		// In C#
		//CMyPen pen(hDC, clr);
		//pen.Rect(p->rect);
	}

	if ((p->bGray) && (bAlarm))
	{
		color = 0x0;
	}

	if (strcmp(szValue, "###") == 0)
	{
		// In C#
		if (p->bGray)
		{
			//	CMyBrush brush(hDC, RGB(217, 217, 217));
			//	brush.Rect(p->rect);
			p->DrawGreyRect = true;
		}

		// ArKu temp comment out. Check in future
		//if (::IsMnemoStyle(MNEMO_EDIT))
		//{
		//	color = 0x80ff;
		//}
		//else
		{
			char strFormat[16];
			sprintf_s(strFormat, "%%.%df", abs(p->nFloat));
			sprintf_s(szValue, strFormat, 0.);
		}
	}

	if (p->bHS)
	{
		strcpy(szValue, "###");
		color = 0x80ff;
		if (p->nNumber >= 0)
		{
			CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nNumber);
			if (Sh.RetCode == CShBase::rcOK)
			{
				if (Sh.mOn)
				{
					strcpy(szValue, "1");
				}
				else
				{
					strcpy(szValue, "0");
				}

				color = p->mTxt;
			}
		}

		// In C#
		//CMyFont font(hDC, NULL, color);
		//font.DrawS(p->rect, szValue, p->mAlign);
		p->TextColor = color;
	}
	else
	{
		// In C#
		//CMyFont font(hDC, NULL, color);
		p->TextColor = color;
		if (*p->szUnits)
		{
			bool bShowElement = true;
			if (p->nHS >= 0)
			{
				CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nHS);
				if (Sh.RetCode == CShBase::rcOK)
				{
					if (!Sh.mOn)
					{
						bShowElement = false;
					}

					int nStateElement = 0;
					if (Sh.mOn)
					{
						nStateElement = 1;
					}
					else
					{
						nStateElement = 0;
					}

					if (p->nEqual != -1)
					{
						bShowElement = (nStateElement == p->nEqual);
					}
				}
			}

			p->ShowElement = bShowElement;
			// In C#
			//RECT rcUnit;
			//font.Calc(rcUnit, p->szUnits);
			//if (bShowElement)
			//{
			//	font.DrawS(p->rect, p->szUnits, DT_RIGHT);
			//}

			// In C#
			//RECT rcValue = p->rect;
			//int nWidthOfUnit = rcUnit.right - rcUnit.left;
			//rcValue.right = rcValue.right - nWidthOfUnit;
			//if (p->bValueBoldPlus2)
			//{
			//	HFONT hFont = (HFONT)::SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
			//	LOGFONT lf;
			//	GetObject(hFont, sizeof(LOGFONT), &lf);
			//	lf.lfHeight -= 2;
			//	lf.lfWeight = 900;
			//	HFONT hFontB = CreateFontIndirect(&lf);
			//	{
			//		COLORREF clrBlue = RGB(0, 0, 153);
			//		CMyFont fontB(hDC, hFontB, clrBlue);
			//		fontB.DrawS(rcValue, szValue, p->mAlign);
			//	}
			//	::SelectObject(hDC, hFont);
			//	::DeleteObject(hFontB);
			//}
			//else
			//{
			//	if (bShowElement)
			//	{
			//		font.DrawS(rcValue, szValue, p->mAlign);
			//	}
			//}
		}
		else
		{
			if (p->nHS >= 0)
			{
				CShHS& Sh = (CShHS&)q.m_pPipe->SH(p->nHS);
				int nState;
				if (Sh.RetCode == CShBase::rcOK)
				{
					if (Sh.mOn)
					{
						nState = 1;
					}
					else
					{
						nState = 0;
					}

					if (p->nEqual != -1)
					{
						p->bShowValue = (nState == p->nEqual);
					}

					if (p->nNoEqual != -1)
					{
						p->bShowValue = (nState != p->nNoEqual);
					}
				}
			}

			// In C#
			//if (p->bShowValue)
			//{
			//	font.DrawS(p->rect, szValue, p->mAlign);
			//}
		}
	}

	// In C#
	//if (p->mEdge)
	//{
	//	::DrawEdge(hDC, &p->rect, p->mEdge, BF_RECT);
	//}

	strcpy(p->DrawValue, szValue);

	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddValue(DefineTegs)
{
	AddQueueEx(Queue_Value);
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	p->paint = &PaintValue;
	p->mBkg = 0xffffffff;
	p->mColor = RGB(0, 255, 0);
	p->mTxt = RGB(0, 0, 0);
	p->mAlign = 0;
	p->bColorEdge = false;
	p->bRamka = true;
	char szObj[32] = "sensor";
	p->nEqual = -1;
	p->nNoEqual = -1;
	p->bGreen = false;
	p->nHS = -1;
	p->nKlapan = -1;
	p->bIsGreen = false;
	char szHS[64] = "";
	char szKlapan[64] = "";
	char szSFcom[64] = "\0";
	p->nFloat = 0;
	p->bHS = false;
	p->bNoFillAlarm = false;
	p->bValueBoldPlus2 = false;
	p->bGray = false;
	p->dScale = 1;
	p->showAlways = 0;
	p->bDubler = false;
	p->bShowValue = true;
	// Not original. For C# Draw
	std::memset(p->DrawValue, 0, sizeof(p->DrawValue));
	p->NoDraw = false;
	p->DrawGreyRect = false;
	p->ShowElement = false;
	p->EdgeColor = 0;
	p->TextColor = 0;
	//
	STegObj obj[] =
	{
	{ 'I', "edge" , &p->mEdge    },
	{ 'A', "txt"  , &p->mTxt     },
	{ 'A', "bkg"  , &p->mBkg     },
	{ 'W', "align", &p->mAlign   },
	{ 'S', "text" ,  p->szText,31},
	{ 'S', "meas" ,  p->szMeas,31},
	{ 'S', "units" ,  p->szUnits,31},
	{ 'S', "obj"  ,  szObj,31},
	{ 'B', "colorEdge",&p->bColorEdge},
	{ 'B', "ramka",&p->bRamka},
	{ 'A', "color"  , &p->mColor     },
	{ 'I', "equal", &p->nEqual },
	{ 'I', "noequal", &p->nNoEqual },
	{ 'B', "green", &p->bGreen },
	{ 'B', "gray", &p->bGray },
	{ 'S', "view", szHS, 63 },
	{ 'S', "viewKlapan", szKlapan, 63 },
	{ 'L', "hs", &p->bHS },
	{ 'L', "noFillAlarms", &p->bNoFillAlarm },
	{ 'L', "valueBoldPlus2", &p->bValueBoldPlus2 },
	{ 'S', "suffcommon", szSFcom, 63 },
	{ 'D', "scale" ,  &p->dScale },
	{ 'L', "showAlways", &p->showAlways},
	// Дополнительная информация
	EXT_QUEUE
	};
	::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	// Форматы: (Name.Param) | (Name) | (.Param) | (.)
	// Текущий активный элемент

	if (p->mAlign == 0)
	{
		p->mAlign = DT_CENTER;
	}

	static char* DEF = "PV";
	UINT eType = 0;
	char* parm = "";
	if (!*szSFcom)
	{
		bool bActive = *teg[0].value == '.';
		parm = strchr(teg[0].value, '.');
		if (parm == NULL) parm = DEF;
		else *parm++ = 0;
		if (bActive)
		{
			p->nNumber = def.GetActiveObj();
			eType = def.GetMnemoType(p->nNumber);
		}
		else
		{
			eType = TextToType(szObj);
			if (p->bHS)
			{
				eType = id_HS;
			}

			p->nNumber = def.AddKindExPipe(eType, teg[0].value);
		}

		if (p->bColorEdge)
		{
			p->nTeg = ::FindTagEx(teg[0].value, eType);
		}
	}
	else
	{
		//здесь добавляется суффикс к переменной
		char teg0Value[256];
		strcpy_s(teg0Value, teg[0].value);
		strcat_s(teg0Value, szSFcom);
		bool bActive = *teg0Value == '.';
		parm = strchr(teg0Value, '.');
		if (parm == NULL)
		{
			parm = DEF;
		}
		else
		{
			*parm++ = 0;
		}

		if (bActive)
		{
			p->nNumber = def.GetActiveObj();
			eType = def.GetMnemoType(p->nNumber);
		}
		else
		{
			eType = TextToType(szObj);
			if (p->bHS)
			{
				eType = id_HS;
			}

			p->nNumber = def.AddKindExPipe(eType, teg0Value);
		}

		if (p->bColorEdge)
		{
			p->nTeg = ::FindTagEx(teg0Value, eType);
		}
	}

	p->eType = eType;
	p->defOut = NameToValue(eType, parm);
	ASSERT(p->defOut != NULL);
	p->rect = p->mRect;
	if (*p->szUnits)
	{
		if (strcmp(p->szUnits, "PROCENT") == 0)
		{
			strcpy(p->szUnits, "%");
		}
		else if ((strcmp(p->szUnits, "С") == 0) || (strcmp(p->szUnits, "C") == 0))
		{
			char szTempC[32] = "\xB0";
			strcat(szTempC, p->szUnits);
			strcpy(p->szUnits, szTempC);
		}
		else if (strcmp(p->szUnits, "м") == 0)
		{
			char szTempC[32] = "\xB5";
			strcat(szTempC, p->szUnits);
			strcpy(p->szUnits, szTempC);
		}

		strcat(p->szUnits, " ");
	}

	if (*szHS)
	{
		p->nHS = def.AddKindPipe(id_HS, szHS);
	}

	if (*szKlapan)
	{
		p->nKlapan = def.AddKind(id_Klapan, szKlapan);
	}

	if (p->mEdge != 0)// Чтобы рамка не наезжала на текст
	{
		::InflateRect(&p->rect, -2, -2);
	}

	// vladexl if ( !::IsMnemoStyle(MNEMO_EDIT) )
	::SetRectEmpty(&p->mRect);

	managed::RegisterElement(teg->name, p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
