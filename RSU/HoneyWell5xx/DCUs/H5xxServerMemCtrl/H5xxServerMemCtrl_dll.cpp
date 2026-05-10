#include <crossplatform.h>
#include <rsuMemoryManager.h>
#include <MemoryServiceH5xx.h>
#include <rsuErr.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif


static KRsuMemoryManager *s_memoryHW = nullptr;

static void* _ObjsMemAllocatorHW( size_t size )
{
  void* p = s_memoryHW->AllocMemory( (DWORD)size );
  ASS( p );
  return p;
}

#ifndef _WIN32
void OnLoadLibraryH5SMC() __attribute__((constructor));
#else
void OnLoadLibraryH5SMC();
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
      OnLoadLibraryH5SMC();
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
#endif
void OnLoadLibraryH5SMC()
{
    if (!s_memoryHW)
        s_memoryHW = new KRsuMemoryManager();
    s_memoryHW->Create("H5xx_Mem", 128);
    KMemoryServiceH5xx::Instance().InitMemAllocator(_ObjsMemAllocatorHW);
}
#ifdef _MANAGED
#pragma managed(pop)
#endif
