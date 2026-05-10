#include "stdafx.h"
#include "CC_1.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"
#include "TestNodes.h"


NODE_LIST
NODE_IN  ( 1, 0, "i|in_gas","Вход")
NODE_OUT ( 1, 1, "o|out_gas","Выход")
//NODE_IN  ( 2, 0, "in_oil","Вход смазки")
//NODE_OUT ( 2, 1, "out_oil","Выход смазки")
//NODE_IN  ( 3, 0, "in_uplot","Вход уплотнения")
//NODE_OUT ( 3, 1, "out_uplot","Выход уплотнения")
MOTOR_IN ( 2, "i_motor", "Cоединение c валом")
MOTOR_OUT ( 3, "o_motor", "Cоединение c датчиком")
END_LIST

STD_TEST

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	int kIO = 0;
	int kErr = 0;
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
	if(kNodes > kIO)
		ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData ), kErr++;
	return kErr;
}


int CCC_1::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes( ObjName, kNodes, ppNodes );
	if ( Res )
		return Res;
	return 0;
}

#include "SetMatr.h"

// Q(F) = A * F + C      F - моль
void CCC_1::Напор ( double Flow_mol, double & A, double & C )
{
	double M_Flow = Flow_Surge * Oborot;
	if ( M_Flow < 1. )
	{
		A = -1. / ( Omega_Inv * P_in );
		C = 1.;
		Is_Surge = false;
		return;
	}
	Flow_M3 = Flow_mol * To_m3_in;
	// 
	if ( Flow_M3 < M_Flow ) // Везде M3
	{
		A = 0.;
		C = 1.;
		Is_Surge = true;
		return;
	}
	Is_Surge = false;
	Surge_stock = Flow_M3 / M_Flow;
	double F = Surge_stock - 1.;
	while ( F < F_Характеристики[pos_Характеристики] && pos_Характеристики > 0 )
		pos_Характеристики--;
	if ( pos_Характеристики >= k_Points )
		pos_Характеристики = k_Points - 1;
	while ( F > F_Характеристики[pos_Характеристики+1] && pos_Характеристики < k_Points - 1 )
		pos_Характеристики++;
	double F0 = F_Характеристики[pos_Характеристики];
	double F1 = F_Характеристики[pos_Характеристики+1];
	double P0 = P_Характеристики[pos_Характеристики];
	double P1 = P_Характеристики[pos_Характеристики+1];
	// Q = P0 + ( F - F0 ) * ( P1 - P0 ) / ( F1 - F0 )
	// Q = a * F + c
	// F = Fcompr * M3 / M_Flow - 1.
	// Q = a * Fcompr * M3 / M_Flow + c - a
	double N = ( P1 - P0 ) / ( F1 - F0 );
	double a = N;
	double c = P0 - F0 * N;
	A = a * To_m3_in / M_Flow;      //    A ~ .../mol
	C = c - a;
}


int CCC_1::SetMatr( void * pExternals, int PntGroup,  double dt, 
				   double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	if(PntGroup == 1)
	{
		bool Инверсия = false;
		if ( Oborot < 0.1 && ST_1._P_in > ST_1._P_out + 0.1 )
		{
			Инверсия = true;
			//    O = Omega_Inv;
		}
		double * Eq = A;
		// P0 * k0 * Q - P1 = 0
		// P * k0 * ( aF * F + cF ) - P1 = 0
		// _P0 * k0 * aF * F + k0 * cF * P0  - P1 = 0
		// Выключение:  
		// _P0 * k0 * aF * F + k0 * cF * P0  - P1 = 0
		// P0 - P1 - F / O = 0
		Напор ( ComprFlow, aF, cF );
		double Q = aF * ComprFlow + cF;
		if ( 1 )//Q * ST_1.k_dP > 1. )
		{
			Eq[P_0] = ST_1.k_dP * cF;
			Eq[F_0] = ST_1.k_dP * aF * P_in;
			Eq[P_1] = -1;
		}
		else
		{
			Eq[P_0] = 1.;
			Eq[P_1] = -1;
		}
		//  Eq[P_0] = ST_1.k_dP;
		//  Eq[F_0] = -1. / O;
		Eq += 4;
		Eq[F_0] = 1.;
		Eq[F_1] = -1.;
		if ( RazGerm )
		{
			Eq[F_0] = -OmegaRazGerm;
			B[1] = -OmegaRazGerm;
		}
	}
	return 0;
}

#include "SetVar.h"

int CCC_1::SetVar( void * pExternals, int PntGroup, double dt, 
				  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	if(PntGroup == 1)
	{
  SET_BP BreakPoint;
		if ( Var[F_0] > 0. )
		{
			ST_1.pFlow_In = (CFlow*)pVar[0];
			ST_1.pFlow_Out = (CFlow*)pVar[1];
		}
		else
		{
			ST_1.pFlow_In = (CFlow*)pVar[1];
			ST_1.pFlow_Out = (CFlow*)pVar[0];
		}
		ComprFlow = 0.9 * ComprFlow + 0.1 * Var[F_0];
		//  P_in = 0.9 * P_in + 0.1 * Var[P_0];
		//  P_out = 0.9 * P_out + 0.1 * Var[P_1];
		P_in = Var[P_0];
		P_out = Var[P_1];
		k_dP_Real = aF * ComprFlow + cF;
		k_dP_Summ = ST_1.k_dP * k_dP_Real;
	}
	else if ( PntGroup == 2 )
	{
		//pMotor = (CMotorData*)pVar[0];
	}
	else if( PntGroup == 3 )	
	{
// 		int iSens = PntGroup - VariablesData[N_SENS].GroupInModel;
// 		if(iSens > kSens)
// 			ASS(0);
// 		ppSens[iSens] = (CMotorData*)pVar[0];
// 		ppSens[iSens]->On = On;
// 		ppSens[iSens]->RelOborot = Oborot;
// 		ppSens[iSens]->Power =ST_1._Pow_MWt;
	}
	return 0;
}

