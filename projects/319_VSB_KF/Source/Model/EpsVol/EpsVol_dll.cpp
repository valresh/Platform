#include "stdafx.h"
#include "Data.h"
#include "ComponentControl.h"

extern "C" Q_DECL_EXPORT ComponentInterface * GetComponentInterface(const char* ComponentName, ComponentInterface::TypeComponent TypeDefault, ComponentInterface::TypeSource Load)
{
  return new ComponentControl(ComponentName, TypeDefault, Load);
};

BOOL APIENTRY DllMain( _HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  return TRUE;
}
