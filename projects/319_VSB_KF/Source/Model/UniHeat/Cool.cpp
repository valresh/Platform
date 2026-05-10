#include "stdafx.h"
#define DLL_UniHeat
#include "UniHeat.h"
#include "math.h"
#include "Err.h"
#include "SR.h"
#include "SetData.h"



CCool::CCool( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain ),
In_0( "In_0", this ),
State_0( "State_0", this ),
In_1( "In_1", this ),
Out_1( "Out_1", this )
	{
  Model = "Cool";
	IterH_1 = false;
	kGas_0 = 1.;
	kGas_1 = 1.;
	M_Corp_t = 2.;
	M_0 = 0.1;
	h_0 = 0.;
	}

CCool_W::CCool_W()
	{
	memset ( this, 0, sizeof ( CCool_W ));
	k_Cp_0 = 0.01;
	k_Cp_1 = 0.01; 
	Tau_0 = 0.001; 
	Tau_1 = 0.001;
	Cp_1 = 10.;
	T_in_1 = T_out_1 = 15.;
	T_in_0 = T_0 = 15.;
	}


void CCool::Calc( double dt )
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
  double F_0 = fabs ( pIn_0 -> Flow_mol );
	h_in_0 = pIn_0 -> h_Flow;
	In_0.PH ( pIn_0->P, h_in_0, pComp_0 );
	T_in_0 = In_0.T;
//
	h_in_1 = pIn_1->h_Flow;
	In_1.PH ( pIn_1->P, h_in_1, pComp_1 );
	T_in_1 = In_1.T;
//
// 
	if ( !_finite ( State_0.L ))
		State_0.L = State_0.M_comm;
	double Lev = State_0.L / ( State_0.M_comm + State_0.M_sol );
	double ks = KS * ( 0.95 * Lev + 0.05 );
  double F_1 = fabs ( pIn_1->Flow_mol );
  double Fcp_1 = F_1 * Cp_1;
  Lam_1 = 100.;
  if ( Lam_1 * Fcp_1 > ks )
    Lam_1 = ks / Fcp_1;
///////////////////////////////////////////////
	double D_in = T_in_1 - T_0;
	double D_out = D_in * exp ( -Lam_1 );
	T_out_1 = T_0 + D_out;
//
  double Q = Fcp_1 * fabs ( T_out_1 - T_in_1 );
//
	if ( IterH_1 )
		{
		if ( T_out_1 > T_in_1	)
			h_out_1 = h_in_1 + Q / F_1;
		else
			h_out_1 = h_in_1 - Q / F_1;
		Out_1.PH( pOut_1 -> P, h_out_1, pComp_1 );
		pOut_1->h_Flow = h_out_1;
		double T_out_1_h = Out_1.T;
	////
		if ( Q > 100. )
			{
			double dT = T_out_1_h - T_out_1;
			double kCps; 
			if ( T_out_1 > T_in_1	)
				kCps = LimdX ( -k_Cp_1 * dT, 0.1 );
			else
				kCps = LimdX ( k_Cp_1 * dT, 0.1 );
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
		if ( Q > 100. )
			{
			double dH = F_1 * fabs ( h_out_1 - h_in_1 );
			double Ks = dH / Q - 1.;
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
//
	Q_Heat_0 = F_0 * ( h_in_0 - h_0 ) + Fcp_1 * ( T_in_1 - T_out_1 );
	Q_dg = fabs ( Q_Heat_0 ) * 1e-6;
	Q_cal = Q_dg * M_cal;
	double dH_in = dt * Q_Heat_0;
	h_0 += dH_in / M_0;
	pOut_0 ->h_Flow = h_0;
	double T_min = State_0.T - 10.;
	if ( T_min < T_air )
		T_min = T_air;
	State_0.T_min = T_min;
	State_0.T_max = T_min + 20.;
	State_0.MH_Corp( M_0, M_0 * h_0, pComp_0, M_Corp_t * 500. );
	T_0 = State_0.T;
	P_0 = State_0.P;
	double dM = dt * ( pIn_0 -> Flow_mol - pOut_0 -> Flow_mol );
	if ( dM < -0.1 * M_0 )
		dM = -0.1 * M_0;
	if ( dM > 0.1 * M_0 )
		dM = 0.1 * M_0;
	M_0 += dM;
	}

int CCool::SaveState( )
	{
	S_CLASS("CCool_W",CCool_W)
	return 0;
	}

int CCool::RestoreState( char * StrName )
	{
	R_CLASS("CCool_W",CCool_W)
	return 1;
	}

int CCool::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		T_in_0 = T_0 = T_air;
		T_in_1 = T_out_1 = T_air;
		h_0 = pN2Flow->h_Flow;
    return 1;
    }
	return 0;
	}

int CCool::GetParams( char * )
	{
	#include "IO_Parms.h"
	PARM( T_in_0, "#Т на входе 0" )
	PARM( T_0, "#Т 0" ) 
	PARM( T_in_1, "#Т на входе 1" ) 
	PARM( T_out_1, "#Т на выходе 1" )
	PARM( M_0, "#M 0" ) 
	PARM( Cp_1, "#Cp 1" )
  PARM( Lam_1, "#Lam 1" )
	PARM( pIn_0->Flow_mol, "#Поток 0 моль" )
	PARM( pIn_1->Flow_mol, "#Поток 1 моль" )
  PARM( Q_cal, "#Теплопередача Гкал/ч" )
  PARM( Q_dg, "#Теплопередача ГДж/ч" )
	return 0;
	}
