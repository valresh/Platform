#include <crossplatform.h>
#include "BlockCreator.h"
#include <SharedFlexBlocks.h>
#include <FlexRsuConnect.h>
#include <FlexPrivateDeclare.h>

struct S_TMR_P
{
#include "PrivateVars/TMR.h"
};


class K_TMR 
  : public KFlexBlockBase
  , protected S_TMR_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  S_TMR *W;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _TMR("TMR", K_TMR::Create);

KFlexBlockBase *K_TMR::Create()
{
  return new K_TMR;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_TMR, 4 )
DECLARE_SHARED_VARSINFO( S_TMR, 6 )
//////////////////////////////////////////////////////////////////////////
void K_TMR::InitParm()
{
#include <FlexBlocks/TMR.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/TMR.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TMR::OnCreated()
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

void K_TMR::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'I':
      (*pIn) >> PV;
      break;
    case 'T':
      (*pIn) >> W->TM_SP;
      break;
    case 'R':
      (*pIn) >> RESET;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'T':
      if( 'C'==pOut->m_szSource[3] )
        (*pOut) << W->TM_COMPLETE;
      else if( 'R'==pOut->m_szSource[3] )
        (*pOut) << W->TM_REMAIN;
      break;
    case 'O':
      (*pOut) << OUT_D;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_TMR::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevReset = RESET;
  tDigital32 prevPV = PV;
  
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( RESET )
  {
    W->TM_COMPLETE = 0;
    W->TM_REMAIN = W->TM_SP;
    W->TIMER = 0;
    OUT_D = 0;
    if( !prevReset )
    {
      for( DWORD i=0; i<m_nOuts; ++i )
      {
        KFlexConnection *pOut = m_ppOut[i];
        pOut->DataReady2Send();
      }
    }
    return sr;
  }
  if( !PV )
  {
    if( prevPV )
    {
      for( DWORD i=0; i<m_nOuts; ++i )
      {
        KFlexConnection *pOut = m_ppOut[i];
        pOut->DataReady2Send();
      }
    }
	}
  
  switch( W->MODE )
    {
    case 0:
    if( !PV )
    {
      if( prevPV )
      {
        for( DWORD i=0; i<m_nOuts; ++i )
        {
          KFlexConnection *pOut = m_ppOut[i];
          pOut->DataReady2Send();
        }
      }
    return sr;
  }

  if( !OUT_D )
  {
    W->TIMER += params.dtS;
    W->TM_COMPLETE = (tDigital32)W->TIMER;
  }

  if( W->TM_COMPLETE >= W->TM_SP )
    W->TM_COMPLETE = W->TM_SP;

  W->TM_REMAIN = W->TM_SP - W->TM_COMPLETE;
  if( W->TM_REMAIN < 0 )
    W->TM_REMAIN = 0;

  if( !W->TM_REMAIN )
    OUT_D = 1;
  break;
  case 1:
  if (!prevPV && PV)
  {
    ACT = 1;
  }

  if( ACT == 1 )
  {
	OUT_D = 1;
    W->TIMER += params.dtS;
    if( W->TIMER >= W->TM_SP )
    {
      OUT_D = 0;
      W->TIMER = 0;
      ACT = 0;
    }
  }
	break;
	}

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
