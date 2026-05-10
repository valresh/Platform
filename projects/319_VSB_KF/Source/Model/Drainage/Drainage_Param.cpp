#include "stdafx.h"
#include "Drainage.h"
#include "Err.h"
#include "CommProc.h"
#include "SoftGDI.h"


_W_Drain::_W_Drain()
	{
  memset ( this, 0, sizeof ( _W_Drain ));
	}

// Жидкость
double CDrainage::Ro_std_liq = 1000.0;// кг/м3
double CDrainage::Mu_std_liq = 18.0;
double CDrainage::Dy_std_liq = 25.0;// мм
double CDrainage::Q_std_liq = 10.0; // м3/ч
double CDrainage::dP_std_liq = 1.0;
// Газ
double CDrainage::Dy_std_gas = 25.0;// мм
double CDrainage::Q_std_gas = 500.0; // нм3/ч
double CDrainage::dP_std_gas = 1.0;
//
double CDrainage::T_std_gas = 298.15; // К
double CDrainage::Mu_std_gas = 16.0;
double CDrainage::Pin_std_gas = 30.0;

void CDrainage::CalcK( )
  {
  if ( Type == Газ )
    {
    // Omega = K * V                           Ksi < 0.5   Q = Omega * dP
    // Q = K * 0.5 * Pin * V / sqrt ( Mu )     Ksi >= 0.5
    double Q_mol = Q_std_gas * iM3;
    K_Drain = К_Reg * Q_mol * Dy * Dy * sqrt ( Mu_std_gas ) / 
      ( sqrt ( Mu ) * Dy_std_gas * Dy_std_gas * dP_std_gas );
    return;
    }
  double Q_mol = Q_std_liq * Ro_std_liq / Mu_std_liq;
  K_Drain = К_Reg * Q_mol * Dy * Dy * sqrt ( Ro_liq ) * Mu_std_liq / 
    ( sqrt ( Ro_std_liq ) * Mu * Dy_std_liq * Dy_std_liq * dP_std_liq );
  }

int CDrainage::GetParams( char * StrName )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  Defect_Params(this);
  ACS_Params( "АСУ", this );
	return 0;
	}

int CDrainage::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &Ventil )
    {
    return 0;
    }
  CalcK( );
  return 0;
  }

//#include "ShowParams.h"

int CDrainage::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
    case sp_Actions:
      {
      if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
        {
        if ( Ventil == 0. )
          Ventil = 100.;
        else
          Ventil = 0.;
        return 0;
        }
      }
		return 1;
		}
  return 0;
  }

COLORREF ColZadv ( double Vent )
  {
  if ( Vent <= 0. )
    return RGB( 255, 0, 0 );
  if ( Vent <= 10. )
    return RGB( 255, 150, 0 );
  if ( Vent <= 50. )
    return RGB( 255, 255, 0 );
  return RGB( 0, 255, 0 );
  }

void CDrainage::DrawObj ( struct CDrawObjData * pDraw )
  {
  if(g_bInstructor)
    return;
  pDraw->rc.top -= 5;
  pDraw->rc.left -= 5;
  pDraw->rc.right += 2;
  // TransparentStd( pDraw->hDC, pDraw->rc, ColZadv ( Ventil ));
  }
