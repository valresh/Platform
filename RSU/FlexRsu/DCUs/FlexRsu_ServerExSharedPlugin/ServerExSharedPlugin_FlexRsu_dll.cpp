#include <crossplatform.h>
#include "ServerExSharedPlugin_FlexRsu.h"
#include <rsuNoNames.h>
#include <DataTypes_.h>
#include <RsuX.h>
#include <FlexRsuCommon.h>

KNoName* g_SharedMemory = nullptr;

nRSUx::SParamInfo RsuConnectParamInfoTypesImpl( LPCSTR pszPointName, DWORD TypeID );

// Instead Of DllMain()/__attribute__((constructor))
static struct Library
{
    Library()
    {
        KNoName::RegisterClassInNames(SFlex::TypeID, "FlexBlock");
        KNoName::RegisterClassInNames(CFlexSystem::TypeID, "FlexSystem");
        KNoName::RegisterClassInNames(SFlexFutureTrend::TypeID, "FlexFutureTrend");
        {
            char szName[64];
            ns_FlexRsu::GetSharedName(szName);
            g_SharedMemory = KNoName::Create(szName, false);
        }

        if (pRegisterRsuConnection)
            pRegisterRsuConnection(NULL, NULL, RsuConnectParamInfoTypesImpl);
    }

    ~Library()
    {
    }
} s_library;
