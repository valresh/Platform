#include "QB_Server.h"
#include "../QuickBuilderH5xxDebug/QuickBuilderH5xxDebug.h"

#ifdef _WIN32
bool KServer::ShowACSObject( HWND hMainWnd, char * Name, void * pData )
{
  KQbBase* pPnt = __super::Find( Name );
  if( pPnt )
    return ShowDebugWnd( pPnt );
  return false;
}
#endif
