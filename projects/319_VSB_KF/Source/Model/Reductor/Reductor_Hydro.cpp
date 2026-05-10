#include "stdafx.h"
#include "Reductor.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST
NODE_IN  ( 1, 0, "i","Вход" )
NODE_OUT ( 1, 1, "o","Выход")
//NODE_IN ( 2, 0, "gas","Газ")
END_LIST

STD_TEST

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::StdTestNodes( ObjName, kNodes, ppNodes );
	if(Res)
		return Res;
	if(kNodes != 2)
	{
		ModMsg ( "У редуктора '%s' неправильные входы", ObjName);
		Res = 1;
	}
	return Res;
}

int CReductor::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes( ObjName, kNodes, ppNodes );
	if ( Res )
		return Res;
	return 0;
}

#include "SetMatr.h"

int CReductor::SetMatr( void * pExternals, int PntGroup,  double dt, 
					   double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	double * Eq = A;
	const double O = 1e-6;
	if ( PntGroup == 2 )
	{
		Eq[F_0] = 1.;// F = 0
		return 0;
	}
	double Pz_a = P_Z + 1.;
	if ( _P_out_Z )
	{
		if ( P_in <= Pz_a )
		{
			MATR_OMEGA(Omega)
			return 0;
		}
		if ( P_out <= Pz_a )
		{
			// F = O * ( Pz - P_out )
			Eq[P_1] = Omega;
			B[0] = Omega * Pz_a;
			Eq[F_0] = 1.;
			Eq += 4;
			Eq[F_0] = 1.;
			Eq[F_1] = -1.;
		}
		else
		{
			Eq[F_0] = 1.; // Потоки = 0 назад
			Eq += 4;
			Eq[F_1] = 1.;
		}
		return 0;
	}
	if ( _dP )
	{
		if ( P_in - P_out < P_Z || Flow_out < 0. )
		{
			MATR_OMEGA(O)
		}
		else
		{
			Eq[P_0] = 1.;
			Eq[P_1] = -1.;
			B[0] = P_Z * 1.0001;
			//
			Eq += 4;
			Eq[F_0] = 1.;
			Eq[F_1] = -1.;
		}
		return 0;
	}
	//////////////////////////////////////////
	// Задано P_in
	if ( P_out > Pz_a )
	{
		MATR_OMEGA(O)
		return 0;
	}
	if(F_in > F_Max)
	{
		double P_k = F_Max / Omega + Pz_a;
		double b = P_k - F_Max / Omega_F;
		Eq[P_0] = Omega_F;
		B[0] = Omega_F * b;
		Eq[F_0] = -1.;
		Eq += 4;
		Eq[F_0] = 1.;
		Eq[F_1] = -1.;

		return 0;
	}
	if ( P_in > Pz_a )
	{
		// F = O * ( P_in - Pz )
		Eq[P_0] = Omega;
		B[0] = Omega * Pz_a;
		Eq[F_0] = -1.;
		Eq += 4;
		Eq[F_0] = 1.;
		Eq[F_1] = -1.;
	}
	else
	{
		Eq[F_0] = 1.; // Потоки = 0 назад
		Eq += 4;
		Eq[F_1] = 1.;
	}
	return 0;
}

#include "SetVar.h"

int CReductor::SetVar( void * pExternals, int PntGroup, double dt, 
					  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	if ( PntGroup == 2 )
	{
		pGas = (CFlow*)pVar[0];
		return 0;
	}
	//  P_in = 0.9 * P_in + 0.1 * Press[0];
	P_in = Var[P_0];
	_P_in = Var[P_0] - 1.;
	P_out = Var[P_1];
	_P_out = P_out - 1.;
	_P_io = _P_in - _P_out;
	Flow_out = Var[F_1];
	pF_in = (CFlow*)pVar[0];
	pF_out = (CFlow*)pVar[1];
	if ( Flow_out > 0. )
		*pF_out = *pF_in;
	else
		*pF_in = *pF_out;
	F_in = pF_in->Flow_mol;
	_Flow_out = pF_out->Flow_kg();
	return 0;
}

