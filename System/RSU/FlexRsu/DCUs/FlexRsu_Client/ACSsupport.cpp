#include <crossplatform.h>
#include "Client.h"
#include "CBsPool.h"

KCBsPool g_Pool;

#ifdef _WIN32
bool GetFirstACSObject()
{
  return g_Pool.GetFirstACSObject();
}

bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256 * 4], char Data[256 * 4] )
{
  return g_Pool.GetNextACSObject( pData, Name, 256, Data, 256 );
}

bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  return g_Pool.ShowACSObject( pInfo, hMainWnd, Name, pData );
}

bool KClient::GetFirstACSObject()
{
  m_nAcsFindStep = 0;
  return true;
}

bool KClient::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  if( -1!=m_nAcsFindStep )
  {
    /*if( m_Objs.WhileBase( m_nAcsFindStep, -1, &pBase, &pszName, &nFcsNumb ) )
    {
      LPCSTR pszClass = NULL;
      m_Objs.GetNameEx( pBase->ID_PNT_NAME, &pszClass, NULL, NULL );
      strcpy_s( Name, sn, pszName );
      strcpy_s( Data, sd, pszClass );
    }
    else*/
    {
      m_nAcsFindStep = -1;
      strcpy_s( Name, sn, m_szObjName );
      strcpy_s( Data, sd, "FlexRsu" );
    }
    return true;
  }
  return false;
}

bool KClient::ShowACSObject( 
    HWND hMainWnd, 
    char* Name, void* pData)
{
  if( !strcmp(m_szObjName, Name) || !strcmp(ObjName(), Name))
  {
    //__super::ShowObject( m_szObjName );      
    return m_proxy.ShowObject(m_szObjName);
  }
  return false;
}
#endif
