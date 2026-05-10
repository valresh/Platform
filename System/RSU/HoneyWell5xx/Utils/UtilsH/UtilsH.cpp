#include "ListValue.h"
#include <ConnectH.h>
#include "crossruntime.h"
#include "crosssemaphore.h"
#include <assert.h>

std::unique_ptr<cross::counting_semaphore> GiveSem( const char* name )
{
  char szSem[64];
  sprintf_s( szSem, sizeof(szSem), "HONEYWELL_CB_Sem_%s", name );
  return std::unique_ptr<cross::counting_semaphore>(new cross::counting_semaphore(szSem, 1, 1));
}

void InitALL()
{
    CStr::Reset(NULL);
}
//
std::unique_ptr<cross::counting_semaphore> hSemValue;

#ifndef _WIN32
void OnLoadLibraryUtilsH() __attribute__((constructor));
void OnUnloadLibraryUtilsH() __attribute__((destructor));
#else
void OnLoadLibraryUtilsH();
void OnUnloadLibraryUtilsH();
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  reason, LPVOID lpReserved )
{
  switch ( reason )
  {
  case DLL_PROCESS_ATTACH:
    {
      OnLoadLibraryUtilsH();
    }
    break;
  case DLL_PROCESS_DETACH:
    {
      OnUnloadLibraryUtilsH();
    }
    break;
  };
  return TRUE;
}
#endif

void OnLoadLibraryUtilsH()
{
    hSemValue = GiveSem("Value");

    auto moduleFilePath = cross::runtime::module_file_path((void*)&InitALL);
    assert(!moduleFilePath.empty());
    theList.Attach(moduleFilePath);
    hSemValue = nullptr;
}

void OnUnloadLibraryUtilsH()
{
    theList.Detach();
}
