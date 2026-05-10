#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"



CAntiFlowCool::CAntiFlowCool( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
In_0( "In_0", this ),
In_1( "In_1", this ),
Out_0( "Out_0", this ),
Out_1( "Out_1", this )
	{
  Model = "AntiFlowCool";
	IterH_0 = false;
	IterH_1 = false;
	kGas_0 = 1.;
	kGas_1 = 1.;
	Cp_Corp_0 = 100.;
	Cp_Corp_1 = 100.;
	KPD_0 = KPD_1 = 1.;
  T_min_0 = T_min_1 = 1.; 
  T_max_0 = T_max_1 = 1000.;
  }

CAntiFlowCool_W::CAntiFlowCool_W()
	{
	memset ( this, 0, sizeof ( CAntiFlowCool_W ));
	k_Cp_0 = 0.01;
	k_Cp_1 = 0.01; 
	oTau_0 = 0.001; 
	oTau_1 = 0.001;
	Cp_0 = 10.;
	Cp_1 = 10.;
	T_in_0 = T_in_1 = 
	T_out_0 = T_out_1 = 15.;
	}


void CAntiFlowCool::Calc( double dt )
	{
	double Ps0 = pOut_0->P;
	*pOut_0 = *pIn_0;
	pOut_0->P = Ps0;
	double Ps1 = pOut_1->P;
	*pOut_1 = *pIn_1;
	pOut_1->P = Ps1;
//
	CComp * pComp_0 = pIn_0->Get_Comp();
	CComp * pComp_1 = pIn_1->Get_Comp();
	if ( pComp_0 == NULL || pComp_1 == NULL )
		return ;
//
	h_in_0 = pIn_0->h_Flow;
	In_0.PH ( pIn_0->P, h_in_0, pComp_0 );
	T_in_0 = In_0.T;
	double ks_0 = kGas_0 * In_0.E + 1. - In_0.E;
//
	h_in_1 = pIn_1->h_Flow;
	In_1.PH ( pIn_1->P, h_in_1, pComp_1 );
	T_in_1 = In_1.T;
	double ks_1 = kGas_1 * In_1.E + 1. - In_1.E;
//
// 
  double F_0 = fabs ( pIn_0->Flow_mol );
  double F_1 = fabs ( pIn_1->Flow_mol );
  double Fcp_0 = F_0 * Cp_0;
  double Fcp_1 = F_1 * Cp_1;
  Lam_0 = 10000.;
	ASS(KS >0.)
	double ks = 2. * KS / ( 1. / ks_0 + 1. / ks_1 );
  if ( Lam_0 * Fcp_0 > ks )
    Lam_0 = ks / Fcp_0;
  Lam_1 = 100.;
  if ( Lam_1 * Fcp_1 > ks )
    Lam_1 = ks / Fcp_1;
//
	D_1 = ( ks * ( KCool_0 * TCool_0 + KCool_1 * TCool_1 ) + KCool_0 * KCool_1 * TCool_0 ) /
		    ( ks * ( KCool_0 + KCool_1 ) + KCool_0 * KCool_1 );
	D_0 = ( ks * D_1 + KCool_0 * TCool_0 ) / ( ks + KCool_0 );
	double D_1_T = ( ks * D_0 + KCool_1 * TCool_1 ) / ( ks + KCool_1 );
//
	E_0 = 100.;
  if ( E_0 * Fcp_0 > KCool_0 )
		E_0 = KCool_0 / Fcp_0;
	E_1 = 100.;
  if ( E_1 * Fcp_1 > KCool_1 )
		E_1 = KCool_1 / Fcp_1;
//
///////////////////////////////////////////////
	double M_0 = Lam_0 + E_0;
	double M_1 = Lam_1 + E_1;
// 
//	double a = 1.;
	double b = M_1 - M_0;
	double c = Lam_1 * Lam_0 - M_1 * M_0;
	double det = b * b - 4. * c;
	ASS ( det >= 0 );
	det = sqrt ( det );
// Корни
	double Z_1 = ( -b + det ) * 0.5;
	double Z_2 = ( -b - det ) * 0.5;
//
	double R10, R11, R20, R21;
	if ( Z_1 > 0. )
		{
		R10 = 1.;
		R11 = exp ( -Z_1 );
		}
	else
		{
		R10 = exp ( Z_1 );
		R11 = 1.;
		}
	if ( Z_2 > 0. )
		{
		R20 = 1.;
		R21 = exp ( -Z_2 );
		}
	else
		{
		R20 = exp ( Z_2 );
		R21 = 1.;
		}
//
	double Tin0 = T_in_0 + D_0;
	double Tin1 = T_in_1 + D_1;
	double RR = R11 * R20 - R10 * R21;
	double iRR = 1. / RR;
	double Tout1 = ( Tin1 * R10 * R20 * ( Z_2 - Z_1 ) + Lam_1 * Tin0 * RR ) /
		( RR * M_1 + R11 * R20 * Z_2 - R10 * R21 * Z_1 );
	double Tout0 = ( R21 * Lam_1 * Tin0 + R20 * ( Z_1 + M_1 ) * Tin1 - R21 * ( Z_1 + M_1 ) * Tout1 ) /
		( Lam_1 * R20 );
	double T_out_0_new = Tout0 - D_0;
	double T_out_1_new = Tout1 - D_1;
	if ( T_out_0_new < 0. )
		KKK();
	if ( T_out_1_new < 0. )
		KKK();
//
	double a0 = ( Tout0 * R20 - Tin0 * R21 ) * iRR;
	double b0 = ( Tin0 * R11 - Tout0 * R10 ) * iRR;
	double a1 = ( Tin1 * R20 - Tout1 * R21 ) * iRR;
	double b1 = ( Tout1 * R11 - Tin1 * R10 ) * iRR;
	double IntT0 = a0 * ( R10 - R11 ) / Z_1 + b0 * ( R20 - R21 ) / Z_2;
	double IntT1 = a1 * ( R10 - R11 ) / Z_1 + b1 * ( R20 - R21 ) / Z_2;
	Q_cool_0 = KCool_0 * ( TCool_0 - D_0 - IntT0 );
	Q_cool_1 = KCool_1 * ( TCool_1 - D_1 - IntT1 );
//
	Ksi_0 = 100.;
	if ( Fcp_0 * dt * Ksi_0 > Cp_Corp_0 )
		Ksi_0 = Cp_Corp_0 / ( Fcp_0 * dt );
	T_out_0 = ( T_out_0_new + Ksi_0 * T_out_0 ) / ( 1. + Ksi_0 );
	dT_Corp_0 = Ksi_0 * ( T_out_0 - T_out_0_new );
//
	Ksi_1 = 100.;
	if ( Fcp_1 * dt * Ksi_1 > Cp_Corp_1 )
		Ksi_1 = Cp_Corp_1 / ( Fcp_1 * dt );
	T_out_1 = ( T_out_1_new + Ksi_1 * T_out_1 ) / ( 1. + Ksi_1 );
	dT_Corp_1 = Ksi_1 * ( T_out_1 - T_out_1_new );
//
	pOut_0->T = T_out_0;
	pOut_1->T = T_out_1;
	Q_dg = fabs ( Fcp_0 * ( T_out_0 - T_in_0 ) ) * 1e-6;
	Q_cal = Q_dg * M_cal;
// Уточнение Cp
//
	if ( IterH_0 )
		{
		double Q0 = Cp_0 * ( T_out_0 - T_in_0 );
		h_out_0 = h_in_0 + Q0 * KPD_0;
		Out_0.T_max = T_max_0;
		Out_0.T_min = T_min_0;
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
			if ( Cp_0 > 100000. )
				Cp_0 = 100000.;
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
			if ( Cp_0 > 100000. )
				Cp_0 = 100000.;
			}
		if ( !_finite ( Cp_0 ) )
			Cp_0 = 10.;
		}
////
//
	if ( IterH_1 )
		{
		double Q1 = Cp_1 * ( T_out_1 - T_in_1 );
		h_out_1 = h_in_1 + Q1 * KPD_1;
		Out_1.T_max = T_max_1;
		Out_1.T_min = T_min_1;
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

int CAntiFlowCool::SaveState( )
	{
	S_CLASS("CAntiFlowCool_W",CAntiFlowCool_W)
	return 0;
	}

int CAntiFlowCool::RestoreState( char * StrName )
	{
	R_CLASS("CAntiFlowCool_W",CAntiFlowCool_W)
	return 1;
	}

int CAntiFlowCool::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		T_in_0 = T_in_1 = T_air;
		T_out_0 = T_out_1 = T_air;
    return 1;
    }
	return 0;
	}

int CAntiFlowCool::GetParams( char * )
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
	PARM( pIn_0->Flow_mol, "#Поток 0 моль" )
	PARM( pIn_1->Flow_mol, "#Поток 1 моль" )
	PARM( Ksi_0, "#Влияние теплоемкости 0" )
	PARM( Ksi_1, "#Влияние теплоемкости 1" )
	PARM( dT_Corp_0, "#dT от теплоемкости 0" )
	PARM( dT_Corp_1, "#dT от теплоемкости 1" )
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_dg, "#Теплопередача ГДж/ч" )
	return 0;
	}
