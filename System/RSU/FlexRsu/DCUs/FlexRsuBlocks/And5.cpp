#include <crossplatform.h>
#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_And5_P
{
#include "PrivateVars/And5.h"
};


class K_And5 
  : public KFlexBlockBase
  , protected S_And5_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
  bool m_InConnected[5];
public:
  static KFlexBlockBase *Create();
  K_And5()
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

static KBlockCreator _And5("And5", K_And5::Create);

KFlexBlockBase *K_And5::Create()
{
  return new K_And5;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_And5, 7 )
//////////////////////////////////////////////////////////////////////////
void K_And5::InitParm()
{
#include "PrivateVars/And5.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_And5::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_And5_P>( this, &s_private );
}

void K_And5::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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

void K_And5::OnAfterRestoreState()
{
  tDigital32 *ins[] = { &in1, &in2, &in3, &in4, &in5 };
  for( int i=0; i<_countof(ins); ++i )
  {
    if( !m_InConnected[i] )
      *ins[i] = 0;
  }
}

KFlexBlockBase::SReturn K_And5::StepCalc( SCallParams &params )
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
  tDigital32 o = 1;
  int nC = 0;
  for( int i=0; i<_countof(ins); ++i )
  {
    if( !m_InConnected[i] )
      continue;
    ++nC;
    if( !ins[i] )
    {
      o = 0;
      break;
    }
  }
  if( !nC )
    o = 0;
  out = o;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = out;
  }
  return sr;
}
