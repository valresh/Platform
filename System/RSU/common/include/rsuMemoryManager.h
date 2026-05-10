#pragma once

#include "CommonRSUs.h"

#include <mio.hpp>
#include <cstring>

class COMMONRSUS_API KRsuMemoryManager
{
    struct SWorkingKvant
    {
        SWorkingKvant()
        {
            memset(this, 0, sizeof(*this));
        }
        ~SWorkingKvant()
        {
        }
        BYTE* pMem;
        mio::mmap_sink* hVarMapping;
        DWORD Size;
        DWORD Pos;
        SWorkingKvant* pNext;
        BYTE* GetMemory(DWORD count);
        DWORD LeftFree();
    };
    SWorkingKvant* m_pFirstBlock, * m_pActiveBlock;
    DWORD m_addMBytes;
public:
    KRsuMemoryManager();
    ~KRsuMemoryManager();
protected:
    KRsuMemoryManager(KRsuMemoryManager& src);
    KRsuMemoryManager& operator = (KRsuMemoryManager& src);
public:
    size_t Create(LPCSTR pszName, DWORD nInitialMBytes, DWORD addMBytes = 20);
    BYTE* AllocMemory(DWORD size);
    void Clear();
};
