#include "FlexPool.h"

#include <rsuErr.h>
#include <stdlib.h>

#include <rsuStr.h>

using namespace nRSUx;

KFlexPool::KFlexPool()
: m_Count( 0 )
, m_ActiveI( 0 )
{
  memset( m_Flexes, 0, sizeof(m_Flexes) );
}

int KFlexPool::GetCount()
{
  return m_Count;
}

int KFlexPool::Register( IFlexRsuModel *pStn, LPCSTR pszPrjName, LPCSTR pszObjName )
{
  ASS( m_Count < countof(m_Flexes) );
  if( m_Count == countof(m_Flexes) )
    return -1;
  m_Flexes[m_Count].pFlex = pStn;
  m_Flexes[m_Count].pPrjName = pszPrjName;
  m_Flexes[m_Count].pszObjName = pszObjName;
  ++m_Count;
  return m_Count-1;
}

SParamInfo KFlexPool::GetObj( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;
  for( int i=0; i<m_Count; ++i )
  {
    if(_Kstricmp( m_Flexes[i].pPrjName, pSuffix) )
      continue;
    eVarType Type = evtHZ;
    tVarSizeType_ VarSize = 0;
    BYTE *pMem = m_Flexes[i].pFlex->GetVar( (LPSTR)pszPointName, &Type, &VarSize );
    if( pMem )
    {
      switch( Type )
      {
      case evtDigital32:
        info << (tDigital32*)pMem;
        break;
      case evtAnalog32:
        info << (tAnalog32*)pMem;
        break;
      default:
        ASSD(0);
      }
      break;
    }
  }
  return info;
}

BYTE* KFlexPool::GetVar( LPCSTR pszPrjName, LPSTR pszPointName, eVarType *pType /*= NULL*/, tVarSizeType_ *pVarSize /*= NULL*/ )
{
  for( int i=0; i<m_Count; ++i )
  {
    if(_Kstricmp( m_Flexes[i].pPrjName, pszPrjName) )
      continue;
    BYTE *pMem = m_Flexes[i].pFlex->GetVar( pszPointName, pType, pVarSize );
    return pMem;
  }
  return NULL;
}

#ifdef _WIN32
bool KFlexPool::GetFirstACSObject()
{
  m_ActiveI = 0;
  if( !m_Count )
    return false;
  return m_Flexes[m_ActiveI].pFlex->GetFirstACSObject();
}

bool KFlexPool::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  if( !m_Count )
    return false;
  bool r = m_Flexes[m_ActiveI].pFlex->GetNextACSObject( pData, Name, sn, Data, sd );
  if( !r )
  {
    while( 1 )
    {
      ++m_ActiveI;
      if( m_ActiveI>=m_Count )
        return false;
      r = m_Flexes[m_ActiveI].pFlex->GetFirstACSObject();
      if( !r )
        continue;
      r = m_Flexes[m_ActiveI].pFlex->GetNextACSObject( pData, Name, sn, Data, sd );
      if( !r )
        continue;
      break;
    }
  }
  return r;
}

bool KFlexPool::ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  for( int i=0; i<m_Count; ++i )
  {
    if(_Kstricmp(Name, m_Flexes[i].pPrjName) )
    {
      if(_Kstricmp(Name, m_Flexes[i].pszObjName) )
        continue;
    }
    bool r = m_Flexes[i].pFlex->ShowACSObject( pInfo, hMainWnd, m_Flexes[i].pPrjName, pData );
    if( r )
      return r;
  }
  return false;
}
#endif