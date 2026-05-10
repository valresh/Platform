#include "QB_RemoteClient.h"

KQB_RemoteClient* g_Client = NULL;

bool GetFirstACSObject()
{
  return g_Client->GetFirstACSObject();
}

bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Data[256] )
{
  return g_Client->GetNextACSObject( pData, Name, 256, Data, 256 );
}

#ifdef _WIN32
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  return g_Client->ShowACSObject( hMainWnd, Name, pData );
}
#endif

bool KQB_RemoteClient::GetFirstACSObject()
{
  m_nAcsFindStep = 0;
  return true;
}

bool KQB_RemoteClient::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  if( -1==m_nAcsFindStep )
    return false;
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;
  if( m_IOs->WhileBase( m_nAcsFindStep, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    LPCSTR pszClass = NULL;
    m_IOs->GetNameEx( pBase->ID_PNT_NAME, &pszClass, NULL, NULL );
    strcpy_s( Name, sn, pszName );
    strcpy_s( Data, sd, pszClass );
    return true;
  }
  return false;
}

#ifdef _WIN32
bool KQB_RemoteClient::ShowACSObject( HWND hMainWnd, char * Name, void * pData )
{
  if( !strcmp(m_ObjName.c_str(), Name))
  {
    m_proxy.ShowObject( Name );
    return true;
  }
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;
  DWORD ID = 0;
  while( m_IOs->WhileBase( ID, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    if( !strcmp(pszName, Name) )
    {
      m_proxy.ShowObject( Name );
      return true;
    }
  }
  return false;
}
#endif