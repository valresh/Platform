#include "stdafx.h"
#define DLL_ACS
#include "ACScomponent.h"
#include "ACS_Def.h"
#include "Info.h"

BOOL APIENTRY DllMain(_HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  return TRUE;
}

extern "C" __declspec(dllexport) ACSInterface * GetACSInterface()
{
  return new ACScomponent();
};

MODEL_INFO(CACS,0,"","")

