#include <crossplatform.h>
#include <stdio.h>
#include "TrendsRsuKernel.h"
#include <rsuPaths.h>
#include <rsuErr.h>
#include <TrendsDefines.h>

//
bool KTrendsRsuKernel::OpenTrendHead()
{
    if (m_pHeadT) return true;

    char szFile[_MAX_PATH * 4];
    sprintf(szFile, "%s_Trends.dat", rsuGetTrendPath().c_str());
    strcat(szFile, ".hdr");
    std::error_code error;
    DWORD dwSize = 1024 * 32 * sizeof(STrendId);// 32000 имен
    if (Count())
    {
        dwSize = Count() * sizeof(STrendId) + sizeof(CTrendHeader);
    }

    m_hMapHead.map(szFile, 0, dwSize, error, true);
    if (m_hMapHead.is_mapped())
    {
        m_pHeadT = (CTrendHeaderEx*)m_hMapHead.data();
        ASS(m_pHeadT);
        if (!m_pHeadT)
            return false;

        m_pHeadT->nVersion = 0;
        m_pHeadT->dwName = 0;
        m_pHeadT->kBlk = 0;
        m_pHeadT->kVar = 0;
        m_nInterval = std::max(1, m_nInterval);

        if (m_nInterval < 1)
            m_pHeadT->StepTc = 1;
        else
            m_pHeadT->StepTc = m_nInterval;
        return true;
    }
    ASS(FALSE);
    return false;
}

//
bool KTrendsRsuKernel::OpenTrendData(DWORD dwName)
{
    if (m_hMapData.is_mapped())
        m_hMapData.unmap();
    //
    UINT64 size = GetTrendSize();
    DWORD dwLow = DWORD(size);
    DWORD dwHigh = DWORD(size >> 32);
    char szFile[_MAX_PATH * 4];
    sprintf(szFile, "%s_Trends.dat", rsuGetTrendPath().c_str());
    std::error_code error;
    DWORD dwSize = 1024 * 32 * sizeof(STrendId);// 32000 имен
    m_hMapData.map(szFile, 0, dwSize, error, true);
    if (error) ASS(FALSE);
    return !error;
}
//
// Запись в файл трендов
void KTrendsRsuKernel::WriteTrends()
{
    if (!m_dwBlkAlloc)
        m_dwBlkAlloc = maxBlocksOut;

    if (m_pDataT == NULL)
    {
        m_hMapData.unmap();
        DWORD dwBklTotal = m_dwBklTotal;
        m_dwBklTotal = (m_dwBklTotal / m_dwBlkAlloc) * m_dwBlkAlloc;
        MapTrends();
        m_dwBklTotal = dwBklTotal;
    }
    else if ((m_dwBklTotal % m_dwBlkAlloc) == 0)
    {
        // Смена окна
        m_hMapData.unmap();
        MapTrends();
    }
    else if (m_AllocLeft < m_nSizeVar)
    {
        ASSD(0);
        // Смена окна
        m_hMapData.unmap();
        MapTrends();
    }
    //
    float* pTrend = m_pDataT;
//#ifdef WIN32
//    if (!m_pDataT || IsBadWritePtr(m_pDataT, m_nSizeVar))
//#else
//    if (!m_pDataT || mprotect(m_pDataT, m_nSizeVar, PROT_WRITE) != 0)
//#endif
//    {
//        return;
//    }
    //
    UINT nCount = Count();
    STrendDef* list = (STrendDef*)Obj(0);
    for (UINT n = 0; n < nCount; n++)
    {
        float v = 0;
        switch (list[n].eVal)
        {
        case enumValueDbl: v = (float)(*(double*)list[n].Void); break;
        case enumValueInt: v = (float)(*(int*)list[n].Void); break;
        case enumValueChr: v = (float)(*(char*)list[n].Void); break;
        case enumValueBol:
            if (-1 != list[n].bitOffset)
                v = (*(int*)list[n].Void) & (1 << list[n].bitOffset) ? 1.f : 0;
            else
                v = (float)(*(bool*)list[n].Void);
            break;
        case enumValueFlt: v = (float)(*(float*)list[n].Void); break;
        case enumValueI2:  v = (float)(*(short*)list[n].Void); break;
        case enumValueUI2: v = (float)(*(WORD*)list[n].Void); break;
        }
        *pTrend++ = v;
    }
    //
    m_dwBklTotal++;
    m_pHeadT->kBlk++;
    m_pDataT += nCount;
    m_AllocLeft -= m_nSizeVar;
}
//

UINT64 KTrendsRsuKernel::GetTrendSize()
{
    UINT64 r = m_nSizeMB;
    r *= 1024;
    r *= 1024;
    DWORD dwAllocKvant = mio::page_size();
    r /= dwAllocKvant;
    r *= dwAllocKvant;
    return r;
}
// Смена окна
void KTrendsRsuKernel::MapTrends()
{
    float* curDataT = m_pDataT;
    m_pDataT = NULL;
    pbBegWnd = nullptr;
    //
    UINT64 StartAddr = m_dwBklTotal * m_nSizeVar;
    UINT64 kPage = StartAddr / mio::page_size();
    UINT64 StartWnd = kPage * mio::page_size();
    UINT64 AllocSize = m_nSizeVar * m_dwBlkAlloc;
    UINT64 AllocPages = AllocSize / mio::page_size() + 2;
    AllocSize = AllocPages * mio::page_size();
    //
    // Увеличение файла трендов
    if (m_nAddMB > 0)
    {
        if ((StartWnd + AllocSize) >= GetTrendSize())
        {
            while ((StartWnd + AllocSize) >= GetTrendSize())
                m_nSizeMB += m_nAddMB;
            //
            //CSemLock LockIniTrends ( "Trends access", 5000 );
            m_pHeadT->dwName++;
            OpenTrendData(m_pHeadT->dwName);
        }
    }

    if (!m_dwBlkAlloc)
        m_dwBlkAlloc = maxBlocksOut;
    std::string lErr = "";
    while (pbBegWnd == nullptr)
    {
        AllocSize = m_nSizeVar * m_dwBlkAlloc;
        AllocPages = AllocSize / mio::page_size() + 2;
        AllocSize = AllocPages * mio::page_size();
        char szFile[_MAX_PATH * 4];
        sprintf(szFile, "%s_Trends.dat", rsuGetTrendPath().c_str());
        std::error_code error;
        if (m_hMapData.is_mapped())
            m_hMapData.unmap();
        m_hMapData.map(szFile, StartWnd, AllocSize, error);
        if (error.value())
            lErr = error.message();
        pbBegWnd = (BYTE*)m_hMapData.data();
        m_AllocLeft = AllocSize;
        if (pbBegWnd)
            break;
        m_dwBlkAlloc /= 2;
        if (m_dwBlkAlloc == 0)
            return;// Это уже большая жопа
    }
    if (NULL == pbBegWnd)
        return;

    m_pDataT = (float*)(pbBegWnd + (StartAddr - StartWnd));
    if (pbBegWnd == nullptr)
    {
        //LogMsg( "Ошибка MapTrends() '%d'", Err );
        ASS(FALSE);
    }
}
