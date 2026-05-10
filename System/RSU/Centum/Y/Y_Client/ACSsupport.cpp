#include "rsuNoNames2D.h"
#include "Y_RemoteClient.h"

extern KY_RemoteClient * g_pYClient;

bool GetFirstACSObject()
{
  if( !g_pYClient )
    return false;
  return g_pYClient->GetFirstACSObject();
}

bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Info[256] )
{
  if( !g_pYClient )
    return false;
  return g_pYClient->GetNextACSObject( pData, Name, 256, Info, 256 );
}

bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  if( !g_pYClient )
    return false;
  return g_pYClient->ShowACSObject( hMainWnd, Name, pData );
}

bool KY_RemoteClient::GetFirstACSObject()
{
  m_nAcsFindStep = 0;
  return true;
}

bool KY_RemoteClient::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;
  int nDomain = 0;
  if( -1==m_nAcsFindStep )
    return false;
  if( !(m_nAcsFindStep & 0x80000000) )
  {
    if( m_yokoObjs->WhileBase( m_nAcsFindStep, -1, &pBase, &pszName, &nFcsNumb, &nDomain ) )
    {
      LPCSTR pszClass = NULL;
      m_yokoObjs->GetNameEx( pBase->ID_PNT_NAME, &pszClass, NULL, NULL );
      sprintf_s( Name, sn, "%s", pszName );
      sprintf_s( Data, sd, "%s (FCS  %02d)", pszClass, nFcsNumb );
      if( pData )
        *pData = (void*)nFcsNumb;
      return true;
    }
    m_nAcsFindStep = 0x80000000;
  }

  m_nAcsFindStep &= ~0x80000000;

  if( m_yokoIOs->WhileBase( m_nAcsFindStep, -1, &pBase, &pszName, &nFcsNumb, &nDomain ) )
  {
    LPCSTR pszClass = NULL;
    m_yokoIOs->GetNameEx( pBase->ID_PNT_NAME, &pszClass, NULL, NULL );
    sprintf_s( Name, sn, "%s", pszName );
    sprintf_s( Data, sd, "%s (FCS  %02d)", pszClass, nFcsNumb );
    if( pData )
      *pData = (void*)nFcsNumb;
    m_nAcsFindStep |= 0x80000000;
  }
  else
  {
    m_nAcsFindStep = -1;
    strcpy_s( Name, sn, (LPCSTR)ObjName );
    strcpy_s( Data, sd, "Yokogawa" );
  }
  return true;
}

#ifdef _WIN32
bool KY_RemoteClient::ShowACSObject( HWND hMainWnd, char * Name, void * pData )
{
  m_proxy.ShowObject(Name);
  return true;
}
#endif