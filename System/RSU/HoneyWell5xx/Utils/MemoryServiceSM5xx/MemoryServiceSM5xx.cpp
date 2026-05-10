#include <MemoryServiceSM5xx.h>
#include <rsuErr.h>


KMemoryServiceSM5xx::KMemoryServiceSM5xx()
: m_pMemAllocator( NULL )
//, m_pObjectsStructAllocator( NULL )
, m_pIOsStructAllocator( NULL )
//, m_pOnObjectStructCreatedCallback( NULL )
//, m_pObjectsStructFinder( NULL )
{
}

static KMemoryServiceSM5xx *_Instance = nullptr;

KMemoryServiceSM5xx& KMemoryServiceSM5xx::Instance()
{
    if (!_Instance)
        _Instance = new KMemoryServiceSM5xx();
    return *_Instance;
}

void KMemoryServiceSM5xx::InitMemAllocator( tMemAllocatorImpl pfn )
{
  if( m_pMemAllocator )
  {
    ASS( m_pMemAllocator==pfn );
  }
  m_pMemAllocator = pfn;
}

/*void KMemoryServiceSM5xx::InitObjectsStructAllocator( tStructAllocatorImpl pfn, tOnObjectStructCreated pfnCallback, tStructFinder pFinder )
{
  if( m_pObjectsStructAllocator )
  {
    ASS( m_pObjectsStructAllocator==pfn );
  }
  if( pfnCallback && m_pOnObjectStructCreatedCallback )
  {
    ASS( m_pOnObjectStructCreatedCallback==pfnCallback );
  }
  m_pObjectsStructAllocator = pfn;
  m_pOnObjectStructCreatedCallback = pfnCallback;
  m_pObjectsStructFinder = pFinder;
}*/

void KMemoryServiceSM5xx::InitIOsStructAllocator( tIoAllocatorImpl pfn )
{
  if( m_pIOsStructAllocator )
  {
    ASS( m_pIOsStructAllocator==pfn );
  }
  m_pIOsStructAllocator = pfn;
}

void* KMemoryServiceSM5xx::NewMem( size_t size )
{
  ASS( m_pMemAllocator );
  if( !m_pMemAllocator )
    return 0;
  return m_pMemAllocator( size );
}
/*
CBase* KMemoryServiceSM5xx::CreateObjectsStruct( LPCSTR ObjName, DWORD TypeID, int number )
{
  ASS( m_pObjectsStructAllocator );
  if( !m_pObjectsStructAllocator )
    return NULL;
  CBase *pName = m_pObjectsStructAllocator( ObjName, TypeID, number );
  if( m_pOnObjectStructCreatedCallback )
  {
    CBase *pAdd = m_pOnObjectStructCreatedCallback( ObjName, TypeID, number );
  }  
  return pName;
}*/

CBase* KMemoryServiceSM5xx::CreateIOsStruct( LPCSTR pszArmName, LPCSTR pszShortSmName, DWORD TypeID, int number )
{
  ASS( m_pIOsStructAllocator );
  if( !m_pIOsStructAllocator )
    return NULL;
  CBase *pName = m_pIOsStructAllocator( pszArmName, pszShortSmName, TypeID, number );
  return pName;
}
/*
CBase* KMemoryServiceSM5xx::FindObjectsStruct( LPCSTR ObjName, DWORD TypeID, int number )
{
  ASS( m_pObjectsStructFinder );
  if( !m_pObjectsStructFinder )
    return NULL;
  CBase *pF = m_pObjectsStructFinder( ObjName, TypeID, number );
  return pF;
}
*/