#include <crossplatform.h>
#include "TrendsRSU_Server.h"
#include <rsuNoNames.h>
#include <rsuErr.h>

TrendsRSUServer::TrendsRSUServer( LPCSTR pszModelName )
{
}

#ifdef _WIN32
void TrendsRSUServer::ShowObject( LPCSTR pszName )
{
  //if( m_hCmdWnd )
  //  SendMessage( m_hCmdWnd, WM_USER+1, 1, (LPARAM)pszName );
}
#endif