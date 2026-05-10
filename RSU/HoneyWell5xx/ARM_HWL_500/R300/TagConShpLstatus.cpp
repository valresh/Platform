#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConShpLstatus.h"
#include "../Extensions.h"
#include "Scripts127.h"

CTagConShpLstatus::CTagConShpLstatus(int n)
{
	mParam = n;
}
void CTagConShpLstatus::Kerdyk ( KERDYK )
{
	CTag::Kerdyk(hWnd, html);
}
void CTagConShpLstatus::PaintThis( LOCALS )
{
	Matrix(hDC, html);
	if (Style(html).m_eVisible == enumHidden) return;
	CTagDIV::PaintThis(hDC, html);
}
void CTagConShpLstatus::Matrix( LOCALS )
{
	const COLORREF green = 0x0000ff00;
	const COLORREF red = 0x000000ff;
	const COLORREF grey = 0x007f7f7f;
	const COLORREF black = 0;
	//BI1005
	char pnt[32];
	if (mParam == 1) sprintf(pnt, "%s.DIGACQA", html.m_strGroup);
	else sprintf(pnt, "%s.DIGACQA_%d", html.m_strGroup, mParam-1);
	char tbname[16]; sprintf(tbname, "txtblock%d", mParam);
	SParamValueH parm = FindExperion(html, pnt, "NORMAL");
	CTag* textblock = shape_get_object(html, this->m_pPar, tbname);
	char oval_name[32]; sprintf(oval_name, "%s_oval002", this->Style(html).NameID(html));
	CTag* oval = shape_get_object(html, this, oval_name);
    if(!oval)
       KKK(); //не везде эти эдементы такие
	if (parm.def)
	{
		BYTE b = html.pipe.ValueB_H(parm);
		if (b == 2)
		{
			SParamValueH pvfl = FindExperion(html, pnt, "PVFL");
			BYTE c = html.pipe.ValueB_H(pvfl);
			if (c == 0)
			{
				//this.value = 2;
				oval->Style(html).m_clrFill = green;
			}
			else
			{
				//this.value = 3;
				oval->Style(html).m_clrFill = grey;
			}
			//txtblock1.styleClass = "Txt_Label_Normal";
			sprintf(textblock->m_szScriptValue, " ");
		}
		else
		{
			SParamValueH pvfl = FindExperion(html, pnt, "PVFL");
			BYTE c = html.pipe.ValueB_H(pvfl);
			if ((b == 0) && (c == 0))
			{
				//this.value = 3;
				//txtblock1.styleClass = "Txt_Label_Normal";
				oval->Style(html).m_clrFill = grey;
				sprintf(textblock->m_szScriptValue, " ");
			}
			else if ((b == 1) && (c == 1))
			{
				//this.value = 3;
				//txtblock1.styleClass = "Txt_Label_Normal";
				oval->Style(html).m_clrFill = grey;
				sprintf(textblock->m_szScriptValue, " ");
			}
			else
			{
				//this.value = 4;
				oval->Style(html).m_clrFill = red;
				textblock->Style(html).m_eVisible = enumVisible;
				if (c == 0)
				{
					SParamValueH stt = FindExperion(html, pnt, "STATETEXT(1)");
					char * state = html.pipe.ValueS_H(stt);
					sprintf(textblock->m_szScriptValue, "%s", state);
				}
				else if (c == 1)
				{
					SParamValueH stt = FindExperion(html, pnt, "STATETEXT(0)");
					char * state = html.pipe.ValueS_H(stt);
					sprintf(textblock->m_szScriptValue, "%s", state);
				}
			}
		}
	}
	else if(oval)
	{
		//this.value = 1;
		oval->Style(html).m_clrFill = black;
		//textblock->Style(html).m_eVisible = enumHidden;
		sprintf(textblock->m_szScriptValue, " ");
		//Style(html).m_eVisible = enumHidden;
	}
	CTagDIV::Matrix(hDC, html);
}
