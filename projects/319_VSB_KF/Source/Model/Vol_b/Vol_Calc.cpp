#include "stdafx.h"
#include "Vol.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"

extern int NumbCompNames[K_GAS];
extern int kNumbCompNames;

void CVol::CalсHeat ( double dt )
  {
  if ( pFlow_heat_out && pFlow_heat_in )
    {
    Heat.pIn = pFlow_heat_in;
    Heat.pOut = pFlow_heat_out;
    Heat.T_ext = VolBase.T;
    Heat.Lev = VolBase.Lev_liq;
    Heat.KS = k_Heat * S_Heat * 3.6;
    Heat.Calc( dt );
    Q_Heat_1 = Heat.Q_Heat / 3.6e6;
    VolBase.Q_ext += Heat.Q_Heat;
    }
  if ( pFlow_heat_out_2 && pFlow_heat_in_2 )
    {
    //    *pFlow_heat_out = *pFlow_heat_in;
    Heat_2.pIn = pFlow_heat_in_2;
    Heat_2.pOut = pFlow_heat_out_2;
    Heat_2.T_ext = VolBase.T;
    Heat_2.Lev = VolBase.Lev_liq;
    Heat_2.KS = k_Heat_2 * S_Heat_2 * 3.6;
    Heat_2.Calc( dt );
    Q_Heat_2 = Heat_2.Q_Heat / 3.6e6;
    VolBase.Q_ext += Heat_2.Q_Heat;
    }
  if ( pFlow_heat_out_3 && pFlow_heat_in_3 )
    {
    //    *pFlow_heat_out = *pFlow_heat_in;
    Heat_3.pIn = pFlow_heat_in_3;
    Heat_3.pOut = pFlow_heat_out_3;
    Heat_3.T_ext = VolBase.T;
    Heat_3.Lev = VolBase.Lev_liq;
    Heat_3.KS = k_Heat_3 * S_Heat_3 * 3.6;
    Heat_3.Calc( dt );
    Q_Heat_3 = Heat_3.Q_Heat / 3.6e6;
    VolBase.Q_ext += Heat_3.Q_Heat;
    }
  }

void CVol::CallElectroHeat ( double dt )
  {
  if ( Пуск_стоп_нагрев.Use() )
    {
    UseHeater = true;
    if ( Пуск_стоп_нагрев.On() )
      WorkHeat = true;
    else
      WorkHeat = false;
    }
  if ( Пуск_нагрев.Use() )
    {
    UseHeater = true;
    if ( Пуск_нагрев.On() )
      WorkHeat = true;
    }
  if ( Стоп_нагрев.Use() )
    {
    UseHeater = true;
    if ( Стоп_нагрев.On() )
      WorkHeat = false;
    }
  if ( UseHeater )
    {
    if ( WorkHeat )
      {
      Нагрев_работает = 1;
      VolBase.Q_ext += Q_Heat * 36000.;
      }
    else
      {
      Нагрев_работает = 0;
      }
    }
  else
    VolBase.Q_ext += Q_add * 1e3;
  if ( pExtHeat )
    VolBase.Q_ext += *pExtHeat;
  }

void CVol::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( BreakPoint )
    VolBase.Trace = true;
  double F_kg = 0.;
  double Flow_max_abs = 0.;
// Вход с максимальным входным потоком
  double Flow_max_in = 0.;
  int n_max_in = -1;
  for ( int n = 0; n < kIO; n++ )
    {
    if ( VolBase.pFlow[n] )
      {
      double F = VolBase.Flow_kg[n];
      if ( !_finite(F))
        F = 0.;
      if ( fabs( F ) > Flow_max_in )
        {
        n_max_in = n;
        Flow_max_in = fabs( F );
        }
      double Fa = fabs ( F );
      if ( Fa > Flow_max_abs )
        Flow_max_abs = Fa;
      F_kg += F;
      }
    }
  _F_summ_kg = F_kg;
  double Flow_summ_no_out = 0.;
