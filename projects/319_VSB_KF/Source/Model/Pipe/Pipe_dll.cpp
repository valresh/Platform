#include "stdafx.h"
#include "Pipe.h"
#include "Info.h"

#ifdef LINUX
#else
BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved	 )
{
  return TRUE;
}
#endif



//MODEL_INFO(CPipe,0,"Pipe","Трубопровод")
CPipe * pClass = (CPipe*)0;

extern "C" {
EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Type )
  { 
  if ( strncmp ( ObjName, "cin", 3 ) == 0 || strncmp ( ObjName, "cout", 4 ) == 0 )
    return new CCatPipe((char*)ObjName); 
  return new CPipe((char*)ObjName);
  }
EXP int Project = 1;
EXP int Flags = 0;
EXP char * Model = "Pipe";
EXP char * Descr = "Трубопровод"; 
  };


NO_NODE_INFO
//BEGIN_NODE_INFO
//#include "Node.h"
//END_NODE_INFO

NO_PARM_INFO
//#include "IO_Info.h"
//#include "Points.h"
//END_PARM_INFO

NO_ACS_INFO
//BEGIN_ACS_INFO
//#include "ACS.h"
//END_ACS_INFO
