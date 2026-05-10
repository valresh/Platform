#include "ServerExSharedPlugin_Honey.h"
#include <EnumQueryTypes.h>
#include <CommonH.h>
#include <macros/AutoCloser.h>
#include <_alarmentry.h>
#include <ServerExPluginTypes.h>

extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API DWORD AlarmsSupport( BYTE **buffer, int &sizeReq, DWORD rsuID, tMemReallocer pMemR )
{
    DWORD size = 0;
    if (id_Rsu_HoneyWell5xx != rsuID)
        return size;
    BYTE* pRequest = *buffer;
    QueryTypes qType = *(QueryTypes*)pRequest;
    int* N = (int*)(pRequest + sizeof(QueryTypes));

    std::ifstream hFile(GetAlarmFileName((N[2] % 2) == 1), std::ios::binary);
    if (!hFile.is_open())
        return size;

    //
    DWORD nReadBytes = N[1] * sizeof(CAlarmEntry);
    DWORD nBytesRead;
    // Используем в качестве выходного буфера - входной
    *buffer = pMemR(*buffer, nReadBytes, sizeReq);
    BOOL bResult = hFile.read((char*)*buffer, nReadBytes) ? true : false;
    nBytesRead = hFile.gcount();
    hFile.close();

    if (bResult && nReadBytes == nBytesRead)
        return nBytesRead;

    return size;
}

extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API DWORD AnyOtherRequest( BYTE **buffer, int &sizeReq, DWORD rsuID, tMemReallocer pMemR, LPCSTR pszCompName )
{
  DWORD size = 0;
  if( id_Rsu_HoneyWell5xx != rsuID )
    return size;
  BYTE *pRequest = *buffer;
  int offset = 0;
  QueryTypes qType = *(QueryTypes*)&pRequest[offset];
  if( qUpdateSound!=qType )
    return size;
  offset += sizeof(qType);
  if( offset >= sizeReq )
    return size;

  CCBHSystem *pCBSystem = GetControlBuilderHSystemData();
  if( !pCBSystem )
    return size;

  pCBSystem->bSound = false;

  /*CEventEntry send;
  send.bMessage = true;
  send.eData  = id_Msg;
  send.eType  = (EValueType)CEventLogType::etWarning;
  lstrcpyn( send.szName, "Снятие звуковой сигнализации", sizeof(send.szName)+sizeof(send.szValue) );

  lstrcpyn( send.szUser, pszCompName, sizeof(send.szName)+sizeof(send.szValue) );
  WriteEvents( send );*/
  return size;
}