#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_Not_P
{
#include "PrivateVars/Not.h"
};


class K_Not 
  : public KFlexBlockBase
  , protected S_Not_P
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

static KBlockCreator _Not("Not", K_Not::Create);

KFlexBlockBase *K_Not::Create()
{
  return new K_Not;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_Not, 2 )
//////////////////////////////////////////////////////////////////////////
void K_Not::InitParm()
{
#include "PrivateVars/Not.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_Not::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_Not_P>( this, &s_private );
}

void K_Not::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
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
    (*pOut) << out;
  }
}

KFlexBlockBase::SReturn K_Not::StepCalc( SCallParams &params )
{
  SReturn sr;
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }
  out = !in;
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = out;
  }
  return sr;
}
