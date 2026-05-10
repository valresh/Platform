#include "TrendsRemoteClient.h"
#include <time.h>

KTrendsRemoteClient::KTrendsRemoteClient(LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
    : m_proxy(a_cl), m_ObjName(_ObjName)
{
}

KTrendsRemoteClient::~KTrendsRemoteClient()
{
    m_loadedLibs.clear();
    KTrendsRsuKernel::~KTrendsRsuKernel();
}

int KTrendsRemoteClient::Init(int)
{
    return 0;
}

int KTrendsRemoteClient::Step0()
{
    return KTrendsRsuKernel::StepK0();
}

int KTrendsRemoteClient::Step1()
{
    LoadPlugins();
    return 0;
}

int KTrendsRemoteClient::StepT(double dt)
{
    time_t startTime = pSys ? pSys->startTime : time(NULL);
    return KTrendsRsuKernel::StepKT(dt, startTime);
}

void KTrendsRemoteClient::FillHeader()
{
    KTrendsRsuKernel::FillHeader();
    if (pSys)
    {
        pSys->nTrends = m_dwBklTotal;
    }
}
// Запись в файл трендов
void KTrendsRemoteClient::WriteTrends()
{
    KTrendsRsuKernel::WriteTrends();
    if (pSys)
    {
        pSys->nTrends = m_dwBklTotal;
    }
}
