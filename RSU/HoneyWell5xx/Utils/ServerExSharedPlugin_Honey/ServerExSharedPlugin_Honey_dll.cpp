#include "ServerExSharedPlugin_Honey.h"
#include <ConnectH.h>
#include <ConnectSM.h>
#include <ConnectQB.h>
#include <rsuNoNames.h>
#include <MemoryServiceH5xx.h>
#include <dylib.hpp>
#include "LocalUtils.h"

KNoName *g_Objs, *g_IOs, *g_IOsObjsInfo, *g_smIOs, *g_qbIOs;

static CBase* _StructFinder( LPCSTR ObjName, DWORD TypeID, int number )
{
  return g_Objs->FindStruct( ObjName, TypeID, number );
}

static void OnLoadLibraryServExSharedPlugHon5xx()
{
    if (!pRegisterHWstructs)  
        auto lib = new dylib("HW5xxRegisterInRSU");
    if (pRegisterHWstructs)
        pRegisterHWstructs();


    if (!pRegisterSMstructs)
        auto lib = new dylib("SM5xxRegisterInRSU");
    if (pRegisterSMstructs)
        pRegisterSMstructs();

    if (!pRegisterQBstructs)
        auto lib = new dylib("QB5xxRegisterInRSU");
    if (pRegisterQBstructs)
        pRegisterQBstructs();

    g_Objs = KNoName::Create("H5xx_Objs", false);
    g_IOs = KNoName::Create("H5xx_IOs", false);
    g_IOsObjsInfo = KNoName::Create("H5xx_Info", false);

    KMemoryServiceH5xx::Instance().InitObjectsStructAllocator(NULL, NULL, _StructFinder);

    g_smIOs = KNoName::Create("SM5xx_IOs", false);
    g_qbIOs = KNoName::Create("QB5xx_IOs", false);

    // vladexl: Fix init order
    InitiaizeLocalUtils();
}

// Instead Of DllMain()/__attribute__((constructor))
static struct Library
{
    Library()
    {
        OnLoadLibraryServExSharedPlugHon5xx();
    }

    ~Library()
    {
    }
} s_library;
