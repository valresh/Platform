#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_DConst_P
{
#include "PrivateVars/DConst.h"
};


class K_DConst 
  : public KFlexBlockBase
  , protected S_DConst_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_DConst()
  {
    m_nullOrderExec = exnInputsIO;
  }
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void InitParm();
};

static KBlockCreator _DConst("DConst", K_DConst::Create);

KFlexBlockBase *K_DConst::Create()
{
  return new K_DConst;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_DConst, 6 )
//////////////////////////////////////////////////////////////////////////
void K_DConst::InitParm()
{
#include "PrivateVars/DConst.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_DConst::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

}

void K_DConst::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << VALUE;
  }
}

KFlexBlockBase::SReturn K_DConst::StepCalc( SCallParams &params )
{
  SReturn sr;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = VALUE;
  }
  return sr;
}
