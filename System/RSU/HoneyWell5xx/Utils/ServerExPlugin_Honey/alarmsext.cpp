#include "ServerExPlugin_Honey.h"
#include <EnumQueryTypes.h>
#include <CommonH.h>
#include <macros/AutoCloser.h>
#include <_alarmentry.h>
#include <ServerExPluginTypes.h>

extern "C" SERVEREXPLUGIN_HONEY5XX_API DWORD AlarmsSupport( BYTE **buffer, int &sizeReq, DWORD rsuID, tMemReallocer pMemR )
{
  DWORD size = 0;
  if( id_Rsu_HoneyWell5xx != rsuID )
    return size;
  BYTE *pRequest = *buffer;
  QueryTypes qType = *(QueryTypes*) pRequest;
  int* N = (int*)(pRequest + sizeof(QueryTypes));

  std::ifstream hFile(GetAlarmFileName((N[2] % 2) == 1), std::ios::binary);
  if (!hFile.is_open())
      return size;
  
  //
  //OVERLAPPED Overlapped = {0,0,N[0]*sizeof(CAlarmEntry),0,0};
  DWORD nReadBytes      =      N[1]*sizeof(CAlarmEntry);
  DWORD nBytesRead;
  // Используем в качестве выходного буфера - входной
  *buffer = pMemR( *buffer, nReadBytes, sizeReq );
  BOOL bResult = hFile.read((char*)*buffer, nReadBytes) ? true : false;
  nBytesRead = hFile.gcount();
  hFile.close();

  if ( bResult && nReadBytes == nBytesRead )
    return nBytesRead;

  return size;
}
