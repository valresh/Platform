#include "FlexRsu.h"

enum EStateKeys
{
    sKeyNull = 0,
    sKeyDRcount = 0x7A482591,
    sKeyDR = 0x5E8A7692,
    sKeyEnd = 0xC43C1C0D,
    sKeyBlockCount = 0x8A5F3A84,
    sKeyBlockName = 0x6B3C2D04,
    sKeyBlockID = 0x6475AD41,
    sKeyFieldsCount = 0x0BC96E8C,
    sKeyField = 0x42D9849C,
};

void SaveBlock(IFlexStateSer& saver, KFlexBlockBase* pBlock)
{
    DWORD pos = 0;
    DWORD N = 0;
    SVarInfo var;
    while (pBlock->EnumVars(pos, var))
    {
        if (SVarInfo::efSaveInState & var.flags)
        {
            ++N;
        }
    }

    saver.SimpleWrite(sKeyFieldsCount);
    saver.SimpleWrite(N);
    pos = 0;
    while (BYTE* pMem = pBlock->EnumVars(pos, var))
    {
        if (!(SVarInfo::efSaveInState & var.flags))
        {
            continue;
        }

        saver.SimpleWrite(sKeyField);
        saver.WriteStr(var.pszName);
        saver.SimpleWrite(var.evtType);
        saver.SimpleWrite(var.size);
        saver.SimpleWrite(pMem, var.size);
    }
}

void SaveDR(IFlexStateSer& saver, KFlexDR* pDR)
{
    saver.SimpleWrite(sKeyBlockCount);
    saver.SimpleWrite(pDR->m_nBlocks);
    for (DWORD n = 0; n < pDR->m_nBlocks; ++n)
    {
        KFlexBlockBase* pBlock = pDR->m_ppBlocks[n];
        if (!pBlock->m_szVisibleName.empty())
        {
            saver.SimpleWrite(sKeyBlockName);
            saver.WriteStr(pBlock->m_szVisibleName);
        }
        else
        {
            saver.SimpleWrite(sKeyBlockID);
            saver.SimpleWrite(pBlock->m_id);
        }

        saver.WriteStr(pBlock->m_pszType);
        DWORD posStart = saver.GetPosition();
        const DWORD L = saver.SimpleWrite(posStart);
        SaveBlock(saver, pBlock);
        DWORD posEnd = saver.GetPosition();
        DWORD length = posEnd - posStart - L;
        saver.WriteAtPosition(posStart, length);
    }
}

int KFlexRsu::SaveState(IFlexStateSer& saver)
{
    saver.SimpleWrite(sKeyDRcount);
    saver.SimpleWrite(m_nDR);
    for (DWORD n = 0; n < m_nDR; ++n)
    {
        KFlexDR* pDR = m_ppDR[n];
        saver.SimpleWrite(sKeyDR);
        saver.WriteStr(pDR->m_szFileName);
        DWORD posStart = saver.GetPosition();
        const DWORD L = saver.SimpleWrite(posStart);
        SaveDR(saver, pDR);
        DWORD posEnd = saver.GetPosition();
        DWORD length = posEnd - posStart - L;
        saver.WriteAtPosition(posStart, length);
    }

    return 0;
}

template<typename T>
int ReadAndTest(IFlexStateSer& restorer, const T comp)
{
    T r;
    if (sizeof(r) != restorer.SimpleRead(r))
    {
        ASS(!1);
        return 1;
    }

    if (r != comp)
    {
        ASS(!2);
        return 2;
    }

    return 0;
}

