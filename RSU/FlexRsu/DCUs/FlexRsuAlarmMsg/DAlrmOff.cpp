#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_DAlrmOff_P
{
#include "PrivateVars/DigAlrm.h"
};


class K_DAlrmOff
  : public KFlexBlockBase
  , protected S_DAlrmOff_P
{
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_DIGALRM *W;
  SFlex *pRaW;
public:
  K_DAlrmOff()
  {
    m_nullOrderExec = exnOutputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  SReturn StepCalc( SCallParams &params );
  virtual void OnAfterRestoreState();
  static void InitParm();
};

static KBlockCreator _DigAlrm("DALRMOFF", K_DAlrmOff::Create);

KFlexBlockBase *K_DAlrmOff::Create()
{
  return new K_DAlrmOff;
}

namespace ns_DAlrmOff
{
  #include <_InitInfo.h>
  DECLARE_PRIVATE_VARSINFO( K_DAlrmOff, 10 )
  DECLARE_SHARED_VARSINFO( S_DIGALRM, 26 )
}
//////////////////////////////////////////////////////////////////////////
using namespace ns_DAlrmOff;
void K_DAlrmOff::InitParm()
{
#include <FlexBlocks/DigAlrm.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/DigAlrm.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DAlrmOff::OnCreated()
{
  pRaW = CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;
}

void K_DAlrmOff::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    if( !strcmp(pIn->m_szSink, "IN") )
      (*pIn) >> PV;
  }
}

void K_DAlrmOff::OnAfterRestoreState()
{
  W->STATE = 0;
  W->SetAck = 0;
  W->Acked = 1;
  PV = PREVPV = 1;
}

static LPCSTR s_pszOFF = "OFF";
KFlexBlockBase::SReturn K_DAlrmOff::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  if( PV != PREVPV )
  {
    W->STATE = PV ? 0 : 1;
    if( pFlexAlarm )
    {
      char szAtValue[32] = {};
      snprintf( szAtValue, _countof(szAtValue)-1, "%d", PV );
      if( W->STATE )
      {
        W->SetAck = 0;
        W->Acked = 0;
        pFlexAlarm( pRaW, true, s_pszOFF, params.modelTime, params.pszDrName, W->DESCR, "", "", "", W->Acked, "", "", "" );
      }
      else
        pFlexAlarm( pRaW, false, s_pszOFF, params.modelTime, params.pszDrName, W->DESCR, "", "", "", W->Acked, "", "", "" );
    }
  }
  if( W->SetAck )
  {
    if( !W->Acked && pFlexAckingAlarm )
      pFlexAckingAlarm( pRaW, params.modelTime, W->STATE ? true : false );
    W->Acked = 1;
    W->SetAck = 0;
  }
  PREVPV = PV;
  return sr;
}
