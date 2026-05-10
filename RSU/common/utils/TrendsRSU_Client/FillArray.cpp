#include "TrendsRemoteClient.h"
#include <chrono>
#include "LogFile.h"

void KTrendsRemoteClient::EnumPointsCount(tEnumIo_Objs pEnumerator, tTrendInfoPoint pInfo, int& pointCount, std::vector<STrendPointInfo>& pointsInfo)
{
    if (!pEnumerator)
    {
        return;
    }

    const char* pszName;
    CBase* pBase;
    DWORD ID = 0;
    int fcsNumber = 0;
    while (pEnumerator(ID, id_Unknown, &pBase, &pszName, &fcsNumber))
    {
        DWORD N = pBase->ID_CLASS;
        const int c = pInfo(N, pszName, pBase, fcsNumber, (int)pointsInfo.size(), &pointsInfo[0]);
        if (c <= 0)
        {
            continue;
        }

        pointCount += c;
        memset(&pointsInfo[0], 0, sizeof(STrendPointInfo) * c);
        for (int i = 0; i < c; ++i)
        {
            pointsInfo[i].bitOffset = -1;
        }
    }
}

void KTrendsRemoteClient::AddPoints(tEnumIo_Objs pEnumerator, tTrendInfoPoint pInfo, std::vector<STrendPointInfo>& pointsInfo)
{
    if (!pEnumerator)
    {
        return;
    }

    const char* pszName;
    CBase* pBase;
    DWORD ID = 0;
    int fcsNumber = 0;
    while (pEnumerator(ID, id_Unknown, &pBase, &pszName, &fcsNumber))
    {
        DWORD N = pBase->ID_CLASS;
        const int c = pInfo(N, pszName, pBase, fcsNumber, (int)pointsInfo.size(), &pointsInfo[0]);
        if (c <= 0)
        {
            continue;
        }

        for (int i = 0; i < c; i++)
        {
            STrendDef obj;
            obj.dwID = pointsInfo[i].dwID;
            obj.eVal = pointsInfo[i].eValType;
            obj.Void = pointsInfo[i].pAddr;
            obj.bitOffset = pointsInfo[i].bitOffset;
            strcpy_s(obj.name, pointsInfo[i].szName);
            UINT nReturn = AddObj(&obj);
            ASS(nReturn != -1);
        }

        memset(&pointsInfo[0], 0, sizeof(STrendPointInfo) * c);
        for (int i = 0; i < c; ++i)
        {
            pointsInfo[i].bitOffset = -1;
        }
    }
}

void KTrendsRemoteClient::FillArray()
{
    CLogFile::Log("TrendsW::FillArray...");
    auto start = std::chrono::steady_clock::now();
    std::vector<STrendPointInfo> pointsInfo;
    pointsInfo.resize(8 * 1024);
    for (size_t i = 0, c(pointsInfo.size()); i < c; ++i)
    {
        pointsInfo[i].bitOffset = -1;
    }

    char flClasses[0x10000];
    ZeroMemory(flClasses, sizeof(flClasses));
    int pointCount = 0;
    for (int plugCount = 0; plugCount < _countof(m_Plugins); ++plugCount)
    {
        if (!m_Plugins[plugCount].pInfo)
        {
            break;
        }

        if (m_Plugins[plugCount].pEnumObjs)
        {
            EnumPointsCount(m_Plugins[plugCount].pEnumObjs, m_Plugins[plugCount].pInfo, pointCount, pointsInfo);
        }

        if (m_Plugins[plugCount].pEnumIOs)
        {
            EnumPointsCount(m_Plugins[plugCount].pEnumIOs, m_Plugins[plugCount].pInfo, pointCount, pointsInfo);
        }
    }
    CLogFile::LogEx(LOGLEVEL_LOW, "будет добавлено %d точек...\r\n", pointCount);
    Realloc(pointCount);
    for (int plugCount = 0; plugCount < _countof(m_Plugins); ++plugCount)
    {
        if (!m_Plugins[plugCount].pInfo)
        {
            break;
        }

        if (m_Plugins[plugCount].pEnumObjs)
        {
            AddPoints(m_Plugins[plugCount].pEnumObjs, m_Plugins[plugCount].pInfo, pointsInfo);
        }

        if (m_Plugins[plugCount].pEnumIOs)
        {
            AddPoints(m_Plugins[plugCount].pEnumIOs, m_Plugins[plugCount].pInfo, pointsInfo);
        }
    }
    //
    m_nSizeVar = Count() * sizeof(float);
    //
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    CLogFile::Log("формирование точек trend-ов %lld ms\n", static_cast<long long>(elapsed));
}
