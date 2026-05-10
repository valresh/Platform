#include "QB_RemoteClient.h"
#include <ConnectQB.h>
#include <SetDataTypes.h>
#include <Connect.h>
#include <RsuX.h>

#pragma comment( lib, "ConnectQB5xx.lib")

bool GetFirstACSObject( );
bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Info[256] );
#ifdef _WIN32
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

extern KNoName *g_IOs;
extern KQB_RemoteClient* g_Client;

KQB_RemoteClient::KQB_RemoteClient( LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
:m_proxy(a_cl), m_ObjName(_ObjName)//	IBaseModel ( _ObjName )
, m_nAcsFindStep( 0 )
{
  //TypeObj = RSU_Obj;
  g_Client = this;
}

LPCSTR KQB_RemoteClient::GetName()
{
  return m_ObjName.c_str();
}

int KQB_RemoteClient::Init( int )
{
  //int r = KRsuSharedClient::InitializeServer( "QuickBuilderH5xx_Server", ObjName );
  int r = m_proxy.Init(0);
  if( r )
    return 1;

  g_IOs = m_IOs = KNoName::Create( "QB5xx_IOs", false, 8000, 12000, 5*1024*1024 );
#ifdef _WIN32
  if( pRegisterFinderACSobj )
    pRegisterFinderACSobj( ::GetFirstACSObject, ::GetNextACSObject, ::ShowACSObject);
#endif
  if( !pRegisterQBstructs )
    dylib* lib = new dylib("QB5xxRegisterInRSU", true);
  if( pRegisterQBstructs )
    pRegisterQBstructs();

  if( pRegisterRsuConnection )
    pRegisterRsuConnection( "РСУ_QB_", RsuConnectParamInfo, NULL );

  return 0;
}

int KQB_RemoteClient::Step0()
{
  return m_proxy.Step0();
}

int KQB_RemoteClient::Step1()
{
  return m_proxy.Step1();
}
int KQB_RemoteClient::StepT(double dt)
{
  int nStep = 0;
  double dtMs = dt * 3600. * 1000;
  return m_proxy.StepT(dt); //KRsuSharedClient::CalcStep( (int)dtMs );
}

int KQB_RemoteClient::SetData( int TypeData, const char * pData )
{
    return m_proxy.SetDataCmd(TypeData, pData);
}
