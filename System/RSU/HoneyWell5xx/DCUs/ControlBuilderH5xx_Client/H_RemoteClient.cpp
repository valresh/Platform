#include "H_RemoteClient.h"
#include <ConnectH.h>
#include <SetDataTypes.h>
#include <Connect.h>
#include <RsuX.h>
#include "CBsPool.h"

#pragma comment( lib, "ConnectH5xx.lib")

bool GetFirstACSObject( );
bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Info[256] );
#ifdef _WIN32
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif
nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

extern KNoName *g_IOs, *g_Objs;
extern KCBsPool g_Pool;

static bool bRegisteredFind = false;

KH_RemoteClient::KH_RemoteClient( LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
: m_proxy(a_cl), m_ObjName(_ObjName)//m_ObjName(_ObjName)
, m_nAcsFindStep( 0 )
{
  //TypeObj = RSU_Obj;
}

LPCSTR KH_RemoteClient::GetName()
{
  return m_ObjName.c_str();
}

int KH_RemoteClient::Init( int )
{
    /*
#ifdef _DEBUG
  int r = KRsuSharedClient::InitializeServer( "ControlBuilderH5xx_Server", ObjName, true );
#else
  int r = KRsuSharedClient::InitializeServer( "ControlBuilderH5xx_Server", ObjName );
#endif*/
    int r = m_proxy.Init(0);
    if( r )
    return 1;

  g_Objs = m_Objs = KNoName::Create( "H5xx_Objs", false );
  g_IOs = m_IOs = KNoName::Create( "H5xx_IOs", false );

#ifdef _WIN32
  if( pRegisterFinderACSobj && !bRegisteredFind )
  {
    bRegisteredFind = true;
    pRegisterFinderACSobj( ::GetFirstACSObject, ::GetNextACSObject, ::ShowACSObject);
  }
#endif

  if (!pRegisterHWstructs)
  {
      dylib* lib = new dylib("HW5xxRegisterInRSU", true);
      //LoadLibrary("HW5xxRegisterInRSU.dll");
  }
  if( pRegisterHWstructs )
    pRegisterHWstructs();

  if( pRegisterRsuConnection )
    pRegisterRsuConnection( "РСУ_CBH", RsuConnectParamInfo, NULL );

  g_Pool.RegisterCB( this );

  return 0;
}

int KH_RemoteClient::Step0()
{
  return m_proxy.Step0();
}

int KH_RemoteClient::Step1()
{
  return m_proxy.Step1();
}

int KH_RemoteClient::StepT(double dt)
{
  int nStep = 0;
  double dtMs = dt * 3600. * 1000;
  return m_proxy.StepT(dt); //KRsuSharedClient::CalcStep( (int)dtMs );
}

int KH_RemoteClient::SetData( int TypeData, const char * pData )
{
    return m_proxy.SetDataCmd(TypeData, pData);
}
