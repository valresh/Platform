#include <MemoryServiceY.h>
#include <rsuErr.h>
#ifdef LINUX
#include "LogFile.h"
#endif


KMemoryServiceY::KMemoryServiceY()
: centumObjInMemory( 0 )
, prosafeObjInMemory( 0 )
, centumSharedDataInNames( 0 )
, centumStaticMemory( 0 )
, m_pCentumMemAllocator( NULL )
, m_pProsafeMemAllocator( NULL )
, m_pObjectsStructAllocator( NULL )
, m_pIOsStructAllocator( NULL )
, m_pCreateRefToIOsStruct( NULL )
, m_pOnObjectStructCreatedCallback( NULL )
, m_pStructCleaner( NULL )
{
}

KMemoryServiceY& KMemoryServiceY::Instance()
{
  static KMemoryServiceY _Instance;
  return _Instance;
}

void KMemoryServiceY::InitCentumMemAllocator( tMemAllocatorImpl pfn )
{
#ifdef LINUX
	CLogFile::Log ("KMemoryServiceY::InitCentumMemAllocator 1: m_pCentumMemAllocator %08lx, pfn %08lx", m_pCentumMemAllocator, pfn);
#endif // LINUX

  if( m_pCentumMemAllocator )
  {
    ASS( m_pCentumMemAllocator==pfn );
  }
  m_pCentumMemAllocator = pfn;
}

bool KMemoryServiceY::IsPrsMemAllocatorHas()
{
  return m_pProsafeMemAllocator ? true : false;
}

void KMemoryServiceY::InitProsafeMemAllocator( tMemAllocatorImpl pfn )
{
#ifdef LINUX
	CLogFile::Log ("KMemoryServiceY::InitProsafeMemAllocator 1: m_pProsafeMemAllocator %08lx, pfn %08lx", m_pProsafeMemAllocator, pfn);
#endif // LINUX

  if( m_pProsafeMemAllocator )
    return;
  m_pProsafeMemAllocator = pfn;
}

void KMemoryServiceY::InitObjectsStructAllocator( tStructAllocatorImpl pfn, tOnObjectStructCreated pfnCallback )
{
#ifdef LINUX
	CLogFile::Log ("KMemoryServiceY::InitObjectsStructAllocator 1: m_pObjectsStructAllocator %08lx, pfn %08lx", m_pObjectsStructAllocator, pfn);
#endif // LINUX

  if( m_pObjectsStructAllocator )
  {
    ASS( m_pObjectsStructAllocator==pfn );
  }
#ifdef LINUX
	CLogFile::Log ("KMemoryServiceY::InitObjectsStructAllocator 2: pfnCallback %08lx, m_pOnObjectStructCreatedCallback %08lx", pfnCallback, m_pOnObjectStructCreatedCallback);
#endif // LINUX

  if( pfnCallback && m_pOnObjectStructCreatedCallback )
  {
    ASS( m_pOnObjectStructCreatedCallback==pfnCallback );
  }
  m_pObjectsStructAllocator = pfn;
  m_pOnObjectStructCreatedCallback = pfnCallback;
}

void KMemoryServiceY::InitIOsStructAllocator( tStructAllocatorImpl pfn )
{
  if( m_pIOsStructAllocator )
  {
    ASS( m_pIOsStructAllocator==pfn );
  }
  m_pIOsStructAllocator = pfn;
}

void KMemoryServiceY::InitCreateRefToIOsStruct( tCreateRefToIOsStruct pfn )
{
  if( m_pCreateRefToIOsStruct )
  {
    ASS( m_pCreateRefToIOsStruct==pfn );
  }
  m_pCreateRefToIOsStruct = pfn;
}

void KMemoryServiceY::InitStructCleaner( tClearStructImpl pfn )
{
  if( m_pStructCleaner )
  {
    ASS( m_pStructCleaner==pfn );
  }
  m_pStructCleaner = pfn;
}

void* KMemoryServiceY::NewMemCentum( size_t size )
{
  ASS( m_pCentumMemAllocator );
  if( !m_pCentumMemAllocator )
    return 0;
  centumObjInMemory += size;
  return m_pCentumMemAllocator( size );
}

void* KMemoryServiceY::NewMemProsafe( size_t size )
{
  ASS( m_pProsafeMemAllocator );
  if( !m_pProsafeMemAllocator)
    return 0;
  prosafeObjInMemory += size;
  return m_pProsafeMemAllocator( size );
}

CBase* KMemoryServiceY::CreateObjectsStruct( LPCSTR ObjName, DWORD TypeID, int number, int domain, bool bCountUsing /*= true*/ )
{
  ASSD( domain < 65 );
  ASSD( number < 65 );
  ASS( m_pObjectsStructAllocator );
  if( !m_pObjectsStructAllocator )
    return NULL;
  CBase *pName = m_pObjectsStructAllocator( ObjName, TypeID, number, domain );
  if( pName && bCountUsing )
    centumSharedDataInNames += pName->size;
  if( m_pOnObjectStructCreatedCallback )
  {
    CBase *pAdd = m_pOnObjectStructCreatedCallback( ObjName, TypeID, number, domain );
    if( pAdd )
      centumSharedDataInNames += pAdd->size;
  }  
  return pName;
}

CBase* KMemoryServiceY::CreateIOsStruct( LPCSTR ObjName, DWORD TypeID, int number, int domain, bool bCountUsing /*= true*/ )
{
  ASSD( domain < 65 );
  ASSD( number < 65 );
  ASS( m_pIOsStructAllocator );
  if( !m_pIOsStructAllocator )
    return NULL;
  CBase *pName = m_pIOsStructAllocator( ObjName, TypeID, number, domain );
  if( pName && bCountUsing )
    centumSharedDataInNames += pName->size;
  /*if( TypeID>=id_AcyMin && TypeID<=id_AcyMax )
  {
    CBase *pBaseReserve = NULL;
    char szName[128];
    sprintf_s( szName, "%s.reserve", ObjName );
    pBaseReserve = m_pObjectsStructAllocator( szName, CValue::TypeID, number );
    if( pBaseReserve && bCountUsing )
      centumSharedDataInNames += pBaseReserve->size;
  }*/
  return pName;
}

bool KMemoryServiceY::ClearStructs()
{
  if( !m_pStructCleaner )
    return false;
  m_pStructCleaner();
  return true;
}

void KMemoryServiceY::AddStaticMemoryCentum( size_t size )
{
  centumStaticMemory += size;
}

void KMemoryServiceY::CreateRefToIOsStruct( LPCSTR ObjName, CBase *pBase )
{
  if( !m_pCreateRefToIOsStruct )
    return;
  m_pCreateRefToIOsStruct( ObjName, pBase );
}
