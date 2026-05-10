#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>

struct S_AConst_P
{
#include "PrivateVars/AConst.h"
};


class K_AConst 
  : public KFlexBlockBase
  , protected S_AConst_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
public:
  K_AConst()
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

static KBlockCreator _AConst("AConst", K_AConst::Create);

KFlexBlockBase *K_AConst::Create()
{
  return new K_AConst;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_AConst, 6 )
//////////////////////////////////////////////////////////////////////////
void K_AConst::InitParm()
{
#include "PrivateVars/AConst.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_AConst::OnCreated()
{
  if( !s_kprivateVarInfo )
    InitParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

}

void K_AConst::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << VALUE;
  }
}

KFlexBlockBase::SReturn K_AConst::StepCalc( SCallParams &params )
{
  SReturn sr;

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    *pOut = VALUE;
  }
  return sr;
}
