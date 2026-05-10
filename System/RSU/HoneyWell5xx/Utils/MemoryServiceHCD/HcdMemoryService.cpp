#include <crossplatform.h>
#include <HcdMemoryService.h>
#include <rsuErr.h>
#include <rsuNoNames.h>
#include <crosslock.h>

static std::recursive_mutex s_csNamesAccess;
static std::recursive_mutex s_csMemAccess;

KMemoryServiceHCD::KMemoryServiceHCD()
: m_pMemAllocator( NULL )
, m_pStructAllocator( NULL )
, m_pStructGetter( NULL )
, m_pIoAllocator( NULL )
{
}

static KMemoryServiceHCD *_Instance = nullptr;


KMemoryServiceHCD& KMemoryServiceHCD::Instance()
{
    if (!_Instance)
        _Instance = new KMemoryServiceHCD();
    return *_Instance;
}

void KMemoryServiceHCD::InitMemAllocator( tMemAllocatorImpl pfn )
{
  if( m_pMemAllocator )
  {
    ASS( m_pMemAllocator==pfn );
    return;
  }
  m_pMemAllocator = pfn;
}

void KMemoryServiceHCD::InitStructAllocator( tStructAllocatorImpl pfn )
{
  if( m_pStructAllocator )
    return;
  m_pStructAllocator = pfn;
}

void KMemoryServiceHCD::InitStructGetter( tGetStructAllocator pfn )
{
  if( m_pStructGetter )
    return;
  m_pStructGetter = pfn;
}

void KMemoryServiceHCD::InitIoAllocator( tStructAllocatorImpl pfn )
{
  if( m_pIoAllocator )
    return;
  m_pIoAllocator = pfn;
}
//////////////////////////////////////////////////////////////////////////
void* KMemoryServiceHCD::NewMem( size_t size )
{
  if( !size )
    return NULL;
  LOCK(s_csMemAccess);
  //KCsSync _sync( s_csMemAccess );
  if( !m_pMemAllocator )
  {
    return new BYTE[size];
  }
  ObjInMemory += size;
  return m_pMemAllocator( size );
}

CBase* KMemoryServiceHCD::CreateSharedObj( LPCSTR pszName, DWORD ID, int CS )
{
  ASSD( pszName && *pszName );
  LOCK( s_csNamesAccess );

  ASS( m_pStructAllocator );
  if( !m_pStructAllocator )
    return NULL;

  CBase *pName = m_pStructAllocator( pszName, ID, CS );
  return pName;
}

CBase* KMemoryServiceHCD::IoCreateObj( LPCSTR pszName, DWORD ID, int CS )
{
  ASSD( pszName && *pszName );
  LOCK( s_csNamesAccess );

  ASS( m_pIoAllocator );
  if( !m_pIoAllocator )
    return NULL;

  CBase *pName = m_pIoAllocator( pszName, ID, CS );
  return pName;
}

KNoName* KMemoryServiceHCD::GetStructs()
{
  ASSD( m_pStructGetter );
  if( !m_pStructGetter )
    return NULL;
  return m_pStructGetter();
}
