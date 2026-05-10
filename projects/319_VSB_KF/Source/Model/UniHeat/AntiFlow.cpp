#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"



CAntiFlow::CAntiFlow( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
In_0( "In_0", this ),
In_0_Air( "In_0_Air", this ),
In_1( "In_1", this ),
In_1_Air( "In_1_Air", this ),
Out_0( "Out_0", this ),
Out_1( "Out_1", this )
	{
  Model = "AntiFlow";
	IterH_0 = true;
	IterH_1 = true;
	kGas_0 = 1.;
	kGas_1 = 1.;
	Cp_Corp_0 = 100.;
	Cp_Corp_1 = 100.;
  kCp_0 = 1.;
  kCp_1 = 1.;
  KS_0_Air = 0.;
  KS_1_Air = 0.;
	}

CAntiFlow_W::CAntiFlow_W()
	{
	memset ( this, 0, sizeof ( CAntiFlow_W ));
	k_Cp_0 = 0.01;
	k_Cp_1 = 0.01; 
	oTau_0 = 0.001; 
	oTau_1 = 0.001;
	Cp_0 = 10.;
	Cp_1 = 10.;
	T_in_0 = T_in_1 =
	T_out_0 = T_out_1 = 15.;
  T_in_0_Air = T_in_1_Air = 15.;
	}


void CAntiFlow::Calc( double dt )
	{
  if ( pOut_0 == NULL )
    return;
  if ( pOut_1 == NULL )
    return;
	double Ps0 = pOut_0->P;
	*pOut_0 = *pIn_0;
	pOut_0->Flow_mol = pIn_0->Flow_mol;
	pOut_0->P = Ps0;
	double Ps1 = pOut_1->P;
	*pOut_1 = *pIn_1;
	pOut_1->Flow_mol = pIn_1->Flow_mol;
	pOut_1->P = Ps1;
//
	CComp * pComp_0 = pIn_0->Get_Comp();
	CComp * pComp_1 = pIn_1->Get_Comp();
	if ( pComp_0 == NULL || pComp_1 == NULL )
		return ;
//
	h_in_0 = pIn_0->h_Flow;
  if ( KS_0_Air > 0. )
    {
    In_0_Air.PH( pIn_0->P, h_in_0, pComp_0 );
    T_in_0_Air = In_0_Air.T;
    double Fcp_0 = fabs ( pIn_0->Flow_mol ) * Cp_0;
    double K_dt = KS_0_Air * dt;
    T_in_0 = ( Fcp_0 * T_in_0_Air + K_dt * T_air ) / ( Fcp_0 + K_dt );
    In_0.PT( pIn_0->P, T_in_0, pComp_0 );
    h_in_0 = In_0.h;
    }
  else
    {
    In_0.PH ( pIn_0->P, h_in_0, pComp_0 );
	  pIn_0->T = T_in_0 = In_0.T;
    }
  double ks_0 = kGas_0 * In_0.E + 1. - In_0.E;
//
	h_in_1 = pIn_1->h_Flow;
  if ( KS_1_Air > 0. )
    {
    In_1_Air.PH( pIn_1->P, h_in_1, pComp_1 );
    T_in_1_Air = In_1_Air.T;
    double Fcp_1 = fabs ( pIn_1->Flow_mol ) * Cp_1;
    double K_dt = KS_1_Air * dt;
    T_in_1 = ( Fcp_1 * T_in_1_Air + K_dt * T_air ) / ( Fcp_1 + K_dt );
    In_1.PT( pIn_1->P, T_in_1, pComp_1 );
    h_in_1 = In_1.h;
    }
  else
    {
	  In_1.PH ( pIn_1->P, h_in_1, pComp_1 );
	  pIn_1->T = T_in_1 = In_1.T;
    }
	double ks_1 = kGas_1 * In_1.E + 1. - In_1.E;
//
// 
  double F_0_old = F_0;
  double F_1_old = F_1; 
  F_0 = fabs ( pIn_0->Flow_mol );
  F_1 = fabs ( pIn_1->Flow_mol );
  double Fcp_0 = F_0 * Cp_0;
  double Fcp_1 = F_1 * Cp_1;
  Lam_0 = 10000.;
	ASS(KS >0.)
	double ks = 2. * KS / ( 1. / ks_0 + 1. / ks_1 );
  if ( Lam_0 * Fcp_0 > ks )
    Lam_0 = ks / Fcp_0;
	else
		ks = Fcp_0 * Lam_0;
  Lam_1 = 10000.;
  if ( Lam_1 * Fcp_1 > ks )
    Lam_1 = ks / Fcp_1;
///////////////////////////////////////////////
//	double a = 1.;
	double T_out_0_old = T_out_0;
	double T_out_1_old = T_out_1;
  double T_in_0_old = T_in_0;
  double T_in_1_old = T_in_1;
//
	double x = Lam_0 - Lam_1;
	double f_0, f_1;
	if ( fabs ( x ) < 0.001 * Lam_0 )
		{
// D = const
		double Lam = 0.5 * ( Lam_0 + Lam_1 );
		double C = ( T_in_1 - T_in_0 ) / ( 1. + Lam );
		T_out_1 = T_in_1 - Lam * C;
		T_out_0 = T_in_0 + Lam * C;
		}
	else
		{
		if ( x >= 0. )
			{
			f_0 = 1.;
			f_1 = exp ( -x );
			}
		else
			{
			f_0 = exp ( x );
			f_1 = 1.;
			}
	// Ур-е для к-тов
		double a00 = 1.;
		double a01 = f_0;
		double a10 = 1.;
		double a11 = Lam_1 / Lam_0 * f_1;
		double b0 = T_in_0;
		double b1 = T_in_1;
	//
		double det = 1. / ( a00 * a11 - a10 * a01 );
		double a = ( b0 * a11 - b1 * a01 ) * det;
		double b = ( b1 * a00 - b0 * a10 ) * det;
	//
		T_out_0 = a + b * f_1; 
		T_out_1 = a + b * Lam_1 * f_0 / Lam_0; 
		T_in_0 = a + b * f_0; 
		T_in_1 = a + b * Lam_1 * f_1 / Lam_0; 
		}
//
	Ksi_0 = 1000.;
	if ( Fcp_0 * dt * Ksi_0 > Cp_Corp_0 )
		Ksi_0 = Cp_Corp_0 / ( Fcp_0 * dt );
	T_out_0 = ( T_out_0 + Ksi_0 * T_out_0_old ) / ( 1. + Ksi_0 );
	dT_Corp_0 = Ksi_0 * ( T_out_0 - T_out_0_old );
//
	Ksi_1 = 1000.;
	if ( Fcp_1 * dt * Ksi_1 > Cp_Corp_1 )
		Ksi_1 = Cp_Corp_1 / ( Fcp_1 * dt );
	T_out_1 = ( T_out_1 + Ksi_1 * T_out_1_old ) / ( 1. + Ksi_1 );
	dT_Corp_1 = Ksi_1 * ( T_out_1 - T_out_1_old );
//
	pOut_0->T = T_out_0;
	pOut_1->T = T_out_1;
	Q_dg = fabs ( Fcp_0 * ( T_out_0 - T_in_0 ) ) * 1e-6;
	Q_cal = Q_dg * M_cal;
// Уточнение Cp
//
	if ( IterH_0 )
		{
		double Q0 = kCp_0 * Cp_0 * ( T_out_0 - T_in_0 );
    double Q1 = kCp_1 * Cp_1 * ( T_out_1 - T_in_1 );
    double dH = F_0 * Q0 + F_1 * Q1;
//
		h_out_0 = h_in_0 + Q0;
		Out_0.T_max = T_out_0 + 5.;
		Out_0.T_min = T_out_0 - 5.;
		Out_0.PH( pOut_0 -> P, h_out_0, pComp_0 );
		pOut_0->h_Flow = h_out_0;
		double T_out_0_h = Out_0.T;
	////
		if ( fabs ( Q0 ) > 100. )
			{
			double dT = T_out_0_h - T_out_0;
			if ( Q0	< 0. )
				dT = -dT;
			double kCps = LimdX ( -k_Cp_0 * dT, 0.1 );
			Cp_0 *= 1. + kCps;
			if ( Cp_0 < 10. )
				Cp_0 = 10.;
			if ( Cp_0 > 10000000. )
				Cp_0 = 10000000.;
			}
		if ( !_finite ( Cp_0 ) )
			Cp_0 = 10.;
		}
	else
		{
		Out_0.PT( pOut_0->P, T_out_0, pComp_0 );
		h_out_0 = Out_0.h;
		pOut_0->h_Flow = h_out_0;
		double Q1 = Fcp_0 * fabs( T_out_0 - T_in_0 );
		if ( Q1 > 100. )
			{
			double dH = F_0 * fabs ( h_out_0 - h_in_0 );
			double Ks = dH / Q1 - 1.;
			double kCps = LimdX ( k_Cp_0 * Ks, 0.1 );
			Cp_0 *= 1. + kCps;
			if ( Cp_0 < 10. )
				Cp_0 = 10.;
			if ( Cp_0 > 10000000. )
				Cp_0 = 10000000.;
			}
		if ( !_finite ( Cp_0 ) )
			Cp_0 = 10.;
		}
////
//
	if ( IterH_1 )
		{
		double Q1 = kCp_1 * Cp_1 * ( T_out_1 - T_in_1 );
		h_out_1 = h_in_1 + Q1;
		Out_1.T_max = T_out_1 + 5.;
		Out_1.T_min = T_out_1 - 5.;
		Out_1.PH( pOut_1 -> P, h_out_1, pComp_1 );
		pOut_1->h_Flow = h_out_1;
		double T_out_1_h = Out_1.T;
	////
		if ( fabs ( Q1 ) > 100. )
			{
			double dT = T_out_1_h - T_out_1;
			if ( Q1	< 0. )
				dT = -dT;
			double kCps = LimdX ( -k_Cp_1 * dT, 0.1 );
			Cp_1 *= 1. + kCps;
			if ( Cp_1 < 10. )
				Cp_1 = 10.;
			if ( Cp_1 > 100000. )
				Cp_1 = 100000.;
			}
		if ( !_finite ( Cp_1 ) )
			Cp_1 = 10.;
		}
	else
		{
		Out_1.PT( pOut_1->P, T_out_1, pComp_1 );
		h_out_1 = Out_1.h;
		pOut_1->h_Flow = h_out_1;
		double Q1 = Fcp_1 * fabs( T_out_1 - T_in_1 );
		if ( Q1 > 100. )
			{
			double dH = F_1 * fabs ( h_out_1 - h_in_1 );
			double Ks = dH / Q1 - 1.;
			double kCps = LimdX ( k_Cp_1 * Ks, 0.1 );
			Cp_1 *= 1. + kCps;
			if ( Cp_1 < 10. )
				Cp_1 = 10.;
			if ( Cp_1 > 100000. )
				Cp_1 = 100000.;
			}
		if ( !_finite ( Cp_1 ) )
			Cp_1 = 10.;
		}
////
	}

int CAntiFlow::SaveState( )
	{
	S_CLASS("CAntiFlow_W",CAntiFlow_W)
	return 0;
	}

int CAntiFlow::RestoreState( char * StrName )
	{
  COPY_FIRST
	R_CLASS("CAntiFlow_W",CAntiFlow_W)
	return 1;
	}

int CAntiFlow::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		T_in_0 = T_in_1 = T_air;
		T_out_0 = T_out_1 = T_air;
    return 1;
    }
	return 0;
	}

