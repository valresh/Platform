#include <rsuErr.h>
#include "CBPool.h"

KControlBuilder *g_pCur = NULL;


KCBPool::KCBPool()
: m_Count(0)
, m_ActiveI( 0 )
{
  ZeroMemory( m_Ctrls, sizeof(m_Ctrls) );
}

int KCBPool::GetCtrlCount()
{
  return m_Count;
}

void KCBPool::RegisterCB( KControlBuilder *pcb )
{
  ASS( m_Count<_countof(m_Ctrls) );
  if( m_Count==_countof(m_Ctrls) )
    return;
  m_Ctrls[m_Count].pCB = pcb;
  m_Ctrls[m_Count].pName = pcb->m_szObjName;
  g_pCur = pcb;
  ++m_Count;
}

bool KCBPool::GetFirstACSObject()
{
  m_ActiveI = 0;
  if( !m_Count )
    return false;
  return m_Ctrls[m_ActiveI].pCB->GetFirstACSObject();
}

bool KCBPool::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  if( !m_Count )
    return false;
  bool r = m_Ctrls[m_ActiveI].pCB->GetNextACSObject( pData, Name, sn, Data, sd );
  if( !r )
  {
    while( 1 )
    {
      ++m_ActiveI;
      if( m_ActiveI>=m_Count )
        return false;
      r = m_Ctrls[m_ActiveI].pCB->GetFirstACSObject();
      if( !r )
        continue;
      r = m_Ctrls[m_ActiveI].pCB->GetNextACSObject( pData, Name, sn, Data, sd );
      if( !r )
        continue;
      break;
    }
  }
  return r;
}

#ifdef  _WIN32
bool KCBPool::ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  for( int i=0; i<m_Count; ++i )
  {
    bool r = m_Ctrls[i].pCB->ShowACSObject( hMainWnd, Name, pInfo, false );
    if( r )
      return r;
  }
  return false;
}
#endif

bool KCBPool::GetVar( LPCSTR pSuffix, LPCSTR pBlock, LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize /*= NULL*/, LPCSTR *ppszEnum /*= NULL*/ )
{
  for( int i=0; i<m_Count; ++i )
  {
    if( pSuffix && _strcmpi( m_Ctrls[i].pName, pSuffix) )
      continue;
    bool r = m_Ctrls[i].pCB->GetVar( pBlock, pField, ppVar, pType, pVarSize, ppszEnum );
    if( pSuffix || r )
      return r;
  }
  return false;
}
