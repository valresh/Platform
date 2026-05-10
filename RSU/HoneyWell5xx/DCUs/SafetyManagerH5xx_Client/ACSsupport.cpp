#include "SM_RemoteClient.h"
#include "SMsPool.h"

KSMsPool g_Pool;

bool GetFirstACSObject()
{
  return g_Pool.GetFirstACSObject();
}

bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Data[256] )
{
  return g_Pool.GetNextACSObject( pData, Name, 256, Data, 256 );
}

#ifdef _WIN32
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  return g_Pool.ShowACSObject( pInfo, hMainWnd, Name, pData );
}
#endif

bool KSM_RemoteClient::GetFirstACSObject()
{
  m_nAcsFindStep = 0;
  return true;
}

bool KSM_RemoteClient::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  if( 0==m_nAcsFindStep )
  {
    m_nAcsFindStep = -1;
    strcpy_s( Name, sn, m_szRealName );
    strcpy_s( Data, sd, "SafetyManagerH5xx" );
    return true;
  }
  else if( -1==m_nAcsFindStep )
  {
    m_nAcsFindStep = -2;
    strcpy_s( Name, sn, m_ObjName.c_str());
    strcpy_s( Data, sd, "SafetyManagerH5xx" );
    return true;
  }
  return false;
}

#ifdef _WIN32
bool KSM_RemoteClient::ShowACSObject( HWND hMainWnd, char * Name, void * pData )
{
  if( !strcmp(m_szRealName, Name) )
  {
      m_proxy.ShowObject(Name);
    return true;
  }
  return false;
}
#endif
