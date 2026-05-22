#include "stdafx.h"
#include "H_RemoteClient.h"
#include <ConnectH.h>
#include <SetDataTypes.h>
#include <Connect.h>
#include <RsuX.h>
#include "CBsPool.h"

#pragma comment( lib, "ConnectH5xx.lib")

bool GetFirstACSObject( );
bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Info[256] );
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

extern KNoName g_IOs, g_Objs;
extern KCBsPool g_Pool;

static bool bRegisteredFind = false;

KH_RemoteClient::KH_RemoteClient( LPSTR _ObjName ) 
:	IBaseModel ( _ObjName )
, m_nAcsFindStep( 0 )
{
  TypeObj = RSU_Obj;
}

LPCSTR KH_RemoteClient::GetName()
{
  return ObjName;
}

int KH_RemoteClient::Init( int )
{
#ifdef _DEBUG
  int r = KRsuSharedClient::InitializeServer( "ControlBuilderH5xx_Server", ObjName, true );
#else
  int r = KRsuSharedClient::InitializeServer( "ControlBuilderH5xx_Server", ObjName );
#endif
  if( r )
    return 1;

  g_Objs = m_Objs = KNoName::Create( "H5xx_Objs", false );
  g_IOs = m_IOs = KNoName::Create( "H5xx_IOs", false );

  if( pRegisterFinderACSobj && !bRegisteredFind )
  {
    bRegisteredFind = true;
    pRegisterFinderACSobj( ::GetFirstACSObject, ::GetNextACSObject, ::ShowACSObject);
  }

  if( !pRegisterHWstructs )
    LoadLibrary("HW5xxRegisterInRSU.dll");
  if( pRegisterHWstructs )
    pRegisterHWstructs();

  if( pRegisterRsuConnection )
    pRegisterRsuConnection( "РСУ_CBH", RsuConnectParamInfo, NULL );

  g_Pool.RegisterCB( this );

  return 0;
}

int KH_RemoteClient::Step0()
{
  return KRsuSharedClient::StepAfterInit();
}

int KH_RemoteClient::Step1()
{
  return KRsuSharedClient::StepAfterRestored();
}

int KH_RemoteClient::StepT(double dt)
{
  int nStep = 0;
  double dtMs = dt * 3600. * 1000;
  return KRsuSharedClient::CalcStep( (int)dtMs );
}

int KH_RemoteClient::SetData( int TypeData, void * pData )
{
  switch( TypeData )
  {
  case sd_SaveRSUParams:
    return KRsuSharedClient::ParamsSave( (LPCSTR)pData );
  case sd_RestoreRSUParams:
    return KRsuSharedClient::ParamsRestore( (LPCSTR)pData );
  case sd_SaveRSUState:
    return KRsuSharedClient::StateSave( (LPCSTR)pData );
  case sd_RestoreRSUState:
    return KRsuSharedClient::StateRestore( (LPCSTR)pData );
  default:
    return -1;
  }
  return 0;
}
