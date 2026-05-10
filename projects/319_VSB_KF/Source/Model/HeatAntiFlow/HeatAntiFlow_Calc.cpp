#include "stdafx.h"
#include "HeatAntiFlow.h"
#include "Err.h"
#include "CommProc.h"


void CHeatAntiFlow::Calc( double dt )
	{
	Heat.In_0.T_min = T_min_Trub;
	Heat.Out_0.T_min = T_min_Trub;
	Heat.In_1.T_min = T_min_Vol;
	Heat.Out_1.T_min = T_min_Vol;         
	Heat.In_0.T_max = 2000.;
	Heat.Out_0.T_max = 2000.;
	Heat.In_1.T_max = 2000.;
	Heat.Out_1.T_max = 2000.;         
//
  SET_BP BreakPoint;
  double KS =  K * S * 3.6;
  if ( Heat_Defect )
    {
    KS *= k_Heat;
    if ( KS < 1. )
      KS = 1.;
    }
	if ( Reg_T_Z_Trub.Use )
		{
    double T_Z_Trub = Reg_T_Z_Trub;
    *Heat.pOut_0 = *Heat.pIn_0;
    *Heat.pOut_1 = *Heat.pIn_1;
		Heat.Cp_Corp_0 = 0.;
		Heat.Cp_Corp_1 = 0.;
		double Ab = 0.9;
		double Ae = 1.1;
		double A = 0.;
		bool Up;
		if ( Heat.T_out_0 > Heat.T_in_0 )
			Up = true;
		else
			Up = false;
    if ( K >= 999. )
      {
      double T = Heat.T_out_0;
      if ( Up )
        {
        if ( T < T_Z_Trub )
          A = 1.;
        }
      }
    if ( K < 1.001 )
      {
      double T = Heat.T_out_0;
      if ( !Up )
        {
        if ( T > T_Z_Trub )
          A = 1.;
        }
      }
    int kIter = 0;
    if ( A == 0. )
      {
 		  while ( Ae - Ab	> 1e-4 )
			  {
        kIter++;
			  A = 0.5 * ( Ae + Ab );
			  Heat.KS = KS * A;
			  Heat.Calc ( dt );
			  double T = Heat.T_out_0;
			  if ( Up )
				  {
				  if ( T > T_Z_Trub )
					  Ae = A;
				  else
					  Ab = A;
				  }
			  else
				  {
				  if ( T > T_Z_Trub )
					  Ab = A;
				  else
					  Ae = A;
				  }
			  }
      }
    else
      {
      Heat.KS = KS * A;
      Heat.Calc ( dt );
      }
    kIter;
		K *= A;
		if ( K < 1. )
			K = 1.;
		if ( K > 1000. )
			K = 1000.;
    Heat.KS = K * S * 3.6;;
    Heat.Calc ( dt );
		}
	else
		{
		if ( Reg_T_Z_Vol.Use )
			{
      double T_Z_Vol = Reg_T_Z_Vol; 
      *Heat.pOut_0 = *Heat.pIn_0;
      *Heat.pOut_1 = *Heat.pIn_1;
			Heat.Cp_Corp_0 = 0.;
			Heat.Cp_Corp_1 = 0.;
			double Ab = 0.9;
			double Ae = 1.1;
			double A = 0.;
			bool Up;
			if ( Heat.T_out_1 > Heat.T_in_1 )
				Up = true;
			else
				Up = false;
      if ( K >= 999. )
        {
        double T = Heat.T_out_1;
        if ( Up )
          {
          if ( T < T_Z_Vol )
            A = 1.;
          }
        }
      if ( K < 1.001 )
        {
        double T = Heat.T_out_1;
        if ( !Up )
          {
          if ( T > T_Z_Vol )
            A = 1.;
          }
        }
      if ( A == 0. )
        {
			  while ( Ae - Ab	> 1e-4 )
				  {
				  A = 0.5 * ( Ae + Ab );
				  Heat.KS = KS * A;
				  Heat.Calc ( dt );
				  double T = Heat.T_out_1;
				  if ( Up )
					  {
					  if ( T > T_Z_Vol )
						  Ae = A;
					  else
						  Ab = A;
					  }
				  else
					  {
					  if ( T > T_Z_Vol )
						  Ab = A;
					  else
						  Ae = A;
					  }
				  }
        }
			K *= A;
			if ( K < 1. )
				K = 1.;
			if ( K > 1000. )
				K = 1000.;
      Heat.KS = K * S * 3.6;;
      Heat.Calc ( dt );
			}
		else
			{
			Heat.KS = KS * VarK( );
			Heat.Calc ( dt );
			}
		}
	Cp_Trub = Heat.Cp_0;
	Cp_Vol = Heat.Cp_1;
  dP_Hydro = ( 1. - Heat.Out_1.E ) * dP_Hydro_max;
	_T_Trub_in = T_in_Trub = Heat.T_in_0;
  _T_Trub_out = T_out_Trub = Heat.T_out_0;
	_T_Vol_in = T_in_Vol = Heat.T_in_1;
	_T_Vol_out = T_out_Vol = Heat.T_out_1;
//
	_E_Trub_in = Heat.In_0.E;
	_E_Trub_out = Heat.Out_0.E;
	_E_Vol_in = Heat.In_1.E;
	_E_Vol_out = Heat.Out_1.E;
  P_def_inVol += kP_defP * ( Out_Trub->P - P_def_inVol );
  P_def_outVol += kP_defP * ( In_Trub->P - P_def_outVol );
  P_def_Trub += kP_defP * ( Out_Vol->P - P_def_Trub );
  P_def_outTrub += kP_defP * ( In_Vol->P - P_def_outTrub );
  if ( Перемешивание )
    {
    if ( Переток_Vol < 0. )
      {
// из аневризмы в объем
      if ( Vol_0->Flow_mol < 0. )
        {
        Vol_0->ID_COMP = ID_Comp_Mix;
        Vol_0->pComp = Comp_Mix;
        Vol_0->h_Flow = h_Mix;
        }
      if ( Vol_1->Flow_mol > 0. )
        {
        Vol_1->ID_COMP = ID_Comp_Mix;
        Vol_1->pComp = Comp_Mix;
        Vol_1->h_Flow = h_Mix;
        }
      }
    else
      {
// из объема в аневризму
      h_inVol = In_Vol->h_Flow;
      h_outVol = Out_Vol->h_Flow;
      Comp_Vol->Set( In_Vol->Get_Comp());
      }

    if ( Переток_Trub < 0. )
      {
// из аневризмы в трубы
      if ( Trub_0->Flow_mol < 0. )
        {
        Trub_0->ID_COMP = ID_Comp_Mix;
        Trub_0->pComp = Comp_Mix;
        Trub_0->h_Flow = h_Mix;
        }
      if ( Trub_1->Flow_mol > 0. )
        {
        Trub_1->ID_COMP = ID_Comp_Mix;
        Trub_1->pComp = Comp_Mix;
        Trub_1->h_Flow = h_Mix;
        }
      }
    else
      {
 // из труб в аневризму
      h_inTrub = In_Trub->h_Flow;
      h_outTrub = Out_Trub->h_Flow;
      Comp_Trub->Set( In_Trub->Get_Comp());
      }
    double P_Trub = In_Trub->P;
    if ( Out_Trub->P > P_Trub )
      P_Trub = Out_Trub->P;
    double P_Vol = In_Vol->P;
    if ( Out_Vol->P > P_Vol )
      P_Vol = Out_Vol->P;
    Переток_Trub = Def_Trub * ( P_Trub - P_Аневризма );  // направление > 0 в объем
    Переток_Vol = Def_Trub * ( P_Vol - P_Аневризма );    // направление > 0 в объем
    P_Аневризма += LimdX ( k_P_Аневризма * ( Переток_Trub + Переток_Vol ), 0.2 );
//
    FirstMix = true;
    if ( Переток_Trub > 0. )
      {
      if ( In_Trub->h_Flow > Out_Trub->h_Flow )
        Add( Переток_Trub, In_Trub->h_Flow, In_Trub->Get_Comp());
      else
        Add( Переток_Trub, Out_Trub->h_Flow, Out_Trub->Get_Comp());
      }
    if ( Переток_Vol > 0. )
      {
      if ( In_Vol->h_Flow > Out_Vol->h_Flow )
        Add( Переток_Vol, In_Vol->h_Flow, In_Vol->Get_Comp());
      else
        Add( Переток_Vol, Out_Vol->h_Flow, Out_Vol->Get_Comp());
      }
    Norm( IsCompMix, CmolMix );
    Comp_Аневризма->Set( IsCompMix, CmolMix );
    h_Аневризма = h_Mix /= F_Mix;
    }
  else
    {
    h_inVol = In_Vol->h_Flow;
    h_outVol = Out_Vol->h_Flow;
    h_inTrub = In_Trub->h_Flow;
    h_outTrub = Out_Trub->h_Flow;
    Comp_Vol->Set( In_Vol->Get_Comp());
    Comp_Trub->Set( In_Trub->Get_Comp());
    double P_Trub = In_Trub->P;
    if ( Out_Trub->P > P_Trub )
      P_Trub = Out_Trub->P;
    double P_Vol = In_Vol->P;
    if ( Out_Vol->P > P_Vol )
      P_Vol = Out_Vol->P;
    P_Аневризма = 0.5 * ( P_Trub + P_Vol );
    }
#if 0
  if ( F_trub_Z > 0. )
    {
    double F_trub	= fabs ( _F_Trub_kg );
    double K;
    if ( F_trub > 2. * F_trub_Z )
      K = 0.5;
    else
      if ( F_trub < 0.5 * F_trub_Z )
        K = 2.;
      else
        K = F_trub_Z / F_trub;
    OmegaTrub *= LimdX ( K - 1., Reg ) + 1.;
    if ( OmegaTrub > 1e6 )
      OmegaTrub = 1e6;
    if ( OmegaTrub < 1e-3 )
      OmegaTrub = 1e-3;
    }
  if ( dP_trub_Z > 0. )
    {
    double dP_trub	= fabs ( _dP_Trub );
    double K;
    if ( dP_trub > 2. * dP_trub_Z )
      K = 2.;
    else
      if ( dP_trub < 0.5 * dP_trub_Z )
        K = 0.5;
      else
        K = dP_trub / dP_trub_Z;
    OmegaTrub *= LimdX ( K - 1., Reg ) + 1.;
    if ( OmegaTrub > 1e6 )
      OmegaTrub = 1e6;
    if ( OmegaTrub < 1e-3 )
      OmegaTrub = 1e-3;
    }
  if ( F_vol_Z > 0. )
    {
    double F_vol	= fabs ( _F_Vol_kg );
    double K;
    if ( F_vol < 2. * F_vol_Z )
      K = 2.;
    else
      if ( F_vol > 0.5 * F_vol_Z )
        K = 0.5;
      else
        K = F_vol_Z / F_vol;
    OmegaVol *= LimdX ( K - 1., Reg ) + 1.;
    if ( OmegaVol > 1e6 )
      OmegaVol = 1e6;
    if ( OmegaVol < 1e-3 )
      OmegaVol = 1e-3;
    }
  if ( dP_vol_Z > 0. )
    {
    double dP_vol	= fabs ( _dP_Vol );
    double K;
    if ( dP_vol > 2. * dP_vol_Z )
      K = 2.;
    else
      if ( dP_vol < 0.5 * dP_vol_Z )
        K = 0.5;
      else
        K = dP_vol / dP_vol_Z;
    OmegaVol *= LimdX ( K - 1., Reg ) + 1.;
    if ( OmegaVol > 1e6 )
      OmegaVol = 1e6;
    if ( OmegaVol < 1e-3 )
      OmegaVol = 1e-3;
    }
#endif
//......................................................
  if ( k_HeatChange > 0 )
    {
    double Ti = T_in_Vol;
    double To = T_out_Vol;
    double dT = ( To - Ti ) / k_HeatChange;
    double T = Ti + dT;
    T_vol_1 = T;
    T += dT;
    T_vol_2 = T;
    T += dT;
    T_vol_3 = T;
    T += dT;
    T_vol_4 = T;
    T += dT;
    T_vol_5 = T;
    }
  if ( k_HeatChange > 0 )
    {
    double Ti = T_in_Trub;
    double To = T_out_Trub;
    double dT = ( To - Ti ) / k_HeatChange;
    double T = Ti;
    T_trub_1 = T;
    T += dT;
    T_trub_2 = T;
    T += dT;
    T_trub_3 = T;
    T += dT;
    T_trub_4 = T;
    T += dT;
    T_trub_5 = T;
    T += dT;
    T_trub_6 = T;
    }
  T_in_trub = T_in_Trub;
  T_out_trub = T_out_Trub;
  T_in_vol = T_in_Vol;
  T_out_vol = T_out_Vol;
  double _T_med_trub = T_in_Trub + aT_med_trub * ( T_out_Trub - T_in_Trub );
  double _T_med_vol = T_in_Vol + aT_med_vol * ( T_out_Vol - T_in_Vol );
  T_med_trub = _T_med_trub;
  T_med_vol = _T_med_vol;
  if ( T_wall.Use() )
    {
    double Q = Heat.Q_dg;
    double K = K_wall * S * 3.6;
    double dT = Q / K;
    double T_w = _T_med_trub + dT;
    if ( T_w > _T_med_vol )
      T_w = _T_med_vol;
    T_wall = T_w;
    }
	}

