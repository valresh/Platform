#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_M2UOn_P
{
#include "PrivateVars/M2U.h"
};


class K_M2UOn
  : public KFlexBlockBase
  , protected S_M2UOn_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_M2U *W;
public:
  K_M2UOn()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _M2U("M2UON", K_M2UOn::Create);

KFlexBlockBase *K_M2UOn::Create()
{
  return new K_M2UOn;
}

namespace ns_M2UOn
{
  #include <_InitInfo.h>
  DECLARE_PRIVATE_VARSINFO( K_M2UOn, 10 )
  DECLARE_SHARED_VARSINFO( S_M2U, 5 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_M2UOn;
void K_M2UOn::InitParm()
{
#include <FlexBlocks/M2U.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/M2U.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_M2UOn::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_M2UOn::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> PV;
  }
}

KFlexBlockBase::SReturn K_M2UOn::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( params.nStep < 3 )
    W->STATE = 0;
  else if( 3==params.nStep && PV )
  {
    W->STATE = PV;
    if( pFlexM2ULogging )
      pFlexM2ULogging( m_szVisibleName, W->SHORTTEXT, W->STATE ? 1 : 0, params.modelTime, params.pszPrjObjName, params.pszAuditDir );
  }
  else
  {
    W->STATE = PV;
    if( PV != PREVPV )
    {
      if( pFlexM2ULogging )
        pFlexM2ULogging( m_szVisibleName, W->SHORTTEXT, W->STATE ? 1 : 0, params.modelTime, params.pszPrjObjName, params.pszAuditDir );
    }
  }
  PREVPV = PV;
  return sr;
}