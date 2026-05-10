#include "stdafx.h"
#include "Fan.h"
#include "Err.h"
#include "CommProc.h"
//#include "SoftGDI.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}

// Безразмерная F
double CFan::Hss( double Ro, double Q, double Omega )
  {
  double _Qmax = Qmax * Omega;
  if ( Q > _Qmax )
    return 0.;
  double q = Q / _Qmax;
  double h;
  if ( q < _Hwork )
    {
    h = 1.;
    }
  else
    {
    double x = ( q - _Hwork ) / ( 1. - _Hwork );
    h = 1. - x * x;
    }
//  double H = h * Ro * Hmax * Omega * Omega;
  double H = 0.;
  return H;
  }

void CFan::CalcParams( )
  {
  //if ( Qwork >= 0.9 * Qmax )
  //  Qwork = 0.9 * Qmax;
  //if ( Hmax <= 1.1 * Hmax )
  //  Hmax = 1.1 * Hmax;
  //double hnom = Hmax / Hmax;
  //double qnom = sqrt ( 1. - hnom );
  //Qmax = ( Qmax - Qwork ) / qnom + Qwork;
  //_Hwork = Qwork / Qmax;
  //if ( PowMax <= 0. )
  //  {
  //  PowMax = Hmax * 1e-4 * Pascal * Qmax;// кДж / ч
  //  }
  }


int CFan::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CFan::UpdateParam( CParams & Param )
  {
  if ( KPD_M > 100. )
    KPD_M = 100.;
  if ( KPD_M < 10. )
    KPD_M = 10.;
  Hmax.Update ( Param.Addr );
  H_real.Update ( Param.Addr );
  H_fact.Update ( Param.Addr );
  H_zad.Update ( Param.Addr );
  CalcParams( );
  Calc_K_P2_P1( );
  if ( Use_X )
    СпадРаб( );
  else
    SetРаб ( );
  return 0;
  }

//#include "ShowParams.h"

int CFan::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }

void TransparentRect   ( HDC hDC, RECT& rc, COLORREF frg, COLORREF bkg );

HPEN hPenErr = CreatePen(PS_SOLID,3,RGB ( 0, 0, 255 )); 

void CFan::DrawObj ( CDrawObjData * pDraw )
  {
	  if(g_bInstructor)
		  return;
  COLORREF Col = RGB ( 255, 0, 0 );
  if ( On )
    Col = RGB ( 0, 255, 0 );
  TransparentRect ( pDraw->hDC, pDraw->rc, Col, RGB(1,2,3));
  if ( ErrInBlk )
    {
    // Блокировки изменят положение
    HPEN Old = (HPEN)SelectObject ( pDraw->hDC, hPenErr );
    MoveToEx ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.top - 10, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.bottom + 10 );
    MoveToEx ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.top - 10, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.bottom + 10 );
    SelectObject ( pDraw->hDC, Old );
    }
  }