double CHeatAntiFlow::VarK( )
  {
  if ( !Переменный_теплообмен )
    return 1.;
  CFlow * pFlow;
  CComp * pComp;
  CSEps	* pState;
  if ( TV == 0 )
    {
    pComp = In_Trub->Get_Comp();
    pState = &Heat.In_0;
    pFlow = In_Trub;
    }
  else
    {
    pComp = In_Vol->Get_Comp();
    pState = &Heat.In_1;
    pFlow = In_Vol;
    }
  double E = pState->E;
  double P = pFlow->P;
  double T = pState->T;
  double F = pFlow->Flow_mol;
  double Ro = 0;
  double V = 0;
  if ( E >= 1. )
    {
// Газ
    double Mu = Mw( pComp );
    double Ksi = RP * ( T + TK ) / P;
    Ro = Mu / Ksi;
    V = F * Ksi;
    }                
  else
    {
    if ( E <= 0. )
      {
      double Mu = Mw( pComp );
      double To_m3 = To_m3_liq ( T, pComp );
      if ( To_m3 < 0.001 )
        To_m3 = 0.001;
      Ro = Mu / To_m3;
      V = F * To_m3;
      }
    else
      {
      bool IsComp[K_GAS];
      double X[K_GAS];
      double Y[K_GAS];
      pState-> SetCompData ( IsComp, X, Y, NULL, NULL );
      double Mu_gas = Mw( IsComp, Y );
      double Mu_liq = Mw( IsComp, X );
      double To_m3_l = To_m3_liq ( T, IsComp, X );
      double To_m3_g = RP * ( T + TK ) / P;
      V = F * ( E * To_m3_g + ( 1. - E ) * To_m3_l );
      double Ro_g = Mu_gas / To_m3_g;
      double Ro_l = Mu_liq / To_m3_l;
      Ro = ( E * Mu_gas + ( 1. - E ) * Mu_liq ) / ( E * To_m3_g + ( 1. - E ) * To_m3_l );
      }
    }
  Текущая_плотность = Ro;
  Текущая_скорость = fabs ( V );
  if ( Учитывать && Текущая_скорость > Базовая_скорость * 0.01 )
    {
    double kRo = pow ( Текущая_плотность / Базовая_плотность, Степень_плотности );
    double kV = pow ( Текущая_скорость / Базовая_скорость, Степень_скорости );
    K_Eff = kRo * kV;
    }
  else
    {
//    Базовая_плотность = Ro;
//    Базовая_скорость = V;
    K_Eff = 1.;
    }
  FINITE(K_Eff)
  return K_Eff;
  }
