#include "stdafx.h"
#include "CallDrawHandler.h"
#include <ConnectY.h>
#include <rsuErr.h>

void CallDrawHandler( HWND hParent, char type, int len, void *addr, int f )
{
  if( !pFindAndShowYBlk )
    return;
  pFindAndShowYBlk( hParent, (LPSTR)addr, false );
}

void CallDrawCalcuCHandler( HWND hParent, char type, int len, void *addr, int f )
{
  LPSTR pszName = (LPSTR)addr;
  if( !pShowCalcuCDebugWindow )
    return;
  pShowCalcuCDebugWindow( hParent, pszName );
}

void CallDrawLC64Handler( HWND hParent, char type, int len, void *addr, int domain )
{
  if( !pCreateLCWnd || !pFindYObj )
    return;
  LPSTR pszName = (LPSTR)addr;
  CY_Base *pY = pFindYObj( pszName, domain );
  if( !pY )
    return;
  pCreateLCWnd( pszName, NULL, (CY_LC64_E_Base*)pY );	
}
