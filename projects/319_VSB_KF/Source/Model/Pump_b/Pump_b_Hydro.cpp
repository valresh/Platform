#include "stdafx.h"
#include "Pump_b.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"
#include "SetVar.h"

NODE_LIST
	NODE_IN  ( 1, 0, "i","Прием")
	NODE_OUT ( 1, 1, "o","Подача")
    NODE_IN  ( 2, 0, "i_cool","Подача охлаждения")
    NODE_OUT ( 2, 1, "o_cool","Вывод охлаждения")
	MOTOR_IN ( 3, "i_motor", "Cоединение c валом")
	MOTOR_OUT ( 4, "o_motor", "Cоединение c датчиком")
END_LIST

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	int kIO = 0;
	int kMotor = 0;
	int kErr = 0;
	int nGroup = 0;
	for ( int v = 0; v < kVariablesData; v++ )
		VariablesData[v].WasSet = false;
	int nCool = 0;
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
				if(v == 0 || v == 1)
					kIO++;
				else if(v == 2 || v == 3)
					nCool++;
				else if(v == 4 || v == 5)
					kMotor++;
				break;
			}
		}
		if ( !OK )
			ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;

	}
	if(kIO != 2)
		ModMsg ( "У  '%s' неправильно заданы вход/выход продукта", ObjName), kErr++;
	if(nCool != 0 && nCool != 2)
		ModMsg ( "У  '%s' неправильно заданы вход/выход охлаждения", ObjName), kErr++;
	if(kNodes > kIO + kMotor + nCool)
		ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData ), kErr++;
	return kErr;
}

int CPump_b::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	int Res = ::TestNodes( ObjName, kNodes, ppNodes );
	if ( Res )
		return Res;
	bool externalMotor = false;
	bool bCool = false;
	for ( int n = 0; n < kNodes; n++ )
	{
		if (!lstrcmp(ppNodes[n]->PntName, "i_cool") || !lstrcmp(ppNodes[n]->PntName, "in_water"))
			bCool = true;
		if (!lstrcmp( VariablesData[4].SetVarName, ppNodes[n]->PntName))
			externalMotor = true;			
	}
	if(!externalMotor)
	{
		char szMotor[MAX_OBJ_NAME] = {};
        sprintf(szMotor, "Мотор_%s", ObjName);
		_pMotor = new CMotor(szMotor, this);
		pMotor = &_pMotor->MotorData;
		if(bCool)
		{
			char szCoolObj[MAX_OBJ_NAME] = {};
            sprintf(szCoolObj, "Охлаждения_%s", ObjName);
			pCoolObj = new CCoolObj(szCoolObj, this);
			pCoolObj->pMotor = &_pMotor->MotorData;
		}
	}
	return 0;
}


int CPump_b::SetVar( void * pExternals, int PntGroup, double dt, 
					double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	SET_BP BreakPoint;
	if(OBJ("M-610"))
		KKK();
	switch ( PntGroup )
	{
	case 1:
		{
			_P_In = Var[P_0] - 1.;
			_P_Out = Var[P_1] - 1.;
			Head = _P_Out - _P_In;
			F_mol = Var[F_0];
			if(fabs(F_mol) < 1e-5)
				F_mol = 0.0;
			pFlow_i = (CFlow*)pVar[0];
			pFlow_o = (CFlow*)pVar[1];
		}
		break;
	case 2:
		{
			pCoolObj->SetVar(pExternals, 1, dt, Var, pVar);
		}
		break;
	case 3:
		{
			break;
		}
	case 4:
		{
			break;
		}
	default:
			break;
	}
	return 0;
}