//  double Flow_summ_all = 0.;
  double Flow_summ_mol = 0.;
  for ( int n = 0; n < kIO; n++ )
    {
    double F = VolBase.Flow_kg[n];
    if ( !_finite(F))
      F = 0.;
    if ( VolBase.pFlow[n] == NULL )
      continue;
    Flow_summ_mol += VolBase.pFlow[n]->Flow_mol;
    if ( n != nFlow_summ )
      Flow_summ_no_out += F;
    }
  //for ( int n = 0; n < kIO; n++ )
  //  {
  //  if ( VolBase.pFlow[n] )
  //    {
  //    if ( n != nFlow_summ )
  //      Flow_summ_no_out += VolBase.pFlow[n]->Flow_mol;
  //    double F = VolBase.Flow_kg[n];
  //    Flow_summ_all += F;
  //    }
  //  }
  Flow_summ_total = Flow_summ_mol;
  if ( nFlow_summ >= 0 && nFlow_summ < kIO )
    {
    CFlow * pF = VolBase.pFlow[nFlow_summ];
    if ( pF && pF->ID_COMP > 0 )
      {
      double Mw = ::Mw ( pF->Get_Comp());
      if ( Mw > 2. )
        Flow_summ_out +=  k_Flow * 0.2 * ( Flow_summ_no_out / Mw - Flow_summ_out );
      }
    else
      KKK();
    }
  else
    Flow_summ_out = 0.;
  FINITE(Flow_summ_out)
// 
  if ( OverFlow && !Use_Flow_Balance )
    {   
    double Summ_kg = 0.;
    double Max_Flow = 0.;
    for ( int n = 0; n < kIO; n++ )
      {
      if ( VolBase.pFlow[n] )
        {
        CComp * pComp = VolBase.pFlow[n]->Get_Comp();
        Mw_Flow[n] = ::Mw ( pComp );
        double Fkg = VolBase.pFlow[n]->Flow_mol * Mw_Flow[n];
        Flow_No_This[n] = -Fkg;
        Summ_kg += Fkg;
        double Fa = fabs( Fkg );
        if ( Fa > Max_Flow )
          Max_Flow = Fa;    
        }
      }
    if ( UseSumm )
      {
      P_Napor += LimdX( kP_Napor * Summ_kg * 1e-4, 0.1 );
      if ( P_Napor > 0.9 * P_max )
        P_Napor = 0.9 * P_max;
      if ( P_Napor < 0. )
        {
        OverFlow = false;
        P_Napor = 0.;
        }
      }
    for ( int n = 0; n < kIO; n++ )
      {
      Flow_No_This[n] += Summ_kg;
      }
    UseSumm = true;
    }
  else
    {
    if ( !OverFlow )
      P_Napor = 0.;
    UseSumm = false;
    }
  //if ( nFlow_summ >= 0 && _Lev_liq > 20. )
  //  Flow_summ_out = Flow_summ;
  //else
  //  Flow_summ_out = 0.;                 
  _F_summ_kg = F_kg;
  Flow_Ctrl = Flow_max_abs;
//
  VolBase.Q_ext = 0.;
  CalсHeat ( dt );
  CallElectroHeat ( dt );
//
//
  if ( Reg_T.IsOn() )
    VolBase.T_Z = Reg_T;
  else
    VolBase.T_Z = 0.;
  if ( Reg_P.IsOn() )
    VolBase.P_Z = Reg_P;
  else
    VolBase.P_Z = -2.;
  if ( Reg_L_1.IsOn() )
    {
    OverFlow = false;
    VolBase.LevZ = Reg_L_1;
    if ( Reg_L_1 > 0. )
      Reg_L_1.Off();
    }
  else
    VolBase.LevZ = 0.;
  if ( Reg_L_2.IsOn() )
    {
    OverFlow = false;
    VolBase.LevZ_2 = Reg_L_2;
    if ( Reg_L_2 > 0. )
      Reg_L_2.Off();
    }
  else
    VolBase.LevZ_2 = 0.;
