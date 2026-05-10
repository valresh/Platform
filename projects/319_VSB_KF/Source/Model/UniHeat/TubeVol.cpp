#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "SetData.h"


CTubeVolHeat::CTubeVolHeat( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
S_Trub_In("Trub_In",this),
S_Vol_In_Air("Vol_In_Air",this),
S_Vol_In("Vol_In",this),
S_Trub_Out("Trub_Out",this),
S_Vol_Out("Vol_Out",this)
	{
  Model = "TubeVolHeat";
	IterH_Trub = true;
	IterH_Vol = true;
	kGas_Trub = 1.;
	kGas_Vol = 1.;
	kCp_Trub = 1.;
	kCp_Vol = 1.;
	Cp_Corp_Trub = 1.;
	Cp_Corp_Vol = 1.;
	pComp_Trub = NULL;
	pComp_Vol = NULL;
  Static = false;
  KS_Vol_Air = 0.;
	}

CTubeVolHeat_W::CTubeVolHeat_W()
	{
	memset ( this, 0, sizeof ( CTubeVolHeat_W ));
	k_Cp_Trub = 0.01;
	k_Cp_Vol = 0.01; 
	oTau_Trub = 0.001; 
	oTau_Vol = 0.001;
	T_in_Trub = T_in_Vol = T_in_Vol_Air =  
	T_out_Trub = T_out_Vol = 15.;
	}


void CTubeVolHeat::Calc( double dt )
	{
//	*pOut_Trub = *pIn_Trub;
//	*pOut_Vol = *pIn_Vol;
  if ( pOut_Trub )
		{
	  pOut_Trub ->To_m3 = pIn_Trub->To_m3;
		pOut_Trub ->To_kg = pIn_Trub->To_kg;
		pOut_Trub ->ID_COMP = pIn_Trub->ID_COMP;
		pOut_Trub ->Flow_mol = pIn_Trub->Flow_mol;
		}
//
  if ( pOut_Vol )
		{
		pOut_Vol ->To_m3 = pIn_Vol->To_m3;
		pOut_Vol ->To_kg = pIn_Vol->To_kg;
		pOut_Vol ->ID_COMP = pIn_Vol->ID_COMP;
		pOut_Vol ->Flow_mol = pIn_Vol->Flow_mol;
		}
  
  if ( pOut_Trub == NULL || pOut_Vol == NULL )
    {
    if ( pOut_Trub )
      *pOut_Trub = *pIn_Trub;
    if ( pOut_Vol )
      *pOut_Vol = *pIn_Vol;
		return;
    }
//
	pComp_Trub = pIn_Trub->Get_Comp();
	pOut_Trub ->pComp = pIn_Trub->pComp = pComp_Trub;
	pComp_Vol = pIn_Vol->Get_Comp();
	pOut_Vol ->pComp = pIn_Vol->pComp = pComp_Vol;
	if ( pComp_Trub == NULL || pComp_Vol == NULL )
		return ;
//
	S_Trub_In.PH( pIn_Trub->P, pIn_Trub->h_Flow, pComp_Trub );
	T_in_Trub = S_Trub_In.T;
	double ks_Trub = kGas_Trub * S_Trub_In.E + 1. - S_Trub_In.E;
//
  double h_in_Trub = pIn_Trub->h_Flow;
  double h_in_Vol = pIn_Vol->h_Flow;
  if ( KS_Vol_Air > 0. )
    {
    S_Vol_In_Air.PH( pIn_Vol->P, pIn_Vol->h_Flow, pComp_Vol );
    T_in_Vol_Air = S_Vol_In_Air.T;
    double Fcp_Vol = fabs ( pIn_Vol->Flow_mol ) * Cp_Vol;
    double K_dt = KS_Vol_Air * dt;
    T_in_Vol = ( Fcp_Vol * T_in_Vol_Air + K_dt * T_air ) / ( Fcp_Vol + K_dt );
    S_Vol_In.PT( pIn_Vol->P, T_in_Vol, pComp_Vol );
    h_in_Vol = S_Vol_In.h;
    }
  else
    {
	  S_Vol_In.PH( pIn_Vol->P, pIn_Vol->h_Flow, pComp_Vol );
	  T_in_Vol = S_Vol_In.T;
    }
///
	double ks_Vol = kGas_Vol * S_Vol_In.E + 1. - S_Vol_In.E;
//
	if ( Cp_Trub == 0. )
		Cp_Trub = 10.;
	if ( Cp_Vol == 0. )
		Cp_Vol = 10.;
//
	double ks = 2. * KS / ( 1. / ks_Trub + 1. / ks_Vol );
//
  double Fcp_Vol = fabs ( pIn_Vol->Flow_mol ) * Cp_Vol;
  Lam_Vol = 10000.;
  double Eps_Vol;
  if ( Lam_Vol * Fcp_Vol > ks )
		{
    Lam_Vol = ks / Fcp_Vol;
		}
	Eps_Vol = Exp1 ( Lam_Vol );
// 
  double Fcp_Trub = fabs ( pIn_Trub->Flow_mol ) * Cp_Trub;
  Lam_Trub = 10000.;
  if ( Lam_Trub * Fcp_Trub > ks * Eps_Vol )
    Lam_Trub = ks * Eps_Vol / Fcp_Trub;
  double Eps_Trub = Exp1 ( Lam_Trub );
//
  double D0 = T_in_Trub - T_in_Vol;
  double D_Trub = D0 * exp ( -Lam_Trub );
//  double D_Vol = D0 * exp ( -Lam_Vol );
	double T_out_new_trub = T_in_Vol + D_Trub;
  double T_out_new_vol = T_in_Vol + D0 * Eps_Trub * ( 1. - exp ( -Lam_Vol ) );
  double T_out_old_vol = T_out_Vol;
	double T_out_old_trub = T_out_Trub;
//
  Q = KS * Eps_Trub * D0 * Eps_Vol;
	Q_dg = fabs ( Q ) * 1e-6;
	Q_cal = Q_dg * M_cal;
//  double test_Trub = Fcp_Trub * ( T_out_new_trub - T_in_Trub ) / Q;
  if ( Static )
    {
    T_out_Trub = T_out_new_trub;
    dT_Corp_Trub = 0.;
    T_out_Vol = T_out_new_vol;
    dT_Corp_Vol = 0.;
    }
  else
    {
	  Ksi_Trub = 1000.;
    double Cp_trub = Cp_Corp_Trub;
    if ( Cp_trub < 0. )
      {             
      Ksi_Trub = 1000.;
      Cp_trub = -Cp_trub;
      }
	  if ( Fcp_Trub * dt * Ksi_Trub > Cp_trub )
		  Ksi_Trub = Cp_trub / ( Fcp_Trub * dt );
	  T_out_Trub = ( T_out_new_trub + Ksi_Trub * T_out_old_trub ) / ( 1. + Ksi_Trub );
	  dT_Corp_Trub = Ksi_Trub * ( T_out_Trub - T_out_old_trub );
  //
	  Ksi_Vol = 10000.;
	  if ( Fcp_Vol * dt * Ksi_Vol > Cp_Corp_Vol )
		  Ksi_Vol = Cp_Corp_Vol / ( Fcp_Vol * dt );
	  T_out_Vol = ( T_out_new_vol + Ksi_Vol * T_out_old_vol ) / ( 1. + Ksi_Vol );
	  dT_Corp_Vol = Ksi_Vol * ( T_out_Vol - T_out_old_vol );
    }
//
  pOut_Trub->T = T_out_Trub;
  if ( T_out_Vol > 1100. )
    T_out_Vol = 1100.;
  pOut_Vol->T = T_out_Vol;
//
// Уточнение Cp
//
	if ( IterH_Trub )
		{
		double Qs = kCp_Trub * Cp_Trub * ( T_out_Trub - T_in_Trub );
		double h_out_Trub = h_in_Trub + Qs;
		S_Trub_Out.T_max = T_out_Trub + 5.;
		S_Trub_Out.T_min = T_out_Trub - 5.;
		if ( S_Trub_Out.T_min < -80. )
			S_Trub_Out.T_min = -80.;
		S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
		pOut_Trub->h_Flow = S_Trub_Out.h;
		double dT = T_out_Trub - S_Trub_Out.T;
		if ( Qs	< 0. )
			dT = -dT;
		if ( fabs ( Qs ) > 100. )
			{
			double kCps = LimdX ( k_Cp_Trub * dT, 0.1 );
			Cp_Trub *= 1. + kCps;
			if ( Cp_Trub < 10. )
				Cp_Trub = 10.;
			if ( Cp_Trub > 100000. )
				Cp_Trub = 100000.;
			}
		if ( !_finite ( Cp_Trub ) )
			Cp_Trub = 10.;
		}
	else
		{
		S_Trub_Out.PT( pOut_Trub->P, T_out_Trub, pComp_Trub );
		double h_out_Trub = S_Trub_Out.h;
	//
		pOut_Trub->h_Flow = h_out_Trub;
		double Qs = kCp_Trub * Fcp_Trub * fabs( T_in_Trub - T_out_Trub );
		if ( Qs > 100. )
			{
			double Ds = Qs / Q;
			double Hs = fabs ( pIn_Trub->Flow_mol * ( h_in_Trub - h_out_Trub ));
			double Ks = Hs / Qs - 1.;
			double kCps = LimdX ( k_Cp_Trub * Ks, 0.1 );
			Cp_Trub *= 1. + kCps;
			if ( Cp_Trub < 10. )
				Cp_Trub = 10.;
			if ( Cp_Trub > 100000. )
				Cp_Trub = 100000.;
			}
		if ( !_finite ( Cp_Trub ) )
			Cp_Trub = 10.;
		}
	if ( IterH_Vol )
		{
		double Qo = kCp_Vol * Cp_Vol * ( T_out_Vol - T_in_Vol );
		double h_out_Vol = h_in_Vol + Qo;
		S_Vol_Out.T_max = T_out_Vol + 5.;
		S_Vol_Out.T_min = T_out_Vol - 5.;
		if ( S_Vol_Out.T_min < -80. )
			S_Vol_Out.T_min = -80.;
		S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
		pOut_Vol->h_Flow = S_Vol_Out.h;
		double dT = T_out_Vol - S_Vol_Out.T;
		if ( Qo	< 0. )
			dT = -dT;
	//
		if ( fabs ( Qo ) > 100. )
			{
			double kCps = LimdX ( k_Cp_Vol * dT, 0.1 );
			Cp_Vol *= 1. + kCps;
			if ( Cp_Vol < 10. )
				Cp_Vol = 10.;
			if ( Cp_Vol > 10000000. )
				Cp_Vol = 10000000.;
			}
		if ( !_finite ( Cp_Vol ) )
			Cp_Vol = 10.;
		}
	else
		{
		S_Vol_Out.PT( pOut_Vol->P, T_out_Vol, pComp_Vol );
		double h_out_Vol = S_Vol_Out.h;
		pOut_Vol->h_Flow = h_out_Vol;
		double Qo = kCp_Vol * Fcp_Vol * fabs( T_out_Vol - T_in_Vol );
		if ( Qo > 100. )
			{
			double Do = Qo / Q;
			double Ho = fabs ( pIn_Vol->Flow_mol * ( h_out_Vol - h_in_Vol ));
			double Ko = Ho / Qo - 1.;
			double kCpo = LimdX ( k_Cp_Vol * Ko, 0.1 );
			Cp_Vol *= 1. + kCpo;
			if ( Cp_Vol < 10. )
				Cp_Vol = 10.;
			if ( Cp_Vol > 100000. )
				Cp_Vol = 100000.;
			}
		if ( !_finite ( Cp_Vol ) )
			Cp_Vol = 10.;
		}
	}

int CTubeVolHeat::SaveState( )
	{
	S_CLASS("TubeVolHeat_W",CTubeVolHeat_W)
	return 0;
	}

int CTubeVolHeat::RestoreState( char * StrName )
	{
  COPY_FIRST
	R_CLASS("TubeVolHeat_W",CTubeVolHeat_W)
	return 1;
	}

int CTubeVolHeat::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		T_in_Trub = T_in_Vol = T_air;
		T_out_Trub = T_out_Vol = T_air;
    return 1;
    }
	return 0;
	}

