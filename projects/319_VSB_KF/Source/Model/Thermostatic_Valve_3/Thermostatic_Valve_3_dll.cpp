#include "stdafx.h"
#include "Thermostatic_Valve_3.h"
#include "Info.h"


BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved )
{
	return TRUE;
}

MODEL_INFO(CThermostatic_Valve_3, 0, "Thermostatic_Valve_3", "Description")

//BEGIN_NODE_INFO
//#include "Node.h"
//END_NODE_INFO
//
NO_PARM_INFO
//BEGIN_PARM_INFO
//#include "Points.h"
//END_PARM_INFO

NO_ACS_INFO
//BEGIN_ACS_INFO
//#include "ACS.h"
//END_ACS_INFO
