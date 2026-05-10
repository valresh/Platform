#include "ListValue.h"
#include "crossruntime.h"
#include "crosssemaphore.h"
#include <assert.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

std::unique_ptr<cross::counting_semaphore> GiveSem(const char* name)
{
  char szSem[64];
  sprintf_s( szSem, sizeof(szSem), "HCD_Sem_%s", name );
  return std::unique_ptr<cross::counting_semaphore>(new cross::counting_semaphore(szSem, 1, 1));
}

void InitALL()
{
    CStr::Reset(NULL);
}
//
std::unique_ptr<cross::counting_semaphore> hSemValue;

#ifndef _WIN32
void OnLoadLibraryUtilsHCD() __attribute__((constructor));
void OnUnloadLibraryUtilsHCD() __attribute__((destructor));
#else
void OnLoadLibraryUtilsHCD();
void OnUnloadLibraryUtilsHCD();
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					            )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        OnLoadLibraryUtilsHCD();
    }
    break;
    case DLL_PROCESS_DETACH:
    {
        OnUnloadLibraryUtilsHCD();
    }
    break;
    };
    return TRUE;
}
#endif

void OnLoadLibraryUtilsHCD()
{
    hSemValue = GiveSem("Value");

    auto moduleFilePath = cross::runtime::module_file_path((void*)&InitALL);
    assert(!moduleFilePath.empty());
    theList.Attach(moduleFilePath);
    hSemValue = nullptr;
}

void OnUnloadLibraryUtilsHCD()
{
    theList.Detach();
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

