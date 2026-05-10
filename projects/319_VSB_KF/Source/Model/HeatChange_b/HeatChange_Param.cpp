#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  kMix_trub = kMix_vol = 1.;
  Xi_Trub = 1.;
  Xi_Vol = 1.;
  k_Trub = 1.;
  k_Vol = 1.;
	}


int CHeatChange::GetParams(  char * StrName )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  TAB("Теплообмен",5)
  Heat.GetParams( NULL );
  ETAB
//  PARM( Heat.Err_Tz, "Задание невозможно выполнить" )
  CTRL_Params( "Задания", this );
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CHeatChange::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &kAntiFlow )
    {
    if ( kAntiFlow < 0. )
      kAntiFlow = 0.;
    if ( kAntiFlow > 1. )
      kAntiFlow = 1.;
    Heat.kAntiFlow = 1. - kAntiFlow;
    return 0;
    }
  if ( Param.Addr == &Calc_K_trub && Calc_K_trub == 1 )
    {
    CalcEtalonTrub( );
    return 0;
    }
  if ( Param.Addr == &Calc_K_vol && Calc_K_vol == 1 )
    {
    CalcEtalonVol( );
    return 0;
    }
  return 0;
  }

//#include "ShowParams.h"

int CHeatChange::ShowParams( DWORD DataType, struct CShowData * pSD )
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


void CHeatChange::DrawObj ( struct CDrawObjData * pDraw )
{
	if(bRegTask)
	{
		static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 )); 
		static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH ); 
		HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
		HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
		Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
		Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
		SelectObject ( pDraw->hDC, hOldPen );
		SelectObject ( pDraw->hDC, hOldFon );
	}
}