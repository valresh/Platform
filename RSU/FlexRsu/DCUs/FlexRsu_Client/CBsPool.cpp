#include "CBsPool.h"
#include <rsuErr.h>
#include <basemodel.h>

KCBsPool::KCBsPool()
: m_Count(0)
, m_ActiveI( 0 )
{
  memset( m_Ctrls, 0, sizeof(m_Ctrls) );
}

void KCBsPool::RegisterCB( ICBsPool *pCB )
{
  ASS( m_Count < countof(m_Ctrls) );
  if( m_Count == countof(m_Ctrls) )
    return;
  m_Ctrls[m_Count].pCB = pCB;
  m_Ctrls[m_Count].pName = pCB->GetName();
  ++m_Count;
}

#ifdef _WIN32
bool KCBsPool::GetFirstACSObject()
{
  m_ActiveI = 0;
  m_nAcsFindObjStep = 0;
  m_nAcsFindIOStep = 0;
  if( !m_Count )
    return false;
  return m_Ctrls[m_ActiveI].pCB->GetFirstACSObject();
}

bool KCBsPool::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;

  if( !m_Count )
    return false;

  if( !m_nAcsFindObjStep )
  {
    bool r = m_Ctrls[m_ActiveI].pCB->GetNextACSObject( pData, Name, sn, Data, sd );
    if( !r )
    {
      while( 1 )
      {
        ++m_ActiveI;
        if( m_ActiveI>=m_Count )
          break;
        r = m_Ctrls[m_ActiveI].pCB->GetFirstACSObject();
        if( !r )
          continue;
        r = m_Ctrls[m_ActiveI].pCB->GetNextACSObject( pData, Name, sn, Data, sd );
        if( !r )
          continue;
        break;
      }
    }
    if( r )
      return true;
  }

  return false;
}

bool KCBsPool::ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  for( int i=0; i<m_Count; ++i )
  {
    m_Ctrls[i].pCB->ShowACSObject( hMainWnd, Name, pInfo );
  }
  return true;
}
#endif