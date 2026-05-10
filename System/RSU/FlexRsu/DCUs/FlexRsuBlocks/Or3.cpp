#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_Or3_P
{
#include "PrivateVars/Or3.h"
};


class K_Or3 
  : public KFlexBlockBase
  , protected S_Or3_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  bool m_InConnected[3];
public:
  static KFlexBlockBase *Create();
  K_Or3()
  {
    FIXED_OUT = 2;
  }
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
  virtual void OnAfterRestoreState();
};

static KBlockCreator _Or3("Or3", K_Or3::Create);

KFlexBlockBase *K_Or3::Create()
{
  return new K_Or3;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_Or3, 5 )
//////////////////////////////////////////////////////////////////////////
void K_Or3::InitParm()
{
#include "PrivateVars/Or3.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_Or3::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_Or3_P>( this, &s_private );
}

void K_Or3::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[2] )
    {
    case '1':
      (*pIn) >> in1;
      m_InConnected[0] = true;
      break;
    case '2':
      (*pIn) >> in2;
      m_InConnected[1] = true;
      break;
    case '3':
      (*pIn) >> in3;
     m_InConnected[2] = true;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << out;
  }
}

void K_Or3::OnAfterRestoreState()
{
  tDigital32 *ins[] = { &in1, &in2, &in3 };
  for( int i=0; i<_countof(ins); ++i )
  {
    if( !m_InConnected[i] )
      *ins[i] = 0;
  }
}

KFlexBlockBase::SReturn K_Or3::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  switch( FIXED_OUT )
  {
  case 0:
  case 1:
    out = FIXED_OUT;
    for( DWORD i=0; i<m_nOuts; ++i )
    {
      KFlexConnection *pOut = m_ppOut[i];
      *pOut = out;
    }
    return sr;
  }

  tDigital32 ins[] = { in1, in2, in3 };
  tDigital32 o = 0;
  for( int i=0; i<_countof(ins); ++i )
  {
    if( !m_InConnected[i] )
      continue;
    if( ins[i] )
    {
      o = 1;
      break;
    }
  }
  out = o;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = out;
  }
  return sr;
}
