#include <crossplatform.h>
#include "FlexRsuImpl_dll.h"
#include <rsuNoNames.h>
#include <rsuMemoryManager.h>
#include <FlexRsuConnect.h>
#include <rsuErr.h>
#include <FlexRsuCommon.h>
#include <DataTypes_.h>

static KRsuMemoryManager* s_FLEXmemory = nullptr;
KNoName* g_SharedMemory = nullptr;
CFlexSystem* g_pFlexSystem = NULL;

static BYTE* _FlexMemAllocator(DWORD size)
{
    BYTE* p = s_FLEXmemory->AllocMemory(size);
    //ASS(p);
    return p;
}

static void _FlexMemoryResetter()
{
    s_FLEXmemory->Clear();
}

SFlex* CreateFlexSharedObj(LPCSTR pszName, DWORD FlexID, DWORD size, void** pStruct)
{
    SFlex* pFlex = (SFlex*)g_SharedMemory->CreateStruct(pszName, SFlex::TypeID, size + sizeof(SFlex), 0);
    pFlex->ObjSize = size;
    pFlex->TypeFlex = FlexID;
    *pStruct = pFlex->ObjectMem;
    return pFlex;
}

CBase* CreateAnySharedObj(LPCSTR pszName, DWORD ID, DWORD size, void** pStruct)
{
    CBase* pBase = g_SharedMemory->CreateStruct(pszName, ID, size, 0);
    *pStruct = pBase;
    return pBase;
}

// Instead Of DllMain()/__attribute__((constructor))
static struct Library
{
    Library()
    {
        s_FLEXmemory = new KRsuMemoryManager();
        char szName[64 * 4];
        ns_FlexRsu::GetSharedName(szName);
#ifdef _WIN64
        g_SharedMemory = KNoName::Create(szName, true, 30000, 300000, 5500000);
#elif _WIN32
        g_SharedMemory = KNoName::Create(szName, true, 30000, 300000, 300000);
#else
        g_SharedMemory = KNoName::Create(szName, true, 30000, 300000, 2000000);
#endif
        g_SharedMemory->Reset();
        s_FLEXmemory->Create("FlexRsu_Objs", 10, 10);
        pFlexMemoryAllocator = _FlexMemAllocator;
        pFlexMemoryResetter = _FlexMemoryResetter;
        pSFlexSharedObjCreator = CreateFlexSharedObj;
        pAnySharedObjCreator = CreateAnySharedObj;
        KNoName::RegisterClassInNames(SFlex::TypeID, "FlexBlock");
        KNoName::RegisterClassInNames(CFlexSystem::TypeID, "FlexSystem");
        KNoName::RegisterClassInNames(SFlexFutureTrend::TypeID, "FlexFutureTrend");

    }

    ~Library()
    {
        if (s_FLEXmemory)
        {
            delete s_FLEXmemory;
        }

        if (g_SharedMemory)
        {
            delete g_SharedMemory;
            g_SharedMemory = nullptr;
        }

        if (g_pFlexSystem)
        {
            g_pFlexSystem->StartStatus = CFlexSystemA::STATUS_UNKNOWN;
        }
    }
} s_library;
