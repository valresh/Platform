#include "stdafx.h"
#include "UniHydro.h"
#include "Info.h"

BOOL APIENTRY DllMain(_HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  return TRUE;
}

static CUniHydro * point = NULL;
extern "C"
{
Q_DECL_EXPORT IBaseModel * CreateObject(const char * ObjectName)
{
  point = new CUniHydro(ObjectName);
  return point;
}
};
NO_NODE_INFO
NO_PARM_INFO
NO_ACS_INFO

extern "C" Q_DECL_EXPORT IBaseModel * GetBaseModelInterface()
{
    return static_cast<IBaseModel*>(point);
}

extern "C" Q_DECL_EXPORT HydroGroupsInterface * GetHydroGroupsInterface()
{
    HydroGroupsInterface * HydroGroups = static_cast<HydroGroupsInterface*>(point);
    return HydroGroups;
}



