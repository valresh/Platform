#include "stdafx.h"
#include "AVO.h"
#include "Err.h"
#include "CommProc.h"

#ifdef LINUX
#include "WinDraw.h"
#endif


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  kOmega = 1.;
  Jalousie_1 = 1.;
  Jalousie_2 = 1.;
  Jalousie_3 = 1.;
  Jalousie_4 = 1.;
  Jalousie_Eff = 1.;
	}

CParams * CAVO::pParams = NULL;
int CAVO::kParams = -1;

bool CAVO::GetParamsList( struct CParams ** ppParams, int * pkParams )
  {
  if ( kParams <= 0 )
    return false;
  *ppParams = pParams;
  *pkParams = kParams;
  return true;
  }

int CAVO::GetParams(  char * StrName )
	{
	#include "IO_Parms.h"
#define GET_PARAMS
	#include "Points.h"
#undef GET_PARAMS
  CTRL_Params( "Задания", this );
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//  TAB("Модули",5)
	return 0;
	}

int CAVO::UpdateParam( CParams & Param )
{
	return 0;
}

//#include "ShowParams.h"

int CAVO::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( "" );
		return 1;
		}
*/
  return 0;
  }


void CAVO::DrawFixed ( CDrawObjData * pDraw )
  {
  HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 ));
  HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH );
  HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
  Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
  SelectObject ( pDraw->hDC, hOldPen );
  SelectObject ( pDraw->hDC, hOldFon );
  }

 HFONT hFont = NULL;
// HPEN hPen4 = CreatePen(PS_SOLID,3,RGB ( 0, 0, 255 ));

void CAVO::DrawObj ( struct CDrawObjData * pDraw )
  {
  if(g_bInstructor)
	  return;
  //DrawFixed ( pDraw );
  CDrawObjData * pInfo = pDraw;
  RECT rc = pInfo->rc;
  int w = rc.right - rc.left;
  int h = rc.bottom - rc.top;
  int m = w;
  if ( h < m )
    m = h;
  int f = m / 3;
  int x = ( rc.right + rc.left ) / 2;
  int y = ( rc.bottom + rc.top ) / 2;

  HFONT hFontOld;
  int Align, Color, BkColor, Mode;
  BkColor = ::SetBkColor ( pInfo->hDC, RGB( 0, 0, 0 ) );
  Align = ::SetTextAlign ( pInfo->hDC, TA_CENTER|TA_BASELINE);
  Color = ::SetTextColor ( pInfo->hDC, RGB( 255, 255, 0 ));
  Mode = ::SetBkMode ( pInfo->hDC, OPAQUE );
  if ( hFont == NULL )
    {
    hFont = ::CreateFont(
       -18,
       0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
       ANSI_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 5,
       VARIABLE_PITCH, "Arial" );
    }
   hFontOld = (HFONT)::SelectObject( pInfo->hDC, hFont );
   char Txt[256];
   _T_in = 125.;
   _T_out = 25.;
   int l = sprintf_s ( Txt, sizeof(Txt), "%1.1lf°C", _T_in );
   ::TextOut ( pInfo->hDC, x, y - f / 2, Txt, l );
   ::SetTextAlign ( pInfo->hDC, TA_CENTER|TA_TOP);
   l = sprintf_s ( Txt, sizeof(Txt), "%1.1lf°C", _T_out );
// //
   ::TextOut ( pInfo->hDC, x, y + f / 2, Txt, l );

   if ( strstr ( ObjName, "A-3001A"))
     KKK();
   ::SelectObject ( pInfo->hDC, hFontOld );
   ::SetTextAlign ( pInfo->hDC, Align );
   ::SetTextColor ( pInfo->hDC, Color );
   ::SetBkMode ( pInfo->hDC, Mode );
   ::SetBkColor ( pInfo->hDC, BkColor );
  }

void CAVO::AddResources (CGDIResourceMgr *pResMgr)
{
	AddFont (pResMgr, 0, RGB (255, 255, 0), -18, FALSE, _T("Arial"));
}

void CAVO::DrawObject (struct CDrawObjData *pDraw, CGDIResourceMgr *pResMgr)
{
	HFONT hCurFont = GetFont (pResMgr, pDraw->nModelID, 0);
	if ( !hCurFont )
	{
		DrawObj(pDraw);
		return;
	}
	RECT rc = pDraw->rc;
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	int m = w;
	if ( h < m )
		m = h;
	int f = m / 4;
	int x = ( rc.right + rc.left ) / 2;
	int y = ( rc.bottom + rc.top ) / 2;
	HGDIOBJ hFontOld;
	int Align, Color, BkColor, Mode;
	BkColor = ::SetBkColor ( pDraw->hDC, RGB( 0, 0, 0 ) );
	Align = ::SetTextAlign ( pDraw->hDC, TA_CENTER|TA_BASELINE);
	Color = ::SetTextColor ( pDraw->hDC, RGB( 255, 255, 0 ) );
	Mode = ::SetBkMode ( pDraw->hDC, OPAQUE );
	hFontOld = ::SelectObject ( pDraw->hDC, hCurFont );
	char Txt[256];
	int l = sprintf_s ( Txt, sizeof(Txt), "%1.1lf°C", _T_in );
	::TextOut ( pDraw->hDC, x, y - f / 2, Txt, l );
	::SetTextAlign ( pDraw->hDC, TA_CENTER|TA_TOP);
	l = sprintf_s ( Txt, sizeof(Txt), "%1.1lf°C", _T_out );
	::TextOut ( pDraw->hDC, x, y + f / 2, Txt, l );
	::SelectObject ( pDraw->hDC, hFontOld );
	::SetTextAlign ( pDraw->hDC, Align );
	::SetTextColor ( pDraw->hDC, Color );
	::SetBkMode ( pDraw->hDC, Mode );
	::SetBkColor ( pDraw->hDC, BkColor );
}

