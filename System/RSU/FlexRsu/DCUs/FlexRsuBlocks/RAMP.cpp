#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <SharedFlexBlocks.h>
#include <FlexPrivateDeclare.h>

struct S_RAMP_P
{
#include "PrivateVars/RAMP.h"
};


class K_RAMP 
  : public KFlexBlockBase
  , protected S_RAMP_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;

  bool Restart;
  bool Is_Work;
  tAnalog32 dV_dt;

  S_RAMP *W;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void IniRAMParm();
};

static KBlockCreator _RAMP("RAMP", K_RAMP::Create);

KFlexBlockBase *K_RAMP::Create()
{
  return new K_RAMP;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_RAMP, 20 )
DECLARE_SHARED_VARSINFO( S_RAMP, 10 )
//////////////////////////////////////////////////////////////////////////
void K_RAMP::IniRAMParm()
{
#include <FlexBlocks/RAMP.h>
  qsort( s_sharedVarInfo, s_ksharedVarInfo, sizeof( SVarInfo ), CompVarInfo );
#include "PrivateVars/RAMP.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_RAMP::OnCreated()
{
  CreateSFlexSharedObj( m_szVisibleName, &W );

  if( !s_kprivateVarInfo )
    IniRAMParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  m_kSharedVarsInfo = s_ksharedVarInfo;
  m_pSharedVarsInfo = s_sharedVarInfo;
  m_pSharedObject = (BYTE*)W;

  SetInitialVals<S_RAMP_P>( this, &s_private );
}

void K_RAMP::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'I':
      (*pIn) >> W->in;
      break;
    case 'E':
      (*pIn) >> ENABLE;
      break;
    case 'P':
      (*pIn) >> PAUSE;
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'O':
      (*pOut) << W->out;
      break;
    case 'C':
      (*pOut) << W->COMPLETE;
      break;
    }
  }
}

KFlexBlockBase::SReturn K_RAMP::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if ( ENABLE && ENABLE_OLD == 0 )
    Restart = true;

  if ( !ENABLE )
  {
    W->COMPLETE = 0;
    PAUSE = 0;
    W->TIME_REMAIN = 0.;
    Is_Work = false;
    W->out = W->in;
  }

  if( PAUSE )
  {
    for( DWORD i=0; i<m_nOuts; ++i )
    {
      KFlexConnection *pOut = m_ppOut[i];
      pOut->DataReady2Send();
    }
    return sr;
  }
  if( Is_Work )
  {
    if ( !Restart && RAMP_TYPE != RAMP_TYPE_OLD )
      Restart = true;
    if ( !Restart && END_VALUE != END_VALUE_OLD )
      Restart = true;
    if ( RAMP_TYPE )
      {
      if ( !Restart && W->RAMP_RATE != RAMP_RATE_OLD )
        Restart = true;
      }
    else
      {
      if ( !Restart && W->RAMP_TIME != RAMP_TIME_OLD )
        Restart = true;
      }
  }
  if( Restart )
  {
    Restart = false;
// Расчет параметров движения
    if ( RAMP_TYPE )
    {
      if ( W->RAMP_RATE <= 0. )
        W->RAMP_RATE = 10.;
      dV_dt = W->RAMP_RATE;
      if ( END_VALUE < W->in )
        dV_dt = -dV_dt;
      W->TIME_REMAIN = ( END_VALUE - W->in ) / dV_dt;
    }
    else
      {
      if ( W->RAMP_TIME <= 0. )
        W->RAMP_TIME = 10.;
      W->TIME_REMAIN = W->RAMP_TIME;
      dV_dt = ( END_VALUE - W->in ) / W->RAMP_TIME;
      }
    Is_Work = true;
    W->COMPLETE = 0;
    W->out = W->in;
  }
  IN_OLD = W->in;
  END_VALUE_OLD = END_VALUE;
  ENABLE_OLD = ENABLE;
  RAMP_TYPE_OLD = RAMP_TYPE;
  RAMP_RATE_OLD = W->RAMP_RATE;
  RAMP_TIME_OLD = W->RAMP_TIME;
//
  if( Is_Work )
  {
    if ( W->TIME_REMAIN > 0. )
    {
      W->TIME_REMAIN -= params.dtS;
      if ( W->TIME_REMAIN <= 0. )
      {
        W->TIME_REMAIN = 0.;
        W->out = END_VALUE;
        W->COMPLETE = 1;
      }
      else
      {
        W->out += dV_dt * params.dtS;
        W->COMPLETE = 0;
       }
      }
    else
    {
      W->TIME_REMAIN = 0.;
      W->out = END_VALUE;
      W->COMPLETE = 1;
    }
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