int CAntiFlow::GetParams( char * )
	{
	#include "IO_Parms.h"
	PARM( k_Cp_0, "Регулировка Cp 0" )
	PARM( k_Cp_1, "Регулировка Cp 1" )
	PARM( IterH_0, "Расчет 0 по энтальпии" )
	PARM( IterH_1, "Расчет 1 по энтальпии" )
	PARM( kGas_0, "Теплоотдача 0 от газа" )
	PARM( kGas_1, "Теплоотдача 1 от газа" )
	PARM( Cp_Corp_0, "Теплоемкость 0" )
	PARM( Cp_Corp_1, "Теплоемкость 1" )
  PARM( kCp_0, "Поправка к Cp 0" )
  PARM( kCp_1, "Поправка к Cp 1" )
//
	PARM( T_in_0, "#Т на входе 0" )
	PARM( T_out_0, "#Т на выходе 0" ) 
	PARM( T_in_1, "#Т на входе 1" ) 
	PARM( T_out_1, "#Т на выходе 1" )
	PARM( Cp_0, "#Cp 0" ) 
	PARM( Cp_1, "#Cp 1" )
  PARM( Lam_0, "#Lam 0" )
  PARM( Lam_1, "#Lam 1" )
  PARM( In_0.E, "#Eps на входе 0" )
  PARM( Out_0.E, "#Eps на выходе 0" )
  PARM( In_1.E, "#Eps на входе 1" )
  PARM( Out_1.E, "#Eps на выходе 1" )
  if ( pIn_0 )
	  PARM( pIn_0->Flow_mol, "#Поток 0 моль" )
  if ( pIn_1 )
	  PARM( pIn_1->Flow_mol, "#Поток 1 моль" )
	PARM( Ksi_0, "#Влияние теплоемкости 0" )
	PARM( Ksi_1, "#Влияние теплоемкости 1" )
	PARM( dT_Corp_0, "#dT от теплоемкости 0" )
	PARM( dT_Corp_1, "#dT от теплоемкости 1" )
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_dg, "#Теплопередача ГДж/ч" )
	return 0;
	}
