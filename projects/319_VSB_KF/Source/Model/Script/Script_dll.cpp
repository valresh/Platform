#include "stdafx.h"
#include "Script.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CMainScript,0,"Скрипты","Поддержка скриптов")

NO_NODE_INFO
NO_PARM_INFO
NO_ACS_INFO
