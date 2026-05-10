#include "stdafx.h"
#include "AirDry.h"
#include "Err.h"
#include "CommProc.h"


void CAirDry::Calc( double dt )
	{
	In.PH ( pFlow_in );
	double T_in = In.T;
	double F = fabs ( pFlow_in ->Flow_mol );
	double CK = Cp + K * dt;
	double T_out = ( F * T_in * CK + K * Cp * T ) / ( F * CK + K * Cp );
	T = ( Cp * T + K * T_out * dt ) / CK;
	s_T = T;
	s_P = In.P - 1.;
  //sPress->Mes = 0;
  //sTemp->Mes = 0;
  //SetSens(sPress,"U", In.P - 1. );
  //SetSens(sTemp,"U", T );
//
	CComp * pIn = pFlow_in->Get_Comp();
	cGas_out->Set ( pIn );
	if ( cGas_out->IsComp[G_H2O] && cGas_out->Cmol[G_H2O]	> 0. && cGas_out->Cmol[G_H2O]	< 0.1 )
		{
		cGas_out->IsComp[G_H2O] = false;
		cGas_out->Cmol[G_H2O] = 0.;
		cGas_out->Norm();
		}
	pFlow_out ->T = T_out;
	pFlow_out ->h_Flow = H_IG ( T_out, cGas_out );
	pFlow_out ->To_kg = Mw ( cGas_out );
	pFlow_out ->To_m3 = M3;
	pFlow_out ->ID_COMP = ID_Gas_out;
	pFlow_out ->pComp = cGas_out;
	}
