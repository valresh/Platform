#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_TP_P
{
#include "PrivateVars/TP.h"
};


class K_TP 
  : public KFlexBlockBase
  , protected S_TP_P
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
  static void InitParm();
};

static KBlockCreator _TP("TP", K_TP::Create);

KFlexBlockBase *K_TP::Create()
{
  return new K_TP;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_TP, 3 )
//////////////////////////////////////////////////////////////////////////
void K_TP::InitParm()
{
#include "PrivateVars/TP.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_TP::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_TP_P>( this, &s_private );
}

void K_TP::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << PULSE;
  }
}

KFlexBlockBase::SReturn K_TP::StepCalc( SCallParams &params )
{
  SReturn sr;
  waiting += params.dtS;
  if( waiting>=PERIOD )
  {
    PULSE = 1;
    waiting -= PERIOD;
  }
  else
    PULSE = 0;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = PULSE;
  }
  return sr;
}
