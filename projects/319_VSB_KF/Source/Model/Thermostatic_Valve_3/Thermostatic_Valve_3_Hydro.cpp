#include "stdafx.h"
#include "Thermostatic_Valve_3.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"

NODE_LIST
NODE_IN  ( 1, 0, "i1","Вход прямой") 
NODE_IN  ( 1, 1, "i2","Вход 2")
NODE_IN	 ( 1, 2, "o","Выход") 
END_LIST

STD_TEST

double Calc_Omega( double V, double X1, double Y1, double X2, double Y2 )
{
	if ( V < X1 )
		return Y1;
	if ( V > X2 )
		return Y2;
	double Y = Y1 + ( Y2 - Y1 ) * ( V - X1 ) / ( X2 - X1 );
	return Y;
}

extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	return ::StdTestNodes( (char*)ObjName, kNodes, ppNodes );
}

int CThermostatic_Valve_3::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes(ObjName, kNodes, ppNodes);
	if ( Res )
		return Res;
	return 0;
}

int CThermostatic_Valve_3::SetMatr(  void * pExternals, int PntGroup,  double dt, 
						double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	double * Eq = A;
	double O_1 = ::Calc_Omega(Position, Xo1_1, Yo1_1, Xo1_2, Yo1_2 ) * 0.01;
	double O_2 = ::Calc_Omega(Position, Xo2_1, Yo2_1, Xo2_2, Yo2_2 ) * 0.01;
	O_1 += 1e-7;
	O_2 += 1e-7;
	O_1 *= Omega;
	O_2 *= Omega;
	switch ( PntGroup )
	{
	case 1:
		{
			Eq[P_0] = -O_1;
			Eq[P_2] = O_1;
			Eq[F_0] = 1.;
			Eq[F_2] = -O_1 / Omega;
			
			//
			Eq += 6;
			Eq[P_1] = -O_2;
			Eq[P_2] = O_2;
			Eq[F_1] = 1.;
			Eq[F_2] = -O_2 / Omega;
			// Сумма = 0 
			Eq += 6;
			Eq[F_0] = 1.;
			Eq[F_1] = 1.;
			Eq[F_2] = 1.;
		}
		break;

	default:
		ASS(0)
			break;
	}
	return 0;
}

int CThermostatic_Valve_3::SetVar( void * pExternals, int PntGroup, double dt, 
					   // Переменные на входах
					   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			Flow[0] = Var[F_0];
			Flow[1] = Var[F_1];
			Flow[2] = Var[F_2];
			
			pFlow[0] = (CFlow*)pVar[0];
			pFlow[1] = (CFlow*)pVar[1];
			pFlow[2] = (CFlow*)pVar[2];			
		}
		break;
	default:
		ASS(0)
		break;
	}
	return 0;
}

