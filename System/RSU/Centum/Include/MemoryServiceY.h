#pragma once
#include <crossplatform.h>
#include <BaseType.h>

#ifdef MEMORYSERVICEY_EXPORTS
#define MEMORYSERVICEY_API _EXP
#else
#define MEMORYSERVICEY_API _IMP
#endif

typedef void*(*tMemAllocatorImpl)( size_t size );
typedef CBase*(*tStructAllocatorImpl)( LPCSTR ObjName, DWORD TypeID, int number, int domain );
typedef CBase*(*tOnObjectStructCreated)( LPCSTR ObjName, DWORD TypeID, int number, int domain );
typedef int (*tCreateRefToIOsStruct)( LPCSTR ObjName, CBase *pBase );
typedef void(*tClearStructImpl)();

class MEMORYSERVICEY_API KMemoryServiceY
{
  tMemAllocatorImpl m_pCentumMemAllocator;
  tMemAllocatorImpl m_pProsafeMemAllocator;
  tStructAllocatorImpl m_pObjectsStructAllocator;
  tStructAllocatorImpl m_pIOsStructAllocator;

  tClearStructImpl m_pStructCleaner;

  tCreateRefToIOsStruct m_pCreateRefToIOsStruct;

  tOnObjectStructCreated m_pOnObjectStructCreatedCallback;
protected:
  KMemoryServiceY();
public:
  size_t centumObjInMemory;
  size_t prosafeObjInMemory;
  size_t centumSharedDataInNames;
  size_t centumStaticMemory;
public:
  static KMemoryServiceY& Instance();
  void InitCentumMemAllocator( tMemAllocatorImpl pfn );
  void InitProsafeMemAllocator( tMemAllocatorImpl pfn );
  bool IsPrsMemAllocatorHas();
  void InitObjectsStructAllocator( tStructAllocatorImpl pfn, tOnObjectStructCreated pfnCallback );
  void InitIOsStructAllocator( tStructAllocatorImpl pfn );
  void InitCreateRefToIOsStruct( tCreateRefToIOsStruct pfn );
  void InitStructCleaner( tClearStructImpl pfn );
  void* NewMemCentum( size_t size );
  void* NewMemProsafe( size_t size );
  CBase* CreateIOsStruct( LPCSTR ObjName, DWORD TypeID, int number, int domain, bool bCountUsing = true );
  CBase* CreateObjectsStruct( LPCSTR ObjName, DWORD TypeID, int number, int domain, bool bCountUsing = true );
  bool ClearStructs();
  void AddStaticMemoryCentum( size_t size );
  void CreateRefToIOsStruct( LPCSTR ObjName, CBase *pBase );
};

#define NEWY  void *operator new( size_t size ) { return KMemoryServiceY::Instance().NewMemCentum ( size ); };\
              void *operator new[]( size_t size ) { return KMemoryServiceY::Instance().NewMemCentum ( size ); };

#define NEWPRS  void *operator new( size_t size ) { return KMemoryServiceY::Instance().NewMemProsafe ( size ); };\
                void *operator new[]( size_t size ) { return KMemoryServiceY::Instance().NewMemProsafe ( size ); };
