#include <crossplatform.h>
#include "TrendsRsuKernel.h"
#include <time.h>

KTrendsRsuKernel::KTrendsRsuKernel()
    : m_nInterval(1)
    , m_nSizeMB(100)
    , m_nAddMB(100)
    , m_dCurTime(0.0)
    , m_bInit(false)
    //
    , m_nSizeVar(0)
    //
    , m_dwBklTotal(0)
    , m_dwBlkAlloc(maxBlocksOut)
    //
    , m_pHeadT(NULL)
    , m_pDataT(NULL)
    , pbBegWnd(NULL)
    //
    , m_AllocLeft(0)
    , m_nStep(0)
{
}

KTrendsRsuKernel::~KTrendsRsuKernel()
{
    Close();
}

void KTrendsRsuKernel::Close()
{
    if (m_hMapHead.is_mapped()) m_hMapHead.unmap();
    if (m_hMapData.is_mapped()) m_hMapData.unmap();
    pbBegWnd = NULL;
    m_pHeadT = NULL;
}


int KTrendsRsuKernel::StepK0()
{
    // Чтобы была запись при первом шаге
    m_dCurTime = (double)m_nInterval;
    return 0;
}

int KTrendsRsuKernel::StepKT(double dtH, time_t StartTime)
{
    if (!m_bInit)
    {
        FillArray();
        {
            // Обнуляем содержимое файла трендов
            //CSemLock LockIniTrends ( "Trends access", 5000 );
            if (!OpenTrendHead()) return 1;
            if (!OpenTrendData(0)) return 1;
        }
        FillHeader();
        m_pHeadT->kBlk = m_dwBklTotal;
        m_pHeadT->kVar = Count();
        m_bInit = true;

        //Для отображения в АРМ-ах модельного времени
        m_pHeadT->trendsStartTime = m_pHeadT->StartTime = StartTime;

        // Количество записей
        UINT64 dwCountWrite = 0;
        if (m_nSizeVar > 0)
        {
            dwCountWrite = GetTrendSize() / m_nSizeVar;
        }
        //
    }
    //
    m_dCurTime += dtH * 3600.;
    while (m_dCurTime >= m_nInterval)
    {
        WriteTrends();
        m_dCurTime -= m_nInterval;
    }
    return 0;
}
