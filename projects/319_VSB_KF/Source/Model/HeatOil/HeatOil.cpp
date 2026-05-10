#include "stdafx.h"
#define DLL_HeatOil
#include "heatoil.h"
#include "math.h"
#include "Err.h"
#include "CommProc.h"
#include "DataTypes.h"

CHeatOil_W::CHeatOil_W()
{
	memset ( this, 0, sizeof ( CHeatOil_W ));
	T = T_air;
	F_Z_kg = 500.0;
}

CHeatOil::CHeatOil( char * Name, IBaseModel * pMain, double* _pOborot ) : IBaseModel ( Name, pMain )
, In ( "Вход", this )
, Out ( "Выход", this )
{
	Model = "Охлаждение и смазка";
	pFlow_Oil_in = NULL;
	pFlow_Oil_out = NULL;
//////////////////////////////////////////////////////////////////////////
	k_def = 1.0;	
#include "IO_Clear.h"
#include "HeatOil_Pnt.h"
//////////////////////////////////////////////////////////////////////////
	pOborot = _pOborot;
	T_oil_in = 0.;
	T_oil_out = 0.;
	T_Fire = 0.;
	pT_oil = new CAnalogD( pMain, "T_подшипника", "AO" );
	T_oilZ = 50.0 + r01() * 10.0;
}

int CHeatOil::Step1( )
{
	return 0;
}

int CHeatOil::GetParams( char * )
{
#define GET_PARAMS
#include "IO_Parms.h"
#include "HeatOil_Pnt.h"
	return 0;
}

int CHeatOil::SaveState( )
{
	S_CLASS("W",CHeatOil_W)
		return 0;
}

int CHeatOil::RestoreState( char * StrName )
{
	COPY_FIRST
		R_CLASS("W",CHeatOil_W)
		return 1;
}

int CHeatOil::UpdateParam( CParams & Param )
{
	return 0;
}

int CHeatOil::StepT( double dt )
{
	SET_BP BreakPoint;
	if ( pFlow_Oil_out && pFlow_Oil_in )
		*pFlow_Oil_out = *pFlow_Oil_in;
	double Flow_oil_kg;
	if(!pFlow_Oil_in || !pFlow_Oil_out)
		return 1;
	Flow_oil_kg = fabs ( pFlow_Oil_in->Flow_kg());
	In.PH( pFlow_Oil_in );
	T_oil_in = In.T;
	if ( T_oil_in > T_max )
		T_oil_in = T_max;
	if ( F_Z_kg <= 0. )
		F_Z_kg = Flow_oil_kg;
	double Ksi = dt * 3600. / Tau_cool;
	double Old = 1. / ( 1. + Ksi );
	double New = Ksi * Old;
	//double T_room = 20.;
	//////////////////////////////////////////////////////////////////////////
	double W = *pOborot * k_def;
	if ( W < 0.01 )
	{
		T = Old * T + New * T_room;	//dT/dt = (T_room - T)
	}
	else
	{
		double kF = 100.;
		if ( Flow_oil_kg * kF > F_Z_kg )
			kF = F_Z_kg / Flow_oil_kg;
		double T_lim = T_oil_in + ( T_oilZ - T_oil_in ) * W * kF;
		if ( T_lim > T_max )
			T_lim = T_max;
		if ( Use_Tz )//|| kStep <= 10 )
		{
			F_Z_kg = Flow_oil_kg;
			T = T_oilZ;
			Use_Tz = false;
		}
		else
			T = Old * T + New * T_lim;		//dT/dt = (T_lim - T)
	}
	//////////////////////////////////////////////////////////////////////////
	T_oil_out = T + T_Fire;	
	*pFlow_Oil_out = *pFlow_Oil_in;
	Out.PT( pFlow_Oil_out->P, T_oil_out, pFlow_Oil_out->Get_Comp());
	pFlow_Oil_out->h_Flow = Out.h;
	pFlow_Oil_out->T = T_oil_out;
	if ( pT_oil )
		*pT_oil = T_oil_out;	
	return 0;
}

void CHeatOil::NoDefect( )
{
	k_def = 1.;
}


ON_DEFECT(CHeatOil)
{
	if ( k_def == 1. && pT_oil->Use())
	{
		//k_def = Нагрев->Dbl * 1.5;
		Tau_cool *= 0.125;
		kdT_def++;
	}
	return 1;
}

#include "SetMatr.h"

int CHeatOil::SetMatr( void * pExternals, int PntGroup,  double dt, 
               double A[/* kExkV */], double B[ /*kE*/ ] )
  {
	SET_BP BreakPoint;
	/*if ( dP_Z > 0. )
	{
		double K = dP / dP_Z;
		Omega *= 1. + LimdX ( K - 1., 0.1 );
		if ( Omega < 1. )
			Omega = 1.;
		if ( Omega > 10000. )
			Omega = 10000.;
	}*/
  double * Eq = A;
  Eq[P_0] = Omega;
  Eq[P_1] = -Omega;
  Eq[F_0] = -1.;
  //
  Eq += 4;
  Eq[F_0] = -1.;
  Eq[F_1] = 1.;
  //
	return 0;
}

int CHeatOil::SetVar( void * pExternals, int PntGroup, double dt, 
                     double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
	SET_BP BreakPoint;
	if ( Var[F_0] > 0. )
	  {
		pFlow_Oil_in = (CFlow*)pVar[0];
		pFlow_Oil_out = (CFlow*)pVar[1];
	  }
	else
	  {
		pFlow_Oil_in = (CFlow*)pVar[1];
		pFlow_Oil_out = (CFlow*)pVar[0];
	  }
	return 0;
  }
