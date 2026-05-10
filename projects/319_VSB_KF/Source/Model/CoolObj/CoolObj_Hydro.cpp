#include "stdafx.h"
#include "CoolObj.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST
NODE_IN  ( 1, 0, "in_cool","Подача охлаждения")
NODE_OUT ( 1, 1, "out_cool","Вывод охлаждения")
MOTOR_IN ( 2, "i_motor","Обороты аппарата")
//
END_LIST


STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	if ( kNodes > kVariablesData )
	{
		ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData );
		return 1;
	}
	if ( kNodes != kVariablesData )
	{
		ModMsg ( "У '%s' подключены не все точки (%d != %d)", ObjName, kNodes, kVariablesData );
		return 1;
	}
    int kErr = 0;
	for ( int v = 0; v < kVariablesData; v++ )
		VariablesData[v].WasSet = false;
	for ( int n = 0; n < kNodes; n++ )
	{
		CObjectPoint * pObjVar = ppNodes[n];
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
				break;
			}
		}
		if ( !OK )
			ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;
	}
	return kErr;
}

int CCoolObj::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
  if ( strstr ( ObjName, "C-3001S-МС"))
    KKK();
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
	if ( Res )
		return Res;
	return 0;
}

#include "SetMatr.h"

#define P_0 0
#define P_1 2
#define F_0 1
#define F_1 3

int CCoolObj::SetMatr( void * pExternals, int PntGroup,  double dt, 
					  double A[/* kExkV */], double B[ /*kE*/ ] )
{
	SET_BP BreakPoint;
	ASS( PntGroup == 1)
		double * Eq = A;
	Eq[P_0] = Omega;
	Eq[P_1] = -Omega;
	Eq[F_0] = -1.;
	B[0] = 0.;
	Eq += 4;
	Eq[F_0] = 1.;
	Eq[F_1] = -1.;
	return 0;
}

#include "SetVar.h"

int CCoolObj::SetVar( void * pExternals, int PntGroup, double dt, 
					 // Переменные на входах
					 double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	switch(PntGroup)
	{
	case 1:
		{
			CFlow * pFlow0 = (CFlow * )pVar[0];
			CFlow * pFlow1 = (CFlow * )pVar[1];
			if ( Var[F_0] > 0. )
			{
				pFlow_in = pFlow0;
				pFlow_out = pFlow1;
			}
			else
			{
				pFlow_in = pFlow1;
				pFlow_out = pFlow0;
			}
			dP = Var[P_0] - Var[P_1];
			F_kg = pFlow_in->Flow_kg();
			return 0;
		}
	case 2:
		break;
	}	
	return 0;
}

