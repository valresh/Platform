#include "ServerExSharedPlugin_FlexRsu.h"
#include <EnumQueryTypes.h>
#include <macros/AutoCloser.h>
#include <AlarmEntry_.h>
#include <ServerExPluginTypes.h>
#include <FlexRsuCommon.h>
#include <fstream>


extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API DWORD MessagesSupport( BYTE **buffer, int &sizeReq, DWORD rsuID, tMemReallocer pMemR )
{
  DWORD size = 0;
  if( id_Rsu_Flex!= rsuID )
    return size;
  BYTE *pRequest = *buffer;
  QueryTypes qType = *(QueryTypes*) pRequest;
  int* N = (int*)(pRequest + sizeof(QueryTypes));

  std::ifstream hFile(ns_FlexRsu::GetMsgToOperatorFilePath(), std::ios::binary);
  if ( !hFile.is_open()) 
  {
     return size;
  }
  //
  //OVERLAPPED Overlapped = {0,0,N[0]*sizeof(CMessageEntry),0,0};
  DWORD nReadBytes      =      N[1]*sizeof(CMessageEntry);
  DWORD nBytesRead;
  // Используем в качестве выходного буфера - входной
  *buffer = pMemR( *buffer, nReadBytes, sizeReq );
  BOOL bResult = hFile.read((char*) *buffer, nReadBytes )? true:false;
  nBytesRead = hFile.gcount();
  hFile.close();
  if ( bResult && nReadBytes == nBytesRead )
    return nBytesRead;
  return size;
}
