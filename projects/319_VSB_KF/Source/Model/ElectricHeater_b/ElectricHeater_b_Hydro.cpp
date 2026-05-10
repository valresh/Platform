#include "stdafx.h"
#include "ElectricHeater_b.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"

NODE_LIST
NODE_IN(1, 0, "i", "i Description")
NODE_OUT(1, 1, "o", "o Description")
END_LIST

STD_TEST

extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	return ::StdTestNodes( (char*)ObjName, kNodes, ppNodes );
}

int CElectricHeater_b::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes(ObjName, kNodes, ppNodes);
	if ( Res )
		return Res;
	return 0;
}

int CElectricHeater_b::SetMatr(  void * pExternals, int PntGroup,  double dt, 
						double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	double * Eq = A;
	switch ( PntGroup )
	{
	case 1:
		{
			Eq[P_0] = -Omega;
			Eq[P_1] = Omega;
			Eq[F_0] = 1.0;

			Eq += 4;
			Eq[F_0] = 1.0;
			Eq[F_1] = -1.0;
			break;
		}		
	default:
		ASS(0)
		break;
	}
	return 0;
}

int CElectricHeater_b::SetVar( void * pExternals, int PntGroup, double dt, 
					   // Переменные на входах
					   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch ( PntGroup )
	{
	case 1:
		{
			pFlow_i = (CFlow*)pVar[0];
			pFlow_o = (CFlow*)pVar[1];
			Flow_i = Var[F_0];
			P_i = Var[P_0] - 1.0;
			P_o = Var[P_1] - 1.0;
		}
		break;
	default:
		ASS(0)
		break;
	}
	return 0;
}

