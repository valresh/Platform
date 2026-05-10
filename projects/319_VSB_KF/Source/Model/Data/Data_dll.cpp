#include "stdafx.h"
#include "ALL_Data.h"
#include "Info.h"

#ifdef LINUX
#else
bool NoWork;
BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved	 )
{
  if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
  {
    char szEXE[_MAX_PATH] = "\0";
    char Name[_MAX_FNAME];
    ::GetModuleFileName(NULL,szEXE,_MAX_PATH);
    _splitpath_s(szEXE,NULL,0,NULL,0,Name,sizeof(Name),NULL,0);
    if ( lstrcmpi ( Name, "tsedit" ) != 0 )
    {
      NoWork = false;
    }
    else
      NoWork = true;
  }
  return TRUE;
}
#endif



MODEL_INFO(CALL_Data,0,"DATA","Хранение потоков и составов")

NO_NODE_INFO
NO_PARM_INFO
NO_ACS_INFO
