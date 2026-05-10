#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_WP_P
{
#include "PrivateVars/WP.h"
};


class K_WP 
  : public KFlexBlockBase
  , protected S_WP_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void IniWParm();
};

static KBlockCreator _WP("WP", K_WP::Create);

KFlexBlockBase *K_WP::Create()
{
  return new K_WP;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_WP, 6 )
//////////////////////////////////////////////////////////////////////////
void K_WP::IniWParm()
{
#include "PrivateVars/WP.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_WP::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_WP_P>( this, &s_private );
}

void K_WP::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> in;
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    switch( pOut->m_szSource[0] )
    {
    case 'O':
      (*pOut) << out;
      break;
    case 'E':
      switch( pOut->m_szSource[1] )
      {
      case 'P':
        (*pOut) << pulsing;
        break;
      case 'W':
        (*pOut) << waiting;
        break;
      }
      break;
    }
  }
}

KFlexBlockBase::SReturn K_WP::StepCalc( SCallParams &params )
{
  SReturn sr;
  tDigital32 prevIn = in;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  if( !prevIn && in && 0==waiting && 0==pulsing )
  {
    waiting = 0;
    pulsing = 0;
    if( WAIT > 0 )
    {
      waiting = params.dtS / 100.f;
      out = 0;
    }
    else
    {
      pulsing = params.dtS / 100.f;
      out = 1;
    }
  }
  else
  {
    if( waiting>0 )
    {
      waiting += params.dtS;
      if( waiting >= WAIT )
      {
        pulsing = params.dtS / 100.f;
        waiting = 0;
        out = 1;
      }
    }
    else if( pulsing > 0 )
    {
      pulsing += params.dtS;
      if( pulsing >= PULSE )
      {
        pulsing = 0;
        out = 0;
      }
    }
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
