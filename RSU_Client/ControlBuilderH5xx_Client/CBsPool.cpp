#include "stdafx.h"
#include "CBsPool.h"
#include <Err.h>
#include <rsuNoNames.h>
#include <BaseModel.h>

KNoName g_Objs;
extern KNoName g_IOs;

KCBsPool::KCBsPool()
: m_Count(0)
, m_ActiveI( 0 )
{
  ZeroMemory( m_Ctrls, sizeof(m_Ctrls) );
}

void KCBsPool::RegisterCB( ICBsPool *pCB )
{
  ASS( m_Count<_countof(m_Ctrls) );
  if( m_Count==_countof(m_Ctrls) )
    return;
  m_Ctrls[m_Count].pCB = pCB;
  m_Ctrls[m_Count].pName = pCB->GetName();
  ++m_Count;
}

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

  if( g_Objs.WhileBase( m_nAcsFindObjStep, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    LPCSTR pszClass = NULL;
    g_Objs.GetNameEx( pBase->ID_PNT_NAME, &pszClass, NULL, NULL );
    strcpy_s( Name, sn, pszName );
    strcpy_s( Data, sd, pszClass );
    return true;
  }

  if( g_IOs.WhileBase( m_nAcsFindIOStep, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    LPCSTR pszClass = NULL;
    g_IOs.GetNameEx( pBase->ID_PNT_NAME, &pszClass, NULL, NULL );
    strcpy_s( Name, sn, pszName );
    strcpy_s( Data, sd, pszClass );
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
