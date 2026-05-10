#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_Or5_P
{
#include "PrivateVars/Or5.h"
};


class K_Or5 
  : public KFlexBlockBase
  , protected S_Or5_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  bool m_InConnected[5];
public:
  static KFlexBlockBase *Create();
  K_Or5()
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

static KBlockCreator _Or5("Or5", K_Or5::Create);

KFlexBlockBase *K_Or5::Create()
{
  return new K_Or5;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_Or5, 7 )
//////////////////////////////////////////////////////////////////////////
void K_Or5::InitParm()
{
#include "PrivateVars/Or5.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_Or5::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_Or5_P>( this, &s_private );
}

void K_Or5::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
    case '4':
      (*pIn) >> in4;
      m_InConnected[3] = true;
      break;
    case '5':
      (*pIn) >> in5;
      m_InConnected[4] = true;
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

void K_Or5::OnAfterRestoreState()
{
  tDigital32 *ins[] = { &in1, &in2, &in3, &in4, &in5 };
  for( int i=0; i<_countof(ins); ++i )
  {
    if( !m_InConnected[i] )
      *ins[i] = 0;
  }
}

KFlexBlockBase::SReturn K_Or5::StepCalc( SCallParams &params )
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

  tDigital32 ins[] = { in1, in2, in3, in4, in5 };
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
