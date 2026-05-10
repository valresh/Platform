#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"

CCooler::CCooler( char * Name, IBaseModel * pMain )	: IBaseModel ( Name, pMain ),
In ( "Вход", this ),
Out ( "Выход", this )
	{
  Model = "Cooler";
	pIn = NULL;
	pOut = NULL;
	T_corp = 15.;
	Cp_Cool = 10.;
	}

CCooler_W::CCooler_W()
	{
	memset ( this, 0, sizeof ( CCooler_W ));
	k_To_Air = 0.;
	T_max = 100.;
	Cp_Corp = 1000.;
	k_Cp_Cool = 0.01;
	}

void CCooler::CalcCool( double dt )
	{
	*pOut = *pIn;
//
	CComp * pComp = pIn->Get_Comp();
	if ( pComp == NULL )
		return ;
//
  double F = fabs ( pIn -> Flow_mol );
	h_in = pIn -> h_Flow;
	In.T_max = T_max + 10.;
	In.PH ( pIn );
	T_in = In.T;
	double FCp = F * Cp_Cool;
	double Lam_heat = 100.;
	if ( 100. * FCp > KS )
		{
		Lam_heat = KS / FCp;
		}
	double Eps = exp ( -Lam_heat );
	double Eps1 = 1. - Eps;
	T_out = T_air * Eps1 + T_in * Eps;
//
  double Q = Cp_Cool * ( T_out - T_in );
//
	h_out = h_in + Q;
	Out.T_max = T_max + 10.;
	Out.PH( pOut -> P, h_out, pComp );
	pOut -> h_Flow = h_out;
	double T_out_h = Out.T;
	pOut -> T = T_out_h;
////
	if ( fabs ( Q ) > 0.1 )
		{
		double dT = T_out_h - T_out;
		double kCps; 
		if ( T_out > T_in	)
			kCps = LimdX ( -k_Cp_Cool * dT, 0.1 );
		else
			kCps = LimdX ( k_Cp_Cool * dT, 0.1 );
		Cp_Cool *= 1. + kCps;
		if ( Cp_Cool < 10. )
			Cp_Cool = 10.;
		if ( Cp_Cool > 100000. )
			Cp_Cool = 100000.;
		}
	if ( !_finite ( Cp_Cool ) )
		Cp_Cool = 10.;
	}

void CCooler::Calc( double dt )
	{
	*pOut = *pIn;
//
	CComp * pComp = pIn->Get_Comp();
	if ( pComp == NULL )
		return ;
//
  double F = fabs ( pIn -> Flow_mol );
	h_in = pIn -> h_Flow;
	In.T_max = T_max + 10.;
	In.PH ( pIn );
	T_in = In.T;
	double FCp = F * Cp_Cool;
	double FCpKS = FCp + KS;
	T_corp = 
	(	T_corp * FCpKS * Cp_Corp + 
	  dt * ( KS * FCp * T_in + ( k_To_Air * T_air + Q_Heat ) * FCpKS )) /
	(	FCpKS * Cp_Corp + dt * ( k_To_Air * FCpKS + KS * FCp ));
	if ( T_corp > T_max )
		T_corp = T_max + LimdX ( T_corp - T_max, 10. );
	T_out = ( FCp * T_in + KS * T_corp ) / FCpKS;
/* KS->infinity
	T_out = 
		(	T_out * Cp_Corp + dt * ( FCp * T_in + k_To_Air * T_air + Q_Heat )) /
		( Cp_Corp + dt * ( k_To_Air + FCp ));
	if ( T_out > T_max )
		T_out = T_max + LimdX ( T_out - T_max, 10. );
*/
//
  double Q = Cp_Cool * ( T_out - T_in );
//
	h_out = h_in + Q;
	Out.T_max = T_max + 10.;
	Out.PH( pOut -> P, h_out, pComp );
	pOut -> h_Flow = h_out;
	double T_out_h = Out.T;
	pOut -> T = T_out_h;
////
	if ( fabs ( Q ) > 0.1 )
		{
		double dT = T_out_h - T_out;
		double kCps; 
		if ( T_out > T_in	)
			kCps = LimdX ( -k_Cp_Cool * dT, 0.1 );
		else
			kCps = LimdX ( k_Cp_Cool * dT, 0.1 );
		Cp_Cool *= 1. + kCps;
		if ( Cp_Cool < 10. )
			Cp_Cool = 10.;
		if ( Cp_Cool > 100000. )
			Cp_Cool = 100000.;
		}
	if ( !_finite ( Cp_Cool ) )
		Cp_Cool = 10.;
	}

int CCooler::SaveState( )
	{
	S_CLASS("W",CCooler_W)
	return 0;
	}

int CCooler::RestoreState( char * StrName )
	{
	R_CLASS("W",CCooler_W)
	return 1;
	}
