#include "stdafx.h"
#include "H_RemoteClient.h"
#include "CBsPool.h"

KCBsPool g_Pool;

bool GetFirstACSObject()
{
  return g_Pool.GetFirstACSObject();
}

bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Data[256] )
{
  return g_Pool.GetNextACSObject( pData, Name, 256, Data, 256 );
}

bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData )
{
  return g_Pool.ShowACSObject( pInfo, hMainWnd, Name, pData );
}


bool KH_RemoteClient::GetFirstACSObject()
{
  m_nAcsFindStep = 0;
  return true;
}

bool KH_RemoteClient::GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd )
{
  /*CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;*/
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
      strcpy_s( Name, sn, (LPCSTR)ObjName );
      strcpy_s( Data, sd, "CBH" );
    }
    return true;
  }
  return false;
}

bool KH_RemoteClient::ShowACSObject( HWND hMainWnd, char * Name, void * pData )
{
  if( !strcmp(ObjName, Name) )
  {
    KRsuSharedClient::ShowObject( Name );
    return true;
  }
  CBase *pBase = NULL;
  LPCSTR pszName = NULL;
  int nFcsNumb = 0;
  DWORD ID = 0;
  while( m_Objs.WhileBase( ID, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    if( !_strcmpi(pszName, Name) )
    {
      KRsuSharedClient::ShowObject( Name );
      return true;
    }
  }
  ID = 0;
  while( m_IOs.WhileBase( ID, -1, &pBase, &pszName, &nFcsNumb ) )
  {
    if( !_strcmpi(pszName, Name) )
    {
      KRsuSharedClient::ShowObject( Name );
      return true;
    }
  }
  return false;
}
