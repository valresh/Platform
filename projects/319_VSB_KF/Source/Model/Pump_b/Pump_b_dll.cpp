#include "stdafx.h"
#include "Pump_b.h"
#include "Info.h"
#include "Volumetric.h"
#include "Dynamic.h"


BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved )
{
	return TRUE;
}

CPump_b * pClass = (CPump_b*)NULL;
extern "C" 
{
	EXP int Project = 1;
	EXP int Flags = 0;
	EXP char * Model = "Pump_b";
	EXP char * Descr = "Description";
	EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Type )
	{
		if(!lstrcmp(Type, "PDP"))
			return new CVolumetric((char*)ObjName);
		return new CDynamic((char*)ObjName);
	}
};


//MODEL_INFO(CPump_b, 0, "Pump_b", "Description")

//BEGIN_NODE_INFO
//#include "Node.h"
//END_NODE_INFO

NO_PARM_INFO
//BEGIN_PARM_INFO
//#include "Points.h"
//END_PARM_INFO

NO_ACS_INFO
//BEGIN_ACS_INFO
//#include "ACS.h"
//END_ACS_INFO
