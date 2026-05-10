#include "Y_RemoteClient.h"
#include <ConnectY.h>
#include <time.h>
#include <SysDataTypes.h>
#include <Connect.h>
#include <RsuX.h>

bool GetFirstACSObject( );
bool GetNextACSObject( DWORD & Type, void ** pData, char Name[256], char Info[256] );
#ifdef _WIN32
bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif

nRSUx::SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

extern KNoName2D *g_YokoIOs;
extern CCentumSystem* g_pCentumSystem;
KY_RemoteClient * g_pYClient = NULL;

KY_RemoteClient::KY_RemoteClient( LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
: m_proxy(a_cl), m_ObjName(_ObjName)
, m_nAcsFindStep( 0 )
{
  //TypeObj = RSU_Obj;
}

int KY_RemoteClient::Init( int )
{
    int r = m_proxy.Init(0);
    if (r)
        return 1;

  g_YokoIOs = m_yokoIOs = KNoName2D::Create( "Y_IOs", false );
  m_yokoObjs = KNoName2D::Create( "Y_Objs", false );
  g_pYClient = this;

#ifdef _WIN32
  if (pRegisterFinderACSobj)
  {
      pRegisterFinderACSobj(::GetFirstACSObject, ::GetNextACSObject, ::ShowACSObject);
  }
#endif

  if( !pRegisterYWstructs )
  {
      dylib* lib = new dylib("YokoRegisterInRSU", true);
  }
  if( pRegisterYWstructs )
  {
      pRegisterYWstructs();
  }

  CBase* pBase = m_yokoIOs->CreateStruct( "Весь Centum", CCentumSystem::TypeID, 0, 0 );
  if( pBase )
  {
    ASS( pBase->size == sizeof(CCentumSystem) );
    g_pCentumSystem = m_pCentumSystem = (CCentumSystem*)pBase;
  }

  if( pRegisterRsuConnection )
  {
      pRegisterRsuConnection("РСУ_Y", RsuConnectParamInfo, NULL);
  }

  return 0;
}

int KY_RemoteClient::Step0()
{
  return m_proxy.Step0();
}

int KY_RemoteClient::Step1()
{
  return m_proxy.Step1();
}

int KY_RemoteClient::StepT(double dt)
{
  if( m_pCentumSystem )
  {
    m_pCentumSystem->dtAsIs = dt;
    if( pSys )
    {
      m_pCentumSystem->modelTime = pSys->TimeM() ;
      m_pCentumSystem->timeB = pSys->TimeB() ;
      m_pCentumSystem->nStep = pSys->m_nStep;
    }
    else
    {
      m_pCentumSystem->modelTime = time(NULL);
    }
  }
  double dtMs = dt * 3600. * 1000;
  return m_proxy.StepT(dt);
}

int KY_RemoteClient::SetData( int TypeData, const char * pData )
{
    return m_proxy.SetDataCmd(TypeData, pData);
}