//
  VolBase.T_min = T_min;
  VolBase.T_max = T_max;
  VolBase.OverFlow = OverFlow;
  VolBase.Объема_2 = Объема_2;
  VolBase.k_Cond = k_Cond;
 ////////////////////////////////////////////////////
  VolBase.Vol_Gas = Vol * Stable_P;
  VolBase.Calc_2F = Calc_2F;
  VolBase.Тип = Тип;
  VolBase.Static_Fix_P = Static_Fix_P;
  VolBase.Static_Iter_P = Static_Iter_P;
  VolBase.Static_P_fiz = Static_P_fiz;
  VolBase.Static_Out_gas = Static_Out_gas;
  VolBase.Static_Fix_L = Static_Fix_L;
  VolBase.Fast_T = Fast_T;
  VolBase.Static_Calc_Mix = Static_Calc_Mix;
  VolBase.n_Balance = NumbCompNames[nCompBalance];
  VolBase.Static_Out_Liq_1 = Static_Out_1F;
  VolBase.Static_Out_Liq_2 = Static_Out_2F;
  //if ( Разгерметизация )
    //VolBase.VolInit ( pAir, 1., T_air );
  //else
  VolBase.bp = bp;
  VolBase.VolStep( dt );

  if ( VolBase.P > P_max + 1. )
    {
    if ( pSys->IsBlk )
      Разгерметизация = true;
    }
  Vol = VolBase.Vol;
  CalcLev ( dt );
  if ( Reg_L_1.IsOn() && Reg_L_1 >= 0. )
    Reg_L_1.Off();
/////////////////////////////////////////////////
  _Pvap = VolBase.P - 1.;
  if ( OverFlow )
    _P = P_Napor + _Pvap;
  else
    _P = _Pvap;
  _T = VolBase.T;
  _Lev_liq = Lev_0_100( VolBase.Lev_liq, VolBase.Lev_0, VolBase.Lev_100 ) * 100.;
  _Lev_liq_1 = VolBase.Lev_liq_1 * 100.;
  _Lev_liq_2 = VolBase.Lev_liq_2 * 100.;
  _M_liq = VolBase.M_Liq;
  _M_liq_kg = VolBase.M_Liq * VolBase.Mw_liq;
  Вес = _M_liq_kg * k_Вес + Tara_Вес;
  _M_liq_dt = VolBase.dMdt_mol * VolBase.Mw_liq;
  if ( M_old == 0 )
    {
    M_old = VolBase.M_Gas * VolBase.Mw_gas + VolBase.M_Liq * VolBase.Mw_liq;
    }
  else
    {
    double M_new = VolBase.M_Gas * VolBase.Mw_gas + VolBase.M_Liq * VolBase.Mw_liq;;
    dMdt = ( M_new - M_old ) / dt;
    M_old = M_new;
    }

  if ( Reg_Reg_L.IsOn() && numbKlapReg < 0 )
    Reg_Reg_L.Off();
  if ( Reg_Reg_L.IsOn() && !Reg_L_1.IsOn() && KlapReg[0] && numbKlapReg >= 0 )
    {
    if ( pFlowOutZ == NULL )
      {
      IBaseModel * pKlap = IBaseModel::Find( KlapReg );
      if ( pKlap )
        pKlap->SetData ( sd_GetFlowZ, (void*)&pFlowOutZ );
      }
    double Lev_Opt = Reg_Reg_L;
    double Flow_in = 0.;
    for ( int n = 0; n < VolBase.k_Node; n++ )
      {
      if ( n == numbKlapReg )
        continue;
      if ( !VolBase.IsFlow[n] )
        continue;
      CFlow * pFlow = VolBase.pFlow[n];
      Flow_in += pFlow->Flow_mol;
      }
    double F_Reg = 1.;
    F_Reg = Flow_in * ( 1. + 0.5 * ( VolBase.Lev_liq - Lev_Opt *  0.01 ));
    if ( F_Reg < 1. )
      F_Reg = 1.;
    *pFlowOutZ = F_Reg;
    }
  if ( pFlowOutZ && !Reg_Reg_L.IsOn() && KlapReg[0]  )
    {
    IBaseModel * pKlap = IBaseModel::Find( KlapReg );
    if ( pKlap )
      pKlap->SetData ( sd_GetFlowZ, NULL );
    pFlowOutZ = NULL;
    }
  _Sliv = VolBase._Sliv_kg;
  Ps = VolBase.P - 1.;
    Ps += P_Napor;
  Давление = Ps;
  Температура = VolBase.T;
  Температура_стенки = VolBase.T + kT_wall * ( T_air - VolBase.T );
	}

