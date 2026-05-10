#include <rsuNoNames.h>
#include <TrendsSup.h>
#include <SharedFlexBlocks.h>
#include <macros/StrHelps.h>
#include <FlexRsuCommon.h>
#include <stdlib.h>

#ifdef RSU_SHARED_TREND_EXPORT
#define API extern "C" _EXP
#else
#define API extern "C" _IMP
#endif

KNoName* g_SharedMemory = nullptr;

extern "C" API bool EnumObjs(DWORD& ID, const DWORD ClassID, struct CBase** base, LPCSTR* pszName, int* pFcsNumber)
{
    return false;
}

extern "C" API bool EnumIOs(DWORD& ID, const DWORD ClassID, struct CBase** base, LPCSTR* pszName, int* pFcsNumber)
{
    return g_SharedMemory->WhileBase(ID, ClassID, base, pszName, pFcsNumber);
}

extern "C" API int TrendInfoPoint(DWORD type, LPCSTR pObjName, CBase* pBase, int fcsNumber, int cInfs, STrendPointInfo* pInfs)
{
    DWORD N = pBase->ID_CLASS;
    if (N != SFlex::TypeID)
    {
        return 0;
    }

    int c = 0;
    SFlex* pB = (SFlex*)pBase;
    LPCSTR ppFields[] = { "PV", "SP", "OP" };
    for (int i = 0; i < _countof(ppFields); ++i)
    {
        LPCSTR p = ppFields[i];
        if (pB->TypeFlex == S_ANATREND::TypeID)
        {
            p = NULL;
        }

        SVarInfo* pV = GetFieldInfo(pB->TypeFlex, p ? p : "PV");
        if (!pV)
        {
            continue;
        }

        STrendPointInfo& obj = pInfs[c];
        obj.pAddr = pB->ObjectMem;
        obj.pAddr += pV->shift;
        obj.dwID = 0x80000000;
        if (p)
        {
            sprintf(obj.szName, "%s.%s", pObjName, p);
        }
        else
        {
            strcpy_s(obj.szName, pObjName);
        }

        obj.pszSufName = p;
        switch (pV->evtType)
        {
        case evtDigital32:
        {
            obj.eValType = enumValueInt;
            break;
        }
        case evtAnalog32:
        {
            obj.eValType = enumValueFlt;
            break;
        }
        case evtChars:
        {
            return c;
        }
        }

        ++c;
        if (!p)
        {
            break;
        }

        if (c >= cInfs)
        {
            break;
        }
    }

    return c;
}

// Instead Of DllMain()/__attribute__((constructor))
static struct Library
{
    Library()
    {
        char szName[64 * 4];
        ns_FlexRsu::GetSharedName(szName);
        g_SharedMemory = KNoName::Create(szName, false);
    }

    ~Library()
    {
    }
} s_library;
