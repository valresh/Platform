#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

void CVolBase::Static( double dt )
  {
	if ( FullInitN2 )
    {
		VolInit ( pN2, 1., T_air );
    }
	if ( kStep % 10 == 0 )
		ЕстьЗадания = false;
	if ( kStep % 50 == 0 )
		Compress();
//
  SET_BP_SUB
  SummFlow( dt );
//
////////////////////////////////////////////////////////////////////
/////////
	S.Q = Q_ext;
  S.Q_Cool = k_Cool * ( T_air - T );
  S.Q += S.Q_Cool;
//
	if ( Delete )
    DeleteComp( dt );
//
////////////////////////////////////////////////////////
  SET_BP_SUB

  SET_BP_SUB
  Calc_Mix( Static_Vout, Static_Lout, S.Summ_FS_Gas, S.Summ_FS_Liq );
//
  if ( Calc_2F )
    Calc2F( );
  else
    Is_2F = false;
//
  double Ksi = RP * ( T + TK ) / ( Vol * KSi_P * Gas_Vol );
  if ( Static_Fix_P )
    {
    P = Static_P_fiz + 1.;
    Static_Fix_P_Fout = Static_Vout;
    P_Start_Iter_P = P;
    }
  else
    {
    if ( P > 3. * P_Start_Iter_P || P < 0.3 * P_Start_Iter_P )
      Static_Iter_P = false;
    if ( Static_Iter_P && S.dF_Gas_out > 1. )
      {
//      double K = Static_Vout / S.dF_Gas_out;
//      K = LimdX ( K - 1., 0.01 ) + 1.;
//      P *= K;
      double dF = Static_Vout - S.dF_Gas_out;
      double dM = LimdX( dF * 0.0001, M_Gas * 0.1 );
      M_Gas += dM;
      P = M_Gas * Ksi;
      }
    else
      {
       double dF = Static_Vout - S.dF_Gas_out;
      double dP = LimdX( dt * dF * Ksi, 0.2 );
      P += dP;
      P_Start_Iter_P = P;
      }
    M_Gas = P / Ksi;
    }
//
  if ( Static_Fix_L )
    {
    if ( Is_2F )
      {
      M3_liq_1	= To_m3 ( T, IsComp, Cmol_1F	);
      M3_liq_2	= To_m3 ( T, IsComp, Cmol_2F	);
      Static_Fix_L_Flow_1 = Static_Lout * Eps_1F;
      Static_Fix_L_Flow_2 = Static_Lout - Static_Fix_L_Flow_1;
      }
    else
      {
      Static_Fix_L_Flow_1 = Static_Lout;
      Static_Fix_L_Flow_2 = 0.;
      }
    }
  else
    {
    //if ( strstr ( ObjName, "Ф-102" ))
    //  KKK();
    double dM_liq = dt * ( Static_Lout - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost );
    const double D_max = 0.05;
    double dM_liq_max = M_Liq * D_max;
    if ( dM_liq > dM_liq_max && M_Liq > 1. )
      {
      dt *= dM_liq_max / dM_liq; 
      dM_liq = dM_liq_max;
      }
    if ( dM_liq < -dM_liq_max && M_Liq > 1. )
      {
      dt *= -dM_liq_max / dM_liq; 
      dM_liq = -dM_liq_max;
      }
    M_Liq += dM_liq;
    if ( M_Liq < 0. )
      M_Liq = 0.;
    }
///
  if ( Calc_2F )
    Calc2F( );
  else
    Is_2F = false;
//
  if ( Static_Fix_L )
    {
    dMdt_mol = 0.;
    Static_Set_Lev( 0.5 );
    }
  else
    {
    if ( Объема_2 && Calc_2F )
      Level_2Vol( dt );
    else
      Level_1Vol( dt );
    }
//
  if ( T_Z != 0. && ( pSys->bWorkReg || kStepTz > 0 ) && !pSys ->bInit_0 )
    {
    ЕстьЗадания = true;
    T = T_Z;
    }
  else
    {
    if ( Fast_T )
      {
      Fast_Calc_T( T, S.Q + S.dQ_mix );
      if ( T < T_min )
        T = T_min;
      }
    else
      {
      CalcT( dt );
      }
    }
  SET_BP_SUB
  CalcRoH( );
  if ( Add_1F || Add_2F )
    Add();
//
  CalcBalance( dt );
  }
