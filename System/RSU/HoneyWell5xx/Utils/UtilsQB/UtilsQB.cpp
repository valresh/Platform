#include "ListValue.h"
#include "crossruntime.h"
#include <crosssemaphore.h>
#include <assert.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

std::unique_ptr<cross::counting_semaphore> GiveSem(const char* name)
{
    char szSem[64];
    sprintf_s(szSem, sizeof(szSem), "HONEYWELL_QB_Sem_%s", name);
    return std::unique_ptr<cross::counting_semaphore>(new cross::counting_semaphore(szSem, 1, 1));
}

void InitALL()
{
    CStr::Reset(NULL);
}

//
std::unique_ptr<cross::counting_semaphore> hSemValue;

#ifndef _WIN32
void OnLoadLibraryUtilsQB() __attribute__((constructor));
void OnUnloadLibraryUtilsQB() __attribute__((destructor));
#else
void OnLoadLibraryUtilsQB();
void OnUnloadLibraryUtilsQB();
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					            )
{
  switch ( ul_reason_for_call )
  {
  case DLL_PROCESS_ATTACH:
    {
      OnLoadLibraryUtilsQB();
    }
    break;
  case DLL_PROCESS_DETACH:
    {
      OnUnloadLibraryUtilsQB();
    }
    break;
  };
  return TRUE;
}
#endif

void OnLoadLibraryUtilsQB()
{
    hSemValue = GiveSem("Value");

    auto moduleFilePath = cross::runtime::module_file_path((void*)&InitALL);
    assert(!moduleFilePath.empty());
    theList.Attach(moduleFilePath);
    hSemValue = nullptr;
}

void OnUnloadLibraryUtilsQB()
{
    theList.Detach();
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

