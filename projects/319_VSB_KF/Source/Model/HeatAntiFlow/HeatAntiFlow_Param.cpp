#include "stdafx.h"
#include "HeatAntiFlow.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CHeatAntiFlow::GetParams( char *  )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  CTRL_Params( "Задания", this );
  ACS_Params( "АСУ", this );
  TAB("Теплообмен",4)
  Heat.GetParams( "" );
  ETAB
	return 0;
	}

int CHeatAntiFlow::UpdateParam( CParams & Param )
  {
  return 0;
  }

void DrawBorder ( void * pData )
{
	static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 )); 
	static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH ); 
	CDrawObjData * pDraw = (CDrawObjData *)pData;
	HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
	HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
	Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
	Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
	SelectObject ( pDraw->hDC, hOldPen );
	SelectObject ( pDraw->hDC, hOldFon );
}

void CHeatAntiFlow::DrawObj ( struct CDrawObjData * pDraw )
  {
  int xc = ( pDraw->rc.right + pDraw->rc.left ) / 2;
  int yc = ( pDraw->rc.top + pDraw->rc.bottom ) / 2;
	UINT OldAlign = SetTextAlign(	pDraw->hDC, TA_CENTER|TA_TOP );
	UINT OldCol = SetTextColor(	pDraw->hDC, RGB ( 255, 0, 0 ) );
	char Txt[256];
  int L = sprintf_s ( Txt, 255, "т:%5.1lf->%5.1lf", _T_Trub_in, _T_Trub_out );
	TextOut ( pDraw->hDC, xc, yc-10, Txt, L );
  L = sprintf_s ( Txt, 255, "м:%5.1lf->%5.1lf", _T_Vol_in, _T_Vol_out );
	TextOut ( pDraw->hDC, xc, yc+10, Txt, L );
	SetTextAlign(	pDraw->hDC, OldAlign );
	SetTextColor(	pDraw->hDC, OldCol );
//  if ( T_Z_Trub != 0. || T_Z_Vol != 0. || dP_trub_Z != 0. || 
//       dP_vol_Z != 0. || F_trub_Z != 0. || F_vol_Z != 0. )
  if ( Reg_T_Z_Trub.Use || Reg_T_Z_Vol.Use )
    DrawBorder ( pDraw );
  }

int CHeatAntiFlow::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
    // switch ( DataType )
    // 	{
    // 	case sp_Actions:
    // 		IF_ACTION("Tmin=")
    // 			{
    // 			T_min_Trub = T_min_Vol = atof ( StdActionParm );
    // 			sprintf_s ( pSD->Txt, 256, "Tmin = %3.1lf для %s",	T_min_Trub, ObjName );
    // 			return 0;
    // 			}
    // 	return 1;
    // 	}
  return 0;
  }
