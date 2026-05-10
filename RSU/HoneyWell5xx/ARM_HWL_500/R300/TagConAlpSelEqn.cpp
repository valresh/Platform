#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagConAlpSelEqn.h"
#include "../Extensions.h"

CTagConAlpSelEqn::CTagConAlpSelEqn(int n) { mParam = n; }

void CTagConAlpSelEqn::Analiz( ANALIZ ) { CTagDIV::Analiz( html, teg ); }

void CTagConAlpSelEqn::Kerdyk( KERDYK ) { CTag::Kerdyk( hWnd, html ); }

static bool ends_with(char* haystack, char* needle)
{
	int p = (int)strlen(haystack) - (int)strlen(needle);
	return ((p >= 0) && (_strnicmp(haystack + p, needle, strlen(needle)) == 0));
}

static CTag* _get_object(CHTML& html, CTag* object, char * name)
{
	INT_PTR nCount = object->GetSize();
	CTag** obj = object->GetData();
	for ( INT_PTR n = 0; n < nCount; n++ )
	{
		CStyle& s = html.Style(obj[n]->m_nStyle);
		char* id = s.NameID(html);
		if (ends_with(id, name)) return obj[n];
		else if (obj[n]->GetSize() > 0)
		{
			CTag * inn = _get_object(html, obj[n], name);
			if (inn) return inn;
		}
	}
	return 0;
}

void CTagConAlpSelEqn::Matrix( LOCALS )
{
	CStyle& style = Style(html);
	char* id = style.NameID( html );
	if ( style.m_eVisible == enumHidden )
	{
		BYTE b = html.pipe.ValueB_H( mVal );
		if (!this->m_pPar->m_pPar) return;
		
		CTag* GrpB  = _get_object(html, this->m_pPar->m_pPar, "GrpB");
		CTag* GrpRB = _get_object(html, this->m_pPar->m_pPar, "GrpRB");
		
		if (!GrpB || !GrpRB) return;
		
		CStyle& GrpB_style  = GrpB->Style(html);
		CStyle& GrpRB_style = GrpRB->Style(html);
		
		if (b == 255) return;
		else if (b > 1)
		{
			GrpB_style.m_eVisible = enumVisible;
			GrpRB_style.m_eVisible = enumHidden;
		}
		else
		{
			GrpB_style.m_eVisible = enumHidden;
			GrpRB_style.m_eVisible = enumVisible;
		}
	}
}
