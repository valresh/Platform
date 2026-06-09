#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  k_Flow = 1.;
  }


int CSepLiq::GetParams( char * )
	{
	#include "IO_Parms.h"
#define SHOW
	#include "Points.h"
#undef SHOW
  //if ( State[0] )
  //  {
  //  TAB("Вход газа",1)
  //    State[0]->GetParams( NULL );
  //  ETAB
  //  }
  //if ( State[1] )
  //  {
  //  TAB("Выход газа",1)
  //    State[1]->GetParams( NULL );
  //  ETAB
  //  }
  //if ( State[2] )
  //  {
  //  TAB("Выход жидкости",1)
  //    State[2]->GetParams( NULL );
  //  ETAB
  //  }
  CTRL_Params( "Задания", this );
  //if ( pPW )
  //  {
  //  TAB("Вода", 3 )
  //  pPW->GetParams( this );
  //  }
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//  LevCtrl.GetParams( this );
//	TAB("Модули",5)
	return 0;
	}

int CSepLiq::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &Volume )
    {
    double W = Volume;
	if(VolumeZ > 0.0)
		Volume = VolumeZ;
    VolumeZ = W;
    }
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

void CSepLiq::DrawState ( void * pData )
  {
  CDrawObjData * pDraw = (CDrawObjData *)pData;
  if ( Reg_L.IsOn() )
    DrawBorder ( pData );
  }
//#include "ShowParams.h"

int CSepLiq::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }

HBRUSH hFon = CreateSolidBrush( RGB ( 142, 142, 142 )); 
HBRUSH hLev = CreateSolidBrush( RGB ( 192, 0, 0 )); 
HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 0, 255 )); 
void CSepLiq::DrawObj ( struct CDrawObjData * pDraw )
{
	if(g_bInstructor)
		return;
	DrawState( pDraw );
	HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
	HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
	int W = pDraw->rc.right - pDraw->rc.left;
	int H = pDraw->rc.bottom - pDraw->rc.top;
	int w = W / 7;
	if ( w < 10 )
		w = 10;
	int h = H - H / 5; 
	if ( h < 20 )
		h = 20;
	int x0 = pDraw->rc.right + W / 10;//+ w * pDraw->rc.right / 1000 ;
	int y0 = pDraw->rc.top + H / 10;//+ pDraw->rc.top / 5 ;
	if ( !_finite(_Level))
		_Level = 0.;
	double dL = 1. - _Level * 0.01;
	if ( dL < 0. )
	  dL = 0.;
	int dhl_1 = (int)( dL * h ); 
	Rectangle ( pDraw->hDC, x0, y0, x0 + w, y0 + h ); 
	SelectObject ( pDraw->hDC, hLev );
	Rectangle ( pDraw->hDC, x0, y0 + dhl_1, x0 + w, y0 + h ); 
	SelectObject ( pDraw->hDC, hOldFon );
	SelectObject ( pDraw->hDC, hOldPen );
}
