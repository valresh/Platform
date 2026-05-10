#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagShpX.h"
#include "../Extensions.h"

CTagShpX::CTagShpX(int n) { mParam = n; }

void CTagShpX::Analiz( ANALIZ ) { CTagDIV::Analiz( html, teg ); }

void CTagShpX::Kerdyk( KERDYK )
{
	Clear();	
	m_numberOfShapesAnimated = -2;
	Shape( html );
	ASSERT( m_numberOfShapesAnimated == 2 );
	ASSERT( m_useFirstShapeForBadValue == 0 );
	CTag::Kerdyk( hWnd, html );
	char item[16];
	sprintf(item, "SELXFL[%d]", mParam);
	mVal = FindExperion( html, html.m_strObj, item );
}
void CTagShpX::PaintThis( LOCALS )
{
	if ( m_numberOfShapesAnimated == -1 ) return; 
	CStyle& style = Style(html);
	char* id = style.NameID( html );
	BYTE val = html.pipe.ValueB_H(mVal);
	INT_PTR nCount = GetSize();
	CTag** obj = GetData();
	if ( val == 0xff ) { }
	else if ( nCount == 2 )
	{
		int n = (val == mParam) ? 1 : 0;
		obj[val]->PaintThis( hDC, html );
	}
	else ASSUNO;
}
//

CTagScShpX::CTagScShpX(int n) { mParam = n; }
//
void CTagScShpX::Analiz( ANALIZ ) { CTagDIV::Analiz( html, teg ); }
//
void CTagScShpX::Kerdyk( KERDYK )
{
	Clear();	
	m_numberOfShapesAnimated = -2;
	Shape( html );
	ASSERT( m_numberOfShapesAnimated == 2 );
	ASSERT( m_useFirstShapeForBadValue == 0 );
	CTag::Kerdyk( hWnd, html );
	char item[16];
	sprintf(item, "SELXFL[%d]", mParam);
	mVal = FindExperion( html, html.m_strObj, item );
	mVEdit = FindExperion( html, html.m_strObj, "SELXINP" );
}

void CTagScShpX::Action( CHTML& html, HWND hWnd, bool bDown,CPoint * pt )
{
	int nOld = html.pipe.ValueI_H(mVEdit);
	if (nOld != mParam)
	{
		int nNew=mParam;
		SSendToModel send;
        lstrcpy( send.szValue, mVEdit.def->name );
        send.Set( enumValueInt, &nOld, &nNew );
        html.pipe.SendData( mVEdit.nNumber, send, true );
	}
}