int CTubeVolHeat::GetParams( char * )
	{
	#include "IO_Parms.h"
	PARM( k_Cp_Trub, "Регулировка Cp труб" )
	PARM( k_Cp_Vol, "Регулировка Cp объема" )
	PARM( IterH_Trub, "Расчет труб по энтальпии" )
	PARM( IterH_Vol, "Расчет объема по энтальпии" )
	PARM( kGas_Trub, "Теплоотдача труб от газа" )
	PARM( kGas_Vol, "Теплоотдача объема от газа" )
	PARM( Cp_Corp_Trub, "Теплоемкость корпуса труб" )
	PARM( Cp_Corp_Vol, "Теплоемкость корпуса объема" )
	PARM( kCp_Trub, "Поправка к Cp труб" )
	PARM( kCp_Vol, "Поправка к Cp объема" )
//
	PARM( T_in_Trub, "#Т на входе труб" )
	PARM( T_out_Trub, "#Т на выходе труб" ) 
	PARM( T_in_Vol, "#Т на входе объема" ) 
	PARM( T_out_Vol, "#Т на выходе объема" )
	PARM( Cp_Trub, "#Cp труб" ) 
	PARM( Cp_Vol, "#Cp объема" )
  PARM( Lam_Trub, "#Lam труб" )
  PARM( Lam_Vol, "#Lam объема" )
  PARM( S_Trub_In.E, "#Eps на входе труб" )
  PARM( S_Trub_Out.E, "#Eps на выходе труб" )
  PARM( S_Vol_In.E, "#Eps на входе объема" )
  PARM( S_Vol_Out.E, "#Eps на выходе объема" )
	PARM( pIn_Trub->Flow_mol, "#Поток труб моль" )
	PARM( pIn_Vol->Flow_mol, "#Поток объема моль" )
	PARM( Ksi_Trub, "#Влияние теплоемкости труб" )
	PARM( Ksi_Vol, "#Влияние теплоемкости объема" )
	PARM( dT_Corp_Trub, "#dT от теплоемкости труб" )
	PARM( dT_Corp_Vol, "#dT от теплоемкости объема" )
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_dg, "#Теплопередача ГДж/ч" )
	return 0;
	}

