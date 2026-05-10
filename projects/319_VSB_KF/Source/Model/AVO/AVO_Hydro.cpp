#include "stdafx.h"
#include "AVO.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"
#include "TestNodes.h"


#define SZ_MOTOR "i_motor"
#define MOTOR_NUMBER_GROUP	4

NODE_LIST
NODE_IN  ( 1, 0, "i","Вход" )
NODE_OUT ( 1, 1, "o","Выход")
NODE_IN  ( 2, 0, "in_water","Вход воды")
NODE_OUT ( 2, 1, "out_water","Выход воды")
MOTOR_IN ( 3, SZ_MOTOR"_#", "Вход(#-номер cоединение c валом)")
END_LIST



STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
    {
    int kIO = 0;
	int kErr = 0;
	int kMotor = 0;
	int nGroup = 0;
	for ( int v = 0; v < kVariablesData; v++ )
		VariablesData[v].WasSet = false;
	for ( int n = 0; n < kNodes; n++ )
	{
		CObjectPoint * pObjVar = ppNodes[n];
		char * Point = pObjVar->PntName;
		bool OK = false;
		if(strstr(Point, SZ_MOTOR))
		{
			pObjVar->Type = VariablesData[MOTOR_NUMBER_GROUP].TypeVarStruct;
			pObjVar->PntGroup = VariablesData[MOTOR_NUMBER_GROUP].GroupInModel + nGroup++;
			pObjVar->NumbInGroup = VariablesData[MOTOR_NUMBER_GROUP].NumbInGroup;
			pObjVar->IO = VariablesData[MOTOR_NUMBER_GROUP].IO;
			kMotor++;
		}
		else
		{
			for ( int v = 0; v < kVariablesData; v++ )
			{
                OK = ( strcmp ( VariablesData[v].SetVarName, Point ) == 0 );
				if(OK)
				{
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
						kIO++;
					}
					break;
				}
			}
			if ( !OK )
				ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;	
		}	
	}
	//if(kIO != 2)
		//ModMsg ( "У '%s' неправильно заданы вход/выход", (char*)ObjName ), kErr++;
	if(kNodes > kIO + kMotor)
	{
		ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData );
		kErr++;
	}
	return kErr;
}

int CAVO::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{	
	return ::TestNodes(ObjName, kNodes, ppNodes);
}

#include "SetMatr.h"

int CAVO::SetMatr( void * pExternals, int PntGroup,  double dt, 
				  double A[/* kExkV */], double B[ /*kE*/ ] )
{
	switch ( PntGroup )
	{
	case 1:
		{
			SET_BP BreakPoint;
			//      Проходимость = Omega * kOmega;
			//      double O = Проходимость;
			double O = Omega * kOmega;
			double * Eq = A;
			if ( Конденсация )
			{
				Eq[P_0] = O;
				Eq[P_1] = -O;
				Eq[F_0] = -1.;
				B[0] = -O * Eps_cond * dP_cond;
			}
			else
			{
				Eq[P_0] = O;
				Eq[P_1] = -O;
				Eq[F_0] = -1.;
			}
			Eq += 4;
			Eq[F_0] = 1.;
			Eq[F_1] = -1.;
		}
		return 0;
	case 2:
		{
			MATR_OMEGA(OmegaWater)
		}
		return 0;
	default:
		ASS(0)
			break;
	}
	return 0;
}

#include "SetVar.h"

int CAVO::SetVar( void * pExternals, int PntGroup, double dt, 
				 double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	switch ( PntGroup )
	{
	case 1:
		{
			SET_BP BreakPoint;
			if ( Var[F_0] > 0. )
  			{
				Heat.pIn_Trub = (CFlow*)pVar[0];
				Heat.pOut_Trub = (CFlow*)pVar[1];
        Heat1flow.pIn_Work = (CFlow*)pVar[0];
        Heat1flow.pOut_Work = (CFlow*)pVar[1];
				_dP = Var[P_0] - Var[P_1];
			  }
			else
			  {
				Heat.pIn_Trub = (CFlow*)pVar[1];
				Heat.pOut_Trub = (CFlow*)pVar[0];
        Heat1flow.pIn_Work = (CFlow*)pVar[1];
        Heat1flow.pOut_Work = (CFlow*)pVar[0];
				_dP = Var[P_1] - Var[P_0];
			  }
			_F_kg = Heat.pIn_Trub->Flow_kg();
		}
		return 0;
	case 2:
		{
			SET_BP BreakPoint;
			IO_2(pIn_Water,pOut_Water)
		}
		return 0;
	default:
			break;
	}
	if(PntGroup >= VariablesData[MOTOR_NUMBER_GROUP].GroupInModel)
	{
		KKK();
// 		int iMotor = PntGroup - VariablesData[N_MOTOR].GroupInModel;
// 		if(iMotor > kMotor)
// 			ASS(0);
// 		Motors[iMotor] = (CMotorData*)pVar[0];
	}
	return 0;
}

