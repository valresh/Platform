#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

NODE_LIST
NODE_IN  ( 1, 0, "i","Прием")
NODE_OUT ( 1, 1, "o","Подача")
NODE_IN  ( 2, 0, "in_water","Подача охлаждения")
NODE_OUT ( 2, 1, "out_water","Вывод охлаждения")
MOTOR_IN ( 3, "i_motor", "Cоединение c валом")
MOTOR_OUT ( 4, "o_motor", "Cоединение c датчиком")
//
END_LIST

STD_TEST

extern "C" int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	int kIO = 0;
	int kErr = 0;
	int kSens = 0;
	int nGroup = 0;
	for ( int v = 0; v < kVariablesData; v++ )
		VariablesData[v].WasSet = false;
	for ( int n = 0; n < kNodes; n++ )
	{
		CObjectPoint * pObjVar = ppNodes[n];
		pObjVar->Type = eГидравлика;
		char * Point = pObjVar->PntName;
		bool OK = false;

		for ( int v = 0; v < kVariablesData; v++ )
		{
			if ( lstrcmpi_m ( VariablesData[v].SetVarName, Point ) == 0 )
			{
				OK = true;
				if ( VariablesData[v].WasSet )
				{
					ModMsg ( "У  '%s' переменная '%s' задается повторно", ObjName, Point ), kErr++;
				}
				else
				{
					VariablesData[v].WasSet = true;
					pObjVar->Type = VariablesData[v].TypeVarStruct;
					pObjVar->PntGroup = VariablesData[v].GroupInModel;
					pObjVar->NumbInGroup = VariablesData[v].NumbInGroup;
					pObjVar->IO = VariablesData[v].IO;
				}
				kIO++;
				break;
			}
		}
		if ( !OK )
			ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;

	}
	if(kNodes > kIO + kSens)
		ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData ), kErr++;
	return kErr;
}

int CPC_1::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	return ::TestNodes( ObjName, kNodes, ppNodes );
}

#include "SetMatr.h"

int CPC_1::SetMatr( void * pExternals, int PntGroup,  double dt, 
				   double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	if ( PntGroup == 1 )
	{
 /* if ( On && P_in_Z > 0. )
    {
    double * Eq = A;
    Eq[P_0] = Omega_P_in_Z;
    Eq[F_0] = -1.;
    B[0] = Omega_P_in_Z * ( P_in_Z + 1 );
    Eq += 4;
    Eq[F_0] = 1.;
    Eq[F_1] = -1.;
    return 0;
    }
	*/
    double O = 1e-6;
		double * Eq = A;
		Eq[P_0] = -O;
		Eq[P_1] = O;
		Eq[F_0] = 1.;
		B[0] = ST_1.Flow_Gas;
		//
		Eq += 4;
		Eq[F_0] = 1.;
		Eq[F_1] = -1.;
		if ( RazGerm )
		{
			Eq[P_0] = -OmegaRazGerm;
			B[1] = OmegaRazGerm;
		}
		return 0;
	}
	if ( PntGroup == 2 )
	{
		double * Eq = A;
		Eq[P_0] = Omega_Water;
		Eq[P_1] = -Omega_Water;
		Eq[F_0] = -1.;
		//
		Eq += 4;
		Eq[F_0] = 1.;
		Eq[F_1] = -1.;
		return 0;
	}
	return 1;
}

#include "SetVar.h"

int CPC_1::SetVar( void * pExternals, int PntGroup, double dt, 
				  // Переменные на входах
				  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch(PntGroup)
	{
	case 1:
		{
			//if ( Var[F_0] >= 0. )
			//{
				ST_1.pFlow_In = (CFlow*)pVar[0];
				ST_1.pFlow_Out = (CFlow*)pVar[1];
			//}
			//else
			//{
			//	ST_1.pFlow_In = (CFlow*)pVar[1];
			//	ST_1.pFlow_Out = (CFlow*)pVar[0];
			//}
			//    ST_1.Рубашка.pIn_Vol = ST_1.pFlow_In;
			ST_1.Рубашка.pOut_Vol = ST_1.pFlow_Out;
			return 0;
		}
	case 2:
		{
			if ( Var[F_0] > 0. )
			{
				ST_1.Рубашка.pIn_Trub = (CFlow*)pVar[0];
				ST_1.Рубашка.pOut_Trub = (CFlow*)pVar[1];
			}
			else
			{
				ST_1.Рубашка.pIn_Trub = (CFlow*)pVar[1];
				ST_1.Рубашка.pOut_Trub = (CFlow*)pVar[0];
			}
			return 0;
		}
	case 3:
		{
			break;
		}
	case 4:
		{
			break;

		}
	}
	return 0;
}

