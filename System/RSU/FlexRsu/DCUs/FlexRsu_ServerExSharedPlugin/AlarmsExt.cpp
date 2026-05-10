#include "ServerExSharedPlugin_FlexRsu.h"
#include <EnumQueryTypes.h>
#include <macros/AutoCloser.h>
#include <AlarmEntry_.h>
#include <ServerExPluginTypes.h>
#include <FlexRsuCommon.h>
#include <fstream>


extern "C" SERVEREXSHAREDPLUGIN_FLEXRSU_API DWORD AlarmsSupport(
    BYTE** buffer, int& sizeReq, DWORD rsuID, tMemReallocer pMemR)
{
    DWORD size = 0;
    if (id_Rsu_Flex != rsuID)
        return size;

    BYTE* pRequest = *buffer;
    QueryTypes qType = *(QueryTypes*)pRequest;
    int* N = (int*)(pRequest + sizeof(QueryTypes));
    auto fileName = ns_FlexRsu::GetAlarmFileName((N[2] % 2) == 1);
    std::ifstream hFile(fileName, std::ios::binary);
    if (!hFile.is_open())
        return size;

    DWORD offset = N[0] * sizeof(CAlarmEntry);
    DWORD nReadBytes = N[1] * sizeof(CAlarmEntry);
    DWORD nBytesRead;
    *buffer = pMemR(*buffer, nReadBytes, sizeReq);
    hFile.seekg(offset, std::ios::beg);
    if (!hFile.good())
        return size;

    hFile.read((char*)*buffer, nReadBytes);
    nBytesRead = hFile.gcount();
    hFile.close();
    if (nBytesRead == nReadBytes)
        return nBytesRead;

    return size;
}
