#include <crossplatform.h>
#include "FlexRsu.h"
#include <rsuNoNames.h>
#include <DataTypes_.h>
#include <time.h>
#include <FlexRsuConnect.h>
#include <rsuStr.h>

extern KNoName* g_SharedMemory;
extern CFlexSystem *g_pFlexSystem;

#ifdef _WIN32
SPrjCfg::tSkipSimIO_ KFlexRsu::s_bSkipSimIO = false;
#endif

KFlexRsu::KFlexRsu( LPCSTR ObjName )
: m_ppDR( NULL )
, m_nDR( 0 )
, m_nAcsFindStep( 0 )
, m_nStep( 0 )
, m_nInitCounter( 0 )
, m_PageConnectors( NULL )
{
  strcpy( m_szObjName, ObjName );
  m_szDirPrj[0] = 0;
  InitFlexSystem();
}

void KFlexRsu::InitFlexSystem()
{
  g_pFlexSystem = NULL;
  m_pFlexSystem = (CFlexSystem*)g_SharedMemory->CreateStruct("Весь Flex", CFlexSystem::TypeID, sizeof(CFlexSystem), 0);
  if( m_pFlexSystem )
  {
    m_pFlexSystem->StartStatus = CFlexSystemA::STATUS_INIT;
    g_pFlexSystem = m_pFlexSystem;
  }
  ++m_nInitCounter;
}

int KFlexRsu::StepBeforRestore()
{
  return 0;
}

int KFlexRsu::StepAfterRestore()
{
  m_nStep = 0;
  if( m_pFlexSystem )
  {
    m_pFlexSystem->startTime = time(NULL);
    m_pFlexSystem->dModelT = 0;
    m_pFlexSystem->bSound = false;
    m_pFlexSystem->nFlexAlarmXY = 0;
    m_pFlexSystem->nFlexAlarms0 = 0;
    m_pFlexSystem->nFlexAlarms1 = 0;
    m_pFlexSystem->nMsg2Operator = 0;
  }

  for( DWORD n=0; n<m_nDR; ++n )
    m_ppDR[n]->OnAfterRestoreState();

  return 0;
}

LPCSTR KFlexRsu::GetProjectDir()
{
  return m_szDirPrj;
}

KFlexBlockBase* KFlexRsu::FindBlock( LPCSTR pszName )
{
  for( DWORD n=0; n<m_nDR; ++n )
  {
    KFlexDR *pDR = m_ppDR[n];
    KFlexBlockBase *pBlock = pDR->GetBlock( pszName );
    if( pBlock )
      return pBlock;
  }
  return NULL;
}


void *SLocalPageConnector::operator new( size_t size )
{
  return pFlexMemoryAllocator( (DWORD)size ); 
}

SPageConnector* KFlexRsu::RegisterPageConnector( LPCSTR pszName )
{
  if( !m_PageConnectors )
  {
    m_PageConnectors = new SLocalPageConnector;
    m_PageConnectors->name = pszName;
    return m_PageConnectors;
  }
  SLocalPageConnector *p = m_PageConnectors, *pLast = NULL;
  for( ; p; p = p->pNext )
  {
    pLast = p;
    if( _Kstricmp( (LPCSTR)p->name, pszName) )
      continue;
    return p;
  }
  p = pLast->pNext = new SLocalPageConnector;
  p->name = pszName;
  return p;
}