bool RestoreBlock(IFlexStateSer& restorer, KFlexBlockBase* pBlock, DWORD version)
{
    EStateKeys key = sKeyNull;
    if (sizeof(key) != restorer.SimpleRead(key))
    {
        return false;
    }

    switch (key)
    {
    case sKeyFieldsCount:
        break;
    default:
        return false;
    }

    DWORD C = 0;
    restorer.SimpleRead(C);
    for (DWORD n = 0; n < C; ++n)
    {
        if (ReadAndTest(restorer, sKeyField))
            return false;
        char szName[64 * 4] = { 0 };
        eVarType typeS = evtHZ, typeR = evtHZ;
        tVarSizeType_ sizeS = 0, sizeR = 0;
        restorer.ReadLenStr(szName, sizeof(szName));
        restorer.SimpleRead(typeS);
        if (version == StateVersion_win1251)
        {
            unsigned char tmpSizeS = 0;
            restorer.SimpleRead(tmpSizeS);
            sizeS = tmpSizeS;
        }
        else
        {
            restorer.SimpleRead(sizeS);
        }

        BYTE* pMem = pBlock->GetField(szName, &typeR, &sizeR);
        if (!pMem)
        {
            restorer.Shift(sizeS);
            continue;
        }

        if (typeR != typeS || sizeS != sizeR)
        {
            restorer.Shift(sizeS);
            continue;
        }

        restorer.SimpleRead(pMem, sizeS);
    }

    return true;
}

bool RestoreDR(IFlexStateSer& restorer, KFlexDR* pDR, DWORD version)
{
    if (ReadAndTest(restorer, sKeyBlockCount))
    {
        return false;
    }

    DWORD nBlocks = 0;
    if (sizeof(nBlocks) != restorer.SimpleRead(nBlocks))
    {
        return false;
    }

    for (DWORD n = 0; n < nBlocks; ++n)
    {
        EStateKeys key = sKeyNull;
        if (sizeof(key) != restorer.SimpleRead(key))
        {
            return false;
        }

        KFlexBlockBase* pBlock = NULL;
        switch (key)
        {
        case sKeyBlockName:
        {
            char szName[256 * 4] = {};
            restorer.ReadLenStr(szName, sizeof(szName));
            pBlock = pDR->GetBlock(szName);
        }
        break;
        case sKeyBlockID:
        {
            GUID guid;
            restorer.SimpleRead(guid);
            pBlock = pDR->GetBlock(guid);
        }
        break;
        default:
            ASS(0);
            return false;
        }

        char szType[256 * 4] = { 0 };
        restorer.ReadLenStr(szType, sizeof(szType));
        DWORD lenNext = 0;
        restorer.SimpleRead(lenNext);
        if (!pBlock)
        {
            restorer.Shift(lenNext);
            continue;
        }

        if (strcmp(szType, pBlock->m_pszType))
        {
            restorer.Shift(lenNext);
            continue;
        }

        if (!RestoreBlock(restorer, pBlock, version))
        {
            return false;
        }
    }

    return true;
}

int KFlexRsu::RestoreState(IFlexStateSer& restorer, DWORD version)
{
    if (ReadAndTest(restorer, sKeyDRcount))
    {
        return 1;
    }

    DWORD savedCount = 0;
    if (sizeof(savedCount) != restorer.SimpleRead(savedCount))
    {
        return 2;
    }

    for (DWORD n = 0; n < savedCount; ++n)
    {
        if (ReadAndTest(restorer, sKeyDR))
        {
            return 3;
        }

        char szDrName[64 * 4] = {}; // _countof(m_ppDR[0]->m_szFileName);
        restorer.ReadLenStr(szDrName, sizeof(szDrName));
        DWORD lenNext = 0;
        restorer.SimpleRead(lenNext);
        KFlexDR* pDR = NULL;
        for (DWORD d = 0; d < m_nDR; ++d)
        {
            auto e = _strcmpi(m_ppDR[d]->m_szFileName, szDrName);
            if (e)
            {
                continue;
            }

            pDR = m_ppDR[d];
            break;
        }

        if (!pDR)
        {
            restorer.Shift(lenNext);
            continue;
        }

        if (!RestoreDR(restorer, pDR, version))
        {
            ASS(0);
            return 4;
        }
    }

    return 0;
}

