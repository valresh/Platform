#pragma once
#include <CBase.h>
#include <crossplatform.h>


#ifdef HCDMEMORYSERVICE_EXPORTS
#define HCDMEMORYSERVICE_API _EXP
#else
#define HCDMEMORYSERVICE_API _IMP
#pragma comment( lib, "MemoryServiceHCD.lib")
#endif

typedef BYTE*(*tMemAllocatorImpl)( size_t size );
typedef CBase*(*tStructAllocatorImpl)( LPCSTR pszName, DWORD ID, int CS );
typedef class KNoName*(*tGetStructAllocator)();

class HCDMEMORYSERVICE_API KMemoryServiceHCD
{
  tMemAllocatorImpl m_pMemAllocator;
  tStructAllocatorImpl m_pStructAllocator;
  tGetStructAllocator m_pStructGetter;
  tStructAllocatorImpl m_pIoAllocator;

protected:
  KMemoryServiceHCD();
public:
  size_t ObjInMemory;
public:
  static KMemoryServiceHCD& Instance();
  void InitMemAllocator( tMemAllocatorImpl pfn );
  void InitStructAllocator( tStructAllocatorImpl pfn );
  void InitStructGetter( tGetStructAllocator pfn );
  void InitIoAllocator( tStructAllocatorImpl pfn );
public:
  void* NewMem( size_t size );
  CBase* CreateSharedObj( LPCSTR pszName, DWORD ID, int CS );
  CBase* IoCreateObj( LPCSTR pszName, DWORD ID, int CS );
  class KNoName* GetStructs();
};

#define NEWHCD void *operator new( size_t size )   { return KMemoryServiceHCD::Instance().NewMem ( size ); };\
               void *operator new[]( size_t size ) { return KMemoryServiceHCD::Instance().NewMem ( size ); };
