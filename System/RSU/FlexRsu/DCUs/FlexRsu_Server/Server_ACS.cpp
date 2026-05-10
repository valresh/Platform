#include "Server.h"

#ifdef _WIN32
bool KServer::ShowACSObject( void * pInfo, HWND hMainWnd, LPCSTR Name, void * pData )
{
  return KFlexRsu::ShowACSObject( pInfo, hMainWnd, Name, pData );
}
#endif
