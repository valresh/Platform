#include "stdafx.h"
#include "PPK.h"
#include "Err.h"
#include "CommProc.h"
#include "Chem.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


// Жидкость
static double def_Ro_std_liq = 1000.;// кг/м3
static double def_Mu_std_liq = 18.;
static double def_Dy_std_liq = 250.;// мм
static double def_Q_std_liq = 500.; // м3/ч
static double def_dP_std_liq = 2.;
// Газ
static double def_T_std_gas = 298.15; // К
static double def_Mu_std_gas = 16.;
static double def_Dy_std_gas = 200.;// мм
static double def_Q_std_gas = 50000; // нм3/ч
static double def_Pin_std_gas = 30.;
static double def_dP_std_gas = 0.1;
// Жидкость
double CPPK::Ro_std_liq = def_Ro_std_liq;// кг/м3
double CPPK::Mu_std_liq = def_Mu_std_liq;
double CPPK::Dy_std_liq = def_Dy_std_liq;// мм
double CPPK::Q_std_liq = def_Q_std_liq; // м3/ч
double CPPK::dP_std_liq = def_dP_std_liq;
// Газ
double CPPK::Dy_std_gas = def_Dy_std_gas;// мм
double CPPK::Q_std_gas = def_Q_std_gas; // нм3/ч
double CPPK::dP_std_gas = def_dP_std_gas;
//
double CPPK::T_std_gas = def_T_std_gas; // К
double CPPK::Mu_std_gas = def_Mu_std_gas;
double CPPK::Pin_std_gas = def_Pin_std_gas;

void CPPK::CalcK_PPK( )
  {
  NewParams = false;
  if ( GasPPK )
    {
    // Omega = K * V                           Ksi < 0.5   Q = Omega * dP
    // Q = K * 0.5 * Pin * V / sqrt ( Mu )     Ksi >= 0.5
    double Q_mol = Q_std_gas * iM3;
    K_PPK = К_Reg * Q_mol * Dy * Dy * sqrt ( Mu_std_gas ) / 
      ( sqrt ( Mu ) * Dy_std_gas * Dy_std_gas * dP_std_gas );
    return;
    }
  double Q_mol = Q_std_liq * Ro_std_liq / Mu_std_liq;
  K_PPK = К_Reg * Q_mol * Dy * Dy * sqrt ( Ro_liq ) * Mu_std_liq / 
    ( sqrt ( Ro_std_liq ) * Mu * Dy_std_liq * Dy_std_liq * dP_std_liq );
  }

char CPPK::ErrObj[128];
bool CPPK::WorkPPK = false;


int CPPK::GetParams( char * StrName )
	{
#include "IO_Parms.h"
#include "Points.h"
  PARM_S ( ErrObj, 128, "#Срабатывание ППК", "" )
  PARM ( WorkPPK, "Работа ППК" )
  Defect_Params(this);

	return 0;
	}

int CPPK::UpdateParam( CParams & Param )
  {
  NewParams = true;
  if ( Param.Addr == &SetP && SetP )
    {
    SetP = false;
    P_PPK = kP * ( P_In - P_Out );
    return 0;
    }
  CalcK_PPK( );
  return 0;
  }

void CPPK::CalcParms()
  {
  }

//#include "ShowParams.h"

int CPPK::ShowParams( DWORD DataType, struct CShowData * pSD )
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

HBRUSH hFonN = (HBRUSH)GetStockObject( NULL_BRUSH ); 
HBRUSH hFonOn = CreateSolidBrush( RGB ( 255, 255, 255 )); 
HPEN hPenRed = CreatePen(PS_SOLID,2,RGB ( 255, 0, 0 )); 
HBRUSH hZFon = CreateSolidBrush( RGB ( 255, 255, 0 )); 
HBRUSH hZFon2 = CreateSolidBrush( RGB ( 0, 0, 255 )); 
HPEN hZPen1 = CreatePen(PS_SOLID,1,RGB ( 255, 255, 0 )); 
HPEN hZPen2 = CreatePen(PS_SOLID,1,RGB ( 0, 0, 255 )); 

void CPPK::DrawObj ( struct CDrawObjData * pDraw )
  {
	  if(g_bInstructor)
		  return;
  if ( pDraw->ObjName[0] == '.' && pDraw->ObjName[1] == 'П' )
    {
    if ( !Open && P_PPK != 400. && ( Dy != 49. || К_Reg != 1. ))
      return;
    int R = ( pDraw->rc.right - pDraw->rc.left ) / 2;
    int H = ( pDraw->rc.bottom - pDraw->rc.top ) / 2;
    if ( H > R )
      R = H;
    R += 5;
    int xc = ( pDraw->rc.right + pDraw->rc.left ) / 2;
    int yc = ( pDraw->rc.top + pDraw->rc.bottom ) / 2;
    //
    HBRUSH hOldFon;
    if ( Open )
      hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFonOn );
    else
      hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFonN );
    HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPenRed );
    Ellipse ( pDraw->hDC, xc-R, yc-R, xc+R, yc+R );
    //
    SelectObject ( pDraw->hDC, hOldFon );
    SelectObject ( pDraw->hDC, hOldPen );
    return;
    }
  }
