#include "stdafx.h"
#include "Info.h"
#include "TestNodes.h"
#include "HydroStruct.h"
#include "CV.h"
#include "EV.h"
#include "IV.h"


#ifndef LINUX
BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved )
{
	return TRUE;
}
#endif

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

//STD_TEST
int VA_StdTestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )\
  {\
  int kIO = 0;\
  int kErr = 0;\
  if ( kNodes > kVariablesData )\
    {\
    ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData );\
    return 1;\
    }\
    for ( int v = 0; v < kVariablesData; v++ )\
      VariablesData[v].WasSet = false;\
      for ( int n = 0; n < kNodes; n++ )\
        {\
        CObjectPoint * pObjVar = ppNodes[n];\
        char * Point = pObjVar->PntName;\
        bool OK = false;\
        for ( int v = 0; v < kVariablesData; v++ )\
          {\
          if ( lstrcmpi_m ( VariablesData[v].SetVarName, Point ) == 0 )\
            {\
            OK = true;\
            if ( VariablesData[v].WasSet )\
              {\
              ModMsg ( "У  '%s' переменная '%s' задается повторно", ObjName, Point ), kErr++;\
              }\
            else\
              {\
              VariablesData[v].WasSet = true;\
              pObjVar->Type = VariablesData[v].TypeVarStruct;\
              pObjVar->PntGroup = VariablesData[v].GroupInModel;\
              pObjVar->NumbInGroup = VariablesData[v].NumbInGroup;\
              pObjVar->IO = VariablesData[v].IO;\
              }\
              break;\
            }\
          }\
          if ( !OK )\
            ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;\
        }\
        return kErr;\
  }

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
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
	return VA_StdTestNodes(ObjName, kNodes, ppNodes);
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
