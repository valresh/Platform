#include "stdafx.h"
#define DLL_COLUMN
#include "Column.h"
#include "Err.h"
#include "CommProc.h"


void CColumn::Calc( double dt )
	{
  SET_BP BreakPoint;
  //if(::T_min != T_MIN)
  //  Column.T_min = ::T_min;
  //if(::T_max != T_MAX)
	 // Column.T_max = ::T_max;
  int n_H2O = G_H2O;
  Column.pStage[kStage]->Lev_Z = Lev_CubZ; 
  Column.Step( dt );
  if ( GasColumn )
    {
    double Flow_In = pFlow_Gas_in->Flow_mol;
    double Flow_Out = pFlow_Gas_out->Flow_mol;
    double Flow_1 = Column.Flow_1;
    if ( Flow_In > 1. )
      kGasOut = 0.9 * kGasOut + 0.1 * Flow_1 / Flow_In;
    else
      kGasOut = 0.9 * kGasOut + 0.1;
//    if ( kGasOut > 1. )
//      kGasOut = 1.;
    //double Test = -Flow_Out / Flow_1;
    //KKK();
    }
  P_низа = Column.pStage[kStage]->P - 1.;
  P_верха = Column.pStage[1]->P - 1.;
	T_верха = Column.pStage[1]->T_liq;
	T_низа = Column.pStage[kStage]->T_liq;
  double L = Column.pStage[kStage]->Level;
//  Lev_1 = Lev_0_100( L, 0.1, 0.9 ) * 100.;
//  Lev_2 = Lev_0_100( L, 0.1, 0.9 ) * 100.;
	L_куб = L * 100.;
  if ( pLevCtrl )
    pLevCtrl->Control( dt );
//
  if ( L < 0.1 )
    {
const double nL = 1. / 0.07;
    if ( L < 0.03 )
      dP_hydro = 0.;
    else
      dP_hydro = nL * ( L - 0.03 ) * dP_hydro_stat;
    }
  else
    {
const double k = 1. / 0.9;
    dP_hydro = ( L - 0.1 ) * k * dP_hydro_lev + dP_hydro_stat;
    }
  for ( int n = 0; n < kIO; n++ )
    {
    if ( Hydro_dPmax[n] <= 0. )
      continue;
    int N = nST[n];
    if ( N <= 0 )
      continue;
    double L = Column.pStage[N]->Level;
    if ( L < 0.1 )
      Hydro_dP[n] = 10. * L * Hydro_dPmax[n];
    else
      Hydro_dP[n] = Hydro_dPmax[n];
    }
	}
