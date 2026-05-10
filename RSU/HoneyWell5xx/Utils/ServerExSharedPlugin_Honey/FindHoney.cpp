#include "ServerExSharedPlugin_Honey.h"
#include <BaseType_.h>
#include "LocalUtils.h"


extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API DWORD FindHoney( DWORD qRsuNumber, LPSTR pszNameIn, DWORD type, BYTE **buffer, int &sizeReq, tMemReallocer pMemR )
{
//  if( strstr(pszNameIn, "FAO006VE300UZ.CLOSE") ) //szFldName
//    KKK();
  return _FindHoney( qRsuNumber, pszNameIn, type, buffer, sizeReq, pMemR );
}
