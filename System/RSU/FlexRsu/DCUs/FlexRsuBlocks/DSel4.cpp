#include "BlockCreator.h"
#include "ScriptableBlock.h"
#include <FlexPrivateDeclare.h>

struct S_DSel4_P
{
#include "PrivateVars/DSel4.h"
};


class K_DSel4 
  : public KScriptableBlock
  , protected S_DSel4_P
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

static KBlockCreator _DSel4("DSel4", K_DSel4::Create);

KFlexBlockBase *K_DSel4::Create()
{
  return new K_DSel4;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_DSel4, 12 )
//////////////////////////////////////////////////////////////////////////
void K_DSel4::InitParm()
{
#include "PrivateVars/DSel4.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DSel4::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_DSel4_P>( this, &s_private );
}

void K_DSel4::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    switch( pIn->m_szSink[0] )
    {
    case 'A':
      (*pIn) >> A;
      break;
    case 'B':
      (*pIn) >> B;
      break;
    case 'C':
      (*pIn) >> C;
      break;
    case 'D':
      (*pIn) >> D;
      break;
    case 'S':
      (*pIn) >> SEL;
      break;
    default:
      ASSD(!"такого входа нет");
      break;
    }
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << O;
  }
}

KFlexBlockBase::SReturn K_DSel4::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  switch( SEL )
  {
  case 0:
    O = A;
    break;
  case 1:
    O = B;
    break;
  case 2:
    O = C;
    break;
  case 3:
    O = D;
    break;
  default:
    return sr;
  }

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }

  return sr;
}
