#include "stdafx.h"
#include "Motor.h"
#include "Err.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"


NODE_LIST
MOTOR_OUT ( 1, "o_motor", "Cоединение c валом")
//
END_LIST

STD_TEST

extern "C" int EXP TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
    if ( strstr ( ObjName, "10294" ))
        KKK();
	return ::StdTestNodes( ObjName, kNodes, ppNodes );
}

int CMotor::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
{
	return ::TestNodes( ObjName, kNodes, ppNodes );
}

int CMotor::SetMatr( void * pExternals, int PntGroup,  double dt, 
					double A[/* kExkV */], double B[ /*kE*/ ] )   // A * X = B
{
	ASS(0)
		return 0;
}

int CMotor::SetVar( void * pExternals, int PntGroup, double dt, 
				   // Переменные на входах
				   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	// Контролируем есть ли одинаковые адресса(может быть после перепрочтения РСУ)
	for(int i = 0; i <  MAX_DRIVE; i++)
	{
		if(!DrivePow[i])
			continue;
		for(int j = i + 1; i <  MAX_DRIVE; i++)
		{
			if(!DrivePow[i])
				continue;
			if(DrivePow[j] == DrivePow[i])
				DrivePow[j] = NULL;
		}
	}
	return 0;
}