void CVol::CalcLev ( double dt )
	{
	SET_BP
  //if ( strstr ( ObjName, "Е-101") )
  //  KKK();
  double Lev = VolBase.Lev_liq;
  if ( Lev > 0.999 && VolBase.Summ_Flow_kg >= 0.1 )
    OverFlow = true;
//  if ( Lev < 0.9 )
//    OverFlow = false;
  if ( Use_Lev_max_hydro )
    {
    if ( Lev > Lev_max_hydro )
      {
      double L = ( 1. - Lev ) / ( 1. - Lev_max_hydro );
      if ( L < 0. )
        L = 0.;
      double K = L * L;
      KsiTop = 0.9 * KsiTop + 0.1 * K;
      }
    }
  else
    KsiTop = 0.9 * KsiTop + 0.1;

 //	VolBase.Lev.Shft_Lev = Lev_base;
	double L = Lev_0_100( Lev, VolBase.Lev_0, VolBase.Lev_100 );
	//if ( Level_Msg )
	//	{
	//	MSG_OVER.SysMsg2 ( L >= 0.99, L < 0.9, "«%s» переполнен", (char*)ObjName );
	//	MSG_EMPTY.SysMsg2( L <= 0.01, L > 0.1, "«%s» опустошен", (char*)ObjName );
	//	}
  double L_100 = ( L - Shift_Lev ) * Масштаб;
  Уровень = L_100;
  double Lw = Lev_0_100( VolBase.Lev_liq_2, VolBase.Lev_0, VolBase.Lev_100 );
  Уровень_воды = Lw * Масштаб;
/////////////////////////////////////////////////
	double Lev_Rel = VolBase.Lev_liq;
	if ( Lev_Rel > 1. )
		Lev_Rel = 1.;
	double iRoWater = 0.;
	double iRoSteam = 0.;
	bool NoRo = true;
	int nCtrlLev = -1;
	for ( int n = 0; n < kIO; n++ )
		{
		double dH = Lev_Rel - VolBase.H_node[n]; //???
		double dP_Level;
		if ( dH > 0. && Use_Lev_hydro )
			dP_Level = VolBase.Ro_liq * dH * H_Vol * 1e-4;
		else
			dP_Level = 0.;
    if ( dPHydro[n] >= 0. && dH > 0. )
			dP_Level += dPHydro[n] * dH;
	  dP_Hydro[n] = dP_Level;
		}
//////////////////////////////////////////////////
	}

#if 0
const double Eps_Lev = 5.;
const double dLev_dt_max = 100.;

void CVol::CtrlLev ( double dt )
  {
  double Lev = VolBase.Lev_liq;
  if ( !OnCltrLev )
    {
    Lev_old = Lev;
    return;
    }
  if ( Lev_old < 0. )
    {
    Lev_old = Lev;
    УправлениеУровнем = Ctrl_Lev_KLap * 100.;
    return;
    }
  double dLdt = ( Lev - Lev_old ) / dt;
  Lev_old = Lev;
  double L_100 = Lev * 100.;
  double dLdt_Z;
  if ( fabs ( L_100 - CltrLevZ ) > Eps_Lev )
    {
    if ( L_100 > CltrLevZ )
      dLdt_Z = -dLev_dt_max;
    else
      dLdt_Z = dLev_dt_max;
    }
  else
    {
    dLdt_Z = ( CltrLevZ - L_100 ) * dLev_dt_max / Eps_Lev;
//    dLdt_Z = 0.;
    }
  double F = ( dLdt - dLdt_Z ) * VolBase.Vol;
  double dU = F / CltrFlow;
  Ctrl_Lev_KLap += dU;
  if ( Ctrl_Lev_KLap < 0. )
    Ctrl_Lev_KLap = 0.;
  if ( Ctrl_Lev_KLap > 1. )
    Ctrl_Lev_KLap = 1.;
  УправлениеУровнем = Ctrl_Lev_KLap * 100.;
  }

#endif

