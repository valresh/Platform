#include "stdafx.h"
#include "Info.h"
#include "TestNodes.h"
#include "HydroStruct.h"
#include "CV.h"
#include "EV.h"
#include "IV.h"


BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved )
{
	return TRUE;
}

extern "C" 
{
	EXP int Project = 1;
	EXP int Flags = 0;
	EXP char * Model =  "Valve_A_b";
	EXP char * Descr = "Description";
	EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Type )
	{
		IBaseModel* pValve = NULL;
		if(Type[0] == 'I' && Type[1] == 'V')
			pValve = (IBaseModel*)new CIV((char*)ObjName, (char*)Type);
		else if(Type[0] == 'E' && Type[1] == 'V')
			pValve = (IBaseModel*)new CEV((char*)ObjName, (char*)Type);
		else
			pValve = (IBaseModel*)new CCV((char*)ObjName, (char*)Type); 
		return pValve;		
	}
};

NODE_LIST
NODE_IN  ( 1, 0, "i","Входной фланец")
NODE_OUT ( 1, 1, "o","Выходной фланец")
END_LIST

STD_TEST

extern "C" int EXP TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
	if ( kNodes == 1 )
	{
		ModMsg ( "Внимание: У '%s' задана одна точка соединения.", (char*)ObjName );
	}
	else if ( kNodes < 2 )
	{
		ModMsg ( "У клапана '%s' неправильное число точек соединения - %d", (char*)ObjName, kNodes );
		return 1;
	}
	return StdTestNodes(ObjName, kNodes, ppNodes);
}

// MODEL_INFO(CValve_A_b, 0, "Valve_A_b", "Description")
// 
// BEGIN_NODE_INFO
// #include "Node.h"
// END_NODE_INFO

NO_PARM_INFO
//BEGIN_PARM_INFO
//#include "Points.h"
//END_PARM_INFO

NO_ACS_INFO
//BEGIN_ACS_INFO
//#include "ACS.h"
//END_ACS_INFO
