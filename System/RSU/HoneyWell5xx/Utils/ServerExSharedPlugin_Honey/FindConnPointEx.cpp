#include "ServerExSharedPlugin_Honey.h"
#include <BaseType_.h>
#include "LocalUtils.h"


extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API CBase* FindConnPointEx( LPCSTR ObjName, DWORD TypeID )
{
  CBase* pB = _FindConnPoint( ObjName, TypeID, 0 );
  return pB;
}
