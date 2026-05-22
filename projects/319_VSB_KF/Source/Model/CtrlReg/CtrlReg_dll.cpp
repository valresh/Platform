#include "stdafx.h"
#include "CtrlReg.h"
#include "Info.h"

CCtrlReg* g_pCtrlReg;


extern "C" Q_DECL_EXPORT IRegInterface* GetInterface()
{
  return static_cast<IRegInterface*>(g_pCtrlReg);
}

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved )
{
	return TRUE;
}

CCtrlReg * pClass = (CCtrlReg*)NULL;
extern "C" 
{
	EXP IBaseModel * CreateObject(LPCTSTR ObjName )
	{ 
		if(!g_pCtrlReg)
			g_pCtrlReg = new CCtrlReg((char*)ObjName);
		return g_pCtrlReg;
	}
	EXP int Project = 1;
	EXP int Flags = 0;
	EXP char * Model = "CtrlReg";
	EXP char * Descr = "Description"; 
};


////BEGIN_NODE_INFO
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
